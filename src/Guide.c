#include "../header/Guide.h"
#include "../header/UI.h"
#include "../header/Person.h"
#include "raylib.h"

static Texture2D bgGuide;
static int currentSlide = 0;
const int TOTAL_SLIDES = 5;

// Variabel untuk Parallax
static float scrollingBack = 0.0f;

// Judul dan Penjelasan tiap Slide
const char* slideTitles[] = {
    "1. KOMPONEN UTAMA LIFT",
    "2. KONTROL ORANG (PERSON)",
    "3. SISTEM PINTU LIFT",
    "4. SUDUT PANDANG (POV)",
    "5. MEKANISME SHAFT & CW"
};

const char* slideDescs[] = {
    "CAR adalah kotak pengangkut penumpang.\nSHAFT adalah lorong tempat lift bergerak.",
    "Klik P.IN untuk memasukkan orang.\nKlik P.OUT untuk mengeluarkan orang.",
    "Pintu hanya bisa diatur saat lift IDLE.\nGunakaan tombol hijau/merah untuk kontrol.",
    "Klik ikon POV di pojok kanan atas\nuntuk mengganti kamera simulasi.",
    "CW (Counterweight) adalah penyeimbang.\nBergerak berlawanan arah dengan Lift."
};

void InitGuide(void) {
    bgGuide = LoadTexture("assets/Bg_Guide.png");
    currentSlide = 0;
    InitPersonModule(); 
}

void UnloadGuide(void) {
    UnloadTexture(bgGuide);
    UnloadPersonModule(); 
}

void DrawGuideScreen(void) {
    float sw = (float)GetScreenWidth();
    float sh = (float)GetScreenHeight();
    float padding = 50.0f;

    // 1. EFEK PARALLAX BACKGROUND
    scrollingBack -= 0.5f;
    if (scrollingBack <= -sw) scrollingBack = 0;

    if (bgGuide.id != 0) {
        DrawTextureEx(bgGuide, (Vector2){ scrollingBack, 0 }, 0.0f, 1.1f, WHITE);
        DrawTextureEx(bgGuide, (Vector2){ scrollingBack + sw, 0 }, 0.0f, 1.1f, WHITE);
    } else {
        ClearBackground(BLACK);
    }
    DrawRectangle(0, 0, (int)sw, (int)sh, (Color){ 10, 20, 35, 220 });

    // 2. KONTEN BOX
    Rectangle contentBox = { padding, 100, sw - (padding * 2), sh - 250 };
    DrawRectangleRoundedLines(contentBox, 0.1f, 10, SKYBLUE);

    // Area Visual (Kiri)
    Rectangle v = { contentBox.x + 20, contentBox.y + 20, (contentBox.width / 2) - 30, contentBox.height - 40 };
    DrawRectangleRounded(v, 0.1f, 10, (Color){ 30, 40, 55, 255 });

    // 3. LOGIKA VISUAL INTERAKTIF
    switch (currentSlide) {
        case 0: // KOMPONEN LIFT
            for (int i = 1; i <= 5; i++) {
                Rectangle r = { v.x + 20 + (i * 45), v.y + 60, 35, 35 };
                DrawButtonInteractive(r, TextFormat("%d", i), (Color){20, 40, 60, 255}, true);
            }
            DrawRectangleLinesEx((Rectangle){v.x + (v.width/2 - 40), v.y + 120, 80, 100}, 2, SKYBLUE);
            DrawText("LIFT CAR", v.x + (v.width/2 - 35), v.y + 230, 15, WHITE);
            break;

        case 1: // KONTROL ORANG
            static Person dummyP = { .state = PERSON_WAITING, .framesSpeed = 8 };
            dummyP.position = (Vector2){ v.x + v.width/2 - 20, v.y + v.height/2 - 20 };
            DrawPerson(&dummyP); 
            DrawButtonInteractive((Rectangle){ v.x + 40, v.y + 200, 80, 40 }, "P. IN", (Color){ 0, 121, 241, 255 }, true);
            DrawButtonInteractive((Rectangle){ v.x + 140, v.y + 200, 80, 40 }, "P. OUT", (Color){ 255, 161, 0, 255 }, true);
            break;

        case 2: // PINTU
            DrawButtonInteractive((Rectangle){v.x + 60, v.y + 100, 70, 45}, "< >", (Color){30, 150, 50, 255}, true);
            DrawButtonInteractive((Rectangle){v.x + 150, v.y + 100, 70, 45}, "> <", (Color){180, 40, 40, 255}, true);
            break;

        case 3: // POV
            DrawRectangleRounded((Rectangle){v.x + (v.width/2 - 30), v.y + 80, 60, 60}, 0.2f, 5, WHITE);
            DrawText("POV ICON", v.x + (v.width/2 - 30), v.y + 150, 12, SKYBLUE);
            break;
            
        case 4: // SHAFT
            DrawRectangle(v.x + v.width/2 - 5, v.y + 40, 10, v.height - 80, GRAY);
            DrawRectangle(v.x + v.width/2 - 15, v.y + 100, 30, 60, DARKGREEN);
            break;
    }

    // 4. TEKS PENJELASAN (Kanan)
    float textX = contentBox.x + (contentBox.width / 2) + 20;
    DrawText(slideTitles[currentSlide], (int)textX, (int)contentBox.y + 40, 22, SKYBLUE);
    DrawText(slideDescs[currentSlide], (int)textX, (int)contentBox.y + 100, 18, LIGHTGRAY);

    // 5. NAVIGASI
    Rectangle btnPrev = { padding, sh - 120, 120, 45 };
    Rectangle btnNext = { sw - padding - 120, sh - 120, 120, 45 };

    if (currentSlide > 0) {
        if (DrawButtonInteractive(btnPrev, "< PREV", DARKBLUE, true)) currentSlide--;
    }
    if (currentSlide < TOTAL_SLIDES - 1) {
        if (DrawButtonInteractive(btnNext, "NEXT >", DARKBLUE, true)) currentSlide++;
    }

    // Indikator Titik
    for (int i = 0; i < TOTAL_SLIDES; i++) {
        DrawCircle((int)(sw/2 - (TOTAL_SLIDES*15) + (i*30)), (int)(sh - 100), (i == currentSlide) ? 6 : 4, (i == currentSlide) ? SKYBLUE : DARKGRAY);
    }

    DrawText("Tekan [BACKSPACE] untuk Kembali", (int)sw/2 - 130, (int)sh - 40, 15, GRAY);
}