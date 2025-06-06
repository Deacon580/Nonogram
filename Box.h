#ifndef BOX_H
#define BOX_H

#include "raylib.h"

// -------------------- Ayarlar / Settings -------------------------
static const float   BOX_MAX_SIZE = 100.0f;
static const float   BOX_MIN_SIZE = 0.0f;
static const float   BOX_START_SPEED = 10.0f;
static const Vector2 GHOST_SIZE = { 100, 100 };

// Yardımcı işlev (Float clamp)
float ClampF(float v, float lo, float hi);

// Veri Yapısı / Data Struct
struct Box
{
    int     gridX;      // Satır içi sütun indeksi (0…cols-1)
    int     gridY;      // Satır indeksi (0…rows-1)
    Vector2 center;     // Her karede yeniden hesaplanacak ekran koordinatı
    float   size;       // Animasyon boyutu (kenar uzunluğu)
    float   speed;      // Anlık hız
    bool    expanding;  // true = büyüyor, false = küçülüyor
};

// Ghost (tetik) alanı: kutunun merkezinden GHOST_SIZE/2 ötede
Rectangle GetGhost(const Box& b);

// Gerçek kutu dikdörtgeni (size × size) b.center’ı sol-üst köşe kabul eder
Rectangle GetRect(const Box& b);

// Yeni Box başlat (gridX ve gridY önden ayarlanmalı)
void InitBox(Box& b);

// Fare tıklama mantığı
void HandleInput(Box& b);

// Animasyonu güncelle
void UpdateBox(Box& b);

// Kutuyu merkezden çiz (size/2 ofsetli)
void DrawBox(const Box& b);

// Fare hover’ında ghost alanını çiz
void DrawGhostIfHover(const Box& b);

#endif // BOX_H
