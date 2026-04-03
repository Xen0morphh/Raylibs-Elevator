#ifndef TRANSFORMASI_H
#define TRANSFORMASI_H

#include "raylib.h"

// Algoritma Pembuatan Garis
void DrawLineDDA(int x1, int y1, int x2, int y2, Color color);
void DrawLineBresenham(int x1, int y1, int x2, int y2, Color color);

// Algoritma Pembuatan Lingkaran (Bresenham / Midpoint)
void DrawCircleMidpoint(int xCenter, int yCenter, int radius, Color color);

// Bonus: Fungsi kustom untuk menggambar kotak berongga menggunakan algoritma garis Anda
void DrawRectCustom(int x, int y, int width, int height, Color color);

#endif