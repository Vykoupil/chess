#include "chess_graphic.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#include "../piece_moves/move.h"

void load_figure()
{
    white_king = LoadTexture("../Image/white/white_king.png");
    white_queen = LoadTexture("../Image/white/white_queen.png");
    white_pawn = LoadTexture("../Image/white/white_pawn.png");
    white_knight = LoadTexture("../Image/white/white_knight.png");
    white_bishop = LoadTexture("../Image/white/white_bishop.png");
    white_rook = LoadTexture("../Image/white/white_rook.png");

    black_king = LoadTexture("../Image/black/black_king.png");
    black_queen = LoadTexture("../Image/black/black_queen.png");
    black_pawn = LoadTexture("../Image/black/black_pawn.png");
    black_knight = LoadTexture("../Image/black/black_knight.png");
    black_bishop = LoadTexture("../Image/black/black_bishop.png");
    black_rook = LoadTexture("../Image/black/black_rook.png");
}

void unload_figure()
{
    UnloadTexture(white_king);
    UnloadTexture(white_queen);
    UnloadTexture(white_pawn);
    UnloadTexture(white_knight);
    UnloadTexture(white_bishop);
    UnloadTexture(white_rook);

    UnloadTexture(black_king);
    UnloadTexture(black_queen);
    UnloadTexture(black_pawn);
    UnloadTexture(black_knight);
    UnloadTexture(black_bishop);
    UnloadTexture(black_rook);
}

void draw_board(int offset, int board_size, int selected_row, int selected_col, bool is_running)
{
    int squareSize = board_size / 8;

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            int x = offset + c * squareSize;
            int y = offset + r * squareSize;

            if ((r + c) % 2 == 0)
            {
                DrawRectangle(x, y, squareSize, squareSize, BEIGE);
            }
            else
            {
                DrawRectangle(x, y, squareSize, squareSize, BROWN);
            }
        }
    }

    if (is_running)
    {
        
        int checked_colors[] = {1, -1};
        for (int i = 0; i < 2; i++)
        {
            int king_color = checked_colors[i];
            if (check(king_color))
            {
                int target_king = (king_color == 1) ? W_KING : B_KING;
                
                for (int r = 0; r < 8; r++)
                {
                    for (int c = 0; c < 8; c++)
                    {
                        if (chess_board[r][c] == target_king)
                        {
                            int x = offset + c * squareSize;
                            int y = offset + r * squareSize;
                            
                            DrawRectangle(x, y, squareSize, squareSize, ColorAlpha(RED, 0.7f));
                            DrawRectangleLinesEx((Rectangle){ (float)x, (float)y, (float)squareSize, (float)squareSize }, 4, RED);
                        }
                    }
                }
            }
        }

        if (selected_row != -1 && selected_col != -1)
        {
            int x = offset + selected_col * squareSize;
            int y = offset + selected_row * squareSize;

            if (chess_board[selected_row][selected_col] > 0)
            {
                DrawRectangle(x, y, squareSize, squareSize, ColorAlpha(YELLOW, 0.6f));
                DrawRectangleLinesEx((Rectangle){ (float)x, (float)y, (float)squareSize, (float)squareSize }, 3, YELLOW);
            }
            else if (chess_board[selected_row][selected_col] < 0)
            {
                DrawRectangle(x, y, squareSize, squareSize, ColorAlpha(RED, 0.6f));
                DrawRectangleLinesEx((Rectangle){ (float)x, (float)y, (float)squareSize, (float)squareSize }, 3, RED);
            }
        }
    }

    DrawText("Chess Board", board_size / 2 - 50, board_size / 2 - 400, 40, BLACK);
    
    if (is_running)
    {
        if (game_over)
        {
            if (result == 1)
                DrawText("CHECKMATE! WHITE WINS!", offset, offset - 35, 24, GREEN);
            else if (result == -1)
                DrawText("CHECKMATE! BLACK WINS!", offset, offset - 35, 24, RED);
            else if (result == 2)
                DrawText("MATCH ENDED IN A DRAW!", offset, offset - 35, 24, ORANGE);
        }
        else
        {
            if (turn == 1)
                DrawText("Turn: WHITE", offset, offset - 30, 20, DARKGRAY);
            else
                DrawText("Turn: BLACK", offset, offset - 30, 20, BLACK);
        }
    }

    char chars[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    int numbers[] = {8, 7, 6, 5, 4, 3, 2, 1};

    for (int i = 0; i < 8; i++)
    {
        DrawText(TextFormat("%c", chars[i]), offset + i * squareSize + squareSize / 2 - 10, offset + board_size + 10, 20, BLACK);
        DrawText(TextFormat("%d", numbers[i]), offset - 30, offset + i * squareSize + squareSize / 2 - 10, 20, BLACK);
    }
}

// Vykresli obrazek figurky do jednoho policka bez ohledu na velikost PNG.
static void draw_scaled_piece(Texture2D texture, int x, int y, int squareSize)
{
    if (texture.id == 0 || texture.width <= 0 || texture.height <= 0) return;
    float padding = squareSize * 0.05f;
    Rectangle source = {0, 0, (float)texture.width, (float)texture.height};
    Rectangle destination = {(float)x + padding, (float)y + padding,
                             (float)squareSize - 2 * padding, (float)squareSize - 2 * padding};
    DrawTexturePro(texture, source, destination, (Vector2){0, 0}, 0.0f, WHITE);
}

void draw_piece(int board[8][8], int offset, int squareSize)
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            int figure = board[r][c];
            int x = offset + c * squareSize;
            int y = offset + r * squareSize;

            switch (figure)
            {
                case W_PAWN:   draw_scaled_piece(white_pawn, x, y, squareSize); break;
                case W_KNIGHT: draw_scaled_piece(white_knight, x, y, squareSize); break;
                case W_BISHOP: draw_scaled_piece(white_bishop, x, y, squareSize); break;
                case W_ROOK:   draw_scaled_piece(white_rook, x, y, squareSize); break;
                case W_QUEEN:  draw_scaled_piece(white_queen, x, y, squareSize); break;
                case W_KING:   draw_scaled_piece(white_king, x, y, squareSize); break;

                case B_PAWN:   draw_scaled_piece(black_pawn, x, y, squareSize); break;
                case B_KNIGHT: draw_scaled_piece(black_knight, x, y, squareSize); break;
                case B_BISHOP: draw_scaled_piece(black_bishop, x, y, squareSize); break;
                case B_ROOK:   draw_scaled_piece(black_rook, x, y, squareSize); break;
                case B_QUEEN:  draw_scaled_piece(black_queen, x, y, squareSize); break;
                case B_KING:   draw_scaled_piece(black_king, x, y, squareSize); break;

                default: break;
            }
        }
    }
}