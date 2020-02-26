#include "util.h"

namespace SI {
namespace util {
void __ASSERT(const char *str, bool expr, const char *file, int line,
              const std::string &msg) {
  if (!expr) {
    std::cerr << "Assert failed:\t" << msg << "\n"
              << "Expected:\t" << str << "\n"
              << "Source:\t\t" << file << ", line " << line << "\n";
    std::abort();
  }
}
}  // namespace util
}  // namespace SI
