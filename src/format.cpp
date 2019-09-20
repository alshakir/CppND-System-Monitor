#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#include "format.h"

using std::stringstream;

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int h = 0;
  int m = 0;

  myModulo(seconds, 3600, h);

  myModulo(seconds, 60, m);
  stringstream ss;

  ss << h << ":" << std::setfill('0') << std::setw(2) << m << ":"
     << std::setfill('0') << std::setw(2) << seconds;

  std::string result;
  std::getline(ss, result);
  return result;
}

void myModulo(long& numerator, int denominator, int& quotient) {
  quotient = floor(numerator / denominator);
  numerator = numerator % denominator;
}