#ifndef SI_UTIL_H__
#define SI_UTIL_H__

#include <cstdlib>
#include <iostream>

namespace SI {
namespace util {

#define SI_ASSERT_MSG(expr, msg) __ASSERT(#expr, expr, __FILE__, __LINE__, msg)
#define SI_ASSERT(expr) __ASSERT(#expr, expr, __FILE__, __LINE__)

// #define GetMacro(_1, _2, NAME, ...) NAME
// #define ASSERT(...) GetMacro(__VA_ARGS__, __ASSERT, __ASSERT,
// ...)(__VA_ARGS__)

void __ASSERT(const char *str, bool expr, const char *file, int line,
              const char *msg = "") {
  if (!expr) {
    std::cerr << "Assert failed:\t" << msg << "\n"
              << "Expected:\t" << str << "\n"
              << "Source:\t\t" << file << ", line " << line << "\n";
    std::abort();
  }
}

}  // namespace util
}  // namespace SI

#endif  // SI_UTIL_H__