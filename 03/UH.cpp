#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../template/graph/match.h"
using namespace std;
const int buff = 1e3;
char s[buff];
string read() {
  scanf("%s", s);
  return s;
}
int main() {
#ifndef LOCAL
  freopen("heavy.in", "r", stdin);
  freopen("heavy.out", "w", stdout);
#endif
  int n, k;
  scanf("%d%d", &n, &k);
  vector<string> arr(n);
  for (auto &v : arr) v = read();
  int cntp = 0, cnts = 0;
  map<string, int> pid, sid;
  vector<pair<int, int>> edge;
  for (auto v : arr) {
    string pre = v.substr(0, k);
    reverse(v.begin(), v.end());
    string suf = v.substr(0, k);
    if (!pid.count(pre)) pid[pre] = ++cntp;
    if (!sid.count(suf)) sid[suf] = ++cnts;
    edge.push_back({pid[pre], sid[suf]});
  }
  auto [ml, mr] = match::min_cover(edge);
  printf("%lu\n", ml.size() + mr.size());
  for (auto u : ml) {
    vector<int> arr;
    for (int i = 0; i < n; i++)
      if (edge[i].first == u) arr.push_back(i + 1);
    printf("%lu", arr.size());
    for (auto v : arr) printf(" %d", v);
    puts("");
  }
  for (auto u : mr) {
    vector<int> arr;
    for (int i = 0; i < n; i++)
      if (edge[i].second == u) arr.push_back(i + 1);
    printf("%lu", arr.size());
    for (auto v : arr) printf(" %d", v);
    puts("");
  }
  return 0;
}