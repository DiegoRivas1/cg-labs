#pragma once
#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include "Object3D.h"

class Scene3D {
public:
    std::vector<Object3D> objects;
    int selectedIndex = -1;

    void add(ObjectType t) {
        objects.emplace_back(t);
    }

    void duplicateSelected() {
        if (selectedIndex < 0) return;
        Object3D copy = objects[selectedIndex];
        copy.id       = Object3D::nextId++;
        copy.name     = objectTypeName(copy.type) + "_" + std::to_string(copy.id);
        copy.posX    += 0.5f;
        copy.posZ    += 0.5f;
        copy.selected = false;
        objects.push_back(copy);
    }

    void deleteSelected() {
        if (selectedIndex < 0) return;
        objects.erase(objects.begin() + selectedIndex);
        selectedIndex = -1;
    }

    void select(int index) {
        clearSelection();
        if (index >= 0 && index < static_cast<int>(objects.size())) {
            objects[index].selected = true;
            selectedIndex = index;
        }
    }

    void clearSelection() {
        for (auto& o : objects) o.selected = false;
        selectedIndex = -1;
    }

    Object3D* selected() {
        if (selectedIndex < 0) return nullptr;
        return &objects[selectedIndex];
    }

    void drawAll(OBJLoader& teapot) const {
        for (auto& o : objects) o.draw(teapot);
    }

    void save(const std::string& path) const {
        std::ofstream f(path);
        f << objects.size() << "\n";
        for (auto& o : objects) o.serialize(f);
    }

    void load(const std::string& path) {
        std::ifstream f(path);
        if (!f.is_open()) return;
        objects.clear();
        selectedIndex = -1;
        int count; f >> count;
        for (int i = 0; i < count; ++i)
            objects.push_back(Object3D::deserialize(f));
    }

    [[nodiscard]] int count() const { return static_cast<int>(objects.size()); }
};
