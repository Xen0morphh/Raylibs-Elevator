#include "../header/Simulation.h"
#include "../header/Lift.h"
#include "../header/BuildingView.h"
#include "../header/MechView.h"
#include "../header/UI.h"
#include "../header/Person.h"
#include "../header/Camera.h"



void DrawSimulationScreen(Person* p) {
    // 1. BACKGROUND & OVERLAY GELAP (Di luar kamera supaya tetap menutupi seluruh layar)
    DrawMainBackground();
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){ 10, 15, 25, 200 });

    // 2. UPDATE LOGIKA & POSISI (Wajib di-update sebelum digambar)
    UpdateLiftLogic(&myLift, p->state == PERSON_INSIDE); // Update logika lift dengan informasi orang
    UpdatePerson(p, &myLift);
    UpdateSimCamera(p); // Update posisi dan zoom kamera

    // 3. PASANG LENSA KAMERA (Dunia Fisik)
    // Semua yang digambar di bawah ini akan ter-zoom dan bisa digeser!
    BeginMode2D(simCamera); 
        DrawBuildingView(&myLift, p);
        float splitX = GetScreenWidth() * 0.55f;
        DrawLineEx((Vector2){ splitX, -2000 }, (Vector2){ splitX, 2000 }, 2.0f, (Color){ 50, 100, 150, 255 });
        DrawMechView(&myLift);        // Gambar Mesin
    
    EndMode2D(); // <--- LEPASKAN LENSA KAMERA

    // 4. GAMBAR UI (Di luar kamera)
    // Digambar paling bawah supaya posisinya menempel di layar komputer
    DrawSimulationUI(&myLift, p); 
}