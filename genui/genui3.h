#ifndef GENOS_GENUI3_H
#define GENOS_GENUI3_H
#include <stddef.h>
#include <stdint.h>
#define GUI_MAX_WINDOWS 16
#define GUI_EVENT_CAPACITY 32
typedef enum { GUI_KEY=1,GUI_POINTER_MOVE,GUI_POINTER_BUTTON,GUI_REDRAW,GUI_CLOSE } gui_event_type_t;
typedef struct { uint8_t type; int16_t x,y; uint16_t code; uint32_t value; } gui_event_t;
typedef struct { uint32_t id; int16_t x,y,w,h; uint8_t visible,focused,minimized; char title[32]; } gui_window_t;
typedef struct { gui_event_t events[GUI_EVENT_CAPACITY]; size_t head,tail,count; gui_window_t windows[GUI_MAX_WINDOWS]; size_t window_count; uint32_t focused_id,next_id; } gui_context_t;
void gui_init(gui_context_t *ctx);
int gui_post_event(gui_context_t *ctx,const gui_event_t *event);
int gui_next_event(gui_context_t *ctx,gui_event_t *event);
int gui_window_create(gui_context_t *ctx,const char *title,int16_t x,int16_t y,int16_t w,int16_t h,uint32_t *id);
int gui_window_focus(gui_context_t *ctx,uint32_t id);
int gui_window_move(gui_context_t *ctx,uint32_t id,int16_t x,int16_t y);
int gui_window_minimize(gui_context_t *ctx,uint32_t id,int minimized);
int gui_window_close(gui_context_t *ctx,uint32_t id);
#endif
