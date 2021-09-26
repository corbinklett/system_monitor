#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;
using std::stof;
using std::unordered_map;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    using namespace LinuxParser;

    // get value from LinuxParser
    vector<string> cpuvec = CpuUtilization();

    if (cpuvec.size() == 10) {
    // this strategy comes from 
    // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
   
    // loop over LinuxParser::CPUStates
    unordered_map <CPUStates, float> states;
    CPUStates state;
    for (int i = CPUStates::kUser_; i <= CPUStates::kGuestNice_; i++ ) {
        state = static_cast<CPUStates>(i);
        states[state] = stof(cpuvec[i]);
    }

    // int i = CPUStates::kUser_;
    // state = static_cast<CPUStates>(i);
    // states[state] = stof(cpuvec[i]);

    // i = 1;
    // state = static_cast<CPUStates>(i);
    // states[state] = stof(cpuvec[i]);


    // compute the cpu state

    float prev_idle = idle_; 
    idle_ = states[CPUStates::kIdle_] + states[CPUStates::kIOwait_];

    non_idle_ = states[CPUStates::kUser_] + states[CPUStates::kNice_] + states[CPUStates::kSystem_] 
        + states[CPUStates::kIRQ_] + states[CPUStates::kSoftIRQ_] + states[CPUStates::kSteal_];

    float prev_total = total_; 
    total_ = idle_ + non_idle_;

    // differentiate: actual value minus the previous one
    float delta_total = total_ - prev_total;
    float delta_idle = idle_ - prev_idle;

    //CPU_Percentage = 
    return (delta_total - delta_idle)/delta_total;

    } else {
        return 0.0;
    }
}