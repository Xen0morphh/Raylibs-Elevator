#include "raylib.h"
#include "Screen_type.h"
#include "header/Menu.h"
#include "header/UI.h"

int main(void) {
    // ============================================
    // INI BAGIAN UNTUK BIKIN FULL SCREEN
    // ============================================
    // Angka 0, 0 akan menyuruh Raylib mendeteksi resolusi monitor Anda secara otomatis
    InitWindow(0, 0, "Elevator Simulation Project");
    ToggleFullscreen(); // Fungsi ajaib untuk menghilangkan border Windows

    // Tentukan layar pertama saat aplikasi dibuka
    ScreenState currentScreen = SCREEN_MENU;

    // Panggil inisialisasi modul (Load aset langsung dipanggil di sini)
    InitMenuScreen();
    InitUI();
    SetTargetFPS(60);

    // GAME LOOP UTAMA
    while (!WindowShouldClose()) {
        
        // ============================================
        // 1. UPDATE LOGIKA (State Machine)
        // ============================================
        switch (currentScreen) {
            case SCREEN_MENU:
                currentScreen = UpdateMenuScreen();
                break;
            case SCREEN_SIMULATION:
                if (IsKeyPressed(KEY_BACKSPACE)) currentScreen = SCREEN_MENU;
                break;
            case SCREEN_GUIDE:
                if (IsKeyPressed(KEY_BACKSPACE)) currentScreen = SCREEN_MENU;
                break;
            case SCREEN_ABOUT:
                if (IsKeyPressed(KEY_BACKSPACE)) currentScreen = SCREEN_MENU;
                break;
            case SCREEN_EXIT:
                goto EXIT_GAME; 
        }

        // ============================================
        // 2. DRAW VISUAL (Render ke Layar)
        // ============================================
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentScreen) {
            case SCREEN_MENU:
                DrawMenuScreen();
                break;
            case SCREEN_SIMULATION:
                DrawMainBackground();
                DrawText("--- LAYAR SIMULASI ---", 250, 250, 30, DARKGRAY);
                DrawText("Tekan BACKSPACE untuk kembali ke Menu", 200, 300, 20, GRAY);
                break;
            case SCREEN_GUIDE:
                DrawMainBackground();
                DrawText("--- LAYAR PANDUAN ---", 250, 250, 30, DARKBLUE);
                DrawText("Tekan BACKSPACE untuk kembali ke Menu", 200, 300, 20, GRAY);
                break;
            case SCREEN_ABOUT:
                DrawMainBackground();
                DrawText("--- LAYAR TENTANG ---", 250, 250, 30, DARKGREEN);
                DrawText("Tekan BACKSPACE untuk kembali ke Menu", 200, 300, 20, GRAY);
                break;
            default: break;
        }

        EndDrawing();
    }

EXIT_GAME:
    
    UnloadMenuScreen();

    CloseWindow();
    return 0;
}