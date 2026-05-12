#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include "scene_hierarchical.h"
#include "scene_homogeneous.h"

constexpr int  WIDTH  = 1000;
constexpr int  HEIGHT = 900;
constexpr char TITLE[]= "Lab 04 - Transformaciones Jerárquicas y Homogeneas";

// Escena activa
// Parte I ->  1=Tetera sin spin  2=Brazo robótico  3=Figura humana
// Parte II -> 4=T/R/S matrices   5=Composición     6=Lab03Ej1  7=Lab03Ej2  8=Lab03Ej3
static int escena = 1;

OBJLoader teapot;

// Callback teclado
void keyCallback(GLFWwindow* window, int key, int, int action, int) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);

        // Cambiar escena
        if (key == GLFW_KEY_1) escena = 1;
        if (key == GLFW_KEY_2) escena = 2;
        if (key == GLFW_KEY_3) escena = 3;
        if (key == GLFW_KEY_4) escena = 4;
        if (key == GLFW_KEY_5) escena = 5;
        if (key == GLFW_KEY_6) escena = 6;
        if (key == GLFW_KEY_7) escena = 7;
        if (key == GLFW_KEY_8) escena = 8;

        // Controles brazo robótico (escena 2)
        if (key == GLFW_KEY_A) baseAngle  -= 5.0f;
        if (key == GLFW_KEY_D) baseAngle  += 5.0f;
        if (key == GLFW_KEY_W) upperAngle += 5.0f;
        if (key == GLFW_KEY_S) upperAngle -= 5.0f;
        if (key == GLFW_KEY_Q) foreAngle  += 5.0f;
        if (key == GLFW_KEY_E) foreAngle  -= 5.0f;
        if (key == GLFW_KEY_Z) clawAngle  += 3.0f;
        if (key == GLFW_KEY_X) clawAngle  -= 3.0f;
    }
}

// Proyección
void setupProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)WIDTH / HEIGHT, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

// Main
int main() {
    if (!glfwInit()) { std::cerr << "Error GLFW\n"; return -1; }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (!window) { std::cerr << "Error ventana\n"; glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Error GLAD\n"; return -1;
    }

    setupLighting(); //ligting.h
    if (!teapot.load("teapot.obj"))
        std::cerr << "Advertencia: no se pudo cargar teapot.obj\n";

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.10f, 1.0f);
    setupProjection();

    std::cout
              << " Lab 04 - Transformaciones\n"
              << " Parte I Jerarquicas:\n"
              << "  [1] Tetera orbita sin rotacion propia\n"
              << "  [2] Brazo robotico (A/D base, W/S brazo, Q/E antebrazo, Z/X pinza)\n"
              << "  [3] Figura humana caminando\n"
              << " Parte II Homogeneas:\n"
              << "  [4] T / R / S con glMultMatrixf\n"
              << "  [5] Composicion S*T*R manual\n"
              << "  [6] Lab03 Ej1 con matrices\n"
              << "  [7] Lab03 Ej2 con matrices\n"
              << "  [8] Lab03 Ej3 con matrices\n"
              << "  [ESC] Salir\n";

    while (!glfwWindowShouldClose(window)) {
        auto t = static_cast<float>(glfwGetTime());

        switch (escena) {
            case 1: renderP1Ej2(t);       break;
            case 2: renderRoboticArm();   break;
            case 3: renderHumanFigure(t); break;
            case 4: renderP2Ej2(t);       break;
            case 5: renderP2Ej3(t);       break;
            case 6: renderP2Ej4a(t);      break;
            case 7: renderP2Ej4b(t);      break;
            case 8: renderP2Ej4c(t);      break;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}