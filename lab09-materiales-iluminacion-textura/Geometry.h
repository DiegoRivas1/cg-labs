#pragma once
#include <functional>
#include <glad/glad.h>
#include <GL/glu.h>
#include <cmath>
#include "Primitives.h"

constexpr float GEO_PI = 3.14159265f;

inline void drawTerrain(float texRepeat = 8.0f) {
    float h = 20.0f;
    glNormal3f(0, 1, 0);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0);                  glVertex3f(-h, 0, -h);
        glTexCoord2f(texRepeat,0);          glVertex3f( h, 0, -h);
        glTexCoord2f(texRepeat,texRepeat);  glVertex3f( h, 0,  h);
        glTexCoord2f(0,texRepeat);          glVertex3f(-h, 0,  h);
    glEnd();
}

inline void drawCylinder(float radius, float height, int slices, float texRepeat = 1.0f) {
    float step = 2.0f * GEO_PI / slices;
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; ++i) {
        float angle = i * step;
        float x = radius * std::cos(angle);
        float z = radius * std::sin(angle);
        float u = (float)i / slices * texRepeat;
        glNormal3f(x / radius, 0, z / radius);
        glTexCoord2f(u, 0);         glVertex3f(x, 0,      z);
        glTexCoord2f(u, texRepeat); glVertex3f(x, height, z);
    }
    glEnd();
}

inline void drawHouseWalls(float texRepeat = 2.0f) {
    float w = 4.0f, h = 3.0f, d = 5.0f;

    glNormal3f(0, 0, 1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0);         glVertex3f(-w,0, d);
        glTexCoord2f(texRepeat,0); glVertex3f( w,0, d);
        glTexCoord2f(texRepeat,1); glVertex3f( w,h, d);
        glTexCoord2f(0,1);         glVertex3f(-w,h, d);
    glEnd();

    glNormal3f(0, 0, -1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0);         glVertex3f( w,0,-d);
        glTexCoord2f(texRepeat,0); glVertex3f(-w,0,-d);
        glTexCoord2f(texRepeat,1); glVertex3f(-w,h,-d);
        glTexCoord2f(0,1);         glVertex3f( w,h,-d);
    glEnd();

    glNormal3f(-1, 0, 0);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0);         glVertex3f(-w,0,-d);
        glTexCoord2f(texRepeat,0); glVertex3f(-w,0, d);
        glTexCoord2f(texRepeat,1); glVertex3f(-w,h, d);
        glTexCoord2f(0,1);         glVertex3f(-w,h,-d);
    glEnd();

    glNormal3f(1, 0, 0);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0);         glVertex3f( w,0, d);
        glTexCoord2f(texRepeat,0); glVertex3f( w,0,-d);
        glTexCoord2f(texRepeat,1); glVertex3f( w,h,-d);
        glTexCoord2f(0,1);         glVertex3f( w,h, d);
    glEnd();
}

inline void drawHouseRoof(float texRepeat = 2.0f) {
    float w = 4.0f, h = 3.0f, d = 5.0f, rh = 2.0f;

    // Triangulo frontal
    glNormal3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
        glTexCoord2f(0,0);    glVertex3f(-w, h,  d);
        glTexCoord2f(1,0);    glVertex3f( w, h,  d);
        glTexCoord2f(0.5f,1); glVertex3f( 0, h+rh, d);
    glEnd();

    // Triangulo posterior
    glNormal3f(0, 0, -1);
    glBegin(GL_TRIANGLES);
        glTexCoord2f(0,0);    glVertex3f(-w, h, -d);
        glTexCoord2f(1,0);    glVertex3f( w, h, -d);
        glTexCoord2f(0.5f,1); glVertex3f( 0, h+rh,-d);
    glEnd();

    // Plano izquierdo
    float nx = -rh, ny = w;
    float len = std::sqrt(nx*nx + ny*ny);
    glNormal3f(nx/len, ny/len, 0);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0);         glVertex3f(-w, h,   -d);
        glTexCoord2f(texRepeat,0); glVertex3f(-w, h,    d);
        glTexCoord2f(texRepeat,1); glVertex3f( 0, h+rh, d);
        glTexCoord2f(0,1);         glVertex3f( 0, h+rh,-d);
    glEnd();

    // Plano derecho
    glNormal3f(-nx/len, ny/len, 0);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0);         glVertex3f( w, h,    d);
        glTexCoord2f(texRepeat,0); glVertex3f( w, h,   -d);
        glTexCoord2f(texRepeat,1); glVertex3f( 0, h+rh,-d);
        glTexCoord2f(0,1);         glVertex3f( 0, h+rh, d);
    glEnd();
}

inline void drawTree(float trunkR, float trunkH, float foliageR,
                     const std::function<void()>& applyTrunk,
                     const std::function<void()>& applyFoliage) {
    glPushMatrix();
        applyTrunk();
        drawCylinder(trunkR, trunkH, 16);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, trunkH + foliageR * 0.6f, 0);
        applyFoliage();
        drawSphere(foliageR, 16, 16);
    glPopMatrix();
}
