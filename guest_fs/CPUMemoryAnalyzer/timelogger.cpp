#include "timelogger.h"

TimeLogger::TimeLogger()
{
    start();
}

void TimeLogger::start()
{
    tpMessages_.clear();
    tpStart_ = system_clock::now();
}

void TimeLogger::log(const string &message)
{
    system_clock::time_point now = system_clock::now();
    tpMessages_.push_back(pair<system_clock::time_point, string>(now, message));
}

void TimeLogger::writeLogToFile(const string &filename)
{
    ofstream fLog(filename.c_str(), std::ofstream::app);
    fLog << "========================" << endl;
    for (vector< pair<system_clock::time_point, string> >::const_iterator tpMessageIt = tpMessages_.begin();
         tpMessageIt != tpMessages_.end(); ++tpMessageIt) {
        fLog << "[ "
             << std::chrono::duration_cast<milliseconds>(tpMessageIt->first - tpStart_).count()
             << " ms ] : "
             << tpMessageIt->second
             << endl;
    }
    fLog << "========================" << endl;
    fLog << endl;
}
