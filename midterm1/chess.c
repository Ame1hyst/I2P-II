#include <stdio.h>

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Piece {
    char type;
    Position pos;
} Piece;

typedef struct Move {
    Position start;
    Position end;
} Move;

typedef struct GameState {
    int player_count;
    Piece player[16];
    int opponent_count;
    Piece opponent[16];
    Move moves[2];
} GameState;

typedef struct SearchResult {
    int min_steps_to_checkmate;
    Move moves[2];
} SearchResult;

// If you would like to solve with BFS
// typedef struct QueueNode {
//     GameState state;
//     int step;
// } QueueNode;

const int QUEEN_DIRS[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
const int ROOK_DIRS[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
const int KNIGHT_DIRS[8][2] = {{-1, 2}, {1, 2}, {-1, -2}, {1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
const int BISHOP_DIRS[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

void buildBoard(char board[8][8], GameState *state){
    // board;
    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            board[i][j] = '.';
        }
    }

    // player piece
    for(int i = 0; i<state->player_count; i++){
        board[state->player[i].pos.x][state->player[i].pos.y] = state->player[i].type;
    }

    // opponent piece
    for(int i = 0; i<state->opponent_count; i++){
        board[state->opponent[i].pos.x][state->opponent[i].pos.y] = state->opponent[i].type + 32;
    }
}

int in_bounded(int nx, int ny){
    return nx >= 0 && nx < 8 && ny >= 0 && ny < 8;
}

// Move
int genSliding(char board[8][8], int sx, int sy, const int dir[][2], int dir_num, Position move[]){
    int count = 0;
    for(int i = 0; i<dir_num; i++){
        int nx = sx + dir[i][0];
        int ny = sy + dir[i][1];

        while (in_bounded(nx, ny))
        {
            if(board[nx][ny] == '.') move[count++] = (Position){nx, ny};
            else if(board[nx][ny] >= 'a' && board[nx][ny] <= 'z') {move[count++] = (Position){nx, ny}; break;}
            else if(board[nx][ny] == 'k') {move[count++] = (Position){nx, ny}; break;}
            else break;

            nx += dir[i][0];
            ny += dir[i][1];

        }
 
    }
    return count;
}

int knightMove(char board[8][8], int sx, int sy, Position move[]){
    int count = 0;
    for(int i = 0; i<8; i++){
        int nx = sx + KNIGHT_DIRS[i][0];
        int ny = sy + KNIGHT_DIRS[i][1];

        if(in_bounded(nx, ny) && (board[nx][ny] == '.' || board[nx][ny] == 'k' || (board[nx][ny] >= 'a' && board[nx][ny] <= 'z'))) move[count++] = (Position){nx, ny};
    }
    return count;
}

int genMove(char board[8][8], int sx, int sy, Position move[]){
    char type = board[sx][sy];

    switch (type)
    {
    case 'Q': return genSliding(board, sx, sy, QUEEN_DIRS, 8, move);
    case 'R': return genSliding(board, sx, sy, ROOK_DIRS, 4, move);
    case 'B': return genSliding(board, sx, sy, BISHOP_DIRS, 4, move);    
    case 'N': return knightMove(board, sx, sy, move);    
    default : return 0;
    }

}

char applyMove(char board[8][8], int sx, int sy, int nx, int ny){
    char captured = board[nx][ny];
    board[nx][ny] = board[sx][sy];
    board[sx][sy] = '.';

    return captured;
}

void undoMove(char board[8][8], int sx, int sy, int nx, int ny, char captured){
    board[sx][sy] = board[nx][ny];
    board[nx][ny] = captured;
}

// DFS
int dfs(char board[8][8], int depth, int max_depth, Piece cur[], int player_count, Move path[], SearchResult *result){
    for(int i = 0; i < player_count; i++){
        char type = cur[i].type;
        if(!(type == 'Q' || type == 'B' || type == 'N'|| type == 'R')) continue;

        int sx = cur[i].pos.x;
        int sy= cur[i].pos.y;
        Position move_recoard[64];

        int n = genMove(board, sx, sy, move_recoard);
        for(int j = 0; j<n; j++){
            int nx = move_recoard[j].x; 
            int ny = move_recoard[j].y;

            // Recoard
            path[depth].start = (Position){sx, sy};
            path[depth].end = (Position){nx, ny};

            // Base case
            if(board[nx][ny] == 'k'){
                result->min_steps_to_checkmate = depth + 1;
                for(int m = 0; m<=depth; m++){
                    result->moves[m] = path[m];
                }
                return 1;
            }

            // Other base
            if(depth+1 < max_depth){
                Piece old = cur[i];
                cur[i].pos = (Position){nx, ny};
                char captured = applyMove(board, sx, sy, nx, ny);
                
                if(dfs(board, depth + 1, max_depth, cur, player_count, path, result)) return 1;

                // back tracking           
                undoMove(board, sx, sy, nx, ny, captured);
                cur[i] = old;
            }

        }
    }
    return 0;
}

void Check_Checkmate(GameState *state, SearchResult *result) {
    char board[8][8];
    buildBoard(board, state);
    Move path[2];

    Piece cur[16];
    for(int i = 0; i < state->player_count; i++){
        cur[i] = state->player[i];
    }

    if(dfs(board, 0, 1, cur, state->player_count, path, result)) return;
    else if(dfs(board, 0, 2, cur, state->player_count, path, result)) return;

    result->min_steps_to_checkmate = 3;
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        char type, pos_x, pos_y;
        GameState state;
        SearchResult result;
        result.min_steps_to_checkmate = 3;  // 3 indicates it is impossible to capture the King within two steps

        scanf("%d", &state.player_count);
        for (int i = 0; i < state.player_count; i++) {
            scanf(" %c %c%c", &type, &pos_x, &pos_y);
            state.player[i].type = type;
            state.player[i].pos.x = pos_x - 'A';
            state.player[i].pos.y = pos_y - '1';
        }

        scanf("%d", &state.opponent_count);
        for (int i = 0; i < state.opponent_count; i++) {
            scanf(" %c %c%c", &type, &pos_x, &pos_y);
            state.opponent[i].type = type;
            state.opponent[i].pos.x = pos_x - 'A';
            state.opponent[i].pos.y = pos_y - '1';
        }

        Check_Checkmate(&state, &result);

        if (result.min_steps_to_checkmate == 3) {
            printf("None\n");
        }
        else {
            printf("%d\n", result.min_steps_to_checkmate);
            for (int i = 0; i < result.min_steps_to_checkmate; i++) {
                printf("%c%c %c%c\n",
                       result.moves[i].start.x + 'A', result.moves[i].start.y + '1',
                       result.moves[i].end.x + 'A', result.moves[i].end.y + '1');
            }
        }
    }
}