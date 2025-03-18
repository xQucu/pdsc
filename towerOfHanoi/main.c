#include "primlib.h"
#include <stdio.h>
#include <stdbool.h>

// NUMBER_OF_PEGS should be no more than 10
#define PEGS_NUMBER 5
#define DISCS_NUMBER 5

#define ESCAPE_KEY_CODE 27
#define ENTER_KEY_CODE 13

#define DISC_COLOR BLUE
#define PEG_COLOR RED
#define BASE_COLOR YELLOW

#define PEG_HEIGHT 500
#define PEG_WIDTH 10

#define BASE_HEIGHT 10

bool push(int discSize, int *stack, int *size)
{
    if (*size + 1 > PEGS_NUMBER)
    {
        return false;
    }
    stack[*size] = discSize;
    (*size)++;
    return true;
}

bool pop(int *stack, int *size)
{
    if (*size == 0)
    {
        return false;
    }
    int disc = stack[*size];
    (*size)--;
    return disc;
}

void drawPegsAndBase()
{
    gfx_filledRect(0, gfx_screenHeight(), gfx_screenWidth(), gfx_screenHeight() - BASE_HEIGHT, BASE_COLOR);
    for (int i = 1; i <= PEGS_NUMBER; i++)
    {
        int pegLeftXCoord = i * gfx_screenWidth() / (PEGS_NUMBER + 1) - PEG_WIDTH / 2;
        gfx_filledRect(pegLeftXCoord, gfx_screenHeight() - BASE_HEIGHT - 1, pegLeftXCoord + PEG_WIDTH, gfx_screenHeight() - BASE_HEIGHT - PEG_HEIGHT, PEG_COLOR);
    }
    return;
}

// bool move() {}

void initialFillOfFirstPeg(int *stack, int *size)
{
    
    for (int discSize = DISCS_NUMBER; discSize >= 1; discSize--)
    {
        push(discSize, stack, size);
    }
}

void renderDiscs(int stacks[PEGS_NUMBER][DISCS_NUMBER], int sizes[PEGS_NUMBER])
{
    int discHeight = PEG_HEIGHT / (1 + DISCS_NUMBER);
    for (int p = 1; p <= PEGS_NUMBER; p++)
    {
        int discBottomCoord = gfx_screenHeight() - BASE_HEIGHT - 1;
        int pegCenterXCoord = p * gfx_screenWidth() / (PEGS_NUMBER + 1);
        // printf("PEG: %d, COORDS: %d\n", p, pegCenterXCoord);
        // printf("PEG: %d, SIZES[p]: %d\n", p, sizes[p]);
        for (int d = 1; d <= sizes[p - 1]; d++)
        {
            int discWidth = (stacks[p - 1][d] + 1) * 5;
            // printf("PEG: %d, DISC: %d, DISC_WIDTH: %d\n", p-1, d, discWidth);
            float discLeftXCoord = pegCenterXCoord - discWidth / 2.0;
            // printf("PEG: %d, CENTER: %d, LEFT: %f, DISC: %d, DISC_WIDTH: %d\n", p - 1, pegCenterXCoord, discLeftXCoord, d, discWidth);
            // printf("Drawing disc at: LEFT: %f, BOTTOM: %d, RIGHT: %f, TOP: %d\n", discLeftXCoord, discBottomCoord, discLeftXCoord + discWidth, discBottomCoord - discHeight);
            gfx_filledRect(discLeftXCoord, discBottomCoord, discLeftXCoord + discWidth, discBottomCoord - discHeight, DISC_COLOR);
            discBottomCoord -= discHeight;
        }
    }
}

int main()
{
    if (gfx_init())
    {
        exit(3);
    }

    int stacks[PEGS_NUMBER][DISCS_NUMBER];
    int stackSizes[PEGS_NUMBER] = {0};

    initialFillOfFirstPeg(stacks[0], &stackSizes[0]);

    while (true)
    {
        if (gfx_pollkey() == ESCAPE_KEY_CODE)
        {
            break;
        }
        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
                       BLACK);
        drawPegsAndBase();
        for (int i = 0; i < PEGS_NUMBER; i++)
        {
            printf("Stack %d (size %d): ", i, stackSizes[i]);
            for (int j = 0; j < stackSizes[i]; j++)
            {
                printf("%d ", stacks[i][j]);
            }
            printf("\n");
        }
        renderDiscs(stacks, stackSizes);

        gfx_updateScreen();
        SDL_Delay(10);
    }

    return 0;
}