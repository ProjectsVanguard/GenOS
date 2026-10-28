#ifndef GENOS_USB_DESC_H
#define GENOS_USB_DESC_H
#include <stddef.h>
#include <stdint.h>
typedef struct { uint8_t length,type; const uint8_t *bytes; } usb_descriptor_t;
/* Returns 0 on a validated descriptor, -1 on malformed/truncated input. */
int usb_descriptor_at(const void *buffer,size_t total,size_t offset,usb_descriptor_t *out);
int usb_find_interface(const void *buffer,size_t total,uint8_t interface_number,uint8_t *class_code,uint8_t *subclass,uint8_t *protocol);
#endif
