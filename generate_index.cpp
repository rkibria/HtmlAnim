#include "htmlanim.hpp"

void make_index() {
	HtmlAnim::HtmlAnim anim("HtmlAnim", 120, 120);

	anim.set_pre_text(R"(
<h1>HtmlAnim</h1>
<h2>A header-only C++ HTML animation library</h2>
)");

	anim.set_post_text(R"(
<p>The animation above can be generated using this code:</p>
<p><pre>
#include "htmlanim.hpp"
int main() {
	HtmlAnim::HtmlAnim anim("Wandering rectangles", 120, 120);
	const auto n_frames = 60;
	anim.foreground().text(10, 10, "Frame ${frame_counter + 1}/${num_frames}");
	for(double frame = 0; frame < n_frames; ++frame) {
		anim.frame().rect(10 + frame, 15, 40, 40);
		anim.frame().rect(10 + n_frames, 15 + frame, 40, 40, true);
		anim.frame().rect(10 + n_frames - frame, 15 + n_frames, 40, 40);
		anim.frame().rect(10, 15 + n_frames - frame, 40, 40, true);
		if(frame != n_frames - 1)
			anim.next_frame();
	}
	anim.write_file("animation.html");
}
</pre></p>
<p>
More examples:
<ul>
<li><a href="example2.html">Example 2 - Draw a binary tree</a></li>
<li><a href="example3.html">Example 3 - Bubble sort visualization</a></li>
</ul>
</p>
<hr>
<p>
<small>Github project: <a href="https://github.com/rkibria/HtmlAnim">https://github.com/rkibria/HtmlAnim</a></small>
</p>
)");

	const auto n_frames = 60;
	anim.foreground().text(10, 10, "Frame ${frame_counter + 1}/${num_frames}");
	for(double frame = 0; frame < n_frames; ++frame) {
		anim.frame().rect(10 + frame, 15, 40, 40);
		anim.frame().rect(10 + n_frames, 15 + frame, 40, 40, true);
		anim.frame().rect(10 + n_frames - frame, 15 + n_frames, 40, 40);
		anim.frame().rect(10, 15 + n_frames - frame, 40, 40, true);
		if(frame != n_frames - 1)
			anim.next_frame();
	}
	anim.write_file("index.html");
}

void draw_binary_tree(HtmlAnim::HtmlAnim& anim, double x, double y, double delta_y, int depth = 0) {
	if(depth > 7)
		return;
	const auto next_x = x + 20;
	const auto next_y1 = y - delta_y;
	const auto next_y2 = y + delta_y;
	anim.frame().line(x, y, next_x, next_y1);
	anim.next_frame();
	draw_binary_tree(anim, next_x, next_y1, delta_y / 2, depth + 1);
	anim.frame().line(x, y, next_x, next_y2);
	anim.next_frame();
	draw_binary_tree(anim, next_x, next_y2, delta_y / 2, depth + 1);
}

void make_example_2() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 2 - Draw a binary tree", 400, 600);
	anim.set_no_clear(true);

	anim.set_pre_text(R"(
<h2>Example 2</h2>
)");

	anim.set_post_text(R"(
<p>The animation above can be generated using this code:</p>
<p><pre>
#include "htmlanim.hpp"
void draw_binary_tree(HtmlAnim::HtmlAnim& anim, double x, double y, double delta_y, int depth = 0) {
	if(depth > 7)
		return;
	const auto next_x = x + 20;
	const auto next_y1 = y - delta_y;
	const auto next_y2 = y + delta_y;
	anim.frame().line(x, y, next_x, next_y1);
	anim.next_frame();
	draw_binary_tree(anim, next_x, next_y1, delta_y / 2, depth + 1);
	anim.frame().line(x, y, next_x, next_y2);
	anim.next_frame();
	draw_binary_tree(anim, next_x, next_y2, delta_y / 2, depth + 1);
}

int main() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 2 - Draw a binary tree", 400, 600);
	anim.set_no_clear(true);
	draw_binary_tree(anim, 0, 300, 150);
	anim.write_file("binarytree.html");
}
</pre></p>
<hr>
<p>
<small>Github project: <a href="https://github.com/rkibria/HtmlAnim">https://github.com/rkibria/HtmlAnim</a></small>
</p>
)");

	draw_binary_tree(anim, 0, 300, 150);
	anim.write_file("example2.html");
}

void make_example_3() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 3 - Bubble sort visualization", 600, 120);

	anim.set_pre_text(R"(
<h2>Example 3</h2>
)");

	anim.set_post_text(R"(
<p>The animation above can be generated using this code:</p>
<p><pre>
#include "htmlanim.hpp"
int main() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 3 - Bubble sort visualization", 400, 600);
	anim.write_file("binarytree.html");
}
</pre></p>
<hr>
<p>
<small>Github project: <a href="https://github.com/rkibria/HtmlAnim">https://github.com/rkibria/HtmlAnim</a></small>
</p>
)");

	anim.frame().arc(100, 70, 30);
	anim.write_file("example3.html");
}

int main() {
	make_index();
	make_example_2();
	make_example_3();
}
