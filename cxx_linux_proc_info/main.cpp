// #include <stdio.h>
// #include <stdlib.h>

// int main(int, char **) {
//   FILE *cpuinfo = fopen(, "rb");
//   char *arg = 0;
//   size_t size = 0;
//   while (getdelim(&arg, &size, 0, cpuinfo) != -1) {
//     puts(arg);
//   }
//   free(arg);
//   fclose(cpuinfo);
//   return 0;
// }

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

/**
 * @brief Remove all leading spaces on the given string in place.
 */
auto LeftTrim(std::string &s) -> void {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return std::isspace(ch) == 0;
          }));
}

auto RightTrim(std::string &s) -> void {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](int ch) { return std::isspace(ch) == 0; })
              .base(),
          s.end());
}

int main() {
  std::string line;
  std::ifstream myfile("/proc/cpuinfo");
  if (!myfile.is_open()) {
    std::cout << "Unable to open file";
  }

  std::unordered_map<std::string, std::string> map{};
  while (getline(myfile, line)) {
    auto found = std::find(std::begin(line), std::end(line), ':');
    if (found != std::end(line)) {
      auto key = std::string(std::begin(line), found);
      auto value = std::string(++found, line.end());
      LeftTrim(key);
      RightTrim(key);
      LeftTrim(value);
      RightTrim(value);
      map.insert(std::make_pair(key, value));
    }
  }
  myfile.close();

  for (auto const &item : map) {
    std::cout << item.first << ": " << item.second << '\n';
  }

  return 0;
}