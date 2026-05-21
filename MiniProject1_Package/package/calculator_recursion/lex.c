#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lex.h"

static TokenSet getToken(void);
static TokenSet curToken = UNKNOWN;
static char lexeme[MAXLEN];

TokenSet getToken(void)
{
    int i = 0;
    char c = '\0';

    while ((c = fgetc(stdin)) == ' ' || c == '\t');


    if (isdigit(c)) {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while (isdigit(c) && i < MAXLEN) { //handle multi num
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        ungetc(c, stdin); //put back
        lexeme[i] = '\0';
        return INT;
    } 
    // Handle ID -> Multi char int _
    else if(isalpha(c) || c == '_'){
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while ((isalpha(c) || isdigit(c) || c == '_') && i < MAXLEN)
        {
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return ID;     
    }
    // Handle ADDSUB INCDEC + ASSIGN
    else if (c == '+' || c == '-') {
        char first = c;
        char second = fgetc(stdin);
        if(first == second){
            lexeme[0] = first;
            lexeme[1] = first;
            lexeme[2] = '\0';
            return INCDEC;
        }
        else if(second == '='){
            lexeme[0] = first;
            lexeme[1] = '=';
            lexeme[2] = '\0';
            return ADDSUB_ASSIGN;           
        }
        else{
            ungetc(second, stdin);
            lexeme[0] = first;
            lexeme[1] = '\0';
            return ADDSUB;
        }    
    } 
    // Handle MULDIV
    else if (c == '*' || c == '/') {
        lexeme[0] = c;
        lexeme[1] = '\0';
        return MULDIV;
    } 
    // Handle ASSIGN
    else if (c == '=') {
        strcpy(lexeme, "=");
        return ASSIGN;
    } 
    // Handle ()
    else if (c == '(') {
        strcpy(lexeme, "(");
        return LPAREN;
    } 
    else if (c == ')') {
        strcpy(lexeme, ")");
        return RPAREN;
    } 
    // Handle AND XOR OR
    else if (c == '&'){
        strcpy(lexeme, "&");
        return AND;
    }
    else if (c == '^'){
        strcpy(lexeme, "^");
        return XOR;
    }
    else if (c == '|'){
        strcpy(lexeme, "|");
        return OR;
    }
    // Hanldle END + UNKNOW
    else if (c == '\n') {
        lexeme[0] = '\0';
        return END;
    } 
    else if (c == EOF) {
        return ENDFILE;
    } 
    else {
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
