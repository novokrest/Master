#ifndef PROCSSNAPSHOT_H
#define PROCSSNAPSHOT_H

#include "common.h"
#include "logger.h"
#include <proc/readproc.h>

class procs_snapshot_diff
{
    unsigned long total_memory(vector<proc_t> const& procs) const;

public:
    vector<proc_t> removed_procs;
    vector<proc_t> same_procs;
    vector<proc_t> new_procs;

    void add_removed_proc(proc_t const& proc);
    void add_same_proc(proc_t const& proc);
    void add_new_proc(proc_t const& proc);

    unsigned long total_memory_new() const;

    void write_report(string const& filename);
};

class procs_snapshot
{
    vector<proc_t> procs_;

    logger logger_;

public:
    void create();
    void differ_from(procs_snapshot const& other_snapshot, vector<proc_t>& differ_procs) const;
    void compare_with(procs_snapshot const& other_snapshot,
                      procs_snapshot_diff& diff) const;

    void print() const;
};

#endif // PROCSSNAPSHOT_H
