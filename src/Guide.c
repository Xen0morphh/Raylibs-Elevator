#include "../header/Guide.h"
#include "raylib.h"

static Texture2D bgGuide; // Variabel penyimpan gambar

void InitGuide(void) {
    // Muat gambar dari folder assets
    bgGuide = LoadTexture("assets/Bg_Guide.png");
}

void UnloadGuide(void) {
    // Hapus dari memori saat keluar
    UnloadTexture(bgGuide);
}

void DrawGuideScreen(void) {
    // 1. Gambar Background Guide (Pastikan ini yang pertama dipanggil)
    if (bgGuide.id != 0) {
        DrawTexturePro(bgGuide, 
            (Rectangle){ 0, 0, (float)bgGuide.width, (float)bgGuide.height },
            (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
            (Vector2){ 0, 0 }, 0.0f, WHITE);
    } else {
        // Jika gambar gagal load, kasih warna dasar gelap biar teks tetap kebaca
        ClearBackground(BLACK);
    }

    // 2. Gambar Overlay Transparan (Opsional)
    // Gunakan ini jika gambar Bg_Guide kamu terlalu terang dan menutupi teks instruksi
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){ 0, 0, 0, 100 });

    // 3. Tulis Teks Instruksi di atas Background
    DrawText("PANDUAN SIMULASI LIFT", 50, 50, 30, SKYBLUE);
    
    DrawText("- Tekan 1-4 untuk Ganti Kamera (POV)", 50, 120, 20, WHITE);
    DrawText("- Klik Tombol Lantai untuk Menggerakkan Lift", 50, 160, 20, WHITE);
    DrawText("- Tombol P.IN / P.OUT untuk Kontrol Orang", 50, 200, 20, WHITE);
    
    DrawText("Tekan [BACKSPACE] untuk Kembali ke Menu", 50, GetScreenHeight() - 60, 20, GRAY);
}