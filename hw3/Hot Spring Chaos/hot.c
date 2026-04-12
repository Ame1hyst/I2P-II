#include <stdio.h>
#include <stdlib.h>

typedef struct _Node {
    int id, tolerance;
    struct _Node* next, *prev;
} Node;

Node* createList(int n){
    Node* head = NULL, * prev = NULL;
    for(int i = 0; i<n; i++){
        Node* node = malloc(sizeof(Node));
        node->id = i+1;
        scanf("%d", &node->tolerance);
        node->prev = prev;
        node->next = NULL;

        if(prev)  prev->next = node;
        else head = node;
        prev = node;
    }

    head->prev = prev;
    prev->next = head;

    return head;
}
void solve(Node* head, int N, int K){
    Node* cur = head;
    int people = N;
    while (people > 1)
    {   
        Node* target = cur;
        int step = (K>=0) ? (K-1)%people: ((-K)-1)%people;

        if(K>=0){
            for(int i = 0; i<step; i++){
                target = target->next;
            }
        }
        else{
            for(int i = 0; i<step; i++){
                target = target->prev;
            }
        }

        printf("%d ", target->id);
        target->next->prev = target->prev;
        target->prev->next = target->next;
        
        cur = (K >= 0) ? target->next:target->prev;
        K = target->tolerance;
        free(target);
        people--;

    }
    printf("\n%d\n", cur->id);
}
