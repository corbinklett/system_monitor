#include <string>
#include <vector>
#include <unordered_map>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;
using std::stof;
using std::unordered_map;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    // get value from LinuxParser
    vector<string> cpuvec = LinuxParser::CpuUtilization();

    if (cpuvec.size() == 10) {
    // this strategy comes from 
    // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
   
    // loop over LinuxParser::CPUStates
    unordered_map <LinuxParser::CPUStates, float> cpu_states;
    int i = 0;
    for (LinuxParser::CPUStates s = LinuxParser::CPUStates::kUser_; s = LinuxParser::CPUStates::kGuestNice_; s++ ) {
        cpu_states[s] = stof(cpuvec[i]);
        i++;
    }

    // compute the cpu state
      kUser,  kNice_, kSystem_, kIdle_, kIOwait_, kIRQ_, kSoftIRQ_, kSteal_, kGuest_,kGuestNice_

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