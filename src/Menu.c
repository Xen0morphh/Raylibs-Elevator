#include "../header/Menu.h"

// Variabel lokal (hanya bisa diakses di file ini)
static Texture2D bgMenu;
static Rectangle btnStart, btnGuide, btnAbout, btnExit;

void InitMenuScreen(void) {
    // 1. Load Background Menu Anda
    bgMenu = LoadTexture("assets/Bg_Menu.png");

    // 2. Setup area tombol (Posisi X, Posisi Y, Lebar, Tinggi)
    // Asumsi ukuran layar aplikasi Anda 800x600. Sesuaikan posisinya jika perlu!
    btnStart = (Rectangle){ 300, 250, 200, 50 };
    btnGuide = (Rectangle){ 300, 320, 200, 50 };
    btnAbout = (Rectangle){ 300, 390, 200, 50 };
    btnExit  = (Rectangle){ 300, 460, 200, 50 };
}

ScreenState UpdateMenuScreen(void) {
    Vector2 mousePoint = GetMousePosition();

    // Deteksi jika mouse melakukan klik kiri
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        // Cek apakah klik tersebut mengenai area persegi tombol kita
        if (CheckCollisionPointRec(mousePoint, btnStart)) return SCREEN_SIMULATION;
        if (CheckCollisionPointRec(mousePoint, btnGuide)) return SCREEN_GUIDE;
        if (CheckCollisionPointRec(mousePoint, btnAbout)) return SCREEN_ABOUT;
        if (CheckCollisionPointRec(mousePoint, btnExit))  return SCREEN_EXIT;
    }

    // Jika tidak ada yang diklik, tetap bertahan di layar Menu
    return SCREEN_MENU; 
}

void DrawMenuScreen(void) {
    // 1. Gambar Background (Skala mengikuti ukuran asli gambar Anda)
    DrawTexture(bgMenu, 0, 0, WHITE);

    // 2. Gambar area tombol (Sederhana dulu menggunakan kotak abu-abu)
    DrawRectangleRec(btnStart, LIGHTGRAY);
    DrawText("START SIMULATION", 310, 265, 20, BLACK);

    DrawRectangleRec(btnGuide, LIGHTGRAY);
    DrawText("GUIDE", 370, 335, 20, BLACK);

    DrawRectangleRec(btnAbout, LIGHTGRAY);
    DrawText("ABOUT", 370, 405, 20, BLACK);

    DrawRectangleRec(btnExit, LIGHTGRAY);
    DrawText("EXIT", 380, 475, 20, BLACK);
    
    // 3. Tambahkan efek "Hover" (Kotak menjadi merah saat kursor berada di atasnya)
    Vector2 mousePoint = GetMousePosition();
    if (CheckCollisionPointRec(mousePoint, btnStart)) DrawRectangleLinesEx(btnStart, 3, RED);
    if (CheckCollisionPointRec(mousePoint, btnGuide)) DrawRectangleLinesEx(btnGuide, 3, RED);
    if (CheckCollisionPointRec(mousePoint, btnAbout)) DrawRectangleLinesEx(btnAbout, 3, RED);
    if (CheckCollisionPointRec(mousePoint, btnExit))  DrawRectangleLinesEx(btnExit, 3, RED);
}

void UnloadMenuScreen(void) {
    // WAJIB: Bersihkan memori saat aplikasi ditutup
    UnloadTexture(bgMenu);
}