#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int _pid){
    pid = _pid;
    uptime = LinuxParser::UpTime() - LinuxParser::UpTime(_pid);    
    std::string ramS = LinuxParser::Ram(_pid);
    ram = std::stoi(ramS); 
    user = LinuxParser::User(_pid);
    command = LinuxParser::Command(_pid);
    cpuUtilization = (float(LinuxParser::ActiveJiffies(_pid)) / LinuxParser::Jiffies())*100;
}

// TODO: Return this process's ID
int Process::Pid()  { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization()  { return cpuUtilization; }

// TODO: Return the command that generated this process
string Process::Command()  { return command; }

// TODO: Return this process's memory utilization
string Process::Ram()  { return std::to_string(ram/1000); }

// TODO: Return the user (name) that generated this process
string Process::User()  { return user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime()  { return uptime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return cpuUtilization < a.cpuUtilization; 
}