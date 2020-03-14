#ifndef AST_VISITOR_BASE_H__
#define AST_VISITOR_BASE_H__

#include <variant>

#include "Type.h"
#include "util.h"

namespace SI {

class ASTVisitorBase : public SI::util::Noncopyable {
 public:
  virtual void visitCompound(const class Compound &com) = 0;
  virtual void visitAssignment(const class Assignment &assign) = 0;
  virtual GeneralArithVal visitArithExpr(const class ASTNode &ast) = 0;
  virtual GeneralArithVal visitBinOp(const class BinaryOp &op) = 0;
  virtual GeneralArithVal visitUnaryOp(const class UnaryOp &op) = 0;
  virtual GeneralArithVal visitNumber(const class Number &num) = 0;
  virtual void visitProgram(const class Program &prog) = 0;
  virtual void visitBlock(const class Block &block) = 0;
  virtual void visitDecl(const class Declaration &decl) = 0;
  virtual GeneralArithVal visitID(const class ID &var) = 0;
  virtual void visitProcedureDecl(const class ProcedureDecl &decl) = 0;
  virtual void visitParam(const class Param &param) = 0;
  virtual void visitProcedureCall(const class ProcedureCall &call) = 0;
};

}  // namespace SI

#endif  // AST_VISITOR_BASE_H__