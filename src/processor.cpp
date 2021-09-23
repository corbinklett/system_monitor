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
    using LinuxParser::CPUStates;

    // get value from LinuxParser
    vector<string> cpuvec = LinuxParser::CpuUtilization();

    if (cpuvec.size() == 10) {
    // this strategy comes from 
    // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
   
    // loop over LinuxParser::CPUStates
    unordered_map <LinuxParser::CPUStates, float> states;
    int i = 0;
    for (LinuxParser::CPUStates s = LinuxParser::CPUStates::kUser_; s = LinuxParser::CPUStates::kGuestNice_; s++ ) {
        states[s] = stof(cpuvec[i]);
        i++;
    }

    // compute the cpu state
    // kUser,  kNice_, kSystem_, kIdle_, kIOwait_, kIRQ_, kSoftIRQ_, kSteal_, kGuest_,kGuestNice_

    float prevIdle = idle_ + prev_iowait_;
    idle_ = states[kIdle]
    float Idle = states[kIdle] + states[kIOwait];

    PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
    float NonIdle = states[kUser] + states[kNice] + states[kSystem] + states[kIRQ] + states[kSoftIRQ] + states[kSteal];

    PrevTotal = PrevIdle + PrevNonIdle
    float Total = Idle + NonIdle;

    # differentiate: actual value minus the previous one
    totald = Total - PrevTotal
    idled = Idle - PrevIdle

    //CPU_Percentage = 
    return (totald - idled)/totald;

    } else {
        return 0.0;
    }
}