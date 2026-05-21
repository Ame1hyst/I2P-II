#ifndef __CODEGEN__
#define __CODEGEN__

#include "parser.h"

// Evaluate the syntax tree
extern int codeGen(BTNode *root);
extern int evaluateTree(BTNode *root);

// Print the syntax tree in prefix
extern void printPrefix(BTNode *root);

// for testcase 2
extern void genError(void);
extern void genEpilogue(void);



#endif // __CODEGEN__
