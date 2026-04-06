#include "../header/MechView.h"
#include "../header/Lift.h"
#include "../header/Transformasi.h"
#include <math.h>

void DrawRotatedCross(int cx, int cy, int radius, float angle, Color color) {
    int x1 = cx + cosf(angle) * (radius - 2); int y1 = cy + sinf(angle) * (radius - 2);
    int x2 = cx - cosf(angle) * (radius - 2); int y2 = cy - sinf(angle) * (radius - 2);
    DrawLineBresenham(x1, y1, x2, y2, color);

    int x3 = cx + cosf(angle + PI/2) * (radius - 2); int y3 = cy + sinf(angle + PI/2) * (radius - 2);
    int x4 = cx - cosf(angle + PI/2) * (radius - 2); int y4 = cy - sinf(angle + PI/2) * (radius - 2);
    DrawLineBresenham(x3, y3, x4, y4, color);
}

void DrawMechView(Elevator* lift) {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float startX = sw * 0.55f; 
    float shaftCenter = startX + ((sw * 0.45f) / 2.0f) - 40; 

    // Lorong Mekanik
    DrawRectCustom(shaftCenter - 75, 120, 150, sh - 170, (Color){40, 50, 60, 255});
    DrawRectCustom(shaftCenter - 65, 120, 130, sh - 170, DARKGRAY); 

    // Bounding Box Rotor
    DrawRectCustom(shaftCenter - 60, 10, 120, 105, (Color){255, 100, 100, 150}); 
    DrawText("ROTOR BOUNDARY", shaftCenter - 50, 15, 10, (Color){255, 100, 100, 150});

    // Motor
    for (int i=0; i<30; i++) DrawLineBresenham(shaftCenter - 50, 25 + i, shaftCenter + 50, 25 + i, (Color){60, 30, 90, 255});
    DrawRectCustom(shaftCenter - 50, 25, 100, 30, VIOLET);
    DrawText("MOTOR", shaftCenter - 25, 32, 15, LIGHTGRAY);

    // Buffer Bawah
    float bufY = sh - 70;
    for (int i=0; i<20; i++) {
        DrawLineBresenham(shaftCenter - 40, bufY + i, shaftCenter - 10, bufY + i, (Color){80, 30, 30, 255});
        DrawLineBresenham(shaftCenter + 10, bufY + i, shaftCenter + 40, bufY + i, (Color){80, 30, 30, 255});
    }
    DrawRectCustom(shaftCenter - 40, bufY, 30, 20, MAROON);
    DrawRectCustom(shaftCenter + 10, bufY, 30, 20, MAROON);
    DrawText("BUFFER", shaftCenter - 25, bufY + 30, 15, RED);

    // ==========================================
    // KATROL & KABEL
    // ==========================================
    int pulleyY = 85; 
    int rLeft = 20;
    int rRight = 15;
    int cxLeft = shaftCenter - 25;
    int cxRight = shaftCenter + 25;

    // Gambar Katrol
    DrawCircleMidpoint(cxLeft, pulleyY, rLeft, PURPLE); 
    DrawCircleMidpoint(cxRight, pulleyY, rRight, PURPLE); 
    DrawRotatedCross(cxLeft, pulleyY, rLeft, lift->pulleyAngle, VIOLET);
    DrawRotatedCross(cxRight, pulleyY, rRight, -lift->pulleyAngle, VIOLET);

    // Kabel Kiri (Turun dari sisi luar kiri katrol)
    DrawLineBresenham(cxLeft - rLeft, pulleyY, cxLeft - rLeft, lift->y, LIGHTGRAY);
    
    // Kabel Kanan (Turun dari sisi luar kanan katrol)
    DrawLineBresenham(cxRight + rRight, pulleyY, cxRight + rRight, lift->cw_y, LIGHTGRAY);
    
    // Kabel Atas (Menghubungkan pucuk kedua katrol)
    DrawLineBresenham(cxLeft, pulleyY - rLeft, cxRight, pulleyY - rRight, LIGHTGRAY);

    // Counterweight
    int cwX = shaftCenter + 5;
    for (int i = 0; i < 90; i++) DrawLineBresenham(cwX, lift->cw_y + i, cwX + 45, lift->cw_y + i, (Color){ 40, 100, 50, 255 });
    DrawRectCustom(cwX, lift->cw_y, 45, 90, GREEN);
    DrawText("CW", cwX + 12, lift->cw_y + 15, 15, LIME);
    DrawText("-1500", cwX + 4, lift->cw_y + 40, 15, LIME);
    DrawText("kg", cwX + 15, lift->cw_y + 60, 15, LIME);

    // CAR MEKANIK
    int mCarWidth = 65;
    float mCarHeight = (sh - 250) / 5.0f - 10;
    float mCarX = shaftCenter - 70;

    DrawRectCustom(mCarX - 5, lift->y - 5, mCarWidth + 10, mCarHeight + 10, (Color){100, 200, 255, 100}); 
    DrawRectCustom(mCarX, lift->y, mCarWidth, mCarHeight, SKYBLUE);
    DrawLineBresenham(mCarX, lift->y, mCarX + mCarWidth, lift->y + mCarHeight, (Color){50, 100, 150, 150});
    DrawLineBresenham(mCarX + mCarWidth, lift->y, mCarX, lift->y + mCarHeight, (Color){50, 100, 150, 150});

    DrawRectangle(mCarX + 5, lift->y + 10, 30, 20, (Color){10, 20, 35, 255});
    DrawRectCustom(mCarX + 5, lift->y + 10, 30, 20, SKYBLUE);
    DrawText(TextFormat("%d", lift->currentFloor), mCarX + 15, lift->y + 12, 15, GREEN);

    // Labels Anotasi Kanan
    int labelX = shaftCenter + 100;
    DrawLineBresenham(shaftCenter + 50, 35, labelX - 10, 35, (Color){100,100,150,150});
    DrawText("Motor", labelX, 27, 15, VIOLET);
    
    DrawLineBresenham(shaftCenter + 40, pulleyY, labelX - 10, pulleyY, (Color){100,100,150,150});
    DrawText("Pulley", labelX, pulleyY - 8, 15, VIOLET);

    DrawLineBresenham(cxRight, pulleyY - rRight, labelX - 10, pulleyY - 20, (Color){100,100,150,150});
    DrawText("Tali", labelX, pulleyY - 28, 15, LIGHTGRAY);

    DrawLineBresenham(cwX + 45, lift->cw_y + 45, labelX - 10, lift->cw_y + 45, (Color){50,150,50,150});
    DrawText("CounterWeight", labelX, lift->cw_y + 37, 15, GREEN);

    DrawLineBresenham(mCarX + mCarWidth, lift->y + (mCarHeight/2), labelX - 10, lift->y + (mCarHeight/2), (Color){50,100,200,150});
    DrawText("Car", labelX, lift->y + (mCarHeight/2) - 8, 15, SKYBLUE);
}