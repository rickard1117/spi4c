#ifndef NODE_VISITOR_H__
#define NODE_VISITOR_H__

#include <map>
#include <string>
#include <variant>

#include "ASTVisitorBase.h"
#include "util.h"

namespace SI {

class Interpreter : public ASTVisitorBase {
 public:
  void visitCompound(const class Compound &com) override;
  void visitAssignment(const class Assignment &assign) override;
  int visitArithExpr(const class ASTNode &ast) override;
  int visitBinOp(const class BinaryOp &op) override;
  int visitUnaryOp(const class UnaryOp &op) override;
  int visitNumber(const class Number &num) override;
  void visitProgram(const class Program &prog) override;
  void visitBlock(const class Block &block) override;
  void visitDecl(const class Declaration &decl) override;
  int visitVar(const class Var &var) override;
  const std::map<std::string, int> &symbols() const { return symbolTable_; }

 private:
  std::map<std::string, int> symbolTable_;
};

}  // namespace SI

#endif  // NODE_VISITOR_H__