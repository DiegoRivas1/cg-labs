#pragma once
#include "Vec.h"

struct Boid2D {
    Vec2 pos;
    Vec2 vel;
    Vec2 acc;

    float r = 0.5f, g = 0.8f, b = 1.0f;

    void applyForce(Vec2 f) { acc += f; }

    void update(float dt, float minSpeed, float maxSpeed) {
        vel += acc * dt;
        vel.limit(maxSpeed);
        float sp = vel.length();
        if (sp < minSpeed && sp > 0.0001f)
            vel = vel.normalized() * minSpeed;
        pos += vel * dt;
        acc = {0,0};
    }

    void wrapBounds(float w, float h) {
        if (pos.x >  w) pos.x = -w;
        if (pos.x < -w) pos.x =  w;
        if (pos.y >  h) pos.y = -h;
        if (pos.y < -h) pos.y =  h;
    }

    void bounceBounds(float w, float h) {
        if (pos.x >  w || pos.x < -w) vel.x = -vel.x;
        if (pos.y >  h || pos.y < -h) vel.y = -vel.y;
        pos.x = std::max(-w, std::min(w, pos.x));
        pos.y = std::max(-h, std::min(h, pos.y));
    }
};

struct Boid3D {
    Vec3 pos;
    Vec3 vel;
    Vec3 acc;

    float r = 0.4f, g = 0.8f, b = 1.0f;

    void applyForce(Vec3 f) { acc += f; }

    void update(float dt, float minSpeed, float maxSpeed) {
        vel += acc * dt;
        vel.limit(maxSpeed);
        float sp = vel.length();
        if (sp < minSpeed && sp > 0.0001f)
            vel = vel.normalized() * minSpeed;
        pos += vel * dt;
        acc = {0,0,0};
    }

    void wrapBounds(float b_) {
        if (pos.x >  b_) pos.x = -b_;
        if (pos.x < -b_) pos.x =  b_;
        if (pos.y >  b_) pos.y = -b_;
        if (pos.y < -b_) pos.y =  b_;
        if (pos.z >  b_) pos.z = -b_;
        if (pos.z < -b_) pos.z =  b_;
    }

    void bounceBounds(float b_) {
        if (pos.x >  b_ || pos.x < -b_) vel.x = -vel.x;
        if (pos.y >  b_ || pos.y < -b_) vel.y = -vel.y;
        if (pos.z >  b_ || pos.z < -b_) vel.z = -vel.z;
        pos.x = std::max(-b_, std::min(b_, pos.x));
        pos.y = std::max(-b_, std::min(b_, pos.y));
        pos.z = std::max(-b_, std::min(b_, pos.z));
    }
};
