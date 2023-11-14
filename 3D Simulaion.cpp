#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "ctime"
#include "3DObjects.h"

using namespace sf;
int resolutionReduction = 1;

int main()
{
    std::vector<object*> objects;
    Camera cam({ 0, 0, 0 }, { 3.14, 0, 0 }, 90, 10);
    Vector3Df lightPos = { 0, 0, 10 };

    objects.push_back(new sphere({ 0, 3, 1 }, 1, { 204,30, 0 }));
    objects.push_back(new sphere({ 0, 3, 3 }, 1, { 0,30, 204 }));
    objects.push_back(new sphere({ 0, 3, 3 }, 1, { 30,204, 0 }));
    objects.push_back(new sphere({ 0, 3, 3 }, 0.3f, { 204,204, 0 }));


    /*objects.push_back(new sphere({ 0, 3, 7 }, 1, { 252,190, 126 }));
    objects.push_back(new sphere({ 1.8, 3, 7 }, 1, { 252,190, 126 }));
    objects.push_back(new sphere({ 0.9, 3, 1 }, 0.8, { 252,190, 126 }));
    objects.push_back(new sphere({ 0.9, 3, 2 }, 0.8, { 252,190, 126 }));
    objects.push_back(new sphere({ 0.9, 3, 3 }, 0.8, { 252,190, 126 }));
    objects.push_back(new sphere({ 0.9, 3, 4 }, 0.8, { 252,190, 126 }));
    objects.push_back(new sphere({ 0.9, 3, 5 }, 0.8, { 252,190, 126 }));*/

    objects.push_back(new plane({ 0, 0, -2 }, { 0, 0, 1 }, { 0, 205, 0 }));
    
    RenderWindow window(sf::VideoMode(width, height), "3D");
    window.setFramerateLimit(60);

    Image screenbuffer; screenbuffer.create(width, height, Color(0, 0, 0));
    Texture screentexture; screentexture.loadFromImage(screenbuffer);
    Sprite Screen(screentexture);

    
    bool shadows = 1, stopMouse = 1, isKeyPressed = 0;
    float dist = 0, min_dist = 100, color = 0, newdist = -1, lightPower = 30;
    int frame = 0, objNum = 0, time = 0, previousTime = 0;//int light = 0;
    
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event)){
            if (event.type == Event::Closed)
                window.close();
        }
        time = clock();
        if (time > previousTime + 1000) {
            cam.fps = frame;
            std::cout << '\r' << "Fps:" << cam.fps << ", camPos:{" << cam.position.x << ';' << cam.position.y << ';' << cam.position.z << " }, m:" << stopMouse;
            frame = 0; previousTime = time;
        }
        float cosry = cos(cam.rotation.y), cosrx = cos(cam.rotation.x), sinry = sin(cam.rotation.y), 
            sinrx = sin(cam.rotation.x);
        for (int yscreen = 0; yscreen < height / resolutionReduction; yscreen++)
            for (int xscreen = 0; xscreen < width / resolutionReduction; xscreen++){
                Color pixelcolor(0, 190, 255);
                float x = 2 * float(xscreen * resolutionReduction) / width - 1.0f;
                float y =  2 * float(yscreen * resolutionReduction) / height - 1.0f;
                x *= (float)width / (float)height;
                
                Vector3Df Ray(cosry * sinrx + x * cosrx - sinrx * sinry * y,
                    -x * sinrx + cosrx * cosry - y * cosrx * sinry,
                    sinry + y * cosry);
                Ray.normalize();
                
                /*light = -1000;
                for (int k = 0; k < objects.size(); k++) {
                    int templight = objects[k]->trayRay(cam.position, Ray, lightPos, 10);
                    if (templight > light)
                        light = templight;
                }
                if (light >= 0)
                    pixelcolor = Color(255*light, 255*light, 255*light);
                if (light != -1000 && light < 0)
                    pixelcolor = Color(0, 0, 0);*/

                min_dist = 100;
                for (int k = 0; k < objects.size(); k++) {
                    dist = objects[k]->isRayIntersects(cam.position, Ray);
                    if (dist > 0 && dist < min_dist) {
                        min_dist = dist;
                        objNum = k;
                    }
                }
                if (shadows && min_dist > 0 && min_dist < 100)
                {
                    pixelcolor = Color(0, 0, 0);
                    Vector3Df collision = cam.position - (Ray * min_dist);  //координаты точик пересечния луча со сферой
                    Vector3Df to_light = collision - lightPos;       //вектор от точки пересечения до источника света
                    float to_lightLenght = to_light.lenght();
                    to_light.normalize();
                    bool newCollision = 0;
                    for (int k = 0; k < objects.size(); k++) {
                        newdist = -1;
                        if (k != objNum)
                            newdist = objects[k]->isRayIntersects(collision, to_light);
                        if(newdist > 0 && newdist < to_lightLenght){
                            newCollision = 1;
                            color = fmax(0.10, fmin(0.22, newdist / lightPower));
                            break;
                        }
                    }
                    if (!newCollision) {
                        Vector3Df normal = objects[objNum]->countNormal(collision);  //вектор от центра сферы до точки на окружности - нормаль к касательной поверхности
                        color = (to_light * normal) / (normal.lenght());
                        if (color < 0)
                            color = 0.22f;
                        else color = fmax(0.22, color);
                        //color = color * ((float)lightPower / 30.0);
                    }
                    pixelcolor = Color(objects[objNum]->color.x * color, objects[objNum]->color.y * color, objects[objNum]->color.z * color);
                    
                }
                if(!shadows)
                    pixelcolor = Color(std::max(1, int(objects[objNum]->color.x - min_dist * 10)),
                        std::max(1, int(objects[objNum]->color.y - min_dist * 10)),
                        std::max(1, int(objects[objNum]->color.z - min_dist * 10)));

                for (size_t i = 0; i < resolutionReduction * resolutionReduction; i++)
                   screenbuffer.setPixel(xscreen * resolutionReduction + i % resolutionReduction,
                      yscreen * resolutionReduction + i / resolutionReduction, pixelcolor);
                
            }
        frame++;
        for (size_t i = 1; i < 3; i++)
        {
            objects[i]->Center.y = 3 + 2 * sin(time / 500.0 + i*3);
            objects[i]->Center.x = 2 * cos(time / 500.0 + i*3);
        }
        //objects[0]->color = Vector3Df(fabs(255 * sin(time / 500.0)), fabs(255 * cos(time / 500.0)), 100);
        //objects[0]->Center.x = 2 * sin(time / 500.0);
        //lightPos.x = 0.9 + 3 * sin(time / 500.0);
        lightPos.y = 3 + 3 * cos(time / 500.0);

        window.clear();
        screentexture.update(screenbuffer);
        window.draw(Screen);


        if (Keyboard::isKeyPressed(Keyboard::M) && !isKeyPressed) {
            stopMouse = !stopMouse;
            Mouse::setPosition({ width / 2, height / 2 }, window);
            isKeyPressed = 1;
        }
        if (!Keyboard::isKeyPressed(Keyboard::M))
            isKeyPressed = 0;

        if (cam.move(stopMouse, Vector2f(Mouse::getPosition(window))))
            resolutionReduction = 4;
        else resolutionReduction = 1;
        if(stopMouse)
            Mouse::setPosition({ width / 2, height / 2 }, window);



        window.display();
    }
}

