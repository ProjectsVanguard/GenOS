#include "genui3.h"
static size_t len(const char*s,size_t cap){size_t n=0;if(!s)return cap;while(n<cap&&s[n])n++;return n;}
static void cp(char*d,const char*s,size_t cap){size_t i=0;if(!cap)return;while(i+1<cap&&s&&s[i]){d[i]=s[i];i++;}d[i]=0;}
static int find(gui_context_t*c,uint32_t id){if(!c)return -1;for(size_t i=0;i<c->window_count;i++)if(c->windows[i].id==id)return (int)i;return -1;}
void gui_init(gui_context_t*c){if(!c)return;c->head=c->tail=c->count=c->window_count=0;c->focused_id=0;c->next_id=1;}
int gui_post_event(gui_context_t*c,const gui_event_t*e){if(!c||!e||c->count>=GUI_EVENT_CAPACITY)return -1;c->events[c->tail]=*e;c->tail=(c->tail+1)%GUI_EVENT_CAPACITY;c->count++;return 0;}
int gui_next_event(gui_context_t*c,gui_event_t*e){if(!c||!e||!c->count)return -1;*e=c->events[c->head];c->head=(c->head+1)%GUI_EVENT_CAPACITY;c->count--;return 0;}
int gui_window_create(gui_context_t*c,const char*t,int16_t x,int16_t y,int16_t w,int16_t h,uint32_t*id){if(!c||!t||!id||w<=0||h<=0||!len(t,32)||len(t,32)>=32||c->window_count>=GUI_MAX_WINDOWS)return -1;gui_window_t*a=&c->windows[c->window_count++];a->id=c->next_id++;a->x=x;a->y=y;a->w=w;a->h=h;a->visible=1;a->focused=1;a->minimized=0;cp(a->title,t,sizeof(a->title));for(size_t i=0;i+1<c->window_count;i++)c->windows[i].focused=0;c->focused_id=a->id;*id=a->id;return 0;}
int gui_window_focus(gui_context_t*c,uint32_t id){int i=find(c,id);if(i<0||!c->windows[i].visible||c->windows[i].minimized)return -1;for(size_t j=0;j<c->window_count;j++)c->windows[j].focused=(j==(size_t)i);c->focused_id=id;return 0;}
int gui_window_move(gui_context_t*c,uint32_t id,int16_t x,int16_t y){int i=find(c,id);if(i<0)return -1;c->windows[i].x=x;c->windows[i].y=y;return 0;}
int gui_window_minimize(gui_context_t*c,uint32_t id,int minimized){int i=find(c,id);if(i<0)return -1;c->windows[i].minimized=minimized?1:0;c->windows[i].visible=minimized?0:1;if(minimized&&c->focused_id==id){c->windows[i].focused=0;c->focused_id=0;}return 0;}
int gui_window_close(gui_context_t*c,uint32_t id){int i=find(c,id);if(i<0)return -1;for(size_t j=(size_t)i;j+1<c->window_count;j++)c->windows[j]=c->windows[j+1];c->window_count--;if(c->focused_id==id)c->focused_id=0;return 0;}
