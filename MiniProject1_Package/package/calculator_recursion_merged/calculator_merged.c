#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// for lex
#define MAXLEN 256

// Token types
typedef enum {
    UNKNOWN, END, ENDFILE,
    INT, ID,
    ADDSUB, MULDIV,
    ASSIGN,
    LPAREN, RPAREN,
    INCDEC,         // ++ or --
    ADDSUB_ASSIGN,  // += or -=
    AND,            // &
    OR,             // |
    XOR             // ^
} TokenSet;

TokenSet getToken(void);
TokenSet curToken = UNKNOWN;
char lexeme[MAXLEN];

int match(TokenSet token);
void advance(void);
char *getLexeme(void);


// for parser
#define TBLSIZE 256
// Set PRINTERR to 1 to print error message while calling error()
// Make sure you set PRINTERR to 0 before you submit your code
#define PRINTERR 0

// Call this macro to print error message and exit the program
// This will also print where you called it in your program
#define error(errorNum) { \
    if (PRINTERR) \
        fprintf(stderr, "error() called at %s:%d: ", __FILE__, __LINE__); \
    err(errorNum); \
}

// Error types
typedef enum {
    UNDEFINED, MISPAREN, NOTNUMID, NOTFOUND, RUNOUT, NOTLVAL, DIVZERO, SYNTAXERR
} ErrorType;

// Structure of the symbol table
typedef struct {
    int val;
    char name[MAXLEN];
} Symbol;

// Structure of a tree node
typedef struct _Node {
    TokenSet data;
    int val;
    char lexeme[MAXLEN];
    struct _Node *left;
    struct _Node *right;
} BTNode;

int sbcount = 0;
Symbol table[TBLSIZE];

void initTable(void);
int getval(char *str);
int setval(char *str, int val);
BTNode *makeNode(TokenSet tok, const char *lexe);
void freeTree(BTNode *root);

// forward declarations
BTNode *assign_expr(void);
BTNode *or_expr(void);
BTNode *xor_expr(void);
BTNode *and_expr(void);
BTNode *expr(void);
BTNode *expr_tail(BTNode *left);
BTNode *term(void);
BTNode *term_tail(BTNode *left);
BTNode *unary_expr(void);
BTNode *factor(void);
void statement(void);
void err(ErrorType errorNum);

// for codeGen
int  evaluateTree(BTNode *root);
void printPrefix(BTNode *root);
int  codeGen(BTNode *root);
void genEpilogue(void);
void genError(void);


/*============================================================================================
lex implementation
============================================================================================*/

TokenSet getToken(void)
{
    int i = 0;
    char c = '\0';

    while ((c = fgetc(stdin)) == ' ' || c == '\t');

    if (isdigit(c)) {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while (isdigit(c) && i < MAXLEN) {
            lexeme[i] = c; ++i; c = fgetc(stdin);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return INT;
    } else if (isalpha(c) || c == '_') {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while ((isalpha(c) || isdigit(c) || c == '_') && i < MAXLEN) {
            lexeme[i] = c; ++i; c = fgetc(stdin);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return ID;
    } else if (c == '+' || c == '-') {
        char first = c, second = fgetc(stdin);
        if (first == second) {
            lexeme[0] = first; lexeme[1] = first; lexeme[2] = '\0';
            return INCDEC;
        } else if (second == '=') {
            lexeme[0] = first; lexeme[1] = '='; lexeme[2] = '\0';
            return ADDSUB_ASSIGN;
        } else {
            ungetc(second, stdin);
            lexeme[0] = first; lexeme[1] = '\0';
            return ADDSUB;
        }
    } else if (c == '*' || c == '/') {
        lexeme[0] = c; lexeme[1] = '\0';
        return MULDIV;
    } else if (c == '=') { strcpy(lexeme, "=");  return ASSIGN; }
    else if (c == '(') { strcpy(lexeme, "(");  return LPAREN; }
    else if (c == ')') { strcpy(lexeme, ")");  return RPAREN; }
    else if (c == '&') { strcpy(lexeme, "&");  return AND; }
    else if (c == '^') { strcpy(lexeme, "^");  return XOR; }
    else if (c == '|') { strcpy(lexeme, "|");  return OR; }
    else if (c == '\n') { lexeme[0] = '\0';    return END; }
    else if (c == EOF)  { return ENDFILE; }
    else { return UNKNOWN; }
}

void advance(void) { curToken = getToken(); }
int match(TokenSet token) { if (curToken == UNKNOWN) advance(); return token == curToken; }
char *getLexeme(void) { return lexeme; }


/*============================================================================================
parser implementation
============================================================================================*/

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


/*============================================================================================
codeGen implementation
============================================================================================*/

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

int main() {
    initTable();
    fprintf(stderr, ">> ");
    while (1) {
        statement();
    }
    return 0;
}