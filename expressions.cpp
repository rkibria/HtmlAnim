#include "htmlanim.hpp"

int main() {
	HtmlAnim::HtmlAnim anim("expressions1", 600, 600);

	anim.frame().arc(anim.frame().linear_range(10, 100, 60), 20, 10);
	anim.frame().arc(anim.frame().linear_range(100, 10, 120), 40, 10);
	anim.frame().arc(anim.frame().linear_range(10, 100, 180), 60, 10);

	anim.frame().add_drawable(
		std::make_unique<HtmlAnim::Arc>(200, 200,
			anim.frame().add_coord_expression(std::make_unique<HtmlAnim::LinearRangeExpression>(5, 50, 45)),
			0.0, 2 * M_PI,
			false
			)
	);

	// anim.frame().arc(200, 200, 50);

	anim.next_frame();
	anim.frame().arc(300, 200, 50);

	anim.write_file("expressions.html");
}
