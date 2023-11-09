#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class Vector3Df : public Vector3f {
public:
    Vector3Df() { x = 0; y = 0; z = 0; }
    Vector3Df(float X, float Y, float Z) { x = X; y = Y; z = Z; }
    float lenght() {
        return sqrt(x * x + y * y + z * z); 
    }
    void normalize() {
        float len = this->lenght();
        x /= len;
        y /= len;
        z /= len;
    }
    Vector3Df operator+(Vector3Df sec) {
        Vector3Df sum(x + sec.x, y + sec.y, z + sec.z);
        return sum;
    }
    Vector3Df operator-(Vector3Df sec) {
        Vector3Df dif(x - sec.x, y - sec.y, z - sec.z);
        return dif;
    }
    void modulate() {
        x = abs(x); y = abs(y); z = abs(z);
    }
};
    



class Camera 
{
public:
    Vector3Df position, rotation;
    float focalwidth = 100, maxvisdist = 400; 

    Camera(Vector3Df pos, Vector3Df rt, float flwdh, float mvd ) { 
        position = pos, rotation = rt;
        maxvisdist = mvd; focalwidth = flwdh;
    }
    
    void move(Vector2f mouse_pos)
    {
        /*if (Keyboard::isKeyPressed(Keyboard::W))
        {
            position.x += 0.01f * sin(rotation.x);
            position.y += 0.01f * cos(rotation.x);
            position.z += 0.01f * sin(rotation.y);

        }
        if (Keyboard::isKeyPressed(Keyboard::S))
        {
            position.x -= 0.01f * sin(rotation.x);
            position.y -= 0.01f * cos(rotation.x);
            position.z -= 0.01f * sin(rotation.y);
        }*/
        if (Keyboard::isKeyPressed(Keyboard::W))
            position.y += 0.01f;

        if (Keyboard::isKeyPressed(Keyboard::S))
            position.y -= 0.01f;
        if (Keyboard::isKeyPressed(Keyboard::A))
            position.x -= 0.01f;

        if (Keyboard::isKeyPressed(Keyboard::D))
            position.x += 0.01f;
       
        if (Keyboard::isKeyPressed(Keyboard::Space))
            position.z += 0.02;
        if (Keyboard::isKeyPressed(Keyboard::LShift))
            position.z -= 0.02;


        if (mouse_pos.x < 440)
            rotation.x -= 0.01;
        if (mouse_pos.x > 840)
            rotation.x += 0.01;
        if (mouse_pos.y < 160)
            rotation.y -= 0.01;
        if (mouse_pos.y > 560)
            rotation.y += 0.01;
    }
};

class sphere
{
private:
    Vector3Df Center;
    
public:
    float radius;
    sphere() { radius = 1; }
    sphere(float x, float y, float z, float r) { Center.x = x; Center.y = y; Center.z = z; radius = r; }
    float isRayIntersects(Vector3Df camera, Vector3Df RayDirection)
    {
        Vector3Df s = camera - Center; s.modulate();
        float sum = s.x * RayDirection.x + s.y * RayDirection.y + s.z * RayDirection.z,
            RayLenght = RayDirection.lenght(), sLenght = s.lenght();

        float Discriminant = 4 * (int)(sum * sum - RayLenght * RayLenght * (sLenght * sLenght - radius * radius));
        //std::cout << Discriminant << " | ";
        float dist = -1;
        if (Discriminant >= 0)
            dist = (sum - sqrt(Discriminant / 4.0)) / (RayLenght * RayLenght);

        return dist;
    }
};
const int width = 1280, height = 720, resolutionReduction = 4;
int main()
{
    Camera cam({ 0, 0, 0 }, { 0, 0, 0 }, 90, 10);
    sphere sph(0, 2, 0, 1);

    RenderWindow window(sf::VideoMode(width, height), "3D");
    window.setFramerateLimit(60);
    Image screenbuffer; screenbuffer.create(width, height, Color(0, 0, 0));
    Texture screentexture; screentexture.loadFromImage(screenbuffer);
    Sprite Screen(screentexture);
    int frame = 0;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        for (int yscreen = 0; yscreen < height / resolutionReduction; yscreen++)
            for (int xscreen = 0; xscreen < width / resolutionReduction; xscreen++){
                float x = 2 * float(xscreen * resolutionReduction) / width - 1.0f;
                float y =  2 * float(yscreen * resolutionReduction) / height - 1.0f;
                x *= (float)width / (float)height;
                
                Vector3Df Ray(cam.position.x + x + sin(cam.rotation.x) * (float)width / (float)height,
                    cam.position.y + cos(cam.rotation.x) * (float)width / (float)height,
                    cam.position.z + y + sin(cam.rotation.y) * (float)width / (float)height);
                Ray.normalize();

                float dist = sph.isRayIntersects(cam.position, Ray);
                Color pixelcolor(255, 255, 255);
                if (dist > 0)
                    pixelcolor = Color(int(dist/sph.radius * 100), 0, 0);

                for (size_t i = 0; i < resolutionReduction * resolutionReduction; i++)
                {
                   screenbuffer.setPixel(xscreen * resolutionReduction + i % resolutionReduction,
                      yscreen * resolutionReduction + i / resolutionReduction, pixelcolor);
                }
                    

            }
        frame++; std::cout << frame << "-";

        window.clear();
        screentexture.update(screenbuffer);
        window.draw(Screen);

        cam.move(Vector2f(Mouse::getPosition(window)));
        //if (Keyboard::isKeyPressed(Keyboard::A))
            //cam.position.x += 0.01f;
        
        //Mouse::setPosition({ 400, 300 }, window);


        window.display();
    }
}

