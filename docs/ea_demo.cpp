#include <htmlanim.hpp>
#include <htmlanim_shapes.hpp>

#include "ea_base.h"

using namespace EA;

class CirclesSolution : public SolutionBase {
	static constexpr size_t fitness_size = 2;
	static constexpr size_t num_circles = 10;

	GeneType radius(size_t i) const {
		return static_cast<GeneType>(i/5 + 1) * 5.0;
	}

	auto get_x(size_t i) const { return gene_vec[2 * i]; }
	auto get_y(size_t i) const { return gene_vec[2 * i + 1]; }

public:
	CirclesSolution() : SolutionBase(fitness_size, num_circles * 2) {
		randomize(-300, 300);
	}

	void draw(HtmlAnim::HtmlAnim& anim) const {
		for (size_t i = 0; i < gene_vec.size() / 2; ++i) {
			anim.frame().arc(get_x(i) + 300, get_y(i) + 300, radius(i), true);
		}
	}

	virtual void evaluate() override {
		std::fill(fitness.begin(), fitness.end(), 0);

		for (size_t i = 0; i < gene_vec.size() / 2; ++i) {
			const auto x1 = get_x(i);
			const auto y1 = get_y(i);

			double overlap = 0.0;
			double d_others = 0.0;
			for (size_t j = 0; j < gene_vec.size() / 2; ++j) {
				if (i == j)
					continue;
				const auto x2 = get_x(j);
				const auto y2 = get_y(j);
				const auto sq_d = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
				const auto d = sqrt(sq_d);

				const auto touch_d = radius(i) + radius(j);
				if (d < touch_d) {
					overlap += touch_d - d;
				}
				else {
					d_others += d;
				}
			}

			fitness[0] += overlap;
			fitness[1] += d_others;
		}
	}
};

int main() {
	HtmlAnim::HtmlAnim anim("Evolutionary algorithm", 600, 600);
	anim.frame().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);
	anim.frame().add_drawable(HtmlAnimShapes::subdivided_grid(0, 0, 50, 50, 12, 12, 5, 5));
	anim.add_layer();

	Population<CirclesSolution, 8> pop(10000);

	FitnessType best_fitness;
	CirclesSolution best_solution;

	const auto start_time = std::chrono::high_resolution_clock::now();

	auto round_time = [](double t) {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(1) << t;
		return ss.str();
	};

	for (int i = 0; i < 2000; ++i) {
		const auto& current_best = pop.get_best();
		if (i == 0 || current_best->get_fitness() < best_fitness) {
			best_fitness = current_best->get_fitness();
			best_solution = dynamic_cast<CirclesSolution&>(*current_best);
		}
		best_solution.draw(anim);

		const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start_time;

		if (i % 10 == 0) {
			anim.frame()
				.font("bold 30px sans-serif")
				.text(30, 30, "Gen " + std::to_string(i))
				.font("bold 14px sans-serif")
				.text(30, 50, round_time(i / elapsed.count()) + " gens/sec")
				.wait(HtmlAnim::FPS / 10);
			anim.next_frame();
		}

		pop.evolve(0.5 + 10.0 / (static_cast<GeneType>(i) + 1));
	}

	anim.write_file("evolution.html");
}
