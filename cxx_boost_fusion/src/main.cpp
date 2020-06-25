#include <boost/fusion/adapted.hpp>
#include <boost/fusion/algorithm.hpp>
#include <iostream>
#include <type_traits>
#include <vector>
// #include <boost/fusion/sequence.hpp>
// #include <boost/fusion/tuple.hpp>
// #include <boost/fusion/view.hpp>
// #include <boost/mpl/arg.hpp>
// #include <boost/mpl/int.hpp>
// #include <boost/type_traits.hpp>

using namespace boost;

namespace th {
enum class MessageVersion : std::uint8_t {
  Unknown = 0,
  V1 = 1,
};
auto operator<<(std::ostream &out, MessageVersion type) -> std::ostream & {
  switch (type) {
  case th::MessageVersion::Unknown: {
    out << "Unknown";
    break;
  }
  case th::MessageVersion::V1: {
    out << "V1";
    break;
  }
  default:
    break;
  }
  return out;
}

enum class MessageType : std::uint16_t {
  Unknown = 0,
  Parameter = 1,
};
auto operator<<(std::ostream &out, MessageType type) -> std::ostream & {
  switch (type) {
  case th::MessageType::Unknown: {
    out << "Unknown";
    break;
  }
  case th::MessageType::Parameter: {
    out << "Parameter";
    break;
  }
  default:
    break;
  }
  return out;
}
} // namespace th

// clang-format off
BOOST_FUSION_DEFINE_STRUCT(
    (th), MessageHeader,
    (th::MessageType, type)
    (th::MessageVersion, version)
    (std::string, name)
    (double, f)
)
// clang-format on

struct Reader {
public:
  explicit Reader(std::vector<char> const &buffer) : buffer_{buffer.data()} {}

  template <typename T>
  auto operator()(T &val) const
      -> std::enable_if_t<std::is_integral<T>::value> {
    val = *reinterpret_cast<T const *>(buffer_);
    buffer_ = buffer_ + sizeof(T);
  }

  template <typename T>
  auto operator()(T &val) const
      -> std::enable_if_t<std::is_floating_point<T>::value> {
    val = *reinterpret_cast<T const *>(buffer_);
    buffer_ = buffer_ + sizeof(T);
  }

  auto operator()(std::string &val) const -> void {
    auto length = std::uint16_t{0};
    (*this)(length);
    val = std::string(reinterpret_cast<char const *>(buffer_), length);
    buffer_ = buffer_ + length;
  }

  auto operator()(th::MessageVersion &val) const -> void {
    val = *reinterpret_cast<th::MessageVersion const *>(buffer_);
    buffer_ = buffer_ + sizeof(th::MessageVersion);
  }

  auto operator()(th::MessageType &val) const -> void {
    val = *reinterpret_cast<th::MessageType const *>(buffer_);
    buffer_ = buffer_ + sizeof(th::MessageType);
  }

private:
  mutable char const *buffer_;
};

struct Writer {
public:
  explicit Writer(char *buffer) : buffer_{buffer} {}

  template <typename T> void operator()(T const &val) const {
    *reinterpret_cast<T *>(buffer_) = val;
    buffer_ = buffer_ + sizeof(T);
  }

  auto operator()(std::string &val) const -> void {
    auto const length = val.length();
    (*this)(static_cast<std::uint16_t>(length));
    for (auto const c : val) {
      *buffer_ = c;
      buffer_++;
    }
  }

private:
  mutable char *buffer_;
};

struct Printer {
  void operator()(bool val) const { std::puts(val ? "true" : "false"); }
  void operator()(int val) const { std::printf("%d\n", val); }
  void operator()(float val) const { std::printf("%f\n", val); }
  void operator()(double val) const { std::printf("%f\n", val); }
  template <typename T> void operator()(T val) const {
    std::cout << val << '\n';
  }
};

template <typename T> T read(std::vector<char> const &data) {
  auto reader = Reader{data};
  auto result = T{};
  fusion::for_each(result, reader);
  return result;
}

template <typename T> auto write(T &data) {
  auto result = std::vector<char>{0};
  result.reserve(4096);
  auto writer = Writer{result.data()};
  fusion::for_each(data, writer);
  return result;
}

template <typename T> auto print(T const &data) {
  fusion::for_each(data, Printer{});
  std::puts("");
}

int main() {
  // Create struct & print elements
  th::MessageHeader header = {th::MessageType::Parameter,
                              th::MessageVersion::V1, "gain", 1.43};
  print(header);

  // Serialize & Deserialize & Print struct
  auto buffer = write<th::MessageHeader>(header);
  auto msg = read<th::MessageHeader>(buffer);
  print(msg);
}