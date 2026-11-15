#include "logic.h"
#include <stdbool.h>
#include <stdlib.h>
#include "../piece_moves/move.h"

bool mate(int player)
{
    for (int start_r = 0; start_r < 8; start_r++)
    {
        for (int start_c = 0; start_c < 8; start_c++)
        {
            int piece = chess_board[start_r][start_c];

            if ((player == 1 && piece > 0) || (player == -1 && piece < 0))
            {
                for (int target_r = 0; target_r < 8; target_r++)
                {
                    for (int target_c = 0; target_c < 8; target_c++)
                    {
                        bool is_valid_geometry = false;

                        if (piece == W_KNIGHT || piece == B_KNIGHT)
                            is_valid_geometry = knight(start_r, start_c, target_r, target_c, piece);
                        else if (piece == W_ROOK || piece == B_ROOK)
                            is_valid_geometry = rook(start_r, start_c, target_r, target_c, piece);
                        else if (piece == W_QUEEN || piece == B_QUEEN)
                            is_valid_geometry = queen(start_r, start_c, target_r, target_c, piece);
                        else if (piece == W_BISHOP || piece == B_BISHOP)
                            is_valid_geometry = bishop(start_r, start_c, target_r, target_c, piece);
                        else if (piece == W_PAWN || piece == B_PAWN)
                            is_valid_geometry = pawn(start_r, start_c, target_r, target_c, piece);
                        else if (piece == W_KING || piece == B_KING)
                            is_valid_geometry = king(start_r, start_c, target_r, target_c, piece);

                        if (is_valid_geometry)
                        {
                            int temp_target = chess_board[target_r][target_c];
                            int temp_en_passant_victim = EMPTY;
                            int victim_row = -1;

                            if (abs(piece) == W_PAWN && target_r == en_passant_row && target_c == en_passant_col)
                            {
                                victim_row = (piece == W_PAWN) ? target_r + 1 : target_r - 1;
                                temp_en_passant_victim = chess_board[victim_row][target_c];
                                chess_board[victim_row][target_c] = EMPTY;
                            }

                            chess_board[target_r][target_c] = piece;
                            chess_board[start_r][start_c] = EMPTY;

                            bool check_failed = check(player);

                            chess_board[start_r][start_c] = piece;
                            chess_board[target_r][target_c] = temp_target;
                            if (victim_row != -1) chess_board[victim_row][target_c] = temp_en_passant_victim;

                            if (!check_failed) return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}