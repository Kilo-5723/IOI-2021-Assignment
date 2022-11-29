#include <cstdio>
#include <iostream>
#include <map>
#include <optional>
#include <vector>
using namespace std;
typedef long long ll;
const int inf = 1e9 + 5;
const int maxn = 505, maxm = 55;
struct seg {
  ll x, y, d, k;
  int u;
  ll val(int x0) const { return y + (x0 - x) * k; }
};
struct cross {
  int x, u, d;
};
bool operator<(const cross &a, const cross &b) { return a.x < b.x; }
int sgn(ll n) { return (n > 0) - (n < 0); }
vector<cross> arr;
int flg;
void inter(seg a, seg b) {
  if (a.k > b.k) swap(a, b);
  ll x1 = max(a.x, b.x), x2 = min(a.x + a.d, b.x + b.d);
  if (x1 >= x2) return;
  int sgn1 = sgn(a.val(x1) - b.val(x1)), sgn2 = sgn(a.val(x2) - b.val(x2));
  if (sgn1 == sgn2) return;
  ll det = a.y - a.x * a.k - b.y + b.x * b.k;
  double x = (double)det / (b.k - a.k);
  int xl = x - 1e-10, xr = x + 1e-10;
  if (sgn2 && a.u == flg) arr.push_back({xr, a.u, -1});
  if (sgn1 && b.u == flg) arr.push_back({xl, b.u, 1});
}
vector<seg> segs[maxn];
int main() {
  int n, m;
  scanf("%d%d", &n, &m);
  for (int i = 0; i < n; i++) {
    map<int, int> cnt;
    for (int j = 0; j < m; j++) {
      int t;
      scanf("%d", &t);
      cnt[t]++;
    }
    ll k = m, las = 0, sum = 0;
    for (auto pr : cnt) {
      ll d = pr.first - las;
      segs[i].push_back({las, sum, d, k, i});
      sum += d * k;
      k -= pr.second;
      las = pr.first;
    }
    segs[i].push_back({las, sum, inf, 0, i});
  }
  for (int i = 0; i < n; i++) {
    arr.clear();
    for (auto u : segs[i]) {
      flg = i;
      for (int j = 0; j < n; j++)
        if (j != i)
          for (auto v : segs[j]) inter(u, v);
    }
    sort(arr.begin(), arr.end());
    int rnk = n, mnm = n;
    for (int l = 0, r = 0; l < arr.size(); l = r) {
      while (r < arr.size() && arr[r].x == arr[l].x) r++;
      for (int i = l; i < r; i++) {
        auto [x, u, d] = arr[i];
        rnk += d;
        mnm = min(mnm, rnk);
      }
    }
    printf("%d\n", mnm);
  }
  return 0;
}