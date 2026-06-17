#pragma once
#include <glad/glad.h>
#include <string>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class TextureManager {
public:
    // Carga textura desde archivo
    static GLuint load(const std::string& path) {
        int w, h, ch;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 0);
        if (!data) return 0;

        GLuint id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        GLenum fmt = (ch == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return id;
    }

    // Textura procedural de cesped
    static GLuint makeGrass(int size = 128) {
        auto* px = new unsigned char[size * size * 3];
        for (int y = 0; y < size; ++y)
            for (int x = 0; x < size; ++x) {
                int i = (y * size + x) * 3;
                float n = noise(x * 0.15f, y * 0.15f);
                px[i+0] = static_cast<unsigned char>(30 + n * 20);
                px[i+1] = static_cast<unsigned char>(100 + n * 50);
                px[i+2] = static_cast<unsigned char>(20 + n * 15);
            }
        GLuint id = upload(px, size, size, GL_RGB);
        delete[] px;
        return id;
    }

    // Textura procedural de ladrillo
    static GLuint makeBrick(int size = 128) {
        auto* px = new unsigned char[size * size * 3];
        for (int y = 0; y < size; ++y)
            for (int x = 0; x < size; ++x) {
                int i = (y * size + x) * 3;
                int bh = 16, bw = 32;
                int row = y / bh;
                int ox = (row % 2) * (bw / 2);
                int lx = (x + ox) % bw, ly = y % bh;
                bool mortar = (lx < 2 || ly < 2);
                if (mortar) { px[i]=180; px[i+1]=170; px[i+2]=160; }
                else {
                    float n = noise(x*0.3f, y*0.3f) * 30;
                    px[i+0] = (unsigned char)(160 + n);
                    px[i+1] = (unsigned char)(60  + n * 0.4f);
                    px[i+2] = (unsigned char)(40  + n * 0.3f);
                }
            }
        GLuint id = upload(px, size, size, GL_RGB);
        delete[] px;
        return id;
    }

    // Textura procedural de teja
    static GLuint makeTile(int size = 128) {
        auto* px = new unsigned char[size * size * 3];
        for (int y = 0; y < size; ++y)
            for (int x = 0; x < size; ++x) {
                int i = (y * size + x) * 3;
                int th = 20, tw = 24;
                int row = y / th;
                int ox = (row % 2) * (tw / 2);
                int lx = (x + ox) % tw, ly = y % th;
                bool edge = (lx < 1 || ly < 1);
                float n = noise(x*0.2f, y*0.2f) * 20;
                if (edge) { px[i]=80; px[i+1]=40; px[i+2]=30; }
                else {
                    px[i+0] = (unsigned char)(150 + n);
                    px[i+1] = (unsigned char)(60  + n*0.5f);
                    px[i+2] = (unsigned char)(40  + n*0.3f);
                }
            }
        GLuint id = upload(px, size, size, GL_RGB);
        delete[] px;
        return id;
    }

    // Textura procedural de madera
    static GLuint makeWood(int size = 128) {
        auto* px = new unsigned char[size * size * 3];
        for (int y = 0; y < size; ++y)
            for (int x = 0; x < size; ++x) {
                int i = (y * size + x) * 3;
                float ring = std::sin(std::sqrt((float)(x*x + y*y)) * 0.3f) * 0.5f + 0.5f;
                float n = noise(x*0.1f, y*0.1f) * 0.2f;
                float v = ring + n;
                px[i+0] = (unsigned char)(100 + v * 80);
                px[i+1] = (unsigned char)(50  + v * 40);
                px[i+2] = (unsigned char)(10  + v * 15);
            }
        GLuint id = upload(px, size, size, GL_RGB);
        delete[] px;
        return id;
    }

    // Textura procedural de hojas
    static GLuint makeLeaves(int size = 128) {
        auto* px = new unsigned char[size * size * 3];
        for (int y = 0; y < size; ++y)
            for (int x = 0; x < size; ++x) {
                int i = (y * size + x) * 3;
                float n = noise(x*0.25f, y*0.25f);
                px[i+0] = (unsigned char)(10  + n * 20);
                px[i+1] = (unsigned char)(80  + n * 60);
                px[i+2] = (unsigned char)(10  + n * 20);
            }
        GLuint id = upload(px, size, size, GL_RGB);
        delete[] px;
        return id;
    }

    static void bind(GLuint id) {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    static void release(GLuint& id) {
        if (id) { glDeleteTextures(1, &id); id = 0; }
    }

private:
    static GLuint upload(unsigned char* px, int w, int h, GLenum fmt) {
        GLuint id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, px);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        return id;
    }

    static float noise(float x, float y) {
        int xi = (int)x, yi = (int)y;
        float xf = x - xi, yf = y - yi;
        auto r = [](int a, int b) {
            int n = a + b * 57;
            n = (n << 13) ^ n;
            return (1.0f - ((n*(n*n*15731+789221)+1376312589)&0x7fffffff)/1073741824.0f);
        };
        float v00=r(xi,yi), v10=r(xi+1,yi), v01=r(xi,yi+1), v11=r(xi+1,yi+1);
        float sx = xf*xf*(3-2*xf), sy = yf*yf*(3-2*yf);
        return v00 + (v10-v00)*sx + (v01-v00)*sy + (v11-v10-v01+v00)*sx*sy;
    }
};
