#include "../header/Lift.h"
#include <stddef.h>

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

float GetFloorY(int floor) {
    float sh = (float)GetScreenHeight();
    float floorHeight = (sh - 250.0f) / 5.0f; 
    float floorBaseline = sh - 100.0f - ((floor - 1) * floorHeight);
    float carHeight = floorHeight - 10.0f;
    return floorBaseline - carHeight;
}

void TriggerCloseDoor(Elevator* lift) {
    if (lift->state == DOOR_OPEN || lift->state == DOOR_OPENING) {
        lift->state = DOOR_CLOSING;
        lift->timer = 0;
        if (audioLoaded) PlaySound(sndClose);
    }
}

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

// =======================================================
// OTAK ALGORITMA ANTRIAN (SCAN ALGORITHM)
// =======================================================
int CalculateNextTarget(Elevator* lift) {
    // 1. Cek apakah ada tombol biru yang ditekan?
    bool hasRequest = false;
    for (int i = 1; i <= 5; i++) {
        if (lift->floorRequests[i]) hasRequest = true;
    }
    
    // Kalau nggak ada antrian, diam di lantai sekarang
    if (!hasRequest) {
        lift->currentDir = 0;
        return lift->currentFloor;
    }

    // 2. Jika lift habis diam, tentukan arah mau ke atas atau ke bawah
    if (lift->currentDir == 0) {
        for (int i = 1; i <= 5; i++) {
            if (lift->floorRequests[i]) {
                lift->currentDir = (i > lift->currentFloor) ? 1 : -1;
                break;
            }
        }
    }

    // 3. Menyapu antrian ke ATAS
    if (lift->currentDir == 1) {
        for (int i = lift->currentFloor; i <= 5; i++) {
            if (lift->floorRequests[i]) return i;
        }
        lift->currentDir = -1; // Kalau di atas udah bersih, putar balik ke bawah
    }

    // 4. Menyapu antrian ke BAWAH
    if (lift->currentDir == -1) {
        for (int i = lift->currentFloor; i >= 1; i--) {
            if (lift->floorRequests[i]) return i;
        }
        lift->currentDir = 1; // Kalau di bawah udah bersih, putar balik ke atas
        
        // Cek ulang ke atas jaga-jaga ada yang nekan tombol pas lagi putar balik
        for (int i = lift->currentFloor; i <= 5; i++) {
            if (lift->floorRequests[i]) return i;
        }
    }
    
    return lift->currentFloor; 
}


// =======================================================
// LOGIKA UTAMA LIFT
// =======================================================
void UpdateLiftLogic(Elevator* lift, bool personInside) {
    // Inisialisasi awal pas program baru nyala
    if (lift->y == 0.0f) {
        lift->y = GetFloorY(1);
        lift->currentFloor = 1;
        lift->targetFloor = 1;
        lift->speed = 250.0f; 
        lift->speedMultiplier = 1.0f; 
        lift->currentDir = 0;
        for(int i=0; i<6; i++) lift->floorRequests[i] = false;
    }

    float sh = (float)GetScreenHeight();
    float dt = GetFrameTime();   
    
    // Beban kecepatan orang
    float targetMultiplier = personInside ? 0.65f : 1.0f;
    lift->speedMultiplier += (targetMultiplier - lift->speedMultiplier) * dt * 3.0f;
    float speed = lift->speed * lift->speedMultiplier; 

    // =========================================================
    // HITUNG TARGET SEBELUM STATE MACHINE (PENTING BIAR NGGAK MOGOK)
    // =========================================================
    lift->targetFloor = CalculateNextTarget(lift);
    float targetY = GetFloorY(lift->targetFloor); // <--- Sekarang targetY aman!
    
    float maxY = GetFloorY(1); 
    float minY = GetFloorY(5); 
    lift->cw_y = minY + (maxY - lift->y);

    // Update Lantai Dinamis
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
    // STATE MACHINE
    // =========================================================
    switch (lift->state) {
        case IDLE:
            // Jika tombol yg dipencet adalah lantai dia saat ini
            if (lift->floorRequests[lift->currentFloor]) {
                lift->floorRequests[lift->currentFloor] = false; // Matikan birunya
                lift->state = DOOR_OPENING;                      // Langsung buka pintu!
                if (audioLoaded) PlaySound(sndOpen);
            } 
            else if (lift->currentFloor != lift->targetFloor) {
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
                lift->floorRequests[lift->currentFloor] = false; // Matikan tombol biru
                lift->state = DOOR_OPENING;
                if (audioLoaded) { StopSound(sndMove); PlaySound(sndDing); PlaySound(sndOpen); }
            }
            break;

        case MOVING_DOWN:
            lift->y += speed * dt;       
            lift->pulleyAngle -= speed * dt * 0.05f;
            if (lift->y >= targetY) {
                lift->y = targetY;
                lift->currentFloor = lift->targetFloor; 
                lift->floorRequests[lift->currentFloor] = false; // Matikan tombol biru
                lift->state = DOOR_OPENING;
                if (audioLoaded) { StopSound(sndMove); PlaySound(sndDing); PlaySound(sndOpen); }
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