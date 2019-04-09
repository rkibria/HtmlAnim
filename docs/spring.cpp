#include "htmlanim.hpp"

int main() {
	HtmlAnim::HtmlAnim anim("Spring", 600, 600);
	HtmlAnim::Vector2D pos{0, 0}, velocity{1, 0};
	const auto delta_t = 1.0;
	for(double frame = 0; frame < 60; ++frame) {
		anim.frame().line(300, 300,  pos.x + 300, pos.y + 300);
		anim.frame().arc(pos.x + 300, pos.y + 300, 20);

		HtmlAnim::Vector2D acceleration{0, 1};
		acceleration += -pos * 0.01;
		velocity += acceleration;
		pos += delta_t * velocity;

		anim.next_frame();
	}
	anim.remove_last_frame();
	anim.write_file("spring.html");
}
