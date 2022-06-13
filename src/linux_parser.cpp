#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float memory = 0.0;
  float memTotal, memFree;

  string line;
  string key;
  string value;
  std::ifstream memoryStream(kProcDirectory + kMeminfoFilename);
  if (memoryStream.is_open()) {
    while (std::getline(memoryStream, line)) {
      std::istringstream linestream(line);  
      linestream >> key >> value;    
      if (key == "MemTotal") {
        memTotal = std::stoi(value);
      }   
      if (key == "MemFree") {
        memFree = std::stoi(value);
      } 
    }
  }
  memory = (memTotal - memFree) / memTotal;

  return memory; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, upTimeString;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> upTimeString){        
        return std::stoi(upTimeString);                 
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  vector<string> values{};

  string line;
  string key;


  std::ifstream processesStream(kProcDirectory + kStatFilename);
  if (processesStream.is_open()) {
    while (std::getline(processesStream, line)) {
      std::istringstream linestream(line);  
      linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;    
      if (key == "cpu") {
        values.push_back(user);
        values.push_back(nice);
        values.push_back(system);
        values.push_back(idle);
        values.push_back(iowait);
        values.push_back(irq);
        values.push_back(softirq);
        values.push_back(steal);
        values.push_back(guest);
        values.push_back(guest_nice);
        return values;
      }   
    }
  }

  return {}; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  float totalProcesses;

  string line;
  string key;
  string value;
  std::ifstream processesStream(kProcDirectory + kStatFilename);
  if (processesStream.is_open()) {
    while (std::getline(processesStream, line)) {
      std::istringstream linestream(line);  
      linestream >> key >> value;    
      if (key == "processes") {
        totalProcesses = std::stoi(value);
      }   
    }
  }
  return totalProcesses;  
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  float runningProcesses;

  string line;
  string key;
  string value;
  std::ifstream processesStream(kProcDirectory + kStatFilename);
  if (processesStream.is_open()) {
    while (std::getline(processesStream, line)) {
      std::istringstream linestream(line);  
      linestream >> key >> value;    
      if (key == "procs_running") {
        runningProcesses = std::stoi(value);
      }   
    }
  }
  return runningProcesses;  
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string commandValue;

  string key;
  string value;

  std::ifstream commandStream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if (commandStream.is_open()){
    std::getline(commandStream, commandValue);
  }

  return commandValue; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string uidValue = "";

  string key;
  string value;
  string line;

  std::ifstream userStream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if (userStream.is_open()){
    while (std::getline(userStream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        userStream >> key >> value;
        if(key == "Uid")
          uidValue = value;
      }
    }
  }
  return uidValue; 
  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {

  string uid = Uid(pid);

  string userValue = "";

  string key;
  string value;
  string line;

  std::ifstream userStream(kPasswordPath);
  if (userStream.is_open()){
    while (std::getline(userStream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        userStream >> key >> value;
        if(key == uid)
          userValue = value;
      }
    }
  }
  return userValue; 
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long uptimeValue;

  string key;
  string value;
  string line;

  std::ifstream uptimeStream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if (uptimeStream.is_open()){
    while (std::getline(uptimeStream, line)){
      for (int i=1; i<=22; i++){ // reference: value 22 from: https://man7.org/linux/man-pages/man5/proc.5.html
        uptimeStream >> value;
        if (i == 22)
          uptimeValue = std::stoi(value) / sysconf(_SC_CLK_TCK);
      }
    }
  }
  return uptimeValue; 
}
