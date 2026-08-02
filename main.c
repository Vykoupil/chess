#include "raylib.h"

Texture2D white_king, white_queen, white_pawn, white_knight, white_bishop, white_rook;
Texture2D black_king, black_queen, black_pawn, black_knight, black_bishop, black_rook;

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

enum Piece{
    EMPTY = 0,
    W_PAWN = 1, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN , W_KING,
    B_PAWN = -1, B_KNIGHT = -2, B_BISHOP = -3, B_ROOK = -4, B_QUEEN = -5 , B_KING = -6
};

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

void white_figure(int offset, int squareSize, Texture2D king, Texture2D queen, Texture2D pawn, Texture2D knight, Texture2D bishop, Texture2D rook)
{

            //Vykreslování bílích figurek

        //pěšci
        for(int i = 0; i < 8; i++)
        {
            DrawTexture(pawn, offset + i * squareSize, offset + 6 * squareSize, WHITE);
        }

        //věže
        for(int i = 0; i < 8; i+=7)
        {
            DrawTexture(rook, offset + i * squareSize, offset + 7 * squareSize, WHITE);
        }

        //kůň
        for(int i = 1; i < 8; i+=5)
        {
            DrawTexture(knight, offset + i * squareSize, offset + 7 * squareSize, WHITE);
        }

        //střelec
        for(int i = 2; i < 8; i+=3)
        {
            DrawTexture(bishop, offset + i * squareSize, offset + 7 * squareSize, WHITE);
        }

        //dáma
        DrawTexture(queen, offset + 3 * squareSize, offset + 7 * squareSize, WHITE);

        //král
        DrawTexture(king, offset + 4 * squareSize, offset + 7 * squareSize, WHITE);

}

void black_figure(int offset, int squaresize, Texture2D king, Texture2D queen, Texture2D pawn, Texture2D knight, Texture2D bishop, Texture2D rook)
{

    //Vykreslování černých figurek

    //pěšci
    for(int i = 0; i < 8; i++)
    {
        DrawTexture(pawn, offset + i * squaresize, offset + squaresize, WHITE);
    }

    //věže
    for(int i = 0; i < 8; i+=7)
    {
        DrawTexture(rook, offset + i * squaresize, offset, WHITE);
    }

    //koně
    for(int i = 1; i < 8; i+=5)
    {
        DrawTexture(knight, offset + i * squaresize, offset, WHITE);
    }

    //střelec
    for(int i = 2; i < 8; i+=3)
    {
        DrawTexture(bishop, offset + i * squaresize, offset, WHITE);
    }

    //dáma
    DrawTexture(queen, offset + 3 * squaresize, offset, WHITE);

    //král
    DrawTexture(king, offset + 4 * squaresize, offset, WHITE);

}

//funkce na vykreslení šachovnice a figurek
void draw_board(int offset, int board_size)
{
    int selected_row = NULL;
    int selected_col = NULL;

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && GetMouseX() >= offset && GetMouseX() < offset + board_size && GetMouseY() >= offset && GetMouseY() < offset + board_size)
    {
        if ()
        {
            /* code */
        }
        
    }

    bool is_select = false;
    int squareSize = board_size / 8; // Každé políčko bude mít přesně 100x100 pixelů

    // Cyklus pro 8 řádků (shora dolů)
    for (int r = 0; r < 8; r++)
    {
        // Cyklus pro 8 sloupců (zleva doprava)
        for (int c = 0; c < 8; c++)
        {
            int x = offset + c * squareSize;
            int y = offset + r * squareSize;

            bool mouse_tracker_square = (GetMouseX() > x && GetMouseX() <= x + squareSize && GetMouseY() > y && GetMouseY() <= y + squareSize);

            int square_status = board[r][c];

            if (square_status != 0 && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && mouse_tracker_square)
            {
                DrawRectangle(x, y, squareSize, squareSize, YELLOW);                
            }
            
            // Střídání barev políček
            if ((r + c) % 2 == 0)
            {
                // Světlá políčka
                DrawRectangle(x, y, squareSize, squareSize, BEIGE);

            }
            else
            {
                // Tmavá políčka
                DrawRectangle(x, y, squareSize, squareSize, BROWN);
            }
        }
    }

    //Nadpis
    DrawText("Chess Board", board_size / 2 - 50, board_size / 2 - 400, 40, BLACK);
            
    // Název pole
    char chars[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    int numbers[] = {8, 7, 6, 5, 4, 3, 2, 1};

    for(int i = 0; i < 8; i++)
    {
        DrawText(TextFormat("%c", chars[i]), offset + i * squareSize + squareSize / 2 - 10, offset + board_size + 10, 20, BLACK);
        DrawText(TextFormat("%d", numbers[i]), offset - 30, offset + i * squareSize + squareSize / 2 - 10, 20, BLACK);
    }

}

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
                case W_PAWN:
                    DrawTexture(white_pawn, x, y, WHITE);
                    break;

                case W_KNIGHT:
                    DrawTexture(white_knight, x, y, WHITE);
                    break;

                case W_BISHOP:
                    DrawTexture(white_bishop, x, y, WHITE);
                    break;

                case W_ROOK:
                    DrawTexture(white_rook, x, y, WHITE);
                    break;

                case W_QUEEN:
                    DrawTexture(white_queen, x, y, WHITE);
                    break;

                case W_KING:
                    DrawTexture(white_king, x, y, WHITE);
                    break;

                case B_PAWN:
                    DrawTexture(black_pawn, x, y, WHITE);
                    break;

                case B_KNIGHT:
                    DrawTexture(black_knight, x, y, WHITE);
                    break;

                case B_BISHOP:
                    DrawTexture(black_bishop, x, y, WHITE);
                    break;

                case B_ROOK:
                    DrawTexture(black_rook, x, y, WHITE);
                    break;

                case B_QUEEN:
                    DrawTexture(black_queen, x, y, WHITE);
                    break;

                case B_KING:
                    DrawTexture(black_king, x, y, WHITE);
                    break;

                default:
                    break;
            }
        }
    }
}

int main()
{
    const int board_size = 800;
    const int squareSize = board_size / 8; // Každé políčko bude mít přesně 100x100 pixelů
    const int offset = 100;
    const int width = 75;
    const int height = 50;
    const double cord_x = 912.5;
    const double cord_y = 475;
    bool is_running = false;
    
    InitWindow(board_size + 2 * offset, board_size + 2 * offset, "Chess board");
    SetTargetFPS(60);

    //načtení figurek
    load_figure();

    //hlavní smyčka programu
    while(!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(GRAY); // Základní pozadí okna

        // Získání souřadnic myši
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();

        // Ověření, zda je kliknutí uvnitř šachovnice
        if (mouseX >= offset && mouseX < offset + board_size &&
            mouseY >= offset && mouseY < offset + board_size) 
        {
            // Převod na indexy pole (0 až 7)
            int col = (mouseX - offset) / squareSize; // Sloupec (0 = A, 7 = H)
            int row = (mouseY - offset) / squareSize; // Řádek (0 = 8. řada, 7 = 1. řada)

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
            {
                // Získání identifikátoru figurky na kliknutém poli
                int clickedPiece = board[row][col];
                
                // Získání šachové notace (např. "E2")
                char colChar = 'A' + col;
                int rowNumber = 8 - row;

                TraceLog(LOG_INFO, "Kliknuto na pole [%d][%d] (%c%d) - Figura ID: %d", row, col, colChar, rowNumber, clickedPiece);
            }
        }

        draw_board(offset, board_size);
        draw_board_piece(board, offset, squareSize);

        bool mouse_tracker_start = (mouseX >= cord_x && mouseX <= cord_x + width &&
                                    mouseY > cord_y && mouseY <= cord_y + height);

        //tlačítko START

        //ověření že bylo stisknuto tlačítko START
        if (!is_running)
        {
            DrawRectangle(cord_x, cord_y, width, height, GREEN);
            DrawText("Start", cord_x + 10, cord_y + 15, 20, BLACK);

            if (mouse_tracker_start && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                is_running = true;
            }
        }else
        {
            DrawRectangle(cord_x, cord_y, width, height, RED);
            DrawText("End\ngame", cord_x + 15, cord_y + 5, 20, BLACK);

            if(mouse_tracker_start && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
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