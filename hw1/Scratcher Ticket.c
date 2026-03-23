#include <stdio.h>
int main(void){
    long long A, B, P = 0;
    long long total = 10000;
    int over = 0;
    while (scanf("%lld %lld %lld", &A, &B, &P) == 3)
    {
        if (over){
            printf("No balance\n");
            continue;
        }

        if(A>B){
            total += P;        
        }

        else if (B>A){
            total -= P;               
        }

        if (total <= 0)
        {
            over = 1;
            printf("0\n");
        }
        
    }
    if (!over) printf("%lld\n", total);
    return 0;
}