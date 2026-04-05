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


const int QUEEN_DIRS[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
const int ROOK_DIRS[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
const int KNIGHT_DIRS[8][2] = {{-1, 2}, {1, 2}, {-1, -2}, {1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
const int BISHOP_DIRS[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

//Buid Board
void buildBoard(char board[8][8], GameState *state){
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board[i][j] = '.';
    
    for(int i = 0; i<state->player_count; i++){
        board[state->player[i].pos.x][state->player[i].pos.y] = state->player[i].type;
    }

    for(int i =0; i<state->opponent_count; i++){
        board[state->opponent[i].pos.x][state->opponent[i].pos.y] = state->opponent[i].type;
    }
}

// Helper
int in_bound(int nx, int ny){
    return nx >= 0 && nx < 8 && ny >= 0 && ny < 8;
}

// Move




void Check_Checkmate(...) {
    ...
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

        Check_Checkmate(...);

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