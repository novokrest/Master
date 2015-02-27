//#include <cstdlib>
//#include <cstdio>
//#include <iostream>
//#include <cstring>
//#include <string>
//
//#include <sys/stat.h>
//
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <sys/un.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <fcntl.h>
//
//#include <unistd.h>
//
//#include <guestfs.h>
//
//using std::cout;
//using std::endl;
//using std::string;
//
//
//bool check_file_exists(const char* filename)
//{
//    struct stat buffer;
//    return stat(filename, &buffer) == 0;
//}
//
//void print(string const& str)
//{
//    std::cout << str << std::endl;
//}
//
//static int
//compare_keys_len(const void *p1, const void *p2)
//{
//    const char *key1 = *(char * const *)p1;
//    const char *key2 = *(char * const *)p2;
//    return strlen(key1) - strlen(key2);
//}
//
//static size_t
//count_strings(char *const *argv)
//{
//    size_t c;
//
//    for (c = 0; argv[c]; ++c)
//        ;
//    return c;
//}
//
//int create_socket_un()
//{
//    struct sockaddr_un addr;
//    int accept_socket, newsock;
//    string guestfsd_sock;
//
//    accept_socket = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
//    if (accept_socket == -1) {
//        printf("ERROR: couldn't create socket");
//        exit(EXIT_FAILURE);
//    }
//
//    addr.sun_family = AF_UNIX;
//    guestfsd_sock = "/tmp/guestfsd_sock";
//    strncpy(addr.sun_path, guestfsd_sock.c_str(), sizeof(addr.sun_path));
//    addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';
//
//    int size = (offsetof(struct sockaddr_un, sun_path) + strlen(addr.sun_path));
//    if (bind(accept_socket, (struct sockaddr *) &addr, size) == -1) {
//        printf("ERROR: bind socket");
//        exit(EXIT_FAILURE);
//    }
//
//    if (listen(accept_socket, 1) == -1) {
//        printf("ERROR: listen socket");
//        exit(EXIT_FAILURE);
//    }
//
//    printf("Ready to accept\n");
//    newsock = accept4(accept_socket, NULL, NULL, SOCK_CLOEXEC);
//    printf("After accept\n");
//
//    if (newsock < 0) {
//        printf("ERROR: accept()\n");
//        exit(EXIT_FAILURE);
//    }
//
//    return 0;
//}
//
//int create_socket_in()
//{
//    struct sockaddr_in addr, cli_addr;
//    int accept_socket, newsock;
//    string guestfsd_sock;
//
//    accept_socket = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
//    if (accept_socket == -1) {
//        printf("ERROR: couldn't create socket\n");
//        exit(EXIT_FAILURE);
//    }
//
//    bzero((char*)&addr, sizeof(addr));
//    addr.sin_family = AF_INET;
//    addr.sin_addr.s_addr = htonl(INADDR_ANY);
//    addr.sin_port = htons(7777);
//
//    if (bind(accept_socket, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1) {
//        printf("ERROR: bind socket\n");
//        exit(EXIT_FAILURE);
//    }
//
//    if (listen(accept_socket, 1) == -1) {
//        printf("ERROR: listen socket\n");
//        exit(EXIT_FAILURE);
//    }
//
//    bzero((char*)&cli_addr, sizeof(cli_addr));
//    int clilen = sizeof(cli_addr);
//
//    printf("Ready to accept\n");
//    newsock = accept(accept_socket, (struct sockaddr*)&cli_addr, (socklen_t*)&clilen);
//    printf("After accept\n");
//
//    if (newsock < 0) {
//        printf("ERROR: accept()\n");
//        exit(EXIT_FAILURE);
//    }
//
//    return 0;
//}
//
//int mount_disks(guestfs_h* gfs)
//{
//    char **roots, *root, **mountpoints;
//
//    printf("guestfs_inspect_os()...\n");
//    roots = guestfs_inspect_os(gfs);
//    if (roots == NULL || roots[0] == NULL) {
//        printf("ERROR: no roots\n");
//        return 1;
//    }
//
//    printf("iter over founded roots...\n");
//    for (int j = 0; roots[j] != NULL; ++j) {
//        root = roots[j];
//        printf("root: %s\n", root);
//
//        mountpoints = guestfs_inspect_get_mountpoints(gfs, root);
//        if (mountpoints == NULL) {
//            printf("ERROR: failed to inspect mountpoints for root: %s\n", root);
//            return 1;
//        }
//
//        qsort(mountpoints, count_strings(mountpoints) / 2, 2 * sizeof(char*), compare_keys_len);
//        for (int i = 0; mountpoints[i] != NULL; i += 2) {
//            /* Ignore failures from this call, since bogus entries can appear in the guest's /etc/fstab.*/
//            guestfs_mount_ro (gfs, mountpoints[i+1], mountpoints[i]);
//            free (mountpoints[i]);
//            free (mountpoints[i+1]);
//        }
//        free (mountpoints);
//        free (root);
//    }
//    free(roots);
//
//    return 0;
//}
//
//int read_file(guestfs_h* gfs, char const* remote_file_path)
//{
//    size_t fsize;
//    char* file_content = guestfs_read_file(gfs, remote_file_path, &fsize);
//    if (!file_content) {
//        printf("ERROR: read file='%s'\n", remote_file_path);
//        return 1;
//    }
//    printf("file's content: %s\n", file_content);
//    free(file_content);
//
//    return 0;
//}
//
//int main(int argc, char* argv[])
//{
//#ifdef __CYGWIN_ROOT__
//    printf("CYGWIN ROOT is defined!\n");
//#endif
//    if (argc < 2) {
//        printf("Few arguments!\n");
//        exit(EXIT_SUCCESS);
//    }
//    if (strcmp(argv[1], "-in") == 0) {
//        printf("AF_INET\n");
//        create_socket_in();
//        sleep(100);
//        return 0;
//    }
//    else if (strcmp(argv[1], "-un") == 0) {
//        printf("AF_UNIX\n");
//        create_socket_un();
//        sleep(100);
//        return 0;
//    }
//    else if (strcmp(argv[1], "-g") != 0) {
//        printf("Unknown option!\n");
//        exit(EXIT_SUCCESS);
//    }
//
//    printf("guestfs_create()...\n");
//    guestfs_h* gfs = guestfs_create();
//
//    if (gfs == NULL) {
//        printf("guestfs handle is NULL\n");
//        exit(EXIT_FAILURE);
//    }
//
//    printf("guestfs_set_verbose()...\n");
//    if (guestfs_set_verbose(gfs, 1) == -1) {
//        printf("ERROR occurred during guestfs_set_verbose()\n");
//        exit(EXIT_FAILURE);
//    }
//
//    printf("guestfs_get_backend()...\n");
//    char* default_backend = guestfs_get_backend(gfs);
//    if (strcmp(default_backend, "direct")) {
//        printf("guestfs_set_backend()...\n");
//        if (guestfs_set_backend(gfs, "direct") == -1) {
//            printf("ERROR occurred during guestfs_set_backend()\n");
//            exit(EXIT_FAILURE);
//        }
//    }
//    printf("backend is direct\n");
//
//    const char* disk_image_path = "/home/novokrestWin/Master/guest_fs/test_data/appliance.d/disk.img";
//    if (!check_file_exists(disk_image_path)) {
//        printf("disk_image doesn't exist: %s\n", disk_image_path);
//        exit(EXIT_FAILURE);
//    }
//
//    if (guestfs_add_drive_opts(gfs, disk_image_path,
//        GUESTFS_ADD_DRIVE_OPTS_FORMAT, "raw",
//        GUESTFS_ADD_DRIVE_OPTS_READONLY, 0,
//        -1) == -1) {
//        printf("ERROR occurred during guestfs_add_drive_opts\n");
//        exit(EXIT_FAILURE);
//    }
//
//    printf("guestfs_launch()...\n");
//    if (guestfs_launch(gfs) == -1) {
//        printf("ERROR occurred during guestfs_launch()\n");
//        exit(EXIT_FAILURE);
//    }
//
//    if (mount_disks(gfs) == 0) {
//        printf("read_file()...\n");
//        read_file(gfs, "/home/novokrestdeb/123.txt");
//    }
//
//    printf("guestfs_close()...\n");
//    guestfs_close(gfs);
//
//    return 0;
//}