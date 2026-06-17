#pragma once
#include <windows.h>
#include <glad/glad.h>
#include <GL/glu.h>

inline void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    constexpr float ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    constexpr float diffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
    constexpr float specular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    constexpr float position[] = { 10.0f, 20.0f, 10.0f, 0.0f }; // 0=direccional

    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

inline void setLightDirectional() {
    constexpr float position[] = { 10.0f, 20.0f, 10.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

inline void setLightPoint() {
    constexpr float position[] = { 10.0f, 20.0f, 10.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}
