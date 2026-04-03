#include "../header/UI.h"

#include "raylib.h"
#include "../header/Lift.h"

static Texture2D bgMain;

void InitUI(void) {
    bgMain = LoadTexture("assets/Bg_Main.png");
}
void DrawMainBackground(void) {
    // PARALLAX BACKGROUND: Efek gambar bergerak mengikuti posisi mouse
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    Vector2 mousePoint = GetMousePosition();

    float cx = sw / 2.0f;
    float cy = sh / 2.0f;

    // Hitung intensitas tarikan (Parallax). 
    float offsetX = (mousePoint.x - cx) * 0.03f;
    float offsetY = (mousePoint.y - cy) * 0.03f;

    // Perbesar background sebesar 10% (dikali 1.1) agar ujungnya tidak "bocor" saat bergeser
    float bgWidth = sw * 1.1f;
    float bgHeight = sh * 1.1f;

    // Rumus area gambar dan area tujuan di layar
    Rectangle source = { 0.0f, 0.0f, (float)bgMain.width, (float)bgMain.height };
    Rectangle dest = { 
        (sw - bgWidth) / 2.0f - offsetX, // Posisi awal di tengah, dikurangi tarikan mouse X
        (sh - bgHeight) / 2.0f - offsetY, // Posisi awal di tengah, dikurangi tarikan mouse Y
        bgWidth, 
        bgHeight 
    };
    
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(bgMain, source, dest, origin, 0.0f, GRAY); 
}

void UnloadUI(void) {
    UnloadTexture(bgMain);
}

void DrawSimulationUI(Elevator* lift) {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();

    // 1. Garis Pemisah (Split Screen Line)
    DrawLine(sw * 0.55f, 0, sw * 0.55f, sh, (Color){ 50, 100, 150, 255 });

    // 2. Gambar Panel Kiri Bawah (In-Car Controls)
    // Gunakan DrawRectangleRounded...

    // 3. Status Bar Bawah
    DrawRectangle(0, sh - 40, sw, 40, (Color){ 10, 15, 25, 255 }); // Background status bar
    
    // 4. Teks Dinamis
    const char* stateText = (lift->state == MOVING_UP) ? "MOVING UP" : "IDLE"; // Contoh sederhana
    DrawText(TextFormat("STATE: %s", stateText), 20, sh - 30, 20, GREEN);
    DrawText(TextFormat("FLOOR: %d", lift->currentFloor), 250, sh - 30, 20, LIGHTGRAY);
    // Lanjutkan untuk Target, Queue, dan Door status...
}