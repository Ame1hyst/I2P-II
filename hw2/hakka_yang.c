#include <stdio.h>
#include <stdbool.h>

typedef struct
{
   int row, col;
} Point;

void bfs(int n, int m){
    int front =0, back =0;
    Point queue[n*m];
    Point dir[] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    Point t_dir[n*m];

    bool teleport = false;
    
    bool visited[n][m];
    char matrix[n][m+1];
    for(int i =0; i <n; i++){
        scanf("%s", matrix[i]);
        for(int j=0; j<m; j++){
            visited[i][j] = false;
        }
    }

    int idx = 0;
    for(int i =0; i <n; i++){
        for(int j=0; j<m; j++){
            if(matrix[i][j] == 'T') t_dir[idx++] = (Point){i,j};
        }
    } 
    
    
    if(matrix[0][0] == '#'){
        printf("No\n");
        return;
    }

    queue[back++] = (Point){0,0};
    visited[0][0] = true;

    while (front != back)
    {
        Point cur = queue[front++];

        if(cur.row == n-1 && cur.col == m-1){
            printf("Yes\n");
            return;
        }

        //teleport?
        if(matrix[cur.row][cur.col] == 'T' && !teleport){
            teleport = true;
            for(int j =0; j < idx; j++){
                if (visited[t_dir[j].row][t_dir[j].col]) continue;
                queue[back++] = (Point){t_dir[j].row, t_dir[j].col};
                visited[t_dir[j].row][t_dir[j].col] = true;
            }
        }

        for(int i =0; i<4; i++){
            int next_row = cur.row + dir[i].row;
            int next_col = cur.col + dir[i].col;

            //Out of bound
            if(next_row >= n || next_row < 0 || next_col >= m || next_col < 0) continue;

            //Wall
            if(matrix[next_row][next_col] == '#') continue;

            //Visited?
            if(visited[next_row][next_col]) continue;


            queue[back++] = (Point){next_row, next_col};
            visited[next_row][next_col] = true;

        }
    }
    printf("No\n");
    return;
    
}


int main(void){
    int k =0, n=0, m=0;
    scanf("%d", &k);
    for(int i =0; i<k; i++){
        scanf("%d %d", &n, &m);
        bfs(n,m);
    }

    return 0;
}