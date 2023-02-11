#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <string>
#include <vector>

namespace maxflow {

typedef long long flow_t;
const flow_t inf_flow = 1e18;
const int inf_dep = 1e9;

struct FlowEdge {
  int from, to;
  flow_t cap, low = 0, flow = 0;
};

int num_node(const std::vector<FlowEdge> &edges) {
  int n = 0;
  for (const auto &e : edges) n = std::max({n, e.from, e.to});
  return n;
}

flow_t get_flow(const std::vector<FlowEdge> &edges, int s) {
  flow_t flow = 0;
  for (const auto &e : edges) {
    if (e.from == s) flow += e.flow;
  }
  return flow;
}

struct MaxFlow {
  struct Edge {
    int from, to;
    flow_t cap;
  };
  int n;
  std::vector<std::vector<int>> eid;
  std::vector<Edge> edge;
  void build(const std::vector<FlowEdge> &edges) {
    n = num_node(edges);
    eid.assign(n + 1, {});
    edge.clear();
    int num_edges = 0;
    for (const auto &e : edges) {
      eid[e.from].push_back(num_edges++);
      edge.push_back({e.from, e.to, e.cap - e.flow});
      eid[e.to].push_back(num_edges++);
      edge.push_back({e.to, e.from, e.flow});
    }
  }

  std::vector<int> dis;
  std::vector<int> cur;
  bool bfs(int s, int t) {
    if (s > n || t > n) return false;
    dis.assign(n + 1, inf_dep);
    cur.assign(n + 1, 0);
    std::queue<int> que;
    dis[s] = 0;
    que.push(s);
    while (que.size()) {
      int u = que.front();
      que.pop();
      for (auto i : eid[u]) {
        const auto &e = edge[i];
        if (e.cap && dis[e.to] > dis[u] + 1) {
          dis[e.to] = dis[u] + 1;
          que.push(e.to);
        }
      }
    }
    return dis[t] < inf_dep;
  }

  flow_t dfs(int s, int t, flow_t flim) {
    if (s == t) return flim;
    flow_t flow = 0;
    for (int &i = cur[s]; i < eid[s].size() && flow < flim; i++) {
      auto &e = edge[eid[s][i]];
      if (dis[e.to] == dis[s] + 1 && e.cap) {
        auto detf = dfs(e.to, t, std::min(flim - flow, e.cap));
        flow += detf;
        e.cap -= detf;
        edge[eid[s][i] ^ 1].cap += detf;
      }
      if (flow == flim) break;
    }
    return flow;
  }
  flow_t maxflow(int s, int t) {
    flow_t flow = 0;
    while (bfs(s, t)) {
      flow += dfs(s, t, inf_flow);
    }
    return flow;
  }

  std::vector<FlowEdge> to_edge() {
    std::vector<FlowEdge> edges;
    for (int i = 0; i < edge.size(); i += 2)
      edges.push_back({
          .from = edge[i].from,
          .to = edge[i].to,
          .cap = edge[i].cap + edge[i ^ 1].cap,
          .low = 0,
          .flow = edge[i ^ 1].cap,
      });
    return edges;
  }
};

struct Processor {
  std::vector<bool> neg;
  std::vector<flow_t> low;
  std::vector<flow_t> excess;
  void init(std::vector<FlowEdge> &edges) {
    int n = num_node(edges);
    neg.clear();
    neg.reserve(edges.size());
    low.clear();
    low.reserve(edges.size());
    excess.assign(n + 1, 0);
  }
  void rmv_low(std::vector<FlowEdge> &edges) {
    for (auto &e : edges) {
      low.push_back(e.low);
      if (e.flow >= e.low) {
        e.flow -= e.low;
      } else {
        excess[e.from] -= e.low - e.flow;
        excess[e.to] += e.low - e.flow;
        e.flow = 0;
      }
      e.cap -= e.low;
      e.low = 0;
    }
  }
  void add_low(std::vector<FlowEdge> &edges) {
    reverse(low.begin(), low.end());
    for (auto &e : edges) {
      e.low = low.back();
      e.flow += e.low;
      e.cap += e.low;
      low.pop_back();
    }
  }
};

bool excess_flow(std::vector<FlowEdge> &edges,
                 const std::vector<flow_t> &excess) {
  int n = num_node(edges), m = edges.size();
  for (int i = 1; i <= n; i++) {
    if (excess[i] > 0)
      edges.push_back({.from = n + 1, .to = i, .cap = excess[i]});
    if (excess[i] < 0)
      edges.push_back({.from = i, .to = n + 2, .cap = -excess[i]});
  }
  MaxFlow g;
  g.build(edges);
  g.maxflow(n + 1, n + 2);
  edges = g.to_edge();
  for (int i = m; i < edges.size(); i++)
    if (edges[i].flow != edges[i].cap) return false;
  edges.resize(m);
  return true;
}

std::optional<flow_t> feasible_flow(std::vector<FlowEdge> &edges, int s = 0,
                                    int t = 0) {
  if (s && t) edges.push_back({.from = t, .to = s, .cap = inf_flow});
  Processor p;
  p.init(edges);
  p.rmv_low(edges);
  if (!excess_flow(edges, p.excess)) return std::nullopt;
  if (s && t) edges.pop_back();
  p.add_low(edges);
  return get_flow(edges, s);
}

std::optional<flow_t> maximum_flow(std::vector<FlowEdge> &edges, int s, int t) {
  edges.push_back({.from = t, .to = s, .cap = inf_flow});
  Processor p;
  p.init(edges);
  p.rmv_low(edges);
  if (!excess_flow(edges, p.excess)) return std::nullopt;
  edges.pop_back();
  MaxFlow g;
  g.build(edges);
  g.maxflow(s, t);
  edges = g.to_edge();
  p.add_low(edges);
  return get_flow(edges, s);
}

std::optional<flow_t> minimum_flow(std::vector<FlowEdge> &edges, int s, int t) {
  edges.push_back({.from = t, .to = s, .cap = inf_flow});
  Processor p;
  p.init(edges);
  p.rmv_low(edges);
  if (!excess_flow(edges, p.excess)) return std::nullopt;
  edges.pop_back();
  MaxFlow g;
  Processor q;
  excess_flow(edges, q.excess);
  g.build(edges);
  g.maxflow(t, s);
  edges = g.to_edge();
  p.add_low(edges);
  return get_flow(edges, s);
}

}  // namespace maxflow
namespace match {

std::vector<std::pair<int, int>> max_match(
    const std::vector<std::pair<int, int>> &edges) {
  int n = 1, m = 1;
  for (auto [l, r] : edges) {
    n = std::max(n, l);
    m = std::max(m, r);
  }
  std::vector<maxflow::FlowEdge> fedge;
  int s = n + m + 1, t = n + m + 2;
  for (int i = 1; i <= n; i++) fedge.push_back({.from = s, .to = i, .cap = 1});
  for (int i = 1; i <= m; i++)
    fedge.push_back({.from = i + n, .to = t, .cap = 1});
  for (auto [l, r] : edges) fedge.push_back({.from = l, .to = r + n, .cap = 1});
  maxflow::maximum_flow(fedge, s, t);
  std::vector<std::pair<int, int>> res;
  for (auto e : fedge)
    if (e.from != s && e.to != t && e.flow == 1)
      res.push_back({e.from, e.to - n});
  return res;
}

void dfs(int u, std::vector<bool> &vl, std::vector<bool> &vr,
         std::vector<std::vector<int>> &ltr, std::vector<int> &rtl) {
  if (vl[u]) return;
  vl[u] = true;
  for (auto v : ltr[u]) {
    vr[v] = true;
    dfs(rtl[v], vl, vr, ltr, rtl);
  }
}

std::pair<std::vector<int>, std::vector<int>> min_cover(
    const std::vector<std::pair<int, int>> &edges) {
  int n = 1, m = 1;
  for (auto [l, r] : edges) {
    n = std::max(n, l);
    m = std::max(m, r);
  }
  auto match = max_match(edges);
  std::vector<std::vector<int>> ltr(n + 1);
  std::vector<int> rtl(m + 1);
  std::vector<bool> vis(n + 1);
  for (auto [l, r] : match) {
    rtl[r] = l;
    vis[l] = true;
  }
  for (auto [l, r] : edges) ltr[l].push_back(r);
  std::vector<bool> vl(n + 1), vr(m + 1);
  for (int i = 1; i <= n; i++)
    if (!vis[i]) dfs(i, vl, vr, ltr, rtl);
  std::pair<std::vector<int>, std::vector<int>> res;
  for (int i = 1; i <= n; i++)
    if (!vl[i]) res.first.push_back(i);
  for (int i = 1; i <= m; i++)
    if (vr[i]) res.second.push_back(i);
  return res;
}

}  // namespace match

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
  vector<bool> vis(n);
  for (auto u : ml) {
    vector<int> arr;
    for (int i = 0; i < n; i++)
      if (edge[i].first == u && !vis[i]) {
        arr.push_back(i + 1);
        vis[i] = true;
      }
    printf("%lu", arr.size());
    for (auto v : arr) printf(" %d", v);
    puts("");
  }
  for (auto u : mr) {
    vector<int> arr;
    for (int i = 0; i < n; i++)
      if (edge[i].second == u && !vis[i]) {
        arr.push_back(i + 1);
        vis[i] = true;
      }
    printf("%lu", arr.size());
    for (auto v : arr) printf(" %d", v);
    puts("");
  }
  return 0;
}