#include "14574.h"
#include "stdlib.h"

typedef struct Node{
    const int v;
    struct Node *next;
}Node;
void solve(Node* head, int l, int r){
    Node* font = head;
    for(int i = 0; i<l-1; i++) font = font->next;
    

    Node* targetL = font->next;
    Node* back = targetL;
    for(int i =0; i<(r-l)+1; i++) back = back->next;

    //Reverse
    Node* prev = NULL; //cut node
    Node* cur = targetL; //keep track
    while (cur != back)
    {
        Node *nnext = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nnext;
    }

    //re-connect
    font->next = prev;
    targetL->next = back;
 
}
