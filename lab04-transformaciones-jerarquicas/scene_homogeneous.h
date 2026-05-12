#ifndef CG_LABS_SCENE_HOMOGENEOUS_H
#define CG_LABS_SCENE_HOMOGENEOUS_H
//Matirces homogeneas manuales
//#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <cmath>
#include "math_utils.h"
//#include "../shared/Primitives.h"
//#include "../lab03-transformaciones-escena/OBJLoader.h"
#include "Primitives.h"
#include "OBJLoader.h"

extern OBJLoader teapot;

// Polígono simple para demostrar matrices 2D
inline void drawTriangle() {
    glBegin(GL_TRIANGLES);
        glVertex3f( 0.0f,  0.5f, 0.0f);
        glVertex3f(-0.4f, -0.3f, 0.0f);
        glVertex3f( 0.4f, -0.3f, 0.0f);
    glEnd();
}

// Parte II — Ejercicio 2
// Traslación, rotación y escala aplicadas con glMultMatrixf
void renderP2Ej2(float t) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, 8,  0, 0, 0,  0, 1, 0);

    float angle = t * 60.0f;

    // Traslación horizontal (oscila en X)
    glPushMatrix();
        Mat4 T = mat4Translate(std::sin(t) * 2.0f, 0.0f, 0.0f);
        glApplyMatrix(T);
        glColor3f(1.0f, 0.4f, 0.4f);
        drawTriangle();
    glPopMatrix();

    // Rotación constante
    glPushMatrix();
        Mat4 R = mat4RotateZ(angle);
        glApplyMatrix(R);
        glColor3f(0.4f, 1.0f, 0.4f);
        drawTriangle();
    glPopMatrix();

    // Escala pulsante
    glPushMatrix();
        float s = 0.5f + std::abs(std::sin(t)) * 1.0f;
        Mat4 T2 = mat4Translate(0.0f, -1.5f, 0.0f);
        Mat4 S  = mat4Scale(s, s, 1.0f);
        Mat4 M  = mat4Mul(T2, S);
        glApplyMatrix(M);
        glColor3f(0.4f, 0.6f, 1.0f);
        drawTriangle();
    glPopMatrix();
}

// Parte II — Ejercicio 3
// Rota -> traslada → escala, tusando matrices maueale combiandas
void renderP2Ej3(float t) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, 8,  0, 0, 0,  0, 1, 0);

    float angle = t * 45.0f;
    float tx    = std::sin(t * 0.5f) * 2.0f;
    float s     = 0.5f + std::abs(std::sin(t * 0.3f)) * 1.0f;

    // Combinación: S * T * R  (se aplica de derecha a izquierda, primero R, luego T, luego S)
    Mat4 R = mat4RotateZ(angle);
    Mat4 T = mat4Translate(tx, 0.0f, 0.0f);
    Mat4 S = mat4Scale(s, s, 1.0f);
    Mat4 M = mat4Mul(mat4Mul(S, T), R);

    glPushMatrix();
        glApplyMatrix(M);
        glColor3f(0.9f, 0.6f, 0.2f);
        drawTriangle();
    glPopMatrix();
}

// Parte II — Ejercicio 4a: Lab03 Ej1 con matrices homogéneas
// Tetera orbita a 4 unidades del origen girando en Z
void renderP2Ej4a(float t) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, 14,  0, 0, 0,  0, 1, 0);

    float orbitAngle = t * 60.0f;
    float selfAngle  = t * 90.0f;

    // Órbita: R_orbit * T(4,0,0) * R_self * S
    Mat4 Rorbit = mat4RotateZ(orbitAngle);
    Mat4 Tpos   = mat4Translate(4.0f, 0.0f, 0.0f);
    Mat4 Rself  = mat4RotateZ(selfAngle);
    Mat4 S      = mat4Scale(0.4f, 0.4f, 0.4f);
    Mat4 M      = mat4Mul(mat4Mul(mat4Mul(Rorbit, Tpos), Rself), S);

    glPushMatrix();
        glApplyMatrix(M);
        glColor3f(0.2f, 0.8f, 1.0f);
        teapot.dibujar();
    glPopMatrix();
}

// Parte II — Ejercicio 4b: Lab03 Ej2 con matrices homogéneas
// Tetera se traslada en X de -8 a 8 y rota sobre sí misma
void renderP2Ej4b(float t) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 4, 16,  0, 0, 0,  0, 1, 0);

    float posX  = std::sin(t) * 8.0f;
    float angle = t * 90.0f;

    Mat4 T = mat4Translate(posX, 0.0f, 0.0f);
    Mat4 R = mat4RotateZ(angle);
    Mat4 S = mat4Scale(0.4f, 0.4f, 0.4f);
    Mat4 M = mat4Mul(mat4Mul(T, R), S);

    glPushMatrix();
        glApplyMatrix(M);
        glColor3f(1.0f, 0.6f, 0.2f);
        teapot.dibujar();
    glPopMatrix();
}

// Parte II — Ejercicio 4c, Lab03 Ej3 con matrices homogeneas
// Tetera , toro orbitando , cubo independiente, usando matrices manuales
void renderP2Ej4c(float t) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 8, 20,  0, 0, 0,  0, 1, 0);

    float posX    = std::sin(t) * 5.0f;
    float rotZ    = t * 60.0f;
    float rotToro = t * 180.0f;

    // Tetera
    Mat4 T     = mat4Translate(posX, 0.0f, 0.0f);
    Mat4 Rz    = mat4RotateZ(rotZ);
    Mat4 S     = mat4Scale(0.35f, 0.35f, 0.35f);
    Mat4 Mtea  = mat4Mul(mat4Mul(T, Rz), S);

    glPushMatrix();
        glApplyMatrix(Mtea);
        glColor3f(0.2f, 0.8f, 1.0f);
        teapot.dibujar();
    glPopMatrix();

    // Toro, misma posición de la tetera , órbita propia
    Mat4 Rtoro = mat4RotateZ(rotToro);
    Mat4 Ttoro = mat4Translate(3.0f, 0.0f, 0.0f);
    Mat4 Mtoro = mat4Mul(mat4Mul(T, mat4Mul(Rtoro, Ttoro)), mat4Scale(1.0f, 1.0f, 1.0f));

    glPushMatrix();
        glApplyMatrix(Mtoro);
        glColor3f(1.0f, 0.4f, 0.8f);
        drawTorus(0.2f, 0.6f, 12, 24);
    glPopMatrix();

    // Cubo independiente
    Mat4 Tcubo = mat4Translate(-6.0f, 0.0f, 0.0f);
    Mat4 Rcubo = mat4RotateX(t * 45.0f);
    Mat4 Mcubo = mat4Mul(Tcubo, Rcubo);

    glPushMatrix();
        glApplyMatrix(Mcubo);
        glColor3f(0.9f, 0.8f, 0.2f);
        drawCube(2.0f);
    glPopMatrix();
}

#endif //CG_LABS_SCENE_HOMOGENEOUS_H