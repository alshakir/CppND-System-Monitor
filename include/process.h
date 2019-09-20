#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int id, string cmd, float cpu, string user, string ram,
          long int uptime);
  int Pid();                               // Done: See src/process.cpp
  std::string Command();                   // Done: See src/process.cpp
  float CpuUtilization();                  // Done: See src/process.cpp
  std::string User();                      // Done: See src/process.cpp
  std::string Ram();                       // Done: See src/process.cpp
  long int UpTime();                       // Done: See src/process.cpp
  bool operator<(Process const& a) const;  // Done: See src/process.cpp

  // Done: Declare any necessary private members
 private:
  int _pid;
  string _command;
  float _cpuUtilization;
  string _user;
  string _ram;
  long int _uptime;
};

#endif