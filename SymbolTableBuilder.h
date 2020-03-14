#ifndef SYMBOL_TABLE_BUILDER_H__
#define SYMBOL_TABLE_BUILDER_H__

#include "ASTVisitorBase.h"
#include "SymbolTable.h"

namespace SI {

class SymbolTableBuilder : public ASTVisitorBase {
 public:
  SymbolTableBuilder() = default;
  void visitCompound(const class Compound &com) override;
  void visitAssignment(const class Assignment &assign) override;
  GeneralArithVal visitArithExpr(const class ASTNode &ast) override;
  GeneralArithVal visitBinOp(const class BinaryOp &op) override;
  GeneralArithVal visitUnaryOp(const class UnaryOp &op) override;
  GeneralArithVal visitNumber(const class Number &num) override;
  void visitProgram(const class Program &prog) override;
  void visitBlock(const class Block &block) override;
  void visitDecl(const class Declaration &decl) override;
  GeneralArithVal visitID(const class ID &var) override;
  void visitProcedureDecl(const class ProcedureDecl &decl) override;
  void visitParam(const class Param &param) override;
  void visitProcedureCall(const class ProcedureCall &call) override;
  
 private:
  void enterNewScope(const std::string &name);
  void exitScope();
  std::shared_ptr<ScopedSymbolTable> currentTable_;
};

}  // namespace SI

#endif  // SYMBOL_TABLE_BUILDER_H__
