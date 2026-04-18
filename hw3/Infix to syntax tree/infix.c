#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "10966.h"
#define MAXEXPR 256
#define NUMSYM 6

extern char expr[MAXEXPR];  // string to store the input expression.
extern int pos;             // current position of parsing, from end to start

typedef enum {ID_A, ID_B, ID_C, ID_D, OP_AND, OP_OR} TokenSet;
extern char sym[NUMSYM];

typedef struct _Node {
    TokenSet data;
    struct _Node *left, *right;
} BTNode;


BTNode* FACTOR();
BTNode* makeNode(char c);

// EXPR = FACTOR | EXPR OP FACTOR
BTNode* EXPR(){
    BTNode *right = FACTOR();

    if(pos >= 0 && (expr[pos] == sym[OP_AND] || expr[pos] == sym[OP_OR])){
        char op = expr[pos];
        pos--;
        BTNode *left = EXPR();

        BTNode *opNode = makeNode(op);
        opNode->left = left;
        opNode->right = right;
        return opNode;
    }
    
    return right;
}

// FACTOR = ID | (EXPR)
BTNode* FACTOR(){
    BTNode *node;
    if(expr[pos] == ')'){
        pos--;
        node = EXPR();
        pos--;

    }
    else{
        node = makeNode(expr[pos]);
        pos--;
        
    }
    return node;
}

BTNode* makeNode(char c){
    BTNode *node = malloc(sizeof(BTNode));
    node->left = NULL;
    node->right = NULL;

    if(c == sym[ID_A]) node->data = ID_A;
    else if(c == sym[ID_B]) node->data = ID_B;
    else if(c == sym[ID_C]) node->data = ID_C;
    else if(c == sym[ID_D]) node->data = ID_D;
    else if(c == sym[OP_AND]) node->data = OP_AND;
    else if(c == sym[OP_OR]) node->data = OP_OR;

    return node;
}


// void freeTree(BTNode *root){
//     if (root != NULL){
//         freeTree(root->left);
//         freeTree(root->right);
//         free(root);

//     }
// }


// void printPrefix(BTNode *root){
//     if(root->data == '&' || root->data == '|') printf("%c", root->data);
//     else printf("%C", sym[root->data]);
// }