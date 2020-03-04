#ifndef SYMBOL_TABLE_BUILDER_H__
#define SYMBOL_TABLE_BUILDER_H__

#include "ASTVisitorBase.h"

namespace SI {

class SymbolTableBuilder : public ASTVisitorBase {
 public:
  SymbolTableBuilder(std::shared_ptr<class SymbolTable> table)
      : table_(table) {}
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

 private:
  std::shared_ptr<class SymbolTable> table_;
};

}  // namespace SI

#endif  // SYMBOL_TABLE_BUILDER_H__
