#include <sm64_mario_geometry.hpp>

#include <godot_cpp/core/math.hpp>

SM64MarioGeometry::SM64MarioGeometry()
{
    geometry.position = position.data();
    geometry.normal = normal.data();
    geometry.color = color.data();
    geometry.uv = uv.data();

    geometry.numTrianglesUsed = 0;
}

SM64MarioGeometry::SM64MarioGeometry(const SM64MarioGeometry &other)
{
    position = other.position;
    normal = other.normal;
    color = other.color;
    uv = other.uv;

    geometry.position = position.data();
    geometry.normal = normal.data();
    geometry.color = color.data();
    geometry.uv = uv.data();

    geometry.numTrianglesUsed = other.geometry.numTrianglesUsed;
}

SM64MarioGeometry &SM64MarioGeometry::operator=(const SM64MarioGeometry &other)
{
    position = other.position;
    normal = other.normal;
    color = other.color;
    uv = other.uv;

    geometry.numTrianglesUsed = other.geometry.numTrianglesUsed;

    return *this;
}

void SM64MarioGeometry::lerp(const SM64MarioGeometry &last, const SM64MarioGeometry &current, float amount)
{
    geometry.numTrianglesUsed = godot::MIN(last.geometry.numTrianglesUsed, current.geometry.numTrianglesUsed);

    for (int i = 0; i < 9 * geometry.numTrianglesUsed; i++)
        position[i] = godot::Math::lerp(last.position[i], current.position[i], amount);

    for (int i = 0; i < 9 * geometry.numTrianglesUsed; i++)
        normal[i] = godot::Math::lerp(last.normal[i], current.normal[i], amount);

    color = current.color;
    uv = current.uv;
}
