#include <stdio.h>
#include <string.h>

// Movement direction tables for each piece type
const int DIRS_Q[8][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
const int DIRS_R[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
const int DIRS_N[8][2] = {{-1,2},{1,2},{-1,-2},{1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};
const int DIRS_B[4][2] = {{1,1},{1,-1},{-1,1},{-1,-1}};

typedef struct { int x, y; } Pos;
typedef struct { char type; Pos pos; } Piece;
typedef struct { Pos start, end; } Move;

int pc;            // number of player pieces
Piece player[16];  // player's pieces
Pos opp;           // opponent king position
int board[8][8];   // 0=empty, 1..16=player piece index+1, -1=opponent king

// Build the board from current piece positions
void build_board() {
    memset(board, 0, sizeof(board));
    for (int i = 0; i < pc; i++)
        board[player[i].pos.x][player[i].pos.y] = i + 1;
    board[opp.x][opp.y] = -1;
}

// Can piece #pi reach square (tx, ty) with the current board state?
int can_reach(int pi, int tx, int ty) {
    char t = player[pi].type;
    if (t == 'P' || t == 'K') return 0; // P and K never move

    int x = player[pi].pos.x, y = player[pi].pos.y;
    const int (*dirs)[2];
    int ndirs, sliding;

    if      (t == 'Q') { dirs = DIRS_Q; ndirs = 8; sliding = 1; }
    else if (t == 'R') { dirs = DIRS_R; ndirs = 4; sliding = 1; }
    else if (t == 'N') { dirs = DIRS_N; ndirs = 8; sliding = 0; }
    else               { dirs = DIRS_B; ndirs = 4; sliding = 1; } // B

    if (sliding) {
        // Walk step by step in each direction
        for (int d = 0; d < ndirs; d++) {
            int nx = x + dirs[d][0], ny = y + dirs[d][1];
            while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                if (board[nx][ny] > 0) break;       // own piece blocks path
                if (nx == tx && ny == ty) return 1; // reached target!
                if (board[nx][ny] < 0) break;       // opponent king blocks further
                nx += dirs[d][0];
                ny += dirs[d][1];
            }
        }
    } else {
        // Knight: check all 8 L-shape jumps (jumps over pieces, so no blocking)
        for (int d = 0; d < ndirs; d++) {
            int nx = x + dirs[d][0], ny = y + dirs[d][1];
            if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) continue;
            if (board[nx][ny] > 0) continue;        // can't land on own piece
            if (nx == tx && ny == ty) return 1;
        }
    }
    return 0;
}

// Generate all legal moves for piece #pi into moves[]
// Returns the count of moves generated
int gen_moves(int pi, Move *moves) {
    char t = player[pi].type;
    if (t == 'P' || t == 'K') return 0;

    int x = player[pi].pos.x, y = player[pi].pos.y;
    int count = 0;
    const int (*dirs)[2];
    int ndirs, sliding;

    if      (t == 'Q') { dirs = DIRS_Q; ndirs = 8; sliding = 1; }
    else if (t == 'R') { dirs = DIRS_R; ndirs = 4; sliding = 1; }
    else if (t == 'N') { dirs = DIRS_N; ndirs = 8; sliding = 0; }
    else               { dirs = DIRS_B; ndirs = 4; sliding = 1; }

    if (sliding) {
        for (int d = 0; d < ndirs; d++) {
            int nx = x + dirs[d][0], ny = y + dirs[d][1];
            while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                if (board[nx][ny] > 0) break;   // own piece: can't go here
                // Add this square as a valid destination
                moves[count].start.x = x; moves[count].start.y = y;
                moves[count].end.x   = nx; moves[count].end.y  = ny;
                count++;
                if (board[nx][ny] < 0) break;   // king: capture and stop
                nx += dirs[d][0]; ny += dirs[d][1];
            }
        }
    } else { // knight
        for (int d = 0; d < ndirs; d++) {
            int nx = x + dirs[d][0], ny = y + dirs[d][1];
            if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) continue;
            if (board[nx][ny] > 0) continue;
            moves[count].start.x = x; moves[count].start.y = y;
            moves[count].end.x   = nx; moves[count].end.y  = ny;
            count++;
        }
    }
    return count;
}

int min_steps;
Move best_moves[2];

void check_checkmate() {
    min_steps = 3; // 3 means impossible

    // ---- Try 1-move: can any piece directly capture the king? ----
    for (int i = 0; i < pc; i++) {
        if (can_reach(i, opp.x, opp.y)) {
            min_steps = 1;
            best_moves[0].start = player[i].pos;
            best_moves[0].end   = opp;
            return;
        }
    }

    // ---- Try 2-move: try every possible first move, then check again ----
    Move first_moves[64]; // max ~56 moves per piece (queen, open board)
    for (int i = 0; i < pc; i++) {
        int nm = gen_moves(i, first_moves);
        for (int m = 0; m < nm; m++) {
            Move mv = first_moves[m];

            // Skip first moves that directly capture the king
            // (those would've been caught by 1-move check above)
            if (mv.end.x == opp.x && mv.end.y == opp.y) continue;

            // === Apply the first move ===
            board[mv.start.x][mv.start.y] = 0;   // piece leaves old square
            board[mv.end.x][mv.end.y] = i + 1;   // piece arrives at new square
            Pos saved = player[i].pos;
            player[i].pos = mv.end;

            // Check if any piece can now capture the king (2nd move)
            for (int j = 0; j < pc; j++) {
                if (can_reach(j, opp.x, opp.y)) {
                    min_steps = 2;
                    best_moves[0] = mv;
                    best_moves[1].start = player[j].pos;
                    best_moves[1].end   = opp;
                    // Undo before returning
                    board[mv.end.x][mv.end.y]   = 0;
                    board[mv.start.x][mv.start.y] = i + 1;
                    player[i].pos = saved;
                    return;
                }
            }

            // === Undo the first move ===
            board[mv.end.x][mv.end.y]     = 0;
            board[mv.start.x][mv.start.y] = i + 1;
            player[i].pos = saved;
        }
    }
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        char type, px, py;

        scanf("%d", &pc);
        for (int i = 0; i < pc; i++) {
            scanf(" %c %c%c", &type, &px, &py);
            player[i].type    = type;
            player[i].pos.x   = px - 'A'; // 'A'=0 ... 'H'=7
            player[i].pos.y   = py - '1'; // '1'=0 ... '8'=7
        }

        int oc;
        scanf("%d", &oc); // always 1
        scanf(" %c %c%c", &type, &px, &py);
        opp.x = px - 'A';
        opp.y = py - '1';

        build_board();
        check_checkmate();

        if (min_steps == 3) {
            printf("None\n");
        } else {
            printf("%d\n", min_steps);
            for (int i = 0; i < min_steps; i++) {
                printf("%c%c %c%c\n",
                    best_moves[i].start.x + 'A', best_moves[i].start.y + '1',
                    best_moves[i].end.x   + 'A', best_moves[i].end.y   + '1');
            }
        }
    }
    return 0;
}