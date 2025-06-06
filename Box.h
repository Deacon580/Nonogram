#ifndef BOX_H
#define BOX_H

#include "raylib.h"

static const float   BOX_MAX_SIZE = 100.0f;
static const float   BOX_MIN_SIZE = 0.0f;
static const float   BOX_START_SPEED = 10.0f;
static const Vector2 GHOST_SIZE = { 100, 100 };

float ClampF(float v, float lo, float hi);

struct Box
{
    int     gridX;      // Satır içi sütun indeksi (0…cols-1)
    int     gridY;      // Satır indeksi (0…rows-1)
    Vector2 center;     // Her karede yeniden hesaplanacak ekran koordinatı
    float   size;       // Animasyon boyutu (kenar uzunluğu)
    float   speed;      // Anlık hız
    bool    expanding;  // true = büyüyor, false = küçülüyor
};

Rectangle GetGhost(const Box& b);

Rectangle GetRect(const Box& b);

void InitBox(Box& b);

void HandleInput(Box& b);

void UpdateBox(Box& b);

void DrawBox(const Box& b);

void DrawGhostIfHover(const Box& b);

#endif // BOX_H
