#include <stdio.h>

int main(void){
    int n =0;
    scanf("%d", &n);

    int arr[n];

    int sum = 0;
    for(int i =0; i< n; i++){
        int num = 0;
        scanf("%d", &num);
        sum += num;
        arr[i] = sum;
    }

    int q = 0;
    scanf("%d", &q);
    for(int i =0; i<q; i++){
        int a =0, b =0;
        scanf("%d %d", &a, &b);
        if(a == 0) printf("%d\n", arr[b]);
        else printf("%d\n", arr[b] - arr[a-1]);

    }

    return 0;
}