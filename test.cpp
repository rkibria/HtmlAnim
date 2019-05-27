#include <iostream>
#include <memory>
#include <vector>
#include <string>

using CoordType = double;

class ExpressionValue {
protected:
	std::string v;
public:
	virtual ~ExpressionValue() = default;
	ExpressionValue(const std::string& v) : v{ v } {}
	virtual std::string to_string() const { return v; }
};

class CoordExpressionValue : public ExpressionValue {
public:
	CoordExpressionValue(const CoordType& v) : ExpressionValue{ std::to_string(v) } {}
	virtual std::string coord_val() const { return "coordval " + v; };
};

class BoolExpressionValue : public ExpressionValue {
public:
	BoolExpressionValue(const bool& b) : ExpressionValue{ (b ? "true" : "false") } {}
	virtual std::string bool_val() const { return "bool " + v; };
};

class Expression {
public:
	virtual ~Expression() {}
	virtual const ExpressionValue& value() const = 0;
};

using ExpressionVector = std::vector<std::unique_ptr<Expression>>;

ExpressionVector expr_vec;

const ExpressionValue& add_expression(std::unique_ptr<Expression>&& expr) {
	expr_vec.emplace_back(std::move(expr));
	return expr_vec.back()->value();
}

class CoordExpression : public Expression {
	CoordExpressionValue val;
public:
	CoordExpression() : val(123.123) {}
	virtual const ExpressionValue& value() const override { return val; }
};

class BoolExpression : public Expression {
	BoolExpressionValue val;
public:
	BoolExpression() : val(true) {}
	virtual const ExpressionValue& value() const override { return val; }
};

void draw_coord(const ExpressionValue& coord) {
	std::cout << "draw_coord " << dynamic_cast<const CoordExpressionValue&>(coord).coord_val() << std::endl;
}

int main() {
	draw_coord(add_expression(std::make_unique<CoordExpression>()));
	draw_coord(add_expression(std::make_unique<BoolExpression>()));
}
