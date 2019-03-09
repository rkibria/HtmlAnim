#include <htmlanim.hpp>
#include <htmlanim_shapes.hpp>

#include <iostream>
#include <array>
#include <algorithm>
#include <string>
#include <cmath>

static const constexpr auto footer = R"(<hr>
<p>
<small><a href="index.html">Back to index</a></small><br>
<small>Github project: <a href="https://github.com/rkibria/HtmlAnim">https://github.com/rkibria/HtmlAnim</a></small>
</p>
)";

void make_index() {
	HtmlAnim::HtmlAnim anim("HtmlAnim", 120, 120);

	anim.pre_text() << R"(
<h1>HtmlAnim</h1>
<h2>A header-only C++ HTML animation library</h2>
)";

	anim.post_text() << R"(
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
		anim.next_frame();
	}
	anim.remove_last_frame();
	anim.write_file("animation.html");
}
</pre></p>
<p>
More examples:
<ul>
<li><a href="example2.html">Example 2 - Lines: drawing a binary tree</a></li>
<li><a href="example3.html">Example 3 - Circles: bubble sort visualization</a></li>
<li><a href="example4.html">Example 4 - Line colors and styles</a></li>
<li><a href="example5.html">Example 5 - Macros, context saving, translation, rotation and scaling</a></li>
<li><a href="example6.html">Example 6 - Line paths: polygons</a></li>
<li><a href="example7.html">Example 7 - Extensions</a></li>
</ul>
</p>
)";
	anim.post_text() << footer;

	const auto n_frames = 60;
	anim.foreground().text(10, 10, "Frame ${frame_counter + 1}/${num_frames}");
	for(double frame = 0; frame < n_frames; ++frame) {
		anim.frame().rect(10 + frame, 15, 40, 40);
		anim.frame().rect(10 + n_frames, 15 + frame, 40, 40, true);
		anim.frame().rect(10 + n_frames - frame, 15 + n_frames, 40, 40);
		anim.frame().rect(10, 15 + n_frames - frame, 40, 40, true);
		anim.next_frame();
	}
	anim.remove_last_frame();
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
	HtmlAnim::HtmlAnim anim("HtmlAnim example 2 - Lines: drawing a binary tree", 400, 600);
	anim.set_no_clear(true);

	anim.pre_text() << "<h2>Example 2</h2>";

	anim.post_text() << R"(
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
)";
	anim.post_text() << footer;

	draw_binary_tree(anim, 0, 300, 150);
	anim.write_file("example2.html");
}

void make_example_3() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 3 - Circles: bubble sort visualization", 800, 180);
	anim.set_wait_frames(20);

	anim.pre_text() << "<h2>Example 3</h2>";

	anim.post_text() << R"(
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
)";
	anim.post_text() << footer;

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
	HtmlAnim::HtmlAnim anim("HtmlAnim example 4 - Line colors and styles", 600, 300);

	anim.pre_text() << "<h2>Example 4</h2>";

	anim.post_text() << R"(
<p>The animation above can be generated using this code:</p>
<p><pre>
#include "htmlanim.hpp"
int main() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 4 - Line colors and styles", 600, 300);
	const auto n_parts = 16;
	const auto part_len = (anim.get_width() - 60) / n_parts;
	auto get_y = [&anim, n_parts](auto part, auto y_scale)
		{return anim.get_height() / 2 * (1 + 0.75 * y_scale * sin(2 * M_PI / n_parts * part));};
	auto ramp = [](auto n_parts, auto i) {return (i < n_parts / 2) ? i : (n_parts - i);};
	const auto n_frames = 60;
	for(auto frame = 0; frame < n_frames; ++frame) {
		const auto y_scale = sin(2 * M_PI / n_frames * frame);
		for(auto part = 0; part < n_parts; ++part) {
			anim.frame().line_cap("round");
			anim.frame().line_width(5 + 5 * ramp(n_parts, (part + frame / 4) % n_parts));
			const auto color = 255 / n_parts * (part + frame / 4);
			anim.frame().stroke_style(HtmlAnim::rgb_color(color, 255 - color, 128 + color));
			const auto start_x = 30 + part_len * part;
			anim.frame().line(start_x, get_y(part, y_scale), start_x + part_len, get_y(part + 1, y_scale));
		}
		anim.next_frame();
	}
	anim.remove_last_frame();
	anim.write_file("colors_lines.html");
}
</pre></p>
)";
	anim.post_text() << footer;

	const auto n_parts = 16;
	const auto part_len = (anim.get_width() - 60) / n_parts;
	auto get_y = [&anim, n_parts](auto part, auto y_scale)
		{return anim.get_height() / 2 * (1 + 0.75 * y_scale * sin(2 * M_PI / n_parts * part));};
	auto ramp = [](auto n_parts, auto i) {return (i < n_parts / 2) ? i : (n_parts - i);};
	const auto n_frames = 60;
	for(auto frame = 0; frame < n_frames; ++frame) {
		const auto y_scale = sin(2 * M_PI / n_frames * frame);
		for(auto part = 0; part < n_parts; ++part) {
			anim.frame().line_cap("round");
			anim.frame().line_width(5 + 5 * ramp(n_parts, (part + frame / 4) % n_parts));
			const auto color = 255 / n_parts * (part + frame / 4);
			anim.frame().stroke_style(HtmlAnim::rgb_color(color, 255 - color, 128 + color));
			const auto start_x = 30 + part_len * part;
			anim.frame().line(start_x, get_y(part, y_scale), start_x + part_len, get_y(part + 1, y_scale));
		}
		anim.next_frame();
	}
	anim.remove_last_frame();
	anim.write_file("example4.html");
}

void make_example_5() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 5 - Macros, context saving, translation, rotation and scaling", 400, 300);

	anim.pre_text() << "<h2>Example 5</h2>";

	anim.post_text() << R"(
<p>The animation above can be generated using this code:</p>
<p><pre>
#include "htmlanim.hpp"
int main() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 5 - Macros, context saving, translation, rotation and scaling", 400, 300);
	const auto n_frames = 60;
	const constexpr char* smiley = "smiley";
	anim.frame().define_macro(smiley)
		.line_width(3)
		.arc(0, 0, 80)
		.fill_style("yellow")
		.arc(0, 0, 80, true)
		.fill_style("black")
		.arc(-30, -30, 10, true)
		.arc(30, -30, 10, true)
		.arc(0, 0, 50, false, 0, M_PI);
	for(auto frame = 0; frame < n_frames; ++frame) {
		const auto rot = 2 * M_PI / n_frames * frame;
		anim.frame()
			.save()
			.translate(anim.get_width() / 2, anim.get_height() / 2)
			.rotate(rot)
			.scale(1 + 0.5 * sin(rot), 1 + 0.5 * cos(rot))
			.draw_macro(smiley);
		anim.next_frame();
	}
	anim.remove_last_frame();
	anim.write_file("trans_rot_scale.html");
}
</pre></p>
)";
	anim.post_text() << footer;

	const auto n_frames = 60;
	const constexpr char* smiley = "smiley";
	anim.frame().define_macro(smiley)
		.line_width(3)
		.arc(0, 0, 80)
		.fill_style("yellow")
		.arc(0, 0, 80, true)
		.fill_style("black")
		.arc(-30, -30, 10, true)
		.arc(30, -30, 10, true)
		.arc(0, 0, 50, false, 0, M_PI);
	for(auto frame = 0; frame < n_frames; ++frame) {
		const auto rot = 2 * M_PI / n_frames * frame;
		anim.frame()
			.save()
			.translate(anim.get_width() / 2, anim.get_height() / 2)
			.rotate(rot)
			.scale(1 + 0.5 * sin(rot), 1 + 0.5 * cos(rot))
			.draw_macro(smiley);
		anim.next_frame();
	}
	anim.remove_last_frame();
	anim.write_file("example5.html");
}

void make_example_6() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 6 - Line paths: polygons", 600, 250);
	anim.set_wait_frames(20);

	anim.pre_text() << "<h2>Example 6</h2>";

	anim.post_text() << R"(
<p>The animation above can be generated using this code:</p>
<p><pre>
#include "htmlanim.hpp"
int main() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 6 - Line paths: polygons", 600, 250);
	auto poly_points = [](auto n_points, auto radius) {
		HtmlAnim::PointVector pv(n_points);
		for(size_t p_i = 0; p_i < n_points; ++p_i) {
			const auto phi = 2 * M_PI / n_points * p_i;
			pv[p_i].x = radius * cos(phi);
			pv[p_i].y = radius * sin(phi);
		}
		return pv;
	};

	for(size_t n_points = 2; n_points < 9; ++n_points) {
		const auto points = poly_points(n_points, 100);
		anim.frame()
			.save()
			.translate(110, 110)
			.line(points, false, true);
		anim.frame()
			.save()
			.translate(320, 110)
			.line(points, true);
		anim.next_frame();
	}
	anim.remove_last_frame();
	anim.write_file("polygons.html");
}
</pre></p>
<p>See <a href="example7.html">Example 7</a> for how to use an extension to draw regular polygons easily.</p>
)";
	anim.post_text() << footer;

	auto poly_points = [](auto n_points, auto radius) {
		HtmlAnim::PointVector pv(n_points);
		for(size_t p_i = 0; p_i < n_points; ++p_i) {
			const auto phi = 2 * M_PI / n_points * p_i;
			pv[p_i].x = radius * cos(phi);
			pv[p_i].y = radius * sin(phi);
		}
		return pv;
	};

	for(size_t n_points = 2; n_points < 9; ++n_points) {
		const auto points = poly_points(n_points, 100);
		anim.frame()
			.save()
			.translate(110, 110)
			.line(points, false, true);
		anim.frame()
			.save()
			.translate(320, 110)
			.line(points, true);
		anim.next_frame();
	}
	anim.remove_last_frame();
	anim.write_file("example6.html");
}

void make_example_7() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 7 - Extensions", 600, 250);
	anim.set_wait_frames(20);

	anim.pre_text() << "<h2>Example 7</h2>";

	anim.post_text() << R"(
<p>The animation above can be generated using this code:</p>
<p><pre>
// Use the regular polygon drawable from the shapes extension
#include "htmlanim_shapes.hpp"
int main() {
	HtmlAnim::HtmlAnim anim("HtmlAnim example 7 - Extensions", 600, 250);
	anim.set_wait_frames(20);
	for(size_t n_points = 2; n_points < 9; ++n_points) {
		anim.frame()
			.save()
			.translate(110, 110)
			.add_drawable(HtmlAnimShapes::regular_polygon(0, 0, 100, n_points));
		anim.frame()
			.save()
			.translate(320, 110)
			.add_drawable(HtmlAnimShapes::regular_polygon(0, 0, 100, n_points, true));
		anim.next_frame();
	}
	anim.remove_last_frame();
	anim.write_file("extensions.html");
}
</pre></p>
)";
	anim.post_text() << footer;

	for(size_t n_points = 2; n_points < 9; ++n_points) {
		anim.frame()
			.save()
			.translate(110, 110)
			.add_drawable(HtmlAnimShapes::regular_polygon(0, 0, 100, n_points));
		anim.frame()
			.save()
			.translate(320, 110)
			.add_drawable(HtmlAnimShapes::regular_polygon(0, 0, 100, n_points, true));
		anim.next_frame();
	}
	anim.remove_last_frame();
	anim.write_file("example7.html");
}

void make_demo_coordinates() {
	HtmlAnim::HtmlAnim anim("HtmlAnim - Demo: coordinate system", 600, 400);
	anim.set_wait_frames(20);

	anim.pre_text() << "<h2>Demo: coordinate system</h2>";

	anim.post_text() << R"(
<p>The animation above can be generated using this code:</p>
<p><pre>
#include "htmlanim_shapes.hpp"
int main() {
}
</pre></p>
)";
	anim.post_text() << footer;

	anim.frame().add_drawable(HtmlAnimShapes::grid(55, 30, 20, 15, 3, 2));
	anim.frame().add_drawable(HtmlAnimShapes::subdivided_grid(10, 15, 50, 30, 4, 3, 2, 3));
	anim.write_file("demo_coordinates.html");
}

int main() {
	make_index();
	make_example_2();
	make_example_3();
	make_example_4();
	make_example_5();
	make_example_6();
	make_example_7();

	make_demo_coordinates();
}
