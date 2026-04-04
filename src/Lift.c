#include "../header/Lift.h"

// Inisialisasi posisi awal lift
Elevator myLift = {
    .y = 0.0f, .cw_y = 0.0f, .currentFloor = 1, .targetFloor = 1,
    .state = IDLE, .doorOpenness = 0.0f, .pulleyAngle = 0.0f, .timer = 0.0f
};

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

void UpdateLiftLogic(Elevator* lift) {
    // 1. Inisialisasi otomatis di frame pertama (karena GetScreenHeight baru aktif)
    if (lift->y == 0.0f) lift->y = GetFloorY(1);

    // 2. Kunci posisi Counterweight (Otomatis terbalik dengan Lift!)
    float maxY = GetFloorY(1); // Posisi Lantai 1
    float minY = GetFloorY(5); // Posisi Lantai 5
    lift->cw_y = minY + (maxY - lift->y);
    
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
                lift->y = targetY; // Paksa berhenti tepat di garis target
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