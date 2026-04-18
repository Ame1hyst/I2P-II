#include <stdio.h>
#include <stdlib.h>

#define MAXN 200005
#define LOG 18
typedef struct
{
    int depth;
    int ancestor[LOG];

} Node;

Node node[MAXN];

int get_depth(int num){
    if(num == 1) return 0;
    if(node[num].depth != -1) return node[num].depth;
    return node[num].depth = get_depth(node[num].ancestor[0]) + 1;
    
}

void listed_ancestor(int n){
    for(int i = 1; i<LOG; i++){
        for(int j = 1; j<=n; j++)
        node[j].ancestor[i] = node[node[j].ancestor[i-1]].ancestor[i-1];
    }
}

int lca(int a, int b){
    // Make a always deeper
    if(node[a].depth < node[b].depth){
        int temp = a;
        a = b;
        b = temp;
    }

    //Make dapth equal
    int diff = node[a].depth - node[b].depth;
    for(int i = 0; i<LOG; i++){
        if(diff>>i & 1) a = node[a].ancestor[i];
    }

    if(a == b) return a;
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

    node[1].depth = 0;
    node[1].ancestor[0] = 1;
    for(int i = 2; i<=n; i++) node[i].depth = -1;
    for(int i = 2; i<=n; i++){
        scanf("%d", &node[i].ancestor[0]);
        node[i].depth = get_depth(i);
    }

    listed_ancestor(n);
    while (q--)
    {
        int a = 0, b = 0;
        scanf("%d %d", &a, &b);
        printf("%d\n", lca(a, b));
    }
    


    return 0;
}