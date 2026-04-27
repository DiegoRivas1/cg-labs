# cg-labs

Laboratorios de Computación Gráfica  
Curso: Computación Gráfica

## Estructura

```
cg-labs/
├── lab01/
├── lab02/   ← Transformaciones Geométricas en OpenGL
└── ...
```

## Stack

- C++20
- CMake 4.1
- GLFW + GLAD + OpenGL (modo legacy)
- CLion / Windows

## Uso

Cada laboratorio es un proyecto CMake independiente.  
Abre la carpeta `labXX/` como proyecto en CLion y compila normalmente.

> La carpeta `external/` con GLAD y GLFW debe estar un nivel arriba de cada lab,
> o ajusta la variable `EXTERNAL_DIR` en el `CMakeLists.txt` correspondiente.