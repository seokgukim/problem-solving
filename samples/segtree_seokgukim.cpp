// 간단한 Segment Tree 예시
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll N, M, K, a, b, c, arr[1000001], tree[4000004];

void make_tree(int node, int s, int e)
{
    if (s == e)
    {
        tree[node] = arr[s];
        return;
    }
    int m = s + e >> 1;
    make_tree(node << 1, s, m);
    make_tree(node << 1 | 1, m + 1, e);
    tree[node] = tree[node << 1] + tree[node << 1 | 1];
}

void update(int node, int s, int e, int idx, ll val)
{
    if (s > idx || e < idx)
        return;
    if (s == e && s == idx)
    {
        tree[node] = val;
        return;
    }
    int m = s + e >> 1;
    update(node << 1, s, m, idx, val);
    update(node << 1 | 1, m + 1, e, idx, val);
    tree[node] = tree[node << 1] + tree[node << 1 | 1];
}

ll query(int node, int s, int e, int l, int r)
{
    if (s > r || e < l)
        return 0;
    if (l <= s && e <= r)
        return tree[node];
    int m = s + e >> 1;
    return query(node << 1, s, m, l, r) + query(node << 1 | 1, m + 1, e, l, r);
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> N >> M >> K;
    for (int i = 1; i <= N; i++)
        cin >> arr[i];
    make_tree(1, 1, N);

    while (M + K--)
    {
        cin >> a >> b >> c;
        if (a & 1)
            update(1, 1, N, b, c);
        else
            cout << query(1, 1, N, b, c) << "\n";
    }
}