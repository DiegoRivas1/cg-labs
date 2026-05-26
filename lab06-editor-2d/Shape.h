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
        [[nodiscard]] ShapeType type() const override { return ShapeType::Point; }
        [[nodiscard]] std::string name() const override { return "Point"; }

        void draw() const override {
            pushTransform();
            if (selected) { glColor4f(1,1,0,1); glPointSize(10.0f); }
            else          { applyColor(stroke);  glPointSize(5.0f);  }
            glBegin(GL_POINTS);
            //for (auto& v : vertices) glVertex2f(v.x, v.y);
                for (const auto&[x, y] : vertices) glVertex2f(x, y);//
            glEnd();
            glPointSize(1.0f);
            popTransform();
        }
        /*\
        Tenemos un punto dibujado en: (50, 50)
        Hacemos clic en: (55, 54)
        dx = 50 - 55 = -5
        dy = 50 - 54 = -4

        dx*dx + dy*dy = (-5)*(-5) + (-4)*(-4) = 25 + 16 = 41

        ¿41 < 100.0? -> SÍ

        Resultado: El clic SELECCIONA el punto (está dentro del rango)
         */
        [[nodiscard]] bool contains(float x, float y) const override {
            return std::ranges::any_of(vertices, [x, y](const Vec2& v) {
                const float dx = v.x - x;
                const float dy = v.y - y;
                return dx*dx + dy*dy < 100.0f;
            });
        }

};

// Línea
class LineShape : public Shape {
    public:
        [[nodiscard]] ShapeType type() const override { return ShapeType::Line; }
        [[nodiscard]] std::string name() const override { return "Line"; }

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

        [[nodiscard]] bool contains(float x, float y) const override {
            if (vertices.size() < 2) return false;
            const float ax = vertices[0].x;
            const float ay = vertices[0].y;
            const float bx = vertices[1].x;
            const float by = vertices[1].y;
            const float dx = bx-ax; // B.x - A.x
            const float dy = by-ay; // B.y - A.y
            const float len2 = dx*dx + dy*dy;// |B - A| = (bx - ax, by - ay) = dx^2 + dy^2
            //len2 longitud al cuadrado de linea

            if (len2 == 0) return false;
            /*proyeccion de un punto sobrelinea
            t = [(P - A) · (B - A)] / |B - A|²
            Donde:
            - P = punto del clic (x, y)
            - A = primer vértice de la línea
            - B = segundo vértice de la línea
            - · = producto punto (dot product)
            */
            float t = ((x-ax)*dx + (y-ay)*dy) / len2;//
            //t = 0 -> El punto está cerca de A (vectice[0])
            //t = 1 -> El punto está cerca de B (vertice[1])
            //t = 0.5 -> El punto está en el medio
            t = std::max(0.0f, std::min(1.0f, t));
            const float px = ax + t*dx - x;
            const float py = ay + t*dy - y;
            return px*px + py*py < 100.0f;
        }
};

// Polilínea
class PolylineShape : public Shape {
public:
    [[nodiscard]] ShapeType type() const override { return ShapeType::Polyline; }
    [[nodiscard]] std::string name() const override { return "Polyline"; }

    void draw() const override {
        if (vertices.size() < 2) return;
        pushTransform();
        glLineWidth(selected ? 3.0f : 1.5f);
        applyColor(selected ? Color{1,1,0,1} : stroke);
        glBegin(GL_LINE_STRIP);
        for (const auto&[x, y] : vertices) glVertex2f(x, y);
        glEnd();
        glLineWidth(1.0f);
        popTransform();
    }

    [[nodiscard]] bool contains(float x, float y) const override {
        //la lisma logica que  LineShape, solo que ahor a acada segmento que es Lineshape
        for (size_t i = 0; i+1 < vertices.size(); ++i) {
            const float ax = vertices[i].x;
            const float ay = vertices[i].y;
            const float bx = vertices[i+1].x;
            const float by = vertices[i+1].y;
            const float dx = bx-ax;
            const float dy = by-ay;
            const float len2 = dx*dx + dy*dy;

            if (len2 == 0) continue;
            float t = ((x-ax)*dx + (y-ay)*dy) / len2;
            t = std::max(0.0f, std::min(1.0f, t));
            const float px = ax + t*dx - x;
            const float py = ay + t*dy - y;
            if (px*px + py*py < 100.0f) return true;
        }
        return false;
    }
};

// Polígono
class PolygonShape : public Shape {
public:
    [[nodiscard]] ShapeType type() const override { return ShapeType::Polygon; }
    [[nodiscard]] std::string name() const override { return "Polygon"; }

    void draw() const override {
        if (vertices.size() < 3) return;
        pushTransform();

        if (filled) {
            applyColor(selected ? Color{1,1,0,0.4f} : fill);
            glBegin(GL_POLYGON);
            for (auto& v : vertices) glVertex2f(v.x, v.y);
            glEnd();
        }

        //Esta seleccionado
        glLineWidth(selected ? 3.0f : 1.5f);
        applyColor(selected ? Color{1,1,0,1} : stroke);

        glBegin(GL_LINE_LOOP);
            for (auto& v : vertices) glVertex2f(v.x, v.y);
        glEnd();
        glLineWidth(1.0f);
        popTransform();
    }

    /*
    Vértice 1
            /\
           /  \
          /    \
    ------●------ <- Lanza rayo horizontal desde el clic
         /      \
        /        \
       /          \
    Vértice 2   Vértice 3

    - Cuenta las intersecciones del rayo con los bordes
    - Impar = DENTRO
    - Par = FUERA
     */
    [[nodiscard]] bool contains(float x, float y) const override {
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
