#include <utility>
#include <iostream>
#include <sstream>
#include <string>

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

class RecordAnalyzer {
public:
    RecordAnalyzer(clang::CXXRecordDecl& decl):
        decl{decl}
    {}

    void print() const {
        llvm::errs() << "--------------------\n";
        decl.dump();
        llvm::errs() << "--------------------\n";
        for (auto const& field : decl.fields())
            field->dump();
        llvm::errs() << "--------------------\n";
    }

protected:
    clang::CXXRecordDecl& decl;
};

// specify which nodes of the AST to look at
class MyVisitor : public clang::RecursiveASTVisitor<MyVisitor> {
public:
    MyVisitor(clang::Rewriter& r)
        : r{r}
    {}

    bool VisitCXXRecordDecl(clang::CXXRecordDecl* decl) {
        llvm::errs() << "** found a CXX Record Decl\n";
        llvm::errs() << "class/struct " << decl->getQualifiedNameAsString() << "\n";
        RecordAnalyzer{*decl}.print();

        return true;
    }

    bool VisitStmt(clang::Stmt* s) {
        llvm::errs() << "MyVisitor::VisitStmt\n"; 
        if (clang::isa<clang::IfStmt>(s)) {
            auto* ifstmt = clang::cast<clang::IfStmt>(s);
            auto* then = ifstmt->getThen();

            r.InsertText(then->getLocStart(), 
                         "// the 'if' part\n",
                         true, true);

            auto* Else = ifstmt->getElse();
            if (Else)
                r.InsertText(Else->getLocStart(),
                             "// the 'else' part\n",
                             true, true);
        }

        return true;
    }

    bool VisitFunctionDecl(clang::FunctionDecl* f) {
        llvm::errs() << "MyVisitor::VisitFunctionDecl\n";
        if (f->hasBody()) {
            clang::Stmt* func_body = f->getBody();

            // type nmae as string
            clang::QualType qt = f->getReturnType();
            std::string type_str = qt.getAsString();

            // function name
            clang::DeclarationName decl_name = f->getNameInfo().getName();
            std::string func_name = decl_name.getAsString();

            // add comment before
            std::stringstream ssbefore;
            ssbefore << "// begin function " << func_name << " returning " 
                     << type_str << "\n";
            clang::SourceLocation st= f->getSourceRange().getBegin();
            r.InsertText(st, ssbefore.str(), true, true);

            // and after
            std::stringstream ssafter;
            ssafter << "\n// end function " << func_name;
            st = func_body->getLocEnd().getLocWithOffset(1);
            r.InsertText(st, ssafter.str(), true, true);
        }

        return true;
    }

private:
    clang::Rewriter &r;
};

class MyConsumer : public clang::ASTConsumer {
public:
    MyConsumer(clang::Rewriter &r) 
        : visitor{r}
    {}

    // gets called for each parsed top-level declaration
    bool HandleTopLevelDecl(clang::DeclGroupRef dr) override {
        llvm::errs() << "** MyConsumer::HandleTopLevelDecl\n";
        for (clang::DeclGroupRef::iterator b=dr.begin(), e=dr.end(); b != e; ++b) {
            visitor.TraverseDecl(*b);
        }
        
        return true;
    }

    /*
    void HandleTranslationUnit(clang::ASTContext& ctx) override {
        llvm::errs() << "** MyConsumer::HandleTranslationUnit\n";
        visitor.TraverseDecl(ctx.getTranslationUnitDecl());
    }*/

private:
    MyVisitor visitor;
};

class MyFrontendAction : public clang::ASTFrontendAction {
public:
    MyFrontendAction() {}
    
    void EndSourceFileAction() override {
        clang::SourceManager& sm = r.getSourceMgr();
        llvm::errs() << "** EndSourceFileAction for: "
                     << sm.getFileEntryForID(sm.getMainFileID())->getName() << "\n";

        // emit the rewritten buffer

        r.getEditBuffer(sm.getMainFileID()).write(llvm::outs());
    }

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
            clang::CompilerInstance& ci,
            llvm::StringRef file) override {
        llvm::errs() << "** Creating AST consumer for: " << file << "\n";
        r.setSourceMgr(ci.getSourceManager(), ci.getLangOpts());
        return std::make_unique<MyConsumer>(r);
    }

private:
    clang::Rewriter r;
};

static llvm::cl::OptionCategory tooling_sample_category{"Tooling Sample"};

int main(int argc, char const** argv) {
    clang::tooling::CommonOptionsParser op{argc, argv, tooling_sample_category};
    clang::tooling::ClangTool Tool{op.getCompilations(), op.getSourcePathList()};

    return Tool.run(clang::tooling::newFrontendActionFactory<MyFrontendAction>().get());
}
