#ifndef GENOS_NET_PARSE_H
#define GENOS_NET_PARSE_H
#include <stddef.h>
#include <stdint.h>
typedef struct { const uint8_t *payload; size_t payload_len; uint16_t ethertype; } net_frame_t;
typedef struct { uint8_t src[4], dst[4], protocol; const uint8_t *payload; size_t payload_len; } net_ipv4_t;
int net_parse_ethernet(const void *packet,size_t length,net_frame_t *out);
int net_parse_ipv4(const void *packet,size_t length,net_ipv4_t *out);
uint16_t net_checksum16(const void *data,size_t length);
#endif
