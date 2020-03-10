#ifndef ERROR_H__
#define ERROR_H__

#include <stdexcept>
#include <string>

namespace SI {

using InterpreterErrorCode = std::string;

extern const InterpreterErrorCode kErrorUnrecognized;
extern const InterpreterErrorCode kErrorUnexpectedToken;
extern const InterpreterErrorCode kErrorIDNotFound;
extern const InterpreterErrorCode kErrorDupID;

class InterpreterError : public std::runtime_error {
 public:
  InterpreterError(const InterpreterErrorCode &code, const std::string &errstr)
      : std::runtime_error(code + " : " + errstr) {}

  template <typename T>
  InterpreterError(const InterpreterErrorCode &code, const T &err)
      : InterpreterError(code, std::to_string(err)) {}
};



}  // namespace SI

#endif  // ERROR_H__