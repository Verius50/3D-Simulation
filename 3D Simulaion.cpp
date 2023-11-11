#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "ctime"
#include "3DObjects.h"

using namespace sf;


int main()
{
    std::vector<object*> objects;
    Camera cam({ 0, 0, 0 }, { 3.14, 0, 0 }, 90, 10);
    objects.push_back(new sphere({ 0, 3, 2 }, 1, {255, 255, 0}));
    
    objects.push_back(new sphere({ 0, 3, 0 }, 1, { 255, 0, 0 }));
    objects.push_back(new sphere({ 0, 3, 1 }, 1, { 0, 255, 0 }));
    objects.push_back(new sphere({ 0, 3, 2 }, 1, { 0, 0, 255 }));
    objects.push_back(new sphere({ 0, 3, 3 }, 1, { 100, 0, 255 }));
    objects.push_back(new sphere({ 0, 3, 4 }, 1, { 255, 0, 100 }));
    objects.push_back(new sphere({ 0, 3, 5 }, 1, { 0, 255, 100 }));


    objects.push_back(new plane({ 0, 0, -2 }, { 0, 0, 1 }, {0, 205, 0}));

    RenderWindow window(sf::VideoMode(width, height), "3D");
    //window.setFramerateLimit(60);
    Image screenbuffer; screenbuffer.create(width, height, Color(0, 0, 0));
    Texture screentexture; screentexture.loadFromImage(screenbuffer);
    Sprite Screen(screentexture);
    int frame = 0;
    float dist = 0, min_dist = 100; int objNum = 0;
    int time = 0, previousTime = 0; int light = 0;
    Vector3Df lightPos = { 0, 2, 10 };
    while (window.isOpen())
    {
        time = clock();
        if (time > previousTime + 1000){
            cam.fps = frame;
            std::cout << '\r' << "Fps:" << cam.fps << ", camPos:{" << cam.position.x << ';' << cam.position.y << ';' << cam.position.z << '}';
            frame = 0; previousTime = time;
        }
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
                
                Vector3Df Ray(cos(cam.rotation.y) * sin(cam.rotation.x) + x * cos(cam.rotation.x) - sin(cam.rotation.x) * sin(cam.rotation.y) * y,
                    -x * sin(cam.rotation.x) + cos(cam.rotation.x) * cos(cam.rotation.y) - y * cos(cam.rotation.x) * sin(cam.rotation.y),
                    sin(cam.rotation.y) + y * cos(cam.rotation.y));
                Ray.normalize();
                Color pixelcolor(0, 190, 255);
                //Color pixelcolor(0, 0, 0);
                /*light = -1000;
                for (int k = 0; k < objects.size(); k++) {
                    int templight = objects[k]->trayRay(cam.position, Ray, lightPos, 10);
                    if (templight > light)
                        light = templight;
                }
                
                if (light >= 0)
                    pixelcolor = Color(light, light, light);
                if (light != -1000 && light < 0)
                    pixelcolor = Color(0, 0, 0);*/

                min_dist = 100;
                for (int k = 0; k < objects.size(); k++){
                    dist = objects[k]->isRayIntersects(cam.position, Ray);
                    if (dist > 0 && dist < min_dist) {
                        min_dist = dist;
                        objNum = k;
                    }
                }
                if (min_dist > 0 && min_dist < 100)
                    pixelcolor = Color(std::max(1, int(objects[objNum]->color.x - min_dist * 10)),
                        std::max(1, int(objects[objNum]->color.y - min_dist * 10)),
                        std::max(1, int(objects[objNum]->color.z - min_dist * 10)));

                for (size_t i = 0; i < resolutionReduction * resolutionReduction; i++)
                   screenbuffer.setPixel(xscreen * resolutionReduction + i % resolutionReduction,
                      yscreen * resolutionReduction + i / resolutionReduction, pixelcolor);
                
            }
        frame++; 
        for (size_t i = 1; i < objects.size() - 1; i++)
        {
            objects[i]->Center.x = 2 * sin(time / 500.0 + i);
            objects[i]->Center.z = 2+ 2 * cos(time / 500.0 + i);
        }
        //objects[0]->color = Vector3Df(fabs(255 * sin(time / 500.0)), fabs(255 * cos(time / 500.0)), 100);
        window.clear();
        screentexture.update(screenbuffer);
        window.draw(Screen);

        cam.move(Vector2f(Mouse::getPosition(window)));
        Mouse::setPosition({ width / 2, height / 2 }, window);


        window.display();
    }
}

