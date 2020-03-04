#ifndef TYPE_H__
#define TYPE_H__

namespace SI {

class Type {
 public:
  enum Kind { kNull = 0, kInt, kReal };
  Type(Kind kind = Kind::kNull) : kind_(kind) {}
  static Type getArithType(const Type &left, const Type &right);

 private:
  Kind kind_;
};

extern const Type _null;
extern const Type _int;
extern const Type _real;

}  // namespace SI

#endif  // TYPE_H__