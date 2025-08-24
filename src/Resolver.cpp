//
// Alternative approach: Track current expression during resolution
//

#include "Resolver.h"
#include "lox.h"

Resolver::Resolver(Interpreter &interpreter) : interpreter(interpreter)
{
}

void Resolver::resolve(std::vector<std::shared_ptr<Stmt>> statements)
{
	for (auto statement : statements)
	{
		resolve(statement);
	}
}

void Resolver::resolve(std::shared_ptr<Stmt> stmt)
{
	stmt->accept(*this);
}

void Resolver::resolve(std::shared_ptr<Expr> expr)
{
	exprStack.push(expr);
	expr->accept(*this);
	exprStack.pop();
}

void Resolver::beginScope()
{
	scopes.push_back(std::map<std::string, bool>());
}

void Resolver::endScope()
{
	scopes.pop_back();
}

void Resolver::declare(const Token& name)
{
	if (scopes.empty()) return;

	std::map<std::string, bool>& scope = scopes.back();

	if (scope.find(name.getLexme()) != scope.end())
	{
		errorL(name, "Already a variable with this name in this scope.");
	}

	scope[name.getLexme()] = false;
}

void Resolver::define(const Token& name)
{
	if (scopes.empty()) return;
	scopes.back()[name.getLexme()] = true;
}

void Resolver::resolveLocal(std::shared_ptr<Expr> expr, const Token& name)
{
	for (int i = scopes.size() - 1; i >= 0; i--)
	{
		if (scopes[i].find(name.getLexme()) != scopes[i].end())
		{
			interpreter.resolve(expr, scopes.size() - 1 - i);
			return;
		}
	}
}

void Resolver::resolveFunction(std::shared_ptr<StmtFunction> function, FunctionType type)
{
	FunctionType enclosingFunction = currentFunction;
	currentFunction = type;

	beginScope();
	for (const Token& param : function->params)
	{
		declare(param);
		define(param);
	}
	resolve(function->body);
	endScope();
	currentFunction = enclosingFunction;
}

void Resolver::visitBlockStmt(StmtBlock &stmt)
{
	beginScope();
	resolve(stmt.statements);
	endScope();
}

void Resolver::visitVarStmt(StmtVar &stmt)
{
	declare(stmt.name);
	if (stmt.initializer != nullptr)
	{
		resolve(stmt.initializer);
	}
	define(stmt.name);
}

void Resolver::visitGetExpr(ExprGet &expr)
{
	resolve(expr.object);
}

void Resolver::visitClassStmt(StmtClass &stmt)
{
	ClassType enclosingClass = currentClass;
	currentClass = ClassType::CLASS;
	declare(stmt.name);
	define(stmt.name);

	if (stmt.superclass != nullptr&&
		stmt.name.getLexme() == stmt.superclass->name.getLexme())
	{
		errorL(stmt.superclass->name, "Can't inherit from yourself.");
	}
	if (stmt.superclass != nullptr)
	{
		currentClass = ClassType::SUBCLASS;
		resolve(stmt.superclass);
	}

	if (stmt.superclass != nullptr)
	{
		beginScope();
		scopes.back()["super"] = true;
	}

	beginScope();

	scopes.back()["this"] = true;
	for (auto stmt: stmt.methods)
	{
		FunctionType declaration = FunctionType::METHOD;
		if (stmt->name.getLexme() == "init")
		{
			declaration = FunctionType::INITIALIZER;
		}
		resolveFunction(stmt, declaration);
	}
	endScope();
	if (stmt.superclass != nullptr)
	{
		endScope();
	}
	currentClass = enclosingClass;
}

void Resolver::visitSetExpr(ExprSet &expr)
{
	resolve(expr.value);
	resolve(expr.object);
}

void Resolver::visitThisExpr(ExprThis &expr)
{
	if (currentClass == ClassType::NONE)
	{
		errorL(expr.keyword,"Can't use 'this' outside of a class.");
	}
	resolveLocal(exprStack.top(), expr.keyword);
}

void Resolver::visitSuperExpr(ExprSuper &expr)
{
	if (currentClass == ClassType::NONE)
	{
		errorL(expr.keyword, "Can't use 'super' outside of a class.");
	}
	else if (currentClass != ClassType::SUBCLASS)
	{
		errorL(expr.keyword, "Can't use 'super' in a class with no superclass.");
	}
	resolveLocal(exprStack.top(),expr.keyword);
}

void Resolver::visitVariableExpr(ExprVariable &expr)
{
	if (!scopes.empty() &&
		scopes.back().find(expr.name.getLexme()) != scopes.back().end() &&
		scopes.back()[expr.name.getLexme()] == false)
	{
		errorL(expr.name, "Can't read local variable in its own initializer.");
	}

	// Use the expression from the top of the stack
	if (!exprStack.empty()) {
		resolveLocal(exprStack.top(), expr.name);
	}
}

void Resolver::visitAssignExpr(ExprAssign &expr)
{
	resolve(expr.value);
	// Use the expression from the top of the stack
	if (!exprStack.empty()) {
		resolveLocal(exprStack.top(), expr.name);
	}
}

void Resolver::visitFunctionStmt(StmtFunction &stmt)
{
	declare(stmt.name);
	define(stmt.name);

	resolveFunction(std::make_shared<StmtFunction>(stmt), FunctionType::FUNCTION);
}

void Resolver::visitExpressionStmt(StmtExpression &stmt)
{
	resolve(stmt.expression);
}

void Resolver::visitifStmt(Stmtif &stmt)
{
	resolve(stmt.condition);
	resolve(stmt.thenBranch);
	if (stmt.elseBranch != nullptr) resolve(stmt.elseBranch);
}

void Resolver::visitPrintStmt(StmtPrint &stmt)
{
	resolve(stmt.expression);
}

void Resolver::visitReturnStmt(StmtReturn &stmt)
{
	if (currentFunction == FunctionType::NONE)
	{
		errorL(stmt.keyword, "Can't return from top-level code.");
	}

	if (stmt.value != nullptr)
	{
		if (currentFunction == FunctionType::INITIALIZER)
		{
			errorL(stmt.keyword,"Can't return a value from an initializer.");
		}
		resolve(stmt.value);
	}
}

void Resolver::visitwhileStmt(Stmtwhile &stmt)
{
	resolve(stmt.condition);
	resolve(stmt.body);
}

void Resolver::visitBinaryExpr(ExprBinary &expr)
{
	resolve(expr.left);
	resolve(expr.right);
}

void Resolver::visitCallExpr(ExprCall &expr)
{
	resolve(expr.calle);

	for (auto argument : expr.arguments)
	{
		resolve(argument);
	}
}

void Resolver::visitGroupingExpr(ExprGrouping &expr)
{
	resolve(expr.expression);
}

void Resolver::visitLiteralExpr(ExprLiteral &expr)
{
	// Nothing to resolve.
}

void Resolver::visitLogicalExpr(ExprLogical &expr)
{
	resolve(expr.left);
	resolve(expr.right);
}

void Resolver::visitUnaryExpr(ExprUnary &expr)
{
	resolve(expr.right);
}