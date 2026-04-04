#include "../header/Guide.h"
#include "../header/UI.h"
#include "../header/Person.h"
#include "../header/Transformasi.h"
#include "../header/Lift.h"
#include "raylib.h"
#include <stddef.h>
#include <math.h>

// Forward declare dari MechView.c
void DrawRotatedCross(int cx, int cy, int radius, float angle, Color color);

// ============================================================
// TEXTURE & STATE LOKAL
// ============================================================
static Texture2D guideBg;
static Texture2D guidePov;

static int   currentSlide = 0;
static float slideTimer   = 0.0f;
static float pulleyAngle  = 0.0f;

// State animasi slide 2 (harus di luar switch agar static aman di C99)
static float  s2_animX   = -9999.0f;
static int    s2_animDir = 1;
static float  s2_animFT  = 0.0f;
static Person s2_person  = { .state = PERSON_WAITING, .framesSpeed = 6,
                              .dir   = FACING_RIGHT,   .currentFrame = 0 };
// State animasi slide 3
static Person s3_person  = { .state = PERSON_INSIDE, .dir = FACING_RIGHT,
                              .currentFrame = 0, .framesSpeed = 0 };

#define TOTAL_SLIDES 5

// ============================================================
// DATA SLIDE - teks penjelasan kanan
// ============================================================
static const char* slideTitles[TOTAL_SLIDES] = {
    "1. KOMPONEN UTAMA LIFT",
    "2. KONTROL ORANG (PERSON)",
    "3. SISTEM PINTU LIFT",
    "4. SUDUT PANDANG (POV)",
    "5. MEKANISME SHAFT & CW"
};

static const char* slideLines[][15] = {
    // === SLIDE 1 ===
    { "CAR   : Kotak pengangkut penumpang.",
      "SHAFT : Lorong vertikal tempat lift.",
      "FLOOR : Indikator garis tiap lantai.",
      "",
      "CARA MENGGUNAKAN LIFT:",
      "- Klik tombol ANGKA (1-5) di panel",
      "  untuk menggerakkan lift ke tujuan.",
      "- Pintu otomatis terbuka saat sampai.",
      "- Gunakan [ < > ] atau [ > < ] untuk",
      "  membuka/menutup pintu secara manual.", NULL },
      
    // === SLIDE 2 ===
    { "KONTROL PENUMPANG (PERSON):",
      "Gunakan tombol untuk mengontrol",
      "pergerakan penumpang (subjek tes).",
      "",
      "Tombol [ P. IN ] (Warna Biru):",
      "- Memerintahkan subjek MASUK ke lift.",
      "- Aktif HANYA jika lift tiba di lantai",
      "  yang sama & pintu terbuka penuh.",
      "",
      "Tombol [ P. OUT ] (Warna Oranye):",
      "- Memerintahkan subjek KELUAR dari lift.", NULL },

    // === SLIDE 3 ===
    { "SISTEM KONTROL PINTU LIFT:",
      "Pintu lift bergerak secara otomatis",
      "saat lift tiba di lantai tujuan.",
      "",
      "Kontrol Manual (Override):",
      "- Tombol HIJAU [ < > ] untuk MEMBUKA.",
      "- Tombol MERAH [ > < ] untuk MENUTUP.",
      "",
      "Catatan Keamanan:",
      "- Tombol manual HANYA berfungsi saat",
      "  lift diam (IDLE / pintu terbuka).",
      "- Pintu otomatis akan menutup kembali",
      "  setelah ditahan selama 3 detik.", NULL }, // <-- NULL ditambahkan di sini

    // === SLIDE 4 ===
    { "SUDUT PANDANG KAMERA (POV):",
      "Tekan ikon POV (Kanan Atas) atau",
      "tombol angka 1-4 di keyboard:",
      "",
      "[1] Default View : Tampilan penuh simulasi.",
      "[2] Building View: Zoom pada area gedung,",
      "    cocok untuk melihat penumpang.",
      "[3] Mech View    : Zoom pada ruang mesin,",
      "    fokus pada Motor & Counterweight.",
      "[4] Person View  : Kamera otomatis",
      "    mengikuti pergerakan karakter.", NULL },
      
    // === SLIDE 5 (MEKANISME DIPERJELAS) ===
    { "SISTEM MEKANIK & PENGGERAK (MECH):",
      "Lift ini menggunakan sistem Traksi,",
      "dimana Car & CW dihubungkan oleh kabel.",
      "",
      "- MOTOR & PULLEY: Mesin penarik kabel.",
      "  Berputar untuk menarik lift naik/turun.",
      "- COUNTERWEIGHT (CW): Pemberat penyeimbang.",
      "  Berfungsi menyeimbangkan berat Car.",
      "  Membuat kerja motor jauh lebih ringan",
      "  & hemat energi. Bergerak selalu",
      "  BERLAWANAN arah dengan pergerakan lift.",
      "- BUFFER: Pegas pengaman di dasar shaft",
      "  untuk meredam benturan jika lift anjlok.", NULL }
};

// ============================================================
// INIT / UNLOAD / UPDATE
// ============================================================
void InitGuide(void) {
    currentSlide  = 0;
    slideTimer    = 0.0f;
    pulleyAngle   = 0.0f;
    s2_animX      = -9999.0f;
    s2_animDir    = 1;
    guideBg  = LoadTexture("assets/Bg_Guide.png");  
    guidePov = LoadTexture("assets/pov.png");
    InitPersonModule();
}

void UnloadGuide(void) {
    UnloadTexture(guideBg);
    UnloadTexture(guidePov);
    UnloadPersonModule();
}

int UpdateGuide(void) {
    if (IsKeyPressed(KEY_BACKSPACE))                          return 1;
    if (IsKeyPressed(KEY_LEFT)  && currentSlide > 0)         currentSlide--;
    if (IsKeyPressed(KEY_RIGHT) && currentSlide < TOTAL_SLIDES-1) currentSlide++;
    return 0;
}

// ============================================================
// HELPER
// ============================================================
static void DrawLineArray(const char** lines, int x, int y, int fs, int gap, Color col) {
    for (int i = 0; lines[i] != NULL; i++)
        DrawText(lines[i], x, y + i * gap, fs, col);
}

// Klem nilai agar tidak keluar range
static float Clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

// ============================================================
// SLIDE 1 — BUILDING SIDE VIEW
// Referensi: BuildingView.c (floorHeight, shaftWidth, shaftX)
// Semua koordinat RELATIF ke 'vp' (inner panel)
// ============================================================
static void DrawSlide1(Rectangle vp) {
    // 1. Gedung digeser sedikit ke kiri
    float bW = vp.width * 0.45f; 
    float bH = vp.height * 0.85f; 
    float bX = vp.x + 30; // Jarak dari tepi kiri
    float bY = vp.y + (vp.height - bH) / 2.0f;

    // Background Gedung
    DrawRectangle((int)bX, (int)bY, (int)bW, (int)bH, (Color){20, 30, 45, 255});
    DrawRectangleLines((int)bX, (int)bY, (int)bW, (int)bH, (Color){50, 90, 130, 255});

    // Shaft (Lorong Lift)
    float shW = bW * 0.45f;
    float shX = bX + (bW - shW) / 2.0f;
    DrawRectangle((int)shX, (int)bY, (int)shW, (int)bH, (Color){10, 15, 25, 255});
    DrawRectangleLines((int)shX, (int)bY, (int)shW, (int)bH, SKYBLUE);

    // Garis Lantai & Label
    int nFloor = 5;
    float fh = bH / nFloor;
    for (int i = 0; i < nFloor; i++) {
        float lineY = bY + i * fh;
        DrawLine((int)(bX - 20), (int)lineY, (int)(bX + bW), (int)lineY, (Color){50, 90, 130, 150});
        DrawText(TextFormat("LT %d", 5 - i), (int)(bX - 55), (int)(lineY - 6), 11, GRAY);
    }
    DrawLine((int)(bX - 20), (int)(bY + bH), (int)(bX + bW), (int)(bY + bH), (Color){50, 90, 130, 150}); // Garis dasar

    // Mobil Lift (CAR) di Lantai 1
    float carH = fh * 0.8f;
    float carW = shW * 0.8f;
    float carX = shX + (shW - carW) / 2.0f;
    float carY = bY + bH - carH - (fh - carH)/2.0f; 

    // Gambar Pintu Lift
    DrawRectangle((int)carX, (int)carY, (int)carW, (int)carH, (Color){25, 65, 105, 255});
    DrawRectangleLines((int)carX, (int)carY, (int)carW, (int)carH, SKYBLUE);
    DrawLine((int)(carX + carW/2), (int)carY, (int)(carX + carW/2), (int)(carY + carH), (Color){10, 30, 50, 255});
    
    // Indikator lantai
    DrawRectangle((int)(carX + carW/2 - 12), (int)carY - 14, 24, 12, (Color){10, 20, 35, 255});
    DrawText("1", (int)(carX + carW/2 - 3), (int)carY - 12, 10, GREEN);

    // Label Penunjuk
    float labelX = bX + bW + 15;
    DrawLine((int)(shX + shW), (int)(bY + bH/3), (int)labelX, (int)(bY + bH/3), GRAY);
    DrawText("SHAFT", (int)labelX + 5, (int)(bY + bH/3 - 6), 11, SKYBLUE);

    DrawLine((int)(carX + carW), (int)(carY + carH/2), (int)labelX, (int)(carY + carH/2), GRAY);
    DrawText("CAR", (int)labelX + 5, (int)(carY + carH/2 - 6), 11, SKYBLUE);

    // =========================================================
    // PANEL KONTROL SIMULASI (PERSIS "IN-CAR")
    // =========================================================
    float panelW = 140; // Lebar proporsional aslinya
    float panelH = 240; // Tinggi proporsional aslinya
    float panelX = vp.x + vp.width - panelW - 30; // Posisi di sisi kanan panel
    float panelY = bY + (bH - panelH) / 2.0f;     // Posisi vertikal tepat di tengah gedung

    // Background Kotak Panel
    DrawRectangleRounded((Rectangle){panelX, panelY, panelW, panelH}, 0.1f, 5, (Color){15, 25, 40, 255});
    DrawRectangleRoundedLines((Rectangle){panelX, panelY, panelW, panelH}, 0.1f, 5, SKYBLUE);
    
    // Judul Panel
    int titleW = MeasureText("IN-CAR", 12);
    DrawText("IN-CAR", (int)(panelX + (panelW - titleW)/2), (int)panelY + 15, 12, SKYBLUE);

    // Tombol Angka (Layout Zig-Zag Persis Aslinya)
    for (int i = 5; i >= 1; i--) {
        int btnX = (int)panelX + ((i % 2 == 1) ? 35 : 95);
        int btnY = (int)panelY + 60 + ((5 - i) * 25);
        Rectangle r = { btnX - 18, btnY - 18, 36, 36 };
        
        // Tombol 1 di-highlight Biru Terang (karena lift sedang di LT 1)
        Color btnColor = (i == 1) ? (Color){0, 160, 255, 255} : (Color){20, 40, 60, 255}; 
        DrawButtonInteractive(r, TextFormat("%d", i), btnColor, true);
    }

    // Tombol Buka Tutup Pintu di Bawah Angka
    Rectangle btnBuka = {panelX + 15, panelY + 190, 50, 34};
    Rectangle btnTutup = {panelX + 75, panelY + 190, 50, 34};
    
    DrawButtonInteractive(btnBuka, "< >", (Color){30, 150, 50, 255}, true);
    DrawButtonInteractive(btnTutup, "> <", (Color){180, 40, 40, 255}, true);
}

// ============================================================
// SLIDE 2 — PERSON DEMO
// Referensi: Person.c (walkSpeed), UI.c (DrawButtonInteractive)
// ============================================================
static void DrawSlide2(Rectangle vp) {
    float dt = GetFrameTime();
    float cx = vp.x + vp.width / 2.0f;
    float cy = vp.y + vp.height / 2.0f;

    // Garis Lantai
    float floorY = cy + 60;
    DrawLine((int)(vp.x + 20), (int)floorY, (int)(vp.x + vp.width - 20), (int)floorY, GRAY);

    // Kotak Lift (Diberi fill warna biru gelap agar elegan)
    float liftW = 100;
    float liftH = 140;
    float liftX = vp.x + vp.width - liftW - 50;
    float liftY = floorY - liftH;
    
    DrawRectangle((int)liftX, (int)liftY, (int)liftW, (int)liftH, (Color){25, 40, 60, 255});
    DrawRectangleLines((int)liftX, (int)liftY, (int)liftW, (int)liftH, SKYBLUE);
    DrawText("LIFT", (int)(liftX + liftW/2 - MeasureText("LIFT", 14)/2), (int)(liftY - 20), 14, SKYBLUE);

    // Logika Jalan
    if (s2_animX < vp.x) s2_animX = vp.x + 40;
    float targetIn = liftX + liftW/2 - 15;
    float targetOut = vp.x + 40;

    s2_animFT += dt;
    if (s2_animFT > 0.1f) {
        s2_animFT = 0;
        s2_person.currentFrame = (s2_person.currentFrame + 1) % 4;
    }

    if (s2_animDir == 1) {
        s2_animX += 80.0f * dt;
        s2_person.dir = FACING_RIGHT;
        if (s2_animX >= targetIn) { s2_animX = targetIn; s2_animDir = -1; }
    } else {
        s2_animX -= 80.0f * dt;
        s2_person.dir = FACING_LEFT;
        if (s2_animX <= targetOut) { s2_animX = targetOut; s2_animDir = 1; }
    }

    // ==========================================
    // PERBAIKAN: Angkat orang ke atas garis
    // ==========================================
    // Sebelumnya 32, sekarang saya naikkan jadi 54. 
    // Jika masih kurang menapak, tambah angkanya (misal 58). 
    // Jika terlalu melayang, kurangi (misal 48).
    float personOffset = 60.0f; 
    s2_person.position = (Vector2){ s2_animX, floorY - personOffset };
    DrawPerson(&s2_person);

    // Tombol Interaktif
    float btnW = 80, btnH = 36;
    float btnY = floorY + 30;
    DrawButtonInteractive((Rectangle){ cx - btnW - 10, btnY, btnW, btnH }, "P. IN", (Color){0, 121, 241, 255}, true);
    DrawButtonInteractive((Rectangle){ cx + 10, btnY, btnW, btnH }, "P. OUT", (Color){255, 161, 0, 255}, true);
}

// ============================================================
// SLIDE 3 — PINTU ANIMASI
// Referensi: BuildingView.c (doorWidth, doorOpenness)
//            UI.c (btnBuka, btnTutup, ikon panah)
// ============================================================
static void DrawSlide3(Rectangle vp, float timer) {
    float cx = vp.x + vp.width / 2.0f;
    float cy = vp.y + vp.height / 2.0f;

    // Perbesar ukuran lift
    float cW = 180;
    float cH = 220;
    float cX = cx - cW / 2.0f;
    float cY = cy - cH / 2.0f - 20;

    float openness = (sinf(timer * 2.0f) + 1.0f) / 2.0f;

    // Interior Lift (Diberi warna latar agar tidak tembus pandang)
    DrawRectangle((int)cX, (int)cY, (int)cW, (int)cH, (Color){25, 40, 60, 255});
    DrawRectangleLines((int)cX, (int)cY, (int)cW, (int)cH, SKYBLUE);

    // ==========================================
    // PERBAIKAN POSISI ORANG:
    // (cY + cH) adalah lantai dasar kotak lift.
    // Kita kurangi dengan offset 54 (seperti di slide 2) agar pas napak.
    // ==========================================
    float personOffset = 60.0f; 
    s3_person.position = (Vector2){ cx - 14, (cY + cH) - personOffset };
    DrawPerson(&s3_person);

    // Pintu
    float dw = (cW / 2.0f) * (1.0f - openness);
    if (dw > 0.5f) {
        DrawRectangle((int)cX, (int)cY, (int)dw, (int)cH, (Color){20, 60, 100, 255});
        DrawRectangleLines((int)cX, (int)cY, (int)dw, (int)cH, SKYBLUE);
        
        DrawRectangle((int)(cX + cW - dw), (int)cY, (int)dw, (int)cH, (Color){20, 60, 100, 255});
        DrawRectangleLines((int)(cX + cW - dw), (int)cY, (int)dw, (int)cH, SKYBLUE);
    }

    // Teks Persentase
    int pct = (int)(openness * 100);
    const char* text = TextFormat("Pintu Terbuka: %d%%", pct);
    DrawText(text, (int)(cx - MeasureText(text, 16)/2), (int)(cY + cH + 15), 16, openness > 0.5f ? GREEN : RED);

    // Tombol
    float btnW = 70, btnH = 40;
    float btnY = cY + cH + 45;
    DrawButtonInteractive((Rectangle){ cx - btnW - 10, btnY, btnW, btnH }, "< >", (Color){30, 150, 50, 255}, true);
    DrawButtonInteractive((Rectangle){ cx + 10, btnY, btnW, btnH }, "> <", (Color){180, 40, 40, 255}, true);
}

// ============================================================
// SLIDE 4 — POV DEMO
// Referensi: UI.c (DrawSimulationUI — bagian MENU POV)
// ============================================================
static void DrawSlide4(Rectangle vp, float timer) {
    float cx = vp.x + vp.width / 2.0f;

    // Perbesar Ikon
    float povSize = 64;
    float btnX = cx - povSize / 2.0f;
    float btnY = vp.y + 40;

    DrawRectangleRounded((Rectangle){btnX, btnY, povSize, povSize}, 0.2f, 5, WHITE);
    DrawRectangleRoundedLines((Rectangle){btnX, btnY, povSize, povSize}, 0.2f, 5, DARKGRAY);

    if (guidePov.id != 0) {
        DrawTexturePro(guidePov,
            (Rectangle){0, 0, (float)guidePov.width, (float)guidePov.height},
            (Rectangle){btnX+12, btnY+12, povSize-24, povSize-24},
            (Vector2){0,0}, 0.0f, WHITE);
    }

    const char* title = "Simulasi Menu Kamera POV";
    DrawText(title, (int)(cx - MeasureText(title, 14)/2), (int)(btnY + povSize + 15), 14, SKYBLUE);

    // Perbesar & Rapikan Dropdown
    int menuW = 200;
    int itemH = 40;
    float mX = cx - menuW / 2.0f;
    float mY = btnY + povSize + 40;

    DrawRectangleRounded((Rectangle){mX, mY, (float)menuW, (float)(itemH * 4)}, 0.1f, 5, (Color){20, 30, 45, 255});
    DrawRectangleRoundedLines((Rectangle){mX, mY, (float)menuW, (float)(itemH * 4)}, 0.1f, 5, SKYBLUE);

    const char* options[4] = { "Default View", "Building View", "Mech View", "Person View" };
    int highlight = (int)(timer * 1.5f) % 4;

    for (int i = 0; i < 4; i++) {
        Rectangle itemRect = { mX, mY + (i * itemH), (float)menuW, (float)itemH };
        if (i == highlight) DrawRectangleRec(itemRect, (Color){50, 100, 150, 200});
        DrawText(options[i], (int)(itemRect.x + 20), (int)(itemRect.y + 14), 14, i == highlight ? WHITE : LIGHTGRAY);
    }
}

// ============================================================
// SLIDE 5 — SHAFT & COUNTERWEIGHT
// Referensi: MechView.c (shaftCenter, pulleyY, rLeft, rRight,
//            cxLeft, cxRight, cwX, mCarWidth, mCarHeight)
// Semua offset dihitung proporsional dari panel
// ============================================================
static void DrawSlide5(Rectangle vp, float timer) {
    // Shaft center di tengah panel (persis MechView.c pakai shaftCenter)
    float shaftCenterF = vp.x + vp.width / 2.0f;
    int   shaftCenter  = (int)shaftCenterF;

    // Shaft height = panel minus ruang motor atas & buffer bawah
    float shTop = vp.y + 30;
    float shBot = vp.y + vp.height - 30;
    float shH   = shBot - shTop;
    int   shW   = 130;   // lebar shaft, sama dengan MechView.c (150-20 margin)

    // Shaft walls (dua kotak seperti MechView.c)
    DrawRectCustom(shaftCenter - shW/2,    (int)shTop, shW,    (int)shH, (Color){40,50,60,255});
    DrawRectCustom(shaftCenter - shW/2+10, (int)shTop, shW-20, (int)shH, DARKGRAY);

    // Motor di atas shaft — persis MechView.c
    for (int i = 0; i < 22; i++)
        DrawLineBresenham(shaftCenter-44, (int)(shTop-22)+i,
                          shaftCenter+44, (int)(shTop-22)+i, (Color){60,30,90,255});
    DrawRectCustom(shaftCenter-44, (int)(shTop-22), 88, 22, VIOLET);
    int mw = MeasureText("MOTOR", 13);
    DrawText("MOTOR", shaftCenter - mw/2, (int)(shTop-18), 13, LIGHTGRAY);

    // Pulley — persis MechView.c (cxLeft/cxRight, rLeft/rRight)
    int pulleyY = (int)(shTop + 22);
    int rLeft   = 18;
    int rRight  = 14;
    int cxLeft  = shaftCenter - 22;
    int cxRight = shaftCenter + 22;

    DrawCircleMidpoint(cxLeft,  pulleyY, rLeft,  PURPLE);
    DrawCircleMidpoint(cxRight, pulleyY, rRight, PURPLE);
    DrawRotatedCross(cxLeft,  pulleyY, rLeft,   pulleyAngle, VIOLET);
    DrawRotatedCross(cxRight, pulleyY, rRight, -pulleyAngle, VIOLET);

    // Kabel atas antar pulley
    DrawLineBresenham(cxLeft, pulleyY - rLeft, cxRight, pulleyY - rRight, LIGHTGRAY);

    // Animasi car & CW berlawanan (sinusoidal) — persis Lift.c cw_y logic
    float travel = shH * 0.30f;
    float bounce = sinf(timer * 0.9f);

    // mCarHeight dari MechView.c: (sh - 250) / 5.0f - 10
    // Di sini kita proporsionalkan terhadap panel
    int mCarH = (int)(shH / 5.0f) - 4;
    int mCarW = 58;
    int mCarX = shaftCenter - mCarW - 4;

    int cY = (int)(shTop + rLeft*2 + 4 + travel/2 + bounce * travel);
    // Klem car agar tidak keluar shaft
    cY = (int)Clampf((float)cY, shTop + rLeft*2 + 4, shBot - mCarH - 4);

    int cwH = mCarH;
    int cwW = 40;
    int cwX = shaftCenter + 4;   // persis MechView.c cwX = shaftCenter + 5
    int cwY = (int)(shTop + rLeft*2 + 4 + travel/2 - bounce * travel);
    cwY = (int)Clampf((float)cwY, shTop + rLeft*2 + 4, shBot - cwH - 4);

    // Kabel car (kiri) — persis MechView.c: cxLeft - rLeft ke lift->y
    DrawLineBresenham(cxLeft - rLeft, pulleyY, cxLeft - rLeft, cY, LIGHTGRAY);
    // Kabel CW (kanan) — persis MechView.c: cxRight + rRight ke lift->cw_y
    DrawLineBresenham(cxRight + rRight, pulleyY, cxRight + rRight, cwY, LIGHTGRAY);

    // CAR — persis MechView.c (SKYBLUE, silang, indicator lantai)
    DrawRectCustom(mCarX-4, cY-4, mCarW+8, mCarH+8, (Color){100,200,255,80});
    DrawRectCustom(mCarX,   cY,   mCarW,   mCarH,   SKYBLUE);
    DrawLineBresenham(mCarX,       cY,       mCarX+mCarW, cY+mCarH, (Color){50,100,150,150});
    DrawLineBresenham(mCarX+mCarW, cY,       mCarX,       cY+mCarH, (Color){50,100,150,150});
    DrawRectangle(mCarX+4, cY+4, 26, 16, (Color){10,20,35,255});
    DrawRectCustom(mCarX+4, cY+4, 26, 16, SKYBLUE);
    DrawText("3", mCarX+14, cY+6, 13, GREEN);

    // COUNTERWEIGHT — persis MechView.c (green fill loop + border)
    for (int i = 0; i < cwH; i++)
        DrawLineBresenham(cwX, cwY+i, cwX+cwW, cwY+i, (Color){40,100,50,255});
    DrawRectCustom(cwX, cwY, cwW, cwH, GREEN);
    DrawText("CW",    cwX+8,  cwY+4,        13, LIME);
    DrawText("-1500", cwX+2,  cwY+cwH/2-8,  12, LIME);
    DrawText("kg",    cwX+10, cwY+cwH/2+4,  12, LIME);

    // Buffer bawah — persis MechView.c
    int bufY = (int)(shBot - 18);
    int bfW  = 28;
    DrawRectCustom(shaftCenter - bfW*2,    bufY, bfW, 18, MAROON);
    DrawRectCustom(shaftCenter + bfW/2+4,  bufY, bfW, 18, MAROON);
    int bufw = MeasureText("BUFFER", 12);
    DrawText("BUFFER", shaftCenter - bufw/2, bufY + 20, 12, RED);

    // Label anotasi — di DALAM panel, sisi kanan shaft
    int labX  = shaftCenter + shW/2 + 8;
    int labMax = (int)(vp.x + vp.width - 4);

    // Hanya gambar label jika masih muat (minimal 40px untuk teks terpendek)
    if (labX + 40 < labMax) {
        // Garis penghubung + teks
        DrawLineBresenham(shaftCenter+44, (int)(shTop-10), labX-2, (int)(shTop-10), (Color){100,100,150,130});
        DrawText("Motor",  labX, (int)(shTop-18), 12, VIOLET);

        DrawLineBresenham(cxRight+rRight, pulleyY, labX-2, pulleyY, (Color){100,100,150,130});
        DrawText("Pulley", labX, pulleyY-8, 12, PURPLE);

        int midKabel = (int)(shTop + shH * 0.25f);
        DrawLineBresenham(cxRight+rRight, midKabel, labX-2, midKabel, (Color){100,100,150,130});
        DrawText("Kabel",  labX, midKabel-8, 12, LIGHTGRAY);

        DrawLineBresenham(mCarX+mCarW, cY+mCarH/2, labX-2, cY+mCarH/2, (Color){50,100,200,130});
        DrawText("Car",    labX, cY+mCarH/2-8, 12, SKYBLUE);

        DrawLineBresenham(cwX+cwW, cwY+cwH/2, labX-2, cwY+cwH/2, (Color){50,150,50,130});
        DrawText("CW",     labX, cwY+cwH/2-8, 12, GREEN);

        DrawLineBresenham(shaftCenter+shW/2, bufY+9, labX-2, bufY+9, (Color){150,30,30,130});
        DrawText("Buffer", labX, bufY+2, 12, RED);
    }
}

// ============================================================
// DRAW GUIDE SCREEN — UTAMA
// ============================================================
void DrawGuideScreen(void) {
    float sw = (float)GetScreenWidth();
    float sh = (float)GetScreenHeight();
    float dt = GetFrameTime();
    slideTimer  += dt;
    pulleyAngle += 1.8f * dt;

    // ----------------------------------------------------------
    // A. PARALLAX BACKGROUND — persis Menu.c / DrawMainBackground()
    // ----------------------------------------------------------
    float offsetX = (GetMousePosition().x - sw/2.0f) * 0.03f;
    float offsetY = (GetMousePosition().y - sh/2.0f) * 0.03f;
    float bgW = sw * 1.1f;
    float bgH = sh * 1.1f;

    if (guideBg.id != 0) {
        DrawTexturePro(guideBg,
            (Rectangle){0, 0, (float)guideBg.width, (float)guideBg.height},
            (Rectangle){(sw-bgW)/2.0f - offsetX, (sh-bgH)/2.0f - offsetY, bgW, bgH},
            (Vector2){0,0}, 0.0f, GRAY);
    } else {
        ClearBackground((Color){10,15,25,255});
    }
    DrawRectangle(0, 0, (int)sw, (int)sh, (Color){10, 20, 35, 150});

    // ----------------------------------------------------------
    // B. JUDUL
    // ----------------------------------------------------------
    const char* title = "PANDUAN SIMULASI LIFT";
    int tw = MeasureText(title, 26);
    DrawText(title, (int)(sw/2 - tw/2), 12, 26, SKYBLUE);
    DrawLineBresenham(28, 48, (int)sw - 28, 48, (Color){30,80,120,200});

    // ----------------------------------------------------------
    // C. DUA PANEL BERDAMPINGAN
    // ----------------------------------------------------------
    float padOuter = 28.0f;
    float panelY   = 55.0f;
    float navH     = 88.0f;             // tinggi area navigasi bawah
    float panelH   = sh - panelY - navH;
    float gap      = 10.0f;
    float panelW   = (sw - padOuter*2 - gap) / 2.0f;

    Rectangle vPanel = { padOuter,               panelY, panelW, panelH };
    Rectangle tPanel = { padOuter + panelW + gap, panelY, panelW, panelH };

    DrawRectangleRounded(vPanel, 0.03f, 10, (Color){18, 28, 42, 245});
    DrawRectangleRoundedLines(vPanel, 0.03f, 10, (Color){30, 90, 150, 255});

    DrawRectangleRounded(tPanel, 0.03f, 10, (Color){13, 20, 33, 245});
    DrawRectangleRoundedLines(tPanel, 0.03f, 10, (Color){30, 90, 150, 255});

    // Inner area panel kiri — margin 10px semua sisi
    float margin = 10.0f;
    Rectangle inner = {
        vPanel.x + margin,
        vPanel.y + margin,
        vPanel.width  - margin*2,
        vPanel.height - margin*2
    };

    // ----------------------------------------------------------
    // D. VISUAL DEMO PER SLIDE
    // ----------------------------------------------------------
    switch (currentSlide) {
        case 0: DrawSlide1(inner); break;
        case 1: DrawSlide2(inner); break;
        case 2: DrawSlide3(inner, slideTimer); break;
        case 3: DrawSlide4(inner, slideTimer); break;
        case 4: DrawSlide5(inner, slideTimer); break;
    }

    // ----------------------------------------------------------
    // E. TEKS PENJELASAN — panel kanan
    // ----------------------------------------------------------
    int tx = (int)(tPanel.x + 16);
    int ty = (int)(tPanel.y + 16);

    DrawText(slideTitles[currentSlide], tx, ty, 17, SKYBLUE);
    DrawLineBresenham(tx, ty + 24,
                      (int)(tPanel.x + tPanel.width - 16), ty + 24,
                      (Color){30,80,130,200});
    DrawLineArray(slideLines[currentSlide], tx, ty + 36, 14, 22, LIGHTGRAY);

    // Nomor slide — kanan bawah panel teks
    const char* sn = TextFormat("Slide %d / %d", currentSlide + 1, TOTAL_SLIDES);
    int snw = MeasureText(sn, 13);
    DrawText(sn,
             (int)(tPanel.x + tPanel.width - snw - 12),
             (int)(tPanel.y + tPanel.height - 20), 13, DARKGRAY);

    // ----------------------------------------------------------
    // F. NAVIGASI PREV / NEXT
    // ----------------------------------------------------------
    float navY = sh - navH + 12;

    if (currentSlide > 0) {
        if (DrawButtonInteractive((Rectangle){padOuter, navY, 120, 42},
                                  "< PREV", DARKBLUE, true))
            currentSlide--;
    }
    if (currentSlide < TOTAL_SLIDES - 1) {
        if (DrawButtonInteractive((Rectangle){sw - padOuter - 120, navY, 120, 42},
                                  "NEXT >", DARKBLUE, true))
            currentSlide++;
    }

    // Dot indicator
    for (int i = 0; i < TOTAL_SLIDES; i++) {
        bool active = (i == currentSlide);
        DrawCircle((int)(sw/2.0f - TOTAL_SLIDES*10 + i*20),
                   (int)(navY + 21), // <--- INI YANG DIGANTI (sebelumnya navY + 52)
                   active ? 7 : 4,
                   active ? SKYBLUE : DARKGRAY);
    }

    // ----------------------------------------------------------
    // G. FOOTER
    // ----------------------------------------------------------
    const char* footer = "Tekan [BACKSPACE] untuk Kembali  |  [  <  >  ] navigasi slide";
    int fw = MeasureText(footer, 13);
    DrawText(footer, (int)(sw/2 - fw/2), (int)(sh - 22), 13, GRAY);
}