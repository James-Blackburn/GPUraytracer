#ifndef VEC3_HPP_INCLUDED
#define VEC3_HPP_INCLUDED

#define CL_TARGET_OPENCL_VERSION 120

#include <cmath>
#include <CL/cl.h>

struct vec3 {
    float x;
    float y;
    float z;

    inline vec3& operator+=(const vec3& rhs) {
        x += rhs.x; y += rhs.y; z += rhs.z;
        return *this;
    }
    inline vec3& operator-=(const vec3& rhs) {
        x -= rhs.x; y -= rhs.y; z -= rhs.z;
        return *this;
    }
    inline vec3& operator*=(const vec3& rhs) {
        x *= rhs.x; y *= rhs.y; z *= rhs.z;
        return *this;
    }
    inline vec3& operator/=(const vec3& rhs) {
        x /= rhs.x; y /= rhs.y; z /= rhs.z;
        return *this;
    }

    inline vec3& operator-() {
        x = -x; y = -y; z = -z;
        return *this;
    }

    inline operator cl_float3() const {
        return {x, y, z};
    }

    inline void normalize();
    inline float length() const;
};

// operator overloading for useful operators

// vec-float operations
inline constexpr const vec3 operator*(const vec3& left, float right) {
    return { left.x * right, left.y * right, left.z * right };
}
inline constexpr const vec3 operator*(float left, const vec3& right) {
    return { left * right.x, left * right.y, left * right.z };
}
inline constexpr const vec3 operator/(const vec3& left, float right) {
    return { left.x / right, left.y / right, left.z / right };
}
inline constexpr const vec3 operator/(float left, const vec3 & right) {
    return { left / right.x, left / right.y, left / right.z };
}

// vec-vec operations
inline constexpr const vec3 operator+(const vec3& left, const vec3& right) {
    return { left.x + right.x, left.y + right.y, left.z + right.z };
}
inline constexpr const vec3 operator-(const vec3& left, const vec3& right) {
    return { left.x - right.x, left.y - right.y, left.z - right.z };
}
inline constexpr const vec3 operator*(const vec3& left, const vec3& right) {
    return { left.x * right.x, left.y * right.y, left.z * right.z };
}
inline constexpr const vec3 operator/(const vec3& left, const vec3& right) {
    return { left.x / right.x, left.y / right.y, left.z / right.z };
}

// vector operations

void vec3::normalize() {
    const float vlength = length();
    x = x / vlength;
    y = y / vlength;
    z = z / vlength;
}

inline vec3 normalize(vec3 vec) {
    float vlength = vec.length();
    return { vec.x / vlength, vec.y / vlength, vec.z / vlength };
}

float vec3::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

inline float length(const vec3& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

inline float dot(const vec3& v1, const vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

#endif