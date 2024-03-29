## [GK](https://codeforces.com/gym/101630/problem/K)
ACM ICPC 2017–2018, NEERC – Northern Eurasia Finals, Problem K
Solved at 2022.11.29 (+22)

## [UH](https://codeforces.com/gym/100269/problem/H)
ACM ICPC 2013–2014, NEERC, Northern Subregional Contest, Problem H
Solved at 2023.2.12 (+4)

## [QH](https://codeforces.com/gym/101612/problem/H)
ICPC 2017–2018, NEERC, Northern Subregional Contest, Problem H

Solved at 2022.10.18 (+22)

题意：

给出一些有根树，要求将这些树连成一棵以 $1$ 为根的树，同时匹配数尽可能大。

题解：

首先，考虑每棵树内如何匹配。因为每棵树最多只能通过将根节点连到其他节点获得一次匹配，因此在树内部的匹配一定是越多越好。而因为根节点可以通过连到其他失配点上以获得一次匹配，因此当出现失配点时，在不影响匹配数的前提下根节点失配更优。可以用树形 dp 和启发式合并对每棵树求出最大匹配下失配点尽可能为根节点的失配点序列。

然后，统计所有树的答案。我们考虑每次贪心把每棵树的根失配点连到其他树的尽可能非根的失配点上。考虑维护一个失配点序列，其包含当前处理过的树的所有失配点。对于每棵树，若失配点序列非空且根节点失配，则将根节点连到失配点序列里，再将剩余的失配点加入序列中。为了保证得到最优解，将不包含根节点的失配序列排在包含根节点的失配序列前面，同时对于包含根节点的失配序列，将其按序列长度降序排序。这样可以保证贪心得到最大匹配。
