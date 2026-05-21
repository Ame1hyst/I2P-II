#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "codeGen.h"

int sbcount = 0;
Symbol table[TBLSIZE];

void initTable(void) {
    strcpy(table[0].name, "x");
    table[0].val = 0;
    strcpy(table[1].name, "y");
    table[1].val = 0;
    strcpy(table[2].name, "z");
    table[2].val = 0;
    sbcount = 3;
}

int getval(char *str) {
    int i = 0;

    for (i = 0; i < sbcount; i++)
        if (strcmp(str, table[i].name) == 0)
            return table[i].val;

    if (sbcount >= TBLSIZE)
        error(RUNOUT);
    
    strcpy(table[sbcount].name, str);
    table[sbcount].val = 0;
    sbcount++;
    return 0;
}

// Dont auto assign
// int getval(char *str){
//     int i = 0;

//     for(i = 0; i < sbcount; i++){
//         if (strcmp(str, table[i].name) == 0)
//             return table[i].val;
//     }
//     error(NOTFOUND);
//     return 0;
// }

int setval(char *str, int val) {
    int i = 0;

    for (i = 0; i < sbcount; i++) {
        if (strcmp(str, table[i].name) == 0) {
            table[i].val = val;
            return val;
        }
    }

    if (sbcount >= TBLSIZE)
        error(RUNOUT);
    
    strcpy(table[sbcount].name, str);
    table[sbcount].val = val;
    sbcount++;
    return val;
}

BTNode *makeNode(TokenSet tok, const char *lexe) {
    BTNode *node = (BTNode*)malloc(sizeof(BTNode));
    strcpy(node->lexeme, lexe);
    node->data = tok;
    node->val = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void freeTree(BTNode *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// factor := INT
//         | ID
//         | INCDEC ID  (prefix ++ or --)
//         | LPAREN assign_expr RPAREN

// chains: statement -> assign_expr -> or_expr -> xor_expr -> and_expr -> expr -> expr -> term -> unary -> factor
BTNode *factor(void) {
    BTNode *retp = NULL, *left = NULL;
    // plain INT 
    if (match(INT)) {
        retp = makeNode(INT, getLexeme());
        advance();
    } 
    // plain ID -> variable
    else if (match(ID)) {
        retp = makeNode(ID, getLexeme());
        advance();
    }
        // x += expr case

    // ++x or --x case
    else if(match(INCDEC)){
        char op[3];
        strcpy(op, getLexeme());
        advance();

        // ++int case
        if(!match(ID)) error(NOTNUMID);

        // x = x +- 1
        left = makeNode(ID, getLexeme());
        advance();

        retp = makeNode(ASSIGN, "=");
        BTNode *addsub_op = makeNode(ADDSUB, op[0] == '+' ? "+" : "-");
        addsub_op->left = makeNode(ID, left->lexeme); //cpy
        addsub_op->right = makeNode(INT, "1");;

        retp->left = left;
        retp->right = addsub_op;
    }
    
    // () handle
    else if (match(LPAREN)) {
        advance();
        retp = assign_expr();
        if (match(RPAREN))
            advance();
        else
            error(MISPAREN);
    } else {
        error(NOTNUMID);
    }
    return retp;
}

// handle -(-x)
BTNode *unary_expr(void){
    if(match(ADDSUB)){
        BTNode *retp = makeNode(ADDSUB, getLexeme());
        retp->left = makeNode(INT, "0"); // -x -> 0 - x
        advance();
        retp->right = unary_expr();
        return retp;
    }
    return factor();

}


// term
BTNode *term(void) {
    BTNode *node = unary_expr();
    return term_tail(node);
}

// term_tail
BTNode *term_tail(BTNode *left) {
    BTNode *node = NULL;

    if (match(MULDIV)) {
        node = makeNode(MULDIV, getLexeme());
        advance();
        node->left = left;
        node->right = unary_expr();
        return term_tail(node);
    } else {
        return left;
    }
}

// expr
BTNode *expr(void) {
    BTNode *node = term();
    return expr_tail(node);
}

// expr_tail
BTNode *expr_tail(BTNode *left) {
    BTNode *node = NULL;

    if (match(ADDSUB)) {
        node = makeNode(ADDSUB, getLexeme());
        advance();
        node->left = left;
        node->right = term();
        return expr_tail(node);
    } else {
        return left;
    }
}

// assign_expr
BTNode *assign_expr(void) {
    BTNode *node = or_expr();
    return assign_expr_tail(node);
}

// assign_expr_tail
BTNode *assign_expr_tail(BTNode *left) {
    BTNode *node = NULL;

    if(match(ASSIGN)  && left->data == ID){
        BTNode *retp = makeNode(ASSIGN, getLexeme());
        advance();
        retp->left = left;
        retp->right = assign_expr();
        return retp;
    }
    // a += or_expr
    else if(match(ADDSUB_ASSIGN) && left->data == ID){
        char op[3];
        strcpy(op, getLexeme());
        advance();
        BTNode *addsub_op = makeNode(ADDSUB, op[0] == '+' ? "+" : "-");
        addsub_op->left = makeNode(ID, left->lexeme);
        addsub_op->right = assign_expr(); // x += y += 2 case
        BTNode *retp  = makeNode(ASSIGN, "=");
        retp->left = left;
        retp->right = addsub_op;
        return retp;
    }

    return left;
}

// or_expr
BTNode *or_expr(void) {
    BTNode *node = xor_expr();
    return or_expr_tail(node);
}

// or_expr_tail
BTNode *or_expr_tail(BTNode *left) {
    BTNode *node = NULL;

    if (match(OR)) {
        node = makeNode(OR, getLexeme());
        advance();
        node->left = left;
        node->right = xor_expr();
        return or_expr_tail(node);
    } else {
        return left;
    }
}


// xor_expr
BTNode *xor_expr(void) {
    BTNode *node = and_expr();
    return xor_expr_tail(node);
}

// xor_expr_tail
BTNode *xor_expr_tail(BTNode *left) {
    BTNode *node = NULL;

    if (match(XOR)) {
        node = makeNode(XOR, getLexeme());
        advance();
        node->left = left;
        node->right = and_expr();
        return xor_expr_tail(node);
    } else {
        return left;
    }
}

// and_expr
BTNode *and_expr(void) {
    BTNode *node = expr();
    return and_expr_tail(node);
}

// and_expr_tail
BTNode *and_expr_tail(BTNode *left) {
    BTNode *node = NULL;

    if (match(AND)) {
        node = makeNode(AND, getLexeme());
        advance();
        node->left = left;
        node->right = expr();
        return and_expr_tail(node);
    } else {
        return left;
    }
}


// statement := ENDFILE | END | expr END
void statement(void) {
    BTNode *retp = NULL;

    if (match(ENDFILE)) {
        genEpilogue();
        exit(0);
    } else if (match(END)) {
        fprintf(stderr, ">> ");    // stays on terminal
        advance();
    } else {
        retp = assign_expr(); // chain -> assign_expr
        if (match(END)) {
            evaluateTree(retp);
            // printf("%d\n", evaluateTree(retp));
            // printf("Prefix traversal: ");
            // printPrefix(retp);
            // printf("\n");
            freeTree(retp);
            fprintf(stderr, ">> ");    // stays on terminal
            advance();
        } else {
            genError();
            exit(0);
        }
    }
}

void err(ErrorType errorNum) {
    if (PRINTERR) {
        fprintf(stderr, "error: ");
        switch (errorNum) {
            case MISPAREN:
                fprintf(stderr, "mismatched parenthesis\n");
                break;
            case NOTNUMID:
                fprintf(stderr, "number or identifier expected\n");
                break;
            case NOTFOUND:
                fprintf(stderr, "variable not defined\n");
                break;
            case RUNOUT:
                fprintf(stderr, "out of memory\n");
                break;
            case NOTLVAL:
                fprintf(stderr, "lvalue required as an operand\n");
                break;
            case DIVZERO:
                fprintf(stderr, "divide by constant zero\n");
                break;
            case SYNTAXERR:
                fprintf(stderr, "syntax error\n");
                break;
            default:
                fprintf(stderr, "undefined error\n");
                break;
        }
    }
    genError();
    exit(0);
}
