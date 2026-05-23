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
    ADDSUB_ASSIGN,
    INCDEC,
    OR, XOR, AND,
} TokenSet;

TokenSet getToken(void);
TokenSet curToken = UNKNOWN;
char lexeme[MAXLEN];

// Test if a token matches the current token
int match(TokenSet token);
// Get the next token
void advance(void);
// Get the lexeme of the current token
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

// Initialize the symbol table with builtin variables
void initTable(void);
// Get the value of a variable
int getval(char *str);
// Set the value of a variable
int setval(char *str, int val);
// Make a new node according to token type and lexeme
BTNode *makeNode(TokenSet tok, const char *lexe);
// Free the syntax tree
void freeTree(BTNode *root);
BTNode *factor(void);
BTNode *unary_expr(void);
BTNode *term(void);
BTNode *term_tail(BTNode *left);
BTNode *expr(void);
BTNode *expr_tail(BTNode *left);
BTNode *and_expr(void);
BTNode *and_expr_tail(BTNode *left);
BTNode *xor_expr(void);
BTNode *xor_expr_tail(BTNode *left);
BTNode *or_expr(void);
BTNode *or_expr_tail(BTNode *left);
BTNode *assign_expr(void);
void statement(void);
// Print error message and exit the program
void err(ErrorType errorNum);


// for codeGen
int isConstExpr(BTNode *root);
int evalConst(BTNode *root);
int getVarAddr(char *name);
// Evaluate the syntax tree
int evaluateTree(BTNode *root);
int codeGen(BTNode *root);

// End + error
void genPrologue(void);
void genError(void);
void genEpilogue(void);


// Print the syntax tree in prefix
void printPrefix(BTNode *root);


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
        while (isdigit(c) && i < MAXLEN) { // handle multi INT
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return INT;
    } else if (isalpha(c)|| c == '_'){
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while ((isdigit(c) || isalpha(c) || c == '_') && i < MAXLEN) { // handle multi ID
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return ID;        
    }
    
    else if (c == '+' || c == '-') {
        char first = c;
        char next = fgetc(stdin);

        if (first == next){
            lexeme[0] = first;
            lexeme[1] = first;
            lexeme[2] = '\0';
            return INCDEC;
        } else if (next == '='){
            lexeme[0] = first;
            lexeme[1] = next;
            lexeme[2] = '\0';
            return ADDSUB_ASSIGN;            
        }
        else{
            ungetc(next, stdin);
            lexeme[0] = c;
            lexeme[1] = '\0';
            return ADDSUB;
        }

    } else if (c == '*' || c == '/') {
        lexeme[0] = c;
        lexeme[1] = '\0';
        return MULDIV;
    } else if (c == '\n') {
        lexeme[0] = '\0';
        return END;
    } else if (c == '&') {
        strcpy(lexeme, "&");
        return AND;
    } else if (c == '|') {
        strcpy(lexeme, "|");
        return OR;
    } else if (c == '^') {
        strcpy(lexeme, "^");
        return XOR;
    } else if (c == '=') {
        strcpy(lexeme, "=");
        return ASSIGN;
    } else if (c == '(') {
        strcpy(lexeme, "(");
        return LPAREN;
    } else if (c == ')') {
        strcpy(lexeme, ")");
        return RPAREN;
    } else if (c == EOF) {
        return ENDFILE;
    } else {
        return UNKNOWN;
    }
}

void advance(void) {
    curToken = getToken();
}

int match(TokenSet token) {
    if (curToken == UNKNOWN)
        advance();
    return token == curToken;
}

char *getLexeme(void) {
    return lexeme;
}


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

// factor := INT | ADDSUB INT |
//		   	 ID  | ADDSUB ID  | 
//		   	 ID ASSIGN expr |
//		   	 LPAREN expr RPAREN |
//		   	 ADDSUB LPAREN expr RPAREN
BTNode *factor(void) {
    BTNode *retp = NULL, *left = NULL;

    if (match(INT)) {
        retp = makeNode(INT, getLexeme());
        advance();
    } else if (match(ID)) {
        retp = makeNode(ID, getLexeme());
        advance();
    } else if (match(LPAREN)) {
        advance();
        retp = assign_expr();
        if (match(RPAREN))
            advance();
        else
            error(MISPAREN);
    } else if(match(INCDEC)){ // x = x + 1
        char op[3];
        strcpy(op, getLexeme());
        advance();

        left = makeNode(ID, getLexeme());
        advance();
        BTNode *addsub_op = makeNode(ADDSUB, op[0] == '+'? "+": "-");
        addsub_op->left = makeNode(ID, left->lexeme);
        addsub_op->right = makeNode(INT, "1");

        retp = makeNode(ASSIGN, "=");
        retp->left = left;
        retp->right = addsub_op;
    }
    
    else {
        error(NOTNUMID);
    }
    return retp;
}

// unary_expr       := ADDSUB unary_expr | factor 
// right tree
BTNode *unary_expr(void){
    if(match(ADDSUB)){ // 0 - x -> -x
        BTNode *retp = makeNode(ADDSUB, getLexeme());
        advance();
        retp->left = makeNode(INT, "0");
        retp->right = unary_expr();
        return retp;
    }
    return factor();
}

// term := factor term_tail
BTNode *term(void) {
    BTNode *node = unary_expr();
    return term_tail(node);
}

// term_tail := MULDIV factor term_tail | NiL
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

// expr := term expr_tail
BTNode *expr(void) {
    BTNode *node = term();
    return expr_tail(node);
}

// expr_tail := ADDSUB term expr_tail | NiL
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

BTNode *and_expr(void) {
    BTNode *node = expr();
    return and_expr_tail(node);
}

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

BTNode *xor_expr(void) {
    BTNode *node = and_expr();
    return xor_expr_tail(node);
}

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

BTNode *or_expr(void) {
    BTNode *node = xor_expr();
    return or_expr_tail(node);
}

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

// assign_expr      := ID ASSIGN assign_expr | ID ADDSUB_ASSIGN assign_expr | or_expr
// right tree
BTNode *assign_expr(void){
    BTNode *left = or_expr();
    if(match(ASSIGN) && left->data == ID){
        BTNode *retp = makeNode(ASSIGN, "=");
        advance();
        retp->left = left;
        retp->right = assign_expr();
        return retp;
    }
    else if (match(ADDSUB_ASSIGN) && left->data == ID){ // x = x + assign_expr
        char op[3];
        strcpy(op, getLexeme());
        advance();

        BTNode *addsub_op = makeNode(ADDSUB, op[0] == '+'? "+": "-");
        addsub_op->left = makeNode(ID, left->lexeme);
        addsub_op->right = assign_expr();

        BTNode *retp = makeNode(ASSIGN, "=");
        retp->left = left;
        retp->right = addsub_op;
        return retp;
    }
    return left;
}
// statement := ENDFILE | END | expr END
void statement(void) {
    BTNode *retp = NULL;

    if (match(ENDFILE)) {
        genEpilogue();
        exit(0);
    } else if (match(END)) {
        fprintf(stderr, ">> ");
        advance();
    } else {
        retp = assign_expr();
        if (match(END)) {
            // printf("%d\n", evaluateTree(retp));
            // printf("Prefix traversal: ");
            // printPrefix(retp);
            // printf("\n");
            evaluateTree(retp);
            freeTree(retp);
            fprintf(stderr, ">> ");
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

// gen
void genError(void){
    printf("EXIT 1\n");
}

void genEpilogue(void){
    printf("MOV r0, [0]\n");
    printf("MOV r1, [4]\n");
    printf("MOV r2, [8]\n");
    printf("EXIT 0\n");
}

// helper
int getVarAddr(char *name){
    for(int i = 0; i < sbcount; i++){
        if(strcmp(table[i].name, name) == 0) return i*4;
    }
    return -1;
}

int isConstExpr(BTNode *root){
    if(root == NULL) return 1;
    if(root->data == ID) return 0;
    return isConstExpr(root->left) && isConstExpr(root->right);
}

int evalConst(BTNode *root){
    if(root == NULL) return 0;

    int left = evalConst(root->left);
    int right  = evalConst(root->right);
    switch (root->data)
    {
    case ASSIGN: return setval(root->left->lexeme, right);
    case INT: return atoi(root->lexeme);
    case ID: return getval(root->lexeme);
    
    case ADDSUB:
        if(root->lexeme[0] == '+') return left + right;
        else return left - right;
    
    case MULDIV:
        if(root->lexeme[0] == '/') return left / right;
        else return left * right;
    
    case XOR: return left ^ right;
    case AND: return left & right;
    case OR: return left | right;
    default: return 0;
    }
}

static int regCount = 0;
int codeGen(BTNode *root){
    if(root == NULL) return 0;
    int address, reg, rReg, lReg;
    
    switch (root->data)
    {
    case INT: 
        reg = regCount++;
        printf("MOV r%d, %s\n", reg, root->lexeme);
        return reg;

    case ID:
        address = getVarAddr(root->lexeme);
        if(address == -1) {genError(); exit(0);}
        reg = regCount++;
        printf("MOV r%d, [%d]\n", reg, address);
        return reg;
    
    case ASSIGN:
        if(getVarAddr(root->left->lexeme) == -1){
            strcpy(table[sbcount].name, root->left->lexeme);
            table[sbcount++].val = 0;
        }
        
        rReg = codeGen(root->right);
        address = getVarAddr(root->left->lexeme);
        printf("MOV [%d], r%d\n", address, rReg);
        return rReg;
    
    case ADDSUB:
        lReg = codeGen(root->left);
        rReg = codeGen(root->right);
        if(strcmp("+", root->lexeme) == 0) printf("ADD r%d, r%d\n", lReg, rReg);
        else printf("SUB r%d, r%d\n", lReg, rReg);
        regCount = lReg + 1;
        return lReg;
    
    case MULDIV:
        lReg = codeGen(root->left);
        rReg = codeGen(root->right);
        if(strcmp("*", root->lexeme) == 0) printf("MUL r%d, r%d\n", lReg, rReg);
        else {
            if(isConstExpr(root->right) && evalConst(root->right) == 0){
                genError();
                exit(0);
            }
            else printf("DIV r%d, r%d\n", lReg, rReg);
        }
        regCount = lReg + 1;
        return lReg;
    
    case XOR:
        lReg = codeGen(root->left);
        rReg = codeGen(root->right);
        printf("XOR r%d, r%d\n", lReg, rReg);
        regCount = lReg + 1;
        return lReg;
    
    case OR:
        lReg = codeGen(root->left);
        rReg = codeGen(root->right);
        printf("OR r%d, r%d\n", lReg, rReg);
        regCount = lReg + 1;
        return lReg;
    
    case AND:
        lReg = codeGen(root->left);
        rReg = codeGen(root->right);
        printf("AND r%d, r%d\n", lReg, rReg);
        regCount = lReg + 1;
        return lReg;
    
    default: return 0;
    }
}

int evaluateTree(BTNode *root) {
    regCount = 0;
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


/*============================================================================================
main
============================================================================================*/

// This package is a calculator
// It works like a Python interpretor
// Example:
// >> y = 2
// >> z = 2
// >> x = 3 * y + 4 / (2 * z)
// It will print the answer of every line
// You should turn it into an expression compiler
// And print the assembly code according to the input

// This is the grammar used in this package
// You can modify it according to the spec and the slide
// statement  :=  ENDFILE | END | expr END
// expr    	  :=  term expr_tail
// expr_tail  :=  ADDSUB term expr_tail | NiL
// term 	  :=  factor term_tail
// term_tail  :=  MULDIV factor term_tail| NiL
// factor	  :=  INT | ADDSUB INT |
//		   	      ID  | ADDSUB ID  |
//		   	      ID ASSIGN expr |
//		   	      LPAREN expr RPAREN |
//		   	      ADDSUB LPAREN expr RPAREN

int main() {
    initTable();
    fprintf(stderr, ">> ");
    while (1) {
        statement();
    }
    return 0;
}
