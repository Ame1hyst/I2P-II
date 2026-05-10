#include <iostream>
using namespace std;

#define MAXN 1000005

typedef struct Edge
{
    int to;
    struct Edge *next;
    long long weigh;
} Edge;

typedef struct
{
    long long dist;
} Node;

Edge *head[MAXN];
Node node[MAXN];
int queue[MAXN];

void add_edge(int src, int dest, long long weigh){
    Edge *e = new Edge();
    e->to = dest;
    e->next = head[src];
    e->weigh = weigh;

    head[src] = e;
}

int bfs(int src, int n){
    for(int i = 0; i < n; i++) node[i].dist = -1;

    int front = 0, back = 0;
    queue[back++] = src;
    node[src].dist = 0;
    int farthest = src;

    while (front != back)
    {
        int cur = queue[front++];
        if(node[cur].dist > node[farthest].dist) farthest = cur;
        for(Edge *e = head[cur]; e != NULL; e = e->next){
            if(node[e->to].dist == -1){
                node[e->to].dist = node[cur].dist + e->weigh;
                queue[back++] = e->to;
            }
        }
    }
    return farthest;
}

int main(void){
    int n;
    cin >> n;

    long long total = 0;
    for(int i = 0; i < n; i++){
        int a, b;
        long long w;
        cin >> a >> b >> w;
        add_edge(a, b, w);
        add_edge(b, a, w);
    }

    int deepest = bfs(0, n);
    int farthest = bfs(deepest, n);



    return 0;
}

