#ifndef SYMBOL_H__
#define SYMBOL_H__

#include <string>

#include "Type.h"
#include "util.h"

namespace SI {

class Symbol {
 public:
  Symbol(const std::string &name, Type type) : name_(name), type_(type) {}
  const std::string &name() const { return name_; };

 protected:
  std::string name_;
  Type type_;
};

}  // namespace SI

#endif  // SYMBOL_H__