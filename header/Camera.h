#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"
#include "Person.h"

// Daftar 3 Mode Fokus Kamera
typedef enum {
    CAM_FOCUS_GLOBAL,   // Tampilan awal (Full Screen pas seperti semula)
    CAM_FOCUS_BUILDING, // Zoom ke Gedung (kiri)
    CAM_FOCUS_MECH,     // Zoom ke Mesin (kanan)
    CAM_FOCUS_PERSON    // Zoom ke Subjek Tes
}CameraFocusMode;

// Variabel Global untuk Kamera agar bisa diakses file lain
extern Camera2D simCamera;
extern CameraFocusMode currentCamMode;

void InitSimCamera(void);
void UpdateSimCamera(Person* p);

#endif