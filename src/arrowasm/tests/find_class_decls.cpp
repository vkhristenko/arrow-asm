#include <iostream>

#include "clang//AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

class FindNamedClassVisitor
    : public clang::RecursiveASTVisitor<FindNamedClassVisitor> {

public:
    FindNamedClassVisitor(clang::ASTContext* ctx, std::string const& ss)
        : ctx{ctx}, ss{ss} {}

    bool VisitCXXRecordDecl(clang::CXXRecordDecl* decl) {
        if (decl->getQualifiedNameAsString() == ss) {
            clang::FullSourceLoc full_loc = ctx->getFullLoc(decl->getLocStart());
            if (full_loc.isValid())
                llvm::outs() << "found declaration at "
                             << full_loc.getSpellingLineNumber() << ":"
                             << full_loc.getSpellingColumnNumber() << "\n";
        }

        return true;
    }

private:
    clang::ASTContext* ctx;
    std::string ss;
};

class FindNamedClassConsumer : public clang::ASTConsumer {
public:
    explicit FindNamedClassConsumer(clang::ASTContext* ctx, std::string const& ss)
        : visitor{ctx, ss} {}

    virtual void HandleTranslationUnit(clang::ASTContext& ctx) {
        visitor.TraverseDecl(ctx.getTranslationUnitDecl());
    }

private:
    FindNamedClassVisitor visitor;
};

class FindNamedClassAction : public clang::ASTFrontendAction {
public:
    explicit FindNamedClassAction(std::string const& ss)
        : ss{ss} {}

    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& compiler, llvm::StringRef inFile) {
        return std::unique_ptr<clang::ASTConsumer>{
            new FindNamedClassConsumer{&compiler.getASTContext(), ss}
        };
    }

private:
    std::string ss;
};

int main(int argc, char **argv) {
    if (argc > 2) {
        clang::tooling::runToolOnCode(new FindNamedClassAction{argv[1]}, argv[2]);
    } else {
        std::cout << "./run <class decl to search> <input stream>\n";
    }
}
