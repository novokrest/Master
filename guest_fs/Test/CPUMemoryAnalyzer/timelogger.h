#ifndef TIMELOGGER_H
#define TIMELOGGER_H

#include "common.h"

class TimeLogger
{
    system_clock::time_point tpStart_;

    vector< pair<system_clock::time_point, string> > tpMessages_;

public:
    void start();
    void log(string const& message);
    void writeLogToFile(string const& filename);
};

#endif // TIMELOGGER_H
