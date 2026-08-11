#include "raylib.h"
#include <stdlib.h>

Texture2D white_king, white_queen, white_pawn, white_knight, white_bishop, white_rook;
Texture2D black_king, black_queen, black_pawn, black_knight, black_bishop, black_rook;

// Funkce pro načtení textur figurek do paměti GPU
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

// Funkce pro uvolnění textur figurek z paměti GPU
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

// Definice typů figurek
enum Piece{
    EMPTY = 0,
    W_PAWN = 1, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN , W_KING,
    B_PAWN = -1, B_KNIGHT = -2, B_BISHOP = -3, B_ROOK = -4, B_QUEEN = -5 , B_KING = -6
};

// Inicializace šachovnice s počátečními pozicemi figurek
int board[8][8] = {
    {-4, -2, -3, -5, -6, -3, -2, -4},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1 ,1 ,1 ,1 ,1 ,1 ,1},
    {4 ,2 ,3 ,5 ,6 ,3 ,2 ,4}
};

// Funkce na vykreslení šachovnice a případného výběru
void draw_board(int offset, int board_size, int selected_row, int selected_col, bool is_running)
{
    int squareSize = board_size / 8;
    

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            int x = offset + c * squareSize;
            int y = offset + r * squareSize;

            // Střídání barev políček
            if ((r + c) % 2 == 0)
            {
                DrawRectangle(x, y, squareSize, squareSize, BEIGE);
            }
            else
            {
                DrawRectangle(x, y, squareSize, squareSize, BROWN);
            }

            // Zvýraznění vybraného pole
            if(is_running)
            {
                if (r == selected_row && c == selected_col && board[r][c] > 0)
                {
                    DrawRectangle(x, y, squareSize, squareSize, ColorAlpha(YELLOW, 0.6f));
                    DrawRectangleLinesEx((Rectangle){ (float)x, (float)y, (float)squareSize, (float)squareSize }, 3, YELLOW);
                }else if (r == selected_row && c == selected_col && board[r][c] < 0)
                {
                    DrawRectangle(x, y, squareSize, squareSize, ColorAlpha(RED, 0.6f));
                    DrawRectangleLinesEx((Rectangle){ (float)x, (float)y, (float)squareSize, (float)squareSize }, 3, RED);
                }else{}
            }
        }
    }

    // Nadpis
    DrawText("Chess Board", board_size / 2 - 50, board_size / 2 - 400, 40, BLACK);
            
    // Název polí (A-H, 1-8)
    char chars[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    int numbers[] = {8, 7, 6, 5, 4, 3, 2, 1};

    for(int i = 0; i < 8; i++)
    {
        DrawText(TextFormat("%c", chars[i]), offset + i * squareSize + squareSize / 2 - 10, offset + board_size + 10, 20, BLACK);
        DrawText(TextFormat("%d", numbers[i]), offset - 30, offset + i * squareSize + squareSize / 2 - 10, 20, BLACK);
    }
}

// Vykreslení figurek podle stavu v matici board[8][8]
void draw_board_piece(int board[8][8], int offset, int squareSize)
{
    for(int r = 0; r < 8; r++)
    {
        for(int c = 0; c < 8; c++)
        {
            int figure = board[r][c];
            int x = offset + c * squareSize;
            int y = offset + r * squareSize;

            switch(figure)
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

bool knight_move(int starting_row, int starting_col, int final_row, int final_col, int piece)
{
    //rozdíl políček mezi startem a cílem
    int row_diff = abs(final_row - starting_row);
    int col_diff = abs(final_col - starting_col);

    // Kontrola, zda je pohyb ve tvaru "L" (2 políčka v jednom směru a 1 políčko v druhém směru)
    bool is_valid = (row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2);

    if(!is_valid)
    {
        return false;
    }

    int final_square = board[final_row][final_col];

    if(piece > 0 && final_square > 0)
    {
        return false; // Nelze jít na políčko s figurkou stejné barvy
    }
    else if(piece < 0 && final_square < 0)
    {
        return false; // Nelze jít na políčko s figurkou stejné barvy
    }else
    {
        return true;
    }
}

bool rook_move(int starting_row, int starting_col, int final_row, int final_col, int piece)
{
    int row_diff = abs(final_row - starting_row);
    int col_diff = abs(final_col - starting_col);

    bool is_valid = (row_diff == 0 && col_diff > 0) || (col_diff == 0 && row_diff > 0);

    if (!is_valid)
    {
        return false;
    }

    int row_step = 0;

    // zajištění aby neělo skákat přes figurky
    if(final_row > starting_row)
    {
        row_step = 1;
    }else if(final_row < starting_row)
    {
        row_step = -1;
    }else{}

    int col_step = 0;

    if (final_col > starting_col)
    {
        col_step = 1;
    }else if(final_col < starting_col)
    {
        col_step = -1;
    }else{}

    int current_row = starting_row + row_step;   
    int current_col = starting_col + col_step;
    
    while(current_row != final_row || current_col != final_col)
    {
        if (board[current_row][current_col] != EMPTY)
        {
            return false;
        }
        
        current_row += row_step;
        current_col += col_step;
    }

    // ověření že neseberu mojí figurku
    int final_square = board[final_row][final_col];

    if (piece > 0 && final_square > 0)
    {
        return false;
    }else if(piece < 0 && final_square < 0)
    {
        return false;
    }else
    {
        return true;
    }
    
    
}

bool queen_move(int starting_row, int starting_col, int final_row, int final_col, int piece)
{
    int row_diff = abs(final_row - starting_row);
    int col_diff = abs(final_col - starting_col);

    bool is_valid = (row_diff == 0 && col_diff > 0) || (row_diff > 0 && col_diff == 0) || (row_diff == col_diff);

    if (!is_valid)
    {
        return false;
    }

    // nemužu skákat přes figurky
    int row_step = 0;

    if (final_row > starting_row)
    {
        row_step = 1;
    }else if(final_row < starting_row)
    {
        row_step = -1;
    }else{}

    int col_step = 0;
    
    if (final_col > starting_col)
    {
        col_step = 1;
    }else if(final_col < starting_col)
    {
        col_step = -1;
    }else{}

    int current_row = starting_row + row_step;
    int current_col = starting_col + col_step;
    
    while(current_row != final_row || current_col != final_col)
    {
        if(board[current_row][current_col] != EMPTY)
        {
            return false;
        }
        
        current_row += row_step;
        current_col += col_step;
    }
    
    // ověření že neberu vlastní figurku
    int final_square = board[final_row][final_col];

    if (piece > 0 && final_square > 0)
    {
        return false;
    }else if(piece < 0 && final_square < 0)
    {
        return false;
    }else
    {
        return true;
    }
}

bool bishop_move(int starting_row, int starting_col, int final_row, int final_col, int piece)
{
    int row_diff = abs(final_row - starting_row);
    int col_diff = abs(final_col - starting_col);

    bool is_valid = (row_diff == col_diff);

    if (!is_valid)
    {
        return false;
    }
    
    int row_step = 0;

    if (final_row > starting_row)
    {
        row_step = 1;
    }else if(final_row < starting_row)
    {
        row_step = -1;
    }else{}

    int col_step = 0;
    
    if (final_col > starting_col)
    {
        col_step = 1;
    }else if(final_col < starting_col)
    {
        col_step = -1;
    }else{}

    int current_row = starting_row + row_step;
    int current_col = starting_col + col_step;

    while(current_row != final_row || current_col != final_col)
    {
        if (board[current_row][current_col] != EMPTY)
        {
            return false;
        }
        
        current_row += row_step;
        current_col += col_step;
    }

    int final_square = board[final_row][final_col];

    if (piece > 0 && final_square > 0)
    {
        return false;
    }else if(piece < 0 && final_square < 0)
    {
        return false;
    }else
    {
        return true;
    }
}

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

    // Proměnné pro uložení výběru pole. 
    // Nastavíme na -1 (žádné pole od 0 do 7 není vybrané).
    int selected_row = -1;
    int selected_col = -1;
    
    InitWindow(board_size + 2 * offset, board_size + 2 * offset, "Chess board");
    SetTargetFPS(60);

    load_figure();

    // Hlavní herní smyčka
    while(!WindowShouldClose())
    {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();

        // -------------------------------------------------------------
        // 1. REAKCE NA KLIKNUTÍ MYŠÍ (Logika hry)
        // -------------------------------------------------------------
        
        // Zkontrolujeme, zda je hra spuštěná A ZÁROVEŇ kurzor stojí nad šachovnicí
        if (is_running && mouseX >= offset && mouseX < offset + board_size &&
            mouseY >= offset && mouseY < offset + board_size) 
        {
            // Přepočet souřadnic pixelů na indexy políčka (0 až 7)
            int col = (mouseX - offset) / squareSize;
            int row = (mouseY - offset) / squareSize;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
            {
                // SCÉNÁŘ A: Klikli jsme znovu na již vybranou figurku -> ZRUŠIT VÝBĚR
                if (selected_row == row && selected_col == col)
                {
                    selected_row = -1;
                    selected_col = -1;
                }
                
                // SCÉNÁŘ B: Už máme vybranou figurku a klikáme na JINÉ pole -> POKUS O TAH
                else if (selected_row != -1 && selected_col != -1)
                {
                    // Zjistíme, jaká figurka stojí na původně vybraném poli
                    int currentPiece = board[selected_row][selected_col];

                    // Zkontrolujeme, zda se jedná o KONĚ (Bílý = 2 [W_KNIGHT], Černý = -2 [B_KNIGHT])
                    if (currentPiece == W_KNIGHT || currentPiece == B_KNIGHT)
                    {
                        // Zavoláme naši funkci a ověříme, zda je tah koněm platný
                        if (knight_move(selected_row, selected_col, row, col, currentPiece))
                        {
                            // TAH JE PLATNÝ -> Provedeme změnu v poli board[8][8]:
                            board[row][col] = currentPiece;        // 1. Dáme koně na nové pole
                            board[selected_row][selected_col] = EMPTY; // 2. Staré pole vyprázdníme

                            TraceLog(LOG_INFO, "Knight moves to the [%d][%d]", row, col);
                        }
                    }else if (currentPiece == W_ROOK || currentPiece == B_ROOK)
                    {
                        if (rook_move(selected_row, selected_col, row, col, currentPiece))
                        {
                            board[row][col] = currentPiece;
                            board[selected_row][selected_col] = EMPTY;

                            TraceLog(LOG_INFO, "Rook moves to the [%d][%d]", row, col);
                        } 
                    }else if(currentPiece == W_QUEEN ||currentPiece == B_QUEEN)
                    {
                        if (queen_move(selected_row, selected_col, row, col, currentPiece))
                        {
                            board[row][col] = currentPiece;
                            board[selected_row][selected_col] = EMPTY;

                            TraceLog(LOG_INFO, "Queen moves to the [%d][%d]", row, col);
                        }
                        
                    }else if(currentPiece == W_BISHOP || currentPiece == B_BISHOP)
                    {
                        if (bishop_move(selected_row, selected_col, row, col, currentPiece))
                        {
                            board[row][col] = currentPiece;
                            board[selected_row][selected_col] = EMPTY;

                            TraceLog(LOG_INFO, "Biship moves to the [%d][%d]", row, col);
                        }
                        
                    }


                    

                    // Ať už byl tah platný nebo ne, po pokusu o přesun výběr odznačíme
                    selected_row = -1;
                    selected_col = -1;
                }
                
                // SCÉNÁŘ C: Nemáme nic vybráno a klikli jsme na pole s figurkou -> VYBRAT FIGURKU
                else if (board[row][col] != EMPTY)
                {
                    selected_row = row;
                    selected_col = col;
                }
            }
        }

        // -------------------------------------------------------------
        // 2. VYKRESLOVÁNÍ SCÉNY NA OBRAZOVKU
        // -------------------------------------------------------------
        BeginDrawing();
        ClearBackground(GRAY);

        // A) Vykreslení desky a zeleného zvýraznění vybrané figurky
        draw_board(offset, board_size, selected_row, selected_col, is_running);

        // B) Vykreslení figurek navrch desky
        draw_board_piece(board, offset, squareSize);

        // C) Tlačítko START / END GAME
        bool mouse_tracker_start = (mouseX >= cord_x && mouseX <= cord_x + width &&
                                    mouseY >= cord_y && mouseY <= cord_y + height);

        if (!is_running)
        {
            // Tlačítko START
            DrawRectangle(cord_x, cord_y, width, height, GREEN);
            DrawText("Start", cord_x + 10, cord_y + 15, 20, BLACK);

            if (mouse_tracker_start && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                is_running = true;
            }
        }
        else
        {
            // Tlačítko END GAME
            DrawRectangle(cord_x, cord_y, width, height, RED);
            DrawText("End\ngame", cord_x + 15, cord_y + 5, 20, BLACK);

            if (mouse_tracker_start && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                is_running = false;
                
                // Při ukončení hry vyčistíme výběr
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