#ifndef GUIDE_H
#define GUIDE_H

#include "raylib.h"

// 1. Fungsi untuk memuat aset (seperti gambar Bg_Guide.png) saat masuk menu Guide
void InitGuide(void);

// 2. Fungsi untuk menghapus aset dari RAM saat keluar dari menu Guide
void UnloadGuide(void);

// 3. Fungsi untuk menggambar tampilan halaman panduan/instruksi
void DrawGuideScreen(void);

// 4. Fungsi untuk menangani input (seperti menekan Backspace untuk kembali)
//    Biasanya fungsi ini mengembalikan angka (int) untuk memberi tahu Main.c harus pindah ke screen mana.
int UpdateGuide(void);

#endif