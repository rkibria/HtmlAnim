#include <htmlanim.hpp>
#include <htmlanim_shapes.hpp>

int main() {
	const auto width = 800;
	const auto height = 450;
	HtmlAnim::HtmlAnim anim("Logo", width, height);

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

	anim.write_file("logo.html");
}
