#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    int row, col;
} Point;



int main(void){
    int front =0, back = 0;
    int n =0, m =0;
    int count = 1;
    
    scanf("%d %d", &n, &m);

    Point queue[n*m];
    int visit[100][100] = {false};
    Point dir[] = {{1,0}, {-1, 0}, {0,1}, {0,-1}};
    
    int matrix[n][m];

    for(int i = 0; i <n; i++){
        for(int j =0; j<m; j++){
            scanf("%d", &matrix[i][j]);
        }
    }

    visit[0][0] = true;

    queue[back].row = 0;
    queue[back].col = 0;
    back++;

    while(front!=back){
        int cur_r = queue[front].row;
        int cur_c = queue[front].col;
        int cur_num = matrix[cur_r][cur_c];
        front++;

        // Check for enqueue

        for(int i =0; i<4; i++){
            int next_r = cur_r + dir[i].row;
            int next_c = cur_c + dir[i].col;

            //Check Bound
            if(0>next_r || next_r>=n) continue;
            if(0>next_c || next_c>=m) continue;

            // Visited?
            if(visit[next_r][next_c]) continue;

            // Height?
            if(cur_num < matrix[next_r][next_c]) continue;

            queue[back].row = next_r;
            queue[back].col = next_c;
            visit[next_r][next_c] = true;
            back++;
            count++;
         

        }

    }
    printf("%d\n", count);
    return 0;
}