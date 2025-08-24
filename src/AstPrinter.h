//
// Created by 123ja on 8/19/2025.
//

#ifndef INTERPETER_CPP_ASTPRINTER_H
#define INTERPETER_CPP_ASTPRINTER_H
#include "Expr.h"
#include  <memory>
#include  <vector>

#include "Stmt.h"

class AstPrinter :  VisitorExpr<std::string>
{
public:
	std::string print(std::shared_ptr<Expr> expr);
	std::string visitBinaryExpr(ExprBinary& expr) override;
	std::string visitGroupingExpr(ExprGrouping& expr) override;
	std::string visitLiteralExpr(ExprLiteral& expr) override;
	std::string visitUnaryExpr(ExprUnary& expr) override;
private:
	std::string parenthesize(const std::string& name,std::vector<std::shared_ptr<Expr>> exprs);
};


#endif //INTERPETER_CPP_ASTPRINTER_H