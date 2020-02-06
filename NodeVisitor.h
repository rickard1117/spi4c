#ifndef NODE_VISITOR_H__
#define NODE_VISITOR_H__

#include <map>
#include <string>
#include <variant>

namespace SI {
namespace Interpreter {

template <typename T1, typename T2>
class NumWrapper {
    using CommonT = std::common_type_t<T1, T2>;
    using LeftT = typename TypeLeft<T1, T2, CommonT>::type;
  public:
    explicit NumWrapper(T1 t):val_(t) {}
    explicit NumWrapper(T2 t):val_(t) {}
    NumWrapper< T1,  T2> operator+(const NumWrapper<T1, T2> &r)
    {
      return NumWrapper<T1, T2>(val_ + r.val_);
    }
    NumWrapper< T1,  T2> operator-(const NumWrapper<T1, T2> &r)
    {
      return NumWrapper<T1, T2>(val_ - r.val_);
    }
    NumWrapper< T1,  T2> operator*(const NumWrapper<T1, T2> &r)
    {
      return NumWrapper<T1, T2>(val_ * r.val_);
    }
    NumWrapper< T1,  T2> operator/(const NumWrapper<T1, T2> &r)
    {
      return NumWrapper<T1, T2>(val_ / r.val_);
    }
  private:
    CommonT val_;
};
using Num = NumWrapper<int, double>;

// class BinOpVisitor {
// public:
//   Num visitBinOp(const class AST &);
// };

class NodeVisitor {
 public:
  // int visit(const class Token &);
  // int visit(const class Num &);
  // int visit(const class BinOp &);
  // int visit(const class UnaryOp &);
  // int visit(const class Var &);
  // int visit(const class Assign &);
  // int visit(const class Compound &);
  // int visit(const class NoOp &);
  // int visit(const class Block &);
  // int visit(const class VarDecl &);
  // using std::variant<int, double> 
  Num visit(const class AST &);
  // Num visitBinOp(const class AST &);
  const std::map<std::string, int> &varsTable() const { return varsTable_; }

 private:
  std::string currentVar_;
  std::map<std::string, int> varsTable_;
};
}  // namespace Interpreter
}  // namespace SI

#endif  // NODE_VISITOR_H__