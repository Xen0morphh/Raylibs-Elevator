#include "../header/BuildingView.h"
#include "../header/Lift.h"

void DrawBuildingView(Elevator* lift) {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float leftWidth = sw * 0.55f; // Batas area kiri (55%)

    // 1. Gambar Lorong Lift (Shaft) di tengah area kiri
    float shaftWidth = 100;
    float shaftX = (leftWidth - shaftWidth) / 2.0f;
    DrawRectangle(shaftX, 50, shaftWidth, sh - 100, BLACK);

    // 2. Gambar Garis 5 Lantai
    float floorHeight = (sh - 150) / 5.0f;
    for (int i = 0; i < 5; i++) {
        float floorY = sh - 100 - (i * floorHeight);
        DrawLine(50, floorY, leftWidth - 50, floorY, DARKGRAY);
        DrawText(TextFormat("LT %d", i + 1), 70, floorY - 30, 20, GRAY);
    }

    // 3. Gambar Elevator Car (Posisinya mengambil dari lift->y)
    DrawRectangle(shaftX + 5, lift->y, shaftWidth - 10, floorHeight - 10, (Color){ 50, 100, 150, 255 });
    
    // Nanti ditambahkan: Logika pintu terbuka (menggunakan lift->doorOpenness)
}