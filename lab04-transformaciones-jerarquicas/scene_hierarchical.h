#ifndef CG_LABS_SCENE_HIERARCHICAL_H
#define CG_LABS_SCENE_HIERARCHICAL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <cmath>
#include "Primitives.h"
#include "lighting.h"
#include "OBJLoader.h"

extern OBJLoader teapot;

// Parte I — Ejercicio 2
void renderP1Ej2(float t) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, 14,  0, 0, 0,  0, 1, 0);
    updateLight();

    glPushMatrix();
        glColor3f(1.0f, 0.9f, 0.1f);
        drawSphere(0.5f, 12, 12);
    glPopMatrix();

    glPushMatrix();
        glRotatef(t * 60.0f, 0, 0, 1);
        glTranslatef(4.0f, 0.0f, 0.0f);
        glRotatef(t * -60.0f, 0, 0, 1); //Rotcion propia de la teopot
        glScalef(0.4f, 0.4f, 0.4f);
        glColor3f(0.2f, 0.8f, 1.0f);
        teapot.dibujar();
    glPopMatrix();
}

// Ángulos de la grúa controlados desde main
inline float baseAngle  = 0.0f;   // rotación horizontal base  (A/D)
inline float upperAngle = 0.0f;   // inclinación columna α     (W/S)
inline float foreAngle  = 0.0f;   // inclinación brazo β       (Q/E)
inline float clawAngle  = 5.0f;   // largo de la cuerda        (Z/X)

constexpr float BASE_H   = 0.3f;
constexpr float COLUMN_H = 3.5f;
constexpr float ARM_L    = 3.0f;

// Parte I — Ejercicio 3: Grúa
void renderRoboticArm() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(5, 5, 10,  0, 2, 0,  0, 1, 0);
    updateLight();

    glPushMatrix();
        glRotatef(baseAngle, 0, 1, 0);

        // Base
        glPushMatrix();
            glScalef(2.0f, BASE_H, 1.2f);
            glColor3f(0.5f, 0.6f, 0.8f);
            drawCube(1.0f);
        glPopMatrix();

        glTranslatef(0.0f, BASE_H, 0.0f);

        glPushMatrix();
            glRotatef(upperAngle, 0, 0, 1);  // α — inclinación columna

            // Columna vertical
            glPushMatrix();
                glTranslatef(0.0f, COLUMN_H / 2.0f, 0.0f);
                glScalef(0.22f, COLUMN_H, 0.22f);
                glColor3f(0.7f, 0.7f, 0.85f);
                drawCube(1.0f);
            glPopMatrix();

            glTranslatef(0.0f, COLUMN_H, 0.0f);  // punta de la columna

            glPushMatrix();
                glRotatef(foreAngle - 90.0f, 0, 0, 1);  // β — brazo sale horizontal

                // Brazo horizontal
                glPushMatrix();
                    glTranslatef(0.0f, ARM_L / 2.0f, 0.0f);
                    glScalef(0.18f, ARM_L, 0.18f);
                    glColor3f(0.6f, 0.8f, 0.6f);
                    drawCube(1.0f);
                glPopMatrix();

                glTranslatef(0.0f, ARM_L, 0.0f);  // punta del brazo

                // Cuerda — cancelar rotación acumulada para que caiga vertical
                glPushMatrix();
                    float mat[16];
                    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
                    mat[0]=1; mat[1]=0; mat[2]=0;
                    mat[4]=0; mat[5]=1; mat[6]=0;
                    mat[8]=0; mat[9]=0; mat[10]=1;
                    glLoadMatrixf(mat);

                    float ropeLen = 0.5f + clawAngle * 0.12f;

                    disableLighting();
                    glBegin(GL_LINES);
                        glColor3f(0.9f, 0.9f, 0.9f);
                        glVertex3f(0.0f,  0.0f, 0.0f);
                        glVertex3f(0.0f, -ropeLen, 0.0f);
                    glEnd();
                    enableLighting();

                    glTranslatef(0.0f, -ropeLen, 0.0f);
                    glColor3f(0.9f, 0.7f, 0.1f);
                    glScalef(0.22f, 0.22f, 0.22f);
                    drawCube(1.0f);
                glPopMatrix();

            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

// Parte I — Ejercicio 4: Figura humana
void renderHumanFigure(float t) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 2, 8,  0, 1, 0,  0, 1, 0);
    updateLight();

    float swing = std::sin(t * 3.0f) * 35.0f;

    glPushMatrix();

        // Torso
        glPushMatrix();
            glTranslatef(0.0f, 1.5f, 0.0f);
            glColor3f(0.3f, 0.5f, 0.9f);
            glScalef(0.7f, 1.0f, 0.4f);
            drawCube(1.0f);
        glPopMatrix();

        // Cabeza
        glPushMatrix();
            glTranslatef(0.0f, 2.4f, 0.0f);
            glColor3f(0.95f, 0.75f, 0.6f);
            drawSphere(0.35f, 12, 12);
        glPopMatrix();

        // Brazo izquierdo
        glPushMatrix();
            glTranslatef(-0.55f, 1.9f, 0.0f);
            glRotatef(swing, 1, 0, 0);
            glTranslatef(0.0f, -0.4f, 0.0f);
            glColor3f(0.3f, 0.5f, 0.9f);
            glScalef(0.2f, 0.8f, 0.2f);
            drawCube(1.0f);
        glPopMatrix();

        // Brazo derecho
        glPushMatrix();
            glTranslatef(0.55f, 1.9f, 0.0f);
            glRotatef(-swing, 1, 0, 0);
            glTranslatef(0.0f, -0.4f, 0.0f);
            glColor3f(0.3f, 0.5f, 0.9f);
            glScalef(0.2f, 0.8f, 0.2f);
            drawCube(1.0f);
        glPopMatrix();

        // Pierna izquierda
        glPushMatrix();
            glTranslatef(-0.22f, 0.9f, 0.0f);
            glRotatef(-swing, 1, 0, 0);
            glTranslatef(0.0f, -0.5f, 0.0f);
            glColor3f(0.2f, 0.2f, 0.6f);
            glScalef(0.25f, 1.0f, 0.25f);
            drawCube(1.0f);
        glPopMatrix();

        // Pierna derecha
        glPushMatrix();
            glTranslatef(0.22f, 0.9f, 0.0f);
            glRotatef(swing, 1, 0, 0);
            glTranslatef(0.0f, -0.5f, 0.0f);
            glColor3f(0.2f, 0.2f, 0.6f);
            glScalef(0.25f, 1.0f, 0.25f);
            drawCube(1.0f);
        glPopMatrix();

    glPopMatrix();
}

#endif //CG_LABS_SCENE_HIERARCHICAL_H