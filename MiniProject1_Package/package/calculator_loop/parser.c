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

// term      := factor term_tail
// term_tail := MULDIV factor term_tail | NiL
BTNode *term(void) {
    BTNode *retp = NULL, *left = NULL;

    retp = left = unary_expr();
    while (match(MULDIV)) {
        retp = makeNode(MULDIV, getLexeme());
        advance();
        retp->left = left;
        retp->right = unary_expr();
        left = retp;
    }
    return retp;
}

// expr      := term expr_tail
// expr_tail := ADDSUB term expr_tail | NiL
BTNode *expr(void) {
    BTNode *retp = NULL, *left = NULL;

    retp = left = term();
    while (match(ADDSUB)) {
        retp = makeNode(ADDSUB, getLexeme());
        advance();
        retp->left = left;
        retp->right = term();
        left = retp;
    }
    return retp;
}

BTNode *assign_expr(void){
    BTNode *left = or_expr();
    
    // a = b = c =.. ID
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

BTNode *or_expr(void){
    BTNode *retp = NULL, *left = NULL;
    retp = left = xor_expr();
    while (match(OR)) {
        retp = makeNode(OR, getLexeme());
        advance();
        retp->left = left;
        retp->right = xor_expr();
        left = retp;
    }
    return retp;
}

BTNode *xor_expr(void){
    BTNode *retp = NULL, *left = NULL;
    retp = left = and_expr();
    while (match(XOR)) {
        retp = makeNode(XOR, getLexeme());
        advance();
        retp->left = left;
        retp->right = and_expr();
        left = retp;
    }
    return retp;
}

BTNode *and_expr(void){
    BTNode *retp = NULL, *left = NULL;
    retp = left = expr();
    while (match(AND)) {
        retp = makeNode(AND, getLexeme());
        advance();
        retp->left = left;
        retp->right = expr();
        left = retp;
    }
    return retp;
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
