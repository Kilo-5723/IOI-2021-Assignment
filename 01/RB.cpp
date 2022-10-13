// #define LOCAL

#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;
string gen(int n, int a, int b) {
  if ((a ^ b ^ n) & 1) return "";
  int k = a + b - n;
  if (k > 2 * n || k <= 0) return "";
  int x = a - k + 1, y = b - k + 1;
  if (x < 1 || y < 1) return "";
  string res = "";
  while (x--) res += "B";
  while (y--) res += "G";
  while (res.size() < n) res += "BG";
  return res;
}
string shift(string a) {
  if (!a.size()) return "";
  string c = {a[0]};
  int n = a.size() / 2;
  for (int i = 1; i <= n; i++) c = c + a[i + n] + a[i];
  return c;
}
string comb(string a, string b) {
  if (!a.size() || !b.size()) return "";
  string c;
  for (int i = 0; i < a.size(); i++) c = c + a[i] + b[i];
  return c;
}
string solve(int n, int a, int b) {
  if (n & 1) {
    if (a == n && b == 0) return string(n, 'B');
    if (a == 0 && b == n) return string(n, 'G');
    return shift(gen(n, a, b));
  } else {
    string res;
    int m = n >> 1;
    if (a == m && b == m) return comb(string(m, 'B'), string(m, 'G'));
    if (a == n && b == 0) return string(n, 'B');
    if (a == 0 && b == n) return string(n, 'G');
    res = gen(m, a - m, b);
    if (res.size()) return comb(string(m, 'B'), res);
    res = gen(m, a, b - m);
    if (res.size()) return comb(string(m, 'G'), res);
    for (int i = a / 2 - 2; i < a / 2 + 2; i++)
      for (int j = b / 2 - 2; j < b / 2 + 2; j++) {
        res = comb(gen(m, i, j), gen(m, a - i, b - j));
        if (res.size()) return res;
      }
    return "";
  }
}
int main() {
#ifndef LOCAL
  freopen("boysgirls.in", "r", stdin);
  freopen("boysgirls.out", "w", stdout);
#endif
  int n, a, b;
  cin >> n >> a >> b;
  string arr = solve(n, a, b);
  if (arr.size())
    cout << arr << endl;
  else
    cout << "Impossible" << endl;
  return 0;
}