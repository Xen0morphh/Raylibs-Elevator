#include "../header/Person.h"
#include "../header/Lift.h"
#include <math.h>

static Texture2D texWalkRight;
static Texture2D texWalkLeft;

// KONFIGURASI SPRITE
#define NUM_FRAMES_PER_SHEET  4  
#define SPRITE_SCALE          2.0f 

// KOORDINAT AREA (Sesuaikan dengan shaftX di BuildingView.c Anda)
// Jika shaftX Anda adalah ((leftWidth - 140) / 2.0f) + 60
#define WAITING_X_ZONE       300.0f // Area berdiri di depan lift
#define INSIDE_LIFT_X        470.0f // Titik tengah persis di dalam lift

void InitPersonModule(void) {
    texWalkRight = LoadTexture("assets/walk_right-Sheet.png");
    texWalkLeft = LoadTexture("assets/walk_left-Sheet.png");
}

float GetPersonFloorY(int floor) {
    float sh = GetScreenHeight();
    float floorHeight = (sh - 250) / 5.0f; 
    float floorBaseline = sh - 100 - ((floor - 1) * floorHeight);
    float frameHeight = (float)texWalkRight.height * SPRITE_SCALE;
    return floorBaseline - frameHeight; 
}

void UpdatePerson(Person *p, Elevator *lift) {
    float dt = GetFrameTime();
    float walkSpeed = 150.0f; 

    // --- LOGIKA TOMBOL INPUT ---
    if (IsKeyPressed(KEY_I) && p->state == PERSON_WAITING) {
        // Tekan 'I' untuk perintahkan orang masuk (Hanya jika sedang menunggu)
        if (lift->currentFloor == p->startFloor && lift->doorOpenness >= 0.9f) {
             p->state = PERSON_ENTERING;
        }
    }
    if (IsKeyPressed(KEY_O) && p->state == PERSON_INSIDE) {
        // Tekan 'O' untuk keluar (HANYA BISA JIKA PINTU TERBUKA)
        if (lift->doorOpenness >= 0.9f) {
            p->state = PERSON_EXITING;
        }
    }

    // --- STATE MACHINE ---
    switch (p->state) {
        case PERSON_WAITING:
            // Kondisi: Orang diam di lantai menunggu lift datang
            p->position.x = WAITING_X_ZONE;
            p->position.y = GetPersonFloorY(p->startFloor);
            p->dir = FACING_RIGHT; // Menghadap ke arah lift
            break;

        case PERSON_ENTERING:
            // Kondisi: Orang sedang berjalan masuk ke dalam lift
            p->dir = FACING_RIGHT;
            p->position.x += walkSpeed * dt;
            
            // Jika sudah sampai di tengah lift, berhenti dan ganti status
            if (p->position.x >= INSIDE_LIFT_X) {
                p->position.x = INSIDE_LIFT_X;
                p->state = PERSON_INSIDE;
            }
            break;

        case PERSON_INSIDE:
            // Kondisi: Orang diam di dalam lift, posisi Y ikut naik/turun dengan lift
            p->position.x = INSIDE_LIFT_X;
            float sh = GetScreenHeight();
            float floorHeight = (sh - 250) / 5.0f;
            float carHeight = floorHeight - 10;
            float pHeight = (float)texWalkRight.height * SPRITE_SCALE;
            
            // Kunci kaki orang di lantai lift
            p->position.y = lift->y + carHeight - pHeight;
            break;

        case PERSON_EXITING:
            // Kondisi: Orang berjalan keluar dari lift
            p->dir = FACING_LEFT;
            p->position.x -= walkSpeed * dt;
            
            // Jika sudah sampai di luar (area tunggu), berhenti
            if (p->position.x <= WAITING_X_ZONE) {
                p->position.x = WAITING_X_ZONE;
                p->state = PERSON_WAITING; 
                p->startFloor = lift->currentFloor; // Dia sekarang nunggu di lantai baru
            }
            break;

        case PERSON_GONE:
            // Kondisi: Orang menghilang/keluar dari simulasi
            p->position.x = -500.0f; // Dibuang jauh-jauh dari layar
            break;

        default:
            break;
    }
}

void DrawPerson(Person *p) {
    Texture2D currentTex = (p->dir == FACING_RIGHT) ? texWalkRight : texWalkLeft;
    float fWidth = (float)currentTex.width / NUM_FRAMES_PER_SHEET;
    float fHeight = (float)currentTex.height;

    Rectangle src = { p->currentFrame * fWidth, 0, fWidth, fHeight };
    Rectangle dst = { p->position.x, p->position.y, fWidth * SPRITE_SCALE, fHeight * SPRITE_SCALE };
    
    // Trik Agar "Ikut Tertutup Pintu":
    // Kita hanya gambar orang jika dia di luar lift, 
    // ATAU jika di dalam lift tapi status pintunya terbuka (doorOpenness > 0)
    // Tapi karena kita ingin dia seolah di DALAM, kita harus menggambarnya
    // DI ANTARA gambar Interior Lift dan gambar Pintu di BuildingView.c
    
    DrawTexturePro(currentTex, src, dst, (Vector2){0,0}, 0.0f, WHITE);
}

void UnloadPersonModule(void) {
    UnloadTexture(texWalkRight);
    UnloadTexture(texWalkLeft);
}