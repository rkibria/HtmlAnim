#pragma once

#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include <thread>

namespace EA {

using FitnessType = std::vector<double>;
using GeneType = double;
using GeneVector = std::vector<GeneType>;

class SolutionBase {
protected:
	FitnessType fitness;
	GeneVector gene_vec;

	auto get_random_seed() {
		return static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	}

public:
	explicit SolutionBase(size_t fitness_size, size_t s)
		: fitness(fitness_size, 0), gene_vec(s, 0) {
	}

	virtual ~SolutionBase() {}

	virtual SolutionBase& operator=(const SolutionBase& rhs) {
		gene_vec = rhs.gene_vec;
		return *this;
	}

	virtual const FitnessType& get_fitness() const { return fitness; }

	virtual void randomize(GeneType lb, GeneType ub) {
		std::default_random_engine generator(get_random_seed());
		std::uniform_real_distribution<GeneType> distribution(lb, ub);
		for (auto& g : gene_vec) {
			g = distribution(generator);
		}
	}

	virtual void mutate(GeneType mean, GeneType stddev) {
		std::default_random_engine generator(get_random_seed());
		std::normal_distribution<GeneType> distribution(mean, stddev);
		for (auto& g : gene_vec) {
			const auto r = distribution(generator);
			g += r;
		}
	}

	virtual void evaluate() = 0;
};

template<class T, size_t n_threads>
class Population {
	using SolutionVector = std::vector<std::unique_ptr<SolutionBase>>;
	SolutionVector sol_vec;

	void sort_by_fitness() {
		std::sort(sol_vec.begin(), sol_vec.end(),
			[](const auto& sol1, const auto& sol2) {
				return sol1->get_fitness() < sol2->get_fitness(); 
			});
	}

	void mutate_and_evaluate(GeneType mutation_stddev) {
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

	void procreate() {
		for (size_t i = 0; i < sol_vec.size() / 2; ++i) {
			*sol_vec[sol_vec.size() / 2 + i] = *sol_vec[i];
		}
	}

public:
	Population(size_t s) {
		sol_vec.resize(s);
		for (size_t i = 0; i < s; ++i) {
			sol_vec[i] = std::make_unique<T>();
		}

		mutate_and_evaluate(0);
		sort_by_fitness();
	}

	const auto& get_best() const {
		return sol_vec.front();
	}

	void evolve(GeneType stddev) {
		procreate();
		mutate_and_evaluate(stddev);
		sort_by_fitness();
	}

};

} // namespace EA
