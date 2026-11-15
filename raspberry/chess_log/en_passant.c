#include "logic.h"
#include <stdbool.h>
#include <stdlib.h>
#include "../piece_moves/move.h"

bool en_passant(int sr, int sc, int er, int ec, int piece)
{
    return (abs(piece) == W_PAWN && er == en_passant_row && ec == en_passant_col);
}

void en_passant_take(int er, int ec, int piece, int *victim_row, int *victim_col, int *captured_piece)
{
    *victim_row = (piece > 0) ? er + 1 : er - 1;
    *victim_col = ec;
    *captured_piece = chess_board[*victim_row][*victim_col];
    chess_board[*victim_row][*victim_col] = EMPTY;
}