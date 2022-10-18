#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
const int maxn = 1e5 + 5, k = 10;
int tr[k][k][k][k][k][k][k][k];
int v[maxn][k], q[k];
char s[10];
inline int lowbit(int n) { return n & -n; }
void add(int a[], int v) {
  for (int i1 = a[0]; i1 < k; i1 += lowbit(i1))
    for (int i2 = a[1]; i2 < k; i2 += lowbit(i2))
      for (int i3 = a[2]; i3 < k; i3 += lowbit(i3))
        for (int i4 = a[3]; i4 < k; i4 += lowbit(i4))
          for (int i5 = a[4]; i5 < k; i5 += lowbit(i5))
            for (int i6 = a[5]; i6 < k; i6 += lowbit(i6))
              for (int i7 = a[6]; i7 < k; i7 += lowbit(i7))
                for (int i8 = a[7]; i8 < k; i8 += lowbit(i8))
                  tr[i1][i2][i3][i4][i5][i6][i7][i8] += v;
}
int query(int a[]) {
  int ans = 0;
  for (int i1 = a[0]; i1; i1 -= lowbit(i1))
    for (int i2 = a[1]; i2; i2 -= lowbit(i2))
      for (int i3 = a[2]; i3; i3 -= lowbit(i3))
        for (int i4 = a[3]; i4; i4 -= lowbit(i4))
          for (int i5 = a[4]; i5; i5 -= lowbit(i5))
            for (int i6 = a[5]; i6; i6 -= lowbit(i6))
              for (int i7 = a[6]; i7; i7 -= lowbit(i7))
                for (int i8 = a[7]; i8; i8 -= lowbit(i8))
                  ans += tr[i1][i2][i3][i4][i5][i6][i7][i8];
  return ans;
}
int main() {
  int n, t;
  scanf("%d%d", &n, &t);
  int m = 0;
  while (n--) {
    scanf("%s", s);
    switch (s[0]) {
      case 'C': {
        int r;
        scanf("%d", &r);
        fill(v[m], v[m + 1], 9);
        while (r--) {
          int t;
          scanf("%d", &t);
          v[m][t - 1]--;
        }
        add(v[m++], 1);
        break;
      }
      case 'D': {
        int u;
        scanf("%d", &u);
        add(v[u - 1], -1);
        break;
      }
      case 'J': {
        int r;
        scanf("%d", &r);
        fill(q, q + k, 9);
        while (r--) {
          int t;
          scanf("%d", &t);
          q[t - 1]--;
        }
        printf("%d\n", query(q));
        break;
      }
    }
  }
  return 0;
}