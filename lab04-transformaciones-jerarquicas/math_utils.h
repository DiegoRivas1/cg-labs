//#pragma once
#ifndef CG_LABS_MATH_UTILS_H
#define CG_LABS_MATH_UTILS_H
#include <cmath>
#include <glad/glad.h>

// Una matriz 4x4 en column-major mimsma entrada que opneg
struct Mat4 {
    float m[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};

// Identidad
inline Mat4 mat4Identity() {
    Mat4 r;
    return r;
}

// Multiplicación A * B
inline Mat4 mat4Mul(const Mat4& A, const Mat4& B) {
    Mat4 C;
    for (int col = 0; col < 4; ++col)
        for (int row = 0; row < 4; ++row) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k)
                sum += A.m[k * 4 + row] * B.m[col * 4 + k];
            C.m[col * 4 + row] = sum;
        }
    return C;
}

// Traslación
inline Mat4 mat4Translate(float tx, float ty, float tz) {
    Mat4 T;
    T.m[12] = tx;
    T.m[13] = ty;
    T.m[14] = tz;
    return T;
}

// Rotación alrededor del eje Y (grados)
inline Mat4 mat4RotateY(float deg) {
    float r = deg * 3.14159265f / 180.0f;
    Mat4 R;
    R.m[0]  =  std::cos(r);
    R.m[2]  =  std::sin(r);
    R.m[8]  = -std::sin(r);
    R.m[10] =  std::cos(r);
    return R;
}

// Rotación alrededor del eje Z (grados)
inline Mat4 mat4RotateZ(float deg) {
    float r = deg * 3.14159265f / 180.0f;
    Mat4 R;
    R.m[0]  =  std::cos(r);
    R.m[1]  =  std::sin(r);
    R.m[4]  = -std::sin(r);
    R.m[5]  =  std::cos(r);
    return R;
}

// Rotación alrededor del eje X (grados)
inline Mat4 mat4RotateX(float deg) {
    float r = deg * 3.14159265f / 180.0f;
    Mat4 R;
    R.m[5]  =  std::cos(r);
    R.m[6]  =  std::sin(r);
    R.m[9]  = -std::sin(r);
    R.m[10] =  std::cos(r);
    return R;
}

// Escala
inline Mat4 mat4Scale(float sx, float sy, float sz) {
    Mat4 S;
    S.m[0]  = sx;
    S.m[5]  = sy;
    S.m[10] = sz;
    return S;
}

// Aplicar matriz a OpenGL (acumula sobre la matriz actual)
inline void glApplyMatrix(const Mat4& M) {
    glMultMatrixf(M.m);
}

#endif
