#pragma once
#include <functional>
#include <glad/glad.h>
#include <GL/glu.h>
#include "Geometry.h"
#include "Material.h"
#include "Lighting.h"

extern bool directionalLight;

class Scene {
public:
    virtual ~Scene() = default;
    virtual void init() {}
    virtual void render(float t) = 0;
    virtual void cleanup() {}
    virtual const char* name() const = 0;

protected:
    void setupCamera() {
        glLoadIdentity();
        gluLookAt(15, 12, 25,  0, 2, 0,  0, 1, 0);
        if (directionalLight) setLightDirectional();
        else                  setLightPoint();
    }

    void drawSceneGeometry(
        const std::function<void()>& applyTerrain,
        const std::function<void()>& applyWall,
        const std::function<void()>& applyRoof,
        const std::function<void()>& applyTrunk,
        const std::function<void()>& applyFoliage,
        float texRepeat = 2.0f)
    {
        // Terreno
        glPushMatrix();
            applyTerrain();
            drawTerrain(texRepeat);
        glPopMatrix();

        // Casa — paredes
        glPushMatrix();
            applyWall();
            drawHouseWalls(texRepeat);
        glPopMatrix();

        // Casa — techo (material/textura diferente)
        glPushMatrix();
            applyRoof();
            drawHouseRoof(texRepeat);
        glPopMatrix();

        // Arbol 1
        glPushMatrix();
            glTranslatef(10, 0, 5);
            drawTree(0.6f, 3.0f, 2.5f, applyTrunk, applyFoliage);
        glPopMatrix();

        // Arbol 2
        glPushMatrix();
            glTranslatef(-12, 0, -6);
            drawTree(0.5f, 2.5f, 2.0f, applyTrunk, applyFoliage);
        glPopMatrix();
    }
};
