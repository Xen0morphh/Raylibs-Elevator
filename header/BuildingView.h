#ifndef BUILDING_VIEW_H
#define BUILDING_VIEW_H
#include "raylib.h"
#include "Lift.h" // Agar compiler tahu apa itu tipe data 'Elevator'
#include "Person.h" // Agar compiler tahu apa itu tipe data 'Person'

void DrawBuildingView(Elevator* lift, Person* p);

#endif