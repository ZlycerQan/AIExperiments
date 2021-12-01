// pso_algorithm.cpp
#include <iostream>
#include <array>
#include <random>
#include <chrono>
#include <iomanip>

int identification() {
	std::cout << "+------------+-----------+" << '\n';
	std::cout << "| 1908090117 | Liquanzhi |" << '\n';
	std::cout << "+------------+-----------+" << '\n';
	return 1;
}

int me = identification();

using flo = double;

constexpr int NUMBER_OF_EVOLUTION = 862353;
constexpr int X_NUMBER = 30;
constexpr int PARTICLE_NUMBER = 30;
constexpr flo UPPER_BOUND = 2;
constexpr flo LOWER_BOUND = 0;
constexpr flo C1 = 0.05;
constexpr flo C2 = 0.05;
constexpr flo V_MAX = 0.0005;

using particle = std::array<flo, X_NUMBER>;
using swarm = std::array<particle, PARTICLE_NUMBER>;

using velocity = std::array<flo, X_NUMBER>;

std::mt19937 rng = std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count());

std::uniform_real_distribution<> get_real_random(const flo& l, const flo& r) {
	return std::uniform_real_distribution<>(l, r);
}

swarm create_initial_swarm() {
	swarm result;
	auto gen  = get_real_random(LOWER_BOUND, UPPER_BOUND);
	for (int i = 0; i < PARTICLE_NUMBER; ++ i) {
		particle x;
		for (int j = 0; j < X_NUMBER; ++ j) {
			x[j] = gen(rng);
		}
		result[i] = std::move(x);
	}
	return result;
}

flo f(const particle& x) {
	flo result = 0;
	for (const flo& i : x) {
		result += i * i;
	}
	return result;
}

void run_pso() {
	flo result = 1e9;
	particle result_pos;
	auto gen = get_real_random(0, 1);
	swarm sw = create_initial_swarm();
	std::array<flo, PARTICLE_NUMBER> p_value;
	std::array<particle, PARTICLE_NUMBER> p_pos;
	std::array<velocity, PARTICLE_NUMBER> v;
	for (int i = 0; i < PARTICLE_NUMBER; ++ i) {
		p_value[i] = f(sw[i]);
		p_pos[i] = sw[i];
		v[i].fill(0);
	}
	for (int evo = 1; evo <= NUMBER_OF_EVOLUTION; ++ evo) {
		particle g_pos;
		flo g_value = 1e9;
		for (int i = 0; i < PARTICLE_NUMBER; ++ i) {
			flo y = f(sw[i]);
			if (y < result) {
				result = y;
				result_pos = sw[i];
			}
			if (y < p_value[i]) {
				p_value[i] = y;
				p_pos[i] = sw[i];
			}
			if (y < g_value) {
				g_value = y;
				g_pos = sw[i];
			}
		}
		for (int i = 0; i < PARTICLE_NUMBER; ++ i) {
			for (int j = 0; j < X_NUMBER; ++ j) {
				v[i][j] += C1 * gen(rng) * (p_pos[i][j] - sw[i][j]) + C2 * gen(rng) * (g_pos[j] - sw[i][j]);
				if (v[i][j] >= V_MAX) {
					v[i][j] = V_MAX;
				}
				if (v[i][j] <= -V_MAX) {
					v[i][j] = -V_MAX;
				}
				sw[i][j] += v[i][j];
				if (sw[i][j] > UPPER_BOUND) {
					sw[i][j] = LOWER_BOUND + fmod(sw[i][j] - LOWER_BOUND, UPPER_BOUND - LOWER_BOUND);
				} else if (sw[i][j] < LOWER_BOUND) {
					sw[i][j] = UPPER_BOUND - fmod(UPPER_BOUND - sw[i][j], UPPER_BOUND - LOWER_BOUND);
				}
			}
		}
	}
	
	for (int i = 0; i < X_NUMBER; ++ i) {
		std::cout << "x" << i + 1 << ": " << std::fixed << std::setprecision(2) << result_pos[i] << '\n'; 	
	}
	printf("%.2lf", result);
}

int main() {
	run_pso();
	return 0;
}
