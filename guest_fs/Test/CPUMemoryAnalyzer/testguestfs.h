#ifndef TESTGUESTFS_H
#define TESTGUESTFS_H

#include "common.h"
#include "timelogger.h"
#include <guestfs.h>

enum GuestFSBackend
{
    QEMU, QEMU_KVM, UML
};

class TestGuestFS
{
    TimeLogger logger_;

    GuestFSBackend backend_;
    string hvPath_;
    string diskImagePath_;

    string testDirPath_;
    string remoteTestDirPath_;
    string bigFilesDirName_;
    string smallFilesDirName_;

    guestfs_h* gfs_;

    bool mount_up_disk();

public:
    TestGuestFS(string const& testDirPath, string const& remoteTestDirPath, string const& bigFilesDirName, string const& smallFilesDirName);

    bool createContext(GuestFSBackend backend, string const& hvPath, string const& diskimagePath);
    bool closeContext();

    void start();

    bool checkFileContent(string const& filename, const char* fileContent);

    void writeFile(const string& sourceFilePath, const string& remoteFilePath);
    void writeAll(string const& sourceDirPath, string const& remoteDirPath);

    void readFile(string const& remoteFilePath, string const& sourceFilePath);
    void readAll(string const& remoteFileDir, string const& sourceFileDir);

    void writeLog(const string & logName);
};

#endif // TESTGUESTFS_H
