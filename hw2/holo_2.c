#include <stdio.h>

int main(void){

    char num[201], first[101], second[101], total[301];
    int carry = 0;

    scanf("%s", &num);

    int found = 0;
    int len1 = 0, len2 = 0;
    for(int i =0; num[i] != '\0'; i++){
        if(!found && num[i] != '+'){
            first[len1++] = num[i];
        }

        else if(num[i] == '+') found = 1;

        else second[len2++] = num[i];
    }
    first[len1] = '\0';
    second[len2] = '\0';

    
    int index = 0;
    while (len1 >0 || len2 >0)
    {
        int fd = (len1-1 >=0) ? first[len1-1] - '0': 0;
        int sd = (len2-1 >=0) ? second[len2-1] - '0': 0;
        len1--;
        len2--;

        int sum = fd + sd + carry;

        total[index++] = sum%10 + '0';
        carry = sum/10;
    }
    if(carry) total[index++] = carry + '0';
    total[index] = '\0';
    
    for(int i =index-1; total[i]!='\0'; i--){
        printf("%c", total[i]);
    }
    

    return 0;
}