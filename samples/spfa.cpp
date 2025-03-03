// shortest path faster algorithm by seokgukim
#include <vector>
#include <queue>
using namespace std;
// Shortest Path Faster Algorithm
// S는 시작 정점, edges에 간선 정보 [p, w]로 저장.
vector<int> spfa(int S, const vector<vector<pair<int, int>>> &edges) {
    // 반환할 거리 벡터 d와 큐 안의 삽입여부 in_q
    vector<int> d(edges.size(), 1e9 + 7), in_q(edges.size());
    queue<int> q;
    d[S] = 0;
    q.push(S);
    in_q[S] = 1;
    // 현재 정점으로부터 최단거리를 갱신해주며 큐가 빌 때까지 반복
    while (!q.empty()) {
        int u = q.front();
        in_q[u] = 0;
        q.pop();

        for (auto &[p, w] : edges[u]) {
            int nxt = d[u] + w;
            if (nxt < d[p]) {
                d[p] = nxt;
                if (!in_q[p]) {
                    q.push(p);
                    in_q[p] = 1;
                }
            }
        }
    }

    return d;
}