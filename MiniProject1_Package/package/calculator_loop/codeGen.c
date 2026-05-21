#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codeGen.h"

static int getVarAddr(char *name) {
    int i;
    for (i = 0; i < sbcount; i++)
        if (strcmp(name, table[i].name) == 0)
            return i * 4;
    return -1;
}

static int isConstExpr(BTNode *root) {
    if (root == NULL)     return 1;
    if (root->data == ID) return 0;
    return isConstExpr(root->left) && isConstExpr(root->right);
}

static int evalConst(BTNode *root) {
    if (root->data == INT) return atoi(root->lexeme);
    int lv = evalConst(root->left);
    int rv = evalConst(root->right);
    if (strcmp(root->lexeme, "+") == 0) return lv + rv;
    if (strcmp(root->lexeme, "-") == 0) return lv - rv;
    if (strcmp(root->lexeme, "*") == 0) return lv * rv;
    if (strcmp(root->lexeme, "/") == 0) return lv / rv;
    if (strcmp(root->lexeme, "&") == 0) return lv & rv;
    if (strcmp(root->lexeme, "|") == 0) return lv | rv;
    if (strcmp(root->lexeme, "^") == 0) return lv ^ rv;
    return 0;
}


void genEpilogue(void) {
    // load final x y z into r0 r1 r2 for the assembly parser to report
    printf("MOV r0, [0]\n");
    printf("MOV r1, [4]\n");
    printf("MOV r2, [8]\n");
    printf("EXIT 0\n");
}

void genError(void) {
    printf("EXIT 1\n");
}


static int regCount = 0;
int codeGen(BTNode *root) {
    if (root == NULL) return 0;
    int lReg, rReg, reg, addr;

    switch (root->data) {

        // INT: load constant into next free register
        case INT:
            reg = regCount++;
            printf("MOV r%d, %s\n", reg, root->lexeme);
            return reg;

        // ID: load variable FROM MEMORY into next free register
        case ID:
            addr = getVarAddr(root->lexeme);
            if (addr == -1) { genError(); exit(0); }
            reg = regCount++;
            printf("MOV r%d, [%d]\n", reg, addr);
            return reg;

        // ASSIGN: compute rhs, STORE TO MEMORY
        case ASSIGN:
            // auto-add new variable to table if first assignment
            if (getVarAddr(root->left->lexeme) == -1) {
                strcpy(table[sbcount].name, root->left->lexeme);
                table[sbcount].val = 0;
                sbcount++;
            }
            rReg = codeGen(root->right);
            addr = getVarAddr(root->left->lexeme);
            printf("MOV [%d], r%d\n", addr, rReg);
            return rReg;

        case ADDSUB:
            lReg = codeGen(root->left);
            rReg = codeGen(root->right);
            if (strcmp(root->lexeme, "+") == 0)
                printf("ADD r%d, r%d\n", lReg, rReg);
            else
                printf("SUB r%d, r%d\n", lReg, rReg);
            regCount = lReg + 1;    // ← free rReg and everything above
            return lReg;

        case MULDIV:
            lReg = codeGen(root->left);
            rReg = codeGen(root->right);
            if (strcmp(root->lexeme, "*") == 0) {
                printf("MUL r%d, r%d\n", lReg, rReg);
            } else {
                if (isConstExpr(root->right) && evalConst(root->right) == 0) {
                    genError(); exit(0);
                }
                printf("DIV r%d, r%d\n", lReg, rReg);
            }
            regCount = lReg + 1;    // ← free rReg and everything above
            return lReg;

        case AND:
            lReg = codeGen(root->left);
            rReg = codeGen(root->right);
            printf("AND r%d, r%d\n", lReg, rReg);
            regCount = lReg + 1;    // ← free rReg
            return lReg;

        case OR:
            lReg = codeGen(root->left);
            rReg = codeGen(root->right);
            printf("OR r%d, r%d\n", lReg, rReg);
            regCount = lReg + 1;    // ← free rReg
            return lReg;

        case XOR:
            lReg = codeGen(root->left);
            rReg = codeGen(root->right);
            printf("XOR r%d, r%d\n", lReg, rReg);
            regCount = lReg + 1;    // ← free rReg
            return lReg;


        default:
            return 0;
    }
}

int evaluateTree(BTNode *root) {
    regCount = 0;   // ← reset every expression
    codeGen(root);
    return 0;
}

void printPrefix(BTNode *root) {
    if (root != NULL) {
        printf("%s ", root->lexeme);
        printPrefix(root->left);
        printPrefix(root->right);
    }
}
