#include "raylib.h"
#include "Screen_type.h"
#include "header/Menu.h"

int main(void) {
    // Ukuran layar jendela aplikasi
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Elevator Simulation Project");

    // Tentukan layar pertama saat aplikasi dibuka
    ScreenState currentScreen = SCREEN_MENU;

    // Panggil inisialisasi modul (Load aset)
    InitMenuScreen();
    // Nanti tambahkan: InitSimulationScreen(), InitGuideScreen(), dll.

    SetTargetFPS(60);

    // GAME LOOP UTAMA
    while (!WindowShouldClose()) {
        
        // ============================================
        // 1. UPDATE LOGIKA (State Machine)
        // ============================================
        switch (currentScreen) {
            case SCREEN_MENU:
                // Menu akan mengembalikan state baru jika ada tombol yang diklik
                currentScreen = UpdateMenuScreen();
                break;
            case SCREEN_SIMULATION:
                // Untuk sementara, tekan BACKSPACE untuk kembali ke Menu
                if (IsKeyPressed(KEY_BACKSPACE)) currentScreen = SCREEN_MENU;
                break;
            case SCREEN_GUIDE:
                if (IsKeyPressed(KEY_BACKSPACE)) currentScreen = SCREEN_MENU;
                break;
            case SCREEN_ABOUT:
                if (IsKeyPressed(KEY_BACKSPACE)) currentScreen = SCREEN_MENU;
                break;
            case SCREEN_EXIT:
                // Jika tombol exit ditekan, paksakan keluar dari while loop
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
                DrawText("--- LAYAR SIMULASI ---", 250, 250, 30, DARKGRAY);
                DrawText("Tekan BACKSPACE untuk kembali ke Menu", 200, 300, 20, GRAY);
                break;
            case SCREEN_GUIDE:
                DrawText("--- LAYAR PANDUAN ---", 250, 250, 30, DARKBLUE);
                DrawText("Tekan BACKSPACE untuk kembali ke Menu", 200, 300, 20, GRAY);
                break;
            case SCREEN_ABOUT:
                DrawText("--- LAYAR TENTANG ---", 250, 250, 30, DARKGREEN);
                DrawText("Tekan BACKSPACE untuk kembali ke Menu", 200, 300, 20, GRAY);
                break;
            default: break;
        }

        EndDrawing();
    }

EXIT_GAME:
    
    // ============================================
    // 3. UNLOAD (Bersihkan RAM)
    // ============================================
    UnloadMenuScreen();
    // Nanti tambahkan: UnloadSimulationScreen(), dll.

    CloseWindow();
    return 0;
}