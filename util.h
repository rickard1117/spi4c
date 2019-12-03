#ifndef SI_UTIL_H__
#define SI_UTIL_H__

#include <string>

namespace SI {
namespace util {

class StringPiece {
  public:
    StringPiece(const std::string &str) : str_(&str[0]), len_(str.length()) {}

    bool empty() const { return len_ == 0; }

    std::size_t length() const { return len_; }

  private:
    const char *str_;
    std::size_t len_;
};

} // namespace util
} // namespace SI

#endif // SI_UTIL_H__