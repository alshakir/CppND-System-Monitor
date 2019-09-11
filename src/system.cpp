#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include "format.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 

    auto pids = LinuxParser::Pids();

    for(int i : pids){
    string pFolder = LinuxParser::kProcDirectory + std::to_string(i);

    string ram ;
    string usr ;
    auto status = LinuxParser::ReadFile(pFolder + LinuxParser::kStatusFilename);
    if( status.size() > 0 ){
        for (string s : status){
            size_t index = s.find("VmSize");
            if(index != string::npos){
                string ramString = s.substr(8);
                int r = std::stoi(ramString);
                r /= 1000;
                ram = std::to_string(r);
            }    
            
            index = s.find("Uid");
            if(index != string::npos){
                string uid;
                std::stringstream ss(s);
                ss>> uid >> uid;

               auto users = LinuxParser::ReadFile(LinuxParser::kPasswordPath);
               if(users.size() > 0){
                   for(string s: users){
                       std::istringstream is(s);

                       string s1, s2;
                       std::getline(is,usr, ':');
                       std::getline(is,s1, ':');
                       std::getline(is,s2, ':');
                       if(s2 == uid)
                       break;
                       
                   }
               }
            }
        }
    }
    
    auto stat = LinuxParser::ReadFile(pFolder + LinuxParser::kStatFilename);
    float processor_usage;
    int processTime ;


    if(stat.size()>0){
            string line = stat[0];
            string utime, stime, cutime,cstime,starttime;
            utime = extractTokenFromString(line,13);
            stime = extractTokenFromString(line,14);
            cutime = extractTokenFromString(line,15);
            cstime = extractTokenFromString(line,16);
            starttime = extractTokenFromString(line,21);


            int i_utime = std::stoi(utime);
            int i_stime = std::stoi(stime);
            int i_cutime = std::stoi(cutime);
            int i_cstime = std::stoi(cstime);
            int i_starttime = std::stoi(starttime);

            double totalTicks = i_utime + i_stime + i_cutime + i_cstime;
            float hz = sysconf(_SC_CLK_TCK);

            processTime = i_starttime/hz;

            float uptime;
            auto uptimecpu = LinuxParser::ReadFile(LinuxParser::kProcDirectory +LinuxParser::kUptimeFilename);
            
            if(uptimecpu.size()>0){
                string s = extractTokenFromString(uptimecpu[0],0);
                uptime = std::stoi(s);
            }

            float sys_uptime = uptime - processTime;

            processor_usage = ((totalTicks/hz) / sys_uptime) ;
            //* 100.0;

    }
    auto cmd = LinuxParser::ReadFile(pFolder + LinuxParser::kCmdlineFilename);
    if( cmd.size() > 0 && !cmd[0].empty()){

    Process p(i,cmd[0],processor_usage,usr,ram, processTime) ;
    processes_.push_back(p);
    }
    }
   
       
    sort(processes_.begin(),processes_.end());
    return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    auto t = LinuxParser::RunningProcesses();
    return t; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    auto t = LinuxParser::TotalProcesses();
    return t;  }

// TODO: Return the number of seconds since the system started running
long System::UpTime() { 
    return LinuxParser::UpTime();
     }