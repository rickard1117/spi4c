#ifndef SYMBOL_TABLE_H__
#define SYMBOL_TABLE_H__

#include <memory>
#include <optional>
#include <unordered_map>

#include "Symbol.h"
#include "Type.h"

namespace SI {

class SymbolTable {
 public:
  const Symbol *lookup(const std::string &name) const;
  void define(const std::string &name, Type::Kind kind);

 private:
  std::unordered_map<std::string, Symbol> table_;
};


}  // namespace SI

#endif  // SYMBOL_TABLE_H__