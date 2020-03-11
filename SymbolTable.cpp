#include "SymbolTable.h"

#include "util.h"

namespace SI {

using namespace SI::util;

const Symbol *SymbolTable::lookup(const std::string &name) const {
  const auto it = table_.find(name);
  if (it == table_.cend()) {
    return nullptr;
  } else {
    return &it->second;
  }
}

bool SymbolTable::define(const std::string &name, TypeKind kind) {
  auto result = table_.emplace(name, Symbol(name, Type(kind)));
  return result.second;
}

const Symbol *ScopedSymbolTable::lookup(const std::string &name) const {
  const auto it = table_.find(name);
  if (it != table_.cend()) {
    return &it->second;
  }

  if (upper_ == nullptr) {
    return nullptr;
  }

  return upper_->lookup(name);
}

bool ScopedSymbolTable::define(const std::string &name, TypeKind kind) {
  auto result = table_.emplace(name, Symbol(name, Type(kind)));
  return result.second;
}

std::string ScopedSymbolTable::metaInfo() const {
  std::string s;
  if (upper_ != nullptr) {
    s += upper_->metaInfo();
  }
  return s + "|" + name_ ;
}

}  // namespace SI