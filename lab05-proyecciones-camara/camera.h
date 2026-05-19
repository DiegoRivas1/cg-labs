#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <cmath>

// Cámara libre (ejercicio 3)
struct FreeCamera {
    float eyeX = 0.0f, eyeY = 2.0f, eyeZ = 10.0f;
    float targetX = 0.0f, targetY = 0.0f, targetZ = 0.0f;

    void apply() const {
        gluLookAt(eyeX, eyeY, eyeZ,
                  targetX, targetY, targetZ,
                  0.0f, 1.0f, 0.0f);
    }

    void moveLeft()    { eyeX -= 0.3f; targetX -= 0.3f; }
    void moveRight()   { eyeX += 0.3f; targetX += 0.3f; }
    void moveUp()      { eyeY += 0.3f; targetY += 0.3f; }
    void moveDown()    { eyeY -= 0.3f; targetY -= 0.3f; }
    void zoomIn()      { eyeZ -= 0.5f; }
    void zoomOut()     { eyeZ += 0.5f; }
};

// Cámara trackball (ejercicio 5)
struct Trackball {
    float rotX    =  20.0f;   // pitch
    float rotY    =  30.0f;   // yaw
    float dist    =  12.0f;   // distancia al origen
    float lastX   =   0.0f;
    float lastY   =   0.0f;
    bool  dragging = false;

    void apply() const {
        glLoadIdentity();
        // Posición de la cámara calculada manualmente desde ángulos esféricos
        constexpr float rad2deg = 3.14159265f / 180.0f;
        const float ex = dist * std::sin(rotY * rad2deg) * std::cos(rotX * rad2deg);
        const float ey = dist * std::sin(rotX * rad2deg);
        const float ez = dist * std::cos(rotY * rad2deg) * std::cos(rotX * rad2deg);
        gluLookAt(ex, ey, ez,  0, 0, 0,  0, 1, 0);
    }

    void onMouseButton(const int button, const int action, const double x, const double y) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            dragging = (action == GLFW_PRESS);
            lastX = static_cast<float>(x);
            lastY = static_cast<float>(y);
        }
    }

    void onMouseMove(const double x, const double y) {
        if (!dragging) return;
        const float dx = static_cast<float>(x) - lastX;
        const float dy = static_cast<float>(y) - lastY;
        rotY += dx * 0.4f;
        rotX += dy * 0.4f;
        rotX = std::max(-89.0f, std::min(89.0f, rotX));  // limitar pitch
        lastX = static_cast<float>(x);
        lastY = static_cast<float>(y);
    }

    void onScroll(const double dy) {
        dist -= static_cast<float>(dy) * 0.8f;
        dist = std::max(2.0f, std::min(50.0f, dist));
    }
};

// Gizmo de ejes (ejercicio 5)
inline void drawGizmo(float size = 1.5f) {
    glDisable(GL_LIGHTING);
    glLineWidth(2.5f);
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0,0,0); glVertex3f(size,0,0);  // X rojo
        glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0,0,0); glVertex3f(0,size,0);  // Y verde
        glColor3f(0.2f, 0.4f, 1.0f); glVertex3f(0,0,0); glVertex3f(0,0,size);  // Z azul
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}
