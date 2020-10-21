#include <boost/process.hpp>

#include <iostream>
#include <string>

namespace bp = boost::process;

int main() {
  auto c = bp::child{bp::search_path("g++"), "--version"};

  while (c.running()) {
    std::cout << "do stuff\n";
  }

  c.wait();
  int result = c.exit_code();
  return result;
}