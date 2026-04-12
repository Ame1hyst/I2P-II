#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int value;
    struct Node* left;
    struct Node* right;
} Node;

Node* newNode(int value){
    Node* node = malloc(sizeof(Node));
    node->value = value;  
    node->left = NULL;
    node->right = NULL;

    return node;
}

Node* treeX(char s[], int *pt){
    if(s[*pt] != '(') return NULL;
    
    (*pt)++; // Skip '('
    int root = 0;
    while (s[*pt] >= '0' && s[*pt] <= '9') root = root * 10 + (s[(*pt)++] - '0');
    Node *node = newNode(root);
    (*pt)++; // Skip '/'
    node->left = treeX(s, pt);
    (*pt)++; // Skip '/'
    node->right = treeX(s, pt);
    (*pt)++; // Skip ')'
    
    return node;
}

Node* treeY(char s[], int *pt){
    if(!(s[*pt] <= '9' && s[*pt] >= '0')) return NULL;

    int root = 0;
    while (s[*pt] >= '0' && s[*pt] <= '9') root = root * 10 + (s[(*pt)++] - '0');

    
    Node *node = newNode(root);
    (*pt)++; 
    node->left = treeY(s, pt);
    (*pt)+=2;
    node->right = treeY(s, pt);
    (*pt)++;
    
    return node;
}

int compare(Node* a, Node* b){
    if(!a && !b) return 1;
    else if(!a || !b) return 0;
    return a->value == b->value && compare(a->left, b->left) && compare(a->right, b->right);
}

int main(void){
    char sx[1000000], sy[1000000];
    int n = 0;
    
    scanf("%d", &n);
    scanf("%s", sx);
    scanf("%s", sy);

    int p1 = 0, p2 = 0;
    Node* x = treeX(sx, &p1);
    Node* y = treeY(sy, &p2);

    char *ans = compare(x, y) ? "YES": "NO";

    printf("%s", ans);
}