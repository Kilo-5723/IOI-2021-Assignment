#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;
typedef vector<vector<int>> mtx;
const int maxn = 305;
char s[maxn];
mtx init(int n, int m) {
  mtx a(n);
  for (auto &r : a) r.resize(m);
  return a;
}
void diagflip(mtx &a) {
  int n = a.size(), m = a[0].size();
  mtx b = init(m, n);
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) b[j][i] = a[i][j];
  a = b;
}
void horflip(mtx &a) { reverse(a.begin(), a.end()); }
void strip(mtx &a) {
  while (a.size()) {
    for (auto v : a.back())
      if (v) return;
    a.pop_back();
  }
}
mtx seek(mtx &a) {
  mtx b = init(a.size() - 2, a[0].size() - 2);
  for (int i = 0; i < b.size(); i++)
    for (int j = 0; j < b[0].size(); j++) {
      b[i][j] = a[i][j];
      for (int di = 0; di <= 2; di++)
        for (int dj = 0; dj <= 2; dj++) a[i + di][j + dj] ^= b[i][j];
    }
  return b;
}
int finderr(mtx a) {
  for (int i = 0; i < a.size(); i++) {
    int s[3];
    memset(s, 0, sizeof(s));
    for (int j = 0; j < a[i].size(); j++) s[j % 3] ^= a[i][j];
    for (int j = 1; j < 3; j++)
      if (s[j] != s[0]) return i;
  }
  return -1;
}
mtx shrink(mtx a) {
  int n = a.size(), m = a[0].size();
  if (n <= 2 || m <= 2) return {};
  int x = finderr(a);
  diagflip(a);
  int y = finderr(a);
  diagflip(a);
  if (~x) a[x][y] ^= 1;
  mtx b = seek(a);
  for (auto &r : a)
    for (auto v : r)
      if (v) return {};
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      strip(b);
      horflip(b);
    }
    diagflip(b);
  }
  return b;
}
void write(mtx a) {
  for (auto &r : a) {
    for (auto v : r) printf(v ? "#" : ".");
    puts("");
  }
}
int main() {
  int n, m;
  scanf("%d%d", &m, &n);
  mtx a = init(n, m);
  for (int i = 0; i < n; i++) {
    scanf("%s", s);
    for (int j = 0; j < m; j++) a[i][j] = (s[j] == '#');
  }
  while (true) {
    auto b = shrink(a);
    if (!b.size()) break;
    a = b;
  }
  write(a);
  return 0;
}