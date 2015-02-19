#include <cstdlib>
#include <iostream>

#include <guestfs.h>

int main()
{
    std::cout << "guestfs_create()..." << std::endl;
    guestfs_h* gfs = guestfs_create();

    if (gfs == NULL) {
        std::cout << "gfs handle is NULL" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "guestfs_set_backend()..." << std::endl;
    guestfs_set_backend(gfs, "direct");

    std::cout << "guestfs_get_backend()..." << std::endl;
    char* default_backend = guestfs_get_backend(gfs);
    std::cout << "default backend is " << default_backend << std::endl;

    std::cout << "guestfs_close()..." << std::endl;
    guestfs_close(gfs);

    return 0;
}