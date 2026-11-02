#include "move.h"
#include <stdio.h>
#include <stdlib.h>

bool knight(int starting_row, int starting_col, int final_row, int final_col, int piece)
{
    int row_diff = abs(final_row - starting_row);
    int col_diff = abs(final_col - starting_col);

    bool is_valid = (row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2);
    if (!is_valid) return false;

    int final_square = chess_board[final_row][final_col];
    if (piece > 0 && final_square > 0) return false;
    if (piece < 0 && final_square < 0) return false;

    return true;
}