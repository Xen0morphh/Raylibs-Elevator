#include "../header/BuildingView.h"
#include "../header/Lift.h"
#include "../header/Transformasi.h"
#include "../header/Person.h"
#include "../header/UI.h"

void DrawBuildingView(Elevator* lift, Person* p) {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float leftWidth = sw * 0.55f;
    float floorHeight = (sh - 250) / 5.0f; 
    float shaftWidth = 140; 
    float shaftX = ((leftWidth - shaftWidth) / 2.0f) + 60; 

    // Lorong Lift
    DrawRectCustom(shaftX, 120, shaftWidth, sh - 170, DARKGRAY);

    for (int i = 1; i <= 5; i++) {
        float floorBaseline = sh - 100 - ((i - 1) * floorHeight);
        DrawLineBresenham(200, floorBaseline, leftWidth - 50, floorBaseline, (Color){100, 100, 100, 200});
        DrawText(TextFormat("LT %d", i), 210, floorBaseline - 25, 15, GRAY);
    }

    float carWidth = shaftWidth - 10;
    float carHeight = floorHeight - 10;
    float carX = shaftX + 5;
    float carY = lift->y;

    // ==========================================
    // KOTAK INDIKATOR LANTAI (Kecil di Atas Pintu)
    // ==========================================
    int indW = 30;
    int indH = 20;
    float indX = carX + (carWidth / 2.0f) - (indW / 2.0f);
    float indY = carY - indH - 5; // Posisinya tepat 5 pixel di atas atap lift

    DrawRectangle(indX, indY, indW, indH, (Color){10, 20, 35, 255});
    DrawRectCustom(indX, indY, indW, indH, SKYBLUE);
    
    // Teks di-tengah-tengah kotak indikator
    DrawText(TextFormat("%d", lift->currentFloor), indX + 10, indY + 3, 15, GREEN);

    // ==========================================
    // GAMBAR MOBIL LIFT & PINTU
    // ==========================================
    DrawRectangle(carX, carY, carWidth, carHeight, (Color){40, 40, 40, 255}); 
    
    // Gambar orang di dalam lift hanya jika dia benar-benar berada di area lift
    if (p->state != PERSON_GONE && p->position.x < leftWidth) {
        DrawPerson(p);
    }   
    
    // ==========================================
    // KOTAK LIFT — dengan wireframe mode
    // ==========================================
    bool wf = GetWireframeMode(); 

    if (!wf) {
        // Mode Normal: isi penuh
        DrawRectangle(carX, carY, carWidth, carHeight, (Color){40, 40, 40, 255});
    }
    // Outline selalu digambar (baik normal maupun wireframe)
    DrawRectCustom(carX, carY, carWidth, carHeight, BLUE);

    // ==========================================
    // INDIKATOR BEBAN DI DALAM CAR
    // (Tampil saat PERSON_INSIDE, di pojok kanan atas car)
    // ==========================================
    if (p->state == PERSON_INSIDE) {
        // Latar kecil
        int iX = carX + carWidth - 38;
        int iY = carY + 6;
        DrawRectangleRounded((Rectangle){iX, iY, 32, 44}, 0.3f, 5,
                             (Color){10, 10, 10, 200});
        DrawRectangleRoundedLines((Rectangle){iX, iY, 32, 44}, 0.3f, 5, ORANGE);

        // Label
        DrawText("kg", iX + 8, iY + 3, 8, ORANGE);

        // Bar beban (persen terhadap kapasitas 320kg, asumsi 1 orang = 70kg)
        float loadPct = 70.0f / 320.0f; // ~22%
        int barH    = 28;
        int fillH   = (int)(barH * loadPct);
        int barX    = iX + 10;
        int barY    = iY + 14;

        DrawRectangle(barX, barY, 12, barH, (Color){30, 30, 30, 255});         // Background bar
        DrawRectangle(barX, barY + barH - fillH, 12, fillH, ORANGE);            // Isi bar
        DrawRectangleLines(barX, barY, 12, barH, GRAY);                          // Border

        // Angka persen kecil
        DrawText(TextFormat("%d%%", (int)(loadPct * 100)),
                 iX + 2, iY + 34, 8, ORANGE);
    }

    // Gambar orang
    if (p->state != PERSON_GONE && p->position.x < leftWidth) {
        DrawPerson(p);
    }

    // ==========================================
    // PINTU — dengan wireframe mode
    // ==========================================
    float doorWidth = (carWidth / 2.0f) * (1.0f - lift->doorOpenness);
    if (doorWidth > 0.0f) {
        if (!wf) {
            DrawRectangle(carX, carY, doorWidth, carHeight,
                          (Color){20, 60, 100, 255});
            DrawRectangle(carX + carWidth - doorWidth, carY, doorWidth, carHeight,
                          (Color){20, 60, 100, 255});
        }
        DrawRectCustom(carX, carY, doorWidth, carHeight, SKYBLUE);
        DrawRectCustom(carX + carWidth - doorWidth, carY, doorWidth, carHeight, SKYBLUE);
    }

    // ==========================================
    // LORONG DAN LANTAI — wireframe mode
    // ==========================================
    if (!wf) {
        DrawRectCustom(shaftX, 120, shaftWidth, sh - 170, DARKGRAY);
    } else {
        // Hanya outline lorong
        DrawRectCustom(shaftX, 120, shaftWidth, sh - 170, (Color){80, 80, 80, 180});
    }
}