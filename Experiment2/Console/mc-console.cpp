// mc-console.cpp
#include <iostream>
#include <queue>
#include <algorithm>
#include <tuple>
#include <map>

struct state {
	
	int x1, x2, x3;
	
	bool operator == (const state& rhs) const {
		return x1 == rhs.x1 && x2 == rhs.x2 && x3 == rhs.x3;
	}
	
	bool operator < (const state& rhs) const {
		if (x1 == rhs.x1) {
			if (x2 == rhs.x2) {
				return x3 < rhs.x3;
			}
			return x2 < rhs.x2;
		}
		return x1 < rhs.x1;
	}
	
};

std::ostream &operator << (std::ostream& out, const state& rhs) {
	out << "(" << rhs.x1 << ", " << rhs.x2 << ", " << rhs.x3 << ")";
	return out;
}

int f(const state& key) {
	return key.x1 + key.x2 - (key.x3 << 1);
}

struct CompareByF {
	bool operator () (const state& lhs, const state& rhs) {
		return f(lhs) < f(rhs);
	}
};

std::vector<state> solve(int n, int c) {
	const state end = {0, 0, 0};
	const state border = {-1, -1, -1};
	const state start = state{n, n, 1};
	std::priority_queue<state, std::vector<state>, CompareByF> q;
	std::map<state, state> pre;
	q.push(start);
	pre[start] = border;
	auto contains = [=](const state& s) {
		return (s.x1 >= s.x2 || s.x1 == 0) && (s.x1 >= 0 && s.x2 >= 0);
	};
	auto opposite = [=](const state& s) {
		return state{n - s.x1, n - s.x2, s.x3 ^ 1};
	};
	auto calc_process = [&]() {
		std::vector<state> result;
		state now = {0, 0, 0};
		state border = {-1, -1, -1};
		for (; !(now == border); now = pre[now]) {
			result.push_back(now);
		}
		std::reverse(result.begin(), result.end());
		return result;
	};
	for (; !q.empty(); ) {
		state now = q.top();
		q.pop();
		if (now == end) {
			return calc_process();
		}
		for (int j = 0; j <= c; ++ j) {
			for (int i = j; i + j <= c; ++ i) {
				if (i + j == 0) {
					continue;
				}
				int t = now.x3 ? -1 : 1;
				state tmp = {now.x1 + t * i, now.x2 + t * j, now.x3 ^ 1};
				state opt = opposite(tmp);
				if (contains(tmp) && contains(opt) && !pre.count(tmp)) {
					q.emplace(tmp);
					pre[tmp] = now;
				}
			}
		}
	}
	return {};
}

template <typename T>
void log(T x) {
	std::cout << x << '\n';
}

int main() {
	int n, k;
	log("Please input the n and c.");
	std::cin >> n >> k;
	auto p = solve(n, k);
	if (p.size() == 0) {
		log("No solution!");
		return 0;
	}
	log("Total " + std::to_string(p.size() - 1) + " step.");
	for (int i = 0; i < (int)p.size(); ++ i) {
		std::cout << p[i];
		if (i != p.size() - 1) {
			std::cout << "->";
		}
	}
	return 0;
}
