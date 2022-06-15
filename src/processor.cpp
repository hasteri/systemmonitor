#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float utilization;
    long activeJiffies = LinuxParser::ActiveJiffies();
    long jiffies = LinuxParser::Jiffies();
    utilization = float(activeJiffies) / float(jiffies);
    return utilization;
}