#include <stdio.h>
#include <stdlib.h>

#define MAXN 200005
#define LOG 18

typedef struct{
    int depth;
    int ancestor[LOG];

} Node;

Node nodes[MAXN];

int get_depth(int num){
    if(num == 1) return 0;
    if(nodes[num].depth != -1) return nodes[num].depth;
    return nodes[num].depth = get_depth(nodes[num].ancestor[0]) + 1;
}

void listed_ancestor(int n){
    for(int i = 1; i<LOG; i++){
        for(int j = 1; j<=n; j++){
            nodes[j].ancestor[i] = nodes[nodes[j].ancestor[i-1]].ancestor[i-1];
        }
    }
}

int lca(int a, int b){
    // A depper
    if(nodes[a].depth < nodes[b].depth){
        int temp = a;
        a = b;
        b = temp;
    }

    //Move depth a
    int diff = nodes[a].depth - nodes[b].depth;
    for(int i = 0; i<LOG; i++){
        if ((diff >> i) & 1) a = nodes[a].ancestor[i];
    }

    if(a == b) return a;
    //Jump to top first then go down
    for(int i = LOG - 1; i>=0; i--){
        if(nodes[a].ancestor[i] != nodes[b].ancestor[i]){
            a = nodes[a].ancestor[i];
            b = nodes[b].ancestor[i];
        }
    }
    return nodes[a].ancestor[0];
    
}


int main(void){

    int n = 0, q = 0;
    scanf("%d %d", &n, &q);

    
    nodes[1].depth = 0;
    nodes[1].ancestor[0] = 1;
    for(int i = 2; i<=n; i++) nodes[i].depth = -1;
    for(int i = 2; i<=n; i++){
        int ances = 0;
        scanf("%d", &ances);
        nodes[i].ancestor[0] = ances;
        nodes[i].depth = get_depth(i);

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