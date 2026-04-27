#include "shapes.h"

#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr float PI = 3.14159265358979323846f;

// Ejercicio 1
void drawSquare(float cx, float cy, float side) {
    float half = side / 2.0f;

    // Vértices calculados manualmente a partir del centro
    float x0 = cx - half,  y0 = cy - half;  // inferior izquierdo
    float x1 = cx + half,  y1 = cy - half;  // inferior derecho
    float x2 = cx + half,  y2 = cy + half;  // superior derecho
    float x3 = cx - half,  y3 = cy + half;  // superior izquierdo

    glBegin(GL_LINE_LOOP);
        glVertex2f(x0, y0);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
    glEnd();
}

// Ejercicio 2
void drawCircle(float cx, float cy, float radius, int segments) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        // Ángulo del vértice i, distribuido uniformemente
        float angle = 2.0f * PI * static_cast<float>(i) / static_cast<float>(segments);

        // Coordenadas calculadas manualmente (sin glRotate ni glTranslate)
        float x = cx + radius * std::cos(angle);
        float y = cy + radius * std::sin(angle);

        glVertex2f(x, y);
    }
    glEnd();
}

// Ejercicio 3
void drawCircleSequenceConcentric(float cx, float cy, float radius, int count, float reductionPct, int segments) {

    for (int i = 0; i < count; ++i) {
        drawCircle(cx, cy, radius, segments);

        // Reducción del radio según el porcentaje dado
        radius *= (1.0f - reductionPct);
    }
}
// Ejercicio 3b
void drawCircleSequenceConcentricLeft(float leftEdge, float cy, float radius, int count, float reductionPct, int segments) {

    for (int i = 0; i < count; ++i) {
        const float cx = leftEdge + radius;   // centro se desplaza para fijar el borde izquierdo
        drawCircle(cx, cy, radius, segments);
        radius *= (1.0f - reductionPct);
    }

    //drawCircleSequenceConcentric(leftEdge + radius, cy, radius, count, reductionPct, segments);
}

// Ejercicio 3c borde derecho fijo
void drawCircleSequenceConcentricRight(float rightEdge, float cy, float radius, int count, float reductionPct, int segments) {
    for (int i = 0; i < count; ++i) {
        float cx = rightEdge - radius;  // centro se desplaza para fijar el borde derecho
        drawCircle(cx, cy, radius, segments);
        radius *= (1.0f - reductionPct);
    }
}

// Ejercicio 3d borde superior fijo
void drawCircleSequenceConcentricTop(float cx, float topEdge, float radius, int count, float reductionPct, int segments) {
    for (int i = 0; i < count; ++i) {
        float cy = topEdge - radius;    // centro baja para fijar el borde superior
        drawCircle(cx, cy, radius, segments);
        radius *= (1.0f - reductionPct);
    }
}

// Ejercicio 3e borde inferior fijo
void drawCircleSequenceConcentricBottom(float cx, float bottomEdge, float radius, int count, float reductionPct, int segments) {
    for (int i = 0; i < count; ++i) {
        float cy = bottomEdge + radius; // centro sube para fijar el borde inferior
        drawCircle(cx, cy, radius, segments);
        radius *= (1.0f - reductionPct);
    }
}

//Ejercicio 4
void drawCircleSequenceHorizontal(float cx, float cy, float radius, int count, float reductionPct, int segments) {
    float currentX = cx;

    for (int i = 0; i < count; ++i) {
        drawCircle(currentX, cy, radius, segments);

        float nextRadius = radius * (1.0f - reductionPct);

        // El siguiente círculo se posiciona tangente al actual:
        // borde derecho del actual + radio del siguiente
        currentX += radius + nextRadius;
        radius = nextRadius;
    }
}

// Ejercicio 5
void drawCircleSequenceSpiral(float cx, float cy, float radius, int count, float reductionPct, float angleDeg, int segments) {
    float angle = 0.0f;

    for (int i = 0; i < count; ++i) {
        drawCircle(cx, cy, radius, segments);

        float nextRadius = radius * (1.0f - reductionPct);

        // Distancia entre centros: radio actual + radio siguiente (tangentes)
        float dist = radius + nextRadius;

        // Convertir ángulo acumulado a radianes y calcular nueva posición
        float rad = angle * PI / 180.0f;
        cx = cx + dist * std::cos(rad);
        cy = cy + dist * std::sin(rad);

        radius  = nextRadius;
        angle  += angleDeg;  // rotar el ángulo para el siguiente círculo
    }
}