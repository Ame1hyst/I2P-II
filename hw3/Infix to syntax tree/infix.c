#include <stdlib.h>
#include "10966.h"


BTNode* FACTOR();
BTNode* makeNode(char c);

// EXPR = FACTOR | EXPR OP FACTOR
BTNode* EXPR(){
    BTNode *right = FACTOR();
    if(pos >=0 && (expr[pos] == sym[OP_AND] || expr[pos] == sym[OP_OR])){
        BTNode *opNode = makeNode(expr[pos]);
        pos--;
        
        BTNode *left = EXPR();

        opNode->left = left;
        opNode->right = right;
        return opNode;
    }

    return right;
}

// FACTOR = ID | (EXPR)
BTNode* FACTOR(){
    BTNode *node;
    if(expr[pos] != ')') {node = makeNode(expr[pos]); pos--;}
    else {
        pos--; //skip )
        node = EXPR();
        pos--; //skip (
        
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
