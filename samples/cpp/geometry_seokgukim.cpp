// 간단한 2차원 기하학
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
using ll = long long;
using pll = pair<ll, ll>;

// 선분 구조체
struct line {
  pll s, e;
};

// ccw
int ccw(pii a, pii b, pii c) {
  ll res = 1LL * a.first * b.second + 1LL * b.first * c.second +
           1LL * c.first * a.second;
  res -= 1LL * b.first * a.second + 1LL * c.first * b.second +
         1LL * a.first * c.second;
  if (res > 0)
    return 1;
  if (res)
    return -1;
  return 0;
}

// 2차원 유클리드 거리
ll dist(pii a, pii b) {
  return 1LL * (b.first - a.first) * (b.first - a.first) +
         1LL * (b.second - a.second) * (b.second - a.second);
}

// 2차원 선분 교차 여부
bool is_intersect(line a, line b) {
  int aa = ccw(a.s, a.e, b.s) * ccw(a.s, a.e, b.e),
      bb = ccw(b.s, b.e, a.s) * ccw(b.s, b.e, a.e);

  if (aa == 0 && bb == 0) {
    if (a.e.first < b.s.first || a.s.first > b.e.first)
      return 0;
    if (a.s.first == a.e.first) {
      if (max(a.s.second, a.e.second) < min(b.s.second, b.e.second) ||
          max(b.s.second, b.e.second) < min(a.s.second, a.e.second)) {
        return 0;
      }
    } else if (a.s.second == a.e.second) {
      if (max(a.s.first, a.e.first) < min(b.s.first, b.e.first) ||
          max(b.s.first, b.e.first) < min(a.s.first, a.e.first)) {
        return 0;
      }
    }
  }

  return aa <= 0 && bb <= 0;
}

// 2차원 볼록 껍질 (Graham Scan)
vector<pii> convex_hull(vector<pii> &v) {
  int mnx = 1e9, mny = 1e9;
  for (auto &i : v) {
    if (i.second < mny)
      mnx = i.first, mny = i.second, swap(v[0], i);
    else if (i.second == mny && i.first < mnx)
      mnx = i.first, swap(v[0], i);
  }

  auto cmp = [&](pii a, pii b) {
    int ccwd = ccw(v[0], a, b);
    if (ccwd)
      return ccwd > 0;
    return dist(v[0], a) < dist(v[0], b);
  };
  sort(v.begin() + 1, v.end(), cmp);

  vector<pii> res;
  for (auto &i : v) {
    while (res.size() >= 2 && ccw(res[res.size() - 2], res.back(), i) <= 0)
      res.pop_back();
    res.push_back(i);
  }

  return res;
}
