#pragma once
#include <string>
#include <fstream>
#include <glad/glad.h>
#include <GL/glu.h>
#include "Primitives.h"
#include "OBJLoader.h"

enum class ObjectType { Cube, Sphere, Torus, Teapot };

inline std::string objectTypeName(const ObjectType t) {
    switch (t) {
        case ObjectType::Cube:   return "Cube";
        case ObjectType::Sphere: return "Sphere";
        case ObjectType::Torus:  return "Torus";
        case ObjectType::Teapot: return "Teapot";
    }
    return "Unknown";
}

inline ObjectType objectTypeFromString(const std::string& s) {
    if (s == "Cube")   return ObjectType::Cube;
    if (s == "Sphere") return ObjectType::Sphere;
    if (s == "Torus")  return ObjectType::Torus;
    return ObjectType::Teapot;
}

struct Object3D {
    static inline int nextId = 1;

    int        id;
    ObjectType type;
    std::string name;

    float posX = 0, posY = 0, posZ = 0;
    float rotX = 0, rotY = 0, rotZ = 0;
    float scaX = 1, scaY = 1, scaZ = 1;
    float r = 0.6f, g = 0.6f, b = 0.9f;

    bool selected  = false;
    bool wireframe = false;

    explicit Object3D(const ObjectType t) : type(t) {
        id   = nextId++;
        name = objectTypeName(t) + "_" + std::to_string(id);
    }

    void draw(const OBJLoader& teapot) const {
        glPushMatrix();
        glTranslatef(posX, posY, posZ);
        glRotatef(rotX, 1, 0, 0);
        glRotatef(rotY, 0, 1, 0);
        glRotatef(rotZ, 0, 0, 1);
        glScalef(scaX, scaY, scaZ);

        if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        if (selected)
            glColor3f(1.0f, 1.0f, 0.0f);
        else
            glColor3f(r, g, b);

        switch (type) {
            case ObjectType::Cube:
                drawCube(1.0f);
                break;
            case ObjectType::Sphere:
                drawSphere(0.6f, 16, 16);
                break;
            case ObjectType::Torus:
                drawTorus(0.2f, 0.5f, 16, 32);
                break;
            case ObjectType::Teapot:
                glScalef(0.3f, 0.3f, 0.3f);
                teapot.dibujar();
                break;
        }

        if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glPopMatrix();
    }

    void serialize(std::ofstream& f) const {
        f << objectTypeName(type) << " "
          << posX << " " << posY << " " << posZ << " "
          << rotX << " " << rotY << " " << rotZ << " "
          << scaX << " " << scaY << " " << scaZ << " "
          << r    << " " << g    << " " << b    << "\n";
    }

    static Object3D deserialize(std::ifstream& f) {
        std::string typeName;
        f >> typeName;
        Object3D obj(objectTypeFromString(typeName));
        f >> obj.posX >> obj.posY >> obj.posZ
          >> obj.rotX >> obj.rotY >> obj.rotZ
          >> obj.scaX >> obj.scaY >> obj.scaZ
          >> obj.r    >> obj.g    >> obj.b;
        return obj;
    }
};
