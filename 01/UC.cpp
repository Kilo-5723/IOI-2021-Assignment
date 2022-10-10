#include <cstdio>
#include <iostream>
#include <map>
#include <vector>

using namespace std;
typedef long long ll;
const ll mod1 = 1e9 + 7, mod2 = 1e9 + 9, mul = 233;
const int maxn = 2e3 + 5;
ll pow1[maxn], ipw1[maxn], pow2[maxn], ipw2[maxn];
ll qpow(ll a, ll b, ll mod) {
  ll res = 1;
  while (b) {
    if (b & 1) res = res * a % mod;
    a = a * a % mod;
    b >>= 1;
  }
  return res;
}
struct hsh {
  ll hsh1, hsh2;
  int len;
};
bool operator<(hsh a, hsh b) {
  if (a.hsh1 != b.hsh1) return a.hsh1 < b.hsh1;
  if (a.hsh2 != b.hsh2) return a.hsh2 < b.hsh2;
  if (a.len != b.len) return a.len < b.len;
  return false;
}
bool operator==(hsh a, hsh b) { return !(a < b) && !(b < a); }
hsh operator+(hsh a, hsh b) {
  return {(a.hsh1 * pow1[b.len] % mod1 + b.hsh1) % mod1,
          (a.hsh2 * pow2[b.len] % mod2 + b.hsh2) % mod2, a.len + b.len};
}
hsh reml(hsh a, hsh b) {
  return {(a.hsh1 - b.hsh1 * pow1[a.len - b.len] % mod1 + mod1) % mod1,
          (a.hsh2 - b.hsh2 * pow2[a.len - b.len] % mod2 + mod2) % mod2,
          a.len - b.len};
}
char s[maxn], t[maxn];
ll hshs1[maxn], hshs2[maxn], hsht1[maxn], hsht2[maxn];
hsh get_hsh(ll hsh1[], ll hsh2[], int l, int r) {
  return reml({hsh1[r], hsh2[r], r}, {hsh1[l], hsh2[l], l});
}
hsh subs(const vector<int> &arr, int len1, int len2, int n) {
  hsh res = get_hsh(hshs1, hshs2, 0, arr[0]),
      add = get_hsh(hsht1, hsht2, arr[0], arr[0] + len2);
  for (int i = 0; i < arr.size(); i++) {
    res = res + add +
          get_hsh(hshs1, hshs2, arr[i] + len1,
                  i < arr.size() - 1 ? arr[i + 1] : n);
  }
  return res;
}
map<hsh, vector<int>> seq;
int main() {
  pow1[0] = pow2[0] = 1;
  for (int i = 1; i < maxn; i++) {
    pow1[i] = pow1[i - 1] * mul % mod1;
    pow2[i] = pow2[i - 1] * mul % mod2;
  }
  ipw1[maxn - 1] = qpow(pow1[maxn - 1], mod1 - 2, mod1);
  ipw2[maxn - 1] = qpow(pow2[maxn - 1], mod2 - 2, mod2);
  int n = 0, m = 0;
  for (char ch = getchar(); ch != '\n'; ch = getchar()) s[n++] = ch;
  for (char ch = getchar(); ch != '\n'; ch = getchar()) t[m++] = ch;
  for (int i = 0; i < n; i++) {
    hshs1[i + 1] = (hshs1[i] * mul + s[i]) % mod1;
    hshs2[i + 1] = (hshs2[i] * mul + s[i]) % mod2;
    hsht1[i + 1] = (hsht1[i] * mul + t[i]) % mod1;
    hsht2[i + 1] = (hsht2[i] * mul + t[i]) % mod2;
  }
  for (int i = 0; i < n; i++)
    for (int j = 1; i + j <= n; j++) {
      hsh tmp = get_hsh(hshs1, hshs2, i, i + j);
      if (!seq.count(tmp)) {
        seq[tmp] = {i};
      } else {
        auto &arr = seq[tmp];
        if (arr.back() + j <= i) arr.push_back(i);
      }
    }
  int l = 0, r1 = n, r2 = m;
  for (auto pr : seq) {
    int len = pr.first.len;
    auto arr = pr.second;
    if ((n - m) % arr.size()) continue;
    if (2 * len - (n - m) / arr.size() >= r1 + r2 - 2 * l) continue;
    if (subs(arr, len, len - (n - m) / arr.size(), n) ==
        get_hsh(hsht1, hsht2, 0, m)) {
      l = arr[0];
      r1 = len;
      r2 = len - (n - m) / arr.size();
    }
  }
  printf("s/");
  for (int i = l; i < r1; i++) putchar(s[i]);
  printf("/");
  for (int i = l; i < r2; i++) putchar(t[i]);
  puts("/g");
  return 0;
}