#include "raylib.h"

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

    // Proměnné pro držení vybraného pole (-1 = žádné není vybrané)
    int selected_row = -1;
    int selected_col = -1;
    
    InitWindow(board_size + 2 * offset, board_size + 2 * offset, "Chess board");
    SetTargetFPS(60);

    load_figure();

    while(!WindowShouldClose())
    {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();

        // KLIKÁNÍ:
        if (mouseX >= offset && mouseX < offset + board_size &&
            mouseY >= offset && mouseY < offset + board_size) 
        {
            // Převod souřadnic myši na indexy políčka
            int col = (mouseX - offset) / squareSize;
            int row = (mouseY - offset) / squareSize;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
            {
                // Pokud kliknu na již vybranou figurku, zruším výběr
                if (selected_row == row && selected_col == col)
                {
                    selected_row = -1;
                    selected_col = -1;
                }
                // Vyberu figurku pouze pokud na políčku NENÍ prázdno (board[row][col] != 0)
                else if (board[row][col] != 0)
                {
                    selected_row = row;
                    selected_col = col;

                    char colChar = 'A' + col;
                    int rowNumber = 8 - row;
                    TraceLog(LOG_INFO, "Vybrana figurka [%d][%d] (%c%d) - Figura ID: %d", row, col, colChar, rowNumber, board[row][col]);
                }
            }
        }

        // VYKRESLOVÁNÍ SCÉNY:
        BeginDrawing();
        ClearBackground(GRAY);

        //šachovnice + zelné zvýraznění vybraného pole
        draw_board(offset, board_size, selected_row, selected_col, is_running);

        //figurky navrch
        draw_board_piece(board, offset, squareSize);

        // 3. Tlačítko START / END GAME
        bool mouse_tracker_start = (mouseX >= cord_x && mouseX <= cord_x + width &&
                                    mouseY >= cord_y && mouseY <= cord_y + height);

        if (!is_running)
        {
            DrawRectangle(cord_x, cord_y, width, height, GREEN);
            DrawText("Start", cord_x + 10, cord_y + 15, 20, BLACK);

            if (mouse_tracker_start && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                is_running = true;
            }
        }
        else
        {
            DrawRectangle(cord_x, cord_y, width, height, RED);
            DrawText("End\ngame", cord_x + 15, cord_y + 5, 20, BLACK);

            if(mouse_tracker_start && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                is_running = false;
            }
        }

        EndDrawing();
    }

    unload_figure();
    CloseWindow();
    return 0;
}