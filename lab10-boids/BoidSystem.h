#pragma once
#include <vector>
#include <cstdlib>
#include <cmath>
#include "Boid.h"

struct BoidParams {
    int   count        = 80;
    float neighborDist = 3.0f;
    float sepDist      = 1.0f;
    float sepWeight    = 1.5f;
    float aliWeight    = 1.0f;
    float cohWeight    = 1.0f;
    float minSpeed     = 1.5f;
    float maxSpeed     = 4.0f;
    bool  toroidal     = true;   // true=wrap, false=bounce
};

// Sistema 2D
class BoidSystem2D {
public:
    std::vector<Boid2D> boids;
    BoidParams params;

    void init(float w, float h) {
        boids.resize(params.count);
        for (auto& b : boids) {
            b.pos = { randF(-w, w), randF(-h, h) };
            const float angle = randF(0, 6.2831f);
            const float sp = randF(params.minSpeed, params.maxSpeed);
            b.vel = { std::cos(angle)*sp, std::sin(angle)*sp };
            b.r = randF(0.3f,1.0f);
            b.g = randF(0.5f,1.0f);
            b.b = randF(0.7f,1.0f);
        }
    }

    void resize(float w, float h) {
        const int target = params.count;
        while ((int)boids.size() < target) {
            Boid2D b;
            b.pos = { randF(-w,w), randF(-h,h) };
            const float angle = randF(0,6.2831f);
            const float sp = randF(params.minSpeed, params.maxSpeed);
            b.vel = { std::cos(angle)*sp, std::sin(angle)*sp };
            b.r=randF(0.3f,1.0f); b.g=randF(0.5f,1.0f); b.b=randF(0.7f,1.0f);
            boids.push_back(b);
        }
        while ( (int) boids.size() > target)
            boids.pop_back();
    }

    void update(float dt, float w, float h) {
        for (size_t i = 0; i < boids.size(); ++i) {
            Vec2 sep{}, ali{}, coh{};
            int  sepN=0, aliN=0, cohN=0;

            for (size_t j = 0; j < boids.size(); ++j) {
                if (i == j) continue;
                Vec2 diff = boids[i].pos - boids[j].pos;
                float d = diff.length();

                if (d < params.sepDist && d > 0.0001f) {
                    sep += diff.normalized() / d;
                    ++sepN;
                }
                if (d < params.neighborDist) {
                    ali += boids[j].vel;
                    coh += boids[j].pos;
                    ++aliN; ++cohN;
                }
            }

            if (sepN > 0) boids[i].applyForce(sep.normalized() * params.sepWeight);
            if (aliN > 0) {
                Vec2 desired = (ali / static_cast<float>(aliN)).normalized() * params.maxSpeed;
                boids[i].applyForce((desired - boids[i].vel).normalized() * params.aliWeight);
            }
            if (cohN > 0) {
                Vec2 center = coh / static_cast<float>(cohN);
                Vec2 desired = (center - boids[i].pos).normalized() * params.maxSpeed;
                boids[i].applyForce((desired - boids[i].vel).normalized() * params.cohWeight);
            }

            boids[i].update(dt, params.minSpeed, params.maxSpeed);
            if (params.toroidal) boids[i].wrapBounds(w, h);
            else                 boids[i].bounceBounds(w, h);
        }
    }

private:
    static float randF(float a, float b) {
        return a + static_cast<float>(rand()) / RAND_MAX * (b - a);
    }
};

// Sistema 3D
class BoidSystem3D {
public:
    std::vector<Boid3D> boids;
    BoidParams params;

    void init(float bound) {
        boids.resize(params.count);
        for (auto& b : boids) {
            b.pos = { randF(-bound,bound), randF(-bound,bound), randF(-bound,bound) };
            Vec3 dir = { randF(-1,1), randF(-1,1), randF(-1,1) };
            const float sp = randF(params.minSpeed, params.maxSpeed);
            b.vel = dir.normalized() * sp;
            b.r=randF(0.3f,1.0f); b.g=randF(0.5f,1.0f); b.b=randF(0.7f,1.0f);
        }
    }

    void resize(float bound) {
        const int target = params.count;
        while ((int)boids.size() < target) {
            Boid3D b;
            b.pos = { randF(-bound,bound), randF(-bound,bound), randF(-bound,bound) };
            Vec3 dir = { randF(-1,1), randF(-1,1), randF(-1,1) };
            const float sp = randF(params.minSpeed, params.maxSpeed);
            b.vel = dir.normalized() * sp;
            b.r=randF(0.3f,1.0f); b.g=randF(0.5f,1.0f); b.b=randF(0.7f,1.0f);
            boids.push_back(b);
        }
        while ((int)boids.size() > target)
            boids.pop_back();
    }

    void update(float dt, float bound) {
        for (size_t i = 0; i < boids.size(); ++i) {
            Vec3 sep{}, ali{}, coh{};
            int  sepN=0, aliN=0, cohN=0;

            for (size_t j = 0; j < boids.size(); ++j) {
                if (i == j) continue;
                Vec3 diff = boids[i].pos - boids[j].pos;
                float d = diff.length();

                if (d < params.sepDist && d > 0.0001f) {
                    sep += diff.normalized() / d;
                    ++sepN;
                }
                if (d < params.neighborDist) {
                    ali += boids[j].vel;
                    coh += boids[j].pos;
                    ++aliN; ++cohN;
                }
            }

            if (sepN > 0) boids[i].applyForce(sep.normalized() * params.sepWeight);
            if (aliN > 0) {
                Vec3 desired = (ali / static_cast<float>(aliN)).normalized() * params.maxSpeed;
                boids[i].applyForce((desired - boids[i].vel).normalized() * params.aliWeight);
            }
            if (cohN > 0) {
                Vec3 center = coh / static_cast<float>(cohN);
                Vec3 desired = (center - boids[i].pos).normalized() * params.maxSpeed;
                boids[i].applyForce((desired - boids[i].vel).normalized() * params.cohWeight);
            }

            boids[i].update(dt, params.minSpeed, params.maxSpeed);
            if (params.toroidal) boids[i].wrapBounds(bound);
            else                 boids[i].bounceBounds(bound);
        }
    }

private:
    static float randF(float a, float b) {
        return a + static_cast<float>(rand()) / RAND_MAX * (b - a);
    }
};
