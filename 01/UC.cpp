// #define LOCAL

#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;
typedef long long ll;
const ll mod = (ll)1e9 + 7;
const int mul = 233;
const int maxn = 2e3 + 5;
ll _pow[maxn];
struct str {
  ll hsh;
  int len;
};
bool operator<(const str &a, const str &b) {
  if (a.hsh != b.hsh) return a.hsh < b.hsh;
  if (a.len != b.len) return a.len < b.len;
  return false;
}
bool operator==(const str &a, const str &b) { return !(a < b) && !(b < a); }
str operator+(const str &a, const str &b) {
  return {(a.hsh * _pow[b.len] % mod + b.hsh) % mod, a.len + b.len};
}
str remove_l(const str &a, const str &b) {
  return {(a.hsh - b.hsh * _pow[a.len - b.len] % mod + mod) % mod,
          a.len - b.len};
}
char s[maxn], t[maxn];
str hs[maxn], ht[maxn];
str get_hsh(str hs[], int l, int r) { return remove_l(hs[r], hs[l]); }
str replace(const vector<int> &arr, int len1, int len2, int n) {
  str res = get_hsh(hs, 0, arr[0]), add = get_hsh(ht, arr[0], arr[0] + len2);
  for (int i = 0; i < arr.size(); i++) {
    res = res + add +
          get_hsh(hs, arr[i] + len1, i < arr.size() - 1 ? arr[i + 1] : n);
  }
  return res;
}
unordered_map<ll, int> id;
vector<vector<int>> seq;
int main() {
#ifndef LOCAL
  freopen("curiosity.in", "r", stdin);
  freopen("curiosity.out", "w", stdout);
#endif
  _pow[0] = 1;
  for (int j = 1; j < maxn; j++) _pow[j] = _pow[j - 1] * mul % mod;
  int n = 0, m = 0;
  for (char ch = getchar(); ch != '\n'; ch = getchar()) s[n++] = ch;
  for (char ch = getchar(); ch != '\n'; ch = getchar()) t[m++] = ch;
  for (int i = 0; i < n; i++) hs[i + 1] = hs[i] + str{s[i], 1};
  for (int i = 0; i < m; i++) ht[i + 1] = ht[i] + str{t[i], 1};
  int l = 0, r1 = n, r2 = m;
  for (int j = 1; j <= n; j++) {
    for (int i = 0; i + j <= n; i++) {
      str tmp = get_hsh(hs, i, i + j);
      if (!id.count(tmp.hsh)) {
        id[tmp.hsh] = seq.size();
        seq.push_back({i});
      } else {
        int k = id[tmp.hsh];
        if (seq[k].back() + j <= i) seq[k].push_back(i);
      }
    }
    for (auto &arr : seq) {
      int k = arr.size();
      if ((n - m) % k) continue;
      int len1 = j;
      int len2 = len1 - (n - m) / k;
      if (len2 < 0 || len1 + len2 >= r1 + r2 - 2 * l) continue;
      if (replace(arr, len1, len2, n) == get_hsh(ht, 0, m)) {
        l = arr[0];
        r1 = l + len1;
        r2 = l + len2;
      }
    }
    id.clear();
    seq.clear();
  }
  printf("s/");
  for (int i = l; i < r1; i++) putchar(s[i]);
  printf("/");
  for (int i = l; i < r2; i++) putchar(t[i]);
  puts("/g");
  return 0;
}