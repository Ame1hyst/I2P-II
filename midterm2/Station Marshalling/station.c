#include "14934.h"
#include <stdlib.h>

static Node *node_of[1000005];

void enter(int p, int c) {
    Node *node = malloc(sizeof(Node));

    node->prev = tails[p];
    node->id = c;
    node->next = NULL;

    if(tails[p]) tails[p]->next = node;
    else heads[p] = node;

    tails[p] = node;
    node_of[c] = node;
}

void merge(int p_src, int p_dest) {
    if(heads[p_src] == NULL) return;

    if(tails[p_dest]){
        tails[p_dest]->next = heads[p_src];
        heads[p_src]->prev = tails[p_dest];
    }
    else{
        heads[p_dest] = heads[p_src];
    }
    tails[p_dest] = tails[p_src];
    heads[p_src] = NULL;
    tails[p_src] = NULL;
}

void split(int p_src, int c, int p_dest) {
    Node *target = node_of[c];

    heads[p_dest] = target;
    tails[p_dest] = tails[p_src];

    if(target->prev){
        target->prev->next = NULL;
        tails[p_src] = target->prev;
    }
    else{
        heads[p_src] = NULL;
        tails[p_src] = NULL;
    }

    target->prev = NULL;
}

void reverse(int p) {
    Node *cur = heads[p], *temp;

    while (cur)
    {
        temp = cur->prev;
        cur->prev = cur->next;
        cur->next = temp;
        cur = cur->prev;
    }
    
    temp = heads[p];
    heads[p] = tails[p];
    tails[p] = temp;
}

int check(int p, int c, int k) {
    Node *cur = node_of[c];

    while (k-- && cur->prev)
        cur = cur->prev;

    return cur->id;
}