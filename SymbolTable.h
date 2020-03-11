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
  bool define(const std::string &name, TypeKind kind);

 private:
  std::unordered_map<std::string, Symbol> table_;
};

class ScopedSymbolTable {
 public:
  ScopedSymbolTable(const std::string &name,
                    std::shared_ptr<ScopedSymbolTable> upper)
      : name_(name),
        level_(upper == nullptr ? 1 : upper->level_ + 1),
        upper_(std::move(upper)) {}
  const Symbol *lookup(const std::string &name) const;
  bool define(const std::string &name, TypeKind kind);
  std::string metaInfo() const;
  std::shared_ptr<ScopedSymbolTable> upperScope() { return upper_; }

 private:
  const std::string name_;
  const int level_;
  std::shared_ptr<ScopedSymbolTable> upper_;
  std::unordered_map<std::string, Symbol> table_;
};

}  // namespace SI

#endif  // SYMBOL_TABLE_H__