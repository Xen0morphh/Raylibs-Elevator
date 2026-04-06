#ifndef UI_H
#define UI_H
#include "Lift.h"
#include "Person.h"
#include "UI.h"

// Fungsi untuk mengecek apakah mode wireframe aktif (untuk digunakan di berbagai file)
bool GetWireframeMode(void);

// Fungsi untuk load gambar global
void InitUI(void);

// Fungsi untuk menggambar background utama (fullscreen)
void DrawMainBackground(void);

// Fungsi untuk membuang gambar dari RAM
void UnloadUI(void);

// Fungsi untuk menggambar elemen UI di layar simulasi (status bar, garis tengah, dll)
void DrawSimulationUI(Elevator* lift, Person* p);

bool DrawButtonInteractive(Rectangle rect, const char* text, Color baseColor, bool enabled);

#endif