#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include "../Screen_type.h" 

void InitMenuScreen(void);
ScreenState UpdateMenuScreen(void);
void DrawMenuScreen(void);
void UnloadMenuScreen(void);

#endif