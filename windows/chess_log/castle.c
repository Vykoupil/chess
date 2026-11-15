#include "logic.h"
#include "../piece_moves/move.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool castling(int sr, int sc, int er, int ec, int piece)
{
    return (abs(piece) == W_KING && abs(ec - sc) == 2);
}

bool castling_safe(int row, int start_c, int end_c, int color)
{
    int step = (end_c > start_c) ? 1 : -1;
    for (int c = start_c; c != end_c + step; c += step) {
        if (under_attack(row, c, -color)) {
            return false;
        }
    }
    return true;
}


void castling_rook(int row, int start_c, int end_c)
{
    if (end_c > start_c) { // Malá rošáda (krátká)
        chess_board[row][5] = chess_board[row][7];
        chess_board[row][7] = EMPTY;
    } else { // Velká rošáda (dlouhá)
        chess_board[row][3] = chess_board[row][0];
        chess_board[row][0] = EMPTY;
    }
}