//
// Created by 123ja on 8/23/2025.
//

#ifndef INTERPETER_CPP_RESOLVER_H
#define INTERPETER_CPP_RESOLVER_H
#include "Interpreter.h"
#include "Stmt.h"
#include <stack>
enum class FunctionType
{
	NONE,
	FUNCTION,
	METHOD,
	INITIALIZER
};
enum class ClassType
{
	NONE,
	CLASS,
	SUBCLASS
};
class Resolver : VisitorExpr<void>, VisitorStmt<void>
{
public:
	explicit Resolver( Interpreter &interpreter);
	virtual ~Resolver() = default;
	void visitwhileStmt(Stmtwhile &stmt) override;
	void visitAssignExpr(ExprAssign &expr) override;
	void visitBinaryExpr(ExprBinary &expr) override;
	void visitBlockStmt(StmtBlock &stmt) override;
	void visitCallExpr(ExprCall &expr) override;
	void visitExpressionStmt(StmtExpression &stmt) override;
	void visitFunctionStmt(StmtFunction &stmt) override;
	void visitGroupingExpr(ExprGrouping &expr) override;
	void visitifStmt(Stmtif &stmt) override;
	void visitLiteralExpr(ExprLiteral &expr) override;
	void visitLogicalExpr(ExprLogical &expr) override;
	void visitPrintStmt(StmtPrint &stmt) override;
	void visitReturnStmt(StmtReturn &stmt) override;
	void visitUnaryExpr(ExprUnary &expr) override;
	void visitVariableExpr(ExprVariable &expr) override;
	void visitVarStmt(StmtVar &stmt) override;
	void visitGetExpr(ExprGet &expr) override;
	void visitClassStmt(StmtClass &stmt) override;
	void visitSetExpr(ExprSet &expr) override;
	void visitThisExpr(ExprThis &expr) override;
	void visitSuperExpr(ExprSuper &expr) override;

	void resolve(std::vector<std::shared_ptr<Stmt>> statements);
	void resolve(std::shared_ptr<Stmt> stmt);
	void resolve(std::shared_ptr<Expr> expr);
	void resolveLocal(std::shared_ptr<Expr> expr, const Token& name);
	void resolveFunction(std::shared_ptr<StmtFunction> function,FunctionType type);

	void beginScope();
	void endScope();

	void declare(const Token& name);
	void define(const Token& name);

private:
	Interpreter& interpreter;
	std::vector<std::map<std::string, bool>> scopes;
	FunctionType currentFunction = FunctionType::NONE;
	std::stack<std::shared_ptr<Expr>> exprStack;
	ClassType currentClass = ClassType::NONE;
};
#endif //INTERPETER_CPP_RESOLVER_H