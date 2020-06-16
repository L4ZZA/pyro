#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"

static inline constexpr glm::vec3 axis_x    = glm::vec3( 1.f, 0.f, 0.f );
static inline constexpr glm::vec3 axis_y    = glm::vec3( 0.f, 1.f, 0.f );
static inline constexpr glm::vec3 axis_z    = glm::vec3( 0.f, 0.f, 1.f );
static inline constexpr glm::vec3 zero_vec3 = glm::vec3(0.0f, 0.0f, 0.0f);
static inline constexpr glm::vec3 origin    = zero_vec3;

inline bool operator<(const glm::vec3 &left, const glm::vec3 &right)
{
    return left.x < right.x &&left.y < right.y &&left.z < right.z;
}

inline bool operator<=(const glm::vec3 &left, const glm::vec3 &right)
{
    return left.x <= right.x && left.y <= right.y && left.z <= right.z;
}

inline bool operator>(const glm::vec3 &left, const glm::vec3 &right)
{
    return left.x > right.x && left.y > right.y && left.z > right.z;
}

inline bool operator>=(const glm::vec3 &left, const glm::vec3 &right)
{
    return left.x >= right.x && left.y >= right.y && left.z >= right.z;
}

inline bool operator==(const glm::vec3 &left, const glm::vec3 &right)
{
    return left.x == right.x && left.y == right.y && left.z == right.z;
}

inline bool operator<(const glm::vec2 &left, const glm::vec2 &right)
{
    return left.x < right.x &&left.y < right.y;
}

inline bool operator<=(const glm::vec2 &left, const glm::vec2 &right)
{
    return left.x <= right.x && left.y <= right.y;
}

inline bool operator>(const glm::vec2 &left, const glm::vec2 &right)
{
    return left.x > right.x && left.y > right.y;
}

inline bool operator>=(const glm::vec2 &left, const glm::vec2 &right)
{
    return left.x >= right.x && left.y >= right.y;
}

inline bool operator==(const glm::vec2 &left, const glm::vec2 &right)
{
    return left.x == right.x && left.y == right.y;
}