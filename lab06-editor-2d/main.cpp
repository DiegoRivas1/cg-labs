#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

#include "Editor.h"

constexpr int  WIDTH  = 1280;
constexpr int  HEIGHT = 720;
constexpr char TITLE[]= "Lab 06 - Editor Grafico 2D";

static Editor editor;

// Convierte coordenadas de pantalla a coordenadas del mundo OpenGL
//Offset del punto el problema es la conversión screenToWorld.
//El viewport de dibujo empieza en x=220 pero la conversión usa el ancho total de la ventana.
static void screenToWorld(GLFWwindow* w, const double sx, const double sy, float& wx, float& wy) {
    int width, height;
    glfwGetWindowSize(w, &width, &height);
    const int drawWidth = width - 220;  // descontar el panel
    const float nx = static_cast<float>(sx - 220) / drawWidth;  // normalizamos solo en el area de dibujo
    const float aspect = static_cast<float>(drawWidth) / height;
    wx = (nx * 2.0f - 1.0f) * aspect;
    wy = -(static_cast<float>(sy) / height * 2.0f - 1.0f);
}

// Callbacks
static void mouseButtonCallback(GLFWwindow* w, const int button, const int action, int) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    if (action != GLFW_PRESS) return;

    double sx, sy;
    glfwGetCursorPos(w, &sx, &sy);
    float wx, wy;
    screenToWorld(w, sx, sy, wx, wy);

    if (button == GLFW_MOUSE_BUTTON_LEFT)  editor.onLeftClick(wx, wy);
    if (button == GLFW_MOUSE_BUTTON_RIGHT) editor.onRightClick(wx, wy);
}

static void cursorPosCallback(GLFWwindow* w, const double sx, const double sy) {
    float wx, wy;
    screenToWorld(w, sx, sy, wx, wy);
    editor.mouseX = wx;
    editor.mouseY = wy;
}

static void keyCallback(GLFWwindow* w, const int key, int, const int action, int) {
    if (ImGui::GetIO().WantCaptureKeyboard) return;
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

    switch (key) {
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(w, GLFW_TRUE); break;
        case GLFW_KEY_1: editor.setMode(EditorMode::Point);    break;
        case GLFW_KEY_2: editor.setMode(EditorMode::Line);     break;
        case GLFW_KEY_3: editor.setMode(EditorMode::Polyline); break;
        case GLFW_KEY_4: editor.setMode(EditorMode::Polygon);  break;
        case GLFW_KEY_5: editor.setMode(EditorMode::Select);   break;
        case GLFW_KEY_ENTER: editor.finishPolyline();          break;
        case GLFW_KEY_DELETE: editor.deleteSelected();         break;
        case GLFW_KEY_F: editor.toggleFill();                  break;

        // Transformaciones sobre objeto seleccionado
        case GLFW_KEY_LEFT:  editor.translateSelected(-0.05f, 0.0f);  break;
        case GLFW_KEY_RIGHT: editor.translateSelected( 0.05f, 0.0f);  break;
        case GLFW_KEY_UP:    editor.translateSelected( 0.0f,  0.05f); break;
        case GLFW_KEY_DOWN:  editor.translateSelected( 0.0f, -0.05f); break;
        case GLFW_KEY_R:     editor.rotateSelected(5.0f);             break;
        case GLFW_KEY_T:     editor.rotateSelected(-5.0f);            break;
        case GLFW_KEY_EQUAL: editor.scaleSelected(1.1f, 1.1f);        break;
        case GLFW_KEY_MINUS: editor.scaleSelected(0.9f, 0.9f);        break;
        default: break;
    }
}

// Panel ImGui
static void drawUI() {
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({220, static_cast<float>(HEIGHT)});
    ImGui::Begin("Editor", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::Text("Modo activo:");
    ImGui::TextColored({1,1,0,1}, "%s", editor.modeName().c_str());
    ImGui::Separator();

    ImGui::Text("Herramientas");
    if (ImGui::Button("Point   [1]",  {200,30})) editor.setMode(EditorMode::Point);
    if (ImGui::Button("Line    [2]",  {200,30})) editor.setMode(EditorMode::Line);
    if (ImGui::Button("Polyline[3]",  {200,30})) editor.setMode(EditorMode::Polyline);
    if (ImGui::Button("Polygon [4]",  {200,30})) editor.setMode(EditorMode::Polygon);
    if (ImGui::Button("Select  [5]",  {200,30})) editor.setMode(EditorMode::Select);
    ImGui::Separator();

    ImGui::Text("Colores");
    ImGui::ColorEdit3("Stroke", &editor.strokeColor.r);
    ImGui::ColorEdit3("Fill",   &editor.fillColor.r);
    ImGui::Checkbox("Relleno [F]", &editor.fillEnabled);
    ImGui::Separator();

    ImGui::Text("Transformaciones");
    ImGui::Text("Flechas: trasladar");
    ImGui::Text("R/T: rotar");
    ImGui::Text("+/-: escalar");
    ImGui::Text("DEL: eliminar");
    ImGui::Separator();

    ImGui::Text("Objetos: %d", editor.scene.count());
    if (editor.scene.selected)
        ImGui::TextColored({0,1,1,1}, "Seleccionado: %s",
            editor.scene.selected->name().c_str());

    ImGui::Separator();
    if (ImGui::Button("Limpiar escena", {200,30})) editor.scene.clear();

    ImGui::End();
}

int main() {
    if (!glfwInit()) { std::cerr << "Error GLFW\n"; return -1; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (!window) { std::cerr << "Error ventana\n"; glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetKeyCallback(window, keyCallback);

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
    glClearColor(0.08f, 0.08f, 0.12f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    std::cout << "Lab 06 - Editor Grafico 2D\n"
              << "  [1] Punto  [2] Linea  [3] Polilinea  [4] Poligono  [5] Seleccion\n"
              << "  Flechas: trasladar  R/T: rotar  +/-: escalar  DEL: eliminar\n"
              << "  Clic derecho / ESC: cancelar figura\n";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawUI();

        glClear(GL_COLOR_BUFFER_BIT);

        // Viewport solo en el area de dibujo (derecha del panel)
        glViewport(220, 0, WIDTH - 220, HEIGHT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        constexpr float aspect = static_cast<float>(WIDTH - 220) / HEIGHT;
        glOrtho(-aspect, aspect, -1.0, 1.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        editor.scene.drawAll();
        editor.drawCurrent();

        // Restaurar viewport para ImGui
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
