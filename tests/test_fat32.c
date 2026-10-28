#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../kernel/subsystems/fat32.h"
#include "../kernel/subsystems/vfs.h"
static void put16(uint8_t*p,uint16_t x){p[0]=(uint8_t)x;p[1]=(uint8_t)(x>>8);}
static void put32(uint8_t*p,uint32_t x){p[0]=(uint8_t)x;p[1]=(uint8_t)(x>>8);p[2]=(uint8_t)(x>>16);p[3]=(uint8_t)(x>>24);}
int main(void){
    uint8_t *img=(uint8_t*)calloc(100,512);assert(img);
    uint8_t*b=img;b[0]=0xeb;put16(b+11,512);b[13]=1;put16(b+14,1);b[16]=1;put32(b+32,100);put32(b+36,1);put32(b+44,2);b[510]=0x55;b[511]=0xaa;
    uint8_t*fat=img+512;put32(fat+2*4,0x0fffffff);put32(fat+3*4,0x0fffffff);
    uint8_t*root=img+2*512;memcpy(root,"HELLO   TXT",11);root[11]=0x20;put16(root+26,3);put32(root+28,5);root[32]=0;
    memcpy(img+3*512,"GenOS",5);
    fat32_volume_t v;assert(fat32_mount(&v,img,100*512)==0);
    fat32_entry_t e;assert(fat32_find_root(&v,"hello.txt",&e)==0);assert(e.size==5);
    char out[8]={0};size_t n=0;assert(fat32_read_file(&v,&e,out,sizeof(out),&n)==0);assert(n==5&&!memcmp(out,"GenOS",5));
    assert(fat32_read_file(&v,&e,out,4,&n)<0);assert(fat32_find_root(&v,"missing.txt",&e)<0);
    vfs_t fs;assert(vfs_init(&fs)==0);char imported[16]={0};
    assert(vfs_import_fat32_root_file(&fs,&v,"HELLO.TXT","/hello.txt",imported,sizeof(imported))==0);
    vfs_handle_t handle;assert(vfs_open(&fs,"/hello.txt",&handle)==0);memset(out,0,sizeof(out));
    assert(vfs_read(&handle,out,sizeof(out))==5&&!memcmp(out,"GenOS",5));
    assert(fat32_mount(&v,img,511)<0);free(img);return 0;
}
