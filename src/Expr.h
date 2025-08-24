#pragma once
#include <memory>
#include <vector>
#include <exception>
#include "Obj.h "
#include "Token.h"
using namespace std;
template <typename R>
class VisitorExpr;

class ExprAssign ;
class ExprBinary   ;
class ExprGrouping ;
class ExprLiteral  ;
class ExprLogical;
class ExprUnary    ;
class ExprVariable ;
class ExprCall     ;
class ExprGet    ;
class ExprSet   ;
class ExprThis  ;
class ExprSuper ;
 class Expr
{
public:
virtual ~Expr() = default;
template <typename R>
 R accept(VisitorExpr<R>& visitor);
};
class ExprAssign : public Expr
{
public:

ExprAssign( Token name, shared_ptr<Expr> value):
name(name),value(value){
}
 Token name;
 shared_ptr<Expr> value;


template <typename R>
R accept(VisitorExpr<R>& visitor)
{
return visitor.visitAssignExpr(*this);
}
};
class ExprBinary : public Expr
{
public:

ExprBinary( shared_ptr<Expr> left, Token op, shared_ptr<Expr> right):
left(left),op(op),right(right){
}
 shared_ptr<Expr> left;
 Token op;
 shared_ptr<Expr> right;


template <typename R>
R accept(VisitorExpr<R>& visitor)
{
return visitor.visitBinaryExpr(*this);
}
};
class ExprGrouping : public Expr
{
public:

ExprGrouping( shared_ptr<Expr> expression):
expression(expression){
}
 shared_ptr<Expr> expression;


template <typename R>
R accept(VisitorExpr<R>& visitor)
{
return visitor.visitGroupingExpr(*this);
}
};
class ExprLiteral : public Expr
{
public:

ExprLiteral( Obj value):
value(value){
}
 Obj value;


template <typename R>
R accept(VisitorExpr<R>& visitor)
{
return visitor.visitLiteralExpr(*this);
}
};
class ExprLogical : public Expr
{
public:

ExprLogical( shared_ptr<Expr> left, Token op, shared_ptr<Expr> right):
left(left),op(op),right(right){
}
 shared_ptr<Expr> left;
 Token op;
 shared_ptr<Expr> right;


template <typename R>
R accept(VisitorExpr<R>& visitor)
{
return visitor.visitLogicalExpr(*this);
}
};
class ExprUnary : public Expr
{
public:

ExprUnary( Token op, shared_ptr<Expr> right):
op(op),right(right){
}
 Token op;
 shared_ptr<Expr> right;


template <typename R>
R accept(VisitorExpr<R>& visitor)
{
return visitor.visitUnaryExpr(*this);
}
};
class ExprVariable : public Expr
{
public:

ExprVariable( Token name):
name(name){
}
 Token name;


template <typename R>
R accept(VisitorExpr<R>& visitor)
{
return visitor.visitVariableExpr(*this);
}
};
class ExprCall : public Expr
{
public:

ExprCall( shared_ptr<Expr> calle, Token paren, vector<shared_ptr<Expr>> arguments):
calle(calle),paren(paren),arguments(arguments){
}
 shared_ptr<Expr> calle;
 Token paren;
 vector<shared_ptr<Expr>> arguments;


template <typename R>
R accept(VisitorExpr<R>& visitor)
{
return visitor.visitCallExpr(*this);
}
};
class ExprGet : public Expr
{
public:

ExprGet( shared_ptr<Expr> object, Token name):
object(object),name(name){
}
 shared_ptr<Expr> object;
 Token name;


template <typename R>
R accept(VisitorExpr<R>& visitor)
{
return visitor.visitGetExpr(*this);
}
};
class ExprSet : public Expr
{
public:

ExprSet( shared_ptr<Expr> object, Token name, shared_ptr<Expr> value):
object(object),name(name),value(value){
}
 shared_ptr<Expr> object;
 Token name;
 shared_ptr<Expr> value;


template <typename R>
R accept(VisitorExpr<R>& visitor)
{
return visitor.visitSetExpr(*this);
}
};
class ExprThis : public Expr
{
public:

ExprThis( Token keyword):
keyword(keyword){
}
 Token keyword;


template <typename R>
R accept(VisitorExpr<R>& visitor)
{
return visitor.visitThisExpr(*this);
}
};
class ExprSuper : public Expr
{
public:

ExprSuper( Token keyword, Token method):
keyword(keyword),method(method){
}
 Token keyword;
 Token method;


template <typename R>
R accept(VisitorExpr<R>& visitor)
{
return visitor.visitSuperExpr(*this);
}
};
template <typename R>
class VisitorExpr
{
public:
virtual R visitAssignExpr( ExprAssign& expr) = 0;
virtual R visitBinaryExpr( ExprBinary& expr) = 0;
virtual R visitGroupingExpr( ExprGrouping& expr) = 0;
virtual R visitLiteralExpr( ExprLiteral& expr) = 0;
virtual R visitLogicalExpr( ExprLogical& expr) = 0;
virtual R visitUnaryExpr( ExprUnary& expr) = 0;
virtual R visitVariableExpr( ExprVariable& expr) = 0;
virtual R visitCallExpr( ExprCall& expr) = 0;
virtual R visitGetExpr( ExprGet& expr) = 0;
virtual R visitSetExpr( ExprSet& expr) = 0;
virtual R visitThisExpr( ExprThis& expr) = 0;
virtual R visitSuperExpr( ExprSuper& expr) = 0;
};
template <typename R>
 R Expr::accept(VisitorExpr<R>& visitor)
{
if (auto* expr = dynamic_cast<ExprAssign*>(this)){
return visitor.visitAssignExpr(*expr);}
if (auto* expr = dynamic_cast<ExprBinary*>(this)){
return visitor.visitBinaryExpr(*expr);}
if (auto* expr = dynamic_cast<ExprGrouping*>(this)){
return visitor.visitGroupingExpr(*expr);}
if (auto* expr = dynamic_cast<ExprLiteral*>(this)){
return visitor.visitLiteralExpr(*expr);}
if (auto* expr = dynamic_cast<ExprLogical*>(this)){
return visitor.visitLogicalExpr(*expr);}
if (auto* expr = dynamic_cast<ExprUnary*>(this)){
return visitor.visitUnaryExpr(*expr);}
if (auto* expr = dynamic_cast<ExprVariable*>(this)){
return visitor.visitVariableExpr(*expr);}
if (auto* expr = dynamic_cast<ExprCall*>(this)){
return visitor.visitCallExpr(*expr);}
if (auto* expr = dynamic_cast<ExprGet*>(this)){
return visitor.visitGetExpr(*expr);}
if (auto* expr = dynamic_cast<ExprSet*>(this)){
return visitor.visitSetExpr(*expr);}
if (auto* expr = dynamic_cast<ExprThis*>(this)){
return visitor.visitThisExpr(*expr);}
if (auto* expr = dynamic_cast<ExprSuper*>(this)){
return visitor.visitSuperExpr(*expr);}
throw std::logic_error("Unknown expression type");
}
