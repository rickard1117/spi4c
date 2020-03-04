
#include "AST.h"

#include "util.h"

namespace SI {

using namespace SI::util;

namespace {
bool isFloat(const std::string &val) {
  auto index = val.find('.');
  return index != std::string::npos;
}
}  // namespace

Number::Number(const std::string &val) {
  if (isFloat(val)) {
    num_ = std::stof(val);
    evalType_ = _real;
  } else {
    num_ = std::stoi(val);
    evalType_ = _int;
  }
}

}  // namespace SI