# 🚀 Elevator Simulation (Raylib + C)

A simple yet powerful **Elevator Simulation** project built using **C** and **Raylib**. This project demonstrates graphical rendering, basic simulation logic, and structured C project organization using a **Makefile**.

---

## 📁 Project Structure

```
.
├── assets/              # Images, textures, and other resources
├── header/              # Header files (.h)
├── src/                 # Source files (.c)
├── Elevator_Simulation.exe
├── Main.c
├── Makefile
├── Makefile.win
├── resource.c
├── resource.o
├── resource.rc
├── Screen_type.h
├── readme.md
```

---

## ⚙️ Requirements

Make sure you have installed:

* **C Compiler** (GCC / MinGW)
* **Raylib Library**
* **Make**

### 🔧 Install Raylib (Windows - MinGW)

1. Download Raylib from official source
2. Extract and set include & lib path
3. Add to environment variables (optional)

---

## 🛠️ How to Build

### ▶️ Using Makefile (Recommended)

Open terminal in project folder:

```bash
make
```

If you're on Windows with MinGW:

```bash
make -f Makefile.win
```

---

## ▶️ How to Run

After build:

```bash
./Elevator_Simulation.exe
```

Or simply double click the `.exe` file.

---

## 🎮 Controls (Example)

> You can adjust this based on your implementation

* `UP / DOWN` → Move elevator
* `ESC` → Exit simulation

---

## 💡 Features

* Real-time rendering using Raylib
* Elevator movement simulation
* Organized project structure (src, header, assets)
* Cross-platform build using Makefile

---

## 🧠 How It Works (Simple Explanation)

The simulation uses:

* **Game Loop** from Raylib (`InitWindow → Update → Draw → Close`)
* Elevator state (position, direction)
* Input handling for controlling movement
* Rendering objects on screen (floors, elevator, UI)

---

## 📌 Notes

* Make sure Raylib is correctly linked
* If build fails, check compiler path & library path
* `.exe` is already provided if you don’t want to build

---

## 👨‍💻 Author

Made with Xenomorph using C & Raylib

---

## ⭐ Contribute

Feel free to fork, improve, or add features to this project!
