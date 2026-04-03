#include "../header/Simulation.h"
#include "../header/Lift.h"
#include "../header/BuildingView.h"
#include "../header/MechView.h"
#include "../header/UI.h"
#include "../header/Person.h"
// (Forward declarations manual yang salah sudah Dihapus dari sini)

void DrawSimulationScreen(Person* p) {
    DrawMainBackground();
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){ 10, 15, 25, 200 });
    UpdateLiftLogic(&myLift);
    UpdatePerson(p, &myLift);
    DrawBuildingView(&myLift, p);
    DrawMechView(&myLift);
    DrawSimulationUI(&myLift, p);
}