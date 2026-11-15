#include "logic.h"
#include <stdbool.h>
#include <stdlib.h>
#include "../piece_moves/move.h"

bool under_attack(int target_row, int target_col, int attacker_color)
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            int piece = chess_board[r][c];

            if ((attacker_color == 1 && piece > 0) || (attacker_color == -1 && piece < 0))
            {
                int abs_piece = abs(piece);

                if (abs_piece == W_PAWN)
                {
                    int dir = (piece > 0) ? -1 : 1;
                    if (r + dir == target_row && (c + 1 == target_col || c - 1 == target_col))
                        return true;
                }
                else if (abs_piece == W_KNIGHT && knight(r, c, target_row, target_col, piece)) return true;
                else if (abs_piece == W_BISHOP && bishop(r, c, target_row, target_col, piece)) return true;
                else if (abs_piece == W_ROOK   && rook(r, c, target_row, target_col, piece))   return true;
                else if (abs_piece == W_QUEEN  && queen(r, c, target_row, target_col, piece))  return true;
                else if (abs_piece == W_KING   && abs(r - target_row) <= 1 && abs(c - target_col) <= 1) return true;
            }
        }
    }
    return false;
}
