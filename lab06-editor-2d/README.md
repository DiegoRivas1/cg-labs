# lab06-editor-2d — Editor Gráfico 2D en OpenGL

**Curso:** Computación Gráfica — UNSA  
**Autor:** Percy Maldonado Quispe  
**Fecha:** 25 de mayo de 2026

## Descripción

Editor gráfico 2D interactivo con panel lateral (Dear ImGui). Permite crear, seleccionar y transformar primitivas geométricas usando mouse y teclado. Las transformaciones se aplican mediante matrices homogéneas construidas manualmente.

---

## Dependencias adicionales

```bash
pacman -S mingw-w64-ucrt-x86_64-imgui
```

---

## Arquitectura

| Archivo | Responsabilidad |
|---------|----------------|
| `main.cpp` | Ventana GLFW, callbacks, loop principal, panel ImGui |
| `Editor.h` | Lógica central: modos, construcción de figuras, transformaciones |
| `Scene.h` | Almacenamiento, selección y gestión de objetos |
| `Shape.h` | Jerarquía de clases + Factory |
| `math_utils.h` | Matrices homogéneas 4×4 (de `shared/`) |

### Jerarquía de clases

```
Shape  (abstracta)
├── PointShape
├── LineShape
├── PolylineShape
└── PolygonShape

ShapeFactory::create(ShapeType) → unique_ptr<Shape>
```

---

## Herramientas

| Tecla | Herramienta |
|-------|-------------|
| `1` | Punto |
| `2` | Línea (2 clics) |
| `3` | Polilínea (Enter para finalizar) |
| `4` | Polígono (clic derecho para cerrar) |
| `5` | Selección |

---

## Controles

| Tecla / Mouse | Acción |
|---------------|--------|
| Clic izquierdo | Agregar vértice / seleccionar |
| Clic derecho | Cerrar polígono / cancelar |
| `Enter` | Finalizar polilínea |
| `DEL` | Eliminar objeto seleccionado |
| `F` | Activar/desactivar relleno |
| Flechas | Trasladar objeto seleccionado |
| `R` / `T` | Rotar objeto seleccionado |
| `+` / `-` | Escalar objeto seleccionado |
| `ESC` | Salir |

---

## Capturas

### Panel y herramienta de puntos
![Puntos](screenshots/points.png)

### Líneas y polilíneas
![Líneas](screenshots/lines.png)

### Polígonos con relleno
![Polígonos](screenshots/polygons.png)

### Selección y transformaciones
![Transformaciones](screenshots/transforms.png)

### Escena final
![Escena final](screenshots/scene_final.png)

> Coloca tus capturas en `lab06-editor-2d/screenshots/`.
