#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include <guestfs.h>

static const char * REPORT_FILENAME = "report.txt";
static const char * REPORT_DELIMITER = "=======================";

bool check_file_exists(const char* filename)
{
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}

static int
compare_keys_len(const void *p1, const void *p2)
{
    const char *key1 = *(char * const *)p1;
    const char *key2 = *(char * const *)p2;
    return strlen(key1) - strlen(key2);
}

static size_t
count_strings(char *const *argv)
{
    size_t c;

    for (c = 0; argv[c]; ++c)
        ;
    return c;
}

int mount_disks(guestfs_h* gfs)
{
    char **roots, *root, **mountpoints;

    printf("guestfs_inspect_os()...\n");
    roots = guestfs_inspect_os(gfs);
    if (roots == NULL || roots[0] == NULL) {
        printf("ERROR: no roots\n");
        return 1;
    }

    printf("iter over founded roots...\n");
    for (int j = 0; roots[j] != NULL; ++j) {
        root = roots[j];
        printf("root: %s\n", root);

        mountpoints = guestfs_inspect_get_mountpoints(gfs, root);
        if (mountpoints == NULL) {
            printf("ERROR: failed to inspect mountpoints for root: %s\n", root);
            return 1;
        }

        qsort(mountpoints, count_strings(mountpoints) / 2, 2 * sizeof(char*), compare_keys_len);
        for (int i = 0; mountpoints[i] != NULL; i += 2) {
            /* Ignore failures from this call, since bogus entries can appear in the guest's /etc/fstab.*/
            guestfs_mount_ro (gfs, mountpoints[i+1], mountpoints[i]);
            free (mountpoints[i]);
            free (mountpoints[i+1]);
        }
        free (mountpoints);
        free (root);
    }
    free(roots);

    return 0;
}

int read_file(guestfs_h* gfs, char const* remote_file_path)
{
    size_t fsize;
    char *file_content;

    printf("READ '%s'\n", remote_file_path);
    file_content = guestfs_read_file(gfs, remote_file_path, &fsize);
    if (!file_content) {
        printf("ERROR: read file='%s'\n", remote_file_path);
        return 1;
    }
    //printf("file's content: %s\n", file_content);
    free(file_content);

    return 0;
}

int download_file(guestfs_h *g, const char *remote_filepath, const char *filepath)
{
    if (guestfs_download(g, remote_filepath, filepath) == -1) {
        printf("ERROR: guestfs_download()\n");
        return -1;
    }
    printf("File has been downloaded to '%s'\n", filepath);
    return 0;
}

void
test_guestfs()
{
    char* ts_titles[] = { "t_start", "t_created", "t_drive_added", "t_launched", "t_mounted", "t_big_readed", "t_smalls_readed", "t_closed", "t_end" };
    time_t ts[sizeof(ts_titles)];
    int ts_cur = 0;
    guestfs_h *g;

    time(ts + ts_cur++);

    g = guestfs_create();
    time(ts + ts_cur++);
    if (g == NULL) {
        printf("ERROR: guestfs_create()\n");
        exit(EXIT_FAILURE);
    }

    printf("guestfs_set_verbose()...\n");
    if (guestfs_set_verbose(g, 1) == -1) {
        printf("ERROR occurred during guestfs_set_verbose()\n");
        exit(EXIT_FAILURE);
    }

    printf("guestfs_get_backend()...\n");
    char* default_backend = guestfs_get_backend(g);
    if (strcmp(default_backend, "direct")) {
        printf("guestfs_set_backend()...\n");
        if (guestfs_set_backend(g, "direct") == -1) {
            printf("ERROR occurred during guestfs_set_backend()\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("backend is direct\n");

    const char* disk_image_path = "/var/tmp/.guestfs-1000/appliance.d/disk.img";
    if (!check_file_exists(disk_image_path)) {
        printf("disk_image doesn't exist: %s\n", disk_image_path);
        exit(EXIT_FAILURE);
    }

    if (guestfs_add_drive_opts(g, disk_image_path,
        GUESTFS_ADD_DRIVE_OPTS_FORMAT, "raw",
        GUESTFS_ADD_DRIVE_OPTS_READONLY, 0,
        -1) == -1) {
        printf("ERROR occurred during guestfs_add_drive_opts\n");
        exit(EXIT_FAILURE);
    }
    time(ts + ts_cur++);

    printf("guestfs_launch()...\n");
    if (guestfs_launch(g) == -1) {
        printf("ERROR occurred during guestfs_launch()\n");
        exit(EXIT_FAILURE);
    }
    time(ts + ts_cur++);

    if (mount_disks(g) == -1) {
        printf("ERROR occurred during mount_disks()\n");
        exit(EXIT_FAILURE);
    }
    time(ts + ts_cur++);

    //read_file(g, "/home/novokrestdeb/123.txt");

    download_file(g, "/home/novokrestdeb/test/bigfile", "bigfile");
    time(ts + ts_cur++);

    //for (size_t i = 0; i < 10000; ++i) {
    //    char buf[50];
    //    sprintf(buf, "/home/novokrestdeb/test/%u", i);
    //    read_file(g, buf);
    //}
    download_file(g, "/home/novokrestdeb/test/1", "smallfile");
    time(ts + ts_cur++);

    printf("guestfs_close()...\n");
    guestfs_close(g);
    time(ts + ts_cur++);

    FILE* freport = fopen(REPORT_FILENAME, "a");
    fprintf(freport, "%s\n", REPORT_DELIMITER);
    for (int i = 1; i < ts_cur; ++i) {
        fprintf(freport,"%s: %f\n", ts_titles[i], difftime(ts[i], ts[i - 1]));
    }
    fprintf(freport, "total_time: %f\n", difftime(ts[ts_cur - 1], ts[0]));
    fprintf(freport, "%s\n", REPORT_DELIMITER);
}
