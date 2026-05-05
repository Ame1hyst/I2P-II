#include <iostream>

typedef struct Node {
    struct Node *next;
} Node;

/* Returns the node where the loop begins.
   If there is no loop, returns the last node before the exit. */
Node *detectCycle(Node *head){
    Node *cur = head, *nnext = head;

    while(nnext != NULL && nnext->next !=NULL){
        cur = cur->next;
        nnext = nnext->next->next; // walk 2
        if(cur == nnext){
            cur = head;
            // cycle until find
            while (cur != nnext)
            {
                cur = cur->next;
                nnext = nnext->next;
            }
            return cur;
            
        }
    }

    // No loop
    cur = head;
    while (cur->next != NULL) cur = cur->next;
    return cur;
}
