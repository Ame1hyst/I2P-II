#include <stdio.h>
#include <stdlib.h>

#define MAXN 200005
#define LOG 18

typedef struct
{
    int depth;
    int ancestor[LOG];
} Node;

typedef struct Edge
{
    int to;
    struct Edge *next;
} Edge;

Edge *adj[MAXN];
Node node[MAXN];

void add_edge(int next, int to){
    Edge *edge = malloc(sizeof(Edge));
    edge->to = to;
    edge->next = adj[next];
    adj[next] = edge;

}

void free_edge(int n){
    for(int i = 1; i<=n; i++){
        Edge *edge = adj[i];
        while(edge != NULL){
            Edge *temp = edge->next;
            free(edge);
            edge = temp;
        }
    }
}

int queue[MAXN];
int visited[MAXN] = {0};
void bfs(){
    int front = 0, back = 0;
    queue[back++] = 1;
    visited[1] = 1;
    node[1].depth = 0;
    node[1].ancestor[0] = 1;

    while (front != back)
    {
        int cur = queue[front++];
        for(Edge *e = adj[cur]; e != NULL; e = e->next){
            int target = e->to;
            if(!visited[target]){
                visited[target] = 1;

                node[target].depth = node[cur].depth + 1;
                node[target].ancestor[0] = cur;
                queue[back++] = target;
            }
        }
    }
    
}

void listed_ancestor(int n){
    for(int i = 1; i<LOG; i++){
        for(int j = 1; j<=n; j++){
            node[j].ancestor[i] = node[node[j].ancestor[i-1]].ancestor[i-1];
        }
    }
}

int lca(int a, int b){
    if(node[a].depth < node[b].depth){
        int temp = a;
        a = b;
        b = temp;
    }

    int diff = node[a].depth - node[b].depth;
    for(int i = 0; i<LOG; i++){
        if(diff>>i & 1) a = node[a].ancestor[i];
    }

    if( a == b) return a;

    for(int i = LOG - 1; i>=0; i--){
        if(node[a].ancestor[i] != node[b].ancestor[i]){
            a = node[a].ancestor[i];
            b = node[b].ancestor[i];
        }
    }

    return node[a].ancestor[0];
}

int main(void){
    int n = 0, q = 0;
    scanf("%d %d", &n, &q);

    for(int i = 2; i<=n; i++){
        int p = 0;
        scanf("%d", &p);

        add_edge(p, i);
        add_edge(i, p);
    }

    bfs();
    listed_ancestor(n);
    while (q--)
    {
        int a = 0, b = 0;
        scanf("%d %d", &a, &b);
        printf("%d\n", lca(a, b));
    }
    

    return 0;
}