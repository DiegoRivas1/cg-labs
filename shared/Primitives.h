#pragma once
#include <glad/glad.h>
#include <cmath>

constexpr float TAU = 6.28318530717958647692f;

// Esfera TAU/4 = PI/2
/*
El problema es el rango de latitud. Con TAU/4.0f * (-1 + i/stacks) cuando i va de 0 a stacks-1,
el rango va de -TAU/4 hasta casi 0 — solo el hemisferio inferior.
Necesitas que llegue hasta +TAU/4:

El 2.0f hace que cuando i = stacks, el valor llegue a TAU/4 * (-1 + 2) = TAU/4 = π/2. Sin el 2.0f,
cuando i = stacks llegamos a TAU/4 * (-1 + 1) = 0.
Eso es exactamente la mitad del recorrido.
 */
inline void drawSphere(float radius, int slices, int stacks) {

    for (int i = 0; i < stacks; ++i) {
        float lat0 = TAU / 4.0f * (-1.0f + 2.0f * (float)i       / stacks);
        float lat1 = TAU / 4.0f * (-1.0f + 2.0f * (float)(i + 1) / stacks);
        float z0  = std::sin(lat0), zr0 = std::cos(lat0);
        float z1  = std::sin(lat1), zr1 = std::cos(lat1);

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float lng = TAU * (float)j / slices;
            float x = std::cos(lng), y = std::sin(lng);
            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(radius * x * zr0, radius * y * zr0, radius * z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
        }
        glEnd();
    }
}

// Toro
inline void drawTorus(float innerR, float outerR, int sides, int rings) {
    for (int i = 0; i < rings; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= sides; ++j) {
            for (int k = 0; k <= 1; ++k) {
                float s = (float)((i + k) % rings) + 0.5f;
                float t = (float)(j % sides);
                float u = s / rings  * TAU;
                float v = t / sides  * TAU;
                float x = (outerR + innerR * std::cos(v)) * std::cos(u);
                float y = (outerR + innerR * std::cos(v)) * std::sin(u);
                float z =  innerR * std::sin(v);
                float nx = std::cos(v) * std::cos(u);
                float ny = std::cos(v) * std::sin(u);
                float nz = std::sin(v);
                glNormal3f(nx, ny, nz);
                glVertex3f(x, y, z);
            }
        }
        glEnd();
    }
}
//cubo
inline void drawCube(float size) {
    float h = size / 2.0f;
    glBegin(GL_QUADS);
        // frente
        glNormal3f(0,0,1);
        glVertex3f(-h,-h, h); glVertex3f( h,-h, h);
        glVertex3f( h, h, h); glVertex3f(-h, h, h);
        // atrás
        glNormal3f(0,0,-1);
        glVertex3f( h,-h,-h); glVertex3f(-h,-h,-h);
        glVertex3f(-h, h,-h); glVertex3f( h, h,-h);
        // izquierda
        glNormal3f(-1,0,0);
        glVertex3f(-h,-h,-h); glVertex3f(-h,-h, h);
        glVertex3f(-h, h, h); glVertex3f(-h, h,-h);
        // derecha
        glNormal3f(1,0,0);
        glVertex3f( h,-h, h); glVertex3f( h,-h,-h);
        glVertex3f( h, h,-h); glVertex3f( h, h, h);
        // arriba
        glNormal3f(0,1,0);
        glVertex3f(-h, h, h); glVertex3f( h, h, h);
        glVertex3f( h, h,-h); glVertex3f(-h, h,-h);
        // abajo
        glNormal3f(0,-1,0);
        glVertex3f(-h,-h,-h); glVertex3f( h,-h,-h);
        glVertex3f( h,-h, h); glVertex3f(-h,-h, h);
    glEnd();
}