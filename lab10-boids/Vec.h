#pragma once
#include <cmath>

struct Vec2 {
    float x = 0, y = 0;

    Vec2 operator+(const Vec2& o) const { return {x+o.x, y+o.y}; }
    Vec2 operator-(const Vec2& o) const { return {x-o.x, y-o.y}; }
    Vec2 operator*(float s)       const { return {x*s,   y*s};   }
    Vec2 operator/(float s)       const { return {x/s,   y/s};   }
    Vec2& operator+=(const Vec2& o) { x+=o.x; y+=o.y; return *this; }
    Vec2& operator-=(const Vec2& o) { x-=o.x; y-=o.y; return *this; }

    float length()  const { return std::sqrt(x*x + y*y); }
    float length2() const { return x*x + y*y; }

    Vec2 normalized() const {
        float l = length();
        return l > 0.0001f ? Vec2{x/l, y/l} : Vec2{0,0};
    }

    void limit(float max) {
        float l = length();
        if (l > max) { x = x/l*max; y = y/l*max; }
    }
};

struct Vec3 {
    float x = 0, y = 0, z = 0;

    Vec3 operator+(const Vec3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vec3 operator-(const Vec3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vec3 operator*(float s)       const { return {x*s,   y*s,   z*s};   }
    Vec3 operator/(float s)       const { return {x/s,   y/s,   z/s};   }
    Vec3& operator+=(const Vec3& o) { x+=o.x; y+=o.y; z+=o.z; return *this; }
    Vec3& operator-=(const Vec3& o) { x-=o.x; y-=o.y; z-=o.z; return *this; }

    float length()  const { return std::sqrt(x*x + y*y + z*z); }
    float length2() const { return x*x + y*y + z*z; }

    Vec3 normalized() const {
        float l = length();
        return l > 0.0001f ? Vec3{x/l, y/l, z/l} : Vec3{0,0,0};
    }

    void limit(float max) {
        float l = length();
        if (l > max) { x=x/l*max; y=y/l*max; z=z/l*max; }
    }

    Vec3 cross(const Vec3& o) const {
        return { y*o.z - z*o.y,
                 z*o.x - x*o.z,
                 x*o.y - y*o.x };
    }

    float dot(const Vec3& o) const { return x*o.x + y*o.y + z*o.z; }
};
