-----

# System Programming and Extensions Assessment

This repository contains solutions for a multi-part assessment covering C system-level programming, x86 Assembly, Python C extensions, and C shared libraries.

## Table of Contents

  * [Prerequisites](https://www.google.com/search?q=%23-prerequisites)
  * [Task 1: ELF Disassembler](https://www.google.com/search?q=%23%EF%B8%8F-task-1-elf-disassembler)
  * [Task 2: x86 Assembly ATM](https://www.google.com/search?q=%23-task-2-x86-assembly-atm)
  * [Task 3: IoT Dashboard (Python C Extension)](https://www.google.com/search?q=%23-task-3-iot-dashboard-python-c-extension)
  * [Task 4: Encrypted Diary Manager (Shared Library)](https://www.google.com/search?q=%23-task-4-encrypted-diary-manager-shared-library)

-----

## 🔧 Prerequisites

Before compiling and running these tasks, please ensure you have the following tools installed:

  * **GCC (C Compiler):** `gcc`
  * **Binutils:** `objdump` (Usually installed with `gcc`)
  * **NASM Assembler:** `nasm`
  * **32-bit Libraries (for 64-bit systems):**
      * On Debian/Ubuntu: `sudo apt-get install gcc-multilib`
  * **Python 3:** `python3`
  * **Python 3 Development Headers:**
      * On Debian/Ubuntu: `sudo apt-get install python3-dev`
  * **Python `matplotlib`:** `pip install matplotlib`

-----

## 🏛️ Task 1: ELF Disassembler

A C program that accepts the path to an ELF executable, uses the `objdump` command-line tool to disassemble its `.text` (code) section, and saves the output to `disassembly.txt`.

### Files

  * `disassemble.c`

### How to Run

1.  **Compile the program:**

    ```bash
    gcc disassemble.c -o disassemble
    ```

2.  **Run the program** (using `/bin/ls` as an example target):

    ```bash
    ./disassemble /bin/ls
    ```

3.  **Check the output:**

    ```bash
    cat disassembly.txt
    ```

-----

## 🏦 Task 2: x86 Assembly ATM

A 32-bit x86 assembly program (written for NASM) that simulates basic ATM functions: Deposit, Withdraw, Check Balance, and Exit. It uses only Linux syscalls for all I/O.

### Files

  * `atm.asm`

### How to Run

1.  **Install 32-bit libraries (if on a 64-bit system):**

    ```bash
    sudo apt-get install gcc-multilib
    ```

2.  **Assemble with NASM (creating a 32-bit ELF object):**

    ```bash
    nasm -f elf32 atm.asm -o atm.o
    ```

3.  **Link with `ld` (creating a 32-bit executable):**

    ```bash
    ld -m elf_i386 atm.o -o atm
    ```

4.  **Run the ATM simulation:**

    ```bash
    ./atm
    ```

-----

## 📡 Task 3: IoT Dashboard (Python C Extension)

A lightweight monitoring backend for a simulated IoT dashboard. A C program simulates sensor readings and is compiled as a Python extension module. A Python script uses this module to fetch data every 2 seconds and plots it in real-time using `matplotlib`.

### Files

  * `sensor_module.c` (The C extension source)
  * `setup.py` (The build script for the extension)
  * `monitor.py` (The Python dashboard)

### How to Run

1.  **Install prerequisites:**

    ```bash
    sudo apt-get install python3-dev
    pip install matplotlib
    ```

2.  **Build the C extension:**

    ```bash
    python3 setup.py build
    ```

3.  **Copy the compiled shared object (`.so`) file** from the `build` directory to the current directory.

    ```bash
    # This command finds the .so file and copies it
    find build -name "*.so" -exec cp {} . \;
    ```

4.  **Run the Python dashboard:**

    ```bash
    python3 monitor.py
    ```

    A `matplotlib` window will open and display the graphs updating in real-time.

-----

## 📓 Task 4: Encrypted Diary Manager (Shared Library)

A command-line diary manager that encrypts and decrypts entries. The encryption logic (a simple Caesar cipher) is contained in a shared library (`libdiary.so`) which is dynamically loaded by the main C executable at runtime.

### Files

  * `libdiary.c` (Source for the shared library `encrypt`/`decrypt` functions)
  * `diary_manager.c` (Source for the main executable)

### How to Run

1.  **Compile the shared library (`.so`):**

    ```bash
    gcc -fPIC -shared -o libdiary.so libdiary.c
    ```

2.  **Compile the main executable** (linking the dynamic loader library `-ldl`):

    ```bash
    gcc diary_manager.c -o diary_manager -ldl
    ```

3.  **Run the diary manager:**

    ```bash
    ./diary_manager
    ```

    The executable will automatically find and load `libdiary.so` from the same directory.