#pragma once
#include "SFML/Graphics.hpp"
using namespace sf;

const int moveSpeed = 10, rotationSpeed = 10;
const int width = 1080, height = 720, resolutionReduction = 4;


class Vector3Df : public Vector3f {
public:
    Vector3Df();
    Vector3Df(float X, float Y, float Z);
    float lenght();
    void normalize();
    Vector3Df operator+(Vector3Df sec);
    Vector3Df operator-(Vector3Df sec);
    float operator*(Vector3Df sec);
    Vector3Df operator*(float sec);
    void modulate();
};


class Camera
{
public:
    int fps = 60;
    Vector3Df position, rotation;
    float focalwidth = 100, maxvisdist = 400;

    Camera(Vector3Df pos, Vector3Df rt, float flwdh, float mvd);

    void move(Vector2f mouse_pos);
};

class object {
public:
    Vector3Df color;
    Vector3Df Center;
    virtual float isRayIntersects(Vector3Df camera, Vector3Df RayDirection) = 0;
    virtual float trayRay(Vector3Df camera, Vector3Df RayDirection, Vector3Df light, int lightPow) = 0;
};

class sphere : public object
{
public:
    float radius;
    sphere();
    sphere(Vector3Df pos, float r, Vector3Df color);
    float isRayIntersects(Vector3Df camera, Vector3Df RayDirection);
    float trayRay(Vector3Df camera, Vector3Df RayDirection, Vector3Df light, int lightPow);
};
class plane : public object
{
private:
    Vector3Df normal;
public:
    plane();
    plane(Vector3Df cen, Vector3Df nrm, Vector3Df color);
    float isRayIntersects(Vector3Df camera, Vector3Df RayDirection);
    float trayRay(Vector3Df camera, Vector3Df RayDirection, Vector3Df light, int lightPow);
};