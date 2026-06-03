#include <bits/stdc++.h>
using namespace std;

vector<int> spf;

vector<int> getDivisors(int x) {
    vector<int> divs;
    divs.push_back(1);
    int tmp = x;
    while (tmp > 1) {
        int p = spf[tmp], cnt = 0;
        while (tmp % p == 0) { tmp /= p; cnt++; }
        int sz = divs.size();
        int pk = 1;
        for (int i = 0; i < cnt; i++) {
            pk *= p;
            for (int j = 0; j < sz; j++)
                divs.push_back(divs[j] * pk);
        }
    }
    return divs;
}

int main() {
    int n;
    scanf("%d", &n);

    spf.resize(n + 1);
    for (int i = 0; i <= n; i++) spf[i] = i;
    for (int i = 2; (long long)i * i <= n; i++)
        if (spf[i] == i)
            for (int j = i * i; j <= n; j += i)
                if (spf[j] == j) spf[j] = i;

    vector<int> dist(n + 1, -1);
    queue<int> q;
    dist[n] = 0;
    q.push(n);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        vector<int> divs = getDivisors(u);
        for (int i = 0; i < (int)divs.size(); i++) {
            int d = divs[i];
            int nb[2] = {u - d, u + d};
            for (int k = 0; k < 2; k++) {
                int v = nb[k];
                if (v >= 1 && v <= n && dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
    }

    for (int i = 1; i <= n; i++)
        printf("%d ", dist[i]);
}