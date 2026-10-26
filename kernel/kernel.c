#include <stdint.h>
#include <stddef.h>
#include "limine.h"
#include "native_apps.h"

LIMINE_BASE_REVISION(0);
LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests"))) static volatile struct limine_framebuffer_request fb_req = { LIMINE_FRAMEBUFFER_REQUEST_ID, LIMINE_FRAMEBUFFER_REQUEST_REVISION, 0 };
LIMINE_REQUESTS_END_MARKER;

static struct limine_framebuffer *fb;
static volatile uint32_t *pixels;
static uint32_t width, height, stride;
static uint32_t color(uint8_t r,uint8_t g,uint8_t b){ return ((uint32_t)r<<16)|((uint32_t)g<<8)|b; }
static void putpixel(int x,int y,uint32_t c){ if(fb && x>=0 && y>=0 && (uint32_t)x<width && (uint32_t)y<height) pixels[(uint64_t)y*stride+x]=c; }
static void rect(int x,int y,int w,int h,uint32_t c){ for(int yy=y;yy<y+h;yy++) for(int xx=x;xx<x+w;xx++) putpixel(xx,yy,c); }
/* Tiny built-in 5x7 font: letters needed by the boot banner and menu. */
static const uint8_t font[][5] = {
 {0x7e,0x11,0x11,0x11,0x7e},{0x7f,0x49,0x49,0x49,0x36},{0x3e,0x41,0x41,0x41,0x22},{0x7f,0x41,0x41,0x22,0x1c},{0x7f,0x49,0x49,0x49,0x41},{0x7f,0x09,0x09,0x09,0x01},{0x3e,0x41,0x49,0x49,0x7a},{0x7f,0x08,0x08,0x08,0x7f},{0x00,0x41,0x7f,0x41,0x00},{0x20,0x40,0x41,0x3f,0x01},{0x7f,0x08,0x14,0x22,0x41},{0x7f,0x40,0x40,0x40,0x40},{0x7f,0x02,0x0c,0x02,0x7f},{0x7f,0x04,0x08,0x10,0x7f},{0x3e,0x41,0x41,0x41,0x3e},{0x7f,0x09,0x09,0x09,0x06},{0x3e,0x41,0x51,0x21,0x5e},{0x7f,0x09,0x19,0x29,0x46},{0x46,0x49,0x49,0x49,0x31},{0x01,0x01,0x7f,0x01,0x01},{0x3f,0x40,0x40,0x40,0x3f},{0x1f,0x20,0x40,0x20,0x1f},{0x3f,0x40,0x38,0x40,0x3f},{0x63,0x14,0x08,0x14,0x63},{0x07,0x08,0x70,0x08,0x07},{0x61,0x51,0x49,0x45,0x43},
 {0,0,0,0,0},{0x00,0x00,0x5f,0x00,0x00},{0x00,0x07,0x00,0x07,0x00},{0x14,0x7f,0x14,0x7f,0x14},{0x24,0x2a,0x7f,0x2a,0x12},{0x23,0x13,0x08,0x64,0x62},{0x36,0x49,0x55,0x22,0x50},{0x00,0x05,0x03,0x00,0x00},{0x00,0x1c,0x22,0x41,0x00},{0x00,0x41,0x22,0x1c,0x00},{0x14,0x08,0x3e,0x08,0x14},{0x08,0x08,0x3e,0x08,0x08},{0x00,0x50,0x30,0x00,0x00},{0x08,0x08,0x08,0x08,0x08},{0x00,0x60,0x60,0x00,0x00},{0x20,0x10,0x08,0x04,0x02},
 {0x3e,0x51,0x49,0x45,0x3e},{0x00,0x42,0x7f,0x40,0x00},{0x42,0x61,0x51,0x49,0x46},{0x21,0x41,0x45,0x4b,0x31},{0x18,0x14,0x12,0x7f,0x10},{0x27,0x45,0x45,0x45,0x39},{0x3c,0x4a,0x49,0x49,0x30},{0x01,0x71,0x09,0x05,0x03},{0x36,0x49,0x49,0x49,0x36},{0x06,0x49,0x49,0x29,0x1e}
};
static void text(int x,int y,const char*s,uint32_t c,int scale){ for(;*s;s++,x+=6*scale)for(int col=0;col<5;col++)for(int row=0;row<7;row++){char ch=*s;int idx=(ch>='A'&&ch<='Z')?ch-'A':(ch>='0'&&ch<='9')?42+ch-'0':ch==' '?26:ch=='.'?40:ch==':'?29:ch=='-'?38:ch=='!'?27:ch=='/'?41:26; if(idx>=0&&idx<(int)(sizeof(font)/sizeof(font[0]))&&(font[idx][col]&(1<<row)))rect(x+col*scale,y+row*scale,scale,scale,c);}}
static void desktop(void){ if(!fb)return; rect(0,0,width,height,color(14,22,42)); for(uint32_t y=0;y<height*2/3;y++){uint8_t b=60+(uint8_t)(y*50/(height?height:1));rect(0,y,width,1,color(16,30,b));} rect(0,height-42,width,42,color(18,24,39)); rect(16,height-34,120,26,color(47,91,166)); text(28,height-26,"GENOS MENU",color(255,255,255),2); rect(width/2-190,height/2-100,380,200,color(30,41,63)); rect(width/2-190,height/2-100,380,3,color(83,160,255)); text(width/2-160,height/2-75,"GENOS DEVELOPER PREVIEW",color(255,255,255),2); text(width/2-160,height/2-25,"GENKERNEL BOOTED",color(115,220,170),2); text(width/2-160,height/2+15,"GENUI FRAMEBUFFER SHELL",color(220,230,245),2); text(width/2-160,height/2+55,"GEM APPS: USE SDK ON HOST",color(220,230,245),1); }

static uint32_t ui_rgb(uint8_t r,uint8_t g,uint8_t b){return color(r,g,b);}
static void ui_rect(int x,int y,int w,int h,uint32_t c){rect(x,y,w,h,c);}
static void ui_text(int x,int y,const char*s,uint32_t c,int scale){text(x,y,s,c,scale);}
static unsigned keyboard_key(uint8_t sc){
    if(sc==0x48)return 256; if(sc==0x50)return 257; if(sc==0x1c)return 258;
    if(sc==0x01)return 259; if(sc==0x0e)return 260;
    if(sc>=0x02&&sc<=0x0b){static const char d[]="1234567890";return (unsigned)d[sc-0x02];}
    if(sc==0x0c)return '-'; if(sc==0x0d)return '=';
    if(sc==0x10)return 'q'; if(sc==0x11)return 'w'; if(sc==0x12)return 'e';
    if(sc==0x13)return 'r'; if(sc==0x14)return 't'; if(sc==0x15)return 'y';
    if(sc==0x16)return 'u'; if(sc==0x17)return 'i'; if(sc==0x18)return 'o'; if(sc==0x19)return 'p';
    if(sc==0x1e)return 'a'; if(sc==0x1f)return 's'; if(sc==0x20)return 'd'; if(sc==0x21)return 'f';
    if(sc==0x22)return 'g'; if(sc==0x23)return 'h'; if(sc==0x24)return 'j'; if(sc==0x25)return 'k'; if(sc==0x26)return 'l';
    if(sc==0x2c)return 'z'; if(sc==0x2d)return 'x'; if(sc==0x2e)return 'c'; if(sc==0x2f)return 'v';
    if(sc==0x30)return 'b'; if(sc==0x31)return 'n'; if(sc==0x32)return 'm';
    if(sc==0x33)return ','; if(sc==0x34)return '.'; if(sc==0x35)return '/'; if(sc==0x39)return ' ';
    return 0;
}
static unsigned keyboard_poll(void){
    uint8_t status; __asm__ volatile("inb %1,%0":"=a"(status):"Nd"((uint16_t)0x64));
    if(!(status&1))return 0;
    uint8_t sc; __asm__ volatile("inb %1,%0":"=a"(sc):"Nd"((uint16_t)0x60));
    if(sc&0x80)return 0;
    return keyboard_key(sc);
}
static void native_desktop_loop(void){
    genos_native_ui ui={width,height,ui_rgb,ui_rect,ui_text};
    unsigned selected=0,screen=0; char note[561]={0},calc[24]={0},result[32]="READY";
    unsigned note_len=0,calc_len=0;
    for(;;){
        genos_native_apps_render(&ui,selected,screen,note,note_len,calc,result);
        unsigned key=0;
        while(!key)key=keyboard_poll();
        unsigned state=genos_native_apps_key(key,selected,screen,note,&note_len,calc,&calc_len,result);
        selected=state&255u; screen=(state>>8)&255u;
        if(screen==0){calc_len=0;calc[0]=0;}
    }
}

static void serial_init(void){ __asm__ volatile("outb %0,%1"::"a"((uint8_t)0x00),"Nd"((uint16_t)0x3F9)); __asm__ volatile("outb %0,%1"::"a"((uint8_t)0x80),"Nd"((uint16_t)0x3FB)); __asm__ volatile("outb %0,%1"::"a"((uint8_t)0x03),"Nd"((uint16_t)0x3F8)); __asm__ volatile("outb %0,%1"::"a"((uint8_t)0x00),"Nd"((uint16_t)0x3F9)); __asm__ volatile("outb %0,%1"::"a"((uint8_t)0x03),"Nd"((uint16_t)0x3FB)); __asm__ volatile("outb %0,%1"::"a"((uint8_t)0xC7),"Nd"((uint16_t)0x3FA)); __asm__ volatile("outb %0,%1"::"a"((uint8_t)0x0B),"Nd"((uint16_t)0x3FC)); }
static void serial_text(const char *s){ for(;*s;s++){ for(unsigned int i=0;i<1000000;i++){ uint8_t v; __asm__ volatile("inb %1,%0":"=a"(v):"Nd"((uint16_t)0x3FD)); if(v&0x20)break; } __asm__ volatile("outb %0,%1"::"a"((uint8_t)*s),"Nd"((uint16_t)0x3F8)); } }
static void halt(void){ for(;;)__asm__ volatile("hlt"); }
void genkernel_main(void){ serial_init(); serial_text("GENKERNEL ENTRY\\r\\n"); if(!LIMINE_BASE_REVISION_SUPPORTED){serial_text("LIMINE BASE REVISION UNSUPPORTED\\r\\n");halt();} if(!fb_req.response||!fb_req.response->framebuffer_count){serial_text("FRAMEBUFFER RESPONSE MISSING\\r\\n");halt();} fb=fb_req.response->framebuffers[0]; if(!fb||!fb->address||fb->bpp!=32||fb->width==0||fb->height==0||fb->width>8192||fb->height>8192||fb->pitch<fb->width*4U||(fb->pitch&3U)!=0){serial_text("SECURITY CHECK FAILED: INVALID FRAMEBUFFER\\r\\n");halt();} width=fb->width;height=fb->height;stride=fb->pitch/4;pixels=(volatile uint32_t*)fb->address; desktop(); serial_text("GENKERNEL BOOTED\\r\\n"); native_desktop_loop(); }
void _start(void){ genkernel_main(); }
