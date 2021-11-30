#include <fs.h>
#include <device.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
  {"/dev/events", 0, 0, events_read, invalid_write},
#include "files.h"
};

static size_t *open_offset;
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

void init_fs() {
  open_offset = malloc(sizeof(size_t) * sizeof(file_table) / sizeof(Finfo));
  // TODO: initialize the size of /dev/fb
}

int fs_open(const char *pathname, int flags, int mode) {
#ifdef STRACE
  printf("Open \"%s\"\n", pathname);
#endif
  for (int i = 0; i < sizeof(file_table) / sizeof(Finfo); i++) {
    if (strcmp(file_table[i].name, pathname) == 0) {
      open_offset[i] = file_table[i].disk_offset;
      return i;
    }
  }
  panic("File not found");
}

size_t fs_read(int fd, void *buf, size_t len) {
  ReadFn read = file_table[fd].read;
  if (read == NULL) {
    len = 
      (open_offset[fd] + len > file_table[fd].disk_offset + file_table[fd].size) ? 
        file_table[fd].disk_offset + file_table[fd].size - open_offset[fd]:
        len;
#ifdef STRACE
    printf("Read from \"%s\", offset:%d, len:%d\n", file_table[fd].name, open_offset[fd], len);
#endif
    ramdisk_read(buf, open_offset[fd], len);
    open_offset[fd] += len;
    assert(open_offset[fd] <= file_table[fd].disk_offset + file_table[fd].size);
    return len;
  }
  else {
    return read(buf, 0, len);
  }
}

size_t fs_write(int fd, const void *buf, size_t len) {
  WriteFn write = file_table[fd].write;
  if (write == NULL) {
    len = 
      (open_offset[fd] + len > file_table[fd].disk_offset + file_table[fd].size) ? 
        file_table[fd].disk_offset + file_table[fd].size - open_offset[fd]:
        len;
#ifdef STRACE
    printf("Write into \"%s\", offset:%d, len:%d\n", file_table[fd].name, open_offset[fd], len);
#endif
    ramdisk_write(buf, open_offset[fd], len);
    open_offset[fd] += len;
    assert(open_offset[fd] <= file_table[fd].disk_offset + file_table[fd].size);
    return len;
  }
  else {
    return write(buf, 0, len);
  }
}

size_t fs_lseek(int fd, size_t offset, int whence) {
  switch (whence) {
    case SEEK_SET:
      open_offset[fd] = file_table[fd].disk_offset + offset;
      assert(open_offset[fd] >= file_table[fd].disk_offset);
      assert(open_offset[fd] <= file_table[fd].disk_offset + file_table[fd].size);
      break;

    case SEEK_CUR:
      open_offset[fd] += offset;
      assert(open_offset[fd] >= file_table[fd].disk_offset);
      assert(open_offset[fd] <= file_table[fd].disk_offset + file_table[fd].size);
      break;

    case SEEK_END:
      open_offset[fd] = file_table[fd].disk_offset + file_table[fd].size + offset;
      assert(open_offset[fd] >= file_table[fd].disk_offset);
      assert(open_offset[fd] <= file_table[fd].disk_offset + file_table[fd].size);
      break;

    default:
      return -1;
  }
  return open_offset[fd] - file_table[fd].disk_offset;
}

int fs_close(int fd) {
#ifdef STRACE
    printf("Close \"%s\"\n", file_table[fd].name);
#endif
  return 0;
}