#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

/**
 * @brief RAII temporary file. Creates a file in the systems tmp folder ojn
 * construction. Deleted from filesystem on destruction. Useful for testing.
 */
class ScopedTempFile {
public:
  /**
   * @brief Creates a file with the given content.
   */
  ScopedTempFile(std::string content = "") {
    fs::ofstream file{path_};
    file << content;
    file.close();
  }

  /**
   * @brief Removes the file.
   */
  ~ScopedTempFile() noexcept { Remove(); }

  /**
   * @brief Returns the full path as a string.
   */
  [[nodiscard]] auto Path() const -> std::string { return path_.string(); }

  /**
   * @brief Removes the file.
   */
  auto Remove() noexcept -> bool {
    if (Exists()) {
      auto ec = boost::system::error_code{};
      if (fs::remove(path_, ec) && !ec) {
        return true;
      }
    }
    return false;
  }

  /**
   * @brief Returns true if the file exists.
   */
  [[nodiscard]] auto Exists() const noexcept -> bool {
    auto ec = boost::system::error_code{};
    if (fs::exists(path_, ec) && !ec) {
      return true;
    }
    return false;
  }

  /**
   * @brief Returns the size in bytes. Zero if the file doesn't exist.
   */
  [[nodiscard]] auto SizeInBytes() const noexcept -> size_t {
    if (Exists()) {
      auto ec = boost::system::error_code{};
      auto const size = fs::file_size(path_, ec);
      return !ec ? static_cast<size_t>(size) : 0;
    }
    return 0;
  }

private:
  fs::path path_{fs::temp_directory_path() / fs::unique_path()};
};

int main() {
  auto file = ScopedTempFile{"test\n"};
  std::cout << file.Path() << '\n';
  std::cout << file.SizeInBytes() << '\n';
}