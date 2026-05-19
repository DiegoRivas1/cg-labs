#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include "scenes.h"

constexpr int  WIDTH  = 1280;
constexpr int  HEIGHT = 720;
constexpr char TITLE[]= "Lab 05 - Proyecciones y Camara";

static int escena = 1;

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);

        if (key == GLFW_KEY_1) { escena = 1; setupOrtho(WIDTH, HEIGHT); }
        if (key == GLFW_KEY_2) { escena = 2; setupPerspective(WIDTH, HEIGHT); }
        if (key == GLFW_KEY_3) { escena = 3; setupPerspective(WIDTH, HEIGHT); }
        if (key == GLFW_KEY_4) {
            escena = 4;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45.0, static_cast<double>(WIDTH) / HEIGHT, 0.1, 200.0);
            glMatrixMode(GL_MODELVIEW);
        }
        if (key == GLFW_KEY_5) { escena = 5; setupPerspective(WIDTH, HEIGHT); }

        // Cambiar vista del sistema solar
        if (key == GLFW_KEY_V && escena == 4)
            solarView = static_cast<SolarView>((static_cast<int>(solarView) + 1) % 3);

        // Camara libre
        if (escena == 3) {
            if (key == GLFW_KEY_A) freeCamera.moveLeft();
            if (key == GLFW_KEY_D) freeCamera.moveRight();
            if (key == GLFW_KEY_W) freeCamera.moveUp();
            if (key == GLFW_KEY_S) freeCamera.moveDown();
            if (key == GLFW_KEY_Q) freeCamera.zoomIn();
            if (key == GLFW_KEY_E) freeCamera.zoomOut();
        }
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    trackball.onMouseButton(button, action, x, y);
}

void cursorPosCallback(GLFWwindow*, double x, double y) {
    trackball.onMouseMove(x, y);
}

void scrollCallback(GLFWwindow*, double, double dy) {
    trackball.onScroll(dy);
}

int main() {
    if (!glfwInit()) { std::cerr << "Error GLFW\n"; return -1; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (!window) { std::cerr << "Error ventana\n"; glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Error GLAD\n"; return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.10f, 1.0f);
    setupLighting();
    setupOrtho(WIDTH, HEIGHT);

    std::cout << " Lab 05 - Proyecciones y Camara\n"
              << "  [1] Proyeccion ortografica\n"
              << "  [2] Proyeccion perspectiva\n"
              << "  [3] Camara libre  (A/D W/S Q/E)\n"
              << "  [4] Sistema solar  (V = cambiar vista)\n"
              << "  [5] Trackball + gizmo  (mouse izq + rueda)\n"
              << "  [ESC] Salir\n";

    while (!glfwWindowShouldClose(window)) {
        auto t = static_cast<float>(glfwGetTime());
        switch (escena) {
            case 1: renderOrtho(t);       break;
            case 2: renderPerspective(t); break;
            case 3: renderFreeCamera(t);  break;
            case 4: renderSolarSystem(t); break;
            case 5: renderTrackball(t);   break;
            default: ;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}