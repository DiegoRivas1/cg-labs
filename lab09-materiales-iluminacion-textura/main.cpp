#include <iostream>
#include <memory>
#include <cmath>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

#include "Lighting.h"
#include "SceneMaterials.h"
#include "SceneProcedural.h"
#include "SceneTextured.h"

constexpr int  WIDTH  = 1280;
constexpr int  HEIGHT = 720;
constexpr int  PANEL  = 220;
constexpr char TITLE[]= "Lab 09 - Materiales, Iluminacion y Texturas";

bool directionalLight = true;

static int   sceneIndex = 0;
static float rotX=20, rotY=30, dist=30;
static float lastX=0, lastY=0;
static bool  dragging=false;

static std::unique_ptr<Scene> scenes[3];
static const char* sceneNames[] = {
    "1 - Materiales",
    "2 - Texturas Procedurales",
    "3 - Texturas Reales"
};

static void setSceneBackground(int index) {
    switch (index) {
        case 0: glClearColor(0.18f, 0.18f, 0.20f, 1.0f); break; // materiales: gris neutro
        case 1: glClearColor(0.12f, 0.35f, 0.12f, 1.0f); break; // procedurales: verde oscuro
        case 2: glClearColor(0.53f, 0.81f, 0.98f, 1.0f); break; // reales: celeste
        default: glClearColor(0.0f, 1.0f, 0.0f, 1.0f); break;
    }
}

static void mouseButtonCallback(GLFWwindow*, int button, int action, int) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        dragging = (action == GLFW_PRESS);
}

static void cursorPosCallback(GLFWwindow*, double x, double y) {
    if (dragging) {
        rotY += ((float)x - lastX) * 0.4f;
        rotX += ((float)y - lastY) * 0.4f;
        rotX = std::max(-89.0f, std::min(89.0f, rotX));
    }
    lastX=(float)x; lastY=(float)y;
}

static void scrollCallback(GLFWwindow*, double, double dy) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    dist -= (float)dy;
    dist  = std::max(5.0f, std::min(80.0f, dist));
}

static void keyCallback(GLFWwindow* w, int key, int, int action, int) {
    if (action != GLFW_PRESS) return;
    if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(w, GLFW_TRUE);
    if (key == GLFW_KEY_1) sceneIndex = 0;
    if (key == GLFW_KEY_2) sceneIndex = 1;
    if (key == GLFW_KEY_3) sceneIndex = 2;
    if (key == GLFW_KEY_L) directionalLight = !directionalLight;
}

static void applyTrackball() {
    glLoadIdentity();
    float rad = 3.14159265f / 180.0f;
    float ex = dist * std::sin(rotY*rad) * std::cos(rotX*rad);
    float ey = dist * std::sin(rotX*rad);
    float ez = dist * std::cos(rotY*rad) * std::cos(rotX*rad);
    gluLookAt(ex, ey, ez,  0, 2, 0,  0, 1, 0);
    if (directionalLight) setLightDirectional();
    else                  setLightPoint();
}

static void drawGizmo() {
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(3,0,0);
        glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,3,0);
        glColor3f(0.3f,0.5f,1); glVertex3f(0,0,0); glVertex3f(0,0,3);
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

static void drawUI() {
    ImGui::SetNextWindowPos({0,0});
    ImGui::SetNextWindowSize({(float)PANEL,(float)HEIGHT});
    ImGui::Begin("Lab 09", nullptr,
        ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|
        ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::Text("Escena activa:");
    for (int i = 0; i < 3; ++i)
        if (ImGui::RadioButton(sceneNames[i], sceneIndex==i))
            sceneIndex = i;

    ImGui::Separator();
    ImGui::Text("Luz [L]:");
    if (ImGui::RadioButton("Direccional", directionalLight))  directionalLight = true;
    if (ImGui::RadioButton("Puntual",    !directionalLight))  directionalLight = false;

    ImGui::Separator();
    ImGui::Text("Camara trackball");
    ImGui::Text("Bot. der + arrastrar: rotar");
    ImGui::Text("Rueda: zoom");
    ImGui::SliderFloat("Dist", &dist, 5.0f, 80.0f);

    ImGui::End();
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    setSceneBackground(sceneIndex);
    setupLighting();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)(WIDTH-PANEL)/HEIGHT, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);

    scenes[0] = std::make_unique<SceneMaterials>();
    scenes[1] = std::make_unique<SceneProcedural>();
    scenes[2] = std::make_unique<SceneTextured>();
    for (auto& s : scenes) s->init();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        drawUI();

        setSceneBackground(sceneIndex);
        glViewport(PANEL, 0, WIDTH-PANEL, HEIGHT);
        applyTrackball();
        scenes[sceneIndex]->render((float)glfwGetTime());
        drawGizmo();

        glViewport(0, 0, WIDTH, HEIGHT);
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    for (auto& s : scenes) s->cleanup();
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
