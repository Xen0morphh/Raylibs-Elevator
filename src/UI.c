#include "../header/UI.h"
#include "raylib.h"
#include "../header/Lift.h"
#include "../header/Transformasi.h"
#include "../header/Person.h"
#include "../header/Camera.h"

static Texture2D bgMain;
static bool showPovMenu = false;

static bool wireframeMode = false;

bool GetWireframeMode(void) { 
    return wireframeMode; 
}



bool DrawButtonInteractive(Rectangle rect, const char* text, Color baseColor, bool enabled) {
    Vector2 mousePoint = GetMousePosition();
    bool clicked = false;
    Color displayColor = baseColor;
    int offset = 0;

    if (!enabled) {
        displayColor = (Color){ 80, 80, 80, 255 }; // Warna abu-abu jika mati
    } else {
        if (CheckCollisionPointRec(mousePoint, rect)) {
            displayColor = ColorBrightness(baseColor, 0.2f); // Terang saat hover
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                offset = 2; // Efek masuk ke dalam
                displayColor = ColorBrightness(baseColor, -0.3f); // Gelap saat ditekan
            }
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) clicked = true;
        }
    }

    // Gambar Bayangan/Border Bawah (Efek 3D)
    if (enabled) DrawRectangleRounded((Rectangle){ rect.x, rect.y + 2, rect.width, rect.height }, 0.3f, 5, DARKGRAY);
    
    // Gambar Tombol Utama
    DrawRectangleRounded((Rectangle){ rect.x, rect.y + offset, rect.width, rect.height }, 0.3f, 5, displayColor);
    DrawRectangleRoundedLines((Rectangle){ rect.x, rect.y + offset, rect.width, rect.height }, 0.3f, 5, enabled ? SKYBLUE : GRAY);
    
    // Gambar Teks (Ikut offset saat ditekan)
    int fontSize = 12;
    int textX = rect.x + (rect.width - MeasureText(text, fontSize)) / 2;
    int textY = rect.y + (rect.height - fontSize) / 2 + offset;
    DrawText(text, textX, textY, fontSize, enabled ? WHITE : LIGHTGRAY);

    return (enabled && clicked);
}

void InitUI(void) {
    bgMain = LoadTexture("assets/Bg_Main.png");
    // povIcon SUDAH DIMUSNAHKAN!
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
    // povIcon SUDAH DIMUSNAHKAN!
}

void DrawSimulationUI(Elevator* lift, Person* p) {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();

    // KOTAK INFO STATUS (Kiri Atas)
    static int  displayFloor  = 1;   // Lantai yang sedang ditampilkan
    static int  prevFloor     = 1;   // Lantai sebelumnya (yang sedang slide keluar)
    static float animTimer    = 1.0f; // 0.0 = mulai animasi, 1.0 = selesai
    static int  animDir       = 1;   // +1 = naik (angka masuk dari bawah), -1 = turun

    float animSpeed = 8.0f; // Kecepatan animasi (semakin besar semakin cepat)

    // Deteksi pergantian lantai → mulai animasi
    if (lift->currentFloor != displayFloor) {
        prevFloor    = displayFloor;
        displayFloor = lift->currentFloor;
        animDir      = (displayFloor > prevFloor) ? 1 : -1;
        animTimer    = 0.0f; // Reset timer animasi
    }

    // Update timer animasi setiap frame
    if (animTimer < 1.0f) {
        animTimer += GetFrameTime() * animSpeed;
        if (animTimer > 1.0f) animTimer = 1.0f;
    }

    // Easing: ease-out (decelerates near end)
    float t = 1.0f - (1.0f - animTimer) * (1.0f - animTimer);

    int infoX = 20;
    int infoY = 20;
    int boxH  = 80;
    int numFontSize = 30;

    DrawRectangleRounded((Rectangle){infoX, infoY, 120, boxH}, 0.2f, 10, (Color){ 10, 20, 35, 230 });
    DrawRectangleRoundedLines((Rectangle){infoX, infoY, 120, boxH}, 0.2f, 10, (Color){ 30, 80, 120, 255 });

    // Scissor (clip) agar angka tidak keluar kotak
    BeginScissorMode(infoX + 5, infoY + 5, 110, 50);

    // Anchor tengah angka di dalam kotak
    int numCenterX = infoX + 45;
    int numBaseY   = infoY + 15; // posisi Y akhir angka baru

    int slideRange = 40; // jarak slide dalam pixel

    // --- Angka LAMA (slide keluar) ---
    if (animTimer < 1.0f) {
        int oldOffsetY = (int)(t * slideRange * -animDir); // keluar ke arah berlawanan
        unsigned char alpha = (unsigned char)((1.0f - t) * 255);
        Color oldColor = (Color){ 135, 206, 235, alpha }; // SKYBLUE memudar
        DrawText(TextFormat("%d", prevFloor),
                 numCenterX, numBaseY + oldOffsetY,
                 numFontSize, oldColor);
    }

    // --- Angka BARU (slide masuk) ---
    {
        int newOffsetY = (int)((1.0f - t) * slideRange * animDir); // masuk dari arah animDir
        unsigned char alpha = (unsigned char)(t * 255);
        Color newColor = (Color){ 135, 206, 235, alpha }; // SKYBLUE muncul
        DrawText(TextFormat("%d", displayFloor),
                 numCenterX, numBaseY + newOffsetY,
                 numFontSize, newColor);
    }

    EndScissorMode();

    // Teks Arah (NAIK/TURUN/DIAM) tetap di bawah angka
    const char* dirText = "DIAM -";
    Color dirColor = GRAY;
    if (lift->state == MOVING_UP)   { dirText = "NAIK ^";  dirColor = GREEN; }
    else if (lift->state == MOVING_DOWN) { dirText = "TURUN v"; dirColor = RED; }
    DrawText(dirText, infoX + 30, infoY + 55, 15, dirColor);

    // PANEL KONTROL IN-CAR (Kiri Bawah)
    int panelX = 20; 
    int panelY = sh - 350; 
    DrawRectangleRounded((Rectangle){panelX, panelY, 140, 240}, 0.2f, 10, (Color){ 10, 20, 35, 230 });
    DrawRectangleRoundedLines((Rectangle){panelX, panelY, 140, 240}, 0.2f, 10, SKYBLUE);
    DrawText("IN-CAR", panelX + 45, panelY + 15, 15, SKYBLUE);

    // ==========================================
    // LOGIKA TOMBOL ANGKA YANG LEBIH RESPONSIF
    // ==========================================
    for (int i = 5; i >= 1; i--) {
        int btnX = panelX + ((i % 2 == 1) ? 35 : 95); 
        int btnY = panelY + 60 + ((5 - i) * 25);
        Rectangle r = { btnX - 18, btnY - 18, 36, 36 };
        
        bool isTarget = (lift->targetFloor == i);
        Color btnColor = isTarget ? (Color){0, 160, 255, 255} : (Color){20, 40, 60, 255}; 
        bool canPress = (lift->state == IDLE || lift->state == DOOR_OPEN);

        if (DrawButtonInteractive(r, TextFormat("%d", i), btnColor, canPress)) {
            lift->targetFloor = i;
            if (lift->state == DOOR_OPEN || lift->state == DOOR_OPENING) { 
                lift->timer = 0; 
                lift->state = DOOR_CLOSING; 
            }
        }
    }

    // ==========================================
    // LOGIKA TOMBOL BUKA / TUTUP PINTU
    // ==========================================
    Rectangle btnBuka = {panelX + 15, panelY + 190, 50, 30};
    Rectangle btnTutup = {panelX + 75, panelY + 190, 50, 30};
    
    // 1. Tombol BUKA (Hijau Gelap)
    if (DrawButtonInteractive(btnBuka, "", (Color){30, 150, 50, 255}, true)) {
        TriggerOpenDoor(lift);
    }
    DrawLineEx((Vector2){btnBuka.x + 22, btnBuka.y + 10}, (Vector2){btnBuka.x + 15, btnBuka.y + 15}, 2, WHITE); 
    DrawLineEx((Vector2){btnBuka.x + 15, btnBuka.y + 15}, (Vector2){btnBuka.x + 22, btnBuka.y + 20}, 2, WHITE); 
    DrawLineEx((Vector2){btnBuka.x + 28, btnBuka.y + 10}, (Vector2){btnBuka.x + 35, btnBuka.y + 15}, 2, WHITE); 
    DrawLineEx((Vector2){btnBuka.x + 35, btnBuka.y + 15}, (Vector2){btnBuka.x + 28, btnBuka.y + 20}, 2, WHITE); 

    // 2. Tombol TUTUP (Merah Gelap)
    if (DrawButtonInteractive(btnTutup, "", (Color){180, 40, 40, 255}, true)) {
        TriggerCloseDoor(lift);
    }
    DrawLineEx((Vector2){btnTutup.x + 15, btnTutup.y + 10}, (Vector2){btnTutup.x + 22, btnTutup.y + 15}, 2, WHITE); 
    DrawLineEx((Vector2){btnTutup.x + 22, btnTutup.y + 15}, (Vector2){btnTutup.x + 15, btnTutup.y + 20}, 2, WHITE); 
    DrawLineEx((Vector2){btnTutup.x + 35, btnTutup.y + 10}, (Vector2){btnTutup.x + 28, btnTutup.y + 15}, 2, WHITE); 
    DrawLineEx((Vector2){btnTutup.x + 28, btnTutup.y + 15}, (Vector2){btnTutup.x + 35, btnTutup.y + 20}, 2, WHITE);

    // ==========================================
    // TEKS STATUS BAR BAWAH
    // ==========================================
    const char* stateStr = "IDLE";
    if (lift->state == MOVING_UP) stateStr = "MOVING_UP";
    else if (lift->state == MOVING_DOWN) stateStr = "MOVING_DOWN";
    else if (lift->state == DOOR_OPENING) stateStr = "OPENING";
    else if (lift->state == DOOR_OPEN) stateStr = "OPEN (WAIT)";
    else if (lift->state == DOOR_CLOSING) stateStr = "CLOSING";

    const char* doorStr = (lift->doorOpenness == 0.0f) ? "TERTUTUP" :
                          (lift->doorOpenness == 1.0f) ? "TERBUKA" : "PROSES...";

    DrawText(TextFormat("STATE: %s", stateStr), 20, sh - 30, 20, GREEN);
    DrawText(TextFormat("FLOOR: %d", lift->currentFloor), 250, sh - 30, 20, DARKGRAY);
    DrawText(TextFormat("TARGET: %d", lift->targetFloor), 380, sh - 30, 20, DARKGRAY);
    DrawText(TextFormat("DOOR: %s", doorStr), 520, sh - 30, 20, ORANGE);

    const char* helpText = "ESC: Exit Program  |  BACKSPACE: Kembali ke Menu";
    int textWidth = MeasureText(helpText, 15);
    DrawText(helpText, sw - textWidth - 20, sh - 28, 15, DARKGRAY);

    // ==========================================
    // TOMBOL KONTROL ORANG (Person Control)
    // ==========================================   
    int pControlY = panelY + 250; 
    DrawText("PERSON CONTROL", panelX + 15, pControlY, 12, GRAY);

    bool canIn  = (lift->doorOpenness >= 0.9f)
               && (p->state == PERSON_WAITING)
               && (lift->currentFloor == p->startFloor);

    bool canOut = (lift->doorOpenness >= 0.9f)
               && (p->state == PERSON_INSIDE);

    Rectangle rIn = { panelX + 2, pControlY + 20, 60, 35 };
    if (DrawButtonInteractive(rIn, "P. IN", (Color){ 0, 121, 241, 255 }, canIn)) {
        p->state = PERSON_ENTERING;
    }

    Rectangle rOut = { panelX + 68, pControlY + 20, 60, 35 };
    if (DrawButtonInteractive(rOut, "P. OUT", (Color){ 255, 161, 0, 255 }, canOut)) {
        p->state = PERSON_EXITING;
    }

    // ==========================================
    // PANEL SPEED 
    // ==========================================
    int spdPanelW = 140;
    int spdPanelH = 130;
    int spdPanelX = (sw - spdPanelW) / 2.0f; 
    int spdPanelY = 20; 

    DrawRectangleRounded((Rectangle){spdPanelX, spdPanelY, spdPanelW, spdPanelH}, 0.2f, 10,
                        (Color){ 10, 20, 35, 230 });
    DrawRectangleRoundedLines((Rectangle){spdPanelX, spdPanelY, spdPanelW, spdPanelH}, 0.2f, 10, SKYBLUE);
    DrawText("SPEED CTRL", spdPanelX + 35, spdPanelY + 10, 12, SKYBLUE);
    DrawLine(spdPanelX + 15, spdPanelY + 25, spdPanelX + spdPanelW - 15, spdPanelY + 25, (Color){30, 80, 120, 255});

    DrawText(TextFormat("%.0f px/s", lift->speed * lift->speedMultiplier),
            spdPanelX + 35, spdPanelY + 35, 15, WHITE);

    Color loadColor = GREEN;
    const char* loadStr = "KOSONG";
    if (p->state == PERSON_INSIDE) {
        loadColor = ORANGE;
        loadStr   = "BERPENUMPANG";
        DrawText(loadStr, spdPanelX + 25, spdPanelY + 55, 10, loadColor);
    } else {
        DrawText(loadStr, spdPanelX + 45, spdPanelY + 55, 10, loadColor);
    }

    Rectangle btnSpeedDown = { spdPanelX + 15,  spdPanelY + 75, 45, 25 };
    if (DrawButtonInteractive(btnSpeedDown, "- SPD", (Color){80, 40, 40, 255}, true)) {
        lift->speed -= 25.0f;
        if (lift->speed < 50.0f) lift->speed = 50.0f; 
    }

    Rectangle btnSpeedUp = { spdPanelX + 80, spdPanelY + 75, 45, 25 };
    if (DrawButtonInteractive(btnSpeedUp, "+ SPD", (Color){40, 80, 40, 255}, true)) {
        lift->speed += 25.0f;
        if (lift->speed > 600.0f) lift->speed = 600.0f; 
    }

    Rectangle btnSpeedReset = { spdPanelX + 15, spdPanelY + 105, 110, 20 };
    if (DrawButtonInteractive(btnSpeedReset, "RESET NORMAL", (Color){40, 40, 80, 255}, true)) {
        lift->speed = 250.0f;
    }

    // ==========================================
    // TOMBOL WIREFRAME (Pojok Kanan Bawah)
    // ==========================================
    Rectangle btnWire = { sw - 200, sh - 55, 130, 25 };
    Color wireColor = wireframeMode ? (Color){255, 165, 0, 255} : (Color){30, 60, 90, 255};
    if (DrawButtonInteractive(btnWire, 
        wireframeMode ? "WIREFRAME: ON" : "WIREFRAME: OFF", wireColor, true)) {
        wireframeMode = !wireframeMode;
    }
    if (IsKeyPressed(KEY_F)) wireframeMode = !wireframeMode;

    // ==========================================
    // MENU POV (SUDUT PANDANG) DI KANAN ATAS
    // ==========================================
    int povBtnSize = 40;
    Rectangle btnPov = { sw - povBtnSize - 20, 20, povBtnSize, povBtnSize };
    Vector2 mousePoint = GetMousePosition();
    bool povHover = CheckCollisionPointRec(mousePoint, btnPov);

    bool wf = GetWireframeMode(); 

    // 1. GAMBAR BACKGROUND TOMBOL POV
    if (!wf) {
        Color btnColor = povHover ? (Color){ 220, 230, 240, 255 } : WHITE;
        DrawRectangleRounded(btnPov, 0.3f, 5, btnColor);
        DrawRectangleRoundedLines(btnPov, 0.3f, 5, DARKGRAY);
    } else {
        Color wireBtnColor = povHover ? GREEN : SKYBLUE;
        DrawRectangleRoundedLines(btnPov, 0.3f, 5, wireBtnColor);
    }

    // 2. GAMBAR IKON POV (FULL KODINGAN MURNI!)
    int cx = btnPov.x + 20; 
    int cy = btnPov.y + 20; 
    Color iconColor = povHover ? GREEN : SKYBLUE; 
    Color solidColor = povHover ? DARKGRAY : BLACK; 

    if (!wf) {
        // Mode Normal: Gambar ikon kamera solid pakai shape Raylib
        DrawRectangle(cx - 10, cy - 7, 20, 14, solidColor); // Body kotak
        DrawRectangle(cx - 7, cy - 10, 8, 3, solidColor);   // Tombol jepret
        DrawCircle(cx, cy, 4, WHITE);                       // Lensa putih
        DrawCircle(cx, cy, 2, solidColor);                  // Titik tengah lensa
    } 
    else {
        // Mode Wireframe: Gambar Ikon Kamera pakai garis murni
        DrawRectangleLines(cx - 10, cy - 7, 20, 14, iconColor);
        DrawLine(cx - 7, cy - 7, cx - 7, cy - 10, iconColor);
        DrawLine(cx - 3, cy - 7, cx - 3, cy - 10, iconColor);
        DrawLine(cx - 7, cy - 10, cx - 3, cy - 10, iconColor);
        DrawCircleLines(cx, cy, 4, ORANGE);
    }

    // Logika Buka/Tutup Menu saat tombol diklik
    if (povHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showPovMenu = !showPovMenu;
    }

    // GAMBAR DROPDOWN MENU (HANYA JIKA AKTIF)
    if (showPovMenu) {
        int menuWidth = 140;
        int itemHeight = 35;
        Rectangle menuRect = { sw - menuWidth - 20, 20 + povBtnSize + 5, menuWidth, itemHeight * 4 };

        DrawRectangleRounded(menuRect, 0.1f, 5, (Color){ 30, 40, 55, 250 });
        DrawRectangleRoundedLines(menuRect, 0.1f, 5, SKYBLUE);

        const char* options[4] = { "Default View", "Building View", "Mech View", "Person View" };
        CameraFocusMode modes[4] = { CAM_FOCUS_GLOBAL, CAM_FOCUS_BUILDING, CAM_FOCUS_MECH, CAM_FOCUS_PERSON };

        for (int i = 0; i < 4; i++) {
            Rectangle itemRect = { menuRect.x, menuRect.y + (i * itemHeight), menuWidth, itemHeight };
            bool isHoverItem = CheckCollisionPointRec(mousePoint, itemRect);
            bool isActive = (currentCamMode == modes[i]);

            if (isHoverItem) DrawRectangleRec(itemRect, (Color){ 50, 100, 150, 200 });
            if (isActive) DrawRectangleLinesEx(itemRect, 2, GREEN);

            DrawText(options[i], itemRect.x + 15, itemRect.y + 12, 12, isActive ? GREEN : (isHoverItem ? WHITE : LIGHTGRAY));

            if (isHoverItem && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentCamMode = modes[i];
                showPovMenu = false; 
            }
        }

        if (!CheckCollisionPointRec(mousePoint, menuRect) && !povHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            showPovMenu = false;
        }
    }
}