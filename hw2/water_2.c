#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    int row, col;
} Point;


int main(void){
    int n = 0, m =0;
    int front = 0, back = 0;
    int count = 1;


    scanf("%d %d", &n, &m);

    Point queue[n*m];
    bool visited[100][100] = {false};
    Point dir[] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    
    int matrix[n][m];  
    for(int i = 0; i<n; i++){
        for(int j = 0; j<m; j++){
            scanf("%d", &matrix[i][j]);
        }
    }

    //Count 0,0
    queue[back++] = (Point){0,0};
    visited[0][0] = true;

    while (front!=back)
    {
       Point cur = queue[front++];
       int cur_num = matrix[cur.row][cur.col];

       // Check the codition
       for(int i = 0; i <4; i++){
        int next_r = cur.row + dir[i].row;
        int next_c = cur.col + dir[i].col;

        //Out of matrix
        if(next_r<0 || next_r>=n) continue;
        if(next_c<0 || next_c>=m) continue;

        //Visited?
        if(visited[next_r][next_c]) continue;

        //Height
        if(cur_num < matrix[next_r][next_c]) continue;

        queue[back++] = (Point){next_r, next_c};
        visited[next_r][next_c] = true;
        count++;

       }

    }
    printf("%d\n", count);
    



    return 0;
}