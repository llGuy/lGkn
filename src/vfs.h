#ifndef VFS_H
#define VFS_H

#include "int.h"

#define FS_FILE 0x01
#define FS_DIRECTORY 0x02
#define FS_CHARDEVICE 0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE 0x05
#define FS_SYMLINK 0x06
#define FS_MOUNTPOINT 0x08;

struct fs_node;

typedef uint32_t (*read_type_t)(struct fs_node*,uint32_t,uint32_t,uint8_t*);
typedef uint32_t (*write_type_t)(struct fs_node*,uint32_t,uint32_t,uint8_t*);
typedef void (*open_type_t)(struct fs_node*);
typedef void (*close_type_t)(struct fs_node*);
typedef struct dirent * (*readdir_type_t)(struct fs_node*,uint32_t);
typedef struct fs_node * (*finddir_type_t)(struct fs_node*,char *name);

typedef struct fs_node {
  char name[128];

  uint32_t mask; // Permissions
  uint32_t uid; // Owning user
  uint32_t gid; // Owning group
  uint32_t flags;
  // This is device-specific - provides a way for a filesystem to identify files
  uint32_t inode;
  uint32_t length;
  uint32_t impl; // Implementation defined number

  read_type_t read;
  write_type_t write;
  open_type_t open;
  close_type_t close;
  readdir_type_t readdir;
  finddir_type_t finddir;
    
  // Used by mountpoints and symlinks
  struct fs_node *ptr;
} fs_node_t;

typedef struct dirent {
  char name[128];
  uint32_t inode;
} dirent_t;

extern fs_node_t *fs_root;

uint32_t fs_read(
  fs_node_t *node,
  uint32_t offset,
  uint32_t size,
  uint8_t *buffer);

uint32_t fs_write(
  fs_node_t *node,
  uint32_t offset,
  uint32_t size,
  uint8_t *buffer);

void fs_open(fs_node_t *node, uint8_t read, uint8_t write);
void fs_close(fs_node_t *node);
dirent_t *fs_readdir(fs_node_t *node, uint32_t index);
fs_node_t *fs_finddir(fs_node_t *node, char *name);

struct initrd_header {
  uint32_t nfiles;
};

struct initrd_file_header {
  uint32_t magic;
  char name[64];
  uint32_t offset;
  uint32_t length;
};

fs_node_t *initrd_init(uint32_t location /* Address of multiboot module */);

#endif
