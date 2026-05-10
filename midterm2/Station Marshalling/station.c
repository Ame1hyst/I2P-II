#include <stdlib.h>
typedef struct Node {
    int id;
    struct Node *prev;
    struct Node *next;
} Node;

extern Node* heads[100005];
extern Node* tails[100005];

Node *node_of[100005];

// 1. ENTER p c
void enter(int p, int c){
    Node *node = malloc(sizeof(Node));

    node->id = c;
    node->prev = tails[p];
    node->next = NULL;

    if (tails[p]) 
        tails[p]->next = node;
    else           
        heads[p] = node;

    tails[p] = node;
    node_of[c] = node;

}

// 2. MERGE p_src p_dest
void merge(int p_src, int p_dest){
    if(!heads[p_dest]) return;

    if(tails[p_dest]){
        
    }
}

// 3. SPLIT p_src c p_dest
void split(int p_src, int c, int p_dest);

// 4. REVERSE p
void reverse(int p);

// 5. CHECK p c k
int check(int p, int c, int k);
