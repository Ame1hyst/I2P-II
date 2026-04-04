#include <stdio.h>
typedef struct
{
    int height, energy;
    long long absorb;
} Spire;


int main(void){
    long n = 0;
    scanf("%ld", &n);
    
    Spire arr[n];

    for(int i =0; i<n; i++)
    {
        Spire crystal;
        scanf("%lld %lld", &crystal.height, &crystal.energy);
        crystal.absorb = 0;
        arr[i] = crystal;
    }

    int stack[n];
    int top = -1;
    for(int i = 0; i < n; i++)
    {
        while(top != -1 && arr[stack[top]].height < arr[i].height){
            top--;
        }

        if(top != -1){
            arr[stack[top]].absorb += arr[i].energy;
        }

        stack[++top] = i;

    
    }

    top = -1;
    for(int i = n-1; i >= 0; i--)
    {
        while(top != -1 && arr[stack[top]].height < arr[i].height){
            top--;
        }

        if(top != -1){
            arr[stack[top]].absorb += arr[i].energy;
        }

        stack[++top] = i;

    
    }

    int maxIdx = 0;
    for(int i = 1; i < n; i++)
        if(arr[i].absorb > arr[maxIdx].absorb)
            maxIdx = i;

    printf("%d %lld\n", maxIdx + 1, arr[maxIdx].absorb);

    
    
    return 0;
}