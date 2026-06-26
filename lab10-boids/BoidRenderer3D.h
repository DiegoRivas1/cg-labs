#pragma once
#include <glad/glad.h>
#include <GL/glu.h>
#include <cmath>
#include "BoidSystem.h"

class BoidRenderer3D {
public:
    float rotX = 25.0f, rotY = 30.0f, dist = 20.0f;
    float lastX = 0, lastY = 0;
    bool  dragging = false;

    void applyCamera() const{
        glLoadIdentity();
        constexpr float rad = 3.14159265f / 180.0f;
        const float ex = dist * std::sin(rotY*rad) * std::cos(rotX*rad);
        const float ey = dist * std::sin(rotX*rad);
        const float ez = dist * std::cos(rotY*rad) * std::cos(rotX*rad);
        gluLookAt(ex,ey,ez, 0,0,0, 0,1,0);
    }

    void onMouseButton(int button, int action, double x, double y) {
        if (button == 1) { // right
            dragging = (action == 1);
            lastX=static_cast<float>(x); lastY=static_cast<float>(y);
        }
    }

    void onMouseMove(double x, double y) {
        if (!dragging) { lastX=static_cast<float>(x); lastY=static_cast<float>(y); return; }
        rotY += (static_cast<float>(x) - lastX) * 0.4f;
        rotX += (static_cast<float>(y) - lastY) * 0.4f;
        rotX = std::max(-89.0f, std::min(89.0f, rotX));
        lastX=static_cast<float>(x); lastY=static_cast<float>(y);
    }

    void onScroll(double dy) {
        dist -= static_cast<float>(dy) * 0.8f;
        dist  = std::max(5.0f, std::min(60.0f, dist));
    }

    static void draw(const BoidSystem3D& sys) {
        for (const auto& b : sys.boids)
            drawBoid(b);
        drawBounds(sys.params.toroidal ? 8.0f : 8.0f);
    }

private:
    static void drawBoid(const Boid3D& b) {
        const Vec3 dir = b.vel.normalized();
        Vec3 up  = {0, 1, 0};

        // Si la direccion es casi paralela a up, usamos otro eje
        if (std::abs(dir.dot(up)) > 0.99f) up = {1, 0, 0};

        const Vec3 right = dir.cross(up).normalized();
        //Vec3 newUp = right.cross(dir).normalized();
        auto [x, y, z] = right.cross(dir).normalized();

        // Matriz de rotacion column-major para OpenGL
        const float m[16] = {
            right.x,  right.y,  right.z,  0,
            x,  y,  z,  0,
            -dir.x,   -dir.y,   -dir.z,   0,
            0,        0,        0,        1
        };

        glPushMatrix();
        glTranslatef(b.pos.x, b.pos.y, b.pos.z);
        glMultMatrixf(m);

        constexpr float sz = 0.25f;

        // Cono (punta hacia adelante = -Z local tras la rotacion)
        glColor3f(b.r, b.g, b.b);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0, 0, -sz*2);   // punta
            constexpr int slices = 8;
            for (int i = 0; i <= slices; ++i) {
                const float a = i * 2.0f * 3.14159265f / slices;
                glVertex3f(std::cos(a)*sz*0.5f, std::sin(a)*sz*0.5f, sz*0.5f);
            }
        glEnd();

        // Base del cono
        glColor3f(b.r*0.7f, b.g*0.7f, b.b*0.7f);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0, 0, sz*0.5f);
            for (int i = slices; i >= 0; --i) {
                const float a = i * 2.0f * 3.14159265f / slices;
                glVertex3f(std::cos(a)*sz*0.5f, std::sin(a)*sz*0.5f, sz*0.5f);
            }
        glEnd();

        glPopMatrix();
    }

    static void drawBounds(float b) {
        //glDisable(GL_LIGHTING);
        glColor4f(1,1,1,0.15f);
        glLineWidth(1.0f);
        glBegin(GL_LINE_LOOP);
            glVertex3f(-b,-b,-b); glVertex3f( b,-b,-b);
            glVertex3f( b, b,-b); glVertex3f(-b, b,-b);
        glEnd();
        glBegin(GL_LINE_LOOP);
            glVertex3f(-b,-b, b); glVertex3f( b,-b, b);
            glVertex3f( b, b, b); glVertex3f(-b, b, b);
        glEnd();
        glBegin(GL_LINES);
            glVertex3f(-b,-b,-b); glVertex3f(-b,-b, b);
            glVertex3f( b,-b,-b); glVertex3f( b,-b, b);
            glVertex3f( b, b,-b); glVertex3f( b, b, b);
            glVertex3f(-b, b,-b); glVertex3f(-b, b, b);
        glEnd();
        //glEnable(GL_LIGHTING);
    }

    int slices = 8;
};
