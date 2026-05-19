#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <cmath>
#include "camera.h"
#include "Primitives.h"
#include "lighting.h"

// Vista del sistema solar
enum class SolarView { FOLLOW_EARTH, TOP, FREE };
inline SolarView solarView = SolarView::FOLLOW_EARTH;

inline void applySolarCamera(float earthX, float earthY, float earthZ) {
    switch (solarView) {
        case SolarView::FOLLOW_EARTH:
            gluLookAt(earthX, earthY + 15.0f, earthZ + 20.0f,
                      earthX * 0.5f, earthY, earthZ * 0.5f,
                      0.0f, 1.0f, 0.0f);
            break;
        case SolarView::TOP:
            gluLookAt(0, 50, 0,  0, 0, 0,  0, 0, -1);
            break;
        case SolarView::FREE:
            gluLookAt(30, 20, 30,  0, 0, 0,  0, 1, 0);
            break;
    }
}

// Cuerpo celeste
struct Body { float r{}, g{}, b{}, radius{}; int seg = 8; };



inline void drawBody(const Body& b) {
    glColor3f(b.r, b.g, b.b);
    drawSphere(b.radius, b.seg, b.seg);
}

// Escena base: cubo + esfera + toro
inline void drawBaseScene() {
    glPushMatrix();
        glTranslatef(-3.0f, 0.0f, 0.0f);
        glColor3f(0.3f, 0.6f, 0.9f);
        drawCube(1.5f);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.9f, 0.5f, 0.2f);
        drawSphere(0.9f, 24, 24);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(3.0f, 0.0f, 0.0f);
        glColor3f(0.3f, 0.85f, 0.5f);
        drawTorus(0.3f, 0.8f, 16, 32);
    glPopMatrix();
}

// Proyecciones
inline void setupOrtho(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = static_cast<float>(w) / h;
    glOrtho(-6.0 * aspect, 6.0 * aspect, -6.0, 6.0, -20.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

inline void setupPerspective(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, static_cast<double>(w) / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Escena 1: Ortográfica
inline void renderOrtho(float) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(3, 4, 8,  0, 0, 0,  0, 1, 0);
    updateLight();
    drawBaseScene();
}

// Escena 2: Perspectiva
inline void renderPerspective(float) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(3, 4, 8,  0, 0, 0,  0, 1, 0);
    updateLight();
    drawBaseScene();
}

//Escena 3: Cámara libre
inline FreeCamera freeCamera;

inline void renderFreeCamera(float) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    freeCamera.apply();
    updateLight();
    drawBaseScene();
}

// Escena 4: Sistema solar
inline void renderSolarSystem(const float t) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const float Vs = t * 20.0f;
    const float Vt = t * 8.0f;

    // Posición de la Tierra para seguirla con la cámara
    const float earthAngle = Vt * 3.14159265f / 180.0f;
    const float earthX = 10.0f * std::sin(earthAngle);
    const float earthZ = 10.0f * std::cos(earthAngle);

    glLoadIdentity();
    applySolarCamera(earthX, 0.0f, earthZ);
    updateLight();

    constexpr Body sol   = {1.0f, 0.9f, 0.1f, 4.0f, 8};
    constexpr Body tierra = {0.2f, 0.4f, 1.0f, 2.0f, 8};
    constexpr Body luna  = {0.8f, 0.8f, 0.8f, 0.6f, 8};
    constexpr Body marte = {0.9f, 0.3f, 0.1f, 1.2f, 8};

    glPushMatrix();
        glRotatef(Vs, 0, 1, 0);
        drawBody(sol);

        // Tierra
        glPushMatrix();
            glRotatef(Vt, 0, 1, 0);
            glTranslatef(10.0f, 0.0f, 0.0f);
            glRotatef(3.0f * Vs, 0, 1, 0);
            drawBody(tierra);

            // Luna
            glPushMatrix();
                glRotatef(2.0f * Vt, 0, 1, 0);
                glTranslatef(3.5f, 0.0f, 0.0f);
                glRotatef(1.5f * Vs, 0, 1, 0);
                drawBody(luna);
            glPopMatrix();
        glPopMatrix();

        // Marte
        glPushMatrix();
            glRotatef(Vt * 0.6f, 0, 1, 0);
            glTranslatef(18.0f, 0.0f, 0.0f);
            glRotatef(Vs, 0, 1, 0);
            drawBody(marte);
        glPopMatrix();

    glPopMatrix();
}

// Escena 5 Trackball + gizmo
inline Trackball trackball;

inline void renderTrackball(float) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    trackball.apply();
    updateLight();
    drawGizmo(2.0f);
    drawBaseScene();
}