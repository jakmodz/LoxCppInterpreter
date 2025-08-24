//
// Created by 123ja on 8/19/2025.
//

#include "AstPrinter.h"
#
std::string AstPrinter::print(std::shared_ptr<Expr> expr)
{
	return expr->accept(*this);
}
std::string AstPrinter::visitBinaryExpr(ExprBinary& expr)
{
	return parenthesize(expr.op.getLexme(), {expr.left, expr.right});
}

std::string AstPrinter::visitGroupingExpr(ExprGrouping& expr)
{
	return parenthesize("group", {expr.expression});
}

std::string AstPrinter::visitLiteralExpr(ExprLiteral& expr)
{
	// if (expr.value == null) return "nil";
	// return expr.value.toString();
	return "literal_value";
}

std::string AstPrinter::visitUnaryExpr(ExprUnary& expr)
{
	return parenthesize(expr.op.getLexme(), {expr.right});
}

std::string AstPrinter::parenthesize(const std::string& name, std::vector<std::shared_ptr<Expr>> exprs)
{
	std::string str;
	str.append("(");
	str.append(name);
	for (std::shared_ptr<Expr> expr : exprs)
	{
		str.append(" ");
		str.append(expr->accept(*this));
	}
	str.append(")");
	return str;
}