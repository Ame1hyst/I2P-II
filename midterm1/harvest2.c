#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    int height, value;
    long long absorb;
} Spires;

int main(void){
    int n =0;
    scanf("%d", &n);

    Spires *num = malloc(sizeof(Spires) * n);
    for(int i =0; i<n; i++){
        scanf("%d %d", &num[i].height, &num[i].value);
        num[i].absorb = 0;
    }

    int *stack_index = malloc(sizeof(int)*n);
    int top = -1;
    
    //Left side taller
    for(int i =0; i<n; i++){
        while (top != -1 && num[stack_index[top]].height <= num[i].height)
        {
            top--; //pop
        };

        if(top != -1){ //left higher than right
            num[stack_index[top]].absorb += num[i].value;
        }

        stack_index[++top] = i; //add
        
    }

    //Right side taller
    top = -1;
    for(int i =n-1; i>=0; i--){
        while (top != -1 && num[stack_index[top]].height <= num[i].height)
        {
            top--; //pop
        };

        if(top != -1){ //right higher than left
            num[stack_index[top]].absorb += num[i].value;
        }

        stack_index[++top] = i; //add
        
    }

    int max_index = 0;
    for(int i =1; i<n; i++){
        max_index = (num[max_index].absorb >= num[i].absorb) ? max_index: i;
    }
     printf("%d %lld\n", max_index+1, num[max_index].absorb);

    free(num);
    free(stack_index);
}


