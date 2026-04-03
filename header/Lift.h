#ifndef LIFT_H
#define LIFT_H

#include "raylib.h"

// Status lift saat ini
typedef enum {
    IDLE,
    MOVING_UP,
    MOVING_DOWN,
    DOOR_OPENING,
    DOOR_CLOSING
} LiftState;

// Struct utama penyimpan data
typedef struct {
    float y;             // Posisi vertikal Elevator Car
    float cw_y;          // Posisi vertikal Counterweight (bergerak terbalik dari y)
    int currentFloor;
    int targetFloor;
    LiftState state;
    float doorOpenness;  // 0.0f (tertutup) sampai 1.0f (terbuka penuh)
} Elevator;

// Deklarasi variabel global agar bisa dibaca file lain (nanti diinisialisasi di Lift.c)
extern Elevator myLift; 

#endif