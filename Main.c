#include "raylib.h"
#include "Screen_type.h"
#include "header/Menu.h"
#include "header/UI.h"
#include "header/Simulation.h"
#include "header/Person.h"
#include "header/Guide.h"
#include "header/About.h"
Person testPerson; // Variabel global untuk subjek tes (bisa diakses semua file)
extern Person myPerson; 
extern Elevator myLift;

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
    InitPersonModule();
    testPerson.startFloor = 1;
    testPerson.targetFloor = 4;
    testPerson.position = (Vector2){ 250.0f, 0.0f }; // X menunggu di luar, Y dihitung di Update
    testPerson.state = PERSON_WAITING;
    testPerson.framesSpeed = 8; // 8 frame per detik
    testPerson.currentFrame = 0;
    testPerson.framesCounter = 0;

    SetTargetFPS(60);

    // GAME LOOP UTAMA
    while (!WindowShouldClose()) {
        
        // ============================================
        // 1. UPDATE LOGIKA (State Machine)
        // ============================================
        switch (currentScreen) {
            case SCREEN_MENU: {
                ScreenState next = UpdateMenuScreen();
                if (next != SCREEN_MENU) {
                    if (next == SCREEN_GUIDE) InitGuide();     
                    if (next == SCREEN_SIMULATION) DrawSimulationScreen(&testPerson);
                    if (next == SCREEN_ABOUT) InitAbout(); 
                    currentScreen = next;
                }
            }break;
            
            case SCREEN_SIMULATION:
                if (IsKeyPressed(KEY_BACKSPACE)) currentScreen = SCREEN_MENU;
                break;
            case SCREEN_GUIDE:
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    UnloadGuide();
                    currentScreen = SCREEN_MENU;
                }
                break;
            case SCREEN_ABOUT:
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    UnloadAbout();
                    currentScreen = SCREEN_MENU;
                }
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
                // Panggil fungsi gambar untuk layar simulasi (termasuk background, building view, mech view, dan UI)
                DrawSimulationScreen(&testPerson);
                break;
            case SCREEN_GUIDE:
                DrawGuideScreen();
                break;
            case SCREEN_ABOUT:
                DrawAboutScreen();
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