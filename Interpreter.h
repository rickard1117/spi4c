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

  const std::map<std::string, GeneralArithVal> &symbols() const {
    return symbolTable_;
  }

 private:
  std::map<std::string, GeneralArithVal> symbolTable_;
};

}  // namespace SI

#endif  // NODE_VISITOR_H__