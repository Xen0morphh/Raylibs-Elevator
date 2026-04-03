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

    // 1. Garis Belah Tengah & Status Bar
    DrawLineBresenham(sw * 0.55f, 0, sw * 0.55f, sh, (Color){ 50, 100, 150, 255 });
    DrawRectCustom(0, sh - 40, sw, 40, LIGHTGRAY);

    // 2. KOTAK INFO STATUS (Kiri Atas)
    int infoX = 20;
    int infoY = 20;
    DrawRectangleRounded((Rectangle){infoX, infoY, 120, 80}, 0.2f, 10, (Color){ 10, 20, 35, 230 });
    DrawRectangleRoundedLines((Rectangle){infoX, infoY, 120, 80}, 0.2f, 10, (Color){ 30, 80, 120, 255 });

    DrawText(TextFormat("%d", lift->currentFloor), infoX + 45, infoY + 15, 30, SKYBLUE);

    const char* dirText = "DIAM -";
    Color dirColor = GRAY;
    if (lift->state == MOVING_UP) { dirText = "NAIK ^"; dirColor = GREEN; }
    else if (lift->state == MOVING_DOWN) { dirText = "TURUN v"; dirColor = RED; }
    DrawText(dirText, infoX + 30, infoY + 55, 15, dirColor);

    // 3. PANEL KONTROL IN-CAR (Kiri Bawah)
    int panelX = 20; 
    int panelY = sh - 350; 
    DrawRectangleRounded((Rectangle){panelX, panelY, 140, 280}, 0.2f, 10, (Color){ 10, 20, 35, 230 });
    DrawRectangleRoundedLines((Rectangle){panelX, panelY, 140, 280}, 0.2f, 10, SKYBLUE);
    DrawText("IN-CAR", panelX + 45, panelY + 15, 15, SKYBLUE);

    // ==========================================
    // LOGIKA TOMBOL ANGKA YANG LEBIH RESPONSIF
    // ==========================================
    for (int i = 5; i >= 1; i--) {
        int btnX = panelX + ((i % 2 == 1) ? 35 : 95); 
        int btnY = panelY + 60 + ((5 - i) * 25);
        Color btnColor = (lift->targetFloor == i) ? BLUE : DARKBLUE; 
        
        if (CheckCollisionPointCircle(mousePoint, (Vector2){btnX, btnY}, 18)) {
            DrawCircleLines(btnX, btnY, 20, WHITE); 
            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                // Pastikan lift tidak sedang melaju di tengah jalan
                if (lift->state != MOVING_UP && lift->state != MOVING_DOWN) {
                    lift->targetFloor = i; 
                    
                    // JIKA PINTU LAGI TERBUKA ATAU PROSES BUKA, PAKSA LANGSUNG TUTUP!
                    if (lift->state == DOOR_OPEN || lift->state == DOOR_OPENING) {
                        lift->timer = 0.0f;           // Matikan waktu tunggu 3 detik
                        lift->state = DOOR_CLOSING;   // Paksa pintu langsung geser tutup
                    }
                }
            }
        }

        DrawCircle(btnX, btnY, 18, btnColor);
        DrawCircleLines(btnX, btnY, 18, SKYBLUE);
        DrawText(TextFormat("%d", i), btnX - 5, btnY - 8, 18, RAYWHITE);
    }

    // ==========================================
    // LOGIKA TOMBOL BUKA / TUTUP PINTU
    // ==========================================
    Rectangle btnBuka = {panelX + 15, panelY + 220, 50, 35};
    Rectangle btnTutup = {panelX + 75, panelY + 220, 50, 35};
    
    // Tombol BUKA [ ]
    if (CheckCollisionPointRec(mousePoint, btnBuka)) {
        DrawRectangleRoundedLines(btnBuka, 0.3f, 5, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Bisa dibuka lagi kalau sedang Idle atau setengah menutup
            if (lift->state == IDLE || lift->state == DOOR_CLOSING) lift->state = DOOR_OPENING;
            // Kalau sudah kebuka, reset waktu idle kembali ke 3 detik
            if (lift->state == DOOR_OPEN) lift->timer = 3.0f;
        }
    }
    
    // Tombol TUTUP ][
    if (CheckCollisionPointRec(mousePoint, btnTutup)) {
        DrawRectangleRoundedLines(btnTutup, 0.3f, 5, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Paksa potong waktu tunggu dan langsung tutup
            if (lift->state == DOOR_OPEN || lift->state == DOOR_OPENING) {
                lift->timer = 0.0f;
                lift->state = DOOR_CLOSING;
            }
        }
    }

    // Gambar Visual Tombol Buka/Tutup
    DrawRectangleRoundedLines(btnBuka, 0.3f, 5, GREEN);
    DrawText("[ ]", btnBuka.x + 18, btnBuka.y + 10, 15, GREEN);
    DrawRectangleRoundedLines(btnTutup, 0.3f, 5, RED);
    DrawText("][", btnTutup.x + 18, btnTutup.y + 10, 15, RED);

    // 4. TEKS STATUS BAR BAWAH
    const char* stateStr = "IDLE";
    if (lift->state == MOVING_UP) stateStr = "MOVING_UP";
    else if (lift->state == MOVING_DOWN) stateStr = "MOVING_DOWN";
    else if (lift->state == DOOR_OPENING) stateStr = "OPENING";
    else if (lift->state == DOOR_OPEN) stateStr = "OPEN (WAIT)";
    else if (lift->state == DOOR_CLOSING) stateStr = "CLOSING";

    // Mengubah nilai float Pintu menjadi Teks
    const char* doorStr = (lift->doorOpenness == 0.0f) ? "TERTUTUP" :
                          (lift->doorOpenness == 1.0f) ? "TERBUKA" : "PROSES...";

    // Menggambar teks berjajar di kiri bawah
    DrawText(TextFormat("STATE: %s", stateStr), 20, sh - 30, 20, GREEN);
    DrawText(TextFormat("FLOOR: %d", lift->currentFloor), 250, sh - 30, 20, DARKGRAY);
    DrawText(TextFormat("TARGET: %d", lift->targetFloor), 380, sh - 30, 20, DARKGRAY);
    DrawText(TextFormat("DOOR: %s", doorStr), 520, sh - 30, 20, ORANGE);

    // INFO TOMBOL (Kanan Bawah)
    const char* helpText = "ESC: Exit Program  |  BACKSPACE: Kembali ke Menu";
    int textWidth = MeasureText(helpText, 15);
    DrawText(helpText, sw - textWidth - 20, sh - 28, 15, DARKGRAY);
}