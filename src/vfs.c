#include "vfs.h"

fs_node_t *fs_root = 0;

uint32_t fs_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
    if (node->read != 0) {
        return node->read(node, offset, size, buffer);
    }
    else {
        return 0;
    }
}

struct initrd_header *ird_header;
struct initrd_file_header *file_headers;
fs_node_t *initrd_root;
fs_node_t *initrd_dev;
fs_node_t *root_nodes;
int32_t nroot_nodes;

struct dirent dir_entry;

static uint32_t initrd_read(
    fs_node_t *node,
    uint32_t offset,
    uint32_t size,
    uint8_t *buffer) {
    struct initrd_file_header header = file_headers[node->inode];
    if (offset > header.length) {
        return 0;
    }
    if (offset + size > header.length) {
        size = header.length - offset;
    }

    
}
