#include "../header/Simulation.h"
#include "../header/Lift.h"
#include "../header/BuildingView.h"
#include "../header/MechView.h"
#include "../header/UI.h"

void DrawSimulationScreen(void) {
    // 1. Gambar Background Utama (Gelap estetik sesuai mockup Anda)
    ClearBackground((Color){ 15, 25, 40, 255 }); 

    // 2. Gambar Sisi Kiri (55%)
    DrawBuildingView(&myLift); 

    // 3. Gambar Sisi Kanan (45%)
    DrawMechView(&myLift);

    // 4. Gambar UI (Panel Tombol, Garis Tengah, dan Status Bar)
    // Dipanggil terakhir agar selalu berada di atas (overlay)
    DrawSimulationUI(&myLift); 
}