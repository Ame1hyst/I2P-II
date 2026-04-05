#include <stdio.h>

typedef struct { int x, y; } Position;
typedef struct { char type; Position pos; } Piece;
typedef struct { Position start, end; } Move;

typedef struct {
    int player_count;
    Piece player[16];
    int opponent_count;
    Piece opponent[16];
    Move moves[2];
} GameState;

typedef struct {
    int min_steps_to_checkmate;
    Move moves[2];
} SearchResult;

const int QUEEN_DIRS[8][2]  = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
const int ROOK_DIRS[4][2]   = {{1,0},{-1,0},{0,1},{0,-1}};
const int KNIGHT_DIRS[8][2] = {{-1,2},{1,2},{-1,-2},{1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};
const int BISHOP_DIRS[4][2] = {{1,1},{1,-1},{-1,1},{-1,-1}};

// ── HELPER ────────────────────────────────────────────────────
int in_bounds(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

// ── BUILD BOARD ───────────────────────────────────────────────
// Converts GameState → 2D char board
// '.' = empty, uppercase = player piece, 'k' = enemy King
void build_board(char board[8][8], GameState *state) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board[i][j] = '.';

    for (int i = 0; i < state->player_count; i++)
        board[state->player[i].pos.x][state->player[i].pos.y] = state->player[i].type;

    for (int i = 0; i < state->opponent_count; i++)
        board[state->opponent[i].pos.x][state->opponent[i].pos.y] = 'k';
}

// ── MOVE GENERATORS ───────────────────────────────────────────
// Sliding pieces (Queen, Rook, Bishop):
// Keep stepping in each direction until blocked or out of bounds
int gen_sliding(char board[8][8], int sx, int sy,
                const int dirs[][2], int nd, Position out[], int max) {
    int count = 0;
    for (int d = 0; d < nd && count < max; d++) {
        int nx = sx + dirs[d][0];
        int ny = sy + dirs[d][1];
        while (in_bounds(nx, ny) && count < max) {
            if      (board[nx][ny] == '.') { out[count++] = (Position){nx, ny};          }
            else if (board[nx][ny] == 'k') { out[count++] = (Position){nx, ny}; break;   }
            else                           {                                     break;   }
            nx += dirs[d][0];
            ny += dirs[d][1];
        }
    }
    return count;
}

// Knight: L-shape jump, ignores pieces in between
int gen_knight(char board[8][8], int sx, int sy, Position out[], int max) {
    int count = 0;
    for (int d = 0; d < 8 && count < max; d++) {
        int nx = sx + KNIGHT_DIRS[d][0];
        int ny = sy + KNIGHT_DIRS[d][1];
        if (in_bounds(nx, ny) && (board[nx][ny] == '.' || board[nx][ny] == 'k'))
            out[count++] = (Position){nx, ny};
    }
    return count;
}

// Dispatch to correct generator based on piece type
int get_moves(char board[8][8], int sx, int sy, Position out[], int max) {
    char t = board[sx][sy];
    if (t == 'Q') return gen_sliding(board, sx, sy, QUEEN_DIRS,  8, out, max);
    if (t == 'R') return gen_sliding(board, sx, sy, ROOK_DIRS,   4, out, max);
    if (t == 'B') return gen_sliding(board, sx, sy, BISHOP_DIRS, 4, out, max);
    if (t == 'N') return gen_knight (board, sx, sy,               out, max);
    return 0; // P and K don't move
}

// ── APPLY / UNDO ──────────────────────────────────────────────
// Apply: move piece, return what was captured (for undo)
char apply_move(char board[8][8], int sx, int sy, int ex, int ey) {
    char captured = board[ex][ey];
    board[ex][ey] = board[sx][sy];
    board[sx][sy] = '.';
    return captured;
}

// Undo: restore board exactly as it was before apply
void undo_move(char board[8][8], int sx, int sy, int ex, int ey, char captured) {
    board[sx][sy] = board[ex][ey];
    board[ex][ey] = captured;
}

// ── DFS ───────────────────────────────────────────────────────
// board     : current board (modified in place, restored after)
// depth     : moves made so far (0 = no moves yet)
// max_depth : search limit (1 or 2)
// path[]    : records moves made so far
// result    : written to when solution is found
//
// Call tree example (max_depth=2):
//
//   dfs(depth=0)
//   └── try piece Q at H1 → move H1→H5
//       ├── board[H5] != 'k', so recurse
//       ├── apply H1→H5
//       ├── dfs(depth=1)
//       │   └── try piece Q at H5 → move H5→H8
//       │       ├── board[H8] == 'k' → FOUND!
//       │       └── save path, return 1
//       ├── return 1 (bubbles up)
//       └── (undo never reached because we returned)
//
// Note: undo IS still called when a branch fails (returns 0)
int dfs(char board[8][8], int depth, int max_depth,
        Move path[], SearchResult *result) {

    // Try every square on the board
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            char c = board[x][y];

            // Skip non-moving pieces (P, K) and empty squares
            if (!(c=='Q' || c=='R' || c=='N' || c=='B')) continue;

            Position moves_buf[64];
            int n = get_moves(board, x, y, moves_buf, 64);

            for (int i = 0; i < n; i++) {
                int ex = moves_buf[i].x;
                int ey = moves_buf[i].y;

                // Record this move in the path
                path[depth].start = (Position){x,  y };
                path[depth].end   = (Position){ex, ey};

                // ── BASE CASE: captured the King! ──────────
                // No need to recurse, we're done
                if (board[ex][ey] == 'k') {
                    result->min_steps_to_checkmate = depth + 1;
                    for (int m = 0; m <= depth; m++)
                        result->moves[m] = path[m];
                    return 1;
                }

                // ── RECURSE if we still have moves left ────
                // Only go deeper if depth allows it
                if (depth + 1 < max_depth) {
                    char captured = apply_move(board, x, y, ex, ey);

                    if (dfs(board, depth + 1, max_depth, path, result)) {
                        return 1;  // bubble success up
                    }

                    // ── BACKTRACK ──────────────────────────
                    // This branch failed, restore board and try next move
                    undo_move(board, x, y, ex, ey, captured);
                }
            }
        }
    }

    return 0; // no solution found at this depth
}

// ── CHECK CHECKMATE ───────────────────────────────────────────
// Try depth 1 first (minimum moves), then depth 2
// This guarantees we always find the SHORTEST solution
void Check_Checkmate(GameState *state, SearchResult *result) {
    char board[8][8];
    build_board(board, state);
    Move path[2];

    // Always try shorter depth first!
    // If we only ran max_depth=2, we might record a 2-move
    // solution without realising a 1-move solution exists
    if (dfs(board, 0, 1, path, result)) return;
    if (dfs(board, 0, 2, path, result)) return;

    result->min_steps_to_checkmate = 3; // impossible
}

// ── MAIN ──────────────────────────────────────────────────────
int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        char type, pos_x, pos_y;
        GameState state;
        SearchResult result;
        result.min_steps_to_checkmate = 3;

        scanf("%d", &state.player_count);
        for (int i = 0; i < state.player_count; i++) {
            scanf(" %c %c%c", &type, &pos_x, &pos_y);
            state.player[i].type  = type;
            state.player[i].pos.x = pos_x - 'A';
            state.player[i].pos.y = pos_y - '1';
        }

        scanf("%d", &state.opponent_count);
        for (int i = 0; i < state.opponent_count; i++) {
            scanf(" %c %c%c", &type, &pos_x, &pos_y);
            state.opponent[i].type  = type;
            state.opponent[i].pos.x = pos_x - 'A';
            state.opponent[i].pos.y = pos_y - '1';
        }

        Check_Checkmate(&state, &result);

        if (result.min_steps_to_checkmate == 3) {
            printf("None\n");
        } else {
            printf("%d\n", result.min_steps_to_checkmate);
            for (int i = 0; i < result.min_steps_to_checkmate; i++) {
                printf("%c%c %c%c\n",
                       result.moves[i].start.x + 'A', result.moves[i].start.y + '1',
                       result.moves[i].end.x   + 'A', result.moves[i].end.y   + '1');
            }
        }
    }
    return 0;
}