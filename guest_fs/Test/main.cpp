#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <guestfs.h>
#include <sys/stat.h>
#include <assert.h>

static int
compare_keys_len (const void *p1, const void *p2)
{
    const char *key1 = * (char * const *) p1;
    const char *key2 = * (char * const *) p2;
    return strlen (key1) - strlen (key2);
}

static size_t
count_strings (char *const *argv)
{
    size_t c;

    for (c = 0; argv[c]; ++c)
        ;
    return c;
}

static const char* UML_KERNEL_PATH  = "/home/novokrest/Github/Master/bin/linux";
static const char* DISK_IMG_PATH    = "/home/novokrest/Github/Master/bin/root_fs";
static const char* TEST_FILE_PATH  = "/root/test_guest_fs/hello.txt";

void check_file_exist(const char* path)
{
    struct stat buffer;
    assert(stat(path, &buffer) == 0);
}

int main (int argc, char *argv[])
{
    check_file_exist(DISK_IMG_PATH);
    check_file_exist(UML_KERNEL_PATH);

    guestfs_h *g;
    const char *disk;
    char **roots, *root, *str, **mountpoints, **lines;
    size_t i, j;

//    if (argc != 2) {
//        fprintf (stderr, "usage: inspect_vm disk.img\n");
//        exit (EXIT_FAILURE);
//    }
//    disk = argv[1];

    disk = DISK_IMG_PATH;

    g = guestfs_create ();
    if (g == NULL) {
        perror ("failed to create libguestfs handle");
        exit (EXIT_FAILURE);
    }

    guestfs_set_verbose(g, 1);
    if (guestfs_set_hv(g, UML_KERNEL_PATH) == -1) {
        fprintf(stderr, "can't set UML as hypervisor");
        exit(EXIT_FAILURE);
    }

    /* Attach the disk image read-only to libguestfs. */
    if (guestfs_add_drive_opts (g, disk,
                                /* GUESTFS_ADD_DRIVE_OPTS_FORMAT, "raw", */
                                GUESTFS_ADD_DRIVE_OPTS_READONLY, 1,
                                -1) /* this marks end of optional arguments */
            == -1)
        exit (EXIT_FAILURE);

    /* Run the libguestfs back-end. */
    if (guestfs_launch (g) == -1)
        exit (EXIT_FAILURE);

    /* Ask libguestfs to inspect for operating systems. */
    roots = guestfs_inspect_os (g);
    if (roots == NULL)
        exit (EXIT_FAILURE);
    if (roots[0] == NULL) {
        fprintf (stderr, "inspect_vm: no operating systems found\n");
        exit (EXIT_FAILURE);
    }

    for (j = 0; roots[j] != NULL; ++j) {
        root = roots[j];

        printf ("Root device: %s\n", root);

        /* Print basic information about the operating system. */
        str = guestfs_inspect_get_product_name (g, root);
        if (str)
            printf ("  Product name: %s\n", str);
        free (str);

        printf ("  Version:      %d.%d\n",
                guestfs_inspect_get_major_version (g, root),
                guestfs_inspect_get_minor_version (g, root));

        str = guestfs_inspect_get_type (g, root);
        if (str)
            printf ("  Type:         %s\n", str);
        free (str);
        str = guestfs_inspect_get_distro (g, root);
        if (str)
            printf ("  Distro:       %s\n", str);
        free (str);

        /* Mount up the disks, like guestfish -i.
      *
      * Sort keys by length, shortest first, so that we end up
      * mounting the filesystems in the correct order.
      */
        mountpoints = guestfs_inspect_get_mountpoints (g, root);
        if (mountpoints == NULL)
            exit (EXIT_FAILURE);

        qsort (mountpoints, count_strings (mountpoints) / 2, 2 * sizeof (char *),
               compare_keys_len);
        for (i = 0; mountpoints[i] != NULL; i += 2) {
            /* Ignore failures from this call, since bogus entries can
        * appear in the guest's /etc/fstab.
        */
            guestfs_mount_ro (g, mountpoints[i+1], mountpoints[i]);

            free (mountpoints[i]);
            free (mountpoints[i+1]);
        }
        free (mountpoints);

        /* If /etc/issue.net file exists, print up to 3 lines. */
        if (guestfs_is_file (g, "/etc/issue.net") > 0) {
            printf ("--- /etc/issue.net ---\n");
            lines = guestfs_head_n (g, 3, "/etc/issue.net");
            if (lines == NULL)
                exit (EXIT_FAILURE);
            for (i = 0; lines[i] != NULL; ++i) {
                printf ("%s\n", lines[i]);
                free (lines[i]);
            }
            free (lines);
        }

        printf("\n===============\n");
        size_t file_size;
        char* file_content = guestfs_read_file(g,TEST_FILE_PATH, &file_size);
        if (file_size == 0) {
            printf("Cant read file %s", TEST_FILE_PATH);
        }
        else {
            printf("Content of \"%s\":\n%s", TEST_FILE_PATH, file_content);
        }
        free(file_content);
        printf("===============\n");

        /* Unmount everything. */
        if (guestfs_umount_all (g) == -1)
            exit (EXIT_FAILURE);

        free (root);
    }
    free (roots);

    guestfs_close (g);

    exit (EXIT_SUCCESS);
}
