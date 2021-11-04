// maze.cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>

int identification() {
	std::cout << "+------------+-----------+" << '\n';
	std::cout << "| 1908090117 | Liquanzhi |" << '\n';
	std::cout << "+------------+-----------+" << '\n';
	return 1;
}

int me = identification();

using maze = std::vector<std::vector<int>>;
using pos = std::pair<int, int>;
using path = std::vector<pos>;

template <typename T>
void log(T x) {
	std::cout << x << '\n';
}

void trim(std::string& x) {
	int cnt = 0;
	for (int i = x.size() - 1; i >= 0; -- i) {
		if (x[i] != ' ' && x[i] != '\r' && x[i] != '\n') {
			break;
		}
		++ cnt;
	}
	for (; cnt; -- cnt, x.pop_back());
}

std::istream &operator >> (std::istream& in, maze& rhs) {
	int m = 0;
	for (; ; ) {
		std::string x;
		if (!std::getline(in, x)) {
			break;
		}
		trim(x);
		std::vector<int> dat;
		for (int i = 0; i < (int)x.size(); i += 2) {
			dat.push_back(x[i] == '*');
		}
		rhs.emplace_back(dat);
		m = std::max(m, (int)dat.size());
	}
	for (int i = 0; i < (int)rhs.size(); ++ i) {
		for (; (int)rhs[i].size() < m; ) {
			rhs[i].emplace_back(0);
		}
	}
	return in;
}

std::ostream &operator << (std::ostream& out, const pos& rhs) {
	out << "(" << rhs.first << ", " << rhs.second << ")";
	return out;
}

std::ostream &operator << (std::ostream& out, const path& rhs) {
	for (int i = 0; i < (int)rhs.size(); ++ i) {
		out << rhs[i];
		if (i != (int)rhs.size() - 1) {
			out << "->";
		}
	}
	return out;
}

std::ostream &operator << (std::ostream& out, const maze& rhs) {
	for (const auto& i : rhs) {
		for (int j = 0; j < (int)i.size(); ++ j) {
			char x;
			if (i[j] == 1) {
				x = '*';
			} else if (i[j] == 0) {
				x = ' ';
			} else {
				x = '0';
			}
			out << x << (" \n"[j == (int)i.size() - 1]);
		}
	}
	return out;
}

bool operator == (const pos& lhs, const pos& rhs) {
	return lhs.first == rhs.first && lhs.second == rhs.second;
}

std::pair<pos, pos> get_st(const maze& mz) {
	int n = mz.size(), m = mz[0].size();
	std::vector<pos> lis;
	for (int i = 0; i < m; ++ i) {
		if (mz[0][i] == 0) {
			lis.emplace_back(0, i);
		}
		if (mz[n - 1][i] == 0) {
			lis.emplace_back(n - 1, i);
		}
	}
	for (int i = 0; i < n; ++ i) {
		if (mz[i][0] == 0) {
			lis.emplace_back(i, 0);
		}
		if (mz[i][m - 1] == 0) {
			lis.emplace_back(i, m - 1);
		}
	}
	if (lis.size() != 2) {
		log("maze is illegal.");
		exit(0);
	}
	return {lis[0], lis[1]};
}

bool not_maze(const maze& mz) {
	int n = mz.size();
	if (n < 3) {
		return true;
	}
	int m = mz[0].size();
	if (m < 3) {
		return true;
	}
	return false;
}

maze mz;
int n, m;
pos s, t;
const static int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};
bool contains(int x, int y) {
	return x >= 0 && y >= 0 && x < n && y < m;
}

std::map<pos, pos> pre;
std::vector<path> ans;
path calc_process() {
	pos border = {-1, -1};
	path result;
	for (pos now = t; !(now == border); ) {
		result.push_back({now.first, now.second});
		now = pre[now];
	}
	std::reverse(result.begin(), result.end());
	return result;
}

void dfs(pos now) {
	if (now == t) {
		ans.emplace_back(calc_process());
		return ;
	}
	for (int i = 0; i < 4; ++ i) {
		int tx = now.first + dx[i];
		int ty = now.second + dy[i];
		if (contains(tx, ty) && !mz[tx][ty] && !pre.count({tx, ty})) {
			pre[{tx, ty}] = now;
			dfs({tx, ty});
			pre.erase({tx, ty});
		}
	}
}

int main() {
	log("Please input the maze.");
	log("Press Ctrl + Z to end the input.");
	std::cin >> mz;
	if (not_maze(mz)) {
		log("maze is illegal.");
		return 0;
	}
	n = mz.size();
	m = mz[0].size();
	auto st = get_st(mz);
	s = st.first;
	t = st.second;
	pre[s] = {-1, -1};
	dfs(s);
	log("Total " + std::to_string(ans.size()) + " paths.");	
	for (int i = 0; i < (int)ans.size(); ++ i) {
		log(i + 1);
		maze tmp = mz;
		log(ans[i]);
		for (pos j : ans[i]) {
			tmp[j.first][j.second] = 2;
		}
		log("");
		log(tmp);
	}
		
	return 0;
}

/*
* * * * * * * * * *
      *       *   *
* *   *       *   *
*         * *     *
*   * * *         *
*       *          
*   *       *     *
*   * * *   * *   *
* *               *
* * * * * * * * * *
*/
