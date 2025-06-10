#include "raylib.h"
#include <vector>
#include <cmath>
#include "Box.h"
#include "Grid.h"

// Yeni eklenen satır:
#include "nonogramAlgorithm.h"

// -------------------- Ayarlar / Settings -------------------------
const int   BOX_COUNT = 25;           // <––– Sadece bunu değiştirin!
const float BORDER_THICK = 2.0f;      // Çerçeve kalınlığı

// Beklenen id matrisini burada tutuyoruz:
bool expectedIds[5][5] = { { 1, 0, 1, 1, 1 },
                           { 1, 1, 1, 0, 1 },
                           { 1, 1, 0, 0, 1 },
                           { 1, 0, 0, 0, 0 },
                           { 0, 0, 0, 0, 1 }, };
bool noMatch = false;
Color color = WHITE;

// test, yyz, xzy, gridy artık nonogramAlgorithm.cpp içinde tanımlı.
// Burada hiçbir yerde tekrar int yyz[5][2] veya void test(...) yok.


// ------------------------------ main -----------------------------
int main()
{
    // Başlangıçta pencere oluştur, yeniden boyutlandırılabilir
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Responsive Expanding Boxes");
    SetTargetFPS(60);

    // Kutular ve grid ölçüleri saklanacak
    std::vector<Box> boxes;
    int cols, rows;
    GenerateBoxes(BOX_COUNT, boxes, cols, rows);

    while (!WindowShouldClose())
    {
        // 1) Girdi + Animasyon Güncellemesi
        for (auto& b : boxes)
        {
            HandleInput(b);
            UpdateBox(b);
        }

        // 2) Dinamik Grid & Border Hesaplaması (Her Karede)
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();
        float pitch = GHOST_SIZE.x + CELL_GAP;

        // Grid’in net genişlik ve yüksekliği:
        float gridW = cols * pitch - CELL_GAP;
        float gridH = rows * pitch - CELL_GAP;

        // Izgarayı pencere ortasına yerleştirmek için başlangıç noktaları:
        float startX0 = (screenW - gridW) / 2 + GHOST_SIZE.x / 2;
        float startY0 = (screenH - gridH) / 2 + GHOST_SIZE.y / 2;

        // Her kutu için güncellenmiş merkez koordinatları:
        for (auto& b : boxes)
        {
            b.center.x = startX0 + b.gridX * pitch;
            b.center.y = startY0 + b.gridY * pitch;
        }

        // Playground (border) koordinatları:
        Rectangle playground = {
            (screenW - gridW) / 2 - CELL_GAP / 2,
            (screenH - gridH) / 2 - CELL_GAP / 2,
            gridW + CELL_GAP,
            gridH + CELL_GAP
        };

        noMatch = false;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (expectedIds[i][j] != boxes[j + i * 5].expanding) {
                    noMatch = true;
                }
            }
        }
        if (!noMatch) color = GREEN;
        else color = WHITE;

        // 3) Çizim
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText(TextFormat("2 \n1"), playground.x + 50, playground.y - 80, 30, RED);
        DrawText(TextFormat("3"), playground.x + 170, playground.y - 80, 30, RED);
        DrawText(TextFormat("5"), playground.x + 290, playground.y - 80, 30, RED);
        DrawText(TextFormat("2"), playground.x + 410, playground.y - 80, 30, RED);
        DrawText(TextFormat("1 \n1"), playground.x + 530, playground.y - 80, 30, RED);

        DrawText(TextFormat("1 1 1"), playground.x - 60, playground.y + 40, 30, RED);
        DrawText(TextFormat(" 3"), playground.x - 60, playground.y + 160, 30, RED);
        DrawText(TextFormat(" 3"), playground.x - 60, playground.y + 280, 30, RED);
        DrawText(TextFormat(" 3"), playground.x - 60, playground.y + 400, 30, RED);
        DrawText(TextFormat("1 1 1"), playground.x - 60, playground.y + 520, 30, RED);

        // Beyaz çerçeve (playground)
        DrawRectangleLinesEx(playground, BORDER_THICK, WHITE);

        // Ghost (hover) alanları, ardından gerçek kutular
        for (const auto& b : boxes) DrawGhostIfHover(b);
        for (const auto& b : boxes) DrawBox(b);

        EndDrawing();

    }

    CloseWindow();
    return 0;
}
