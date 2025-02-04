// 간단한 2-SAT 예시
#include <bits/stdc++.h>
using namespace std;

int N, M, a, b, cnt;
vector<vector<int>> fv, rv;
vector<int> dfn, visited, scc;
// 역정점
int inv(int n)
{
	return n > N ? n - N : n + N;
}
// dfs
void dfs(int n, int color, vector<vector<int>> &v)
{
	visited[n] = 1;
	for (int i : v[n])
		if (!visited[i])
			dfs(i, color, v);
	if (color)
		scc[n] = color;
	else
		dfn.push_back(n);
}

int main()
{
	ios::sync_with_stdio(0);
	cin.tie(0);
	cin >> N >> M;
	fv.resize(N * 2 + 1);
	rv = fv;
	visited.resize(N * 2 + 1);
	scc = visited;
	while (M--)
	{
		cin >> a >> b;
		a = a < 0 ? inv(-a) : a;
		b = b < 0 ? inv(-b) : b;

		fv[inv(a)].push_back(b);
		fv[inv(b)].push_back(a);
		rv[a].push_back(inv(b));
		rv[b].push_back(inv(a));
	}

	for (int i = 1; i <= N * 2; i++)
		if (!visited[i])
			dfs(i, 0, fv);
	visited.assign(N * 2 + 1, 0);
	while (dfn.size())
	{
		int cur = dfn.back();
		dfn.pop_back();
		if (!visited[cur])
			dfs(cur, ++cnt, rv);
	}

	for (int i = 1; i <= N; i++)
	{
		if (scc[i] == scc[inv(i)])
		{
			cout << 0;
			return 0;
		}
	}

	cout << 1;
}