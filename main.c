#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>

// ============================================================================
// DOPŘEDNÉ DEKLARACE FUNKCÍ (PROTOTYPY)
// ============================================================================
bool is_in_check(int king_color);
bool is_square_attacked(int target_row, int target_col, int attacker_color);
bool knight_move(int starting_row, int starting_col, int final_row, int final_col, int piece);
bool rook_move(int starting_row, int starting_col, int final_row, int final_col, int piece);
bool queen_move(int starting_row, int starting_col, int final_row, int final_col, int piece);
bool bishop_move(int starting_row, int starting_col, int final_row, int final_col, int piece);
bool pawn_move(int starting_row, int starting_col, int final_row, int final_col, int piece);
bool king_move(int starting_row, int starting_col, int final_row, int final_col, int piece);
void reset_game();

// ============================================================================
// TEXTURY A GLOBÁLNÍ PROMĚNNÉ
// ============================================================================
Texture2D white_king, white_queen, white_pawn, white_knight, white_bishop, white_rook;
Texture2D black_king, black_queen, black_pawn, black_knight, black_bishop, black_rook;

enum Piece {
    EMPTY = 0,
    W_PAWN = 1, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN = -1, B_KNIGHT = -2, B_BISHOP = -3, B_ROOK = -4, B_QUEEN = -5, B_KING = -6
};

int board[8][8] = {
    {-4, -2, -3, -5, -6, -3, -2, -4},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {4, 2, 3, 5, 6, 3, 2, 4}
};

// Řízení stavu hry a střídání tahů
int current_turn = 1;     // 1 = Bílý, -1 = Černý
bool is_game_over = false;
int game_result = 0;      // 1 = Výhra bílého, -1 = Výhra černého, 2 = Pat (remíza)

// Speciální pravidla (Rošáda & En Passant)
int en_passant_row = -1;
int en_passant_col = -1;

bool white_king_moved = false;
bool white_rook_a1_moved = false;
bool white_rook_h1_moved = false;

bool black_king_moved = false;
bool black_rook_a8_moved = false;
bool black_rook_h8_moved = false;

// ============================================================================
// NAČÍTÁNÍ A UVOLŇOVÁNÍ TEXTUR
// ============================================================================
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

// Funkce pro kompletní restart desky
void reset_game()
{
    int initial_board[8][8] = {
        {-4, -2, -3, -5, -6, -3, -2, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 2, 3, 5, 6, 3, 2, 4}
    };

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            board[r][c] = initial_board[r][c];
        }
    }

    current_turn = 1;
    is_game_over = false;
    game_result = 0;
    en_passant_row = -1;
    en_passant_col = -1;
    white_king_moved = false;
    white_rook_a1_moved = false;
    white_rook_h1_moved = false;
    black_king_moved = false;
    black_rook_a8_moved = false;
    black_rook_h8_moved = false;
}

// ============================================================================
// VYKRESLOVACÍ FUNKCE
// ============================================================================
void draw_board(int offset, int board_size, int selected_row, int selected_col, bool is_running)
{
    int squareSize = board_size / 8;

    // 1. Šachovnicové pole
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
        // 2. Červené zvýraznění krále při šachu
        int checked_colors[] = {1, -1};
        for (int i = 0; i < 2; i++)
        {
            int king_color = checked_colors[i];
            if (is_in_check(king_color))
            {
                int target_king = (king_color == 1) ? W_KING : B_KING;
                
                for (int r = 0; r < 8; r++)
                {
                    for (int c = 0; c < 8; c++)
                    {
                        if (board[r][c] == target_king)
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

        // 3. Zvýraznění vybrané figurky
        if (selected_row != -1 && selected_col != -1)
        {
            int x = offset + selected_col * squareSize;
            int y = offset + selected_row * squareSize;

            if (board[selected_row][selected_col] > 0)
            {
                DrawRectangle(x, y, squareSize, squareSize, ColorAlpha(YELLOW, 0.6f));
                DrawRectangleLinesEx((Rectangle){ (float)x, (float)y, (float)squareSize, (float)squareSize }, 3, YELLOW);
            }
            else if (board[selected_row][selected_col] < 0)
            {
                DrawRectangle(x, y, squareSize, squareSize, ColorAlpha(RED, 0.6f));
                DrawRectangleLinesEx((Rectangle){ (float)x, (float)y, (float)squareSize, (float)squareSize }, 3, RED);
            }
        }
    }

    // 4. Nadpisy a stav hry
    DrawText("Chess Board", board_size / 2 - 50, board_size / 2 - 400, 40, BLACK);
    
    if (is_running)
    {
        if (is_game_over)
        {
            if (game_result == 1)
                DrawText("CHECKMATE! WHITE WINS!", offset, offset - 35, 24, GREEN);
            else if (game_result == -1)
                DrawText("CHECKMATE! BLACK WINS!", offset, offset - 35, 24, RED);
            else if (game_result == 2)
                DrawText("MATCH ENDED IN A DRAW!", offset, offset - 35, 24, ORANGE);
        }
        else
        {
            if (current_turn == 1)
                DrawText("Turn: WHITE", offset, offset - 30, 20, DARKGRAY);
            else
                DrawText("Turn: BLACK", offset, offset - 30, 20, BLACK);
        }
    }
            
    // Souřadnice na okrajích
    char chars[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    int numbers[] = {8, 7, 6, 5, 4, 3, 2, 1};

    for (int i = 0; i < 8; i++)
    {
        DrawText(TextFormat("%c", chars[i]), offset + i * squareSize + squareSize / 2 - 10, offset + board_size + 10, 20, BLACK);
        DrawText(TextFormat("%d", numbers[i]), offset - 30, offset + i * squareSize + squareSize / 2 - 10, 20, BLACK);
    }
}

void draw_board_piece(int board[8][8], int offset, int squareSize)
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
                case W_PAWN:   DrawTexture(white_pawn, x, y, WHITE); break;
                case W_KNIGHT: DrawTexture(white_knight, x, y, WHITE); break;
                case W_BISHOP: DrawTexture(white_bishop, x, y, WHITE); break;
                case W_ROOK:   DrawTexture(white_rook, x, y, WHITE); break;
                case W_QUEEN:  DrawTexture(white_queen, x, y, WHITE); break;
                case W_KING:   DrawTexture(white_king, x, y, WHITE); break;

                case B_PAWN:   DrawTexture(black_pawn, x, y, WHITE); break;
                case B_KNIGHT: DrawTexture(black_knight, x, y, WHITE); break;
                case B_BISHOP: DrawTexture(black_bishop, x, y, WHITE); break;
                case B_ROOK:   DrawTexture(black_rook, x, y, WHITE); break;
                case B_QUEEN:  DrawTexture(black_queen, x, y, WHITE); break;
                case B_KING:   DrawTexture(black_king, x, y, WHITE); break;

                default: break;
            }
        }
    }
}

// ============================================================================
// POHYBY FIGUREK (GEOMETRIE)
// ============================================================================
bool knight_move(int starting_row, int starting_col, int final_row, int final_col, int piece)
{
    int row_diff = abs(final_row - starting_row);
    int col_diff = abs(final_col - starting_col);

    bool is_valid = (row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2);
    if (!is_valid) return false;

    int final_square = board[final_row][final_col];
    if (piece > 0 && final_square > 0) return false;
    if (piece < 0 && final_square < 0) return false;

    return true;
}

bool rook_move(int starting_row, int starting_col, int final_row, int final_col, int piece)
{
    int row_diff = abs(final_row - starting_row);
    int col_diff = abs(final_col - starting_col);

    bool is_valid = (row_diff == 0 && col_diff > 0) || (col_diff == 0 && row_diff > 0);
    if (!is_valid) return false;

    int row_step = (final_row > starting_row) ? 1 : ((final_row < starting_row) ? -1 : 0);
    int col_step = (final_col > starting_col) ? 1 : ((final_col < starting_col) ? -1 : 0);

    int current_row = starting_row + row_step;   
    int current_col = starting_col + col_step;
    
    while (current_row != final_row || current_col != final_col)
    {
        if (board[current_row][current_col] != EMPTY) return false;
        current_row += row_step;
        current_col += col_step;
    }

    int final_square = board[final_row][final_col];
    if (piece > 0 && final_square > 0) return false;
    if (piece < 0 && final_square < 0) return false;

    return true;
}

bool queen_move(int starting_row, int starting_col, int final_row, int final_col, int piece)
{
    int row_diff = abs(final_row - starting_row);
    int col_diff = abs(final_col - starting_col);

    bool is_valid = (row_diff == 0 && col_diff > 0) || (row_diff > 0 && col_diff == 0) || (row_diff == col_diff);
    if (!is_valid) return false;

    int row_step = (final_row > starting_row) ? 1 : ((final_row < starting_row) ? -1 : 0);
    int col_step = (final_col > starting_col) ? 1 : ((final_col < starting_col) ? -1 : 0);

    int current_row = starting_row + row_step;
    int current_col = starting_col + col_step;
    
    while (current_row != final_row || current_col != final_col)
    {
        if (board[current_row][current_col] != EMPTY) return false;
        current_row += row_step;
        current_col += col_step;
    }
    
    int final_square = board[final_row][final_col];
    if (piece > 0 && final_square > 0) return false;
    if (piece < 0 && final_square < 0) return false;

    return true;
}

bool bishop_move(int starting_row, int starting_col, int final_row, int final_col, int piece)
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
        if (board[current_row][current_col] != EMPTY) return false;
        current_row += row_step;
        current_col += col_step;
    }

    int final_square = board[final_row][final_col];
    if (piece > 0 && final_square > 0) return false;
    if (piece < 0 && final_square < 0) return false;

    return true;
}

bool pawn_move(int starting_row, int starting_col, int final_row, int final_col, int piece)
{
    int row_diff = final_row - starting_row;
    int col_diff = abs(final_col - starting_col);
    int final_square = board[final_row][final_col];

    if (piece > 0) // Bílý pěšec
    {
        if (col_diff == 0 && row_diff == -1 && final_square == EMPTY) return true;
        if (col_diff == 0 && row_diff == -2 && starting_row == 6 && final_square == EMPTY && board[starting_row - 1][starting_col] == EMPTY) return true;
        if (col_diff == 1 && row_diff == -1 && final_square < 0) return true;
        if (col_diff == 1 && row_diff == -1 && final_row == en_passant_row && final_col == en_passant_col) return true;
    }
    else if (piece < 0) // Černý pěšec
    {
        if (col_diff == 0 && row_diff == 1 && final_square == EMPTY) return true;
        if (col_diff == 0 && row_diff == 2 && starting_row == 1 && final_square == EMPTY && board[starting_row + 1][starting_col] == EMPTY) return true;
        if (col_diff == 1 && row_diff == 1 && final_square > 0) return true;
        if (col_diff == 1 && row_diff == 1 && final_row == en_passant_row && final_col == en_passant_col) return true;
    }

    return false;
}

bool king_move(int starting_row, int starting_col, int final_row, int final_col, int piece)
{
    int row_diff = abs(final_row - starting_row);
    int col_diff = abs(final_col - starting_col);

    // Rošáda
    if ((row_diff == 0 && col_diff == 2) && (piece > 0 && starting_row == 7 && starting_col == 4 && !white_king_moved))
    {
        if ((final_col == 6 && !white_rook_h1_moved) && (board[7][6] == EMPTY && board[7][5] == EMPTY)) return true;
        if ((final_col == 2 && !white_rook_a1_moved) && (board[7][1] == EMPTY && board[7][2] == EMPTY && board[7][3] == EMPTY)) return true;
        return false;
    }
    else if ((row_diff == 0 && col_diff == 2) && (piece < 0 && starting_row == 0 && starting_col == 4 && !black_king_moved))
    {
        if ((final_col == 6 && !black_rook_h8_moved) && (board[0][5] == EMPTY && board[0][6] == EMPTY)) return true;
        if ((final_col == 2 && !black_rook_a8_moved) && (board[0][3] == EMPTY && board[0][2] == EMPTY && board[0][1] == EMPTY)) return true;
        return false;
    }

    bool is_valid = (row_diff <= 1 && col_diff <= 1) && (row_diff > 0 || col_diff > 0);
    if (!is_valid) return false;

    int final_square = board[final_row][final_col];
    if (piece < 0 && final_square < 0) return false;
    if (piece > 0 && final_square > 0) return false;

    return true;
}

// ============================================================================
// KONTROLA ŠACHU, MATU A NAPADENÝCH POLÍ
// ============================================================================
bool is_square_attacked(int target_row, int target_col, int attacker_color)
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            int piece = board[r][c];

            if ((attacker_color == 1 && piece > 0) || (attacker_color == -1 && piece < 0))
            {
                int abs_piece = abs(piece);

                if (abs_piece == W_PAWN)
                {
                    int dir = (piece > 0) ? -1 : 1;
                    if (r + dir == target_row && (c + 1 == target_col || c - 1 == target_col))
                        return true;
                }
                else if (abs_piece == W_KNIGHT && knight_move(r, c, target_row, target_col, piece)) return true;
                else if (abs_piece == W_BISHOP && bishop_move(r, c, target_row, target_col, piece)) return true;
                else if (abs_piece == W_ROOK   && rook_move(r, c, target_row, target_col, piece))   return true;
                else if (abs_piece == W_QUEEN  && queen_move(r, c, target_row, target_col, piece))  return true;
                else if (abs_piece == W_KING   && abs(r - target_row) <= 1 && abs(c - target_col) <= 1) return true;
            }
        }
    }
    return false;
}

bool is_in_check(int king_color)
{
    int king_piece = (king_color == 1) ? W_KING : B_KING;
    int king_row = -1, king_col = -1;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board[r][c] == king_piece) {
                king_row = r;
                king_col = c;
                break;
            }
        }
    }

    if (king_row == -1) return false;

    int enemy_color = -king_color;
    return is_square_attacked(king_row, king_col, enemy_color);
}

// Ověří, zda má daný hráč alespoň 1 platný legální tah
bool has_legal_moves(int player_color)
{
    for (int start_r = 0; start_r < 8; start_r++)
    {
        for (int start_c = 0; start_c < 8; start_c++)
        {
            int piece = board[start_r][start_c];

            if ((player_color == 1 && piece > 0) || (player_color == -1 && piece < 0))
            {
                for (int target_r = 0; target_r < 8; target_r++)
                {
                    for (int target_c = 0; target_c < 8; target_c++)
                    {
                        bool is_valid_geometry = false;

                        if (piece == W_KNIGHT || piece == B_KNIGHT)
                            is_valid_geometry = knight_move(start_r, start_c, target_r, target_c, piece);
                        else if (piece == W_ROOK || piece == B_ROOK)
                            is_valid_geometry = rook_move(start_r, start_c, target_r, target_c, piece);
                        else if (piece == W_QUEEN || piece == B_QUEEN)
                            is_valid_geometry = queen_move(start_r, start_c, target_r, target_c, piece);
                        else if (piece == W_BISHOP || piece == B_BISHOP)
                            is_valid_geometry = bishop_move(start_r, start_c, target_r, target_c, piece);
                        else if (piece == W_PAWN || piece == B_PAWN)
                            is_valid_geometry = pawn_move(start_r, start_c, target_r, target_c, piece);
                        else if (piece == W_KING || piece == B_KING)
                            is_valid_geometry = king_move(start_r, start_c, target_r, target_c, piece);

                        if (is_valid_geometry)
                        {
                            int temp_target = board[target_r][target_c];
                            int temp_en_passant_victim = EMPTY;
                            int victim_row = -1;

                            if (abs(piece) == W_PAWN && target_r == en_passant_row && target_c == en_passant_col)
                            {
                                victim_row = (piece == W_PAWN) ? target_r + 1 : target_r - 1;
                                temp_en_passant_victim = board[victim_row][target_c];
                                board[victim_row][target_c] = EMPTY;
                            }

                            board[target_r][target_c] = piece;
                            board[start_r][start_c] = EMPTY;

                            bool check_failed = is_in_check(player_color);

                            board[start_r][start_c] = piece;
                            board[target_r][target_c] = temp_target;
                            if (victim_row != -1) board[victim_row][target_c] = temp_en_passant_victim;

                            if (!check_failed) return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

// ============================================================================
// HLAVNÍ SMYČKA MAIN
// ============================================================================
int main()
{
    const int board_size = 800;
    const int squareSize = board_size / 8;
    const int offset = 100;
    const int width = 75;
    const int height = 50;
    const double cord_x = 912.5;
    const double cord_y = 475;
    
    bool is_running = false;

    int selected_row = -1;
    int selected_col = -1;
    
    InitWindow(board_size + 2 * offset, board_size + 2 * offset, "Chess board");
    SetTargetFPS(60);

    load_figure();

    while (!WindowShouldClose())
    {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();

        // Klikání na plochu šachovnice (aktivní jen, pokud hra běžela a NESKONČILA matem)
        if (is_running && !is_game_over && mouseX >= offset && mouseX < offset + board_size &&
            mouseY >= offset && mouseY < offset + board_size) 
        {
            int col = (mouseX - offset) / squareSize;
            int row = (mouseY - offset) / squareSize;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
            {
                if (selected_row == row && selected_col == col)
                {
                    selected_row = -1;
                    selected_col = -1;
                }
                else if (selected_row != -1 && selected_col != -1)
                {
                    int currentPiece = board[selected_row][selected_col];
                    bool is_valid_geometry = false;

                    if (currentPiece == W_KNIGHT || currentPiece == B_KNIGHT)
                        is_valid_geometry = knight_move(selected_row, selected_col, row, col, currentPiece);
                    else if (currentPiece == W_ROOK || currentPiece == B_ROOK)
                        is_valid_geometry = rook_move(selected_row, selected_col, row, col, currentPiece);
                    else if (currentPiece == W_QUEEN || currentPiece == B_QUEEN)
                        is_valid_geometry = queen_move(selected_row, selected_col, row, col, currentPiece);
                    else if (currentPiece == W_BISHOP || currentPiece == B_BISHOP)
                        is_valid_geometry = bishop_move(selected_row, selected_col, row, col, currentPiece);
                    else if (currentPiece == W_PAWN || currentPiece == B_PAWN)
                        is_valid_geometry = pawn_move(selected_row, selected_col, row, col, currentPiece);
                    else if (currentPiece == W_KING || currentPiece == B_KING)
                        is_valid_geometry = king_move(selected_row, selected_col, row, col, currentPiece);

                    if (is_valid_geometry)
                    {
                        int player_color = (currentPiece > 0) ? 1 : -1;
                        
                        int temp_target = board[row][col];
                        int temp_en_passant_victim = EMPTY;
                        int victim_row = -1;

                        if (abs(currentPiece) == W_PAWN && row == en_passant_row && col == en_passant_col)
                        {
                            victim_row = (currentPiece == W_PAWN) ? row + 1 : row - 1;
                            temp_en_passant_victim = board[victim_row][col];
                            board[victim_row][col] = EMPTY;
                        }

                        board[row][col] = currentPiece;
                        board[selected_row][selected_col] = EMPTY;

                        // Proměna pěšce (automaticky na dámu)
                        if (currentPiece == W_PAWN && row == 0) {
                            board[row][col] = W_QUEEN;
                        } else if (currentPiece == B_PAWN && row == 7) {
                            board[row][col] = B_QUEEN;
                        }

                        bool castle_failed = false;
                        if (abs(currentPiece) == W_KING && abs(col - selected_col) == 2)
                        {
                            int step = (col - selected_col > 0) ? 1 : -1;
                            int enemyColor = -player_color;
                            for (int c = selected_col; c != col + step; c += step)
                            {
                                if (is_square_attacked(selected_row, c, enemyColor))
                                {
                                    castle_failed = true;
                                    break;
                                }
                            }
                        }

                        if (is_in_check(player_color) || castle_failed)
                        {
                            // TAH JE NEPLATNÝ -> Vracíme zpět
                            board[selected_row][selected_col] = currentPiece;
                            board[row][col] = temp_target;
                            if (victim_row != -1) board[victim_row][col] = temp_en_passant_victim;

                            TraceLog(LOG_INFO, "Neplatny tah: Kral je nebo by byl v sachu!");
                        }
                        else
                        {
                            // TAH JE PLATNÝ -> Potvrdíme změny
                            if (abs(currentPiece) == W_KING && abs(col - selected_col) == 2)
                            {
                                if (col - selected_col == 2) { board[row][5] = board[row][7]; board[row][7] = EMPTY; }
                                else if (col - selected_col == -2) { board[row][3] = board[row][0]; board[row][0] = EMPTY; }
                            }

                            if (currentPiece == W_KING) white_king_moved = true;
                            if (currentPiece == B_KING) black_king_moved = true;
                            if (currentPiece == W_ROOK && selected_row == 7 && selected_col == 0) white_rook_a1_moved = true;
                            if (currentPiece == W_ROOK && selected_row == 7 && selected_col == 7) white_rook_h1_moved = true;
                            if (currentPiece == B_ROOK && selected_row == 0 && selected_col == 0) black_rook_a8_moved = true;
                            if (currentPiece == B_ROOK && selected_row == 0 && selected_col == 7) black_rook_h8_moved = true;

                            if (abs(currentPiece) == W_PAWN && abs(row - selected_row) == 2)
                            {
                                en_passant_col = col;
                                en_passant_row = (currentPiece == W_PAWN) ? selected_row - 1 : selected_row + 1;
                            }
                            else
                            {
                                en_passant_row = -1;
                                en_passant_col = -1;
                            }

                            // Přepnutí tahu
                            current_turn = -current_turn;

                            // Kontrola MATU / PATU pro nového hráče
                            if (!has_legal_moves(current_turn))
                            {
                                is_game_over = true;
                                if (is_in_check(current_turn))
                                {
                                    game_result = -current_turn;
                                }
                                else
                                {
                                    game_result = 2;
                                }
                            }
                        }
                    }

                    selected_row = -1;
                    selected_col = -1;
                }
                else if (board[row][col] != EMPTY)
                {
                    if ((current_turn == 1 && board[row][col] > 0) ||
                        (current_turn == -1 && board[row][col] < 0))
                    {
                        selected_row = row;
                        selected_col = col;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(GRAY);

        draw_board(offset, board_size, selected_row, selected_col, is_running);
        draw_board_piece(board, offset, squareSize);

        bool mouse_tracker_start = (mouseX >= cord_x && mouseX <= cord_x + width &&
                                    mouseY >= cord_y && mouseY <= cord_y + height);

        if (!is_running)
        {
            DrawRectangle(cord_x, cord_y, width, height, GREEN);
            DrawText("Start", cord_x + 10, cord_y + 15, 20, BLACK);

            if (mouse_tracker_start && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                reset_game(); // Kompletní reset šachovnice při startu
                is_running = true;
            }
        }
        else
        {
            DrawRectangle(cord_x, cord_y, width, height, RED);
            DrawText("End\ngame", cord_x + 15, cord_y + 5, 20, BLACK);

            if (mouse_tracker_start && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                is_running = false;
                selected_row = -1;
                selected_col = -1;
            }
        }

        EndDrawing();
    }

    unload_figure();
    CloseWindow();
    return 0;
}