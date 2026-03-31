CC = gcc
CFLAGS = -Wall -Wextra -std=c99
# Beritahu kompilator untuk mencari header di folder "header" dan folder root "."
INCLUDES = -I./header -I. 
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Masukkan Main.c (root) dan semua file .c di dalam folder src
SRCS = Main.c src/About.c src/Guide.c src/Lift.c src/Menu.c src/Simulation.c src/Transformasi.c
TARGET = simulasi_lift

build:
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)