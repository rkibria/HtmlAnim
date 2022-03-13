#include <htmlanim.hpp>
#include <htmlanim_shapes.hpp>

int main() {
	HtmlAnim::HtmlAnim anim("Offscreens", 600, 600);
    anim.set_num_surfaces(1);

    anim.frame().surface(0).line(0,0, 600, 600).line(0,600, 600, 0);
    anim.frame().save().stroke_style("red").line(0,0, 600, 600).line(0,600, 600, 0);
    anim.frame().drawImage(0,
        0, 0, 600, 600,
        0, 0, 300, 300);

	anim.write_file("offscreens.html");
}
