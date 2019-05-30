#include <htmlanim.hpp>
//#include <htmlanim_shapes.hpp>

int main() {
	const auto width = 800;
	const auto height = 450;
	HtmlAnim::HtmlAnim anim("HtmlAnim", width, height);

	anim.frame().text(20, 20, "test");

	anim.write_file("test.html");
}
