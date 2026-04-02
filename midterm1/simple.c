#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void hexchange(char *input, char *output){
    output[0] = '\0';
    char *token = strtok(input, "|");
    char change[50];
    while(token != NULL){
        sprintf(change, "%X", atoi(token));
        strcat(output, change);
        token = strtok(NULL, "|");
    }
}

void Insert(int index, char *add, char *base){
    int len_base = strlen(base);
    int len_add = strlen(add);
    memmove(base + index + len_add, base+index, len_base - index + 1);
    memcpy(base + index, add, len_add);
}

void Remove(int index, int amount, char *base){
    memmove(base + index, base + index + amount, strlen(base) - index - amount +1);
}



int main(void){
    char s[10001] = "";
    scanf("%s", s);

    char shex[20001] = "";
    hexchange(s, shex);

    int q = 0;
    scanf("%d", &q);

    for(int i =0; i<q; i++){
        char option[10];
        scanf("%s", option);

        if (strcmp(option, "Insert") == 0){
            int index;
            char str[10001];
            char add[20001];
            scanf("%d %s", &index, str);

            hexchange(str, add);
            Insert(index, add, shex);
    
        }

        else if(strcmp(option, "Remove") == 0){
            int index;
            int amount;
            scanf("%d %d", &index, &amount);
            Remove(index, amount, shex);
        }

        else continue;
    }

    int sLen = strlen(shex);
    for (int i = 0; i < sLen; i += 2) {
        char hexByte[3] = {shex[i], shex[i+1], '\0'};
        printf("%c", (int)strtol(hexByte, NULL, 16));
    }
    printf("\n");

    return 0;
}