#include <htmlanim.hpp>
#include <htmlanim_shapes.hpp>

#include <chrono>
#include <random>
#include <algorithm>

using FitnessType = std::vector<double>;
using GeneType = double;
using GeneVector = std::vector<GeneType>;

class Solution {
	static constexpr size_t fitness_size = 2;
	FitnessType fitness;
	GeneVector gene_vec;
	const GeneType radius = 10.0;

	auto get_random_seed() {
		return static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	}

public:
	Solution() : fitness(fitness_size, 0) {}

	Solution(size_t s) : fitness(fitness_size, 0), gene_vec(s * 2, 0) {
	}

	Solution& operator=(const Solution& rhs) {
		gene_vec = rhs.gene_vec;
		return *this;
	}

	const auto& get_fitness() const { return fitness; }

	void randomize(GeneType lb, GeneType ub) {
		std::default_random_engine generator(get_random_seed());
		std::uniform_real_distribution<GeneType> distribution(lb, ub);
		for (auto& g : gene_vec) {
			g = distribution(generator);
		}
	}

	void mutate(GeneType mean, GeneType stddev) {
		std::default_random_engine generator(get_random_seed());
		std::normal_distribution<GeneType> distribution(mean, stddev);
		for (auto& g : gene_vec) {
			const auto r = distribution(generator);
			g += r;
		}
	}

	void draw(HtmlAnim::HtmlAnim& anim) const {
		for (size_t i = 0; i < gene_vec.size() / 2; ++i) {
			const auto x = gene_vec[2 * i];
			const auto y = gene_vec[2 * i + 1];
			anim.frame().arc(x + 300, y + 300, radius, true);
		}
	}

	void evaluate() {
		std::fill(fitness.begin(), fitness.end(), 0);
		for (size_t i = 0; i < gene_vec.size() / 2; ++i) {
			const auto x1 = gene_vec[2 * i];
			const auto y1 = gene_vec[2 * i + 1];

			double overlap = 0.0;
			double d_others = 0.0;
			for (size_t j = 0; j < gene_vec.size() / 2; ++j) {
				if (i == j)
					continue;
				const auto x2 = gene_vec[2 * j];
				const auto y2 = gene_vec[2 * j + 1];
				const auto sq_d = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
				const auto d = sqrt(sq_d);

				const auto touch_d = 2 * radius;
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

using SolutionVector = std::vector<std::unique_ptr<Solution>>;

class Population {
	SolutionVector sol_vec;

	void sort_by_fitness() {
		std::sort(sol_vec.begin(), sol_vec.end(),
			[](const auto& sol1, const auto& sol2) {
				return sol1->get_fitness() < sol2->get_fitness(); 
			});
	}

	void evaluate() {
		for (auto& sol : sol_vec) {
			sol->evaluate();
		}
	}

	void mutate(GeneType stddev) {
		for (auto& sol : sol_vec) {
			sol->mutate(0, stddev);
		}
	}

	void randomize() {
		for (auto& sol : sol_vec) {
			sol->randomize(-300, 300);
		}
	}

public:
	Population(size_t s, size_t n_genes) {
		sol_vec.resize(s);
		for (size_t i = 0; i < s; ++i) {
			sol_vec[i] = std::make_unique<Solution>(n_genes);
		}
		randomize();
		evaluate();
		sort_by_fitness();
	}

	const auto& get_best() const {
		return sol_vec.front();
	}

	void evolve(int gen) {
		for (size_t i = 0; i < sol_vec.size() / 2; ++i) {
			*sol_vec[sol_vec.size() / 2 + i] = *sol_vec[i];
		}

		mutate(0.5 + 10.0 / (static_cast<GeneType>(gen) + 1));

		evaluate();
		sort_by_fitness();
	}

};

int main() {
	HtmlAnim::HtmlAnim anim("Evolutionary algorithm", 600, 600);
	anim.frame().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);
	anim.frame().add_drawable(HtmlAnimShapes::subdivided_grid(0, 0, 50, 50, 12, 12, 5, 5));
	anim.add_layer();

	Population pop(10000, 20);

	FitnessType best_fitness;
	Solution best_solution;

	for (int i = 0; i < 2000; ++i) {
		const auto& current_best = pop.get_best();
		if (i == 0 || current_best->get_fitness() < best_fitness) {
			best_fitness = current_best->get_fitness();
			best_solution = *current_best;
		}
		best_solution.draw(anim);

		anim.frame()
			.font("bold 30px sans-serif")
			.text(30, 30, std::to_string(i));
			//.wait(HtmlAnim::FPS / 10);
		anim.next_frame();

		pop.evolve(i);
	}

	anim.write_file("evolution.html");
}
