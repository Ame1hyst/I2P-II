#include <stdio.h>
#include <stdlib.h>

#define MAXN  200005
#define LOG   18

typedef struct Edge{
    int to;
    struct Edge *next;
    
} Edge;

typedef struct{
    int depth;
    int ancestor[LOG];
} Node;

Edge *head[MAXN];
Node node[MAXN];

void add_edge(int nnext, int cur){
    Edge* edge = malloc(sizeof(Edge));
    edge->to = cur;
    edge->next = head[nnext];
    head[nnext] = edge; 
}

int visited[MAXN] = {0};
int queue[MAXN];
void bfs(){
    int front = 0, back = 0;

    queue[back++] = 1;
    visited[1] = 1;
    node[1].depth = 0;
    node[1].ancestor[0] = 1;

    while (front < back)
    {
        int cur = queue[front++];
        for(Edge *e = head[cur]; e != NULL; e = e->next){
            if(!visited[e->to]){
                node[e->to].depth = node[cur].depth + 1;
                node[e->to].ancestor[0] = cur;
                queue[back++] = e->to;

                visited[e->to] = 1;

            }
        }
    }
    
}

void listed_ancestor(int n){
    for (int i = 1; i < LOG; i++)
        for (int j = 1; j <= n; j++)
            node[j].ancestor[i] = node[node[j].ancestor[i-1]].ancestor[i-1];
}

int lca(int a, int b){
    // Make a always depper
    if(node[a].depth < node[b].depth){
        int temp = a;
        a = b;
        b = temp;
    }

    //make b depth same level as a jump 2**k
    int diff = node[a].depth - node[b].depth;
    for (int k = 0; k < LOG; k++){
        if ((diff >> k) & 1)
            a = node[a].ancestor[k];        
    }
    
    if(a == b) return a;

    for (int k = LOG - 1; k >= 0; k--)
        if (node[a].ancestor[k] != node[b].ancestor[k]) {
            a = node[a].ancestor[k];
            b = node[b].ancestor[k];
        }

    return node[a].ancestor[0];
    
    
}


int main(void){
    int n = 0, q = 0;
    scanf("%d %d", &n, &q);

    for (int i = 2; i <= n; i++) {
        int nnext;
        scanf("%d", &nnext);
        add_edge(nnext, i);
        add_edge(i, nnext);
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