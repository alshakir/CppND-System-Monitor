#include <unistd.h>
#include <algorithm>
#include <cstddef>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "format.h"
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Done: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Done: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  // notice this method assume that processes are created in sequence
  // if multiple threads create processes simultaneously then
  // there will be a problem in user ID and name if threads are racing on it
  processes_.clear();

  // I had to use my method myPids() that implement c++17 filesyste
  // my compiler supports it : (gcc version 8.3.0 (Ubuntu 8.3.0-6ubuntu1))
  // but I had to add a line to the cmake file
  // target_link_libraries(monitor stdc++fs)
  auto pids = LinuxParser::myPids();

  for (int i : pids) {
    //*** find the command
    auto cmd = LinuxParser::Command(i);

    //*** find the username and the RAM size
    std::vector<string> ram_and_user;
    ram_and_user.clear();
    ram_and_user = LinuxParser::Ram_and_User(i);
    string ram = ram_and_user[0];
    string user = ram_and_user[1];

    //*** find the processor usage and the process uptime(seconds)
    auto process_usage_and_seconds = LinuxParser::PUsage_and_Pseconds(i);
    float processor_usage = process_usage_and_seconds[0];
    float process_seconds = process_usage_and_seconds[1];
    Process p(i, cmd, processor_usage, user, ram, process_seconds);

    processes_.push_back(p);
  }

  sort(processes_.begin(), processes_.end());
  return processes_;
}

// Done: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Done: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Done: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Done: Return the number of processes actively running on the system
int System::RunningProcesses() {
  auto t = LinuxParser::RunningProcesses();
  return t;
}

// Done: Return the total number of processes on the system
int System::TotalProcesses() {
  auto t = LinuxParser::TotalProcesses();
  return t;
}

// Done: Return the number of seconds since the system started running
long System::UpTime() { return LinuxParser::UpTime(); }