#include "cpu_memory_analyzer_utils.h"

void print_proc_info(const proc_t &proc)
{
    cout << "tid: " << setw(7) << proc.tid
         << setw(10)
         << "pid: " << setw(7) << proc.tgid
         << setw(20)
         << "memory(KiB): " << setw(9) << proc.vm_size
         << "cmd: " << proc.cmd
         << endl;
}

void print_procs(const vector<proc_t> &procs)
{
    for (vector<proc_t>::const_iterator proc_it = procs.begin(); proc_it != procs.end(); ++proc_it) {
        print_proc_info(*proc_it);
    }
}

void list_dir(const char *dir_path, vector<dirent> &entries)
{
    DIR* dir = opendir(dir_path);
    if (dir) {
        struct dirent* ent;
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
            entries.push_back(*ent);
        }
        closedir(dir);
    }
}

void check_file_exist(const char* path)
{
    struct stat buffer;
    assert(stat(path, &buffer) == 0);
}

