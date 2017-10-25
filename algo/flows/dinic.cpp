#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define mp make_pair
#define X first
#define Y second

typedef long long flow_t;
const flow_t flow_inf = 1e18;

struct edge
{
	int v, u;
	flow_t c, f;
	edge(int v, int u, flow_t c): v(v), u(u), c(c), f(0) {}
};

struct Network
{
	int n;
	int s, t;
	vector <edge> es;
	vector <vector <int> > g;
	Network(int n, int s, int t): n(n), s(s), t(t), g(n) {}

	void add_edge(int v, int u, flow_t c)
	{
		g[v].push_back(es.size());
		es.push_back(edge(v, u, c));
		g[u].push_back(es.size());
		es.push_back(edge(u, v, 0));
	}

	void add_flow(int e, flow_t f)
	{
		es[e].f += f;
		es[e ^ 1].f -= f;
	}
};

class DinicMaxFlow
{
public:
	Network& G;

	int *pos, *d, *q;

	DinicMaxFlow(Network& G): G(G)
	{
		pos = new int[G.n];
		q = new int[G.n];
		d = new int[G.n];
	}

	int bfs()
	{
		const int MXD = G.n + 1;
		fill(d, d + G.n, MXD);
		d[G.s] = 0;
		q[0] = G.s;
		int l = 0, r = 1;
		while (l < r)
		{
			int v = q[l++];
			for (int e: G.g[v])
			{
				if (G.es[e].c == G.es[e].f)
					continue;
				int u = G.es[e].u;
				if (d[u] == MXD)
				{
					d[u] = d[v] + 1;
					q[r++] = u; 
				}
			}
		}
		return d[G.t] < MXD;
	}

	flow_t dfs(int v, flow_t f)
	{
		if (v == G.t)
			return f;
		flow_t res = 0;
		for (int &i = pos[v]; i < G.g[v].size(); ++i)
		{
			int e = G.g[v][i];
			int u = G.es[e].u;
			flow_t delta = min(f, G.es[e].c - G.es[e].f);
			if (delta == 0 || d[u] != d[v] + 1)
				continue;
			delta = dfs(u, delta);
			f -= delta;
			res += delta;
			G.add_flow(e, delta);
			if (f == 0)
				break;
		}
		return res;
	}



	flow_t max_flow()
	{
		flow_t flow = 0;
		while (bfs())
		{
			fill(pos, pos + G.n, 0);
			while (flow_t plus = dfs(G.s, flow_inf))
				flow += plus;
		}
		return flow;
	}
};

void test()
{
//	freopen("t.in", "r", stdin);
	int n, m;
	cin >> n >> m;
	Network G(n, 0, n - 1);	
	for (int i = 0; i < m; ++i)
	{
		int v, u;
		flow_t c;
		cin >> v >> u >> c;
		--v;
		--u;
		G.add_edge(v, u, c);
	}
	DinicMaxFlow dinic(G);
	cout << dinic.max_flow() << endl;
}

int main()
{
	test();	    
    return 0;
}

//http://informatics.mccme.ru/mod/statements/view3.php?id=262&chapterid=2784#1