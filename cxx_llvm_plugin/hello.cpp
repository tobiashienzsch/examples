int bar() noexcept;
int baz() noexcept;

void condition(int &x) noexcept {
  if (x == 1) {
    x = bar();
    return;
  }
  x = baz();
  return;
}
