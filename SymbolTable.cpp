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

void SymbolTable::define(const std::string &name, Type::Kind kind) {
  auto result = table_.emplace(name, Symbol(name, Type(kind)));
  SI_ASSERT(result.second);
}
}  // namespace SI