#include <bits/stdc++.h>

using namespace std;

#define pb push_back
typedef long double dbl;
const dbl eps = 1e-8;

int gauss(vector <vector <dbl> > a, vector <dbl>& x) {
	int n = a.size();
	int m = (int)a[0].size() - 1;
	vector <int> where(m, -1);
	for (int col = 0, row = 0; col < m && row < n; ++col) {
		int sel = row;
		for (int i = row; i < n; ++i)
			if (abs(a[i][col]) > abs(a[sel][col]))
				sel = i;
		if (abs(a[sel][col]) < eps)
			continue;
		swap(a[sel], a[row]);
		where[col] = row;
		for (int i = 0; i < n; ++i) {
			if (i != row) {
				dbl c = a[i][col] / a[row][col];
				for (int j = col; j <= m; ++j)
					a[i][j] -= a[row][j] * c;
			}
		}
		++row;
	}
	x.assign(m, 0);
	for (int col = 0; col < m; ++col)
		if (where[col] != -1)
			x[col] = a[where[col]][m] / a[where[col]][col];
	for (int i = 0; i < n; ++i) {
		dbl sum = 0;
		for (int j = 0; j < m; ++j)
			sum += a[i][j] * x[j];
		if (abs(sum - a[i][m]) > eps)
			return 0;
	}
	for (int j = 0; j < m; ++j)
		if (where[j] == -1)
			return 1e9;
	return 1;
}

void test() {
	freopen("t.in", "r", stdin);
	int n, m;
	vector <vector <dbl> > a;
	cin >> n >> m;
	a.resize(n);
	for (int i = 0; i < n; ++i) {
		a[i].resize(m + 1);
		for (int j = 0; j < m + 1; ++j) {
			cin >> a[i][j];
		}
	}
	vector <dbl> x;
	gauss(a, x);
	cout << fixed << setprecision(2);
	for (int i = 0; i < m; ++i)
		cout << x[i] << endl;

}

int main() {
	test();
}