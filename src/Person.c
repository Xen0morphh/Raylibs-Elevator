#include "../header/Person.h"
#include "../header/Lift.h"
#include <math.h>

static Texture2D texWalkRight;
static Texture2D texWalkLeft;

#define NUM_FRAMES_PER_SHEET  4  
#define SPRITE_SCALE          2.0f 

#define WAITING_X_ZONE       300.0f
#define INSIDE_LIFT_X        550.0f

void InitPersonModule(void) {
    texWalkRight = LoadTexture("assets/walk_right-Sheet.png");
    texWalkLeft  = LoadTexture("assets/walk_left-Sheet.png");
}

float GetPersonFloorY(int floor) {
    float sh          = (float)GetScreenHeight();
    float floorHeight = (sh - 250.0f) / 5.0f;
    float floorBase   = sh - 100.0f - ((floor - 1) * floorHeight);
    float frameHeight = (float)texWalkRight.height * SPRITE_SCALE;
    return floorBase - frameHeight;
}

// ============================================================
// HELPER: Update frame animasi sprite
// Dipanggil setiap frame saat orang bergerak atau idle
// framesSpeed = berapa frame sprite per detik (mis. 8 = 8fps)
// ============================================================
static void UpdatePersonAnimation(Person *p) {
    p->framesCounter++;
    // Maju ke frame berikutnya setiap (60 / framesSpeed) game-frame
    int threshold = (p->framesSpeed > 0) ? (60 / p->framesSpeed) : 8;
    if (p->framesCounter >= threshold) {
        p->framesCounter = 0;
        p->currentFrame++;
        if (p->currentFrame >= NUM_FRAMES_PER_SHEET)
            p->currentFrame = 0;
    }
}

void UpdatePerson(Person *p, Elevator *lift) {
    float dt        = GetFrameTime();
    float walkSpeed = 150.0f;

    // --- INPUT ---
    if (IsKeyPressed(KEY_I) && p->state == PERSON_WAITING) {
        if (lift->currentFloor == p->startFloor && lift->doorOpenness >= 0.9f)
            p->state = PERSON_ENTERING;
    }
    if (IsKeyPressed(KEY_O) && p->state == PERSON_INSIDE) {
        if (lift->doorOpenness >= 0.9f)
            p->state = PERSON_EXITING;
    }

    // --- STATE MACHINE ---
    switch (p->state) {

        case PERSON_WAITING:
            p->position.x    = WAITING_X_ZONE;
            p->position.y    = GetPersonFloorY(p->startFloor);
            p->dir           = FACING_RIGHT;
            // Diam total — beku di frame 0
            p->currentFrame  = 0;
            p->framesCounter = 0;
            break;

        case PERSON_ENTERING:
            p->dir         = FACING_RIGHT;
            p->position.x += walkSpeed * dt;
            UpdatePersonAnimation(p);   // <-- INI YANG HILANG SEBELUMNYA
            if (p->position.x >= INSIDE_LIFT_X) {
                p->position.x = INSIDE_LIFT_X;
                p->state      = PERSON_INSIDE;
            }
            break;

        case PERSON_INSIDE: {   
            p->position.x     = INSIDE_LIFT_X;
            float sh          = (float)GetScreenHeight();
            float floorHeight = (sh - 250.0f) / 5.0f;
            float carHeight   = floorHeight - 10.0f;
            float pHeight     = (float)texWalkRight.height * SPRITE_SCALE;
            p->position.y     = lift->y + carHeight - pHeight;
            p->currentFrame  = 0;
            p->framesCounter = 0;
            break;
        }

        case PERSON_EXITING:
            p->dir         = FACING_LEFT;
            p->position.x -= walkSpeed * dt;
            UpdatePersonAnimation(p); 
            if (p->position.x <= WAITING_X_ZONE) {
                p->position.x = WAITING_X_ZONE;
                p->state      = PERSON_WAITING;
                p->startFloor = lift->currentFloor;
            }
            break;

        case PERSON_GONE:
            p->position.x = -500.0f;
            break;

        default:
            break;
    }
}

void DrawPerson(Person *p) {
    Texture2D currentTex = (p->dir == FACING_RIGHT) ? texWalkRight : texWalkLeft;

    // Guard: jika texture belum load, skip
    if (currentTex.id == 0) return;

    float fWidth  = (float)currentTex.width / NUM_FRAMES_PER_SHEET;
    float fHeight = (float)currentTex.height;

    Rectangle src = { p->currentFrame * fWidth, 0, fWidth, fHeight };
    Rectangle dst = { p->position.x, p->position.y,
                      fWidth  * SPRITE_SCALE,
                      fHeight * SPRITE_SCALE };

    DrawTexturePro(currentTex, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
}

void UnloadPersonModule(void) {
    UnloadTexture(texWalkRight);
    UnloadTexture(texWalkLeft);
}