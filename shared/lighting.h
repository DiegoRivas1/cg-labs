#pragma once
#include <windows.h>
#include <glad/glad.h>
#include <GL/glu.h>

inline float lightPos[] = { 5.0f, 10.0f, 5.0f, 1.0f };

inline void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
    // Normalizar normales para corregir efectos cuando hay escalados no uniformes
    glEnable(GL_NORMALIZE);

    // Valores más moderados para evitar sobreiluminación
    float ambient[]  = { 0.10f, 0.10f, 0.10f, 1.0f };  // luz base baja
    float diffuse[]  = { 0.75f, 0.75f, 0.75f, 1.0f };  // difusa principal
    float specular[] = { 0.20f, 0.20f, 0.20f, 1.0f };  // algo de especular

    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Material con algo de especular y brillo moderado
    float matSpec[] = { 0.20f, 0.20f, 0.20f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0f);
}

inline void updateLight() {
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

inline void disableLighting() { glDisable(GL_LIGHTING); }
inline void enableLighting()  { glEnable(GL_LIGHTING);  }