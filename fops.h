#pragma once
#define FUSE_USE_VERSION 30

#include <fuse.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


int do_getattr(const char *path, struct stat *st);

int do_readdir(const char *path, void *buffer, fuse_fill_dir_t filler,
                      off_t offset, struct fuse_file_info *fi);

int do_read(const char *path, char *buffer, size_t size, off_t offset,
                   struct fuse_file_info *fi);

int do_mkdir(const char *path, mode_t mode);

int do_open(const char *path, struct fuse_file_info *fi);

int do_mknod(const char *path, mode_t mode, dev_t rdev);

int do_write(const char *path, const char *buffer, size_t size,
                    off_t offset, struct fuse_file_info *info);
