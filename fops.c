#include "fops.h"

static char dir_list[256][256];
static int curr_dir_idx = -1;
static char files_list[256][256];
static int curr_file_idx = -1;
static char files_content[256][256];
static int curr_file_content_idx = -1;

static void add_dir(const char *dir_name) {
  curr_dir_idx++;
  strcpy(dir_list[curr_dir_idx], dir_name);
}

static int is_dir(const char *path) {
  path++;

  for (int curr_idx = 0; curr_idx <= curr_dir_idx; curr_idx++)
    if (strcmp(path, dir_list[curr_idx]) == 0)
      return 1;

  return 0;
}

static void add_file(const char *filename) {
  curr_file_idx++;
  strcpy(files_list[curr_file_idx], filename);

  curr_file_content_idx++;
  strcpy(files_content[curr_file_content_idx], "");
}

static int is_file(const char *path) {
  path++;

  for (int curr_idx = 0; curr_idx <= curr_file_idx; curr_idx++)
    if (strcmp(path, files_list[curr_idx]) == 0)
      return 1;

  return 0;
}

static int get_file_index(const char *path) {
  path++;

  for (int curr_idx = 0; curr_idx <= curr_file_idx; curr_idx++)
    if (strcmp(path, files_list[curr_idx]) == 0)
      return curr_idx;

  return -1;
}

static void write_to_file(const char *path, const char *new_content) {
  int file_idx = get_file_index(path);

  if (file_idx == -1)
    return;

  strcpy(files_content[file_idx], new_content);
}

int do_getattr(const char *path, struct stat *st) {
  st->st_uid = getuid();
  st->st_gid = getgid();
  st->st_atime = time(NULL);
  st->st_mtime = time(NULL);

  if (strcmp(path, "/") == 0 || is_dir(path) == 1) {
    st->st_mode = S_IFDIR | 0755;
    st->st_nlink = 2;
  } else if (is_file(path) == 1) {
    st->st_mode = S_IFREG | 0644;
    st->st_nlink = 1;
    st->st_size = 1024;
  } else {
    return -ENOENT;
  }

  return 0;
}

int do_readdir(const char *path, void *buffer, fuse_fill_dir_t filler,
               off_t offset, struct fuse_file_info *fi) {
  filler(buffer, ".", NULL, 0);
  filler(buffer, "..", NULL, 0);

  if (strcmp(path, "/") == 0) {
    for (int curr_idx = 0; curr_idx <= curr_dir_idx; curr_idx++)
      filler(buffer, dir_list[curr_idx], NULL, 0);

    for (int curr_idx = 0; curr_idx <= curr_file_idx; curr_idx++)
      filler(buffer, files_list[curr_idx], NULL, 0);
  }

  return 0;
}

int do_read(const char *path, char *buffer, size_t size, off_t offset,
            struct fuse_file_info *fi) {
  int file_idx = get_file_index(path);

  if (file_idx == -1)
    return -1;

  char *content = files_content[file_idx];

  memcpy(buffer, content + offset, size);

  return strlen(content) - offset;
}

int do_mkdir(const char *path, mode_t mode) {
  path++;
  add_dir(path);

  return 0;
}

int do_mknod(const char *path, mode_t mode, dev_t rdev) {
  path++;
  add_file(path);

  return 0;
}

int do_write(const char *path, const char *buffer, size_t size, off_t offset,
             struct fuse_file_info *info) {
  write_to_file(path, buffer);

  return size;
}

int do_open(const char *path, struct fuse_file_info *fi)
{
    if(strcmp(path, "/test") != 0)
        return -ENOENT;
    if((fi->flags & 3) != O_RDONLY)
        return -EACCES;
    return 0;
}
