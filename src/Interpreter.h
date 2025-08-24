//
// Created by 123ja on 8/20/2025.
//

#ifndef INTERPETER_CPP_INTERPRETER_H
#define INTERPETER_CPP_INTERPRETER_H
#include  "Expr.h"
#include  "Obj.h"
#include "lox.h"
#include  <memory>
#include <vector>
#include "LoxCallable.h"
#include "Env.h"
#include <stack>
#include <chrono>
#include  "RuntimeError.h"
#include "Stmt.h"


class Interpreter:VisitorExpr<Obj>,VisitorStmt<void>
{
public:
	Interpreter();

	void interpret(std::vector<std::shared_ptr<Stmt>> statements);

	void execute(std::shared_ptr<Stmt> stmt);

	void visitExpressionStmt(StmtExpression &stmt) override;
	void visitClassStmt(StmtClass &stmt) override;
	void visitPrintStmt(StmtPrint &stmt) override;
	void visitVarStmt(StmtVar &stmt) override;
	void visitBlockStmt(StmtBlock &stmt) override;
	void visitifStmt(Stmtif &stmt) override;
	void visitwhileStmt(Stmtwhile &stmt) override;
	void visitFunctionStmt(StmtFunction &stmt) override;
	void visitReturnStmt(StmtReturn &stmt) override;

	Obj visitGetExpr(ExprGet &expr) override;
	Obj visitSetExpr(ExprSet &expr) override;
	Obj visitBinaryExpr(ExprBinary &expr) override;
	Obj visitGroupingExpr(ExprGrouping &expr) override;
	Obj visitLiteralExpr(ExprLiteral &expr) override;
	Obj visitUnaryExpr(ExprUnary &expr) override;
	Obj visitVariableExpr(ExprVariable &expr) override;
	Obj visitThisExpr(ExprThis &expr) override;
	Obj visitAssignExpr(ExprAssign &expr) override;
	Obj visitSuperExpr(ExprSuper &expr) override;
	Obj visitLogicalExpr(ExprLogical &expr) override;
	Obj visitCallExpr(ExprCall &expr) override;
	void resolve(std::shared_ptr<Expr> expr,int depth);
	void executeBlock(std::vector<shared_ptr<Stmt>> statments, std::shared_ptr<Env> environment);
private:
	Obj evaluate(std::shared_ptr<Expr> expr);
	bool isTruthy(const Obj& object);
	bool isEqual(const Obj& a,const Obj& b);

	Obj lookUpVariable(const Token& name,std::shared_ptr<Expr> expr);

	void checkNumberOperand(const Token&token,const Obj& operand);
	void checkNumberOperands(const Token&token,const Obj& left,const Obj& right);

	std::map<std::shared_ptr<Expr>,int> locals;
	std::stack<std::shared_ptr<Expr>> exprStack;
	std::shared_ptr<Env> globals;
	std::shared_ptr<Env> env;
	std::string stringify(const Obj& object);

};


#endif //INTERPETER_CPP_INTERPRETER_H