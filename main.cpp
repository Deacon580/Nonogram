/********************************************************************
*  Multiple Expanding Boxes – Responsive Grid & Centered Border
*
*  • Pencere yeniden boyutlandırıldığında kutular ızgarada merkezlenir
*  • GRID hücre konumları saklanır; her karede pencere boyutuna göre yeniden hesaplanır
*  • CELL_GAP değerini değiştirince border da otomatik güncellenir
********************************************************************/

#include "raylib.h"
#include <vector>
#include <iostream>
#include <cmath>  
#include "NonogramSolver.cpp"

// -------------------- Ayarlar / Settings -------------------------
const int   BOX_COUNT = 36;           // <––– Sadece bunu değiştirin!
const float BOX_MAX_SIZE = 100.0f;
const float BOX_MIN_SIZE = 0.0f;
const float BOX_START_SPEED = 10.0f;
const Vector2 GHOST_SIZE = { 100, 100 };
// Hücre merkezden merkeze toplam mesafe = GHOST_SIZE.x + CELL_GAP
// Border boyutu da CELL_GAP’e bağlı olarak otomatik çalışır
const float  CELL_GAP = 20.0f;    // Örneğin 50, 20, 10 vb.
const float  BORDER_THICK = 2.0f;     // Çerçeve kalınlığı

bool expectedIds[6][6] = { { 1, 1, 0, 1, 1, 1 },
                           { 1, 0, 1, 0, 0, 1 },
                           { 1, 0, 0, 0, 1, 1 },
                           { 1, 0, 1, 0, 0, 0 },
                           { 0, 0, 1, 0, 0, 0 },
                           { 1, 1, 0, 0, 1, 1 } };
bool noMatch = false;
Color color = WHITE;

// -------------------- Veri Yapısı / Data Struct ------------------
struct Box
{
    int     gridX;      // Satır içi sütun indeksi (0…cols-1)
    int     gridY;      // Satır indeksi (0…rows-1)
    Vector2 center;     // Her karede yeniden hesaplanacak ekran koordinatı
    float   size;       // Animasyon boyutu (kenar uzunluğu)
    float   speed;      // Anlık hız
    bool    expanding;  // true = büyüyor, false = küçülüyor
};

// Float clamp
static float ClampF(float v, float lo, float hi)
{
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

int yyz[5][2] = { { 3, 1 }, {3, 1}, { 3, 1 }, {3, 1}, { 3, 1 } };
int xzy[5] = { { 4 }, { 4 }, { 4 }, { 4 }, { 4 } };
int gridy[5][5] = { 0 };

void test(int y[5][2], int x[5], int grid[5][5]) {
    int t = 0;
    int r = 0;

    for (int k = 0; k < 5; k++) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < y[k][i]; j++) {
                grid[t][r] = 1;
                t++;
            }
            if (t < 5) {
                grid[t][r] = 0;
                t++;
            }
            else t = 0;
        }
        r++;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            std::cout << grid[j][i] << ", ";
        }
        std::cout << "\n";
    }
}

// -------------------- Yardımcı İşlevler --------------------------
// Ghost (tetik) alanı: kutunun merkezinden GHOST_SIZE/2 ötede
inline Rectangle GetGhost(const Box& b)
{
    return {
        b.center.x - GHOST_SIZE.x / 2,
        b.center.y - GHOST_SIZE.y / 2,
        GHOST_SIZE.x,
        GHOST_SIZE.y
    };
}

// Gerçek kutu dikdörtgeni (size × size) b.center’ı sol-üst köşe kabul eder
inline Rectangle GetRect(const Box& b)
{
    return {
        b.center.x,
        b.center.y,
        b.size,
        b.size
    };
}

// Yeni Box başlat (gridX ve gridY önden ayarlanmalı)
void InitBox(Box& b)
{
    b.size = BOX_MIN_SIZE;
    b.speed = BOX_START_SPEED;
    b.expanding = false;
}

// Fare tıklama mantığı
void HandleInput(Box& b)
{
    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse, GetGhost(b)) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        b.expanding = !b.expanding;
        b.speed = BOX_START_SPEED; // her tıklamada taze hız
    }
}

// Animasyonu güncelle
void UpdateBox(Box& b)
{
    b.size += (b.expanding ? b.speed : -b.speed);
    b.size = ClampF(b.size, BOX_MIN_SIZE, BOX_MAX_SIZE);

    if (b.speed > 1.0f) b.speed -= 0.2f;
    else                b.speed = 1.0f;

    if (b.size == BOX_MIN_SIZE || b.size == BOX_MAX_SIZE)
        b.speed = BOX_START_SPEED;
}

// Kutuyu merkezden çiz (size/2 ofsetli)
void DrawBox(const Box& b)
{
    Rectangle rec = GetRect(b);
    Vector2   ori = { rec.width / 2, rec.height / 2 };
    DrawRectanglePro(rec, ori, 0.0f, color);
}

// Fare hover’ında ghost alanını çiz
void DrawGhostIfHover(const Box& b)
{
    if (CheckCollisionPointRec(GetMousePosition(), GetGhost(b)))
        DrawRectangleRec(GetGhost(b), Color{ 30, 30, 30, 50 });
}

// ---------------- GenerateBoxes (Grid Hesaplama) -----------------
// requestedCount: yerleştirilmek istenen kutu sayısı
// Çıktı olarak boxes’a gridX, gridY atar (center henüz hesaplanmayacak)
// Maksimum satır kısıtlaması kaldırıldı: satır sayısı = ceil(count/cols)
void GenerateBoxes(int requestedCount, std::vector<Box>& boxes,
    int& outCols, int& outRows)
{
    boxes.clear();
    boxes.reserve(requestedCount);

    // Pencere boyutundan bağımsız, yalnızca count ve CELL_GAP tabanlı hesap:
    // pitch = hücre merkezden merkeze mesafe
    float pitch = GHOST_SIZE.x + CELL_GAP;

    // Önce sütun sayısını bul: ekrana sığabilecek en fazla sütun (maxCols)
    // Not: generateBoxes çağırılırken pencere boyutuna bakmamıza gerek yok; 
    // outCols/outRows, sadece “kutuları kaç x kaç ızgarada saklayacağımızı” belirler.
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();
    int maxCols = std::max(1, (int)std::floor((screenW + CELL_GAP) / pitch));
    int cols = std::min(requestedCount, maxCols);
    int rows = (requestedCount + cols - 1) / cols; // ceil(requestedCount/cols)

    // Eğer satır sayısı dikeyde taşarsa, satırı düşür, sütunu yeniden hesapla:
    int maxRows = std::max(1, (int)std::floor((screenH + CELL_GAP) / pitch));
    if (rows > maxRows)
    {
        rows = maxRows;
        cols = (int)std::ceil(requestedCount / (float)rows);
        cols = ClampF(cols, 1, maxCols);
        rows = (requestedCount + cols - 1) / cols;
    }

    // Şimdi cols×rows, kullanılan ızgara boyutu:
    outCols = cols;
    outRows = rows;

    // Her bir kutunun gridX/gridY’si:
    // Son satır eksik kutu olabilir, ama gridX/gridY sıralı atanır
    int placed = 0;
    for (int r = 0; r < rows; ++r)
    {
        int remaining = requestedCount - placed;
        int rowBoxes = std::min(cols, remaining);
        for (int c = 0; c < rowBoxes; ++c)
        {
            Box b;
            b.gridX = c;
            b.gridY = r;
            InitBox(b);
            boxes.push_back(b);
            placed++;
        }
    }
}

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
        // “+ GHOST_SIZE/2” kutunun merkezini hizalamak için
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
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                if (expectedIds[i][j] != boxes[j + i * 6].expanding) {
                    noMatch = true;
                }
            }
        }
        if (!noMatch) color = GREEN;
        else color = WHITE;

        // 3) Çizim
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

        // Beyaz çerçeve (playground)
        DrawRectangleLinesEx(playground, BORDER_THICK, WHITE);

        // Ghost (hover) alanları, ardından gerçek kutular
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
