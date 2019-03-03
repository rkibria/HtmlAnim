#include "htmlanim.hpp"

int main() {
	HtmlAnim::HtmlAnim anim("HtmlAnim", 400, 300, 20);

	anim.set_pre_text(R"(
<h1>HtmlAnim - HTML animation library</h1>
<p>
Github project: <a href="https://github.com/rkibria/HtmlAnim">https://github.com/rkibria/HtmlAnim</a>
</p>
)");

	anim.set_post_text(R"(
<hr>
<small>Generated with HtmlAnim</small>
)");

	anim.text(30, 25, "Frame ${frame_counter + 1}/${num_frames}");
	anim.rectangle(10, 5, 30, 40);
	anim.rectangle(30, 25, 80, 50, true);
	anim.text(30, 99, "This is text!");
	anim.next_frame();

	anim.text(30, 25, "Frame ${frame_counter + 1}/${num_frames}");
	anim.rectangle(50, 50, 90, 60);
	anim.rectangle(10, 40, 30, 70, true);

	anim.write_file("index.html");
}
