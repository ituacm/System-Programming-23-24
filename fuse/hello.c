/* Copyright (C) 2024 H. Turgut Uyar <uyar@tekir.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define FUSE_USE_VERSION 26

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fuse.h>

static const char *hello_path = "/hello.txt";
static const char *hello_msg = "Hello, world!\n";

static int hello_getattr(const char *path, struct stat *buf) {
    memset(buf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        buf->st_mode = S_IFDIR | 0755;
        buf->st_nlink = 2;
    } else if (strcmp(path, hello_path) == 0) {
        buf->st_mode = S_IFREG | 0444;
        buf->st_nlink = 1;
        buf->st_size = strlen(hello_msg);
    } else {
        return -ENOENT;
    }
    return 0;
}

static int hello_readdir(
    const char *path,
    void *buf,
    fuse_fill_dir_t filler,
    off_t offset,
    struct fuse_file_info *fi
) {
    if (strcmp(path, "/") != 0) {
        return -ENOENT;
    }
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    filler(buf, hello_path + 1, NULL, 0);
    return 0;
}

static int hello_open(const char *path, struct fuse_file_info *fi) {
    if (strcmp(path, hello_path) != 0) {
        return -ENOENT;
    }
    if ((fi->flags & 3) != O_RDONLY) {
        return -EACCES;
    }
    return 0;
}

static int hello_read(
    const char *path,
    char *buf,
    size_t count,
    off_t offset,
    struct fuse_file_info *fi
) {
    size_t len = 0;

    if (strcmp(path, hello_path) != 0) {
        return -ENOENT;
    }

    len = strlen(hello_msg);
    if (offset < len) {
        if (offset + count > len) {
            count = len - offset;
        }
        memcpy(buf, hello_msg + offset, count);
    } else {
        return 0;
    }
    return count;
}

static struct fuse_operations hello_ops = {
    .getattr = hello_getattr,
    .readdir = hello_readdir,
    .open    = hello_open,
    .read    = hello_read,
};

int main(int argc, char *argv[]) {
    return fuse_main(argc, argv, &hello_ops, NULL);
}
