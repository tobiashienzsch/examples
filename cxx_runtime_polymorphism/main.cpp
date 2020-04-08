#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <chrono>
#include <future>
#include <thread>

#include "document.hpp"

int main(int, char **) {

  auto doc = mc::Document{};
  doc.reserve(5);

  doc.emplace_back(0);
  doc.emplace_back(1);
  doc.emplace_back(2);
  doc.emplace_back(doc);
  doc.emplace_back("Test");
  doc.emplace_back(2.3f);

  std::reverse(std::begin(doc), std::end(doc));

  mc::Draw(doc, std::cout, 0);

  printf("--------------------------\n");

  mc::History h{1};
  mc::Current(h).emplace_back(0);
  mc::Current(h).emplace_back("Test");
  mc::Draw(mc::Current(h), std::cout, 0);
  printf("--------------------------\n");

  mc::Commit(h);
  mc::Current(h)[0] = 42;

  auto saving = std::async([doc = mc::Current(h)]() {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    printf("----------save------------\n");
    mc::Draw(doc, std::cout, 0);
  });
  mc::Current(h)[1] = std::string("String");
  mc::Current(h).emplace_back(mc::Current(h));
  mc::Current(h).emplace_back(2.3f);
  mc::Draw(mc::Current(h), std::cout, 0);
  printf("--------------------------\n");

  mc::Undo(h);
  mc::Draw(mc::Current(h), std::cout, 0);

  return EXIT_SUCCESS;
}
