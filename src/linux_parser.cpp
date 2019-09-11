#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;


vector<std::string> LinuxParser::ReadFile(std::string p){
  vector<std::string> v;
  string line;

  std::ifstream filestream(p);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)){
      
      std::istringstream linestream(line);
      v.push_back(line);
    }
    
  }
  return v;
}


vector<string> tokenizeString(string s){
    string word;
    vector<string> tokens;
    std::stringstream ss(s);

    while(ss >> word){
        tokens.push_back(word);
    }

    return tokens;
}


string extractTokenFromString(string s, int idx){
    return tokenizeString(s)[idx];
}

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
  string os, version, kernel;
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
   auto mem = ReadFile(kProcDirectory + kMeminfoFilename);
  string MemTotal, MemFree, MemAvailable, Buffer, Cache;
  string name, val;

  vector<string> valueStrings;

  for(int i = 0; i < 5; i++){
    std::stringstream ss;
    ss << mem[i];
    ss >> name >> val;
    valueStrings.push_back(val);
  }

  size_t sz;
  double d_MemTotal = std::stod(valueStrings[0], &sz);
  
  double d_MemFree = std::stod(valueStrings[1], &sz);
  
  double d_MemAvailable = std::stod(valueStrings[2], &sz);
  
  double d_Buffer = std::stod(valueStrings[3], &sz);
  
  double d_Cache = std::stod(valueStrings[4], &sz);
  

  double d_MemUsed = d_MemTotal - d_MemFree;

 float memUsedPercentage = d_MemUsed/d_MemTotal;

  return memUsedPercentage; }

// TODO: Read and return the system uptime
long  LinuxParser::UpTime() { 
  auto uptime = ReadFile(kProcDirectory + kUptimeFilename);
  string line  = uptime[0];
  std::istringstream stream(line);
  string up;
  stream >> up ;
   size_t sz;
  long val = std::stol(up, &sz);

  return val; }

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
vector<std::string> LinuxParser::CpuUtilization() { 
  vector<std::string> f = ReadFile(LinuxParser::kProcDirectory + "stat");
  return f; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int value;
  
  string proc = "processes";
  string temp;
   vector<std::string> f = ReadFile(LinuxParser::kProcDirectory + "stat");
   
   for (string s : f){
     std::stringstream ss;
     ss << s;
    ss >> temp;
    if(proc == temp){
      ss >> value;
      return value;
    }
   }
  return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {   int value;
  
  string proc = "procs_running";
  string temp;
   vector<std::string> f = ReadFile(LinuxParser::kProcDirectory + "stat");
   
   for (string s : f){
     std::stringstream ss;
     ss << s;
    ss >> temp;
    if(proc == temp){
      ss >> value;
      return value;
    }
   }
  return 0;  }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }