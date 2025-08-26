// 문자열 관련
#include <bits/stdc++.h>
using namespace std;

string s;
// log^2 Suffix Array
vector<int> build_sa(string &s) {
  int n = s.size();
  vector<int> sa(n), r(n + 1), nr(n + 1);
  for (int i = 0; i < n; i++)
    sa[i] = i, r[i] = s[i];
  for (int d = 1; d < n; d <<= 1) {
    auto cmp = [&](int i, int j) {
      if (r[i] == r[j])
        return r[i + d] < r[j + d];
      return r[i] < r[j];
    };
    sort(sa.begin(), sa.end(), cmp);

    nr[sa[0]] = 1;
    for (int i = 1; i < n; i++)
      nr[sa[i]] = nr[sa[i - 1]] + cmp(sa[i - 1], sa[i]);
    r = nr;
  }
  return sa;
}
// LCP Array from Suffix Array
vector<int> build_lcp(vector<int> &sa, const string &s) {
  int n = s.size();
  vector<int> lcp(n), isa(n);
  for (int i = 0; i < n; i++)
    isa[sa[i]] = i;
  for (int k = 0, i = 0; i < n; i++)
    if (isa[i]) {
      for (int j = sa[isa[i] - 1]; s[i + k] == s[j + k]; k++)
        ;
      lcp[isa[i]] = (k ? k-- : 0);
    }
  return lcp;
}

// KMP
// KMP failure function
vector<int> kmp_table(const string &s) {
  int n = s.size();
  vector<int> pi(n);
  for (int i = 1, j = 0; i < n; i++) {
    while (j && s[i] != s[j])
      j = pi[j - 1];
    if (s[i] == s[j])
      j++;
    pi[i] = j;
  }
  return pi;
}

// KMP search
vector<int> kmp_search(const string &s, const string &p) {
  vector<int> pi = kmp_table(p);
  vector<int> res;
  for (int i = 0, j = 0; i < s.size(); i++) {
    while (j && s[i] != p[j])
      j = pi[j - 1];
    if (s[i] == p[j])
      j++;
    if (j == p.size()) {
      res.push_back(i - j + 1);
      j = pi[j - 1];
    }
  }
  return res;
}

int main() {
  cin.tie(0)->sync_with_stdio(0);
  cin >> s;
  auto sa = build_sa(s), lcp = build_lcp(sa, s);
  for (int &i : sa)
    cout << i + 1 << " ";
  cout << "\nx ";
  for (int i = 1; i < lcp.size(); i++)
    cout << lcp[i] << " ";
}
