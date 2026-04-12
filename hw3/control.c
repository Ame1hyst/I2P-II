#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN  200005   // 2 * 10^5 + buffer
#define LOG   18       // 2^18 = 262144 > 200000

int head[MAXN], to[MAXN*2], nxt[MAXN*2], ecnt = 0;
int dep[MAXN];
int up[MAXN][LOG];
int n, q;

int bfs_queue[MAXN];
int visited[MAXN];

void add_nb(int u, int v) {
    ecnt++;
    to[ecnt] = v;
    nxt[ecnt] = head[u];
    head[u] = ecnt;
}

void bfs() {
    int front = 0, back = 0;
    bfs_queue[back++] = 1;
    visited[1] = 1;
    dep[1] = 0;
    up[1][0] = 1;

    while (front < back) {
        int u = bfs_queue[front++];
        for (int e = head[u]; e; e = nxt[e]) {
            int v = to[e];
            if (!visited[v]) {
                visited[v] = 1;
                dep[v] = dep[u] + 1;
                up[v][0] = u;
                bfs_queue[back++] = v;
            }
        }
    }
}

void build() {
    for (int k = 1; k < LOG; k++)
        for (int v = 1; v <= n; v++)
            up[v][k] = up[up[v][k-1]][k-1];
}

int lca(int u, int v) {
    if (dep[u] > dep[v]) { int tmp = u; u = v; v = tmp; }
    int diff = dep[v] - dep[u];

    for (int k = 0; k < LOG; k++)
        if ((diff >> k) & 1)
            v = up[v][k];

    if (u == v) return u;

    for (int k = LOG - 1; k >= 0; k--)
        if (up[u][k] != up[v][k]) {
            u = up[u][k];
            v = up[v][k];
        }

    return up[u][0];
}

int main() {
    scanf("%d %d", &n, &q);

    for (int i = 2; i <= n; i++) {
        int p;
        scanf("%d", &p);
        add_nb(p, i);
        add_nb(i, p);
    }

    bfs();
    build();

    while (q--) {
        int u, v;
        scanf("%d %d", &u, &v);
        printf("%d\n", lca(u, v));
    }
    return 0;
}