#include "move.h"
#include <stdlib.h>

bool bishop(int starting_row, int starting_col, int final_row, int final_col, int piece)
{
    int row_diff = abs(final_row - starting_row);
    int col_diff = abs(final_col - starting_col);

    if (row_diff != col_diff) return false;

    int row_step = (final_row > starting_row) ? 1 : -1;
    int col_step = (final_col > starting_col) ? 1 : -1;

    int current_row = starting_row + row_step;
    int current_col = starting_col + col_step;

    while (current_row != final_row || current_col != final_col)
    {
        if (chess_board[current_row][current_col] != EMPTY) return false;
        current_row += row_step;
        current_col += col_step;
    }

    int final_square = chess_board[final_row][final_col];
    if (piece > 0 && final_square > 0) return false;
    if (piece < 0 && final_square < 0) return false;

    return true;
}