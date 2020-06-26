#include <boost/fusion/adapted.hpp>
#include <boost/fusion/algorithm.hpp>
#include <iostream>
#include <type_traits>
#include <vector>

#include "network.hpp"

namespace asio = boost::asio;
namespace fusion = boost::fusion;

namespace th {

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
    (std::uint16_t, version)
    (std::uint32_t, sessionID)
    (double, value)
    (std::string, name)
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
  template <typename T> void operator()(T val) const {
    std::cout << "val: " << std::boolalpha << val << '\n';
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
  result.reserve(1024);
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
  th::MessageHeader header = {th::MessageType::Parameter, 12, 143, 1.43,
                              "gain"};
  print(header);

  // Serialize & Deserialize & Print struct
  auto buffer = write<th::MessageHeader>(header);
  auto msg = read<th::MessageHeader>(buffer);
  print(msg);
}