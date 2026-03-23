#include <stdio.h>
int main(void){
    int L, m, n = 0;
    scanf("%d %d %d", &L, &m, &n);

    int t = (L/m)*n;
    printf("%d", t);
    return 0;
}