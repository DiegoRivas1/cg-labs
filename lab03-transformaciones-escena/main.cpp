#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

//#include "../shared/OBJLoader.h"
//#include "../shared/Primitives.h"
#include "OBJLoader.h"
#include "Primitives.h"
constexpr int   WIDTH  = 1000;
constexpr int   HEIGHT = 900;
constexpr char  TITLE[]= "Lab 03 - Transformaciones en OpenGL";

// Ejercicio activo (cambia con teclas 1-4)
static int ejercicio = 1;

OBJLoader teapot;

// Callbacks
void keyCallback(GLFWwindow* window, int key, int, int action, int) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
        if (key == GLFW_KEY_1) ejercicio = 1;
        if (key == GLFW_KEY_2) ejercicio = 2;
        if (key == GLFW_KEY_3) ejercicio = 3;
        if (key == GLFW_KEY_4) ejercicio = 4;
    }
}

// Proyección compartida
void setupProjection(double fov, double near_, double far_) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (double)WIDTH / HEIGHT, near_, far_);
    glMatrixMode(GL_MODELVIEW);
}

// Ejercicio 1
// Tetera a 4 unidades del origen, girando sobre el eje Z
void renderEj1(float t) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, 12,   0, 0, 0,   0, 1, 0);

    glPushMatrix();
        glRotatef(t * 60.0f, 0, 0, 1);         // gira alrededor del eje Z
        glTranslatef(4.0f, 0.0f, 0.0f);        // a 4 unidades del origen
        glRotatef(t * 90.0f, 0, 0, 1);         // rotación propia
        glScalef(0.4f, 0.4f, 0.4f);
        glColor3f(0.2f, 0.8f, 1.0f);
        teapot.dibujar();
    glPopMatrix();
}

// Ejercicio 2
// Tetera se traslada en X de -8 a 8 (ida y vuelta) y sigue girando
void renderEj2(float t) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 4, 16,   0, 0, 0,   0, 1, 0);

    float posX = std::sin(t) * 8.0f;           // oscila entre -8 y 8

    glPushMatrix();
        glTranslatef(posX, 0.0f, 0.0f);
        glRotatef(t * 90.0f, 0, 0, 1);         // rotación propia sobre Z
        glScalef(0.4f, 0.4f, 0.4f);
        glColor3f(1.0f, 0.6f, 0.2f);
        teapot.dibujar();
    glPopMatrix();
}

// Ejercicio 3
// Tetera (traslada X + rota Z) + Toro (orbita tetera 3x más rápido) + Cubo (rota X)
void renderEj3(float t) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 8, 20,   0, 0, 0,   0, 1, 0);

    float posX    = std::sin(t) * 5.0f;
    float rotZ    = t * 60.0f;
    float rotToro = t * 180.0f;                 // 3x más rápido que la tetera

    // Tetera
    glPushMatrix();
        glTranslatef(posX, 0.0f, 0.0f);
        glRotatef(rotZ, 0, 0, 1);
        glScalef(0.35f, 0.35f, 0.35f);
        glColor3f(0.2f, 0.8f, 1.0f);
        teapot.dibujar();

        // Toro: hereda la posición de la tetera y orbita a 3 unidades
        glPushMatrix();
            glScalef(1.0f / 0.35f, 1.0f / 0.35f, 1.0f / 0.35f); // salir del scale
            glRotatef(rotToro, 0, 0, 1);
            glTranslatef(3.0f, 0.0f, 0.0f);
            glColor3f(1.0f, 0.4f, 0.8f);
            drawTorus(0.2f, 0.6f, 12, 24);
        glPopMatrix();
    glPopMatrix();

    // Cubo independiente, rota en X
    glPushMatrix();
        glTranslatef(-6.0f, 0.0f, 0.0f);
        glRotatef(t * 45.0f, 1, 0, 0);
        glColor3f(0.9f, 0.8f, 0.2f);
        drawCube(2.0f);
    glPopMatrix();
}

// Ejercicio 4
// Sistema solar: Sol, Tierra + Luna, Marte
void renderEj4(float t) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 30, 40,   0, 0, 0,   0, 1, 0);

    const float Vs = t * 20.0f;   // velocidad base Sol
    const float Vt = t * 8.0f;    // velocidad base Tierra

    // Sol
    glPushMatrix();
        glRotatef(Vs, 0, 1, 0);                 // rotación propia
        glColor3f(1.0f, 0.9f, 0.1f);
        drawSphere(4.0f, 8, 8);

        // Tierra
        glPushMatrix();
            glRotatef(Vt, 0, 1, 0);             // órbita alrededor del Sol
            glTranslatef(10.0f, 0.0f, 0.0f);   // distancia 10
            glRotatef(3.0f * Vs, 0, 1, 0);     // rotación propia 3Vs
            glColor3f(0.2f, 0.4f, 1.0f);
            drawSphere(2.0f, 8, 8);

            // Luna
            glPushMatrix();
                glRotatef(2.0f * Vt, 0, 1, 0); // órbita alrededor Tierra 2Vt
                glTranslatef(3.5f, 0.0f, 0.0f);
                glRotatef(1.5f * Vs, 0, 1, 0); // rotación propia 1.5Vs
                glColor3f(0.8f, 0.8f, 0.8f);
                drawSphere(0.6f, 8, 8);
            glPopMatrix();
        glPopMatrix();

        // Marte
        glPushMatrix();
            glRotatef(Vt, 0, 1, 0);             // órbita igual Vt
            glTranslatef(18.0f, 0.0f, 0.0f);   // distancia 18
            glRotatef(Vs, 0, 1, 0);             // rotación propia Vs
            glColor3f(0.9f, 0.3f, 0.1f);
            drawSphere(1.2f, 8, 8);
        glPopMatrix();

    glPopMatrix();
}
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

    if (!teapot.load("teapot.obj"))
        std::cerr << "Advertencia: no se pudo cargar teapot.obj\n";

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.10f, 1.0f);
    setupProjection(45.0, 0.1, 200.0);

    std::cout << "Teclas: [1] Tetera orbita  [2] Tetera ida/vuelta  "
                 "[3] Escena compuesta  [4] Sistema solar  [ESC] Salir\n";

    while (!glfwWindowShouldClose(window)) {
        auto t = static_cast<float>(glfwGetTime());

        switch (ejercicio) {
            case 1: renderEj1(t); break;
            case 2: renderEj2(t); break;
            case 3: renderEj3(t); break;
            case 4: renderEj4(t); break;
            default: renderEj1(t); break;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}