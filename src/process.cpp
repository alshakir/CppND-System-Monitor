#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int id, string cmd, float cpu, string user, string ram,
                 long int uptime)
    : _pid(id),
      _command(cmd),
      _cpuUtilization(cpu),
      _user(user),
      _ram(ram),
      _uptime(uptime) {}
// Done: Return this process's ID
int Process::Pid() { return _pid; }

// Done: Return this process's CPU utilization
float Process::CpuUtilization() { return _cpuUtilization; }

// Done: Return the command that generated this process
string Process::Command() { return _command; }

// Done: Return this process's memory utilization
string Process::Ram() { return _ram; }

// Done: Return the user (name) that generated this process
string Process::User() { return _user; }

// Done: Return the age of this process (in seconds)
long int Process::UpTime() { return _uptime; }

// Done: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return _cpuUtilization > a._cpuUtilization;
}