#include "14934.h"
#include <stdlib.h>

static Node *node_of[1000005];

void enter(int p, int c) {
    Node *node = malloc(sizeof(Node));
    node->id = c; 
    node->next = NULL; 
    node->prev = tails[p];

    if (tails[p]) 
        tails[p]->next = node;
    else           
        heads[p] = node;

    tails[p] = node;
    node_of[c] = node;
}

void merge(int p_src, int p_dest) {
    if (!heads[p_src]) return;

    if (tails[p_dest]) {
        tails[p_dest]->next = heads[p_src];
        heads[p_src]->prev  = tails[p_dest];
    } else {
        heads[p_dest] = heads[p_src];
    }

    tails[p_dest] = tails[p_src];
    heads[p_src] = tails[p_src] = NULL;
}

void split(int p_src, int c, int p_dest) {
    Node *node = node_of[c];

    heads[p_dest] = node;
    tails[p_dest] = tails[p_src];

    if (node->prev) {
        node->prev->next = NULL;
        tails[p_src]   = node->prev;
    } 
    else {
        heads[p_src] = tails[p_src] = NULL;
    }

    node->prev = NULL;
}

void reverse(int p) {
    Node *cur = heads[p], *tmp;

    while (cur) {
        tmp = cur->prev;
        cur->prev = cur->next;
        cur->next = tmp;
        cur = cur->prev;
    }

    tmp = heads[p];
    heads[p] = tails[p];
    tails[p] = tmp;
}

int check(int p, int c, int k) {
    Node *node = node_of[c];

    while (k-- && node->prev)
        node = node->prev;

    return node->id;
}