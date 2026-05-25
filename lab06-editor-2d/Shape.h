#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glad/glad.h>
#include "math_utils.h"

struct Color {
    float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;
};

struct Vec2 {
    float x = 0.0f, y = 0.0f;
};

enum class ShapeType { Point, Line, Polyline, Polygon };

// Base abstracta
class Shape {
public:
    Color        stroke     = {1.0f, 1.0f, 1.0f, 1.0f};
    Color        fill       = {0.4f, 0.4f, 0.8f, 1.0f};
    bool         filled     = false;
    bool         selected   = false;
    Mat4         transform;
    std::vector<Vec2> vertices;

    Shape() { transform = mat4Identity(); }
    virtual ~Shape() = default;

    [[nodiscard]] virtual ShapeType type() const = 0;
    [[nodiscard]] virtual std::string name() const = 0;
    virtual void draw() const = 0;
    [[nodiscard]] virtual bool contains(float x, float y) const = 0;

    void applyTransform(const Mat4& m) { transform = mat4Mul(m, transform); }

protected:
    static void applyColor(const Color& c) {
        glColor4f(c.r, c.g, c.b, c.a);
    }

    void pushTransform() const {
        glPushMatrix();
        glMultMatrixf(transform.m);
    }

    static void popTransform() { glPopMatrix(); }

    void drawSelectionHighlight() const {
        if (!selected) return;
        glColor4f(1.0f, 1.0f, 0.0f, 0.8f);
        glLineWidth(2.5f);
        glPointSize(8.0f);
    }
};

// Punto
class PointShape : public Shape {
public:
    ShapeType type() const override { return ShapeType::Point; }
    std::string name() const override { return "Point"; }

    void draw() const override {
        pushTransform();
        if (selected) { glColor4f(1,1,0,1); glPointSize(10.0f); }
        else          { applyColor(stroke);  glPointSize(5.0f);  }
        glBegin(GL_POINTS);
        for (auto& v : vertices) glVertex2f(v.x, v.y);
        glEnd();
        glPointSize(1.0f);
        popTransform();
    }

    bool contains(float x, float y) const override {
        for (auto& v : vertices) {
            float dx = v.x - x, dy = v.y - y;
            if (dx*dx + dy*dy < 100.0f) return true;
        }
        return false;
    }
};

// Línea
class LineShape : public Shape {
public:
    ShapeType type() const override { return ShapeType::Line; }
    std::string name() const override { return "Line"; }

    void draw() const override {
        if (vertices.size() < 2) return;
        pushTransform();
        glLineWidth(selected ? 3.0f : 1.5f);
        applyColor(selected ? Color{1,1,0,1} : stroke);
        glBegin(GL_LINES);
        glVertex2f(vertices[0].x, vertices[0].y);
        glVertex2f(vertices[1].x, vertices[1].y);
        glEnd();
        glLineWidth(1.0f);
        popTransform();
    }

    bool contains(float x, float y) const override {
        if (vertices.size() < 2) return false;
        float ax = vertices[0].x, ay = vertices[0].y;
        float bx = vertices[1].x, by = vertices[1].y;
        float dx = bx-ax, dy = by-ay;
        float len2 = dx*dx + dy*dy;
        if (len2 == 0) return false;
        float t = ((x-ax)*dx + (y-ay)*dy) / len2;
        t = std::max(0.0f, std::min(1.0f, t));
        float px = ax + t*dx - x;
        float py = ay + t*dy - y;
        return px*px + py*py < 100.0f;
    }
};

// Polilínea
class PolylineShape : public Shape {
public:
    ShapeType type() const override { return ShapeType::Polyline; }
    std::string name() const override { return "Polyline"; }

    void draw() const override {
        if (vertices.size() < 2) return;
        pushTransform();
        glLineWidth(selected ? 3.0f : 1.5f);
        applyColor(selected ? Color{1,1,0,1} : stroke);
        glBegin(GL_LINE_STRIP);
        for (auto& v : vertices) glVertex2f(v.x, v.y);
        glEnd();
        glLineWidth(1.0f);
        popTransform();
    }

    bool contains(float x, float y) const override {
        for (size_t i = 0; i+1 < vertices.size(); ++i) {
            float ax = vertices[i].x,   ay = vertices[i].y;
            float bx = vertices[i+1].x, by = vertices[i+1].y;
            float dx = bx-ax, dy = by-ay;
            float len2 = dx*dx + dy*dy;
            if (len2 == 0) continue;
            float t = ((x-ax)*dx + (y-ay)*dy) / len2;
            t = std::max(0.0f, std::min(1.0f, t));
            float px = ax + t*dx - x;
            float py = ay + t*dy - y;
            if (px*px + py*py < 100.0f) return true;
        }
        return false;
    }
};

// Polígono
class PolygonShape : public Shape {
public:
    ShapeType type() const override { return ShapeType::Polygon; }
    std::string name() const override { return "Polygon"; }

    void draw() const override {
        if (vertices.size() < 3) return;
        pushTransform();

        if (filled) {
            applyColor(selected ? Color{1,1,0,0.4f} : fill);
            glBegin(GL_POLYGON);
            for (auto& v : vertices) glVertex2f(v.x, v.y);
            glEnd();
        }

        glLineWidth(selected ? 3.0f : 1.5f);
        applyColor(selected ? Color{1,1,0,1} : stroke);
        glBegin(GL_LINE_LOOP);
        for (auto& v : vertices) glVertex2f(v.x, v.y);
        glEnd();
        glLineWidth(1.0f);
        popTransform();
    }

    bool contains(float x, float y) const override {
        // Ray casting
        bool inside = false;
        size_t n = vertices.size();
        for (size_t i = 0, j = n-1; i < n; j = i++) {
            float xi = vertices[i].x, yi = vertices[i].y;
            float xj = vertices[j].x, yj = vertices[j].y;
            if (((yi > y) != (yj > y)) &&
                (x < (xj-xi)*(y-yi)/(yj-yi) + xi))
                inside = !inside;
        }
        return inside;
    }
};

// Factory
class ShapeFactory {
public:
    static std::unique_ptr<Shape> create(ShapeType t) {
        switch (t) {
            case ShapeType::Point:    return std::make_unique<PointShape>();
            case ShapeType::Line:     return std::make_unique<LineShape>();
            case ShapeType::Polyline: return std::make_unique<PolylineShape>();
            case ShapeType::Polygon:  return std::make_unique<PolygonShape>();
        }
        return nullptr;
    }
};
