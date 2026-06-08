#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <cmath>
#include <algorithm>

enum class CameraMode { Free, Trackball };

struct Camera3D {
    CameraMode mode = CameraMode::Trackball;

    // Trackball
    float tbRotX =  20.0f;
    float tbRotY =  30.0f;
    float tbDist =  12.0f;
    float tbLastX = 0.0f, tbLastY = 0.0f;
    bool  tbDragging = false;

    // Free
    float fEyeX = 0, fEyeY = 3, fEyeZ = 10;
    float fTarX = 0, fTarY = 0, fTarZ = 0;

    // Perspectiva
    float fov  = 45.0f;
    float near_ = 0.1f;
    float far_  = 200.0f;

    void applyProjection(int w, int h) const {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fov, static_cast<double>(w) / h, near_, far_);
        glMatrixMode(GL_MODELVIEW);
    }

    void applyView() const {
        glLoadIdentity();
        if (mode == CameraMode::Trackball) {
            constexpr float rad = 3.14159265f / 180.0f;
            const float ex = tbDist * std::sin(tbRotY * rad) * std::cos(tbRotX * rad);
            const float ey = tbDist * std::sin(tbRotX * rad);
            const float ez = tbDist * std::cos(tbRotY * rad) * std::cos(tbRotX * rad);
            gluLookAt(ex, ey, ez,  0, 0, 0,  0, 1, 0);
        } else {
            gluLookAt(fEyeX, fEyeY, fEyeZ,
                      fTarX, fTarY, fTarZ,
                      0, 1, 0);
        }
    }

    void onMouseButton(const int button, const int action, const double x, const double y) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            tbDragging = (action == GLFW_PRESS);
            tbLastX = static_cast<float>(x); tbLastY = static_cast<float>(y);
        }
    }

    void onMouseMove(double x, double y) {
        if (!tbDragging) return;
        const float dx = static_cast<float>(x) - tbLastX;
        const float dy = static_cast<float>(y) - tbLastY;
        tbRotY += dx * 0.4f;
        tbRotX += dy * 0.4f;
        tbRotX = std::max(-89.0f, std::min(89.0f, tbRotX));
        tbLastX = static_cast<float>(x); tbLastY = static_cast<float>(y);
    }

    void onScroll(double dy) {
        tbDist -= static_cast<float>(dy) * 0.8f;
        tbDist  = std::max(2.0f, std::min(100.0f, tbDist));
    }

    void moveLeft()  { fEyeX -= 0.3f; fTarX -= 0.3f; }
    void moveRight() { fEyeX += 0.3f; fTarX += 0.3f; }
    void moveUp()    { fEyeY += 0.3f; fTarY += 0.3f; }
    void moveDown()  { fEyeY -= 0.3f; fTarY -= 0.3f; }
    void zoomIn()    { fEyeZ -= 0.5f; }
    void zoomOut()   { fEyeZ += 0.5f; }
};
