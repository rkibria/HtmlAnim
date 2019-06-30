#include <htmlanim.hpp>
#include <htmlanim_shapes.hpp>

#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include <thread>

using FitnessType = std::vector<double>;
using GeneType = double;
using GeneVector = std::vector<GeneType>;

class Solution {
	static constexpr size_t fitness_size = 2;
	FitnessType fitness;
	GeneVector gene_vec;

	auto get_random_seed() {
		return static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	}

	GeneType radius(size_t i) const {
		return static_cast<GeneType>(i/5 + 1) * 5.0;
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
			anim.frame().arc(x + 300, y + 300, radius(i), true);
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

using SolutionVector = std::vector<std::unique_ptr<Solution>>;

class Population {
	SolutionVector sol_vec;

	static const constexpr size_t n_threads = 8;

	void sort_by_fitness() {
		std::sort(sol_vec.begin(), sol_vec.end(),
			[](const auto& sol1, const auto& sol2) {
				return sol1->get_fitness() < sol2->get_fitness(); 
			});
	}

	void evaluate(GeneType mutation_stddev) {
		std::vector<std::thread> threads;
		if (sol_vec.size() % n_threads != 0) {
			throw std::exception("Population size must be multiple of n_threads");
		}

		auto evaluate_thread = [this, mutation_stddev](size_t start_i, size_t end_i) {
			for (size_t i = start_i; i < end_i; ++i) {
				auto& sol = sol_vec[i];
				sol->mutate(0, mutation_stddev);
				sol->evaluate();
			}
		};

		const auto batch_size = sol_vec.size() / n_threads;
		for (size_t i = 0; i < n_threads; ++i) {
			threads.push_back(std::thread(evaluate_thread, i * batch_size, (i + 1) * batch_size));
		}

		for (auto& t : threads) {
			if (t.joinable())
				t.join();
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
		evaluate(0);
		sort_by_fitness();
	}

	const auto& get_best() const {
		return sol_vec.front();
	}

	void evolve(GeneType stddev) {
		for (size_t i = 0; i < sol_vec.size() / 2; ++i) {
			*sol_vec[sol_vec.size() / 2 + i] = *sol_vec[i];
		}

		evaluate(stddev);

		sort_by_fitness();
	}

};

int main() {
	HtmlAnim::HtmlAnim anim("Evolutionary algorithm", 600, 600);
	anim.frame().save().fill_style("white").rect(0, 0, anim.get_width(), anim.get_height(), true);
	anim.frame().add_drawable(HtmlAnimShapes::subdivided_grid(0, 0, 50, 50, 12, 12, 5, 5));
	anim.add_layer();

	Population pop(10000, 25);

	FitnessType best_fitness;
	Solution best_solution;

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
			best_solution = *current_best;
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
