#include "../header/Menu.h"
#include "Screen_type.h"

// Variabel lokal
static Texture2D bgMenu;
static Rectangle btnStart, btnGuide, btnAbout, btnExit;

void InitMenuScreen(void) {
    bgMenu = LoadTexture("assets/Bg_Menu.png");
}

ScreenState UpdateMenuScreen(void) {
    Vector2 mousePoint = GetMousePosition();

    // Posisi tombol selalu di tengah layar
    float cx = GetScreenWidth() / 2.0f;
    float cy = GetScreenHeight() / 2.0f;

    // Posisi: X (tengah - setengah lebar  tombol) layar akan FullScreen
    btnStart = (Rectangle){ cx - 110, cy - 100, 220, 50 };
    btnGuide = (Rectangle){ cx - 110, cy - 30,  220, 50 };
    btnAbout = (Rectangle){ cx - 110, cy + 40,  220, 50 };
    btnExit  = (Rectangle){ cx - 110, cy + 110, 220, 50 };

    // Deteksi jika mouse melakukan klik kiri
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePoint, btnStart)) return SCREEN_SIMULATION;
        if (CheckCollisionPointRec(mousePoint, btnGuide)) return SCREEN_GUIDE;
        if (CheckCollisionPointRec(mousePoint, btnAbout)) return SCREEN_ABOUT;
        if (CheckCollisionPointRec(mousePoint, btnExit))  return SCREEN_EXIT;
    }
    return SCREEN_MENU;
}

void DrawMenuScreen(void) {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float cx = sw / 2.0f;
    float cy = sh / 2.0f;
    Vector2 mousePoint = GetMousePosition();

    // ==========================================
    // A. EFEK PARALLAX (BACKGROUND BERGERAK)
    // ==========================================
    // Hitung jarak mouse dari titik tengah layar
    float offsetX = (mousePoint.x - cx) * 0.03f;
    float offsetY = (mousePoint.y - cy) * 0.03f;

    // Background diperbesar 10% agar saat bergeser pinggirannya tidak "bocor"/hitam
    float bgWidth = sw * 1.1f;
    float bgHeight = sh * 1.1f;

    Rectangle source = { 0.0f, 0.0f, (float)bgMenu.width, (float)bgMenu.height };
    Rectangle dest = { 
        (sw - bgWidth) / 2.0f - offsetX, // Posisi asal di tengah, dikurangi gaya tarik mouse
        (sh - bgHeight) / 2.0f - offsetY, 
        bgWidth, 
        bgHeight 
    };
    Vector2 origin = { 0.0f, 0.0f };

    DrawTexturePro(bgMenu, source, dest, origin, 0.0f, WHITE);

    // ==========================================
    // B. GAMBAR TOMBOL (ROUNDED & EFEK HOVER)
    // ==========================================
    Rectangle* btns[] = { &btnStart, &btnGuide, &btnAbout, &btnExit };
    const char* labels[] = { "START SIMULATION", "GUIDE", "ABOUT", "EXIT" };

    Color colorNormal = (Color){ 233, 183, 110, 200 }; // Warna dasar transparan
    Color colorHover  = (Color){ 233, 183, 110, 255 }; // Warna solid saat disorot mouse
    Color colorShadow = (Color){ 0, 0, 0, 70 };        // Warna hitam transparan untuk bayangan

    for (int i = 0; i < 4; i++) {
        bool isHover = CheckCollisionPointRec(mousePoint, *btns[i]);
        
        // Kotak akan lebih terang/solid (tidak transparan) saat kursor mendekat
        Color boxColor = isHover ? colorHover : colorNormal;
        Color textColor = isHover ? WHITE : BLACK;

        // A. Gambar Bayangan Dulu (Digeser +4 ke kanan dan +4 ke bawah)
        Rectangle shadowRec = { btns[i]->x + 4, btns[i]->y + 4, btns[i]->width, btns[i]->height };
        DrawRectangleRounded(shadowRec, 0.4f, 10, colorShadow);

        // B. Gambar Kotak Tombol Utama (Menumpuk di atas bayangan)
        DrawRectangleRounded(*btns[i], 0.4f, 10, boxColor);

        // C. Tulis Teks di Tengah Tombol
        int textWidth = MeasureText(labels[i], 20);
        DrawText(labels[i], btns[i]->x + (btns[i]->width - textWidth) / 2, btns[i]->y + 15, 20, textColor);
    }
}

void UnloadMenuScreen(void) {
    UnloadTexture(bgMenu);
}