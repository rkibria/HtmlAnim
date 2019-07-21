#include <htmlanim.hpp>
#include <htmlanim_shapes.hpp>

#include "ea_base.h"

using namespace EA;

class CirclesSolution : public SolutionBase {
	static constexpr size_t fitness_size = 1;
	static constexpr size_t num_circles = 1;

	GeneType radius(size_t i) const {
		return static_cast<GeneType>(i/5 + 1) * 5.0;
	}

	auto get_x(size_t i) const { return gene_vec[2 * i]; }
	auto get_y(size_t i) const { return gene_vec[2 * i + 1]; }

public:
	static constexpr int range = 300;

	CirclesSolution() : SolutionBase(fitness_size, num_circles * 2) {
		randomize(-range, range);
	}

	void draw(HtmlAnim::HtmlAnim& anim) const {
		for (size_t i = 0; i < gene_vec.size() / 2; ++i) {
			anim.frame().arc(get_x(i) + range, get_y(i) + range, radius(i), true);
		}
	}

	virtual void evaluate() override {
		std::fill(fitness.begin(), fitness.end(), 0);

		for (size_t i = 0; i < gene_vec.size() / 2; ++i) {
			const auto x1 = get_x(i);
			const auto y1 = get_y(i);
			fitness[0] += x1 * x1 + y1 * y1;
		}
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
		best_solution.draw(anim);
		anim.next_frame();

		pop.evolve(10);
	}

	anim.write_file("ea_vis1.html");
}
