#ifndef GENOS_VFS_H
#define GENOS_VFS_H
#include <stddef.h>
#include <stdint.h>
#define VFS_MAX_NODES 64
#define VFS_NAME_MAX 48
#define VFS_FILE_MAX 4096
#define VFS_PATH_MAX 192
#define VFS_DIRECTORY 1u
#define VFS_REGULAR 2u
typedef struct { char name[VFS_NAME_MAX]; uint8_t type; const uint8_t *data; size_t size; } vfs_node_t;
typedef struct { vfs_node_t nodes[VFS_MAX_NODES]; size_t count; } vfs_t;
typedef struct { const vfs_node_t *node; size_t offset; } vfs_handle_t;
/* Initramfs is immutable: mount copies metadata, not file payloads. */
int vfs_init(vfs_t *fs);
int vfs_mkdir(vfs_t *fs, const char *absolute_path);\nint vfs_add_file(vfs_t *fs, const char *absolute_path, const void *data, size_t size);
int vfs_stat(const vfs_t *fs, const char *absolute_path, vfs_node_t *out);
int vfs_open(const vfs_t *fs, const char *absolute_path, vfs_handle_t *out);
size_t vfs_read(vfs_handle_t *handle, void *dst, size_t capacity);
int vfs_list(const vfs_t *fs, const char *directory, size_t index, vfs_node_t *out);
int initramfs_mount(vfs_t *fs, const void *image, size_t image_size);
#endif
