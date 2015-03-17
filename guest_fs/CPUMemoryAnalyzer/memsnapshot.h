#ifndef MEMSNAPSHOT_H
#define MEMSNAPSHOT_H

#include "common.h"
#include "timelogger.h"

class mem_usage
{
    TimeLogger logger_;
    vector<struct sysinfo> infos_;

public:
    mem_usage();

    void make_snapshot();
    int max_usage();

    void write_max_usage(string const& filename);
};

#endif // MEMSNAPSHOT_H
