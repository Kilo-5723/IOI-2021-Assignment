#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 li;
li exgcd(li a, li b, li &x, li &y) {
  if (!b) {
    x = 1;
    y = 0;
    return a;
  }
  li res = exgcd(b, a % b, y, x);
  y -= a / b * x;
  return res;
}
li inv(li n, li mod) {
  li x, y;
  exgcd(n, mod, x, y);
  return (x % mod + mod) % mod;
}
void solve1(int n, vector<ull> b, ull s) {
  int m = n / 2;
  vector<pair<ull, int>> a;
  for (int i = 0; i < (1 << m); i++) {
    ull t = 0;
    for (int j = 0; j < m; j++)
      if (i >> j & 1) t += b[j];
    a.push_back({t, i});
  }
  sort(a.begin(), a.end());
  for (int i = 0; i < (1 << n - m); i++) {
    ull t = 0;
    for (int j = 0; j < n - m; j++)
      if (i >> j & 1) t += b[m + j];
    int p =
        lower_bound(a.begin(), a.end(), pair<ull, int>{s - t, 0}) - a.begin();
    if (p != a.size() && a[p].first == s - t) {
      int k = a[p].second;
      for (int j = 0; j < m; j++) putchar('0' + (k >> j & 1));
      for (int j = 0; j < n - m; j++) putchar('0' + (i >> j & 1));
      puts("");
      return;
    }
  }
}
ull lowbit(ull n) { return n & -n; }
void solve2(int n, vector<ull> b, ull s) {
  ull k = lowbit(b[0]);
  ull r0 = inv(b[0] / k, ((li)1 << 64) / k);
  for (int i = 0; i < k; i++) {
    ull r = r0 + i * ((li)1 << 64) / k;
    for (int i = k; i < (1 << 64 - n + 3); i += 2 * k) {
      if (b[0] * r * (i / k) != i) continue;
      vector<ull> a(n);
      for (int j = 0; j < n; j++) a[j] = b[j] * r * (i / k);
      li sum = 0;
      bool flg = true;
      for (auto v : a)
        if (v <= sum) {
          flg = false;
          break;
        } else {
          sum += (li)v;
        }
      if (sum >> 64) continue;
      if (!flg) continue;
      string str;
      reverse(a.begin(), a.end());
      ull t = s * r * (i / k);
      for (auto v : a)
        if (t >= v) {
          t -= v;
          str += '1';
        } else {
          str += '0';
        }
      if (t) continue;
      reverse(str.begin(), str.end());
      printf("%s\n", str.c_str());
      return;
    }
  }
}
int main() {
  int n;
  scanf("%d", &n);
  vector<ull> b(n);
  for (int i = 0; i < n; i++) scanf("%llu", &b[i]);
  ull s;
  scanf("%llu", &s);
  if (n <= 40)
    solve1(n, b, s);
  else
    solve2(n, b, s);
  return 0;
}