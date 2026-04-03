#include "../header/Transformasi.h"
#include <stdlib.h> // Untuk fungsi abs()
#include <math.h>   // Untuk fungsi round() pada DDA
#include "raylib.h"  // Untuk tipe Color dan fungsi DrawPixel()

void DrawLineDDA(int x1, int y1, int x2, int y2, Color color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    
    // Tentukan langkah (step) terbesar antara jarak X atau jarak Y
    int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
    
    // Hitung penambahan per langkah (bisa berupa desimal)
    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;
    
    float x = x1;
    float y = y1;
    
    // Gambar piksel awal hingga akhir
    for (int i = 0; i <= steps; i++) {
        DrawPixel(round(x), round(y), color); // Gambar piksel dengan pembulatan
        x += xInc;
        y += yInc;
    }
}

void DrawLineBresenham(int x1, int y1, int x2, int y2, Color color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    
    // Tentukan arah pergerakan garis (+1 atau -1)
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    
    int err = dx - dy;
    
    while (1) {
        DrawPixel(x1, y1, color); // Gambar piksel saat ini
        
        // Hentikan loop jika sudah sampai di titik akhir
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        
        // Sesuaikan posisi X
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        // Sesuaikan posisi Y
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

static void PlotCirclePoints(int xc, int yc, int x, int y, Color c) {
    DrawPixel(xc + x, yc + y, c);
    DrawPixel(xc - x, yc + y, c);
    DrawPixel(xc + x, yc - y, c);
    DrawPixel(xc - x, yc - y, c);
    DrawPixel(xc + y, yc + x, c);
    DrawPixel(xc - y, yc + x, c);
    DrawPixel(xc + y, yc - x, c);
    DrawPixel(xc - y, yc - x, c);
}

void DrawCircleMidpoint(int xCenter, int yCenter, int radius, Color color) {
    int x = 0;
    int y = radius;
    int p = 1 - radius; // Parameter keputusan awal
    
    PlotCirclePoints(xCenter, yCenter, x, y, color);
    
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1; // Pindah horizontal
        } else {
            y--;
            p += 2 * (x - y) + 1; // Pindah diagonal
        }
        PlotCirclePoints(xCenter, yCenter, x, y, color);
    }
}

void DrawRectCustom(int x, int y, int width, int height, Color color) {
    // Garis Atas
    DrawLineBresenham(x, y, x + width, y, color);
    // Garis Bawah
    DrawLineBresenham(x, y + height, x + width, y + height, color);
    // Garis Kiri
    DrawLineBresenham(x, y, x, y + height, color);
    // Garis Kanan
    DrawLineBresenham(x + width, y, x + width, y + height, color);
}