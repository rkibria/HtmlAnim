#include <htmlanim.hpp>
#include <htmlanim_shapes.hpp>

#include <chrono>
#include <random>

using FitnessType = double;
using GeneType = double;
using GeneVector = std::vector<GeneType>;

class Solution {
	FitnessType fitness;
	GeneVector gene_vec;
	const GeneType radius = 10.0;

	auto get_random_seed() {
		return static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	}

public:
	Solution() = delete;

	Solution(size_t s) : fitness{ 0 }, gene_vec(s * 2, 0) {
	}

	auto get_fitness() const { return fitness; }

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
			anim.frame().arc(x + 300, y + 300, radius);
		}
	}

	void evaluate() {
		fitness = 0;
		for (size_t i = 0; i < gene_vec.size() / 2; ++i) {
			const auto x1 = gene_vec[2 * i];
			const auto y1 = gene_vec[2 * i + 1];
			for (size_t j = i + 1; j < gene_vec.size() / 2; ++j) {
				const auto x2 = gene_vec[2 * i];
				const auto y2 = gene_vec[2 * i + 1];
				const auto d = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
				fitness += abs(d - (radius + radius));
			}
		}
	}
};

using SolutionVector = std::vector<std::unique_ptr<Solution>>;

class Population {
	SolutionVector sol_vec;

	void sort_by_fitness() {

	}

	void evaluate() {

	}

public:
	Population(size_t s, size_t n_genes) {
		sol_vec.resize(s);
		for (size_t i = 0; i < s; ++i) {
			sol_vec[i] = std::make_unique<Solution>(n_genes);
			sol_vec[i]->randomize(-100, 100);
		}
	}

	const auto& get_best() const { return sol_vec.front(); }

	void evolve() {
		for (auto& sol : sol_vec) {
			sol->mutate(0, 1);
			sol->evaluate();
		}
		sort_by_fitness();

	}

};

int main() {
	HtmlAnim::HtmlAnim anim("Evolutionary algorithm", 600, 600);
	anim.frame().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);
	anim.add_layer();

	Population pop(100, 10);

	for (int i = 0; i < 50; ++i) {
		pop.get_best()->draw(anim);

		anim.frame().wait(HtmlAnim::FPS / 10);
		anim.next_frame();

		pop.evolve();
	}

	anim.write_file("evolution.html");
}
