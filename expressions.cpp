#include "htmlanim.hpp"

/*
class CoordExpression {
public:
	virtual std::string value() = 0;
};


class IncrementingCoordExpression : public CoordExpression {
private:
	CoordType current;
public:
	IncrementingCoordExpression(const CoordType& v) : current{v} {}
	virtual std::string value() {return std::to_string(current += 1);}
	operator CoordExpressionValue() {return CoordExpressionValue(value());}
};

void draw(ExpressionValue x) {
	std::cout << "drawing coord " << x.to_string() << std::endl;
}
*/

int main() {
	// draw(1.3);
	// auto inc = IncrementingCoordExpression(100.123);
	// draw(inc);
	// draw(inc);
	// draw(inc);

	HtmlAnim::HtmlAnim anim("expressions1", 600, 600);
	// anim.set_wait_frames(30);
	// anim.frame().arc(200, 200, anim.frame().range_expr(50, 100));
	// auto radius = CoordRangeExpression(50, 100, 5);
	// anim.frame().add_expression(std::make_unique<CoordRangeExpression>(50, 100, 5));
	//anim.frame().arc(200, 200, anim.frame().add_expression(std::make_unique<CoordRangeExpression>(5, 50, 1)));
	
	anim.frame().arc(200, 200, anim.frame().range_expr(5, 150, 1));

	anim.frame().arc(anim.frame().range_expr(10, 50, 1), 20, 10);

	//anim.frame().arc(20, anim.frame().range_expr(200, 30, -5), 10);

	const auto test_val1 = HtmlAnim::CoordExpressionValue(123);

	//anim.frame().add_drawable(
	//	std::make_unique<HtmlAnim::Arc>(200, 200,
	//		anim.frame().add_coord_expression(std::make_unique<HtmlAnim::CoordRangeExpression>(5, 50, 1)),
	//		0.0, 2 * M_PI,
	//		false
	//		)
	//);

	// anim.frame().arc(200, 200, 50);

	anim.next_frame();
	anim.frame().arc(300, 200, 50);

	anim.write_file("expressions.html");
}
