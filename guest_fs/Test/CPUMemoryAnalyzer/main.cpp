#include "testguestfs.h"
#include "procssnapshot.h"
#include "memsnapshot.h"
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

int main(int argc, char* argv[])
{
    GuestFSBackend backend = QEMU_KVM;
    bool keep_history = false;

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (!strcmp(argv[i], "-uml")) {
                backend = UML;
            }
            else if (!strcmp(argv[i], "-v")) {
                keep_history = true;
            }
            else if (!strcmp(argv[i], "-l")) {
                LOG_FILE_NAME = argv[i + 1];
                ++i;
            }
        }
    }

    mem_usage mem_log;
    procs_snapshot snap1, snap2, snap3;
    procs_snapshot_diff diff_12, diff_13;

    if (keep_history) {
        mem_log.make_snapshot();
        snap1.create();
    }

    TestGuestFS test(TEST_DIR_PATH, REMOTE_TEST_DIR_PATH, BIG_FILES_DIR_NAME, SMALL_FILES_DIR_NAME);
    test.createContext(backend, UML_KERNEL_NAME, DISK_IMAGE_NAME);

    if (keep_history) {
        mem_log.make_snapshot();
        snap2.create();
    }

    test.start();

    if (keep_history) {
        mem_log.make_snapshot();
        snap3.create();
    }

    test.closeContext();
    test.writeLog(LOG_FILE_NAME);

    string LOG_FILE_PATH = string(TEST_DIR_PATH) + "/" + LOG_FILE_NAME;

    if (keep_history) {
        mem_log.write_max_usage(LOG_FILE_PATH);

        snap1.compare_with(snap2, diff_12);
        snap1.compare_with(snap3, diff_13);
        diff_12.write_report(LOG_FILE_PATH);
        diff_13.write_report(LOG_FILE_PATH);
    }

    ofstream fout(LOG_FILE_PATH, std::ofstream::app);
    fout << "********************************************" << endl << endl;

    return 0;
}

