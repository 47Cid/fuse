#define FUSE_USE_VERSION 30

#include "fops.h"
#include <fuse.h>

static struct fuse_operations operations = {
    .getattr = do_getattr,
    .readdir = do_readdir,
    .read = do_read,
    .mkdir = do_mkdir,
    .open = do_open,
    .mknod = do_mknod,
    .write = do_write,
};

int main(int argc, char *argv[]) {
  return fuse_main(argc, argv, &operations, NULL);
}
