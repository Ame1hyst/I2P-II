#include <stdio.h>

int main(void){

    int n = 0, q =0, a = 0, b =0;
    scanf("%d", &n);
    int arr[n];

    for(int i = 0; i<n; i++){
        scanf("%d", &arr[i]);
    }

    scanf("%d", &q);
    for(int j =0; j<q; j++){
        scanf("%d %d", &a, &b);
        int sum =0;
        for(int i = a; i<=b; i++){
            sum += arr[i];
        }
        printf("%d\n", sum);

    }

    return 0;
}