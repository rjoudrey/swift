#include "MyExtension.h"
#include "swift/Frontend/Frontend.h"
#include "swift/AST/ASTVisitor.h"

using namespace swift;
using namespace std;

SourceManager *SM;

vector<Var> vars;

string sourceRangeToString(SourceRange sourceRange) {
    return SM->extractText(Lexer::getCharSourceRangeFromSourceRange(*SM, sourceRange)).str();
}

string varTypeName(VarDecl *V) {
    auto SR = V->getTypeSourceRangeForDiagnostics();
    return sourceRangeToString(SR);
}

class MyVisitor : public DeclVisitor<MyVisitor> {
public:
  void visit(const Decl *D) {
      auto kind = D->getDescriptiveKind();
      if (kind != DescriptiveDeclKind::Var && kind != DescriptiveDeclKind::Let) {
          return;
      }
      auto V = (VarDecl *)D;

      auto VtypeStr = varTypeName(V).c_str();
      char* Vtype = new char[strlen(VtypeStr) + 1]; // +1 for terminating NUL
      // Copy source string in dynamically allocated string buffer
      strcpy(Vtype, VtypeStr);

      auto Vname = V->getName().get();
      vars.push_back({ Vname, Vtype });
  }
};

Var* getVars(const char *code, int *count) {
    CompilerInstance CI;
    CompilerInvocation Invocation;
    Invocation.setModuleName("main");
    vars.clear();

    std::unique_ptr<llvm::MemoryBuffer> Buf = llvm::MemoryBuffer::getMemBuffer(code);
    
    Invocation.addInputBuffer(Buf.get());
    SM = &CI.getSourceMgr();
    SM->addNewSourceBuffer(std::move(Buf));;

    CI.setup(Invocation);
    CI.performParseOnly();

    auto *M = CI.getMainModule();
    SmallVector<Decl *, 1> Decls;
    M->getDisplayDecls(Decls);
    
    MyVisitor myVisitor;
    for (auto *D : Decls) {
        myVisitor.visit(D);
    }
    *count = vars.size();
    if (vars.empty()) {
        return NULL;
    }
    return &vars[0];
}
