#include "testguestfs.h"
#include "cpu_memory_analyzer_utils.h"
#include <guestfs.h>

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

TestGuestFS::TestGuestFS(string const& testDirPath,
                         string const& remoteTestDirPath,
                         string const& bigFilesDirName,
                         string const& smallFilesDirName)
    : testDirPath_(testDirPath)
    , remoteTestDirPath_(remoteTestDirPath)
    , bigFilesDirName_(bigFilesDirName)
    , smallFilesDirName_(smallFilesDirName)
{
    logger_.start();
}

bool TestGuestFS::createContext(GuestFSBackend backend, string const& hvPath, string const& diskimagePath)
{
    diskImagePath_ = testDirPath_ + "/" + diskimagePath;
    hvPath_ = testDirPath_ + "/" + hvPath;

    gfs_ = guestfs_create();
    if (!gfs_) {
        logger_.log("ERROR: failed to create guestfs handle");
        return false;
    }

    guestfs_set_verbose(gfs_, 1);

    switch(backend)
    {
    case QEMU:
        /**
         * export LIBGUESTFS_BACKEND_SETTINGS=force_tcg;
        */
        logger_.log("ERROR: failed to create QEMU without KVM as backend");
        return false;
        break;

    case QEMU_KVM:
        /**
         * run as default
        */
        logger_.log("BACKEND DEFAULT SET");
        break;

    case UML:
        guestfs_set_backend(gfs_, "uml");
        if (guestfs_set_hv(gfs_, hvPath_.c_str())) {
            logger_.log("ERROR: failed to set UML as backend");
            return false;
        }
        logger_.log("BACKEND UML SET");
        break;

    default:
        return false;
    }
    backend_ = backend;

    if (guestfs_add_drive_opts(gfs_, diskImagePath_.c_str(),
                               GUESTFS_ADD_DRIVE_OPTS_FORMAT, "raw",
                               GUESTFS_ADD_DRIVE_OPTS_READONLY, 0,
                               -1)) {
        logger_.log("ERROR: failed to attach disk image");
        return false;
    }
    logger_.log("DISK IMAGE ATTACHED");

    logger_.log("START LAUNCHING BACKEND");
    if (guestfs_launch(gfs_)) {
        logger_.log("ERROR: failed to launch libguestfs backend");
        return false;
    }
    logger_.log("END LAUNCHING BACKEND");

    mount_up_disk();

    return true;
}

bool TestGuestFS::mount_up_disk()
{
    char **roots, *root, **mountpoints;

    logger_.log("START INSPECT DISKIMAGE");
    roots = guestfs_inspect_os(gfs_);
    logger_.log("END INSPECT DISKIMAGE");
    if (roots == NULL || roots[0] == NULL) {
        logger_.log("ERROR: failed to inspect disk img");
        return false;
    }

    for (int j = 0; roots[j] != NULL; ++j) {
        root = roots[j];

        mountpoints = guestfs_inspect_get_mountpoints(gfs_, root);
        if (mountpoints == NULL) {
            logger_.log("ERROR: failed to inspect mountpoints for root: " + string(root));
            return false;
        }

        qsort(mountpoints, count_strings(mountpoints) / 2, 2 * sizeof(char*), compare_keys_len);
        for (int i = 0; mountpoints[i] != NULL; i += 2) {
            /* Ignore failures from this call, since bogus entries can appear in the guest's /etc/fstab.*/
            guestfs_mount_ro (gfs_, mountpoints[i+1], mountpoints[i]);
            free (mountpoints[i]);
            free (mountpoints[i+1]);
        }
        free (mountpoints);
        free (root);
    }
    free(roots);

    return true;
}

void TestGuestFS::start()
{
    string testBigFilesDirPath = testDirPath_ + "/" + bigFilesDirName_;
    string remoteTestBigFilesDirPath = remoteTestDirPath_ + "/" + bigFilesDirName_;
    string testSmallFilesDirPath = testDirPath_ + "/" + smallFilesDirName_;
    string remoteTestSmallFilesDirPath = remoteTestDirPath_ + "/" + smallFilesDirName_;

    logger_.log("START: BIG FILES TEST");
//    writeAll(testBigFilesDirPath, remoteTestBigFilesDirPath);
    readAll(remoteTestBigFilesDirPath, testBigFilesDirPath);
    logger_.log("END: BIG FILES TEST");

    logger_.log("START: SMALL FILES TEST");
//    writeAll(testSmallFilesDirPath, remoteTestSmallFilesDirPath);
    readAll(remoteTestSmallFilesDirPath, testSmallFilesDirPath);
    logger_.log("END: SMALL FILES TEST");
}

void TestGuestFS::readAll(string const& remoteDirPath, string const& sourceDirPath)
{
    vector<dirent> entries;
    list_dir(sourceDirPath.c_str(), entries);
    for (vector<dirent>::const_iterator ent_it = entries.begin(); ent_it != entries.end(); ++ent_it) {
        readFile(remoteDirPath + "/" + ent_it->d_name, sourceDirPath + "/" + ent_it->d_name);
    }
}

void TestGuestFS::writeAll(string const& sourceDirPath, string const& remoteDirPath)
{
    vector<dirent> entries;
    list_dir(sourceDirPath.c_str(), entries);
    for (vector<dirent>::const_iterator ent_it = entries.begin(); ent_it != entries.end(); ++ent_it) {
        writeFile(sourceDirPath + "/" + ent_it->d_name, remoteDirPath + "/" + ent_it->d_name);
    }
}

void TestGuestFS::readFile(string const& remoteFilePath, string const& sourceFilePath)
{
    size_t fsize;
    char* file_content = guestfs_read_file(gfs_, remoteFilePath.c_str(), &fsize);
    delete(file_content);
//    bool status = checkFileContent(sourceFilePath, file_content);
//    if (status) {
//        logger_.log("OK");
//    }
}

bool TestGuestFS::checkFileContent(const string &sourceFilePath, const char *fileContent)
{
    ifstream fin(sourceFilePath.c_str());
    string sourceFileContent((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

    return !std::strcmp(sourceFileContent.c_str(), fileContent);
}

void TestGuestFS::writeFile(const string& sourceFilePath, const string& remoteFilePath)
{
    ifstream fin(sourceFilePath.c_str());
    string source_file_content((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

    int status = guestfs_write(gfs_, remoteFilePath.c_str(), source_file_content.c_str(), source_file_content.size());
    if (!status) {
        logger_.log("ERORR: couldn't write file to guest_fs, filename=\'" + sourceFilePath + "\'");
    }
}

bool TestGuestFS::closeContext()
{
    if (guestfs_umount_all (gfs_) == -1) {
        logger_.log("ERROR: failed to unmount");
        exit (EXIT_FAILURE);
    }

    logger_.log("START: CLOSE GUESTFS");
    if (guestfs_shutdown (gfs_) == -1) {
       /* handle write errors here */
     }
     guestfs_close (gfs_);
     logger_.log("END: CLOSE GUESTFS");

     return true;
}

void TestGuestFS::writeLog(const string &logName)
{
    logger_.writeLogToFile(testDirPath_ + "/" + logName);
}

