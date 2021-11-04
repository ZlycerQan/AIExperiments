// eight.cpp
#include <algorithm>
#include <iostream>
#include <queue>
#include <array>

int identification() {
	std::cout << "+------------+-----------+" << '\n';
	std::cout << "| 1908090117 | Liquanzhi |" << '\n';
	std::cout << "+------------+-----------+" << '\n';
	return 1;
}

int me = identification();

using en = std::array<std::array<int, 3>, 3>;
using mask = uint64_t;
using position = std::pair<int, int>;

std::istream &operator >> (std::istream& in, en& rhs) {
    for (int i = 0; i < 3; ++ i) {
        for (int j = 0; j < 3; ++ j) {
            in >> rhs[i][j];
        }
    }
    return in;
}

std::ostream &operator << (std::ostream& out, const en& rhs) {
	out << "+-+-+-+" << '\n';
	for (int i = 0; i < 3; ++ i) {
		out << "|";
		for (int j = 0; j < 3; ++ j) {
			out << rhs[i][j] << "|";
		}
		out << '\n' << "+-+-+-+" << '\n';
	}
	return out;
}

template <typename T> 
void log(T x) {
	std::cout << x << '\n';
}

mask ss, ts;

std::vector<int> extend_list[9];

mask encode(const en& x) {
	mask result = 0;
	for (int i = 0; i < 3; ++ i) {
		for (int j = 0; j < 3; ++ j) {
			result = result * 10 + x[i][j];
		}
	}
	return result;
}

en decode(mask x) {
	en result;
	for (int i = 2; i >= 0; -- i) {
		for (int j = 2; j >= 0; -- j) {
			result[i][j] = x % 10;
			x /= 10;
		}
	}
	return result;
}

int h(mask x) {
	int result = 0;
	mask t = ts;
	for (; x; x /= 10, t /= 10) {
		if (x % 10 != t % 10) {
			++ result;
		}
	}
	return result;
}

std::vector<int> split(const mask& now) {
	std::vector<int> result;
	for (mask i = now; i; i /= 10) {
		result.push_back(i % 10);
	}
	if (result.size() != 9) {
		result.push_back(0);
	}
	std::reverse(result.begin(), result.end());
	return result;	
}

mask merge(const std::vector<int>& now) {
	mask result = 0;
	for (const int& i : now) {
		result = result * 10 + i;
	}
	return result;
}

std::vector<mask> extend(const mask& now, const mask& fa) {
	std::vector<mask> result;
	std::vector<int> data = std::move(split(now));
	for (int i = 0; i < (int)data.size(); ++ i) {
		if (data[i] == 0) {
			for (const int& j : extend_list[i]) {
				std::swap(data[i], data[j]);
				mask t = merge(data);
				if (t != fa) {
					result.emplace_back(t);
				}
				std::swap(data[i], data[j]);
			}
			break;
		}
	}
	return result;
}

std::vector<en> calc_process(const std::vector<std::pair<mask, int>>& list) {
	std::vector<en> result;
	result.emplace_back(decode(ts));
	for (int now = list.back().second; now; now = list[now].second) {
		result.emplace_back(decode(list[now].first));
	}
	std::reverse(result.begin(), result.end());
	return result;
}

struct CompareByH {
	bool operator() (const std::pair<mask, int>& lhs, const std::pair<mask, int>& rhs) const {
		return h(lhs.first) > h(rhs.first);
	}
};

std::vector<en> solve(const en& s, const en& t) {
	ss = encode(s);
	ts = encode(t);
	std::priority_queue<std::pair<mask, int>, std::vector<std::pair<mask, int>>, CompareByH> open;
	std::vector<std::pair<mask, int>> closed;
	open.push({ss, 0});
	closed.emplace_back(0, 0);
	for (; !open.empty(); ) {
		auto [now, fa] = open.top();
		open.pop();
		closed.emplace_back(now, fa);
		if (now == ts) {
			return calc_process(closed);
		}
		std::vector<mask> childs = std::move(extend(now, closed[fa].first));
		for (const mask& i : childs) {
			open.emplace(i, (int)closed.size() - 1); 
		}
	}
	return {};
}

void init_extend_list() {
	const static int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};
	auto contains = [] (int x, int y) {
		return x >= 0 && y >= 0 && x < 3 && y < 3;
	};
	auto id = [] (int x, int y) {
		return x * 3 + y;
	};
	for (int i = 0; i < 3; ++ i) {
		for (int j = 0; j < 3; ++ j) {
			for (int k = 0; k < 4; ++ k) {
				int tx = i + dx[k], ty = j + dy[k];
				if (contains(tx, ty)) {
					extend_list[id(i, j)].push_back(id(tx, ty));
				}
			}
		}
	}
}

int main() {
	init_extend_list();
	en s, t;
	log("Please input the initial state.");
	std::cin >> s;
	log("Please input the target state.");
	std::cin >> t;
	auto process = solve(s, t);
	log("Total " + std::to_string(process.size() - 1) + " steps.");
	for (int i = 0; i < (int)process.size(); ++ i) {
		log(process[i]);
		if (i != (int)process.size() - 1) {
			log("  ||");
			log("  \\/");
		}
	}
}

/*
2 8 3
1 0 4
7 6 5
1 2 3
8 0 4
7 6 5
*/
