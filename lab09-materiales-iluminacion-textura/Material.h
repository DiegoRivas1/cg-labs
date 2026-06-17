#pragma once
#include <glad/glad.h>

struct Material {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shininess;

    void apply() const {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specular);
        glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
};

namespace Materials {
    inline Material terrain() {
        return { {0.1f,0.3f,0.05f,1}, {0.2f,0.6f,0.1f,1}, {0.1f,0.2f,0.1f,1}, 8.0f };
    }
    inline Material wall() {
        return { {0.3f,0.05f,0.05f,1}, {0.7f,0.1f,0.1f,1}, {0.3f,0.1f,0.1f,1}, 16.0f };
    }
    inline Material roof() {
        return { {0.2f,0.2f,0.2f,1}, {0.5f,0.5f,0.5f,1}, {0.4f,0.4f,0.4f,1}, 32.0f };
    }
    inline Material trunk() {
        return { {0.2f,0.1f,0.02f,1}, {0.4f,0.2f,0.05f,1}, {0.1f,0.05f,0.0f,1}, 4.0f };
    }
    inline Material foliage() {
        return { {0.05f,0.2f,0.05f,1}, {0.1f,0.5f,0.1f,1}, {0.05f,0.2f,0.05f,1}, 8.0f };
    }
}
