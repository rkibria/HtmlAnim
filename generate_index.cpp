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
<li><a href="example2.html">Example 2</a></li>
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

void make_example_2() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 2", 800, 400);

	anim.set_pre_text(R"(
<h2>Example 2</h2>
)");

	anim.set_post_text(R"(
<p>The animation above can be generated using this code:</p>
<p><pre>
#include "htmlanim.hpp"
int main() {
	anim.write_file("animation.html");
}
</pre></p>
<p>
More examples:
<ul>
<li><a href="index.html">Example 1</a></li>
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
		anim.frame().line(0, 200, 400, frame * 400.0 / n_frames);

		if(frame != n_frames - 1)
			anim.next_frame();
	}
	anim.write_file("example2.html");
}

int main() {
	make_index();
	make_example_2();
}
