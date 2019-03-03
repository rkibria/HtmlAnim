#include <iostream>
#include "htmlanim.hpp"

int main() {
	HtmlAnim::HtmlAnim anim("Example", 600, 400, 20);

	anim.text(30, 25, "Frame ${frame_counter + 1}/${num_frames}");
	anim.rectangle(10, 5, 30, 40);
	anim.rectangle(30, 25, 80, 50, true);
	anim.text(30, 99, "This is text!");
	anim.next_frame();

	anim.text(30, 25, "Frame ${frame_counter + 1}/${num_frames}");
	anim.rectangle(50, 50, 90, 60);
	anim.rectangle(10, 40, 30, 70, true);

	anim.write_stream(std::cout);
	anim.write_file("out.html");
}
