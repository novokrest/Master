#include "procssnapshot.h"
#include "cpu_memory_analyzer_utils.h"

static bool operator==(proc_t const& p1, proc_t const& p2)
{
    if (p1.tid == p2.tid) {
        assert(p1.tgid == p2.tgid);
        return true;
    }

    return false;
}

template<class T>
static void vector_left_difference(const vector<T>& v1, const vector<T>& v2, vector<T>& difference)
{
    for (typename vector<T>::const_iterator v1_it = v1.begin(); v1_it != v1.end(); ++v1_it) {
        if (std::find(v2.begin(), v2.end(), *v1_it) == v2.end()) {
            difference.push_back(*v1_it);
        }
    }
}

template<class T>
static void vector_right_difference(const vector<T>& v1, const vector<T>& v2, vector<T>& difference)
{
    vector_left_difference<T>(v2, v1, difference);
}

void procs_snapshot_diff::add_removed_proc(const proc_t &proc)
{
    removed_procs.push_back(proc);
}

void procs_snapshot_diff::add_same_proc(const proc_t &proc)
{
    same_procs.push_back(proc);
}

void procs_snapshot_diff::add_new_proc(const proc_t &proc)
{
    new_procs.push_back(proc);
}

unsigned long procs_snapshot_diff::total_memory(vector<proc_t> const& procs) const
{
    unsigned long total_mem = 0;
    for (vector<proc_t>::const_iterator proc_it = procs.begin(); proc_it != procs.end(); ++proc_it) {
        total_mem += proc_it->vm_size;
    }

    return total_mem;
}

unsigned long procs_snapshot_diff::total_memory_new() const
{
    return total_memory(new_procs);
}

void procs_snapshot::create()
{
    logger_.log("START: create processes snapshot");

    procs_.clear();
    proc_t proc_info;
    memset(&proc_info, 0, sizeof(proc_info));

    PROCTAB* hproc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);
    while(readproc(hproc, &proc_info)) {
        procs_.push_back(proc_info);
    }
    closeproc(hproc);

    logger_.log("END: make processes snapshot");
}

void procs_snapshot::compare_with(const procs_snapshot &other_snapshot,
                                  procs_snapshot_diff& diff) const
{
    logger_.log("START: compare snapshots");

    vector_left_difference<proc_t>(procs_, other_snapshot.procs_, diff.removed_procs);
    vector_right_difference<proc_t>(procs_, other_snapshot.procs_, diff.new_procs);
    vector_left_difference<proc_t>(procs_, diff.removed_procs, diff.same_procs);

    logger_.log("END: compare snapshots");
}

void procs_snapshot::differ_from(const procs_snapshot& other_snapshot, vector<proc_t>& differ_procs) const
{
    vector_left_difference<proc_t>(procs_,other_snapshot.procs_, differ_procs);
}

void procs_snapshot::print() const
{
    print_procs(procs_);
}
