#include "../header/MechView.h"
#include "../header/Lift.h"

void DrawMechView(Elevator* lift) {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float startX = sw * 0.55f; // Mulai dari 55% layar
    float rightWidth = sw * 0.45f;

    float shaftCenter = startX + (rightWidth / 2.0f);

    // 1. Gambar Lorong Mesin (Transparan/Outline)
    DrawRectangleLines(shaftCenter - 60, 50, 120, sh - 100, DARKGRAY);

    // 2. Gambar Motor & Pulley (Di ujung atas)
    DrawCircle(shaftCenter - 25, 70, 20, PURPLE); // Pulley Lift
    DrawCircle(shaftCenter + 25, 70, 15, PURPLE); // Pulley Counterweight

    // 3. Gambar Kabel
    // Kabel ke Lift (Kiri)
    DrawLine(shaftCenter - 25, 70, shaftCenter - 25, lift->y, LIGHTGRAY);
    // Kabel ke Counterweight (Kanan)
    DrawLine(shaftCenter + 25, 70, shaftCenter + 25, lift->cw_y, LIGHTGRAY);

    // 4. Gambar Counterweight (Warna Hijau Gelap)
    DrawRectangle(shaftCenter + 10, lift->cw_y, 30, 80, (Color){ 40, 100, 50, 255 });

    // 5. Gambar Lift Mekanik
    DrawRectangleLines(shaftCenter - 45, lift->y, 40, 60, BLUE);
}