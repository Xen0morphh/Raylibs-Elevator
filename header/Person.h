#ifndef PERSON_H
#define PERSON_H

#include "raylib.h"
#include "Lift.h" // Perlu mengetahui state lift

// Enum untuk menentukan arah hadap/jalan
typedef enum {
    FACING_RIGHT,
    FACING_LEFT
} PersonDirection;

// Enum untuk logika perilaku orang
typedef enum {
    PERSON_WAITING,      // Menunggu di lantai
    PERSON_ENTERING,     // Berjalan masuk ke lift
    PERSON_INSIDE,       // Diam di dalam lift
    PERSON_EXITING,      // Berjalan keluar dari lift
    PERSON_GONE          // Sudah keluar dari area simulasi
} PersonState;

typedef struct {
    Vector2 position;         // Posisi (X, Y) di layar
    PersonDirection dir;      // Arah hadap
    PersonState state;        // State perilaku
    int startFloor;           // Lantai awal orang menunggu
    int targetFloor;          // Lantai tujuan orang
    
    // Variabel untuk animasi sprite sheet
    int currentFrame;         // Frame animasi saat ini
    int framesCounter;        // Counter untuk kecepatan animasi
    int framesSpeed;          // Kecepatan animasi (frame per detik)
    float activeTime;         // Opsional: waktu aktif untuk variasi
} Person;

// Deklarasi fungsi global
void InitPersonModule(void);   // Load texture
void UpdatePerson(Person* p, Elevator* lift); // Logika update
void DrawPerson(Person* p);    // Menggambar orang
void UnloadPersonModule(void); // Unload texture


#endif