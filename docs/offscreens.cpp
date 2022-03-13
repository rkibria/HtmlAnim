#include <htmlanim.hpp>
#include <htmlanim_shapes.hpp>

int main() {
	HtmlAnim::HtmlAnim anim("Offscreens", 600, 600);

	anim.write_file("offscreens.html");
}
