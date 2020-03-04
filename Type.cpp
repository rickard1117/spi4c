#include "Type.h"

namespace SI {

const Type arithmeticResultType[3][3] = {
    //          null,  int,   real
    /* null */ {_null, _null, _null},
    /* int  */ {_null, _int, _real},
    /* real */ {_null, _real, _real}};

extern const Type _null = Type(Type::Kind::kNull);
extern const Type _int = Type(Type::Kind::kInt);
extern const Type _real = Type(Type::Kind::kReal);

Type Type::getArithType(const Type &left, const Type &right) {
  return arithmeticResultType[left.kind_][right.kind_];
}

}  // namespace SI