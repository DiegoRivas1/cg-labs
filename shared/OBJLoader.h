//
// Created by DIEGO on 04/05/2026.
//

#ifndef CG_LABS_OBJLOADER_H
#define CG_LABS_OBJLOADER_H

#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Vec3 {
    float x, y, z;
};

class OBJLoader {
public:
    std::vector<Vec3> vertices;
    std::vector<int> indices;

    bool load(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return false;

        std::vector<Vec3> temp_vertices;
        std::string line;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string type;
            ss >> type;

            if (type == "v") {
                Vec3 v{};
                ss >> v.x >> v.y >> v.z;
                temp_vertices.push_back(v);
            }
            else if (type == "f") {
                int a, b, c;
                char slash;

                // soporta formato simple: f v1 v2 v3
                ss >> a >> b >> c;

                indices.push_back(a - 1);
                indices.push_back(b - 1);
                indices.push_back(c - 1);
            }
        }

        vertices = temp_vertices;
        return true;
    }

    void dibujar() const{
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < indices.size(); i++) {
            Vec3 v = vertices[indices[i]];
            glVertex3f(v.x, v.y, v.z);
        }
        glEnd();
    }};

#endif //CG_LABS_OBJLOADER_H