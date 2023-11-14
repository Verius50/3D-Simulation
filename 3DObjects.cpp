#include "SFML/Graphics.hpp"
#include "3DObjects.h"
#include "iostream"

using namespace sf;

Vector3Df::Vector3Df() { x = 0; y = 0; z = 0; }
Vector3Df::Vector3Df(float X, float Y, float Z) { x = X; y = Y; z = Z; }
float Vector3Df::lenght() {
    return sqrt(x * x + y * y + z * z);
}
void Vector3Df::normalize() {
    float len = this->lenght();
    x /= len;
    y /= len;
    z /= len;
}
Vector3Df Vector3Df::operator+(Vector3Df sec) {
    return Vector3Df(x + sec.x, y + sec.y, z + sec.z);
}
Vector3Df Vector3Df::operator-(Vector3Df sec) {
    return Vector3Df(x - sec.x, y - sec.y, z - sec.z);
}
float Vector3Df::operator*(Vector3Df sec)
{
    return x * sec.x + y * sec.y + z * sec.z;
}
Vector3Df Vector3Df::operator*(float sec) {
    return Vector3Df(x * sec, y * sec, z * sec);
}
void Vector3Df::modulate() {
    x = abs(x); y = abs(y); z = abs(z);
}

Camera::Camera(Vector3Df pos, Vector3Df rt, float flwdh, float mvd) {
    position = pos, rotation = rt;
    maxvisdist = mvd; focalwidth = flwdh;
}
bool Camera::move(bool mouse, Vector2f mouse_pos)
{
    bool hasMoved = 0;
    if (Keyboard::isKeyPressed(Keyboard::W)) {
        position.x -= moveSpeed * 0.3 * sin(rotation.x) / fps;
        position.y -= moveSpeed * 0.3 * cos(rotation.x) / fps;
        hasMoved = 1;
    }
    if (Keyboard::isKeyPressed(Keyboard::S)) {
        position.x += moveSpeed * 0.3 * sin(rotation.x) / fps;
        position.y += moveSpeed * 0.3 * cos(rotation.x) / fps;
        hasMoved = 1;
    }

    if (Keyboard::isKeyPressed(Keyboard::D)) {
        position.x -= moveSpeed * 0.3 * cos(rotation.x) / fps;
        position.y += moveSpeed * 0.3 * sin(rotation.x) / fps;
        hasMoved = 1;
    }
    if (Keyboard::isKeyPressed(Keyboard::A)) {
        position.x += moveSpeed * 0.3 * cos(rotation.x) / fps;
        position.y -= moveSpeed * 0.3 * sin(rotation.x) / fps;
        hasMoved = 1;
    }
    if (Keyboard::isKeyPressed(Keyboard::Space)) {
        position.z += moveSpeed * 0.3 / fps;
        hasMoved = 1;
    }
    if (Keyboard::isKeyPressed(Keyboard::LShift)) {
        position.z -= moveSpeed * 0.3 / fps;
        hasMoved = 1;
    }

    if (mouse) {
        if (mouse_pos.x < width / 2) {
            rotation.x -= rotationSpeed * 0.03 / fps * abs(mouse_pos.x - width / 2) / 4;
            hasMoved = 1;
        }
        if (mouse_pos.x > width / 2) {
            rotation.x += rotationSpeed * 0.03 / fps * abs(mouse_pos.x - width / 2) / 4;
            hasMoved = 1;
        }
        if (mouse_pos.y < height / 2) {
            if (rotation.y > -3.14)
                rotation.y -= rotationSpeed * 0.03 / fps * abs(mouse_pos.y - height / 2) / 4;
            hasMoved = 1;
        }
        if (mouse_pos.y > height / 2) {
            if (rotation.y < 3.14)
                rotation.y += rotationSpeed * 0.03 / fps * abs(mouse_pos.y - height / 2) / 4;
            hasMoved = 1;
        }
    }
    return hasMoved;
}


sphere::sphere() { radius = 1; }
sphere::sphere(Vector3Df pos, float r, Vector3Df color) 
{ Center.x = pos.x; Center.y = pos.y; Center.z = pos.z; radius = r; this->color = color; }
float sphere::isRayIntersects(Vector3Df &camera, Vector3Df &RayDirection)
{
    Vector3Df s = camera - Center;
    float sum = s.x * RayDirection.x + s.y * RayDirection.y + s.z * RayDirection.z,
        sLenght = s.lenght(); 

    float Discriminant = sum * sum - (sLenght * sLenght - radius * radius);
    float dist = -1;
    if (Discriminant > 0)
        dist = (sum - sqrt(Discriminant));

    return dist;
}
Vector3Df sphere::countNormal(Vector3Df pointOnObject)
{
    return Center - pointOnObject;
}
float sphere::trayRay(Vector3Df camera, Vector3Df Ray, Vector3Df light, int lightPow)
{
    float dist = this->isRayIntersects(camera, Ray); 
    if (dist >= 0) {
        Vector3Df collision = camera - (Ray * dist);  //координаты точик пересечния луча со сферой
        Vector3Df to_light = light - collision;       //вектор от точки пересечения до источника света
        Vector3Df normal =  Center - collision;  //вектор от центра сферы до точки на окружности - нормаль к касательной поверхности

        return -(to_light * normal) / (to_light.lenght() * normal.lenght());
    }
    else return -1000;
}


plane::plane() { Center = { 0, 0, 0 }; normal = { 0, 0, 1 }; }
plane::plane(Vector3Df cen, Vector3Df nrm, Vector3Df color) { Center = cen; normal = nrm; this->color = color; }
float plane::isRayIntersects(Vector3Df &camera, Vector3Df &RayDirection)
{
    float D = -normal.x * Center.x - normal.y * Center.y - normal.z * Center.z;
    float t = (normal.x * camera.x + normal.y * camera.y + normal.z * camera.z + D) /
        (normal.x * RayDirection.x + normal.y * RayDirection.y + normal.z * RayDirection.z);
    return t;
}
Vector3Df plane::countNormal(Vector3Df pointOnObject)
{
    return normal* -1;
}
float plane::trayRay(Vector3Df camera, Vector3Df RayDirection, Vector3Df light, int lightPow)
{
    return 0.0f;
}
