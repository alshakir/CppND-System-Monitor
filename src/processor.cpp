#include "processor.h"
#include "linux_parser.h"
#include <iostream>
using namespace LinuxParser;
using namespace std;
using std::stof;
using std::stod;


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
   vector<string> f = CpuUtilization();
   std::stringstream s (f[0]);
    
    string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

    s >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
   /*
   I used the folliwng link 
   
   https://github.com/Leo-G/DevopsWiki/wiki/How-Linux-CPU-Usage-Time-and-Percentage-is-calculated

    Formula

    To calculate Linux CPU usage time subtract the idle CPU time from the total CPU time as follows:

    Total CPU time since boot = user+nice+system+idle+iowait+irq+softirq+steal

    Total CPU Idle time since boot = idle + iowait

    Total CPU usage time since boot = Total CPU time since boot - Total CPU Idle time since boot

    Total CPU percentage = Total CPU usage time since boot/Total CPU time since boot X 100

    For real time CPU usage, you will need to calculate the time between two intervals.

    also the following link helped for the real time calculations
    https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
   */
    size_t sz;
    double d_nice = std::stod(nice,&sz);

    double d_user = std::stod(user,&sz);
    double d_system = std::stod(system,&sz);
    double d_idle = std::stod(idle,&sz);
    double d_iowait = std::stod(iowait,&sz);
    double d_irq = std::stod(irq,&sz); 
    double d_softirq = std::stod(softirq,&sz);
    double d_steal = std::stod(steal,&sz);
    // double d_guest = std::stod(guest,&sz); 
    // double d_guest_nice = std::stod(guest_nice,&sz);


    double TotalCPUTime = d_user + d_nice + d_system + d_idle + d_iowait + d_irq + d_softirq + d_steal;
    double TotalIdleTime = d_idle + d_iowait;


    double currentTotalCPUTime = TotalCPUTime - previousTotal;
    double currentTotalIdleTime = TotalIdleTime - previousIdle;

    //reset values for next cycle
    previousTotal = TotalCPUTime;
    previousIdle = TotalIdleTime;
    //ouble TotalCPUUsageSinceBoot = TotalCPUTime - TotalIdleTime;

    double totalCPUPercentageSinceBoot = (currentTotalCPUTime - currentTotalIdleTime )/currentTotalCPUTime ;
    
    
    
    
    return totalCPUPercentageSinceBoot; }