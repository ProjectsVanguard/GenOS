#ifndef GENOS_FAT32_H
#define GENOS_FAT32_H
#include <stddef.h>
#include <stdint.h>
#define FAT32_SECTOR_SIZE 512u
#define FAT32_NAME_MAX 12u
typedef struct {
    const uint8_t *image;
    size_t image_size;
    uint32_t total_sectors, fat_start, fat_sectors, data_start;
    uint32_t root_cluster, cluster_count;
    uint8_t sectors_per_cluster, fat_count;
} fat32_volume_t;
typedef struct {
    char name[FAT32_NAME_MAX];
    uint8_t attributes;
    uint32_t first_cluster, size;
} fat32_entry_t;
/* Read-only FAT32 reader for a memory-mapped/raw image. No block driver or writes. */
int fat32_mount(fat32_volume_t *volume, const void *image, size_t image_size);
int fat32_find_root(const fat32_volume_t *volume, const char *short_name, fat32_entry_t *entry);
int fat32_read_file(const fat32_volume_t *volume, const fat32_entry_t *entry,
                    void *destination, size_t capacity, size_t *bytes_read);
#endif
