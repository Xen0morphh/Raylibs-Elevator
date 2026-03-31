#ifndef SCREEN_TYPE_H
#define SCREEN_TYPE_H

// Mendefinisikan semua kemungkinan layar dalam aplikasi
typedef enum ScreenState {
    SCREEN_MENU = 0,
    SCREEN_SIMULATION,
    SCREEN_GUIDE,
    SCREEN_ABOUT,
    SCREEN_EXIT // Khusus untuk perintah keluar aplikasi
} ScreenState;

#endif // SCREEN_TYPE_H