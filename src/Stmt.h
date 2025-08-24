#pragma once
#include <memory>
#include <vector>
#include <exception>
#include "Obj.h "
#include "Token.h"
using namespace std;
template <typename R>
class VisitorStmt;

class StmtExpression ;
class StmtPrint	   ;
class StmtVar		   ;
class StmtClass  ;
class StmtBlock	   ;
class Stmtif	   ;
class Stmtwhile  ;
class StmtReturn     ;
class StmtFunction ;
 class Stmt
{
public:
virtual ~Stmt() = default;
template <typename R>
 R accept(VisitorStmt<R>& visitor);
};
class StmtExpression : public Stmt
{
public:

StmtExpression( shared_ptr<Expr> expression):
expression(expression){
}
 shared_ptr<Expr> expression;


template <typename R>
R accept(VisitorStmt<R>& visitor)
{
return visitor.visitExpressionStmt(*this);
}
};
class StmtPrint : public Stmt
{
public:

StmtPrint( shared_ptr<Expr> expression):
expression(expression){
}
 shared_ptr<Expr> expression;


template <typename R>
R accept(VisitorStmt<R>& visitor)
{
return visitor.visitPrintStmt(*this);
}
};
class StmtVar : public Stmt
{
public:

StmtVar( Token name, shared_ptr<Expr> initializer):
name(name),initializer(initializer){
}
 Token name;
 shared_ptr<Expr> initializer;


template <typename R>
R accept(VisitorStmt<R>& visitor)
{
return visitor.visitVarStmt(*this);
}
};
class StmtClass : public Stmt
{
public:

StmtClass( Token name, shared_ptr<ExprVariable> superclass, vector<shared_ptr<StmtFunction>> methods):
name(name),superclass(superclass),methods(methods){
}
 Token name;
 shared_ptr<ExprVariable> superclass;
 vector<shared_ptr<StmtFunction>> methods;


template <typename R>
R accept(VisitorStmt<R>& visitor)
{
return visitor.visitClassStmt(*this);
}
};
class StmtBlock : public Stmt
{
public:

StmtBlock( vector<shared_ptr<Stmt>> statements):
statements(statements){
}
 vector<shared_ptr<Stmt>> statements;


template <typename R>
R accept(VisitorStmt<R>& visitor)
{
return visitor.visitBlockStmt(*this);
}
};
class Stmtif : public Stmt
{
public:

Stmtif( shared_ptr<Expr> condition, shared_ptr<Stmt> thenBranch, shared_ptr<Stmt> elseBranch):
condition(condition),thenBranch(thenBranch),elseBranch(elseBranch){
}
 shared_ptr<Expr> condition;
 shared_ptr<Stmt> thenBranch;
 shared_ptr<Stmt> elseBranch;


template <typename R>
R accept(VisitorStmt<R>& visitor)
{
return visitor.visitifStmt(*this);
}
};
class Stmtwhile : public Stmt
{
public:

Stmtwhile( shared_ptr<Expr> condition, shared_ptr<Stmt> body):
condition(condition),body(body){
}
 shared_ptr<Expr> condition;
 shared_ptr<Stmt> body;


template <typename R>
R accept(VisitorStmt<R>& visitor)
{
return visitor.visitwhileStmt(*this);
}
};
class StmtReturn : public Stmt
{
public:

StmtReturn( Token keyword, shared_ptr<Expr> value):
keyword(keyword),value(value){
}
 Token keyword;
 shared_ptr<Expr> value;


template <typename R>
R accept(VisitorStmt<R>& visitor)
{
return visitor.visitReturnStmt(*this);
}
};
class StmtFunction : public Stmt
{
public:

StmtFunction( Token name, vector<Token> params, vector<shared_ptr<Stmt>> body):
name(name),params(params),body(body){
}
 Token name;
 vector<Token> params;
 vector<shared_ptr<Stmt>> body;


template <typename R>
R accept(VisitorStmt<R>& visitor)
{
return visitor.visitFunctionStmt(*this);
}
};
template <typename R>
class VisitorStmt
{
public:
virtual R visitExpressionStmt( StmtExpression& stmt) = 0;
virtual R visitPrintStmt( StmtPrint& stmt) = 0;
virtual R visitVarStmt( StmtVar& stmt) = 0;
virtual R visitClassStmt( StmtClass& stmt) = 0;
virtual R visitBlockStmt( StmtBlock& stmt) = 0;
virtual R visitifStmt( Stmtif& stmt) = 0;
virtual R visitwhileStmt( Stmtwhile& stmt) = 0;
virtual R visitReturnStmt( StmtReturn& stmt) = 0;
virtual R visitFunctionStmt( StmtFunction& stmt) = 0;
};
template <typename R>
 R Stmt::accept(VisitorStmt<R>& visitor)
{
if (auto* expr = dynamic_cast<StmtExpression*>(this)){
return visitor.visitExpressionStmt(*expr);}
if (auto* expr = dynamic_cast<StmtPrint*>(this)){
return visitor.visitPrintStmt(*expr);}
if (auto* expr = dynamic_cast<StmtVar*>(this)){
return visitor.visitVarStmt(*expr);}
if (auto* expr = dynamic_cast<StmtClass*>(this)){
return visitor.visitClassStmt(*expr);}
if (auto* expr = dynamic_cast<StmtBlock*>(this)){
return visitor.visitBlockStmt(*expr);}
if (auto* expr = dynamic_cast<Stmtif*>(this)){
return visitor.visitifStmt(*expr);}
if (auto* expr = dynamic_cast<Stmtwhile*>(this)){
return visitor.visitwhileStmt(*expr);}
if (auto* expr = dynamic_cast<StmtReturn*>(this)){
return visitor.visitReturnStmt(*expr);}
if (auto* expr = dynamic_cast<StmtFunction*>(this)){
return visitor.visitFunctionStmt(*expr);}
throw std::logic_error("Unknown expression type");
}
