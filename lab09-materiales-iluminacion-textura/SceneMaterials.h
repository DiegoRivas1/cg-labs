#pragma once
#include "Scene.h"

class SceneMaterials : public Scene {
public:
    [[nodiscard]] const char* name() const override { return "Escena 1 - Materiales"; }

    void render(float t) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        setupCamera();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_COLOR_MATERIAL);

        auto applyTerrain = []{ Materials::terrain().apply(); };
        auto applyWall    = []{ Materials::wall().apply();    };
        auto applyRoof    = []{ Materials::roof().apply();    };
        auto applyTrunk   = []{ Materials::trunk().apply();   };
        auto applyFoliage = []{ Materials::foliage().apply(); };

        drawSceneGeometry(applyTerrain, applyWall, applyRoof, applyTrunk, applyFoliage);
    }
};
