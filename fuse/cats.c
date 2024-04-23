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
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fuse.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define MAX_BREEDS 120
#define MAX_BREED_NAME_LEN 50
#define MAX_URL_LEN 100

static const char *BREEDS_URL = "https://catfact.ninja/breeds";
static char breeds[MAX_BREEDS][MAX_BREED_NAME_LEN];

static int n_breeds = 0;

struct curl_buf {
    char *data;
    size_t size;
};

static size_t write_buf(
    void *contents,
    size_t size,
    size_t nmemb,
    void *userp
) {
    size_t real_size = size * nmemb;
    struct curl_buf *buf = (struct curl_buf *) userp;
    char *ptr = realloc(buf->data, buf->size + real_size + 1);

    buf->data = ptr;
    memcpy(&(buf->data[buf->size]), contents, real_size);
    buf->size += real_size;
    buf->data[buf->size] = 0;
    return real_size;
}

static cJSON *get_payload(struct curl_buf *chunk, const char *url) {
    cJSON *payload = NULL;
    CURL *curl_handle = NULL;
    CURLcode response;
    cJSON *parsed = NULL;

    chunk->data = malloc(1);  /* grown as needed by realloc */
    chunk->size = 0;

    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_buf);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) chunk);
    response = curl_easy_perform(curl_handle);
    if (response != CURLE_OK) {
        return NULL;
    }
    curl_easy_cleanup(curl_handle);
    parsed = cJSON_Parse(chunk->data);
    free(chunk->data);
    return parsed;
}

static int cats_getattr(const char *path, struct stat *buf) {
    int found = 0;

    if (strcmp(path, "/") == 0) {
        buf->st_mode = S_IFDIR | 0755;
        buf->st_nlink = 2;
        found = 1;
    } else {
        int i = 0;

        for (i = 0; i < n_breeds; i++) {
            if (strncmp(breeds[i], path + 1, MAX_BREED_NAME_LEN) == 0) {
                buf->st_mode = S_IFREG | 0444;
                buf->st_nlink = 1;
                found = 1;
                break;
            }
        }
    }

    return found ? 0 : -ENOENT;
}

static int cats_readdir(
    const char *path,
    void *buf,
    fuse_fill_dir_t filler,
    off_t offset,
    struct fuse_file_info *fi
) {
    int i = 0;

    if (strcmp(path, "/") != 0) {
        return -ENOENT;
    }

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    for (i = 0; i < n_breeds; i++) {
        filler(buf, breeds[i], NULL, 0);
    }
    return 0;
}

static struct fuse_operations cats_ops = {
    .getattr = cats_getattr,
    .readdir = cats_readdir,
};

void fill_breed_names() {
    struct curl_buf chunk = { 0 };
    cJSON *payload = NULL;
    const cJSON *last_page = NULL;
    const cJSON *breed_data = NULL;
    const cJSON *breed = NULL;
    int n_pages = 0;
    char url[MAX_URL_LEN] = "";
    char query[10];
    int page = 0;
    int i = 0;

    /* printf("Fetching page: %s\n", BREEDS_URL); */
    payload = get_payload(&chunk, BREEDS_URL);

    last_page = cJSON_GetObjectItemCaseSensitive(payload, "last_page");
    n_pages = last_page->valueint;
    /* printf("Number of pages: %d\n", n_pages); */

    page = 1;
    while (1) {
        breed_data = cJSON_GetObjectItemCaseSensitive(payload, "data");
        cJSON_ArrayForEach(breed, breed_data) {
            const cJSON *name = cJSON_GetObjectItemCaseSensitive(breed, "breed");
            strncpy(breeds[n_breeds], name->valuestring, MAX_BREED_NAME_LEN);
            /* printf("Breed %d: %s\n", n_breeds + 1, breeds[n_breeds]); */
            n_breeds++;
        }
        cJSON_Delete(payload);

        page++;
        if (page > n_pages) {
            break;
        }
        strncpy(url, BREEDS_URL, MAX_URL_LEN);
        sprintf(query, "?page=%d", page);
        strcat(url, query);
        /* printf("Fetching page: %s\n", url); */
        payload = get_payload(&chunk, url);
    }

    /* for (i = 0; i < n_breeds; i++) {
        if (i != 0) {
            printf(", ");
        }
        printf("%d: %s", i + 1, breeds[i]);
    }
    printf("\n"); */
}

int main(int argc, char *argv[]) {
    curl_global_init(CURL_GLOBAL_ALL);
    fill_breed_names();
    /* cur_global_cleanup(); */
    return fuse_main(argc, argv, &cats_ops, NULL);
}
