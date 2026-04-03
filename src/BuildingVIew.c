#include "../header/BuildingView.h"
#include "../header/Lift.h"
#include "../header/Transformasi.h"
#include "../header/Person.h"

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
    
    float doorWidth = (carWidth / 2.0f) * (1.0f - lift->doorOpenness);
    if (doorWidth > 0.0f) {
        DrawRectangle(carX, carY, doorWidth, carHeight, (Color){20, 60, 100, 255});
        DrawRectCustom(carX, carY, doorWidth, carHeight, SKYBLUE); 
        DrawRectangle(carX + carWidth - doorWidth, carY, doorWidth, carHeight, (Color){20, 60, 100, 255});
        DrawRectCustom(carX + carWidth - doorWidth, carY, doorWidth, carHeight, SKYBLUE); 
    }

    DrawRectCustom(carX, carY, carWidth, carHeight, BLUE);
}