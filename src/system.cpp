#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// DONE-CK: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE-MAYBE-CK: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    // maybe this can both assign values to processes_ and return it
    // get PIDs from LinuxParser
    std::vector<int> processes = LinuxParser::Pids();
    processes_ = {}; // could maybe replace processes that have changed, but that sounds cumbersome
    // but can the same process ID be re-used later on? yes it can.
    for (int pid : processes) {
        Process process(pid);
        processes_.push_back(process);
    }

    // sort this according to cpu use
    sort(processes_.begin(), processes_.end(), std::less<Process>());
    
    return processes_; 
}

// DONE-CK: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel();
}

// DONE-CK: Return the system's memory utilization
float System::MemoryUtilization() {
     return LinuxParser::MemoryUtilization(); 
}

// DONE-CK: Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem();
}

// DONE-CK: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses();
 }

// DONE-CK: Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
}

// DONE-CK: Return the number of seconds since the system started running
long System::UpTime() { 
    return LinuxParser::UpTime(); 
}