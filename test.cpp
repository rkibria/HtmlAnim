#include <htmlanim.hpp>
//#include <htmlanim_shapes.hpp>

int main() {
	const auto width = 800;
	const auto height = 450;
	HtmlAnim::HtmlAnim anim("HtmlAnim", width, height);

	constexpr auto n_frames = 60;
	constexpr auto w = 40;
	constexpr auto h = 40;

	anim.frame().save().fill_style("red").rect(0, 0, anim.get_width(), anim.get_height(), true);
	anim.add_layer();

	anim.frame().rect(anim.frame().linear_range(10, 10 + n_frames, 60), 15, w, h);
	anim.frame().rect(10 + n_frames, anim.frame().linear_range(15, 15 + n_frames, 60), w, h, true);
	anim.frame().rect(anim.frame().linear_range(10 + n_frames, 10, 60), 15 + n_frames, w, h);
	anim.frame().rect(10, anim.frame().linear_range(15 + n_frames, 15, 60), w, h, true);
	//anim.next_frame();

	//anim.frame().text(20, 20, "1");
	//anim.next_frame();
	//anim.frame().text(20, 20, "2");
	//anim.next_frame();
	//anim.frame().text(20, 20, "3");

	anim.write_file("test.html");
}
