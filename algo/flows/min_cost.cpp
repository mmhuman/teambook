#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define mp make_pair
#define X first
#define Y second

typedef long long flow_t;
typedef long long cost_t;
const flow_t flow_inf = 1e18;
const cost_t cost_inf = 1e18;


struct edge {
	int v, to;
	flow_t c, f;
	cost_t cost;
	edge(int v, int to, flow_t c, cost_t cost): v(v), to(to), c(c), f(0), cost(cost) {}
};

struct Network {
	int n;
	int s, t;
	vector <edge> es;
	vector <vector <int> > g;
	Network(int n, int s, int t): n(n), s(s), t(t), g(n) {}

	void add_edge(int v, int to, flow_t c, cost_t cost) {
		g[v].push_back(es.size());
		es.push_back(edge(v, to, c, cost));
		g[to].push_back(es.size());
		es.push_back(edge(to, v, 0, -cost));
	}

	void add_flow(int e, flow_t f) {
		es[e].f += f;
		es[e ^ 1].f -= f;
	}
};


class MinCost {
public:
	flow_t total_flow;
	cost_t total_cost;
	Network& G;
	vector <edge> &es;
	vector <vector <int> > &g;
	cost_t *pot, *d;
	int *from;

	MinCost(Network& G): G(G), es(G.es), g(G.g) {
		pot = new cost_t[G.n];
		memset(pot, 0, sizeof(cost_t) * G.n);
		d = new cost_t[G.n];
		from = new int[G.n];
		total_cost = 0;
		total_flow = 0;
	}

	~MinCost() {
		delete[] pot;
		delete[] d;
		delete[] from;
	}


	bool dikstra() {
		typedef pair <cost_t, int> pr;
		priority_queue <pr, vector <pr>, greater <pr> > q;
		fill(d, d + G.n, cost_inf);
		d[G.s] = 0;
		q.emplace(d[G.s], G.s);
		while (q.size()) {
			int v = q.top().Y;
			cost_t s = q.top().X;
			q.pop();
			if (s != d[v])
				continue;
			for (int e: g[v]) {
				int to = es[e].to;
				if (es[e].f == es[e].c)
					continue;
				cost_t tos = es[e].cost + pot[v] - pot[to] + s;
				if (tos < d[to]) {
					d[to] = tos;
					from[to] = e;
					q.emplace(d[to], to);
				}
			}
		}
		if (d[G.t] == cost_inf)
			return false;
		for (int i = 0; i < G.n; ++i)
			if (d[i] != cost_inf)
				pot[i] += d[i];
		return true;
	}

	bool push() {
		if (!dikstra())
			return false;
		++total_flow;
		int v = G.t;
		while (v != G.s) {
			int e = from[v];
			total_cost += es[e].cost;
			++es[e].f;
			--es[e ^ 1].f;
			v = es[e].v;
		}
		return true;
	}

	cost_t mincost(flow_t flow_limit = flow_inf) {
		while (total_flow < flow_limit && push());
		return total_cost;
	}
};


//https://www.hackerearth.com/practice/algorithms/graphs/minimum-cost-maximum-flow/tutorial/
void test() {
//	freopen("t.in", "r", stdin);
	cost_t a[11][11];
	int n;
	cin >> n;
	int S = 0, T = 2 * n + 1;
	Network G(2 * n + 2, S, T);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j) {
			cin >> a[i][j];
			G.add_edge(i + 1, n + j + 1, 1, a[i][j]);
		}
	for (int i = 0; i < n; ++i) {
		G.add_edge(S, i + 1, 1, 0);
		G.add_edge(n + i + 1, T, 1, 0);
	}
	MinCost mc(G);
	cout << mc.mincost() << endl;
}


int main() {
	test();
}