#ifndef GENOS_NATIVE_APPS_H
#define GENOS_NATIVE_APPS_H
#include <stdint.h>
typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t (*rgb)(uint8_t, uint8_t, uint8_t);
    void (*rect)(int, int, int, int, uint32_t);
    void (*text)(int, int, const char *, uint32_t, int);
} genos_native_ui;
void genos_native_apps_render(const genos_native_ui *ui, unsigned selected, unsigned screen,
                              const char *note, unsigned note_len,
                              const char *calc, const char *result);
unsigned genos_native_apps_key(unsigned key, unsigned selected, unsigned screen,
                               char *note, unsigned *note_len, char *calc, unsigned *calc_len,
                               char *result);
#endif
