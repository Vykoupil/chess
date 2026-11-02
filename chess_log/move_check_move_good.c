#include "logic.h"
#include <stdbool.h>
#include <stdlib.h>
#include "../piece_moves/move.h"

#include "logic.h"
#include <stdbool.h>
#include <stdlib.h>
#include "../piece_moves/move.h"

#include "logic.h"
#include <stdbool.h>
#include <stdlib.h>
#include "../piece_moves/move.h"

bool check_move(int sr, int sc, int er, int ec)
{
    int piece = chess_board[sr][sc];

    if (piece == EMPTY) return false;
    if ((turn == 1 && piece < 0) || (turn == -1 && piece > 0)) return false;

    if (!geometry(sr, sc, er, ec, piece)) return false;

    int color = (piece > 0) ? 1 : -1;
    int abs_p = abs(piece);

    int target = chess_board[er][ec];
    int ep_victim = EMPTY, v_row = -1, v_col = -1;
    bool ep_used = false;

    if (en_passant(sr, sc, er, ec, piece)) {
        en_passant_take(er, ec, piece, &v_row, &v_col, &ep_victim);
        ep_used = true;
    }

    chess_board[er][ec] = piece;
    chess_board[sr][sc] = EMPTY;

    bool promoted = false;
    if (promotion(piece, er)) {
        promote(er, ec, color);
        promoted = true;
    }

    bool castle_fail = false;
    if (castling(sr, sc, er, ec, piece)) {
        if (!castling_safe(sr, sc, ec, color)) castle_fail = true;
    }

    if (check(color) || castle_fail) {
        chess_board[sr][sc] = piece;
        chess_board[er][ec] = target;
        if (ep_used) chess_board[v_row][v_col] = ep_victim;
        return false;
    }

    if (castling(sr, sc, er, ec, piece)) {
        castling_rook(sr, sc, ec);
    }

    if (abs_p == W_KING) {
        if (color == 1) white_king_moved = true;
        else black_king_moved = true;
    }

    return true;
}