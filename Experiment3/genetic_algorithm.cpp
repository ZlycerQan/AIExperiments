#include <random>
#include <iostream>
#include <vector>
#include <bitset>
#include <chrono>
#include <algorithm>

int identification() {
	std::cout << "+------------+-----------+" << '\n';
	std::cout << "| 1908090117 | Liquanzhi |" << '\n';
	std::cout << "+------------+-----------+" << '\n';
	return 1;
}

int me = identification();

using flo = double;

constexpr int NUMBER_PERCISION = 4;
constexpr int NUMBER_EXPANSION_VALUE = pow(10, NUMBER_PERCISION);

constexpr int NUMBER_OF_EVOLUTION = 10000;
constexpr int INITIAL_NUMBER = 400;
constexpr flo LOWER_BOUND = -2.048;
constexpr flo UPPER_BOUND = 2.048;
constexpr flo CROSS_LEFT_GENE_PROBABILITY = 0.2;
constexpr flo CROSS_RIGHT_GENE_PROBABILITY = 0.2;
constexpr flo MUTATE_LEFT_GENE_PROBABILITY = 0.07;
constexpr flo MUTATE_RIGHT_GENE_PROBABILITY = 0.07;

constexpr flo CROSS_PROBABILITY = CROSS_LEFT_GENE_PROBABILITY + CROSS_RIGHT_GENE_PROBABILITY;
constexpr flo MUTATE_PROBABILITY = MUTATE_LEFT_GENE_PROBABILITY + MUTATE_RIGHT_GENE_PROBABILITY;
constexpr int BIT_LENGTH = log2((UPPER_BOUND - LOWER_BOUND) * NUMBER_EXPANSION_VALUE) + 1;
constexpr int BORDER = LOWER_BOUND < 0 ? -LOWER_BOUND * NUMBER_EXPANSION_VALUE : 0;

using gene = std::bitset<BIT_LENGTH>;

gene encode(const flo& value) {
	return gene(value * NUMBER_EXPANSION_VALUE + BORDER);
}

flo decode(const gene& value) {
	return (static_cast<flo>(value.to_ulong()) - BORDER) / NUMBER_EXPANSION_VALUE;
}

using chromosome = std::pair<gene, gene>;
using group = std::vector<chromosome>;

inline flo f(const chromosome& x) {
	const flo x1 = decode(x.first);
	const flo x2 = decode(x.second);
	return 100.0 * (x1 * x1 - x2) * (x1 * x2 - x2) + (1 - x1) * (1 - x1);
}

std::mt19937 rng = std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count());

std::uniform_real_distribution<> get_random(const flo& l, const flo& r) {
	return std::uniform_real_distribution<>(l, r);
}

inline bool contains(const gene& x) {
	flo value = decode(x);
	return value >= LOWER_BOUND && value <= UPPER_BOUND;
}

void select(group& popu) {
	const int n = popu.size();
	auto gen = get_random(0, 1);
	std::vector<flo> adapt;
	flo sum = 0;
	for (const chromosome& i : popu) {
		adapt.emplace_back(f(i));
		sum += adapt.back();
	}
	for (flo& i : adapt) {
		i = i / sum;
	}
	for (int i = 1; i < n; ++ i) {
		adapt[i] = adapt[i - 1] + adapt[i]; 
	}
	group result = popu;
	for (int i = 0; i < n; ++ i) {
		popu[i] = result[std::upper_bound(adapt.begin(), adapt.end(), gen(rng)) - adapt.begin()];
	}
}

void cross_gene(gene& l, gene& r, int lp, int rp = BIT_LENGTH) {;
	for (int i = lp; i < rp; ++ i) {
		int p = l[i], q = r[i];
		l.set(i, q);
		r.set(i, p);
	}
	if (!contains(l) || !contains(r)) {
		for (int i = lp; i < rp; ++ i) {
			int p = l[i], q = r[i];
			l.set(i, q);
			r.set(i, p);
		}
	}
}

void cross_chromosome(chromosome& lhs, chromosome& rhs) {
	auto gen = get_random(0, 1);
	if (gen(rng) <= CROSS_LEFT_GENE_PROBABILITY) {
		int p = gen(rng) * BIT_LENGTH;
		cross_gene(lhs.first, rhs.first, p);
	}
	if (gen(rng) <= CROSS_RIGHT_GENE_PROBABILITY) {
		int p = gen(rng) * BIT_LENGTH;
		cross_gene(lhs.second, rhs.second, p);
	}
}

void cross(group& popu) {
	auto gen = get_random(0, 1);
	std::shuffle(popu.begin(), popu.end(), rng);
	for (int i = 0; i < (int)popu.size(); i += 2) {
		if (gen(rng) <= CROSS_PROBABILITY) {
			cross_chromosome(popu[i], popu[i + 1]);
		}
	}
}

void mutate_chromosome(chromosome& x) {
	auto gen = get_random(0, 1);
	if (gen(rng) <= MUTATE_LEFT_GENE_PROBABILITY) {
		int p = gen(rng) * BIT_LENGTH;
		x.first.flip(p);
		if (!contains(x.first)) {
			x.first.flip(p);
		}
	}
	if (gen(rng) <= MUTATE_RIGHT_GENE_PROBABILITY) {
		int p = gen(rng) * BIT_LENGTH;
		x.second.flip(p);
		if (!contains(x.second)) {
			x.second.flip(p);
		}
	}
}

void mutate(group& popu) {
	auto gen = get_random(0, 1);
	for (chromosome& i : popu) {
		if (gen(rng) <= MUTATE_PROBABILITY) {
			mutate_chromosome(i);
		}
	}
}

flo run_genetic_algorithm() {
	auto gen = get_random(LOWER_BOUND, UPPER_BOUND);
	group popu;
	for (int i = 1; i <= INITIAL_NUMBER; ++ i) {
		popu.emplace_back(encode(gen(rng)), encode(gen(rng)));
	}
	flo ans = 0;
	chromosome res;
	for (int cas = 1; cas <= NUMBER_OF_EVOLUTION; ++ cas) {
		std::vector<flo> adapt(popu.size());
		select(popu);
		cross(popu);
		mutate(popu);
		for (const chromosome& i : popu) {
			if (f(i) > ans) {
				ans = f(i);
				res = i;
			}
		}
	}
	std::cout << "x1: " << decode(res.first) << " x2: " << decode(res.second) << '\n';
	std::cout << "ans: " << ans;
}

int main() {
	run_genetic_algorithm();
	return 0;
}
