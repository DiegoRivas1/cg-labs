#pragma once
#include "Scene.h"
#include "TextureManager.h"

class SceneTextured : public Scene {
    GLuint texGrass=0, texBrick=0, texTile=0, texWood=0, texLeaves=0;

public:
    [[nodiscard]] const char* name() const override { return "Escena 3 - Texturas Reales"; }

    void init() override {
        texGrass  = TextureManager::load("textures/grass.jpg");
        texBrick  = TextureManager::load("textures/brick.jpg");
        texTile   = TextureManager::load("textures/tile.jpg");
        texWood   = TextureManager::load("textures/wood.jpg");
        texLeaves = TextureManager::load("textures/leaves.jpg");
        if (!texGrass)  texGrass  = TextureManager::makeGrass();
        if (!texBrick)  texBrick  = TextureManager::makeBrick();
        if (!texTile)   texTile   = TextureManager::makeTile();
        if (!texWood)   texWood   = TextureManager::makeWood();
        if (!texLeaves) texLeaves = TextureManager::makeLeaves();
    }

    void render(float t) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        setupCamera();
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glColor3f(1,1,1);

        GLuint tg=texGrass,tb=texBrick,tr=texTile,tw=texWood,tl=texLeaves;
        auto applyTerrain = [tg]{ TextureManager::bind(tg); };
        auto applyWall    = [tb]{ TextureManager::bind(tb); };
        auto applyRoof    = [tr]{ TextureManager::bind(tr); };
        auto applyTrunk   = [tw]{ TextureManager::bind(tw); };
        auto applyFoliage = [tl]{ TextureManager::bind(tl); };

        drawSceneGeometry(applyTerrain, applyWall, applyRoof, applyTrunk, applyFoliage, 4.0f);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_COLOR_MATERIAL);
    }

    void cleanup() override {
        TextureManager::release(texGrass); TextureManager::release(texBrick);
        TextureManager::release(texTile);  TextureManager::release(texWood);
        TextureManager::release(texLeaves);
    }
};
