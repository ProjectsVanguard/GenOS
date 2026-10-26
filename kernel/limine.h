#ifndef GENOS_LIMINE_H
#define GENOS_LIMINE_H
#include <stdint.h>
#define LIMINE_BASE_REVISION(N) uint64_t limine_base_revision[3] = {0xf9562b2d5c95a6c8ULL, 0x6a7b384944536bdcULL, (N)}
#define LIMINE_BASE_REVISION_SUPPORTED (limine_base_revision[2] == 0)
#define LIMINE_REQUESTS_START_MARKER __attribute__((used, section(".limine_requests_start"))) static volatile uint64_t limine_requests_start_marker[4] = {0xf6b8f4b39de7d1aeULL, 0xfab91a6940fcb9cfULL, 0x785c6ed015d3e316ULL, 0x181e920a7852b9d9ULL}
#define LIMINE_REQUESTS_END_MARKER __attribute__((used, section(".limine_requests_end"))) static volatile uint64_t limine_requests_end_marker[2] = {0xadc0e0531bb10d03ULL, 0x9572709f31764c62ULL}
#define LIMINE_FRAMEBUFFER_REQUEST_ID {0x9d5827dcd881dd75ULL, 0xa3148604f6fab11bULL}
#define LIMINE_FRAMEBUFFER_REQUEST_REVISION 0
struct limine_framebuffer;
struct limine_framebuffer_response { uint64_t revision, framebuffer_count; struct limine_framebuffer **framebuffers; };
struct limine_framebuffer_request { uint64_t id[4]; uint64_t revision; struct limine_framebuffer_response *response; };
struct limine_framebuffer { void *address; uint64_t width, height, pitch; uint16_t bpp; uint8_t memory_model; uint8_t red_mask_size, red_mask_shift, green_mask_size, green_mask_shift, blue_mask_size, blue_mask_shift; };
#define LIMINE_HHDM_REQUEST_ID {0x48dcf1cb8ad2b852ULL, 0x63984e959a98244bULL}
struct limine_hhdm_response { uint64_t revision, offset; };
struct limine_hhdm_request { uint64_t id[4]; uint64_t revision; struct limine_hhdm_response *response; };
#endif
