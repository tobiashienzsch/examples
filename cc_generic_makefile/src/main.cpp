#include <cstdio>
#include <cstdlib>

int main() {
  struct S {
    int x, y;
  };
  auto *x = new S{};

  printf("TEST\n");
  return EXIT_SUCCESS;
}
