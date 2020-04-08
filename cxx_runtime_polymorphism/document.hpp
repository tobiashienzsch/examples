#pragma once

#include <iostream>
#include <memory>
#include <vector>

namespace mc {

template <typename Type>
void Draw(Type const &x, std::ostream &out, size_t position) {
  out << std::string(position, ' ') << x << '\n';
}

class DocumentEntry {
public:
  template <typename Type>
  DocumentEntry(Type x) : self_(std::make_shared<model<Type>>(std::move(x))) {}

  friend void Draw(DocumentEntry const &x, std::ostream &out, size_t position) {
    x.self_->Draw_(out, position);
  }

private:
  struct concept_t {
    virtual ~concept_t() = default;
    virtual void Draw_(std::ostream &out, size_t position) const = 0;
  };

  template <class Type> struct model final : concept_t {
    model(Type x) : data_(std::move(x)) {}

    void Draw_(std::ostream &out, size_t position) const override {
      Draw(data_, out, position);
    }

    Type data_;
  };

private:
  std::shared_ptr<const concept_t> self_;
};

// Document
using Document = std::vector<DocumentEntry>;
void Draw(Document const &doc, std::ostream &out, size_t position);

// History
using History = std::vector<Document>;
void Commit(History &x);
void Undo(History &x);
Document &Current(History &x);

} // namespace mc
