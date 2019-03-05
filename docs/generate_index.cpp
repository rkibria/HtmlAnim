#include <array>
#include <algorithm>
#include <string>
#include <cmath>
#include "../htmlanim.hpp"

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
	HtmlAnim::HtmlAnim anim("HtmlAnim example 3 - Bubble sort visualization", 800, 180);
	anim.set_wait_frames(20);

	anim.set_pre_text(R"(
<h2>Example 3</h2>
)");

	anim.set_post_text(R"(
<p>The animation above can be generated using this code:</p>
<p><pre>
#include "htmlanim.hpp"
#include &lt;array&gt;
#include &lt;algorithm&gt;
#include &lt;string&gt;
int main() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 3 - Bubble sort visualization", 400, 600);
	anim.set_wait_frames(20);

	std::array<int, 8> numbers{7, 6, 2, 8, 5, 1, 4, 3};
	int iteration = 1;

	auto draw = [&iteration, &anim, &numbers](size_t swap_pos=99) {
		anim.frame().text(10, 150, std::string("Iteration: ") + std::to_string(iteration));
		for(size_t i = 0; i < numbers.size(); ++i)
			anim.frame().arc(50 + i * 100, 50, 10 + numbers[i] * 5, i == swap_pos || i == swap_pos + 1);
		anim.next_frame();
	};

	bool swap = false;
	do {
		draw();
		swap = false;
		for(size_t i = 0; i < numbers.size() - 1; ++i) {
			if(numbers[i] > numbers[i + 1]) {
				draw(i);
				swap = true;
				std::swap(numbers[i], numbers[i + 1]);
				draw(i);
				draw();
			}
		}
		++iteration;
	} while(swap);
	anim.write_file("bubblesort.html");
}
</pre></p>
<hr>
<p>
<small>Github project: <a href="https://github.com/rkibria/HtmlAnim">https://github.com/rkibria/HtmlAnim</a></small>
</p>
)");

	std::array<int, 8> numbers{7, 6, 2, 8, 5, 1, 4, 3};
	int iteration = 1;

	auto draw = [&iteration, &anim, &numbers](size_t swap_pos=99) {
		anim.frame().text(10, 150, std::string("Iteration: ") + std::to_string(iteration));
		for(size_t i = 0; i < numbers.size(); ++i)
			anim.frame().arc(50 + i * 100, 50, 10 + numbers[i] * 5, i == swap_pos || i == swap_pos + 1);
		anim.next_frame();
	};

	bool swap = false;
	do {
		draw();
		swap = false;
		for(size_t i = 0; i < numbers.size() - 1; ++i) {
			if(numbers[i] > numbers[i + 1]) {
				draw(i);
				swap = true;
				std::swap(numbers[i], numbers[i + 1]);
				draw(i);
				draw();
			}
		}
		++iteration;
	} while(swap);
	anim.write_file("example3.html");
}

void make_example_4() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 4 - Stroke and fill styles", 600, 200);
	anim.set_wait_frames(5);

	anim.set_pre_text(R"(
<h2>Example 4</h2>
)");

	anim.set_post_text(R"(
<p>The animation above can be generated using this code:</p>
<p><pre>
}
</pre></p>
<hr>
<p>
<small>Github project: <a href="https://github.com/rkibria/HtmlAnim">https://github.com/rkibria/HtmlAnim</a></small>
</p>
)");

	const auto n_parts = 16;
	const auto part_len = 580 / n_parts;
	auto get_y = [n_parts](auto part, auto y_scale) {return 100 * (1 + y_scale * sin(2 * M_PI / n_parts * part));};
	const auto n_frames = 30;
	for(auto frame = 0; frame < n_frames; ++frame) {
		const auto y_scale = sin(2 * M_PI / n_frames * frame);
		for(auto part = 0; part < n_parts; ++part) {
			anim.frame().stroke_style("red");
			const auto start_x = 10 + part_len * part;
			anim.frame().line(start_x, get_y(part, y_scale), start_x + part_len, get_y(part + 1, y_scale));
		}
		if(frame != n_frames - 1)
			anim.next_frame();
	}
	anim.write_file("example4.html");
}

int main() {
	make_index();
	make_example_2();
	make_example_3();
	make_example_4();
}