#include <iostream>
#include <cstdlib>
#include <ctime>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

#include "BoidSystem.h"
#include "BoidRenderer2D.h"
#include "BoidRenderer3D.h"

constexpr int  WIDTH  = 1280;
constexpr int  HEIGHT = 720;
constexpr int  PANEL  = 240;
constexpr char TITLE[]= "Lab 10 - Boids";
constexpr float BOUND2D_W = 8.0f;
constexpr float BOUND2D_H = 5.5f;
constexpr float BOUND3D   = 8.0f;

static bool mode3D = false;

static BoidSystem2D  sys2d;
static BoidSystem3D  sys3d;
static BoidRenderer2D ren2d;
static BoidRenderer3D ren3d;

static float prevTime = 0;

static void keyCallback(GLFWwindow* w, int key, int, int action, int) {
    if (action != GLFW_PRESS) return;
    if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(w, GLFW_TRUE);
    if (key == GLFW_KEY_TAB)    mode3D = !mode3D;
}

static void mouseButtonCallback(GLFWwindow*, int button, int action, int) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    double x, y;
    glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
    ren3d.onMouseButton(button, action, x, y);
}

static void cursorPosCallback(GLFWwindow*, double x, double y) {
    ren3d.onMouseMove(x, y);
}

static void scrollCallback(GLFWwindow*, double, double dy) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    ren3d.onScroll(dy);
}

static void drawUI() {
    ImGui::SetNextWindowPos({0,0});
    ImGui::SetNextWindowSize({static_cast<float>(PANEL),static_cast<float>(HEIGHT)});
    ImGui::Begin("Boids", nullptr,
        ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|
        ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::Text("Modo [TAB]:");
    if (ImGui::RadioButton("2D", !mode3D)) mode3D = false;
    ImGui::SameLine();
    if (ImGui::RadioButton("3D",  mode3D)) mode3D = true;

    ImGui::Separator();

    BoidParams& p = mode3D ? sys3d.params : sys2d.params;

    ImGui::Text("Poblacion");
    if (ImGui::SliderInt("Boids", &p.count, 10, 300)) {
        if (mode3D) sys3d.resize(BOUND3D);
        else        sys2d.resize(BOUND2D_W, BOUND2D_H);
    }

    ImGui::Separator();
    ImGui::Text("Vecindad");
    ImGui::SliderFloat("Radio vecinos", &p.neighborDist, 0.5f, 8.0f);
    ImGui::SliderFloat("Radio sep.",    &p.sepDist,      0.1f, 3.0f);

    ImGui::Separator();
    ImGui::Text("Pesos");
    ImGui::SliderFloat("Separacion",  &p.sepWeight, 0.0f, 5.0f);
    ImGui::SliderFloat("Alineacion",  &p.aliWeight, 0.0f, 5.0f);
    ImGui::SliderFloat("Cohesion",    &p.cohWeight, 0.0f, 5.0f);

    ImGui::Separator();
    ImGui::Text("Velocidad");
    ImGui::SliderFloat("Min", &p.minSpeed, 0.1f, 5.0f);
    ImGui::SliderFloat("Max", &p.maxSpeed, 1.0f, 10.0f);

    ImGui::Separator();
    ImGui::Text("Bordes");
    if (ImGui::RadioButton("Toroidal", p.toroidal))   p.toroidal = true;
    if (ImGui::RadioButton("Rebote",  !p.toroidal))   p.toroidal = false;

    ImGui::Separator();
    ImGui::Text("Boids activos: %d", static_cast<int>(mode3D ? sys3d.boids.size() : sys2d.boids.size()));

    if (mode3D) {
        ImGui::Separator();
        ImGui::Text("Camara 3D");
        ImGui::Text("Bot. der + arrastrar: rotar");
        ImGui::Text("Rueda: zoom");
    }

    ImGui::End();
}

static void setup2DProjection() {
    constexpr int drawW = WIDTH - PANEL;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    constexpr float aspect = static_cast<float>(drawW) / HEIGHT;
    glOrtho(-BOUND2D_W * aspect / (BOUND2D_W / BOUND2D_W),
             BOUND2D_W * aspect / (BOUND2D_W / BOUND2D_W),
            -BOUND2D_H, BOUND2D_H, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void setup3DProjection() {
    constexpr int drawW = WIDTH - PANEL;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, static_cast<double>(drawW)/HEIGHT, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sys2d.init(BOUND2D_W, BOUND2D_H);
    sys3d.init(BOUND3D);

    prevTime = static_cast<float>(glfwGetTime());

    std::cout << "Lab 10 - Boids\n"
              << "  [TAB] alternar 2D/3D\n"
              << "  Panel izquierdo: ajustar parametros en tiempo real\n";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        const float now = static_cast<float>(glfwGetTime());
        float dt  = now - prevTime;
        prevTime  = now;
        if (dt > 0.05f) dt = 0.05f;

        // Actualizar simulacion
        if (mode3D) sys3d.update(dt, BOUND3D);
        else        sys2d.update(dt, BOUND2D_W, BOUND2D_H);

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        drawUI();

        constexpr int drawW = WIDTH - PANEL;
        glViewport(PANEL, 0, drawW, HEIGHT);
        //glClearColor(0.05f, 0.05f, 0.10f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (mode3D) {
            glEnable(GL_DEPTH_TEST);
            setup3DProjection();
            ren3d.applyCamera();
            //ren3d.draw(sys3d);
        } else {
            glDisable(GL_DEPTH_TEST);
            setup2DProjection();
            //ren2d.draw(sys2d);
            glLoadIdentity();
        }

        glClearColor(0.05f, 0.05f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (mode3D) ren3d.draw(sys3d);
        else        ren2d.draw(sys2d);

        glViewport(0, 0, WIDTH, HEIGHT);
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
