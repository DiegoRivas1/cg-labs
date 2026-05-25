#pragma once
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
        for (auto it = objects.rbegin(); it != objects.rend(); ++it) {
            if ((*it)->contains(x, y)) {
                (*it)->selected = true;
                selected = it->get();
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
        objects.erase(
            std::remove_if(objects.begin(), objects.end(),
                [](const std::unique_ptr<Shape>& s){ return s->selected; }),
            objects.end()
        );
        selected = nullptr;
    }

    void clear() {
        objects.clear();
        selected = nullptr;
    }

    int count() const { return (int)objects.size(); }
};
