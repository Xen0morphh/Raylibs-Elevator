#ifndef LIFT_H
#define LIFT_H

#include "raylib.h"

typedef enum {
    IDLE,
    MOVING_UP,
    MOVING_DOWN,
    DOOR_OPENING,
    DOOR_OPEN,
    DOOR_CLOSING
} LiftState;

typedef struct {
    float y;
    float cw_y;
    int currentFloor;
    int targetFloor;
    LiftState state;
    float doorOpenness;  // 0.0f (tertutup) sampai 1.0f (terbuka penuh)
    float pulleyAngle;   // Menyimpan sudut putaran katrol
    float timer;         // Menyimpan waktu tunggu 3 detik
    float speed;         // Kecepatan naik/turun lift (pixel per detik)
    float speedMultiplier; // Pengali kecepatan untuk variasi (mis. saat orang masuk/keluar)
} Elevator;

extern Elevator myLift;

// Fungsi baru untuk mengupdate logika mesin lift setiap frame
void UpdateLiftLogic(Elevator* lift, bool personInside);
void InitLiftAudio(void);
void UnloadLiftAudio(void);
void TriggerOpenDoor(Elevator* lift);
void TriggerCloseDoor(Elevator* lift);


#endif