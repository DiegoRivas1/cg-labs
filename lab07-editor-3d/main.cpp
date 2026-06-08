#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

#include "Scene3D.h"
#include "Camera3D.h"
#include "lighting.h"

constexpr int  WIDTH  = 1280;
constexpr int  HEIGHT = 720;
constexpr int  PANEL  = 240;
constexpr char TITLE[]= "Lab 07 - Editor 3D";

static Scene3D  scene;
static Camera3D camera;
static OBJLoader teapot;
static bool wireframeAll = false;
static char saveFile[128] = "scene.txt";

inline void drawGizmo(const float size = 1.5f) {
    glDisable(GL_LIGHTING);
    glLineWidth(2.5f);
    glBegin(GL_LINES);
        glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(size,0,0);
        glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,size,0);
        glColor3f(0.2f,0.4f,1); glVertex3f(0,0,0); glVertex3f(0,0,size);
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

static void keyCallback(GLFWwindow* w, const int key, int, const int action, int) {
    if (ImGui::GetIO().WantCaptureKeyboard) return;
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

    Object3D* sel = scene.selected();

    switch (key) {
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(w, GLFW_TRUE); break;
        case GLFW_KEY_DELETE: scene.deleteSelected(); break;
        case GLFW_KEY_D:      if (sel) scene.duplicateSelected(); break;
        case GLFW_KEY_TAB:    camera.mode = (camera.mode == CameraMode::Free)
                                  ? CameraMode::Trackball : CameraMode::Free; break;

        // Traslación
        case GLFW_KEY_LEFT:  if (sel) sel->posX -= 0.2f; break;
        case GLFW_KEY_RIGHT: if (sel) sel->posX += 0.2f; break;
        case GLFW_KEY_UP:    if (sel) sel->posY += 0.2f; break;
        case GLFW_KEY_DOWN:  if (sel) sel->posY -= 0.2f; break;
        case GLFW_KEY_PAGE_UP:   if (sel) sel->posZ -= 0.2f; break;
        case GLFW_KEY_PAGE_DOWN: if (sel) sel->posZ += 0.2f; break;

        // Rotación
        case GLFW_KEY_X: if (sel) sel->rotX += 5.0f; break;
        case GLFW_KEY_Y: if (sel) sel->rotY += 5.0f; break;
        case GLFW_KEY_Z: if (sel) sel->rotZ += 5.0f; break;

        // Escala
        //Usando shift
        //case GLFW_KEY_EQUAL: if (sel) { sel->scaX *= 1.1f; sel->scaY *= 1.1f; sel->scaZ *= 1.1f; } break;
        //case GLFW_KEY_MINUS: if (sel) { sel->scaX *= 0.9f; sel->scaY *= 0.9f; sel->scaZ *= 0.9f; } break;

        //Usando keypad
        case GLFW_KEY_KP_ADD: if (sel) { sel->scaX *= 1.1f; sel->scaY *= 1.1f; sel->scaZ *= 1.1f; } break;
        case GLFW_KEY_KP_SUBTRACT: if (sel) { sel->scaX *= 0.9f; sel->scaY *= 0.9f; sel->scaZ *= 0.9f; } break;

        // Cámara libre
        case GLFW_KEY_A: camera.moveLeft();  break;
        case GLFW_KEY_S: camera.moveDown();  break;
        case GLFW_KEY_W: camera.moveUp();    break;
        case GLFW_KEY_E: camera.zoomIn();    break;
        case GLFW_KEY_Q: camera.zoomOut();   break;
        default: break;
    }
}

static void mouseButtonCallback(GLFWwindow* w, const int button, const int action, int) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    double x, y;
    glfwGetCursorPos(w, &x, &y);
    camera.onMouseButton(button, action, x, y);
}

static void cursorPosCallback(GLFWwindow*, const double x, const double y) {
    camera.onMouseMove(x, y);
}

static void scrollCallback(GLFWwindow*, double, const double dy) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    camera.onScroll(dy);
}

static void drawUI() {
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({static_cast<float>(PANEL), static_cast<float>(HEIGHT)});
    ImGui::Begin("Editor 3D", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

    // Crear objetos
    ImGui::Text("Crear objeto");
    if (ImGui::Button("Cubo",   {108,28})) scene.add(ObjectType::Cube);
    ImGui::SameLine();
    if (ImGui::Button("Esfera", {108,28})) scene.add(ObjectType::Sphere);
    if (ImGui::Button("Toro",   {108,28})) scene.add(ObjectType::Torus);
    ImGui::SameLine();
    if (ImGui::Button("Tetera", {108,28})) scene.add(ObjectType::Teapot);

    ImGui::Separator();

    // Lista de objetos
    ImGui::Text("Objetos (%d)", scene.count());
    for (int i = 0; i < scene.count(); ++i) {
        auto& obj = scene.objects[i];
        bool sel = (scene.selectedIndex == i);
        if (ImGui::Selectable(obj.name.c_str(), sel))
            scene.select(i);
    }

    ImGui::Separator();

    // Propiedades del objeto seleccionado
    Object3D* s = scene.selected();
    if (s) {
        ImGui::Text("Seleccionado: %s", s->name.c_str());
        ImGui::DragFloat3("Pos",  &s->posX, 0.05f);
        ImGui::DragFloat3("Rot",  &s->rotX, 1.0f);
        ImGui::DragFloat3("Sca",  &s->scaX, 0.05f, 0.01f, 20.0f);
        ImGui::ColorEdit3("Color",&s->r);
        ImGui::Checkbox("Wireframe", &s->wireframe);

        if (ImGui::Button("Duplicar [D]",  {220,28})) scene.duplicateSelected();
        if (ImGui::Button("Eliminar [DEL]",{220,28})) scene.deleteSelected();
    } else {
        ImGui::TextDisabled("Ninguno seleccionado");
    }

    ImGui::Separator();

    // Cámara
    ImGui::Text("Camara: %s", camera.mode == CameraMode::Trackball ? "Trackball" : "Libre");
    ImGui::Text("[TAB] cambiar modo");
    ImGui::SliderFloat("FOV",  &camera.fov,   10.0f, 120.0f);
    ImGui::SliderFloat("Near", &camera.near_,  0.01f,  5.0f);
    ImGui::SliderFloat("Far",  &camera.far_,  10.0f, 500.0f);

    ImGui::Separator();

    // Wireframe global
    ImGui::Checkbox("Wireframe global", &wireframeAll);

    ImGui::Separator();

    // Guardar/cargar
    ImGui::Text("Escena");
    ImGui::InputText("Archivo", saveFile, sizeof(saveFile));
    if (ImGui::Button("Guardar", {106,28})) scene.save(saveFile);
    ImGui::SameLine();
    if (ImGui::Button("Cargar",  {106,28})) scene.load(saveFile);

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

    if (!teapot.load("teapot.obj"))
        std::cerr << "Advertencia: teapot.obj no encontrado\n";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.08f, 0.08f, 0.12f, 1.0f);
    setupLighting();

    std::cout << "Lab 07 - Editor 3D\n"
              << "  Flechas: trasladar XY  PgUp/PgDn: Z\n"
              << "  X/Y/Z: rotar  =/- : escalar\n"
              << "  D: duplicar  DEL: eliminar\n"
              << "  TAB: cambiar camara  Boton derecho + arrastrar: rotar\n";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawUI();

        int drawW = WIDTH - PANEL;
        camera.applyProjection(drawW, HEIGHT);

        glViewport(PANEL, 0, drawW, HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera.applyView();
        updateLight();

        if (wireframeAll) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        scene.drawAll(teapot);
        if (wireframeAll) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        drawGizmo(1.5f);

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
