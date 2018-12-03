#include <utility>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/DeclVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "arrow/type.h"
#include "arrow/pretty_print.h"

struct output_streams {
    std::stringstream out;
    std::stringstream types;
};

// specify which nodes of the AST to look at
class MyVisitor : public clang::RecursiveASTVisitor<MyVisitor> {
public:
    MyVisitor(clang::Rewriter& r, std::map<std::string, 
        std::shared_ptr<arrow::DataType>>& types,
        output_streams& outs)
        : rwr{r}, types{types}, outs{outs}
    {}

    bool VisitCXXRecordDecl(clang::CXXRecordDecl* decl) {
        llvm::errs() << "** found a CXX Record Decl\n";
        llvm::errs() << "class/struct " << decl->getQualifiedNameAsString() << "\n";

        // if such a declaration already exists, throw
        if (types.find(decl->getQualifiedNameAsString()) != types.end())
            throw std::runtime_error{"repeatition of class/struct declaration for " 
                + decl->getQualifiedNameAsString()};

        std::vector<std::shared_ptr<arrow::Field>> fields;

        // initialize a function
        outs.out << 
            "template<>\n"
            "void to_arrow_builder(arrow::StructBuilder* pbuilder, " + decl->getQualifiedNameAsString() + " const& s) {\n"
            "\tpbuilder->Append();\n";
        outs.types <<
            "template<>\n"
            "struct to_arrow_type<" << decl->getQualifiedNameAsString() << "> {\n"
            "\tstatic std::shared_ptr<arrow::StructType> get() {\n"
            "\t\treturn std::make_shared<arrow::StructType>(std::vector<std::shared_ptr<arrow::Field>>{\n";
        
        // iterate and populate
        int ifield = 0;
        for (auto const& field : decl->fields()) {
            clang::QualType qtype = field->getType();
            clang::Type const* type = qtype.getTypePtr();
            llvm::errs() << "field type " << qtype.getAsString() << "\n";

            //
            // assume:
            // - no pointers/references
            // - no inheritance
            // - no 
            //
            if (type->isFundamentalType() or type->isEnumeralType()) {
                clang::BuiltinType const* btype = type->castAs<clang::BuiltinType>();
                switch (btype->getKind()) {
                    case clang::BuiltinType::Int:
                        fields.push_back(std::make_shared<arrow::Field>(
                            field->getName().str(), 
                            std::make_shared<arrow::Int32Type>()));
                        outs.out <<
                            "\tstatic_cast<arrow::Int32Builder*>(pbuilder->field_builder( " 
                            << ifield 
                            << " ))->Append( s." << field->getName().str()
                            << " );\n";
                        outs.types << "\t\t\t";
                        if (ifield>0) outs.types<< ",";
                        outs.types <<
                            "std::make_shared<arrow::Field>(\" " + field->getName().str()
                            << " \", std::make_shared<arrow::Int32Type>(), false)\n";
                        break;
                    case clang::BuiltinType::Float:
                        fields.push_back(std::make_shared<arrow::Field>(
                            field->getName().str(), 
                            std::make_shared<arrow::FloatType>()));
                        outs.out <<
                            "\tstatic_cast<arrow::FloatBuilder*>(pbuilder->field_builder( " 
                            << ifield 
                            << " ))->Append( s." << field->getName().str()
                            << " );\n";
                        outs.types << "\t\t\t";
                        if (ifield>0) outs.types << ",";
                        outs.types <<
                            "std::make_shared<arrow::Field>(\" " + field->getName().str()
                            << " \", std::make_shared<arrow::FloatType>(), false)\n";
                        break;
                    default:
                        throw std::runtime_error{"no proper type conversion for type: "};
                }
            } else {
                fields.push_back(
                    std::make_shared<arrow::Field>(
                        field->getName(), types[qtype.getAsString()])
                );
                outs.out <<
                    "\tto_arrow_builder(static_cast<arrow::StructBuilder*>(pbuilder"
                    "->field_builder( "
                    << ifield 
                    << " )), s." << field->getName().str()
                    << " );\n";
                outs.types << "\t\t\t";
                if (ifield>0) outs.types << ",";
                outs.types <<
                    "std::make_shared<arrow::Field>(\" " + field->getName().str()
                    << " \", to_arrow_type< " << qtype.getAsString() << " >::get()"
                    ", false)\n"; 
            }
            ifield++;

        }

        // finalize a fucntion
        outs.out << "}\n\n";
        outs.types << "\t\t});\n\t}\n};\n\n";

        //
        // create an entry
        //
        types[decl->getQualifiedNameAsString()] = 
            std::make_shared<arrow::StructType>(fields);

        return true;
    }

    /*
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
    }*/

private:
    clang::Rewriter &rwr;
    std::map<std::string, std::shared_ptr<arrow::DataType>>& types;
    output_streams& outs;
};

class MyConsumer : public clang::ASTConsumer {
public:
    MyConsumer(clang::Rewriter &r, 
        std::map<std::string, std::shared_ptr<arrow::DataType>>& types,
        output_streams& outs) 
        : visitor{r, types, outs}
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

class ArrowASMAction : public clang::ASTFrontendAction {
public:
    ArrowASMAction() {
        outs.out << 
            "template<typename T>\n"
            "void to_arrow_builder(arrow::StructBuilder*, T const&);\n";
        outs.types <<
            "template<typename T>\n"
            "struct to_arrow_type {\n"
            "\tstatic std::shared_ptr<arrow::StructType> get();\n"
            "};\n";
    }

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
            clang::CompilerInstance& ci, llvm::StringRef file) override {
        llvm::errs() << "*** create ast consumer for " << file << "\n";
        rwr.setSourceMgr(ci.getSourceManager(), ci.getLangOpts());
        return std::make_unique<MyConsumer>(rwr, types, outs);
    }

    void EndSourceFileAction() override {
        clang::SourceManager& sm = rwr.getSourceMgr();
        rwr.getEditBuffer(sm.getMainFileID()).write(llvm::outs());

        /*
        for (auto const& [key, value] : types) {
            std::cout << "struct " << key << std::endl;
            auto schema = std::make_shared<arrow::Schema>(value->children());
            arrow::PrettyPrint(*schema, {2}, &(std::cout));
            std::cout << "\n";
        }*/

        std::cout << "\n" << outs.out.str() << std::endl;
        std::cout << "\n\n\n";
        std::cout << "\n" << outs.types.str() << std::endl;
    }

private:
    clang::Rewriter rwr;
    output_streams outs;
    std::map<std::string, std::shared_ptr<arrow::DataType>> types;
};

static llvm::cl::OptionCategory tooling_sample_category{"Tooling Sample"};

int main(int argc, char const** argv) {
    clang::tooling::CommonOptionsParser op{argc, argv, tooling_sample_category};
    clang::tooling::ClangTool Tool{op.getCompilations(), op.getSourcePathList()};

    return Tool.run(clang::tooling::newFrontendActionFactory<ArrowASMAction>().get());
}
