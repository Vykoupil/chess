#include "logic.h"
#include "../piece_moves/move.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool promotion(int piece, int final_row)
{
    return (abs(piece) == W_PAWN && (final_row == 0 || final_row == 7));
}

// Provede samotnou proměnu pěšce na dámu
void promote(int final_row, int final_col, int color)
{
    if (color == 1) {
        chess_board[final_row][final_col] = W_QUEEN;
    } else {
        chess_board[final_row][final_col] = B_QUEEN;
    }
}