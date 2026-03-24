#include "14899.h"
#include <stdlib.h>

int **create_walkway(int n, const int *len){
    int total = 0;
    for(int i =0; i<n; i++){
        total += len[i];
    }

    int **tile = (int **)malloc(sizeof(int *)*n +
                                sizeof(int)*total
                                );
    
    int *second_layer = (int *)(tile + n);

    int offset = 0;
    for(int i =0; i<n; i++){
        tile[i] = second_layer + offset;
        offset += len[i];
    }

    return tile;

}

void activate_walkway(int n, const int *len, int **tile){
    for(int i =0; i<n; i++){
        int *origin = malloc(sizeof(int) * len[i]);
        for(int j =0; j<len[i]; j++){
            origin[j] = tile[i][j];
        }

        for(int j = 0; j<len[i]; j++){
            int left = (j-1>=0)? origin[j-1]: 0;
            int right = (j+1 < len[i]) ? origin[j+1]: 0;

            tile[i][j] = tile[i][j] + left + right;
        }
        free(origin);
    }

}
void delete_walkway(int n, int **tile){
    free(tile);
}