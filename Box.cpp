#include "Box.h"
#include <cmath>

// Float clamp
float ClampF(float v, float lo, float hi)
{
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

// Ghost (tetik) alan: kutunun merkezinden GHOST_SIZE/2 ötede
Rectangle GetGhost(const Box& b)
{
    return {
        b.center.x - GHOST_SIZE.x / 2,
        b.center.y - GHOST_SIZE.y / 2,
        GHOST_SIZE.x,
        GHOST_SIZE.y
    };
}

// Gerçek kutu dikdörtgeni (size × size) b.center’ı sol-üst köşe kabul eder
Rectangle GetRect(const Box& b)
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
    DrawRectanglePro(rec, ori, 0.0f, WHITE);
}

// Fare hover’ında ghost alanını çiz
void DrawGhostIfHover(const Box& b)
{
    if (CheckCollisionPointRec(GetMousePosition(), GetGhost(b)))
        DrawRectangleRec(GetGhost(b), Color{ 30, 30, 30, 50 });
}
