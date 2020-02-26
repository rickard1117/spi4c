#ifndef NODE_VISITOR_H__
#define NODE_VISITOR_H__

#include <map>
#include <string>

namespace SI {
namespace Interpreter {

class ASTNodeVisitor {
 public:
  ASTNodeVisitor() = default;
  void visit(const class ASTNode &ast);
  void visitCompound(const ASTNode &ast);
  void visitAssignment(const ASTNode &ast);
  int visitArith(const ASTNode &ast);
  void visitProgram(const ASTNode &prog);
  void visitBlock(const ASTNode &block);
  void visitDecl(const ASTNode &decl);
  const std::map<std::string, int> &symbols() const { return symbolTable_; }

 private:
  std::map<std::string, int> symbolTable_;
};

}  // namespace Interpreter
}  // namespace SI

#endif  // NODE_VISITOR_H__