#include "../header/MechView.h"
#include "../header/Lift.h"
#include "../header/Transformasi.h"
#include <math.h>

// Fungsi untuk menggambar simbol silang yang berputar (untuk menunjukkan katrol yang berputar)
void DrawRotatedCross(int cx, int cy, int radius, float angle, Color color) {
    // Garis 1 
    int x1 = cx + cosf(angle) * (radius - 2);
    int y1 = cy + sinf(angle) * (radius - 2);
    int x2 = cx - cosf(angle) * (radius - 2);
    int y2 = cy - sinf(angle) * (radius - 2);
    DrawLineBresenham(x1, y1, x2, y2, color);

    // Garis 2 (Tegak lurus, sudut ditambah 90 derajat atau PI/2)
    int x3 = cx + cosf(angle + PI/2) * (radius - 2);
    int y3 = cy + sinf(angle + PI/2) * (radius - 2);
    int x4 = cx - cosf(angle + PI/2) * (radius - 2);
    int y4 = cy - sinf(angle + PI/2) * (radius - 2);
    DrawLineBresenham(x3, y3, x4, y4, color);
}

void DrawMechView(Elevator* lift) {
    float sw = GetScreenWidth();
    float startX = sw * 0.55f; // Mulai dari 55% layar
    float rightWidth = sw * 0.45f;

    float shaftCenter = startX + (rightWidth / 2.0f);
    // GAMBAR KATROL (Lingkaran dengan simbol silang yang berputar)
    DrawCircleMidpoint(shaftCenter - 25, 70, 20, PURPLE); 
    DrawCircleMidpoint(shaftCenter + 25, 70, 15, PURPLE); 

    // GAMBAR (+) BERPUTAR DI DALAM KATROL
    DrawRotatedCross(shaftCenter - 25, 70, 20, lift->pulleyAngle, VIOLET);
    DrawRotatedCross(shaftCenter + 25, 70, 15, -lift->pulleyAngle, VIOLET); 
    // Putaran berlawanan untuk efek visual yang lebih menarik
    DrawRotatedCross(shaftCenter - 25, 70, 20, lift->pulleyAngle + PI/4, PINK);
    DrawRotatedCross(shaftCenter + 25, 70, 15, -lift->pulleyAngle + PI/4, PINK);

    // GAMBAR KABEL (Garis dari katrol ke lift)
    DrawLineBresenham(shaftCenter - 25, 70, shaftCenter - 25, lift->y, LIGHTGRAY);
    DrawLineBresenham(shaftCenter + 25, 70, shaftCenter + 25, lift->cw_y, LIGHTGRAY);

    Color cwColor = (Color){ 40, 100, 50, 255 };
    int cwX = shaftCenter + 10;
    for (int i = 0; i < 80; i++) {
        DrawLineBresenham(cwX, lift->cw_y + i, cwX + 30, lift->cw_y + i, cwColor);
    }
    DrawRectCustom(cwX, lift->cw_y, 30, 80, GREEN);
    DrawRectCustom(shaftCenter - 45, lift->y, 40, (GetScreenHeight() - 150) / 5.0f - 10, BLUE);
}