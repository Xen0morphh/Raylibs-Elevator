# --- PENGATURAN COMPILER (LINUX/MAC) ---
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
INCLUDES = -I./header -I.

# Library KHUSUS LINUX/MACOS 
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Daftar file sumber
SRCS = Main.c src/About.c src/BuildingView.c src/Camera.c src/Guide.c src/Lift.c src/MechView.c src/Menu.c src/Simulation.c src/Transformasi.c src/UI.c src/Person.c

# Nama aplikasi output 
TARGET = Elevator_Simulation

# --- ATURAN BUILD ---
build:
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)