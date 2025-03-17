#include "primlib.h"
#include <stdio.h>
#include <stdbool.h>

#define NUMBER_OF_VERTICES 5
#define ROTATION_SPEED 0.02

#define RADIUS_CHANGE_SPEED 1
#define SCALING_MARGIN 30
#define ENTER_KEY_CODE 27
#define INITIAL_RADIUS 50

float updateRotationAngle(float angle)
{
    angle += ROTATION_SPEED;
    if (angle > 2 * M_PI)
    {
        return 2 * M_PI - angle;
    }
    return angle;
}

int updateRadius(int radius, int *radiusDelta)
{
    int maxRadius = fmin(gfx_screenHeight(), gfx_screenWidth()) / 2 - SCALING_MARGIN;
    if (radius + *radiusDelta > maxRadius || radius + *radiusDelta < SCALING_MARGIN)
    {
        *radiusDelta *= -1;
    }
    return radius + *radiusDelta;
}

void drawPolygon(float angleBetweenVertices, float rotationAngle, int screenCenterX, int screenCenterY, int radius)
{
    float prevX = 0;
    float prevY = 0;
    for (int i = 0; i <= NUMBER_OF_VERTICES; i++)
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
    float angleBetweenVertices = 2 * M_PI / NUMBER_OF_VERTICES;
    float rotationAngle = 0;
    int radius = INITIAL_RADIUS;
    int radiusDelta = RADIUS_CHANGE_SPEED;

    while (true)
    {
        if (gfx_pollkey() == ENTER_KEY_CODE)
        {
            break;
        }
        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
                       BLACK);

        radius = updateRadius(radius, &radiusDelta);
        rotationAngle = updateRotationAngle(rotationAngle);
        drawPolygon(angleBetweenVertices, rotationAngle, screenCenterX, screenCenterY, radius);

        gfx_updateScreen();
        SDL_Delay(10);
    }

    return 0;
}