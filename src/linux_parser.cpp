#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "format.h"
#include "system.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// This function is copied from the Reviewer's suggestions:
// Intentionally not including it in the LinuxParser namespace, for some reason
template <typename T>
T findValueByKey(std::string const &keyFilter, std::string const &filename) {
  std::string line, key;
  T value;

  std::ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      //while (linestream >> key >> value) {
      linestream >> key >> value;
      if (key == keyFilter) {
        stream.close();
        return value;
      }
      //}
    }
  }
  stream.close();
  return value;
};

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE-CK: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string memtype, memamt, kb;
  float mems[2];
  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  for (int i = 0; i < 2; i++) {
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> memtype >> memamt >> kb;
    }
  mems[i] = stol(memamt); // need a float or long?
  }
  return (float)(mems[0] - mems[1]) / (float)mems[0]; 
}

// DONE-CK: Read and return the system uptime
long LinuxParser::UpTime() { 
  // get the uptime from 
  string up_time, idle_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time >> idle_time;
    return stol(up_time); 
  } else { 
    // didn't read file
    return 0;
  }
}

// DONE-CK: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string dummy;
  string line;
  vector<string> cpu_use_vec(10);
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> dummy;

    for (int i = 0; i < 10; i++) {
      linestream >> cpu_use_vec[i];
    }
  }
  return cpu_use_vec; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  // line 9 of the proc/stat file
  string numproc = findValueByKey<string>("processes", kStatFilename);
  return stoi(numproc);
}

// DONE-CK: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string numproc = findValueByKey<string>("procs_running", kStatFilename);
  return stoi(numproc);
 }

// DONE-CK: Read and return the associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string(); 
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, dummy, ram;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  if ( stream.is_open() ) {
    for (int i = 0; i < 18; i++) {
      std::getline(stream, line);
      if (i == 17) {
        std::istringstream linestream(line);
        linestream >> dummy >> ram;
        int ram_kb = stof(ram);
        int ram_mb = ram_kb / 1000;
        return to_string(ram_mb);
      }
    }
  }
  
  return string(); 
}

// DONE-CK: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, dummy, uid;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (stream.is_open()) {
    for (int i = 0; i < 9; i++) {
      std::getline(stream, line);
      if (i == 8) {
        std::istringstream linestream(line);
        linestream >> dummy >> uid;
        return uid;
      }
    }
  }
  return string(); 
}

// DONE-CK: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string line, dummy, user, uid_guess;
  std::ifstream stream(kPasswordPath);

    while ( stream.good() ) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      std::getline(linestream, user, ':');
      std::getline(linestream, dummy, ':');
      std::getline(linestream, uid_guess, ':');
      //linestream >> user >> dummy >> uid_guess;
      if (uid_guess == uid) {
        return user;
      }
  }

  return string(); 
}

// DONE-CK: Read and return the uptime of a process
// this is an overloaded function (similar function also used by System class)
long LinuxParser::UpTime(int pid) { 
  string line, val;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);

    if ( stream.is_open() ) {
      std::getline(stream, line);
        std::istringstream linestream(line);
        for (int i = 0; i < 22; i++) {
            linestream >> val;
          if (i == 21) {
             return stol(val) / sysconf(_SC_CLK_TCK); // this is long int division
          }
        }
  }
  return 0.0;
}

// ADDED-CK: Read and return cpu utilization of a process

vector<string> LinuxParser::CpuUtilization(int pid) { 
  int n = 5;
  int indices[n] = {13,14,15,16,21};
  string line, dummy;
  vector<string> cpu_use_vec(5);
  //cpu_use_vec = {"999","999","999","999","999"};
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    
    int count = 0;

    for (int i = 0; i <= indices[n-1]; i++) {
      if (i == indices[count]) {
        linestream >> cpu_use_vec[count];
        count++;
        if (count == n) {
          return cpu_use_vec;
        }
      } else {
        linestream >> dummy;
      }
    }
  } 

  return cpu_use_vec; 
  
}