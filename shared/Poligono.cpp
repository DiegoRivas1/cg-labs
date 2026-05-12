//#include "../lab03-transformaciones-escena/Poligono.h"
#include "Poligono.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

Poligono::Poligono() : numeroDeVertices(0){}

Poligono::~Poligono() = default;

Poligono::Poligono(int numeroVertices) : numeroDeVertices(numeroVertices){}

void Poligono::dibujar() const {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numeroDeVertices; ++i) {
        float angle = 2.0f * static_cast<float>(M_PI) * static_cast<float>(i) / static_cast<float>(numeroDeVertices);
        float x = 0.5f * std::cos(angle); // Radio fijo de 0.5
        float y = 0.5f * std::sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}
