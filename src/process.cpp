#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stof;

// DONE-CK: Return this process's ID
int Process::Pid() { return pid_; }

// DONE-CK: Return this process's CPU utilization
float Process::CpuUtilization() { 
    vector<string> cpu_use_vec = LinuxParser::CpuUtilization(pid_);
    //return stof(cpu_use_vec[4]);

    // algorithm from https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    float utime = stof(cpu_use_vec[0]);
    float stime = stof(cpu_use_vec[1]);
    float cutime = stof(cpu_use_vec[2]);
    float cstime = stof(cpu_use_vec[3]);
    float starttime = stof(cpu_use_vec[4]);

    float uptime = LinuxParser::UpTime();

    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
    cpu_use_ = ((total_time / sysconf(_SC_CLK_TCK)) / seconds);

    // for testing:
    //cpu_use_ =  static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return cpu_use_; 
}

// DONE-CK: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_);
}

// DONE-CK: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(pid_);
}

// DONE-CK: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(pid_); 
}

// DONE-CK: Return the age of this process (in seconds)
long int Process::UpTime() { 
    uptime_ = LinuxParser::UpTime(pid_);
    return uptime_;
}

float Process::getCpuUse() const {
    return cpu_use_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return cpu_use_ < a.getCpuUse();
}