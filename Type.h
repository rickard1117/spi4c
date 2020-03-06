#ifndef TYPE_H__
#define TYPE_H__

#include <string>
#include <variant>

namespace SI {

enum TypeKind { kNull = 0, kInt, kReal };
class Type {
 public:
  Type(TypeKind kind = TypeKind::kNull) : kind_(kind) {}
  static Type getArithType(const Type& left, const Type& right);

  friend bool operator==(const Type& lhs, const Type& rhs) {
    return lhs.kind_ == rhs.kind_;
  }

  friend bool operator!=(const Type& lhs, const Type& rhs) {
    return !(lhs == rhs);
  }

 private:
  friend class GeneralArithVal;
  TypeKind kind_;
};

extern const Type _null;
extern const Type _int;
extern const Type _real;

class GeneralArithVal {
 public:
  GeneralArithVal() : type_(_null) {}
  GeneralArithVal(int i) : type_(_int), num_(i) {}
  GeneralArithVal(double f) : type_(_real), num_(f) {}

  // for debug
  std::string debug() const {
    std::string s{"type :"};
    switch (type_.kind_) {
      case TypeKind::kNull:
        s += "null";
        return s;
      case TypeKind::kInt:
        s += "int, " + std::to_string(getint());
        return s;
      case TypeKind::kReal:
        s += "real, " + std::to_string(getreal());
        return s;
      default:
        throw "debug mo match type";
    }
  }

  int getint() const { return std::get<int>(num_); }
  double getreal() const {
    if (type_ == _int) {
      return static_cast<double>(std::get<int>(num_));
    } else if (type_ == _real) {
      return std::get<double>(num_);
    } else {
      throw "GeneralArithVal bad type null";
    }
  }

  friend bool operator==(const GeneralArithVal& lhs,
                         const GeneralArithVal& rhs) {
    if (lhs.type_ != lhs.type_) {
      return false;
    }
    if (lhs.type_ == _int) {
      return lhs.getint() == rhs.getint();
    }
    if (lhs.type_ == _real) {
      return lhs.getreal() == rhs.getreal();
    }

    // _null
    return true;
  }

  friend bool operator!=(const GeneralArithVal& lhs,
                         const GeneralArithVal& rhs) {
    return !(lhs == rhs);
  }

  friend bool operator==(const GeneralArithVal& val, int i) {
    return val.type_ == _int && val.getint() == i;
  }

  friend bool operator==(int i, const GeneralArithVal& val) { return val == i; }

  friend bool operator!=(const GeneralArithVal& val, int i) {
    return !(val == i);
  }

  friend bool operator!=(int i, const GeneralArithVal& val) {
    return !(val == i);
  }

  friend bool operator==(const GeneralArithVal& val, double f) {
    return val.type_ == _real && val.getreal() == f;
  }

  friend bool operator==(double f, const GeneralArithVal& val) {
    return val == f;
  }

  friend bool operator!=(const GeneralArithVal& val, double f) {
    return !(val == f);
  }

  friend bool operator!=(double f, const GeneralArithVal& val) {
    return !(val == f);
  }

  GeneralArithVal& operator+=(const GeneralArithVal& rhs) {
    if (type_ == _real || rhs.type_ == _real) {
      num_ = getreal() + rhs.getreal();
      type_ = _real;
    } else {
      num_ = getint() + rhs.getint();
    }

    return *this;
  }

  GeneralArithVal& operator-=(const GeneralArithVal& rhs) {
    if (type_ == _real || rhs.type_ == _real) {
      num_ = getreal() - rhs.getreal();
      type_ = _real;
    } else {
      num_ = getint() - rhs.getint();
    }

    return *this;
  }

  GeneralArithVal& operator*=(const GeneralArithVal& rhs) {
    if (type_ == _real || rhs.type_ == _real) {
      num_ = getreal() * rhs.getreal();
      type_ = _real;
    } else {
      num_ = getint() * rhs.getint();
    }

    return *this;
  }

  GeneralArithVal& operator/=(const GeneralArithVal& rhs) {
    if (type_ == _real || rhs.type_ == _real) {
      num_ = getreal() / rhs.getreal();
      type_ = _real;
    } else {
      num_ = getint() / rhs.getint();
    }

    return *this;
  }

  friend GeneralArithVal operator+(GeneralArithVal lhs,
                                   const GeneralArithVal& rhs) {
    lhs += rhs;
    return lhs;
  }

  friend GeneralArithVal operator-(GeneralArithVal lhs,
                                   const GeneralArithVal& rhs) {
    lhs -= rhs;
    return lhs;
  }

  friend GeneralArithVal operator*(GeneralArithVal lhs,
                                   const GeneralArithVal& rhs) {
    lhs *= rhs;
    return lhs;
  }

  friend GeneralArithVal operator/(GeneralArithVal lhs,
                                   const GeneralArithVal& rhs) {
    lhs /= rhs;
    return lhs;
  }

 private:
  Type type_;
  std::variant<std::monostate, int, double> num_;
};

}  // namespace SI

#endif  // TYPE_H__