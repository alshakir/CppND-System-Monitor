#include "linux_parser.h"
#include <dirent.h>
#include <unistd.h>
#include <filesystem>  //c++ 17 only
#include <string>
#include <vector>

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

namespace fs = std::filesystem;

// read file and returns its lines as a vector<string>
vector<std::string> LinuxParser::ReadFile(std::string p) {
  vector<std::string> v;
  string line;

  std::ifstream filestream(p);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      v.push_back(line);
    }
  }
  return v;
}

// as the name implies (split string into words using whitespaces)
vector<string> LinuxParser::tokenizeString(string s) {
  string word;
  vector<string> tokens;
  std::stringstream ss(s);

  while (ss >> word) {
    tokens.push_back(word);
  }

  return tokens;
}

// extract specific word from a string by its index
string LinuxParser::extractTokenFromString(string s, int idx) {
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

// I updated this method to  the new c++17 filesystem library
// and renamed it myPids()..
// so the following method is deprecated!
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

// this method uses c++17 filesystem library
// however i had to add the following line to CMakeLists.txt file
// target_link_libraries(monitor stdc++fs)
vector<int> LinuxParser::myPids() {
  vector<int> pids;
  fs::path p("/proc/");
  for (const auto& item : fs::directory_iterator(p)) {
    if (fs::is_directory(item)) {
      string filename = item.path().filename();

      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }

  return pids;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  auto mem = ReadFile(kProcDirectory + kMeminfoFilename);
  string MemTotal, MemFree, MemAvailable, Buffer, Cache;
  string name, val;

  vector<string> valueStrings;

  for (int i = 0; i < 5; i++) {
    std::stringstream ss;
    ss << mem[i];
    ss >> name >> val;
    valueStrings.push_back(val);
  }

  size_t sz;
  double d_MemTotal = std::stod(valueStrings[0], &sz);

  double d_MemFree = std::stod(valueStrings[1], &sz);

  // double d_MemAvailable = std::stod(valueStrings[2], &sz);

  // double d_Buffer = std::stod(valueStrings[3], &sz);

  // double d_Cache = std::stod(valueStrings[4], &sz);

  double d_MemUsed = d_MemTotal - d_MemFree;

  float memUsedPercentage = d_MemUsed / d_MemTotal;

  return memUsedPercentage;
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() {
  auto uptime = ReadFile(kProcDirectory + kUptimeFilename);
  string line = uptime[0];
  std::istringstream stream(line);
  string up;
  stream >> up;
  size_t sz;
  long val = std::stol(up, &sz);

  return val;
}

// TODO: Read and return the number of jiffies for the system

long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Done: Read and return CPU utilization
vector<std::string> LinuxParser::CpuUtilization() {
  vector<std::string> f = ReadFile(LinuxParser::kProcDirectory + "stat");
  return f;
}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int value;

  string proc = "processes";
  string temp;
  vector<std::string> f = ReadFile(LinuxParser::kProcDirectory + "stat");

  for (string s : f) {
    std::stringstream ss;
    ss << s;
    ss >> temp;
    if (proc == temp) {
      ss >> value;
      return value;
    }
  }
  return 0;
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int value;

  string proc = "procs_running";
  string temp;
  vector<std::string> f = ReadFile(LinuxParser::kProcDirectory + "stat");

  for (string s : f) {
    std::stringstream ss;
    ss << s;
    ss >> temp;
    if (proc == temp) {
      ss >> value;
      return value;
    }
  }
  return 0;
}

// Done: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string pFolder = LinuxParser::kProcDirectory + std::to_string(pid);

  auto cmd = LinuxParser::ReadFile(pFolder + LinuxParser::kCmdlineFilename);
  if (cmd.size() > 0 && !cmd[0].empty()) {
    return cmd[0];
  } else {
    return string();
  }
}

// The reason I combind RAM and USER methods in this method is
// that I don't want to read the process status file twice
// so I read the file once (also I break the loop once I find ram and user)
// then I use a helper method to mathc the user to user ID (I open the passwd
// file in that method) for this reason this method returns a vector of strings
// first item  is the RAM and the second is the user name
// if nothing found then there will be an empty string place holder
std::vector<string> LinuxParser::Ram_and_User(int pid) {
  std::vector<string> ru;
  ru.clear();
  string ram;
  string userID;
  string usr;

  int found =
      0;  // we need to find two values(RAM, and UserID ) then break the loop
  string pFolder = LinuxParser::kProcDirectory + std::to_string(pid);

  auto status = LinuxParser::ReadFile(pFolder + LinuxParser::kStatusFilename);

  if (status.size() > 0) {
    for (string s : status) {
      size_t index = s.find("VmSize");
      if (index != string::npos) {
        string ramString = s.substr(8);
        int r = std::stoi(ramString);  // kb will be ignored
        r /= 1000;                     // convert to MegaBytes
        ram = std::to_string(r);

        found++;
        if (found > 1) break;
      }

      index = s.find("Uid");
      if (index != string::npos) {
        std::stringstream ss(s);
        // string looks like this : Uid:	0	0	0	0
        ss >> userID >> userID;

        found++;
        if (found > 1) break;
      }
    }
    usr = LinuxParser::User(userID);
  }
  // the vector should be returned with two items ; even if the items are empty
  // because sometimes the file is created momentarily then deleted
  // e.g. /proc/7373 file will be found when the Pids() function start to scan
  // the /proc folder however  after that the file /proc/7373 will be deleted if
  // the process is killed then this function will find no user and no ram.. so
  // we inject empty string in that case. When i used to put the followin two
  // lines inside the if(status.size()>0) I used to have memory segmentation
  // error because the vector was returned empty
  ru.push_back(ram);  // first item
  ru.push_back(usr);  // second item
  return ru;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// This recieves the userID string from Ram_and_User() method and return the
// username
string LinuxParser::User(string userID) {
  string usr;
  auto users = LinuxParser::ReadFile(LinuxParser::kPasswordPath);
  if (users.size() > 0) {
    for (string s : users) {
      std::istringstream is(s);

      string s1, s2;
      std::getline(is, usr, ':');
      std::getline(is, s1, ':');
      std::getline(is, s2, ':');
      if (s2 == userID) break;
    }
  }
  return usr;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }

// this method returns process processor usage and its uptime in seconds.
std::vector<float> LinuxParser::PUsage_and_Pseconds(int pid) {
  string pFolder = LinuxParser::kProcDirectory + std::to_string(pid);

  // find processor usage and process time
  auto stat = LinuxParser::ReadFile(pFolder + LinuxParser::kStatFilename);
  float processor_usage;
  // float processTime ;

  float process_seconds;
  if (stat.size() > 0) {
    string line = stat[0];
    string utime, stime, cutime, cstime, starttime;
    utime = LinuxParser::extractTokenFromString(line, 13);
    stime = LinuxParser::extractTokenFromString(line, 14);
    cutime = LinuxParser::extractTokenFromString(line, 15);
    cstime = LinuxParser::extractTokenFromString(line, 16);
    starttime = LinuxParser::extractTokenFromString(line, 21);

    int i_utime = std::stoi(utime);
    int i_stime = std::stoi(stime);
    int i_cutime = std::stoi(cutime);
    int i_cstime = std::stoi(cstime);
    int i_starttime = std::stoi(starttime);

    double totalTicks = i_utime + i_stime + i_cutime + i_cstime;
    float hz = sysconf(_SC_CLK_TCK);

    // processTime = i_starttime/hz;

    float uptime;
    auto uptimecpu = LinuxParser::ReadFile(LinuxParser::kProcDirectory +
                                           LinuxParser::kUptimeFilename);

    if (uptimecpu.size() > 0) {
      string s = LinuxParser::extractTokenFromString(uptimecpu[0], 0);
      uptime = std::stoi(s);
    }

    process_seconds = uptime - (i_starttime / hz);

    processor_usage = ((totalTicks / hz) / process_seconds);
    //* 100.0; the multiplication by 100 happens in the ncurse_display fiile
  }

  std::vector<float> v;
  v.push_back(processor_usage);
  v.push_back(process_seconds);

  return v;
}