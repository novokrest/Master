#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>

#include <sys/stat.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <unistd.h>

#include <guestfs.h>

using std::cout;
using std::endl;
using std::string;


bool check_file_exists(const char* filename)
{
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}

void print(string const& str)
{
    std::cout << str << std::endl;
}

int create_socket_un()
{
    struct sockaddr_un addr;
    int accept_socket;
    string guestfsd_sock;

    accept_socket = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (accept_socket == -1) {
        printf("ERROR: couldn't create socket");
        exit(EXIT_FAILURE);
    }

    addr.sun_family = AF_UNIX;
    guestfsd_sock = "/tmp/guestfsd_sock";
    strncpy(addr.sun_path, guestfsd_sock.c_str(), sizeof(addr.sun_path));
    addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';

    int size = (offsetof(struct sockaddr_un, sun_path) + strlen(addr.sun_path));
    if (bind(accept_socket, (struct sockaddr *) &addr, size) == -1) {
        printf("ERROR: bind socket");
        exit(EXIT_FAILURE);
    }

    if (listen(accept_socket, 1) == -1) {
        printf("ERROR: listen socket");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int create_socket_in()
{
    struct sockaddr_in addr, cli_addr;
    int accept_socket, newsock;
    string guestfsd_sock;

    accept_socket = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (accept_socket == -1) {
        printf("ERROR: couldn't create socket\n");
        exit(EXIT_FAILURE);
    }

    bzero((char*)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(7777);

    if (bind(accept_socket, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1) {
        printf("ERROR: bind socket\n");
        exit(EXIT_FAILURE);
    }

    if (listen(accept_socket, 1) == -1) {
        printf("ERROR: listen socket\n");
        exit(EXIT_FAILURE);
    }

    bzero((char*)&cli_addr, sizeof(cli_addr));
    int clilen = sizeof(cli_addr);

    printf("Ready to accept\n");
    newsock = accept(accept_socket, (struct sockaddr*)&cli_addr, &clilen);
    printf("After accept\n");

    if (newsock < 0) {
        printf("ERROR: accept()\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int main()
{
    create_socket_in();
    sleep(100);
    return 0;

    printf("guestfs_create()...\n");
    guestfs_h* gfs = guestfs_create();

    if (gfs == NULL) {
        printf("guestfs handle is NULL\n");
        exit(EXIT_FAILURE);
    }

    printf("guestfs_set_verbose()...\n");
    if (guestfs_set_verbose(gfs, 1) == -1) {
        printf("ERROR occurred during guestfs_set_verbose()\n");
        exit(EXIT_FAILURE);
    }

    printf("guestfs_get_backend()...\n");
    char* default_backend = guestfs_get_backend(gfs);
    if (strcmp(default_backend, "direct")) {
        printf("guestfs_set_backend()...\n");
        if (guestfs_set_backend(gfs, "direct") == -1) {
            printf("ERROR occurred during guestfs_set_backend()\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("backend is direct\n");

    const char* disk_image_path = "/home/novokrestWin/Master/guest_fs/test_data/disk.img";
    if (!check_file_exists(disk_image_path)) {
        printf("disk_image doesn't exist: %s\n", disk_image_path);
        exit(EXIT_FAILURE);
    }

    if (guestfs_add_drive_opts(gfs, disk_image_path,
        GUESTFS_ADD_DRIVE_OPTS_FORMAT, "raw",
        GUESTFS_ADD_DRIVE_OPTS_READONLY, 0,
        -1) == -1) {
        printf("ERROR occurred during guestfs_add_drive_opts\n");
        exit(EXIT_FAILURE);
    }

    printf("guestfs_launch()...");
    if (guestfs_launch(gfs) == -1) {
        printf("ERROR occurred during guestfs_launch()\n");
        exit(EXIT_FAILURE);
    }

    printf("guestfs_close()...\n");
    guestfs_close(gfs);

    return 0;
}