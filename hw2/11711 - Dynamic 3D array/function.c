#include "11711.h"
#include <stdlib.h>

unsigned*** new_3d_array(unsigned n,unsigned m,unsigned k){
    unsigned ***arr = malloc(sizeof(unsigned **) * n);
    arr[0] = malloc(sizeof( unsigned *)*n*m);
    for(int i = 0; i <n; i++){
        arr[i] = arr[0] + (i*m);
    }

    arr[0][0] = malloc(sizeof( unsigned)*n*m*k);
    for(int i = 0; i <n; i++){
        for(int j = 0; j<m;j++){
            arr[i][j] = arr[0][0] + (i*m + j) * k;
        }
    }

    return arr;
}


void delete_3d_array(unsigned ***arr){

    free(arr[0][0]);
    free(arr[0]);
    free(arr);

}
