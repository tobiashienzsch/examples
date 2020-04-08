#include "document.hpp"

#include <cassert>

namespace mc {

void Draw(Document const &doc, std::ostream &out, size_t position) {
  out << std::string(position, ' ') << "<document>" << '\n';
  for (auto const &entry : doc) {
    Draw(entry, out, position + 2);
  }
  out << std::string(position, ' ') << "</document>" << '\n';
}

void Commit(History &x) {
  assert(x.size());
  x.push_back(x.back());
}

void Undo(History &x) {
  assert(x.size());
  x.pop_back();
}

Document &Current(History &x) {
  assert(x.size());
  return x.back();
}

} // namespace mc
