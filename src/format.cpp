#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hr = seconds / 3600;
    int min = (seconds % 3600) / 60;
    seconds = (seconds % 3600) % 60;   

    string hr_string = std::to_string(hr);
    string min_string = std::to_string(min);
    string sec_string = std::to_string(seconds);
    
    if (hr < 10) {
        hr_string = "0" + hr_string;
    }
    if (min < 10) {
        min_string = "0" + min_string;
    }
    if (seconds < 10) {
        sec_string = "0" + sec_string;
    }
        
    return hr_string + ":" + min_string + ":" + sec_string;
}