#ifndef COMMON_H
#define COMMON_H

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <iomanip>
using std::setw;

#include <cstring>
#include <string>
using std::string;

#include <sys/time.h>
#include <chrono>
using std::chrono::system_clock;

#include <algorithm>
using std::pair;
using std::qsort;

#include <cassert>
#include <memory.h>

typedef std::chrono::system_clock c_clock;
typedef std::chrono::system_clock::time_point time_point;
typedef std::chrono::milliseconds milliseconds;

#endif // COMMON_H
