#include <boost/fusion/adapted.hpp>
// #include <boost/fusion/adapted/struct/define_struct.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/view.hpp>
#include <boost/mpl/arg.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits.hpp>
#include <iostream>
#include <string>

using namespace boost;

struct Reader {
public:
  explicit Reader(std::vector<char> const &buffer) : buffer_{buffer.data()} {}

  template <typename T> void operator()(T &val) const {
    val = *reinterpret_cast<T const *>(buffer_);
    buffer_ = buffer_ + sizeof(T);
  }

private:
  mutable char const *buffer_;
};
struct Printer {
  // template <typename T>
  // void operator()(const T &t) const {
  //   std::cout << std::boolalpha << t << '\n';
  // }
  void operator()(bool val) const {
    std::printf("%s\n", val ? "true" : "false");
  }
  void operator()(int val) const { std::printf("%d\n", val); }
  void operator()(float val) const { std::printf("%f\n", val); }
  void operator()(double val) const { std::printf("%f\n", val); }
};

// clang-format off
BOOST_FUSION_DEFINE_STRUCT(
    (th), message,
    (double, f)
    (int, i)
    (int, d)
    (bool, b)
)
// clang-format on

template <typename T> T read(std::vector<char> const &data) {
  auto reader = Reader{data};
  auto result = T{};
  fusion::for_each(result, reader);
  return result;
}
int main() {
  th::message s = {1.43, 10, 3, true};
  fusion::for_each(s, Printer{});
  auto ptr = reinterpret_cast<char *>(&s);
  auto buffer = std::vector<char>(ptr, ptr + sizeof(th::message));
  auto msg = read<th::message>(buffer);
  fusion::for_each(msg, Printer{});
}