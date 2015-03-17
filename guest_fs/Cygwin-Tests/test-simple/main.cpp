#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>

#include <sys/stat.h>
#include <time.h>

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

void test_guestfs(void);

int create_socket_un()
{
    struct sockaddr_un addr;
    int accept_socket, newsock;
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

    printf("Ready to accept\n");
    newsock = accept4(accept_socket, NULL, NULL, SOCK_CLOEXEC);
    printf("After accept\n");

    if (newsock < 0) {
        printf("ERROR: accept()\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int create_socket_in()
{
    struct sockaddr_in addr, cli_addr;
    int accept_socket, newsock;
    string guestfsd_sock;

    accept_socket = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
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
    newsock = accept(accept_socket, (struct sockaddr*)&cli_addr, (socklen_t*)&clilen);
    printf("After accept\n");

    if (newsock < 0) {
        printf("ERROR: accept()\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int main(int argc, char* argv[])
{
#ifdef __CYGWIN_ROOT__
    printf("CYGWIN ROOT is defined!\n");
#endif
    if (argc < 2) {
        printf("Few arguments!\n");
        exit(EXIT_SUCCESS);
    }
    if (strcmp(argv[1], "-in") == 0) {
        printf("AF_INET\n");
        create_socket_in();
        sleep(100);
        return 0;
    }
    else if (strcmp(argv[1], "-un") == 0) {
        printf("AF_UNIX\n");
        create_socket_un();
        sleep(100);
        return 0;
    }
    else if (strcmp(argv[1], "-g") != 0) {
        printf("Unknown option!\n");
        exit(EXIT_SUCCESS);
    }

    test_guestfs();

    return 0;
}
