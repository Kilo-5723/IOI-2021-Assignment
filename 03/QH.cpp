// #define LOCAL

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;
const int maxn = 1e5 + 5;
int p[maxn];
vector<int> e[maxn];
vector<vector<int>> arr;
void comb(vector<int> &a, vector<int> &b) {
  if (a.size() < b.size()) a.swap(b);
  // a.insert(a.end(), b.begin(), b.end());
  for (auto v : b) a.push_back(v);
}
vector<int> dp[maxn];
void dfs(int u) {
  // printf("%d\n", u);
  dp[u].clear();
  bool flg = true;
  for (auto v : e[u]) {
    // printf("%d %d\n", u, v);
    dfs(v);
    if (dp[v].size() && dp[v].back() == v && flg) {
      flg = false;
      dp[v].pop_back();
    }
    comb(dp[u], dp[v]);
  }
  // puts("a");
  if (flg) dp[u].push_back(u);
  // puts("b");
}
vector<int> unp;
int main() {
#ifndef LOCAL
  freopen("hidden.in", "r", stdin);
  freopen("hidden.out", "w", stdout);
#endif
  // puts("1");
  int n;
  scanf("%d", &n);
  // puts("1");
  for (int i = 2; i <= n; i++) {
    scanf("%d", &p[i]);
    if (p[i]) e[p[i]].push_back(i);
  }
  // puts("1");
  for (int i = 1; i <= n; i++)
    if (!p[i]) {
      dfs(i);
      // puts("fin");
      if (!dp[i].size()) continue;
      arr.push_back(dp[i]);
      // arr.back().swap(dp[i]);
    }
  // puts("1");
  p[1] = 1;
  sort(arr.begin(), arr.end(), [](vector<int> &a, vector<int> &b) {
    if ((p[b.back()] == 0) ^ (p[a.back()] == 0)) return p[b.back()] == 0;
    return a.size() > b.size();
  });
  for (auto &q : arr) {
    if (p[q.back()] == 0 && unp.size()) {
      p[q.back()] = unp.back();
      unp.pop_back();
      q.pop_back();
    }
    comb(unp, q);
  }
  for (int i = 2; i <= n; i++)
    if (!p[i]) p[i] = 1;
  printf("%d\n", (n - (int)unp.size()) / 2);
  for (int i = 2; i <= n; i++) printf("%d ", p[i]);
  puts("");
  return 0;
}
