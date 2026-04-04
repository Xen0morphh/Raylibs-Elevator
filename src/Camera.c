#include "../header/Camera.h"

Camera2D simCamera = { 0 };
CameraFocusMode currentCamMode = CAM_FOCUS_GLOBAL;

void InitSimCamera(void) {
    // Kita kosongkan pengaturan posisi di sini untuk mencegah bug layar awal (Zero Offset Bug).
    // Hanya atur zoom dan rotasi dasar.
    simCamera.rotation = 0.0f;
    simCamera.zoom = 1.0f;
}

void UpdateSimCamera(Person* p) {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();

    // UPDATE LENSA SECARA REAL-TIME
    simCamera.offset = (Vector2){ sw / 2.0f, sh / 2.0f };

    // CEK INPUT KEYBOARD (GANTI MODE)
    if (IsKeyPressed(KEY_ONE)) currentCamMode = CAM_FOCUS_GLOBAL;
    if (IsKeyPressed(KEY_TWO)) currentCamMode = CAM_FOCUS_BUILDING;
    if (IsKeyPressed(KEY_THREE)) currentCamMode = CAM_FOCUS_MECH;
    if (IsKeyPressed(KEY_FOUR)) currentCamMode = CAM_FOCUS_PERSON;

    // TENTUKAN TARGET KOORDINAT BERDASARKAN MODE
    Vector2 targetPos = simCamera.target;
    float targetZoom = 1.0f;

    switch (currentCamMode) {
        case CAM_FOCUS_GLOBAL:
            // Mode Tampilan Penuh (Normal seperti sebelum pakai kamera)
            targetPos = (Vector2){ sw / 2.0f, sh / 2.0f };
            targetZoom = 1.0f;
            break;

        case CAM_FOCUS_BUILDING:
            targetPos = (Vector2){ sw * 0.275f, sh / 2.0f }; 
            targetZoom = 1.1f; 
            break;

        case CAM_FOCUS_MECH:
            targetPos = (Vector2){ sw * 0.775f, sh / 2.0f }; 
            targetZoom = 1.1f;
            break;

        case CAM_FOCUS_PERSON:
            if (p->state != PERSON_GONE) {
                targetPos = (Vector2){ p->position.x + 15, p->position.y - 20 };
                targetZoom = 2.5f; 
            } else {
                currentCamMode = CAM_FOCUS_GLOBAL; 
            }
            break;
    }


    // TRANSISI MULUS (DENGAN PENCEGAH LONCATAN AWAL)

    // Jika aplikasi baru dibuka, langsung pasang target tanpa animasi agar tidak geser-geser
    if (simCamera.target.x == 0 && simCamera.target.y == 0) {
        simCamera.target = targetPos;
    } else {
        float speed = 5.0f * GetFrameTime(); 
        simCamera.target.x += (targetPos.x - simCamera.target.x) * speed;
        simCamera.target.y += (targetPos.y - simCamera.target.y) * speed;
        simCamera.zoom += (targetZoom - simCamera.zoom) * speed;
    }
}