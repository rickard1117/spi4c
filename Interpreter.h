#ifndef INTERPRETER_H_
#define INTERPRETER_H_
// #include "Interpreter.h"
#include <string>

namespace SI {
namespace Interpreter {

class Interpreter {
 public:
  int interpret(const std::string &formula) const;
};
}  // namespace Interpreter
}  // namespace SI

#endif  // INTERPRETER_H_