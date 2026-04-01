#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef struct {
    Rectangle bounds;
    const char *text;
    bool isHovered;
    bool isPressed;
} Button;

typedef struct {
    Button *buttons;
    int buttonCount;
    int selectedIndex;
    bool isActive;
} Menu;

// Function declarations
Menu* MenuCreate(int buttonCount);
void MenuDestroy(Menu *menu);
void MenuUpdate(Menu *menu);
void MenuDraw(Menu *menu);
void MenuAddButton(Menu *menu, int index, Rectangle bounds, const char *text);
int MenuGetSelectedButton(Menu *menu);
void MenuSetActive(Menu *menu, bool active);

#endif // MENU_H