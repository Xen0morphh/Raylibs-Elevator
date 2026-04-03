#include "../header/Lift.h"

Elevator myLift = {
    .y = 0.0f, .cw_y = 0.0f, .currentFloor = 1, .targetFloor = 1,
    .state = IDLE, .doorOpenness = 0.0f, .pulleyAngle = 0.0f, .timer = 0.0f
};

float GetFloorY(int floor) {
    float sh = GetScreenHeight();
    // KUNCI PERBAIKAN: (sh - 250) akan menyisakan ruang lega 150px di atap gedung untuk Motor!
    float floorHeight = (sh - 250) / 5.0f; 
    float floorBaseline = sh - 100 - ((floor - 1) * floorHeight);
    float carHeight = floorHeight - 10;
    return floorBaseline - carHeight; 
}

void UpdateLiftLogic(Elevator* lift) {
    if (lift->y == 0.0f) lift->y = GetFloorY(1);

    float topY = GetFloorY(5);
    float bottomY = GetFloorY(1);
    lift->cw_y = topY + bottomY - lift->y; 

    float dt = GetFrameTime();   
    float speed = 250.0f; 
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
            lift->doorOpenness += 1.0f * dt; 
            if (lift->doorOpenness >= 1.0f) {
                lift->doorOpenness = 1.0f;
                lift->state = DOOR_OPEN;
                lift->timer = 3.0f; 
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