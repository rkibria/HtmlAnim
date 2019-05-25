#include "htmlanim.hpp"
using namespace HtmlAnim;


class CoordExpressionValue {
	std::string payload;
public:
	CoordExpressionValue(const std::string& v) : payload{v} {}
	CoordExpressionValue(const CoordType& v) : payload{std::to_string(v)} {}
	std::string value() const {return payload;}
};


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


void draw(CoordExpressionValue x) {
	std::cout << "drawing coord " << x.value() << std::endl;
}


int main() {
	draw(1.3);
	auto inc = IncrementingCoordExpression(100.123);
	draw(inc);
	draw(inc);
	draw(inc);

	// HtmlAnim::HtmlAnim anim("expressions1", 400, 600);
	// anim.set_wait_frames(20);

	// for(int i=1; i < 10; ++i) {
		// anim.frame().arc(200, 200, i * 5);
		// anim.next_frame();
	// }
	// anim.remove_last_frame();

	// anim.write_file("expressions.html");
}
