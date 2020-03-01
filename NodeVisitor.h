#ifndef NODE_VISITOR_H__
#define NODE_VISITOR_H__

#include <map>
#include <string>

namespace SI {
namespace Interpreter {

class ASTNodeVisitor {
 public:
  ASTNodeVisitor() = default;
  void visitCompound(const class Compound &com);
  void visitAssignment(const class Assignment &assign);
  int visitArithExpr(const class ASTNode &ast);
  int visitBinOp(const class BinaryOp &op);
  int visitUnaryOp(const class UnaryOp &op);
  int visitNumber(const class Number &num);
  void visitProgram(const class Program &prog);
  void visitBlock(const class Block &block);
  void visitDecl(const class Declaration &decl);
  int visitVar(const class Var &var);
  const std::map<std::string, int> &symbols() const { return symbolTable_; }

 private:
  std::map<std::string, int> symbolTable_;
};

}  // namespace Interpreter
}  // namespace SI

#endif  // NODE_VISITOR_H__