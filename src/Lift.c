#include "../header/Lift.h"
#include <stddef.h>
#include "../header/Person.h"

// Inisialisasi posisi awal lift
Elevator myLift = {
    .y = 0.0f, .cw_y = 0.0f, .currentFloor = 1, .targetFloor = 1,
    .state = IDLE, .doorOpenness = 0.0f, .pulleyAngle = 0.0f, .timer = 0.0f
};

// Variable dan Fungsi Audio Lift
static Sound sndMove;
static Sound sndDing;
static Sound sndOpen;
static Sound sndClose;
static bool audioLoaded = false;

// Fungsi untuk Initate Audio
void InitLiftAudio(void) {
    sndMove  = LoadSound("Sound/Elevator_Move.mp3");
    SetSoundVolume(sndMove, 0.8f);
    sndDing  = LoadSound("Sound/Elevator_Ding.mp3");
    SetSoundVolume(sndDing, 0.6f);
    sndOpen  = LoadSound("Sound/Elevator_Open.mp3");
    SetSoundVolume(sndOpen, 0.3f);
    sndClose = LoadSound("Sound/Elevator_Close.mp3");
    SetSoundVolume(sndClose, 0.6f);

    audioLoaded = true;
}

void UnloadLiftAudio(void) {
    if (!audioLoaded) return;
    UnloadSound(sndMove);
    UnloadSound(sndDing);
    UnloadSound(sndOpen);
    UnloadSound(sndClose);
}

// Fungsi menghitung patokan Y yang akurat dan dinamis
float GetFloorY(int floor) {
    float sh = (float)GetScreenHeight();
    
    // SESUAIKAN DENGAN BuildingView.c baris 10:
    float floorHeight = (sh - 250.0f) / 5.0f; 
    
    // SESUAIKAN DENGAN BuildingView.c baris 16:
    float floorBaseline = sh - 100.0f - ((floor - 1) * floorHeight);
    
    // SESUAIKAN DENGAN BuildingView.c baris 21-22:
    float carHeight = floorHeight - 10.0f;
    
    // Karena 'carY' di BuildingView.c itu menggambar dari ATAP lift,
    // maka lift harus berhenti di (Baseline - Tinggi Car) agar kakinya pas di garis.
    return floorBaseline - carHeight;
}

// Fungsi untuk memaksa pintu tertutup (Interupsi Timer)
void TriggerCloseDoor(Elevator* lift) {
    if (lift->state == DOOR_OPEN || lift->state == DOOR_OPENING) {
        lift->state = DOOR_CLOSING;
        lift->timer = 0;
        if (audioLoaded) PlaySound(sndClose); // <--- BUNYIKAN SUARA TUTUP
    }
}

// Fungsi untuk memaksa pintu terbuka (Interupsi Timer)
void TriggerOpenDoor(Elevator* lift) {
    if (lift->state == IDLE || lift->state == DOOR_CLOSING) {
        if (lift->currentFloor == lift->targetFloor) {
            lift->state = DOOR_OPENING;
            if (audioLoaded) PlaySound(sndOpen); 
        }
    } else if (lift->state == DOOR_OPEN) {
        lift->timer = 5.0f; 
    }
}

void UpdateLiftLogic(Elevator* lift, bool personInside) {
    // 1. Inisialisasi posisi awal (hanya sekali)
    if (lift->y == 0.0f) {
        lift->y = GetFloorY(1);
        lift->currentFloor = 1;
        lift->targetFloor = 1;
        lift->speed = 250.0f; // Kecepatan default
        lift->speedMultiplier = 1.0f; // Pengali kecepatan default
    }

    // Hitung beban orang (membuat lift melambat jika ada orang)
    float targetMultiplier = personInside ? 0.65f : 1.0f;
    lift->speedMultiplier += (targetMultiplier - lift->speedMultiplier) * GetFrameTime() * 3.0f;

    // =========================================================
    // DEKLARASI VARIABEL UTAMA (HANYA SEKALI SAJA DI SINI)
    // =========================================================
    float sh = (float)GetScreenHeight();
    float dt = GetFrameTime();   
    float speed = lift->speed * lift->speedMultiplier; 
    float targetY = GetFloorY(lift->targetFloor);
    
    float maxY = GetFloorY(1); 
    float minY = GetFloorY(5); 

    // Update posisi Counterweight
    lift->cw_y = minY + (maxY - lift->y);

    // =========================================================
    // 2. UPDATE LANTAI DINAMIS (Hanya saat lift bergerak)
    // =========================================================
    if (lift->state == MOVING_UP || lift->state == MOVING_DOWN) {
        float floorHeight = (sh - 250.0f) / 5.0f;
        float floorBaseline1 = sh - 100.0f; 
        
        float carHeight = floorHeight - 10.0f;
        float centerY = lift->y + (carHeight / 2.0f);
        
        float relativePos = (floorBaseline1 - centerY) / floorHeight;
        int detectedFloor = (int)(relativePos + 1.0f);
        
        if (detectedFloor >= 1 && detectedFloor <= 5) {
            lift->currentFloor = detectedFloor;
        }
    }

    // =========================================================
    // 3. STATE MACHINE (Logika Pergerakan)
    // =========================================================
    switch (lift->state) {
        case IDLE:
            if (lift->currentFloor != lift->targetFloor) {
                lift->state = (lift->y > targetY) ? MOVING_UP : MOVING_DOWN;
                if (audioLoaded) PlaySound(sndMove);
            }
            break;

        case MOVING_UP:
            lift->y -= speed * dt; 
            lift->pulleyAngle += speed * dt * 0.05f;
            if (lift->y <= targetY) {
                lift->y = targetY; 
                lift->currentFloor = lift->targetFloor; 
                lift->state = DOOR_OPENING;
                if (audioLoaded) {
                    StopSound(sndMove); 
                    PlaySound(sndDing); 
                    PlaySound(sndOpen); 
                }
            }
            break;

        case MOVING_DOWN:
            lift->y += speed * dt;       
            lift->pulleyAngle -= speed * dt * 0.05f;
            if (lift->y >= targetY) {
                lift->y = targetY;
                lift->currentFloor = lift->targetFloor; 
                lift->state = DOOR_OPENING;
                if (audioLoaded) {
                    StopSound(sndMove);
                    PlaySound(sndDing);
                    PlaySound(sndOpen);
                }
            }
            break;

        case DOOR_OPENING:
            lift->doorOpenness += 1.0f * dt;
            if (lift->doorOpenness >= 1.0f) {
                lift->doorOpenness = 1.0f;
                lift->state = DOOR_OPEN;
                lift->timer = 5.0f; 
            }
            break;

        case DOOR_OPEN:
            lift->timer -= dt; 
            if (lift->timer <= 0.0f) {
                lift->state = DOOR_CLOSING;
                if (audioLoaded) PlaySound(sndClose);
            }
            break;

        case DOOR_CLOSING:
            lift->doorOpenness -= 1.0f * dt; 
            if (lift->doorOpenness <= 0.0f) {
                lift->doorOpenness = 0.0f;
                lift->state = IDLE;
            }
            break;
    }
}