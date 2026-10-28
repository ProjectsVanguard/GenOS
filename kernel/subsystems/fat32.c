#include "fat32.h"
#define FAT32_EOC 0x0ffffff8u
#define FAT32_BAD 0x0ffffff7u
static uint16_t le16(const uint8_t *p){return (uint16_t)p[0]|((uint16_t)p[1]<<8);}
static uint32_t le32(const uint8_t *p){return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static int span(size_t total,size_t off,size_t n){return off<=total&&n<=total-off;}
static int sector(const fat32_volume_t *v,uint32_t n,const uint8_t **p){
    if(!v||!p||n>=v->total_sectors||!span(v->image_size,(size_t)n*FAT32_SECTOR_SIZE,FAT32_SECTOR_SIZE))return -1;
    *p=v->image+(size_t)n*FAT32_SECTOR_SIZE;return 0;
}
static int cluster_sector(const fat32_volume_t*v,uint32_t c,uint32_t *s){
    if(!v||!s||c<2||c>=v->cluster_count+2)return -1;
    uint64_t n=(uint64_t)v->data_start+(uint64_t)(c-2)*v->sectors_per_cluster;
    if(n>=v->total_sectors||n+v->sectors_per_cluster>v->total_sectors)return -1;
    *s=(uint32_t)n;return 0;
}
static int next_cluster(const fat32_volume_t*v,uint32_t c,uint32_t*n){
    uint64_t off=(uint64_t)v->fat_start*FAT32_SECTOR_SIZE+(uint64_t)c*4;
    if(off+4>(uint64_t)(v->fat_start+v->fat_sectors)*FAT32_SECTOR_SIZE||!span(v->image_size,(size_t)off,4))return -1;
    uint32_t x=le32(v->image+(size_t)off)&0x0fffffffu;
    if(x>=FAT32_EOC){*n=0;return 0;}
    if(x==FAT32_BAD||x<2||x>=v->cluster_count+2)return -1;
    *n=x;return 0;
}
int fat32_mount(fat32_volume_t*v,const void*image,size_t size){
    if(!v||!image||size<512)return -1;
    const uint8_t*b=(const uint8_t*)image;
    if(b[510]!=0x55||b[511]!=0xaa||le16(b+11)!=512)return -1;
    uint8_t spc=b[13],fats=b[16];uint16_t reserved=le16(b+14);
    uint32_t total=le16(b+19);if(!total)total=le32(b+32);
    uint32_t fatsz=le32(b+36),root=le32(b+44);
    if(!spc||(spc&(spc-1))||spc>128||!fats||fats>2||!reserved||!fatsz||!total||root<2)return -1;
    uint64_t fat_start=reserved, data=(uint64_t)reserved+(uint64_t)fats*fatsz;
    if(data>=total||(uint64_t)total*512>size||fat_start+fatsz>total)return -1;
    uint32_t clusters=(uint32_t)((total-data)/spc);
    if(root>=clusters+2)return -1;
    v->image=b;v->image_size=size;v->total_sectors=total;v->fat_start=(uint32_t)fat_start;
    v->fat_sectors=fatsz;v->data_start=(uint32_t)data;v->root_cluster=root;
    v->cluster_count=clusters;v->sectors_per_cluster=spc;v->fat_count=fats;
    return 0;
}
static int name83(const uint8_t*raw,char*out){
    size_t n=0;int dot=0;
    for(size_t i=0;i<8&&raw[i]!=' ';i++){uint8_t c=raw[i];if(c<33||c>126||c=='/'||c=='\\')return -1;out[n++]=(char)c;}
    if(!n)return -1;
    size_t ext=0;for(size_t i=8;i<11&&raw[i]!=' ';i++){uint8_t c=raw[i];if(c<33||c>126||c=='/'||c=='\\')return -1;if(!dot){out[n++]='.';dot=1;}out[n++]=(char)c;ext++;}
    (void)ext;out[n]=0;return 0;
}
static int equal_fold(const char*a,const char*b){while(*a&&*b){char x=*a++,y=*b++;if(x>='a'&&x<='z')x=(char)(x-'a'+'A');if(y>='a'&&y<='z')y=(char)(y-'a'+'A');if(x!=y)return 0;}return *a==*b;}
int fat32_find_root(const fat32_volume_t*v,const char*want,fat32_entry_t*out){
    if(!v||!want||!out)return -1;uint32_t c=v->root_cluster;
    for(uint32_t steps=0;steps<v->cluster_count;steps++){
        uint32_t first;if(cluster_sector(v,c,&first))return -1;
        for(uint32_t si=0;si<v->sectors_per_cluster;si++){const uint8_t*p;if(sector(v,first+si,&p))return -1;
            for(size_t off=0;off<512;off+=32){const uint8_t*e=p+off;if(e[0]==0)return -1;if(e[0]==0xe5||e[11]==0x0f||(e[11]&0x08))continue;
                char name[FAT32_NAME_MAX];if(name83(e,name)==0&&equal_fold(name,want)){out->name[0]=0;size_t j=0;while(name[j]&&j+1<sizeof(out->name)){out->name[j]=name[j];j++;}out->name[j]=0;out->attributes=e[11];out->first_cluster=((uint32_t)le16(e+20)<<16)|le16(e+26);out->size=le32(e+28);return 0;}
            }
        }
        uint32_t next;if(next_cluster(v,c,&next)||!next)return -1;c=next;
    }return -1;
}
int fat32_read_file(const fat32_volume_t*v,const fat32_entry_t*e,void*dst,size_t cap,size_t*read_out){
    if(read_out)*read_out=0;if(!v||!e||!read_out||(!dst&&e->size)||(e->attributes&0x10)||e->size>cap)return -1;
    if(!e->size)return 0;uint8_t*d=(uint8_t*)dst;size_t copied=0;uint32_t c=e->first_cluster;
    for(uint32_t steps=0;steps<v->cluster_count&&copied<e->size;steps++){
        uint32_t first;if(cluster_sector(v,c,&first))return -1;
        for(uint32_t si=0;si<v->sectors_per_cluster&&copied<e->size;si++){const uint8_t*p;if(sector(v,first+si,&p))return -1;size_t n=e->size-copied;if(n>512)n=512;for(size_t i=0;i<n;i++)d[copied+i]=p[i];copied+=n;}
        if(copied==e->size)break;uint32_t next;if(next_cluster(v,c,&next)||!next)return -1;c=next;
    }
    if(copied!=e->size)return -1;*read_out=copied;return 0;
}
