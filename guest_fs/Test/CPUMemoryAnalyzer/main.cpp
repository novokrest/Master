#include "testguestfs.h"
#include "procssnapshot.h"
#include "cpu_memory_analyzer_utils.h"

/**
   * Useful commands:
   *    CPU/Memory info: top
   *    Processes count: ps -ax | wc -l
   */

static const char* UML_KERNEL_NAME = "vmlinux";
static const char* DISK_IMAGE_NAME = "Debian-Jessie-AMD64-root_fs";
static const char* TEST_DIR_PATH = "./test";
static const char* REMOTE_TEST_DIR_PATH = "/home/test";
static const char* BIG_FILES_DIR_NAME = "big";
static const char* SMALL_FILES_DIR_NAME = "small";

static const char* LOG_FILE_NAME = "log.txt";


int main()
{
    procs_snapshot snap1, snap2, snap3;
    procs_snapshot_diff diff;

//    snap1.create();

    TestGuestFS test(TEST_DIR_PATH, REMOTE_TEST_DIR_PATH, BIG_FILES_DIR_NAME, SMALL_FILES_DIR_NAME);
    test.createContext(QEMU_KVM, UML_KERNEL_NAME, DISK_IMAGE_NAME);

//    snap2.create();

    test.start();
    test.closeContext();
    test.writeLog(LOG_FILE_NAME);

//    snap1.compare_with(snap2, diff);
//    cout << "Memory: "<< diff.total_memory_new() << endl;

    return 0;
}

