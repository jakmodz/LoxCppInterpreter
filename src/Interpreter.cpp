//
// Created by 123ja on 8/20/2025.
//

#include "Interpreter.h"
#include "LoxFunction.h"
#include <iostream>
#include "Obj.h"
#include "LoxCallable.h"
#include "ClockCallable.h"
#include "LoxClass.h"
#include "LoxInstance.h"
#include "ReturnValue.h"
Interpreter::Interpreter()
{
	globals = std::make_shared<Env>();
	env = globals;

	globals->define(Token(TokenType::IDENTIFIER,"clock" , nullptr, 1),
				  Obj(std::make_shared<ClockCallable>()));

}

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> statements)
{
	try
	{
		for (auto stmt:statements)
		{
			execute(stmt);
		}

	}
	catch ( RuntimeError& error)
	{
		runtimeError(error);
	}
}

void Interpreter::execute(std::shared_ptr<Stmt> stmt)
{
	stmt->accept(*this);
}

void Interpreter::visitExpressionStmt(StmtExpression &stmt)
{
	evaluate(stmt.expression);
}

void Interpreter::visitClassStmt(StmtClass &stmt)
{
	Obj superclass;
	std::shared_ptr<LoxClass> superclassPtr = nullptr;


	if (stmt.superclass != nullptr)
	{
		superclass = evaluate(stmt.superclass);

		if (!superclass.isTypeOf(Type::Fn))
		{
			throw RuntimeError(stmt.superclass->name,"Superclass must be a class.");
		}

		superclassPtr = std::dynamic_pointer_cast<LoxClass>(superclass.getCallable());
	}

	env->define(stmt.name,Obj());
	if (stmt.superclass != nullptr)
	{
		env = std::make_shared<Env>(env);
		env->define(Token(TokenType::IDENTIFIER, "super", nullptr, 0), superclass);
	}

	std::map<std::string,std::shared_ptr<LoxFunction>> methods;
	for (auto method: stmt.methods)
	{
		auto fn = std::make_shared<LoxFunction>(method, env,method->name.getLexme() == "init");
		methods[method->name.getLexme()] = fn;
	}

	LoxClass klass = LoxClass(stmt.name.getLexme(), superclassPtr, methods);

	if (superclassPtr != nullptr)
	{
		env = env->enclosing;
	}
	env->assign(stmt.name,Obj(std::make_shared<LoxClass>(klass)));
}

void Interpreter::visitPrintStmt(StmtPrint &stmt)
{
	auto value = evaluate(stmt.expression);
	std::cout<<stringify(value)<<std::endl;
}

void Interpreter::visitVarStmt(StmtVar &stmt)
{
	Obj value;

	if (stmt.initializer != nullptr)
	{
		value = evaluate(stmt.initializer);
	}

	env->define(stmt.name,value);
}

void Interpreter::visitBlockStmt(StmtBlock &stmt)
{

	executeBlock(stmt.statements, std::make_shared<Env>(env));
}

void Interpreter::visitifStmt(Stmtif &stmt)
{
	auto condition = evaluate(stmt.condition);
	if (isTruthy(condition))
	{
		execute(stmt.thenBranch);
	}
	else if (stmt.elseBranch != nullptr)
	{
		execute(stmt.elseBranch);
	}
}

void Interpreter::visitwhileStmt(Stmtwhile &stmt)
{

	while (isTruthy(evaluate(stmt.condition)))
	{
		execute(stmt.body);
	}
}

void Interpreter::visitFunctionStmt(StmtFunction &stmt)
{
	std::shared_ptr<LoxFunction> function = std::make_shared<LoxFunction>(
		std::make_shared<StmtFunction>(stmt), env,false);
	env->define(stmt.name, Obj(function));
}

void Interpreter::visitReturnStmt(StmtReturn &stmt)
{
	Obj value;
	if (stmt.value != nullptr) {
		value = evaluate(stmt.value);
	}
	throw ReturnValue(value);
}

Obj Interpreter::visitGetExpr(ExprGet &expr)
{
	Obj obj = evaluate(expr.object);
	if (obj.isTypeOf(Type::Class))
	{
		return obj.getInstance()->get(expr.name);
	}
	throw RuntimeError(expr.name, "Only instances have properties.");
}

Obj Interpreter::visitSetExpr(ExprSet &expr)
{
	Obj obj = evaluate(expr.object);
	if (!obj.isTypeOf(Type::Class))
	{
		throw RuntimeError(expr.name, "Only instances have fields.");
	}
	auto val = evaluate(expr.value);
	obj.getInstance()->set(expr.name, val);
	return val;
}

Obj Interpreter::visitVariableExpr(ExprVariable &expr)
{
	return lookUpVariable(expr.name, exprStack.top());
}

Obj Interpreter::visitThisExpr(ExprThis &expr)
{
	return lookUpVariable(expr.keyword, exprStack.top());
}

Obj Interpreter::visitAssignExpr(ExprAssign &expr)
{
	Obj value = evaluate(expr.value);
	auto exprPtr = exprStack.top();

	auto it = locals.find(exprPtr);
	if (it != locals.end())
	{
		env->assignAt(it->second, expr.name, value);
	}
	else
	{
		globals->assign(expr.name, value);
	}

	return value;
}

Obj Interpreter::visitSuperExpr(ExprSuper &expr)
{
	int distance = locals[exprStack.top()];
	Obj superclass = env->getAt(distance, "super");
	Obj object = env->getAt(distance - 1, "this");


	auto superclassPtr = std::dynamic_pointer_cast<LoxClass>(superclass.getCallable());
	if (superclassPtr == nullptr)
	{
		throw RuntimeError(expr.method, "Superclass is not a valid class.");
	}
	auto method = superclassPtr->findMethod(expr.method.getLexme());
	if (method == nullptr)
	{
		throw RuntimeError(expr.method, "Undefined property '" + expr.method.getLexme() + "'.");
	}

	return Obj(method->bind(object.getInstance()));
}


Obj Interpreter::visitLogicalExpr(ExprLogical &expr)
{
	Obj left = evaluate(expr.left);

	if (expr.op.getType() == TokenType::OR)
	{
		if (isTruthy(left)) return left;
	}
	else
	{
		if (!isTruthy(left)) return left;
	}
	return evaluate(expr.right);
}

Obj Interpreter::visitCallExpr(ExprCall &expr)
{
	Obj callee = evaluate(expr.calle);
	vector<Obj> arguments;
	for (auto argument: expr.arguments)
	{
		arguments.push_back(evaluate(argument));
	}

	if (!callee.isTypeOf(Type::Fn))
	{
		throw RuntimeError(expr.paren, "Can only call functions and classes.");
	}

	std::shared_ptr<LoxCallable> function = callee.getCallable();


	if (arguments.size() != function->arity())
	{
		throw RuntimeError(expr.paren, "Expected " +
						  std::to_string(function->arity()) +
						  " arguments but got " +
						  std::to_string(arguments.size()) + ".");
	}

	return function->call(*this, arguments);
}

Obj Interpreter::visitBinaryExpr(ExprBinary &expr)
{
	Obj left = evaluate(expr.left);
	Obj right = evaluate(expr.right);

	switch (expr.op.getType())
	{
		case TokenType::PLUS:
		{
			if (left.isTypeOf(Type::Num) && right.isTypeOf(Type::Num))
			{
				return Obj(left.getNumber()+right.getNumber());
			}
			else if (left.isTypeOf(Type::Str) && right.isTypeOf(Type::Str))
			{
				return Obj(left.getString()+ right.getString());
			}
			throw RuntimeError(expr.op,"Operns must be two numbers or two strings");
		}
		break;
		case TokenType::GREATER:
		{
			checkNumberOperands(expr.op,left,right);
			return Obj(left.getNumber()>right.getNumber());

		}
		case TokenType::GREATER_EQUAL:
		{
			checkNumberOperands(expr.op,left,right);
			return Obj(left.getNumber()>=right.getNumber());
		}
		case TokenType::LESS:
		{
			checkNumberOperands(expr.op,left,right);
			return Obj(left.getNumber()<right.getNumber());
		}
		case TokenType::LESS_EQUAL:
		{
			checkNumberOperands(expr.op,left,right);
			return Obj(left.getNumber()<=right.getNumber());

		}
		case TokenType::BANG_EQUAL:
		{
			return Obj(!isEqual(left,right));
		}
		case TokenType::EQUAL_EQUAL:
		{
			return Obj(isEqual(left,right));
		}
		case TokenType::MINUS:
		{
			checkNumberOperands(expr.op,left,right);
			return Obj(left.getNumber()-right.getNumber());

		}
		case TokenType::SLASH:
		{
			checkNumberOperands(expr.op,left,right);
			return Obj(left.getNumber()/right.getNumber());

		}
		case TokenType::STAR:
		{
			checkNumberOperands(expr.op,left,right);
			return Obj(left.getNumber()*right.getNumber());
		}
	}
	throw RuntimeError(expr.op,"Operans must be two numbers of two strings.");
}

Obj Interpreter::visitGroupingExpr(ExprGrouping &expr)
{
	return evaluate(expr.expression);
}

Obj Interpreter::visitLiteralExpr(ExprLiteral &expr)
{
	return expr.value;
}

Obj Interpreter::visitUnaryExpr(ExprUnary &expr)
{
	Obj right = evaluate(expr.right);

	switch (expr.op.getType())
	{
		case TokenType::MINUS:
		{
			checkNumberOperand(expr.op,right);
			return Obj(-right.getNumber());
		}
		case TokenType::BANG:
		{
			return Obj(!isTruthy(right));
		}

	}
	throw std::runtime_error("ddd");
}

Obj Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
	exprStack.push(expr);
	Obj result = expr->accept(*this);
	exprStack.pop();
	return result;
}

bool Interpreter::isTruthy(const Obj &object)
{
	if (object.isTypeOf(Null)) return false;
	if (object.isTypeOf(Bool)) return object.getBoolean();
	return true;
}

bool Interpreter::isEqual(const Obj &a, const Obj &b)
{
	if (a.isTypeOf(Type::Num) && b.isTypeOf(Type::Num))
	{
		return a.getNumber() == b.getNumber();
	}
	else if (a.isTypeOf(Type::Str) && b.isTypeOf(Type::Str))
	{
		return a.getString()== b.getString();
	}
	else if (a.isTypeOf(Type::Bool) && b.isTypeOf(Type::Bool))
	{
		return a.getBoolean() == b.getBoolean();
	}
	else if (a.isTypeOf(Type::Null) && b.isTypeOf(Type::Null))
	{
		return true;
	}
	return false;
}

void Interpreter::resolve(std::shared_ptr<Expr> expr, int depth)
{
	locals.insert({expr,depth});
}

Obj Interpreter::lookUpVariable(const Token &name, std::shared_ptr<Expr> expr)
{
	auto it = locals.find(expr);
	if (it != locals.end())
	{
		return env->getAt(it->second, name.getLexme());
	}
	return globals->get(name);
}
void Interpreter::executeBlock(std::vector<shared_ptr<Stmt>> statments, std::shared_ptr<Env> environment)
{
	std::shared_ptr<Env> previous = this->env;
	try
	{
		this->env = environment;

		for (auto stmt : statments)
		{
			execute(stmt);
		}
	}
	catch (...)
	{

		this->env = previous;
		throw;
	}
	this->env = previous;
}




void Interpreter::checkNumberOperand(const Token& token, const Obj &operand)
{
	if (operand.isTypeOf(Num)) return;;
	throw RuntimeError(token,"Operand must be number type");
}

void Interpreter::checkNumberOperands(const Token &token, const Obj &left, const Obj &right)
{
	if (left.isTypeOf(Num) &&right.isTypeOf(Num)) return;
	throw RuntimeError(token,"Operands must be numbers.");
}

std::string Interpreter::stringify(const Obj &object)
{
	switch (object.getType())
	{
		case Type::Num:
		{
			return std::to_string(object.getNumber());
		}break;
		case Type::Str:
		{
			return object.getString();
		}break;
		case Type::Null:
		{
			return "nil";
		}break;
		case Type::Bool:
		{
			return object.getBoolean() ? "true": "false";
		}break;
		case Type::Fn:
		{
			return object.getCallable()->toString();
		}
		case Type::Class:
		{
			return object.getInstance()->getKlass().toString();
		}
		break;
	}
}


