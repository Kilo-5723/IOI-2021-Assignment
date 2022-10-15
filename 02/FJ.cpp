#include <cstdio>
#include <iostream>
using namespace std;
typedef long long ll;
struct frac {
  ll a, b;
  frac(ll a = 0, ll b = 1) : a(a), b(b) {}
};
bool operator<(const frac &a, const frac &b) { return a.a * b.b < b.a * a.b; }
struct cplx {
  ll x, y;
};
struct seg {
  cplx l, r;
  int u;
};
struct cross {
  int u, v;
  frac x;
};
const cross nil = {-1, -1, {0, 1}};
bool isnil(const cross &a) { return !~a.u; }
cross inter(const seg &a, const seg &b) {}