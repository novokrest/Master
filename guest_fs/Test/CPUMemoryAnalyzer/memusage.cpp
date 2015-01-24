#include "memsnapshot.h"

mem_usage::mem_usage()
{
    make_snapshot();
}

void mem_usage::make_snapshot()
{
    struct sysinfo info;
    int err = sysinfo(&info);
    if (err) {
        logger_.log(strerror(errno));
        return;
    }

    infos_.push_back(info);
}

/**
 * max difference of free memory from first snapshot
*/
int mem_usage::max_usage()
{
    int max_usage = -1;

    if (infos_.size() < 2) {
        return -1;
    }

    for (vector<struct sysinfo>::const_iterator info_it = infos_.begin() + 1; info_it != infos_.end(); ++info_it) {
        int diff = infos_[0].freeram - info_it->freeram;
        max_usage = diff > max_usage ? diff : max_usage;
    }

    return max_usage;
}

void mem_usage::write_max_usage(const string &filename)
{
    string memory_usage_message = "MEMORY_USAGE: " + std::to_string(max_usage() / 1024) + " KiB";
    logger_.log(memory_usage_message);
    logger_.writeLogToFile(filename);
}
