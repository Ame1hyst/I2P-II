#include "14903.h"

#include <stdlib.h>
#include <stdio.h>

char **create_string_array(int n){
    char **string_arr = malloc(sizeof(char*) * n);
    for (int i = 0; i < n; i++){
        string_arr[i] = NULL; 
    }
    return string_arr;
}
void append_character(char **string_arr, int *len, int idx_line, char c){
    string_arr[idx_line] = realloc(string_arr[idx_line], len[idx_line] + 1);
    string_arr[idx_line][len[idx_line]] = c;
    len[idx_line]++;
}

void append_string(char **string_arr, int *len, int idx_line, char *str, int str_len){
    for(int i = 0; i < str_len; i++){
        append_character(string_arr, len, idx_line, str[i]);
    }
}

void print_lines(char **string_arr, int *len, int n){
    for(int i =0; i<n; i++){
        for(int j = 0; j<len[i]; j++){
            printf("%c", string_arr[i][j]);
        }
        if(len[i] > 0) printf("\n");

    }
}
void delete_string_array(char **string_arr, int n){
    for(int i =0; i<n; i++){
        free(string_arr[i]);
    }
    free(string_arr);
}