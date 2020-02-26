#ifndef SI_UTIL_H__
#define SI_UTIL_H__

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace SI {
namespace util {

#ifndef SI_ASSERT_MSG
#define SI_ASSERT_MSG(expr, msg) __ASSERT(#expr, expr, __FILE__, __LINE__, msg)
#endif

#ifndef SI_ASSERT
#define SI_ASSERT(expr) __ASSERT(#expr, expr, __FILE__, __LINE__)
#endif

class Noncopyable {
 protected:
  Noncopyable() = default;
  ~Noncopyable() = default;

 public:
  Noncopyable(const Noncopyable &) = delete;
  Noncopyable &operator=(const Noncopyable &) = delete;
};

template <class T>
using Ptr = std::unique_ptr<T>;

// #define GetMacro(_1, _2, NAME, ...) NAME
// #define ASSERT(...) GetMacro(__VA_ARGS__, __ASSERT, __ASSERT,
// ...)(__VA_ARGS__)

void __ASSERT(const char *str, bool expr, const char *file, int line,
              const std::string &msg = "");

template <typename T>
void vectorExtends(std::vector<T> &v1, std::vector<T> v2) {
  v1.insert(std::end(v1), std::make_move_iterator(v2.begin()),
            std::make_move_iterator(v2.end()));
}

}  // namespace util
}  // namespace SI

#endif  // SI_UTIL_H__