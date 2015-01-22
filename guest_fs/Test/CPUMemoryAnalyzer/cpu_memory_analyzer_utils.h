#ifndef CPU_MEMORY_ANALYZER_UTILS_H
#define CPU_MEMORY_ANALYZER_UTILS_H

#include "common.h"
#include <dirent.h>
#include <proc/readproc.h>
#include <sys/stat.h>

void print_proc_info(proc_t const& proc);
void print_procs(vector<proc_t> const& procs);

void list_dir(const char* dir_path, vector<dirent>& entries);
void check_file_exist(const char* path);

#endif // CPU_MEMORY_ANALYZER_UTILS_H
