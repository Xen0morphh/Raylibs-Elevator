#include "../header/About.h"
#include "raylib.h"

// Variabel untuk background
static Texture2D bgAbout;

void InitAbout(void) {
    // Memuat gambar background
    bgAbout = LoadTexture("assets/Bg_About.jpg");
}

void UnloadAbout(void) {
    // Membersihkan memori saat keluar
    UnloadTexture(bgAbout);
}

void DrawAboutScreen(void) {
    float sw = (float)GetScreenWidth();
    float sh = (float)GetScreenHeight();

    // ==========================================
    // 1. GAMBAR BACKGROUND & OVERLAY GELAP
    // ==========================================
    if (bgAbout.id != 0) {
        DrawTexturePro(bgAbout, 
            (Rectangle){ 0, 0, (float)bgAbout.width, (float)bgAbout.height },
            (Rectangle){ 0, 0, sw, sh }, 
            (Vector2){ 0, 0 }, 0.0f, WHITE);
    } else {
        ClearBackground(DARKBLUE); 
    }
    
    // Overlay transparan dikurangi (dari 220 ke 140) agar background lebih jelas
    DrawRectangle(0, 0, (int)sw, (int)sh, (Color){ 10, 15, 25, 140 });

    // ==========================================
    // 2. KOTAK KONTEN (GLASSMORPHISM PANEL)
    // ==========================================
    float panelW = sw * 0.75f;
    float panelH = sh * 0.8f;
    float panelX = (sw - panelW) / 2.0f;
    float panelY = (sh - panelH) / 2.0f;

    // Background kotak lebih transparan (Alpha turun jadi 150)
    DrawRectangleRounded((Rectangle){panelX, panelY, panelW, panelH}, 0.05f, 10, (Color){ 20, 30, 45, 150 });
    DrawRectangleRoundedLines((Rectangle){panelX, panelY, panelW, panelH}, 0.05f, 10, SKYBLUE);

    // ==========================================
    // MACRO: BANTUAN UNTUK TEKS RATA TENGAH
    // ==========================================
    #define DRAW_TEXT_CENTER(txt, y, size, col) \
        DrawText(txt, (int)(panelX + (panelW - MeasureText(txt, size)) / 2), (int)(y), size, col)

    // TEKS KONTEN 
    float currentY = panelY + 40;

    // Judul
    DRAW_TEXT_CENTER("TENTANG APLIKASI", currentY, 28, SKYBLUE);
    currentY += 25;
    DrawLine((int)(panelX + 100), (int)currentY, (int)(panelX + panelW - 100), (int)currentY, (Color){40, 80, 120, 200});
    currentY += 25;

    // Deskripsi Umum
    DRAW_TEXT_CENTER("Aplikasi ini adalah Simulasi Lift (Elevator) interaktif 2D yang dibangun menggunakan", currentY, 14, LIGHTGRAY); currentY += 20;
    DRAW_TEXT_CENTER("bahasa C dan framework Raylib. Simulasi ini menampilkan pergerakan lift secara real-time,", currentY, 14, LIGHTGRAY); currentY += 20;
    DRAW_TEXT_CENTER("lengkap dengan logika Finite State Machine (FSM) untuk pintu, penumpang, dan ruang mesin.", currentY, 14, LIGHTGRAY); currentY += 45;

    // Judul Algoritma
    DRAW_TEXT_CENTER("ALGORITMA GRAFIKA KOMPUTER YANG DITERAPKAN:", currentY, 16, GREEN); currentY += 35;

    // Poin 1
    DRAW_TEXT_CENTER("1. Algoritma Garis Bresenham (Bresenham's Line)", currentY, 15, SKYBLUE); currentY += 20;
    DRAW_TEXT_CENTER("Diterapkan pada: Penggambaran kabel penarik lift, garis lantai gedung,", currentY, 13, LIGHTGRAY); currentY += 18;
    DRAW_TEXT_CENTER("serta garis batas dan UI pada panel panduan.", currentY, 13, LIGHTGRAY); currentY += 30;

    // Poin 2
    DRAW_TEXT_CENTER("2. Algoritma Lingkaran Midpoint (Midpoint Circle)", currentY, 15, SKYBLUE); currentY += 20;
    DRAW_TEXT_CENTER("Diterapkan pada: Penggambaran katrol (pulley) penggerak utama di ruang mesin", currentY, 13, LIGHTGRAY); currentY += 18;
    DRAW_TEXT_CENTER("(Mech View) dan indikator titik navigasi pada menu.", currentY, 13, LIGHTGRAY); currentY += 30;

    // Poin 3
    DRAW_TEXT_CENTER("3. Transformasi Geometri 2D (Translasi & Rotasi)", currentY, 15, SKYBLUE); currentY += 20;
    DRAW_TEXT_CENTER("Diterapkan pada: Translasi (pergerakan Y) kotak lift dan Counterweight,", currentY, 13, LIGHTGRAY); currentY += 18;
    DRAW_TEXT_CENTER("serta Rotasi animasi ruji-ruji katrol yang berputar berdasarkan kecepatan motor.", currentY, 13, LIGHTGRAY); currentY += 30;

    // Poin 4
    DRAW_TEXT_CENTER("4. Parallax Scrolling & Camera 2D", currentY, 15, SKYBLUE); currentY += 20;
    DRAW_TEXT_CENTER("Diterapkan pada: Efek kedalaman (3D illusion) pada background yang mengikuti", currentY, 13, LIGHTGRAY); currentY += 18;
    DRAW_TEXT_CENTER("posisi mouse, serta sistem POV Kamera (Zoom dan Tracking pada subjek).", currentY, 13, LIGHTGRAY);

    // Hapus definisi macro agar tidak bocor ke fungsi lain
    #undef DRAW_TEXT_CENTER

    // ==========================================
    // 4. FOOTER (TOMBOL KEMBALI)
    // ==========================================
    const char* footer = "Tekan [BACKSPACE] untuk Kembali ke Menu Utama";
    int fw = MeasureText(footer, 14);
    DrawText(footer, (int)(sw/2 - fw/2), (int)(sh - 40), 14, GRAY);
}