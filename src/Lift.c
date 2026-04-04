#include "../header/Lift.h"

// Inisialisasi posisi awal lift
Elevator myLift = {
    .y = 0.0f, .cw_y = 0.0f, .currentFloor = 1, .targetFloor = 1,
    .state = IDLE, .doorOpenness = 0.0f, .pulleyAngle = 0.0f, .timer = 0.0f
};

// Fungsi menghitung patokan Y yang akurat dan dinamis
float GetFloorY(int floor) {
    float sh = GetScreenHeight();
    float floorHeight = (sh - 150) / 5.0f;
    float floorBaseline = sh - 100 - ((floor - 1) * floorHeight);
    float carHeight = floorHeight - 10;
    return floorBaseline - carHeight; // Mengembalikan posisi Y (Atap Lift)
}

void UpdateLiftLogic(Elevator* lift) {
    // 1. Inisialisasi otomatis di frame pertama (karena GetScreenHeight baru aktif)
    if (lift->y == 0.0f) lift->y = GetFloorY(1);

    // 2. Kunci posisi Counterweight (Otomatis terbalik dengan Lift!)
    float topY = GetFloorY(5);
    float bottomY = GetFloorY(1);
    lift->cw_y = topY + bottomY - lift->y; 

    float dt = GetFrameTime();   
    float speed = 250.0f; // Kecepatan lift dipercepat sedikit
    float targetY = GetFloorY(lift->targetFloor);

    switch (lift->state) {
        case IDLE:
            if (lift->currentFloor != lift->targetFloor) {
                lift->state = (lift->y > targetY) ? MOVING_UP : MOVING_DOWN;
            }
            break;

        case MOVING_UP:
            lift->y -= speed * dt;       
            lift->pulleyAngle += speed * dt * 0.05f;
            if (lift->y <= targetY) {
                lift->y = targetY; 
                lift->currentFloor = lift->targetFloor;
                lift->state = DOOR_OPENING;
            }
            break;

        case MOVING_DOWN:
            lift->y += speed * dt;       
            lift->pulleyAngle -= speed * dt * 0.05f;
            if (lift->y >= targetY) {
                lift->y = targetY;
                lift->currentFloor = lift->targetFloor;
                lift->state = DOOR_OPENING;
            }
            break;

        case DOOR_OPENING:
            lift->doorOpenness += 1.0f * dt; // Buka pintu 1 detik
            if (lift->doorOpenness >= 1.0f) {
                lift->doorOpenness = 1.0f;
                lift->state = DOOR_OPEN;
                lift->timer = 3.0f; // Tahan 3 detik
            }
            break;

        case DOOR_OPEN:
            lift->timer -= dt; 
            if (lift->timer <= 0.0f) lift->state = DOOR_CLOSING;
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