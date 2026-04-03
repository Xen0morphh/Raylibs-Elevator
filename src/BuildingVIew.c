#include "../header/BuildingView.h"
#include "../header/Lift.h"
#include "../header/Transformasi.h"


void DrawBuildingView(Elevator* lift) {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float leftWidth = sw * 0.55f;
    float floorHeight = (sh - 150) / 5.0f;
    
    float shaftWidth = 100;
    // PENTING: Geser posisi shaft ke kanan (tambah 80) agar tidak nabrak UI
    float shaftX = ((leftWidth - shaftWidth) / 2.0f) + 80; 

    // Gambar Lorong
    DrawRectCustom(shaftX, 50, shaftWidth, sh - 100, DARKGRAY);

    // Gambar Garis Lantai (Mulai dari X = 200 agar tidak nabrak UI)
    for (int i = 1; i <= 5; i++) {
        float floorBaseline = sh - 100 - ((i - 1) * floorHeight);
        DrawLineBresenham(200, floorBaseline, leftWidth - 50, floorBaseline, (Color){100, 100, 100, 200});
        DrawText(TextFormat("LT %d", i), 210, floorBaseline - 25, 15, GRAY);
    }

    // ==========================================
    // GAMBAR ELEVATOR (Dengan Pintu Dinamis)
    // ==========================================
    float carWidth = shaftWidth - 10;
    float carHeight = floorHeight - 10;
    float carX = shaftX + 5;
    float carY = lift->y;

    // 1. Gambar Bagian Dalam Lift (Abu-abu Gelap)
    // Memakai kotak solid agar terlihat seperti interior ruangan
    DrawRectangle(carX, carY, carWidth, carHeight, (Color){40, 40, 40, 255}); 

    // 2. Logika Pintu Kiri dan Kanan (Akan menyusut ke samping saat terbuka)
    float doorWidth = (carWidth / 2.0f) * (1.0f - lift->doorOpenness);
    
    if (doorWidth > 0.0f) {
        // Gambar Pintu Kiri (Biru)
        DrawRectangle(carX, carY, doorWidth, carHeight, (Color){20, 60, 100, 255});
        DrawRectCustom(carX, carY, doorWidth, carHeight, SKYBLUE); // Outline kiri

        // Gambar Pintu Kanan (Biru)
        DrawRectangle(carX + carWidth - doorWidth, carY, doorWidth, carHeight, (Color){20, 60, 100, 255});
        DrawRectCustom(carX + carWidth - doorWidth, carY, doorWidth, carHeight, SKYBLUE); // Outline kanan
    }

    // Frame Utama Lift di bagian paling luar
    DrawRectCustom(carX, carY, carWidth, carHeight, BLUE);
}