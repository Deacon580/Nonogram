#include "raylib.h"
#include <vector>
#include <cmath>
#include "Box.h"
#include "Grid.h"

#include "nonogramAlgorithm.h"


const int   BOX_COUNT = 36;
const float BORDER_THICK = 2.0f;

bool expectedIds[6][6] = { { 1, 1, 0, 1, 1, 1 },
                           { 1, 0, 1, 0, 0, 1 },
                           { 1, 0, 0, 0, 1, 1 },
                           { 1, 0, 1, 0, 0, 0 },
                           { 0, 0, 1, 0, 0, 0 },
                           { 1, 1, 0, 0, 1, 1 } };
bool noMatch = false;
Color color = WHITE;

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Responsive Expanding Boxes");
    SetTargetFPS(60);

    std::vector<Box> boxes;
    int cols, rows;
    GenerateBoxes(BOX_COUNT, boxes, cols, rows);

    while (!WindowShouldClose())
    {
        for (auto& b : boxes)
        {
            HandleInput(b);
            UpdateBox(b);
        }

        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();
        float pitch = GHOST_SIZE.x + CELL_GAP;

        float gridW = cols * pitch - CELL_GAP;
        float gridH = rows * pitch - CELL_GAP;

        float startX0 = (screenW - gridW) / 2 + GHOST_SIZE.x / 2;
        float startY0 = (screenH - gridH) / 2 + GHOST_SIZE.y / 2;

        for (auto& b : boxes)
        {
            b.center.x = startX0 + b.gridX * pitch;
            b.center.y = startY0 + b.gridY * pitch;
        }

        Rectangle playground = {
            (screenW - gridW) / 2 - CELL_GAP / 2,
            (screenH - gridH) / 2 - CELL_GAP / 2,
            gridW + CELL_GAP,
            gridH + CELL_GAP
        };

        noMatch = false;
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                if (expectedIds[i][j] != boxes[j + i * 6].expanding) {
                    noMatch = true;
                }
            }
        }
        if (!noMatch) color = GREEN;
        else color = WHITE;

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText(TextFormat("4\n1"), playground.x + 50, playground.y - 80, 30, RED);
        DrawText(TextFormat("1\n1"), playground.x + 170, playground.y - 80, 30, RED);
        DrawText(TextFormat("1\n2"), playground.x + 290, playground.y - 80, 30, RED);
        DrawText(TextFormat("1"), playground.x + 410, playground.y - 50, 30, RED);
        DrawText(TextFormat("1\n1\n1"), playground.x + 530, playground.y - 110, 30, RED);
        DrawText(TextFormat("3\n1"), playground.x + 650, playground.y - 80, 30, RED);

        DrawText(TextFormat("2 3"), playground.x - 60, playground.y + 40, 30, RED);
        DrawText(TextFormat("1 1 1"), playground.x - 90, playground.y + 160, 30, RED);
        DrawText(TextFormat("1 2"), playground.x - 60, playground.y + 280, 30, RED);
        DrawText(TextFormat("1 1"), playground.x - 60, playground.y + 400, 30, RED);
        DrawText(TextFormat("1"), playground.x - 30, playground.y + 520, 30, RED);
        DrawText(TextFormat("2 2"), playground.x - 60, playground.y + 640, 30, RED);

        DrawRectangleLinesEx(playground, BORDER_THICK, WHITE);

        for (const auto& b : boxes) DrawGhostIfHover(b);
        for (const auto& b : boxes) DrawBox(b);

        EndDrawing();

        if (IsKeyPressed(KEY_U)) {
            test(yyz, xzy, gridy);
        }
    }

    CloseWindow();
    return 0;
}
