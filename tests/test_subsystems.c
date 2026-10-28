#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "../kernel/subsystems/vfs.h"
#include "../kernel/subsystems/net_parse.h"
#include "../kernel/subsystems/usb_desc.h"
#include "../genui/genui3.h"
int main(void){
 vfs_t fs;assert(vfs_init(&fs)==0);static const char msg[]="GenOS";assert(vfs_add_file(&fs,"/boot/hello.txt",msg,5)==0);vfs_handle_t h;char b[8]={0};assert(vfs_open(&fs,"/boot/hello.txt",&h)==0);assert(vfs_read(&h,b,3)==3);assert(!memcmp(b,"Gen",3));assert(vfs_read(&h,b+3,5)==2);assert(!memcmp(b,"GenOS",5));assert(vfs_open(&fs,"/../bad",&h)<0);
 uint8_t eth[14]={0};eth[12]=0x08;eth[13]=0x00;net_frame_t frame;assert(net_parse_ethernet(eth,sizeof eth,&frame)==0);assert(frame.ethertype==0x0800&&frame.payload_len==0);assert(net_parse_ethernet(eth,13,&frame)<0);
 uint8_t usb[]={9,4,2,0,1,3,1,1,0};usb_descriptor_t d;assert(usb_descriptor_at(usb,sizeof usb,0,&d)==0&&d.type==4);uint8_t c,s,p;assert(usb_find_interface(usb,sizeof usb,2,&c,&s,&p)==0&&c==3&&s==1&&p==1);assert(usb_descriptor_at(usb,sizeof usb,1,&d)<0);
 gui_context_t gui;gui_init(&gui);uint32_t a,z;assert(gui_window_create(&gui,"Terminal",1,2,320,200,&a)==0);assert(gui_window_create(&gui,"Settings",2,3,200,100,&z)==0);assert(gui.focused_id==z);assert(gui_window_move(&gui,a,10,20)==0);assert(gui_window_minimize(&gui,z,1)==0);gui_event_t e={GUI_KEY,0,0,13,0},got;assert(gui_post_event(&gui,&e)==0&&gui_next_event(&gui,&got)==0&&got.code==13);assert(gui_window_close(&gui,a)==0);
 return 0;
}