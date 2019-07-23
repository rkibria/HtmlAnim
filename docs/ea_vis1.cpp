#include <htmlanim.hpp>
#include <htmlanim_shapes.hpp>

#include "ea_base.h"

using namespace EA;

class CirclesSolution : public SolutionBase {
	static constexpr size_t radius = 5;

	auto get_x() const { return gene_vec[0]; }
	auto get_y() const { return gene_vec[1]; }

public:
	static constexpr int range = 300;

	CirclesSolution() : SolutionBase(1, 2) {
		randomize(-range, range);
	}

	void draw(HtmlAnim::HtmlAnim& anim) const {
		anim.frame().arc(get_x() + range, get_y() + range, radius, true);
	}

	virtual void evaluate() override {
		fitness[0] = get_x() * get_x() + get_y() * get_y();
	}
};

int main() {
	static constexpr auto screen_size = CirclesSolution::range * 2;
	
	HtmlAnim::HtmlAnim anim("Evolutionary algorithm", screen_size, screen_size);
	anim.frame().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);
	anim.frame().add_drawable(HtmlAnimShapes::subdivided_grid(0, 0, 50, 50, screen_size/50, screen_size/50, 5, 5));
	anim.add_layer();

	Population<CirclesSolution, 2> pop(2);

	FitnessType best_fitness;
	CirclesSolution best_solution;

	for (int i = 0; i < 50; ++i) {
		const auto& current_best = pop.get_best();
		if (i == 0 || current_best->get_fitness() < best_fitness) {
			best_fitness = current_best->get_fitness();
			best_solution = dynamic_cast<CirclesSolution&>(*current_best);
		}

		for (size_t j = 0; j < pop.size(); ++j) {
			pop.get(j)->draw(anim);
		}

		// best_solution.draw(anim);

		anim.next_frame();

		pop.evolve(10);
	}

	anim.write_file("ea_vis1.html");
}
