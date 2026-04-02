#include <stdio.h>

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main(void){
    int t = 0;
    scanf("%d", &t);
    for(int i =0; i<t; i++){
        int a = 0, b = 0;
        scanf("%d %d", &a, &b);
        int answer = gcd(a, b);

        printf("%d\n", answer);

    }
    return 0;
}