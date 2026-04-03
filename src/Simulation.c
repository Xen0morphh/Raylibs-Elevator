#include "../header/Simulation.h"
#include "../header/Lift.h"
#include "../header/BuildingView.h"
#include "../header/MechView.h"
#include "../header/UI.h"

// (Forward declarations manual yang salah sudah Dihapus dari sini)

void DrawSimulationScreen(void) {
    DrawMainBackground();
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){ 10, 15, 25, 200 });
    UpdateLiftLogic(&myLift);
    DrawBuildingView(&myLift); 
    DrawMechView(&myLift);
    DrawSimulationUI(&myLift);
}