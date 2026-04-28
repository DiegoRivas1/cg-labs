# cg-labs

Laboratorios de Computación Gráfica — UNSA

## Estructura

```
cg-labs/
├── CMakeLists.txt         ← orquestador raíz
├── cmake/
│   └── utils.cmake        ← detección automática de labs
├── external/              ← GLAD + GLFW
├── cmake-build-debug/     ← generado por CLion (no se sube al repo)
├── lab02/
├── lab03/
└── ...
```

Cada laboratorio es un subproyecto independiente. Al agregar una carpeta `labXX/` con su `CMakeLists.txt`, se detecta y compila automáticamente.

---

## Stack

- C++20 · CMake 4.1+ · GLFW · GLAD · OpenGL modo inmediato
- Compilador: GCC 15 via MSYS2 ucrt64

---

## Configuración del entorno (MSYS2)

Instalar dependencias una sola vez. Abrir la terminal correspondiente en MSYS2:

**Opción A UCRT64** (recomendado, runtime moderno de Windows 10/11):
```bash
pacman -S mingw-w64-ucrt-x86_64-cmake \
          mingw-w64-ucrt-x86_64-gcc \
          mingw-w64-ucrt-x86_64-glfw \
          mingw-w64-ucrt-x86_64-glad
```

**Opción B MinGW64** (runtime clásico, mayor compatibilidad):
```bash
pacman -S mingw-w64-x86_64-cmake \
          mingw-w64-x86_64-gcc \
          mingw-w64-x86_64-glfw \
          mingw-w64-x86_64-glad
```

> Usar solo una opción, no mezclar paquetes de ucrt64 y mingw64.

---

## Ejecutar con CLion (recomendado)

1. `File → Settings → Build, Execution, Deployment → Toolchains`
   - Agregar toolchain tipo **MinGW**
   - Path: `D:\PROGRAMAS\msys64\ucrt64` (ajustar según donde instalaste MSYS2)
2. `File → Open` → seleccionar la carpeta raíz `cg-labs/`
3. Elegir la configuración deseada (`lab02`, etc.) y presionar `▶ Run` o `Shift+F10`

CLion genera `cmake-build-debug/` automáticamente.

---

## Ejecutar sin IDE (terminal MSYS2 UCRT64)

```bash
# Desde la raíz del repo
mkdir build && cd build

cmake .. -G "MinGW Makefiles" \
  -DCMAKE_C_COMPILER="D:/PROGRAMAS/msys64/ucrt64/bin/gcc.exe" \
  -DCMAKE_CXX_COMPILER="D:/PROGRAMAS/msys64/ucrt64/bin/g++.exe"

cmake --build .

# Ejecutar un lab
./lab02/lab02.exe
```

> Ajusta el path de `gcc.exe` según donde tengas instalado MSYS2.  
> Si ya compilaste antes, borra la carpeta `build/` antes de reconfigurar:
> ```bash
> rm -rf build
> ```

### Compilar un solo lab

Para compilar únicamente un laboratorio específico sin recompilar todo:

```bash
# Solo compilar lab02
cmake --build . --target lab02

# Compilar y ejecutar
cmake --build . --target lab02 && ./lab02/lab02.exe
```

> En CLion el equivalente es seleccionar el target en el selector de configuraciones
> (arriba a la derecha) y presionar `▶ Run` solo compila ese lab.