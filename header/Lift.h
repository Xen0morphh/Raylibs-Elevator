#ifndef LIFT_H
#define LIFT_H

typedef enum {
    IDLE,    // diam, menunggu request
    MOVING,  // sedang naik/turun
    DOOR     // pintu membuka/menutup
} ElevatorState;

#endif