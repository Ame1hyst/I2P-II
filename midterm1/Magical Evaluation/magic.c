#include "14906.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Mage {
    char name[30];
    int value;
} Mage;

// 1. Ascending
int cmp_ascending(const void* a, const void* b){
    int first = ((Mage *)a)->value;
    int second =  ((Mage *)b)->value;

    if(first > second) return 1;
    if(second > first) return -1;
    return strcmp(((Mage *)a)->name, ((Mage *)b)->name);
}

// 2. Square Ascending
int cmp_square_ascending(const void* a, const void* b){
    int first = pow(((Mage *)a)->value, 2);
    int second = pow(((Mage *)b)->value, 2);

    if(first > second) return 1;
    if(second > first) return -1;
    return strcmp(((Mage *)a)->name, ((Mage *)b)->name);
}

// 3. Digit Sum Ascending
int sum(int n){
    int sum = 0;
    n = abs(n);
    while (n > 0)
    {
        sum += n%10;
        n /= 10;
    }
    return sum;
}
int cmp_digit_sum_ascending(const void* a, const void* b){
    int first = sum(((Mage *)a)->value);
    int second = sum(((Mage *)b)->value);

    if(first > second) return 1;
    if(second > first) return -1;
    return strcmp(((Mage *)a)->name, ((Mage *)b)->name);
}

// 4. Swapped Digit Ascending
int swap(int n){
    int negative = (n<0) ? -1: 1;
    n = abs(n);
    
    int tens = n / 10;
    int ones = n % 10;

    int swapped = ones * 10 + tens;
    return swapped * negative;

}
int cmp_swapped_digit_ascending(const void* a, const void* b){
    int first = swap(((Mage *)a)->value);
    int second = swap(((Mage *)b)->value);

    if(first > second) return 1;
    if(second > first) return -1;
    return strcmp(((Mage *)a)->name, ((Mage *)b)->name);
}
int PrimeNumber(int n){
    if (n < 2) return 0;
    int root = (int)sqrt((double)n);
    for(int i =2; i<=root; i++){
        if (n % i == 0) return 0;
    }
    return 1;
}
// 5. Prime First Ascending
int cmp_prime_first_ascending(const void* a, const void* b){
    int first = abs(((Mage *)a)->value);
    int second =  abs(((Mage *)b)->value);

    if(PrimeNumber(first) && PrimeNumber(second)) return cmp_ascending(a, b);
    else if(PrimeNumber(first) && !PrimeNumber(second)) return -1;
    else if(!PrimeNumber(first) && PrimeNumber(second)) return 1;
    else if(!PrimeNumber(first) && !PrimeNumber(second))  return cmp_ascending(a, b);

}

// Use qsort with the corresponding function according to the rule.
void solve(Mage* mages, int n, int rule){
    switch (rule)
    {
    case 0:
        qsort(mages, n, sizeof(Mage), cmp_ascending);
        break;
        
    case 1:
        qsort(mages, n, sizeof(Mage), cmp_square_ascending);
        break;            

    case 2:
        qsort(mages, n, sizeof(Mage), cmp_digit_sum_ascending);
        break;            

    case 3:
        qsort(mages, n, sizeof(Mage), cmp_swapped_digit_ascending);
        break;            

    case 4:
        qsort(mages, n, sizeof(Mage), cmp_prime_first_ascending);
        break;            

    default:
        break;
        }
}