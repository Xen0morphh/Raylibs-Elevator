#include "../header/UI.h"
#include "raylib.h"
#include "../header/Lift.h"
#include "../header/Transformasi.h"

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
    Vector2 mousePoint = GetMousePosition();

    DrawLineBresenham(sw * 0.55f, 0, sw * 0.55f, sh, (Color){ 50, 100, 150, 255 });
    DrawRectCustom(0, sh - 40, sw, 40, LIGHTGRAY);

    // PANEL DIPINDAH KE POJOK KIRI BAWAH
    int panelX = 20; 
    int panelY = sh - 350; 

    DrawRectangleRounded((Rectangle){panelX, panelY, 140, 280}, 0.2f, 10, (Color){ 10, 20, 35, 230 });
    DrawRectangleRoundedLines((Rectangle){panelX, panelY, 140, 280}, 0.2f, 10, SKYBLUE);
    DrawText("IN-CAR", panelX + 45, panelY + 15, 15, SKYBLUE);

    // Tombol Angka 1-5
    for (int i = 5; i >= 1; i--) {
        int btnX = panelX + ((i % 2 == 1) ? 35 : 95); 
        int btnY = panelY + 60 + ((5 - i) * 25);
        
        Color btnColor = (lift->targetFloor == i) ? BLUE : DARKBLUE; 
        
        if (CheckCollisionPointCircle(mousePoint, (Vector2){btnX, btnY}, 18)) {
            DrawCircleLines(btnX, btnY, 20, WHITE); 
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && lift->state == IDLE) {
                lift->targetFloor = i; 
            }
        }

        DrawCircle(btnX, btnY, 18, btnColor);
        DrawCircleLines(btnX, btnY, 18, SKYBLUE);
        DrawText(TextFormat("%d", i), btnX - 5, btnY - 8, 18, RAYWHITE);
    }

    // Tombol BUKA / TUTUP Pintu
    Rectangle btnBuka = {panelX + 15, panelY + 220, 50, 35};
    Rectangle btnTutup = {panelX + 75, panelY + 220, 50, 35};
    
    if (CheckCollisionPointRec(mousePoint, btnBuka)) {
        DrawRectangleRoundedLines(btnBuka, 0.3f, 5, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (lift->state == IDLE) lift->state = DOOR_OPENING;
            if (lift->state == DOOR_OPEN) lift->timer = 3.0f; // Reset waktu
        }
    }
    
    if (CheckCollisionPointRec(mousePoint, btnTutup)) {
        DrawRectangleRoundedLines(btnTutup, 0.3f, 5, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (lift->state == DOOR_OPEN) lift->timer = 0.0f; // Paksa tutup
        }
    }

    DrawRectangleRoundedLines(btnBuka, 0.3f, 5, GREEN);
    DrawText("[ ]", btnBuka.x + 18, btnBuka.y + 10, 15, GREEN);
    
    DrawRectangleRoundedLines(btnTutup, 0.3f, 5, RED);
    DrawText("][", btnTutup.x + 18, btnTutup.y + 10, 15, RED);

    // Teks Status
    const char* stateText = "IDLE";
    if (lift->state == MOVING_UP) stateText = "MOVING UP";
    else if (lift->state == MOVING_DOWN) stateText = "MOVING DOWN";
    else if (lift->state == DOOR_OPENING) stateText = "DOORS OPENING";
    else if (lift->state == DOOR_OPEN) stateText = "DOORS OPEN (WAIT)";
    else if (lift->state == DOOR_CLOSING) stateText = "DOORS CLOSING";
    
    DrawText(TextFormat("STATE: %s", stateText), 20, sh - 30, 20, GREEN);
    DrawText(TextFormat("FLOOR: %d", lift->currentFloor), 350, sh - 30, 20, DARKGRAY);
}