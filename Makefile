CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Beritahu kompilator untuk mencari header di folder "header" dan folder root "."
INCLUDES = -I./header -I.

# Library KHUSUS WINDOWS (Sangat penting agar tidak error saat compile)
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm -lm

# Daftar semua file .c Anda
SRCS = Main.c src/About.c src/Camera.c src/Guide.c src/Lift.c src/Menu.c src/Simulation.c src/Transformasi.c

# Nama aplikasi output (tambahkan .exe untuk Windows)
TARGET = simulasi_lift.exe

build:
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	del $(TARGET)

# Cara pakai:
# 1. Simpan file ini sebagai "Makefile.win" di folder proyek Anda.		
# 2. Jalankan perintah berikut di terminal:
#    mingw32-make -f Makefile.win
# 3. Untuk membersihkan file .exe yang dihasilkan, jalankan:
#    mingw32-make -f Makefile.win clean	
