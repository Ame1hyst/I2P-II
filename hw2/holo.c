#include <stdio.h>
#include <string.h>

int main(void){

    char arr[201];
    scanf("%s", arr);

    char first[101];
    char second[101];
    char total[300];
    int carry = 0;
    int found = 0;

    for(int i = 0; arr[i] != '\0'; i++){
        if(arr[i] == '+'){
            found = i;
            break;
        }
        first[i] = arr[i];
    }
    first[found] = '\0';


    int j = 0;
    for(int i = found + 1; arr[i] != '\0'; i++){
        second[j++] = arr[i];
    }
    second[j] = '\0';

    int f_len = strlen(first) - 1;
    int s_len = strlen(second) - 1;
    
    int index = 0;
    while (f_len >=0 || s_len >=0)
    {   
        int fd = (f_len >=0) ? first[f_len--] - '0': 0;
        int sd = (s_len >=0) ? second[s_len--] - '0': 0;

        int sum = carry + fd + sd;
        carry = sum/10;
        total[index++] = sum%10 + '0';
    }

    if(carry) total[index++] = carry + '0';
    
    for(int i=index-1; i>=0; i--){
        printf("%c", total[i]);
    }
    



    return 0;
}