#pragma once
#include <ranges>
#include <vector>
#include <memory>
#include <algorithm>
#include "Shape.h"

class Scene {
public:
    std::vector<std::unique_ptr<Shape>> objects;
    Shape* selected = nullptr;

    void add(std::unique_ptr<Shape> s) {
        objects.push_back(std::move(s));
    }

    void drawAll() const {
        for (auto& obj : objects) obj->draw();
    }

    // Selecciona el objeto más cercano al punto dado
    Shape* selectAt(float x, float y) {
        clearSelection();
        for (auto & object : std::ranges::reverse_view(objects)) {
            if (object->contains(x, y)) {
                object->selected = true;
                selected = object.get();
                return selected;
            }
        }
        return nullptr;
    }

    void clearSelection() {
        for (auto& obj : objects) obj->selected = false;
        selected = nullptr;
    }

    void deleteSelected() {
        std::erase_if(objects,
                      [](const std::unique_ptr<Shape>& s){ return s->selected; });
        selected = nullptr;
    }

    void clear() {
        objects.clear();
        selected = nullptr;
    }

    [[nodiscard]] int count() const { return static_cast<int>(objects.size()); }
};
