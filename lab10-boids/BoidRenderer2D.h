#pragma once
#include <glad/glad.h>
#include <cmath>
#include "BoidSystem.h"

class BoidRenderer2D {
public:
    static void draw(const BoidSystem2D& sys) {
        for (const auto& b : sys.boids)
            drawBoid(b);
    }

private:
    static void drawBoid(const Boid2D& b) {
        const float angle = std::atan2(b.vel.y, b.vel.x);
        constexpr float size  = 0.15f;

        glPushMatrix();
        glTranslatef(b.pos.x, b.pos.y, 0);
        glRotatef(angle * 180.0f / 3.14159265f - 90.0f, 0, 0, 1);

        glColor3f(b.r, b.g, b.b);
        glBegin(GL_TRIANGLES);
            glVertex2f( 0.0f,       size);       // punta (frente)
            glVertex2f(-size*0.5f, -size*0.5f);  // base izquierda
            glVertex2f( size*0.5f, -size*0.5f);  // base derecha
        glEnd();

        // Contorno
        glColor3f(b.r*0.6f, b.g*0.6f, b.b*0.6f);
        glBegin(GL_LINE_LOOP);
            glVertex2f( 0.0f,       size);
            glVertex2f(-size*0.5f, -size*0.5f);
            glVertex2f( size*0.5f, -size*0.5f);
        glEnd();

        glPopMatrix();
    }
};
