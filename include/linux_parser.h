#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <filesystem>
#include <fstream>
#include <regex>
#include <string>

using std::string;
// using namespace std;

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
std::vector<int> myPids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::vector<std::string> Ram_and_User(int pid);
std::string Uid(int pid);
std::string User(string userID);
long int UpTime(int pid);

std::vector<float> PUsage_and_Pseconds(int pid);

//******* My assistance methods for reading and manipulating strings********

// assistant method for reading a file and returning vector of lines(string)
std::vector<std::string> ReadFile(std::string p);

// break strings into tokens
std::vector<std::string> tokenizeString(string s);

// extract token from string by the index
std::string extractTokenFromString(string s, int idx);

};  // namespace LinuxParser

#endif