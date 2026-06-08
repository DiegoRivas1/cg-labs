#pragma once
#include "Scene.h"
#include "Shape.h"
#include "math_utils.h"

enum class EditorMode {
    Point, Line, Polyline, Polygon, Select
};

class Editor {
public:
    Scene        scene;
    EditorMode   mode       = EditorMode::Point;
    Color        strokeColor = {1.0f, 1.0f, 1.0f, 1.0f};
    Color        fillColor   = {0.4f, 0.4f, 0.8f, 1.0f};
    bool         fillEnabled = false;

    // Figura en construccion
    std::unique_ptr<Shape> current;
    bool                   building = false;

    // Preview del cursor
    float mouseX = 0.0f, mouseY = 0.0f;

    void setMode(EditorMode m) {
        mode = m;
        cancelCurrent();
    }

    void onLeftClick(float x, float y) {
        if (mode == EditorMode::Select) {
            scene.selectAt(x, y);
            return;
        }

        if (!building) {
            ShapeType t = modeToType(mode);
            current = ShapeFactory::create(t);
            current->stroke  = strokeColor;
            current->fill    = fillColor;
            current->filled  = fillEnabled;
            building = true;
        }

        current->vertices.push_back({x, y});

        // Punto y línea se completan solos
        if (mode == EditorMode::Point) commitCurrent();
        if (mode == EditorMode::Line && current->vertices.size() == 2) commitCurrent();
    }

    void onRightClick(float x, float y) {
        if (building && mode == EditorMode::Polygon) {
            if (current->vertices.size() >= 3) commitCurrent();
            else cancelCurrent();
        } else {
            cancelCurrent();
        }
    }

    // Finaliza polilínea con Enter
    void finishPolyline() {
        if (building && mode == EditorMode::Polyline && current->vertices.size() >= 2)
            commitCurrent();
    }

    void cancelCurrent() {
        current.reset();
        building = false;
    }

    // Transformaciones sobre objeto seleccionado
    void translateSelected(float dx, float dy) const {
        if (!scene.selected) return;
        const Mat4 T = mat4Translate(dx, dy, 0.0f);
        scene.selected->applyTransform(T);
    }

    void rotateSelected(float deg) const {
        if (!scene.selected) return;
        const Mat4 R = mat4RotateZ(deg);
        scene.selected->applyTransform(R);
    }

    void scaleSelected(float sx, float sy) const {
        if (!scene.selected) return;
        const Mat4 S = mat4Scale(sx, sy, 1.0f);
        scene.selected->applyTransform(S);
    }

    void deleteSelected() { scene.deleteSelected(); }

    void toggleFill() { fillEnabled = !fillEnabled; }

    // Dibuja la figura en construcción con preview
    void drawCurrent() const {
        if (!building || !current) return;
        current->draw();

        if (current->vertices.empty()) return;

        // Línea de preview desde el último vértice al cursor
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0xAAAA);
        glColor4f(0.8f, 0.8f, 0.8f, 0.6f);
        glBegin(GL_LINES);
            auto& last = current->vertices.back();
            glVertex2f(last.x, last.y);
            glVertex2f(mouseX, mouseY);
        glEnd();
        glDisable(GL_LINE_STIPPLE);
    }

    [[nodiscard]] std::string modeName() const {
        switch (mode) {
            case EditorMode::Point:    return "Point";
            case EditorMode::Line:     return "Line";
            case EditorMode::Polyline: return "Polyline";
            case EditorMode::Polygon:  return "Polygon";
            case EditorMode::Select:   return "Select";
        }
        return "";
    }

private:
    [[nodiscard]] ShapeType modeToType(EditorMode m) const {
        switch (m) {
            case EditorMode::Point:    return ShapeType::Point;
            case EditorMode::Line:     return ShapeType::Line;
            case EditorMode::Polyline: return ShapeType::Polyline;
            case EditorMode::Polygon:  return ShapeType::Polygon;
            default:                   return ShapeType::Point;
        }
    }

    void commitCurrent() {
        scene.add(std::move(current));
        building = false;
    }
};
