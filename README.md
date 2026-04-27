# cg-labs

Laboratorios de Computación Gráfica

## Estructura

```
cg-labs/
├── CMakeLists.txt       ← orquestador raíz
├── cmake/
│   └── utils.cmake      ← detección automática de labs
├── external/            ← GLAD + GLFW
├── lab02/
├── lab03/
└── ...
```

Cada laboratorio es un subproyecto independiente. Al agregar una carpeta `labXX/` con su `CMakeLists.txt`, se detecta y compila automáticamente.

---

## Stack

- C++20 · CMake 4.1+ · GLFW · GLAD · OpenGL modo inmediato
- Compilador: MinGW (MSYS2)

---

## Configuración del entorno (MSYS2)

### 1. Instalar dependencias

Abrir **MSYS2 MinGW 64-bit** y ejecutar:

```bash
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-glfw mingw-w64-x86_64-glad
```

### 2. Compilar todos los labs (sin IDE)

```bash
git clone https://github.com/tu-usuario/cg-labs.git
cd cg-labs
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

### 3. Ejecutar un lab específico

```bash
./lab02/lab02.exe
```

---

## Compilar con CLion

1. `File → Settings → Build, Execution, Deployment → Toolchains`
    - Agregar toolchain tipo **MinGW**
    - Path: `C:\msys64\mingw64`
    - CLion detecta automáticamente `gcc`, `g++` y `cmake`

2. `File → Open` → seleccionar la carpeta raíz `cg-labs/`

3. En el selector de configuraciones elegir el lab deseado (`lab02`, etc.) y presionar `▶ Run` o `Shift+F10`