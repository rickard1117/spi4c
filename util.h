#ifndef SI_UTIL_H__
#define SI_UTIL_H__

#include <string.h>
#include <string>

namespace SI {
namespace util {

class StringPiece {
  public:
    StringPiece(const char *start, std::size_t len) : str_(start), len_(len) {}
    StringPiece(const char *start) : str_(start), len_(strlen(start)) {}
    bool empty() const { return len_ == 0; }

    std::size_t length() const { return len_; }

    bool equal(const char *str) const { return strncmp(str_, str, strlen(str)) == 0; }

  private:
    const char *str_;
    std::size_t len_;
};

} // namespace util
} // namespace SI

#endif // SI_UTIL_H__