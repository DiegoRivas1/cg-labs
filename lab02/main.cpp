#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shapes.h"

// Configuración de ventana
constexpr int   WIDTH    = 800;
constexpr int   HEIGHT   = 800;
constexpr char  TITLE[]  = "Lab 02 Transformaciones Geomeetricas";

//Callback de teclado
void keyCallback(GLFWwindow* window, int key, int , int action, int ) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

//Render aquí llamamops los ejercicios
void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);   // blanco es elcorlor por efecto

    // Ejercicio 1 Cuadrado
    glColor3f(1.0f, 0.8f, 0.2f);   // amarillo
    drawSquare(0.0f, 0.7f, 0.3f);

    // Ejercicio 2 Círculo
    glColor3f(0.2f, 0.8f, 1.0f);   // celeste
    drawCircle(0.0f, 0.25f, 0.15f, 64);

    // Ejercicio 3 Círculos concéntricos
    glColor3f(1.0f, 0.4f, 0.4f);   // rojo suave
    //  Centro (0,0), radio inicial 0.20, 5 círculos, reducción 20%
    drawCircleSequenceConcentric(0.0f, -0.2f, 0.20f, 5, 0.20f, 64);

    // Ejercicio 3b Todos los círculos tocan el borde izquierdo en x = -0.9
    glColor3f(1.0f, 0.4f, 0.4f);
    drawCircleSequenceConcentricLeft(-0.9f, 0.0f, 0.4f, 5, 0.25f, 64);

    //Ejercicio 3c Tdoaso los circulos tocan el  borde derecho en x = 0.9
    drawCircleSequenceConcentricRight(0.9f, 0.0f, 0.4f, 5, 0.25f, 64);

    //Ejercicio 3d todos los circu;os tocan el  borde superior en y = 0.9
    drawCircleSequenceConcentricTop(0.0f, 0.9f, 0.4f, 5, 0.25f, 64);

    //Ejercicio 3e todos los circulos tocan el borde inferior en y = -0.9
    drawCircleSequenceConcentricBottom(0.0f, -0.9f, 0.4f, 5, 0.25f, 64);

    // Ejercicio 4 Círculos horizontales
    glColor3f(0.4f, 1.0f, 0.5f);   // verde
    //  Empieza en la izquierda, radio 0.12, 5 círculos, reducción 20%
    drawCircleSequenceHorizontal(-0.75f, -0.6f, 0.12f, 5, 0.20f, 64);

    // Ejercicio 5 Espiral de círculos
    glColor3f(0.8f, 0.4f, 1.0f);   // violeta
    //  Centro (0.5, -0.6), radio 0.12, 6 círculos, reducción 20%, ángulo 45°
    drawCircleSequenceSpiral(0.5f, -0.6f, 0.12f, 6, 0.20f, 45.0f, 64);
}

// Main
int main() {
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Error: no se pudo inicializar GLFW\n";
        return -1;
    }

    // Contexto OpenGL legacy 2.1 modo inmediato glBegin/glEnd
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (!window) {
        std::cerr << "Error: no se pudo crear la ventana\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    // Inicializar GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Error: no se pudo inicializar GLAD\n";
        return -1;
    }

    glClearColor(0.08f, 0.08f, 0.12f, 1.0f);   // fondo oscuro

    // Proyección ortográfica [-1,1] x [-1,1] — coordenadas normalizadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}