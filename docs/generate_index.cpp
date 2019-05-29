#include <htmlanim.hpp>
#include <htmlanim_shapes.hpp>

#include <iostream>
#include <array>
#include <algorithm>
#include <string>
#include <cmath>
#include <random>

static const constexpr auto footer = R"(<hr>
<p>
<small><a href="index.html">Back to index</a></small><br>
<small>Github project: <a href="https://github.com/rkibria/HtmlAnim">https://github.com/rkibria/HtmlAnim</a></small>
</p>
)";

void make_index() {
	const auto width = 800;
	const auto height = 450;
	HtmlAnim::HtmlAnim anim("HtmlAnim", width, height);

	anim.pre_text() << R"(
<h1>HtmlAnim</h1>
<h2>A header-only C++ HTML animation library</h2>
)";

	anim.post_text() << R"(
<p>
<h3>Examples</h3>
<ul>
<li><a href="example1.html">Basics</a></li>
<li><a href="example2.html">Lines: drawing a binary tree</a></li>
<li><a href="example3.html">Circles: bubble sort visualization</a></li>
<li><a href="example4.html">Line colors and styles</a></li>
<li><a href="example5.html">Macros, context saving, translation, rotation and scaling</a></li>
<li><a href="example6.html">Line paths: polygons</a></li>
<li><a href="example7.html">Extensions</a></li>
<li><a href="example8.html">Rewind</a></li>
<li><a href="example9.html">Fonts: normal distribution</a></li>

<li><a href="demo_sierpinski.html">Demo - Sierpinski triangle</a></li>
</ul>
</p>
<p>
<h3>Other</h3>
<ul>
<li><a href="doxygen/html/index.html">Doygen-generated API documentation.</a></li>
</ul>
</p>
)";
	anim.post_text() << footer;

	anim.background()
		.font("bold 160px sans-serif")
		.save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);

	const std::vector<std::string> rainbow_cols = { "#9400D3", "#4B0082", "#0000FF", "#00FF00", "#FFFF00", "#FF7F00", "#FF0000" };

	const auto total_time = 5;
	const auto radius = 60;
	const auto right_edge = width - radius - 10;
	const auto lower_edge = height - radius - 10;

	anim.frame().text(
		10,
		anim.frame().linear_range(-100.0, lower_edge - radius - 10, 1 * HtmlAnim::FPS),
		"Html"
		);

	const auto n_cols = rainbow_cols.size();
	for (size_t i = 0; i < n_cols; ++i) {
		const auto inv_i = n_cols - 1 - i;
		const auto n_points = 9 - i;
		anim.frame()
			.wait(total_time * HtmlAnim::FPS)
			.save()
			.fill_style(rainbow_cols[i])
			.translate(anim.frame().linear_range(
					right_edge,
					right_edge - (inv_i) * radius * 1.85,
					1.5 * HtmlAnim::FPS),
				lower_edge)
			.rotate(anim.frame().linear_range(0, 2 * HtmlAnim::PI * (i + 1), total_time * HtmlAnim::FPS))
			.add_drawable(HtmlAnimShapes::regular_polygon(0, 0, radius, n_points, true));
	}

	anim.frame().text(
		370,
		anim.frame().linear_range(height + 100, lower_edge - radius, 1 * HtmlAnim::FPS),
		"Anim"
	);

	anim.write_file("index.html");
}

void make_example_1() {
	HtmlAnim::HtmlAnim anim("Expressions", 120, 120);
	anim.pre_text() << "<h2>Expressions</h2>";
	anim.post_text() << R"(
<p>The animation above can be generated using this code:</p>
<p><pre>
#include "htmlanim.hpp"
int main() {
	HtmlAnim::HtmlAnim anim("Wandering rectangles", 120, 120);
	constexpr auto n_frames = 60;
	constexpr auto w = 40;
	constexpr auto h = 40;
	anim.frame().rect(anim.frame().linear_range(10, 10 + n_frames, 60), 15, w, h);
	anim.frame().rect(10 + n_frames, anim.frame().linear_range(15, 15 + n_frames, 60), w, h, true);
	anim.frame().rect(anim.frame().linear_range(10 + n_frames, 10, 60), 15 + n_frames, w, h);
	anim.frame().rect(10, anim.frame().linear_range(15 + n_frames, 15, 60), w, h, true);
	anim.write_file("animation.html");
}
</pre></p>
)";
	anim.post_text() << footer;

	constexpr auto n_frames = 60;
	constexpr auto w = 40;
	constexpr auto h = 40;
	anim.background().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);
	anim.frame().rect(anim.frame().linear_range(10, 10 + n_frames, 60), 15, w, h);
	anim.frame().rect(10 + n_frames, anim.frame().linear_range(15, 15 + n_frames, 60), w, h, true);
	anim.frame().rect(anim.frame().linear_range(10 + n_frames, 10, 60), 15 + n_frames, w, h);
	anim.frame().rect(10, anim.frame().linear_range(15 + n_frames, 15, 60), w, h, true);

	anim.write_file("example1.html");
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
	HtmlAnim::HtmlAnim anim("Lines: drawing a binary tree", 400, 600);
	anim.set_no_clear(true);

	anim.post_text() << R"(
<p>See docs/generate_index.cpp for code.</p>
)";
	anim.post_text() << footer;

	anim.frame().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);
	anim.next_frame();
	draw_binary_tree(anim, 0, 300, 150);
	anim.write_file("example2.html");
}

void make_example_3() {
	HtmlAnim::HtmlAnim anim("Circles: bubble sort visualization", 840, 180);

	anim.post_text() << R"(
<p>See docs/generate_index.cpp for code.</p>
)";
	anim.post_text() << footer;
	anim.background().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);

	std::array<int, 8> numbers{7, 6, 2, 8, 5, 1, 4, 3};
	int iteration = 1;

	auto draw = [&iteration, &anim, &numbers](size_t swap_pos=99) {
		anim.frame().text(10, 150, std::string("Iteration: ") + std::to_string(iteration));
		for(size_t i = 0; i < numbers.size(); ++i)
			anim.frame().arc(50 + i * 100, 70, 10 + numbers[i] * 5, i == swap_pos || i == swap_pos + 1);
		anim.frame().wait(20);
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
	HtmlAnim::HtmlAnim anim("Line colors and styles", 600, 300);

	anim.post_text() << R"(
<p>See docs/generate_index.cpp for code.</p>
)";
	anim.post_text() << footer;
	anim.background().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);

	const auto n_parts = 16;
	const auto part_len = (anim.get_width() - 60) / n_parts;
	auto get_y = [&anim, n_parts](auto part, auto y_scale)
		{return anim.get_height() / 2 * (1 + 0.75 * y_scale * sin(2 * HtmlAnim::PI / n_parts * part));};
	auto ramp = [](auto n_parts, auto i) {return (i < n_parts / 2) ? i : (n_parts - i);};
	const auto n_frames = 60;
	for(auto frame = 0; frame < n_frames; ++frame) {
		const auto y_scale = sin(2 * HtmlAnim::PI / n_frames * frame);
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
	HtmlAnim::HtmlAnim anim("Macros, context saving, translation, rotation and scaling", 400, 300);

	anim.post_text() << R"(
<p>See docs/generate_index.cpp for code.</p>
)";
	anim.post_text() << footer;
	anim.background().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);

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
		.arc(0, 0, 50, false, 0, HtmlAnim::PI);
	for(auto frame = 0; frame < n_frames; ++frame) {
		const auto rot = 2 * HtmlAnim::PI / n_frames * frame;
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
	HtmlAnim::HtmlAnim anim("Line paths: polygons", 600, 250);

	anim.post_text() << R"(
<p>See docs/generate_index.cpp for code.</p>
)";
	anim.post_text() << footer;
	anim.background().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);

	auto poly_points = [](auto n_points, auto radius) {
		HtmlAnim::Vec2Vector pv(n_points);
		for(size_t p_i = 0; p_i < n_points; ++p_i) {
			const auto phi = 2 * HtmlAnim::PI / n_points * p_i;
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
		anim.frame().wait(20);
		anim.next_frame();
	}
	anim.remove_last_frame();
	anim.write_file("example6.html");
}

void make_example_7() {
	HtmlAnim::HtmlAnim anim("Extensions", 600, 250);

	anim.post_text() << R"(
<p>See docs/generate_index.cpp for code.</p>
)";
	anim.post_text() << footer;
	anim.background().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);

	for(size_t n_points = 2; n_points < 9; ++n_points) {
		anim.frame()
			.save()
			.translate(110, 110)
			.add_drawable(HtmlAnimShapes::regular_polygon(0, 0, 100, n_points));
		anim.frame()
			.save()
			.translate(320, 110)
			.add_drawable(HtmlAnimShapes::regular_polygon(0, 0, 100, n_points, true));
		anim.frame().wait(20);
		anim.next_frame();
	}
	anim.remove_last_frame();
	anim.write_file("example7.html");
}

auto count = 0;

void rec_circles(HtmlAnim::HtmlAnim& anim, double x, double y, double r, bool inside, int d=0) {
	if(d > 5)
		return;

	anim.frame().arc(x, y, r);

	const auto f = 0.5;
	if(inside) {
		rec_circles(anim, x - f*r, y, r*f, inside, d+1);
		rec_circles(anim, x + f*r, y, r*f, inside, d+1);
		rec_circles(anim, x, y + f*r, r*f, inside, d+1);
		rec_circles(anim, x, y - f*r, r*f, inside, d+1);
	}
	else {
		rec_circles(anim, x - r - f*r, y, f*r, inside, d+1);
		rec_circles(anim, x + r + f*r, y, f*r, inside, d+1);
		rec_circles(anim, x, y + r + f*r, f*r, inside, d+1);
		rec_circles(anim, x, y - r - f*r, f*r, inside, d+1);
	}

	if(++count % 10 == 0)
		anim.next_frame();
}

void make_example_8() {
	HtmlAnim::HtmlAnim anim("Rewind", 800, 400);
	anim.set_no_clear(true);

	anim.post_text() << R"(
<p>See docs/generate_index.cpp for code.</p>
)";
	anim.post_text() << footer;
	anim.frame().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);

	rec_circles(anim, 200, 200, 90, true);
	anim.rewind();
	rec_circles(anim, 600, 200, 45, false);

	anim.remove_last_frame();
	anim.write_file("example8.html");
}

void make_example_9() {
	HtmlAnim::HtmlAnim anim("Fonts: normal distribution", 520, 620);

	anim.post_text() << R"(
<p>See docs/generate_index.cpp for code.</p>
)";
	anim.post_text() << footer;
	anim.background().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);

	anim.background()
		.add_drawable(HtmlAnimShapes::subdivided_grid(10,10, 50,50, 10,12, 5,5))
		.font("bold 20px sans-serif");
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(25.0, 7.5);
	std::array<int, 50> bins;
	bins.fill(0);
	for(int j = 0; j < 10; ++j) {
		constexpr auto inner_count = 1000;
		anim.frame().text(10, 10+50, std::string("Samples: ") + std::to_string((j+1) * inner_count));
		for(int i = 0; i < inner_count; ++i) {
			const auto number = distribution(generator);
			if((number >= 0.0) && (number < 50.0))
				++bins[static_cast<int>(number)];
		}
		for (int i = 0; i < 50; ++i) {
			const auto count = bins[i];
			anim.frame().rect(10+i*10,10+12*50 - count, 10,count, true);
		}
		anim.frame().wait(20);
		anim.next_frame();
	}
	anim.write_file("example9.html");
}

void equilateral_triangle(HtmlAnim::HtmlAnim &anim, double x, double y, double d) {
	HtmlAnim::Vec2Vector points = {
		HtmlAnim::Vec2(x, y),
		HtmlAnim::Vec2(x + d, y),
		HtmlAnim::Vec2(x + d/2, y - d * sin(HtmlAnim::PI/3))};
	anim.frame().line(points, false, true);
}

void sierpinski(HtmlAnim::HtmlAnim &anim, double x, double y, double d, int depth=0) {
	if(depth > 7)
		return;

	equilateral_triangle(anim, x, y, d);

	sierpinski(anim, x, y, d/2, depth+1);
	sierpinski(anim, x + d/2, y, d/2, depth+1);
	sierpinski(anim, x + d/4, y - d/2 * sin(HtmlAnim::PI/3), d/2, depth+1);

	if(++count % 10 == 0)
		anim.next_frame();
}

void make_demo_sierpinski() {
	HtmlAnim::HtmlAnim anim("HtmlAnim - Demo: Sierpinski triangle", 600, 500);
	anim.set_no_clear(true);

	anim.post_text() << R"(
<p>See docs/generate_index.cpp for code.</p>
)";
	anim.post_text() << footer;
	anim.frame().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);
	sierpinski(anim, 10, 490, 560);
	anim.write_file("demo_sierpinski.html");
}

int main() {
	make_index();
	make_example_1();
	make_example_2();
	make_example_3();
	make_example_4();
	make_example_5();
	make_example_6();
	make_example_7();
	make_example_8();
	make_example_9();

	make_demo_sierpinski();
}
