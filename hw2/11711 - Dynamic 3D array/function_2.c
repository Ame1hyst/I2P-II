#include "11711.h"
#include <stdlib.h>

unsigned*** new_3d_array(unsigned n,unsigned m,unsigned k){

    unsigned ***arr = (unsigned ***)malloc( sizeof(unsigned**)*n +
                                            sizeof(unsigned*)*n*m +
                                            sizeof(unsigned)*n*m*k);
    unsigned **second_layer = (unsigned **)(arr + n);
    unsigned *last_layer = (unsigned *)(second_layer+ n*m);

    for(int i =0; i<n; i++){
        arr[i] = second_layer + (i*m);
        for(int j=0; j<m; j++){
            arr[i][j] = last_layer + (i*m +j)*k;
        }
    }

    return arr;

}
void delete_3d_array(unsigned ***arr){
    free(arr);
}
