#ifndef SYMBOL_TABLE_BUILDER_H__
#define SYMBOL_TABLE_BUILDER_H__

#include "ASTVisitorBase.h"
#include "SymbolTable.h"

namespace SI {

class SymbolTableBuilder : public ASTVisitorBase {
 public:
  SymbolTableBuilder(std::shared_ptr<class SymbolTable> table)
      : table_(table) {}
  void visitCompound(const class Compound &com);
  void visitAssignment(const class Assignment &assign);
  GeneralArithVal visitArithExpr(const class ASTNode &ast);
  GeneralArithVal visitBinOp(const class BinaryOp &op);
  GeneralArithVal visitUnaryOp(const class UnaryOp &op);
  GeneralArithVal visitNumber(const class Number &num);
  void visitProgram(const class Program &prog);
  void visitBlock(const class Block &block);
  void visitDecl(const class Declaration &decl);
  GeneralArithVal visitVar(const class Var &var);

 private:
  std::shared_ptr<class SymbolTable> table_;
};

}  // namespace SI

#endif  // SYMBOL_TABLE_BUILDER_H__
