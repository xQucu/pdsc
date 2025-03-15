#include "primlib.h"
#include <stdio.h>
#include <stdbool.h>

#define numberOfVertices 5
#define rotationSpeed 0.02

#define radiusChangeSpeed 1
#define growAndShrinkMargin 20
#define enterKeyCode 27
#define initialRadius 50

float updateRotationAngle(float angle)
{
    angle += rotationSpeed;
    if (angle > 2 * M_PI)
    {
        return angle - (2 * M_PI);
    }
    return angle;
}

float updateRadius(int radius, bool *isGrowing)
{
    if (*isGrowing)
    {
        if (radius + radiusChangeSpeed < gfx_screenHeight() / 2 - growAndShrinkMargin && radius + radiusChangeSpeed < gfx_screenWidth() / 2 - growAndShrinkMargin)
        {
            radius += radiusChangeSpeed;
        }
        else
        {
            radius -= radiusChangeSpeed;
            *isGrowing = false;
        }
    }
    else
    {
        if (radius - radiusChangeSpeed > growAndShrinkMargin)
        {
            radius -= radiusChangeSpeed;
        }
        else
        {
            radius += radiusChangeSpeed;
            *isGrowing = true;
        }
    }
    return radius;
}

void drawPolygon(float angleBetweenVertices, float rotationAngle, int screenCenterX, int screenCenterY, int radius)
{
    float prevX = 0;
    float prevY = 0;
    for (int i = 0; i <= numberOfVertices; i++)
    {
        float angle = i * angleBetweenVertices + rotationAngle;
        float x = screenCenterX + radius * sin(angle);
        float y = screenCenterY + radius * cos(angle);
        if (prevX != 0 && prevY != 0)
        {
            gfx_line(prevX, prevY, x, y, RED);
        }
        prevX = x;
        prevY = y;
    }
}

int main()
{
    if (gfx_init())
    {
        exit(3);
    }

    int screenCenterX = gfx_screenWidth() / 2;
    int screenCenterY = gfx_screenHeight() / 2;
    float angleBetweenVertices = 2 * M_PI / numberOfVertices;
    float rotationAngle = 0;
    int radius = initialRadius;
    bool isRadiusGrowing = true;

    while (true)
    {
        if (gfx_pollkey() == enterKeyCode)
        {
            break;
        }
        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
                       BLACK);

        radius = updateRadius(radius, &isRadiusGrowing);
        rotationAngle = updateRotationAngle(rotationAngle);
        drawPolygon(angleBetweenVertices, rotationAngle, screenCenterX, screenCenterY, radius);

        gfx_updateScreen();
        SDL_Delay(10);
    }

    return 0;
}