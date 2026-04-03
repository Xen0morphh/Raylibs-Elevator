CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Beritahu kompilator untuk mencari header di folder "header" dan folder root "."
INCLUDES = -I./header -I.

# Library KHUSUS LINUX/MACOS 
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Daftar semua file .c Anda (Sudah termasuk file UI dan View yang baru)
SRCS = SRCS = Main.c src/About.c src/BuildingView.c src/Camera.c src/Guide.c src/Lift.c src/MechView.c src/Menu.c src/Simulation.c src/Transformasi.c src/UI.c src/Person.c

# Nama aplikasi output (TANPA .exe untuk Linux/Mac)
TARGET = simulasi_lift

build:
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
