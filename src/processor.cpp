#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;
using std::stof;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    // get value from LinuxParser
    vector<string> cpuvec = LinuxParser::CpuUtilization();

    if (cpuvec.size() == 10) {
    // this strategy comes from 
    // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
   
    // loop over LinuxParser::CPUStates
    float user = stof(cpuvec[0]);
    float nice = stof(cpuvec[1]);
    float system = stof(cpuvec[2]);
    float idle = stof(cpuvec[3]);
    float iowait = stof(cpuvec[4]);
    float irq = stof(cpuvec[5]);
    float softirq = stof(cpuvec[6]);
    float iowait = stof(cpuvec[7]);
    float irq = stof(cpuvec[7]);
    float softirq = stof(cpuvec[9]);


    revIdle = previdle + previowait
    Idle = idle + iowait

    PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
    NonIdle = user + nice + system + irq + softirq + steal

    PrevTotal = PrevIdle + PrevNonIdle
    Total = Idle + NonIdle

    # differentiate: actual value minus the previous one
    totald = Total - PrevTotal
    idled = Idle - PrevIdle

    CPU_Percentage = (totald - idled)/totald


    string cpu_string = cpu_use_vec[0];

    float prev_used = used_;
    used_ = stof(cpu_string);
    } else {
        return 0.0;
    }
}