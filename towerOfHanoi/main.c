#include "primlib.h"
#include <stdio.h>
#include <stdbool.h>

// NUMBER_OF_PEGS should be no more than 10
#define PEGS_NUMBER 3
#define DISCS_NUMBER 3

#define ENTER_KEY_CODE 13

#define DISC_COLOR BLUE
#define PEG_COLOR RED
#define BASE_COLOR YELLOW

#define PEG_HEIGHT 500
#define PEG_WIDTH 10

#define BASE_HEIGHT 10

#define END_SCREEN_MESSAGE "Congratulations! You have won! Exit(escape) or Play again(enter)"
#define END_SCREEN_MESSAGE_COLOR RED

#define BACKGROUND_COLOR BLACK

enum
{
    PLAYING,
    PIECE_MOVING,
    FINISHED,
};

bool push(int discSize, int *stack, int *size)
{
    if (*size + 1 > DISCS_NUMBER)
    {
        return false;
    }
    stack[*size] = discSize;
    (*size)++;
    return true;
}

int pop(int *stack, int *size)
{
    if (*size == 0)
    {
        return false;
    }
    int disc = stack[*size - 1];
    (*size)--;
    return disc;
}

int peek(int *stack, int *size)
{
    if (*size == 0)
    {
        return 0;
    }
    return stack[*size - 1];
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

bool tryMoving(int *stackIdxToMoveFrom, int *stackIdxToMoveTo, int stacks[PEGS_NUMBER][DISCS_NUMBER], int sizes[PEGS_NUMBER])
{
    // todo check double click handling
    if (*stackIdxToMoveFrom == -1 || *stackIdxToMoveTo == -1 || *stackIdxToMoveTo >= PEGS_NUMBER || *stackIdxToMoveFrom >= PEGS_NUMBER)
    {
        return false;
    }

    // printf("Attempting to move from stack %d to stack %d\n", *stackIdxToMoveFrom, *stackIdxToMoveTo);
    // printf("Top of stack %d: %d\n", *stackIdxToMoveFrom, peek(stacks[*stackIdxToMoveFrom], &sizes[*stackIdxToMoveFrom]));
    // printf("Top of stack %d: %d\n", *stackIdxToMoveTo, peek(stacks[*stackIdxToMoveTo], &sizes[*stackIdxToMoveTo]));
    if ((peek(stacks[*stackIdxToMoveFrom], &sizes[*stackIdxToMoveFrom]) < peek(stacks[*stackIdxToMoveTo], &sizes[*stackIdxToMoveTo]) || peek(stacks[*stackIdxToMoveTo], &sizes[*stackIdxToMoveTo]) == 0) && peek(stacks[*stackIdxToMoveFrom], &sizes[*stackIdxToMoveFrom]) != 0)
    {
        // printf("Moving\n");
        int disc = pop(stacks[*stackIdxToMoveFrom], &sizes[*stackIdxToMoveFrom]);
        // printf("Got the disc\n");

        push(disc, stacks[*stackIdxToMoveTo], &sizes[*stackIdxToMoveTo]);
        *stackIdxToMoveTo = -1;
        *stackIdxToMoveFrom = -1;
        return true;
    }
    if (*stackIdxToMoveFrom != -1 && *stackIdxToMoveTo != -1)
    {
        *stackIdxToMoveTo = -1;
        *stackIdxToMoveFrom = -1;
    }
    return false;
}

void preparePegsForGame(int *stack, int *size)
{

    for (int i = 0; i < PEGS_NUMBER; i++)
    {
        size[i] = 0;
    }
    for (int discSize = DISCS_NUMBER; discSize >= 1; discSize--)
    {
        push(discSize, stack, &size[0]);
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
            // todo change how the width is handled
            int discWidth = (stacks[p - 1][d - 1] + 1) * 10;
            // printf("PEG: %d, DISC: %d, DISC_WIDTH: %d\n", p - 1, d, discWidth);
            float discLeftXCoord = pegCenterXCoord - discWidth / 2.0;
            // printf("PEG: %d, CENTER: %d, LEFT: %f, DISC: %d, DISC_WIDTH: %d\n", p - 1, pegCenterXCoord, discLeftXCoord, d, discWidth);
            // printf("Drawing disc at: LEFT: %f, BOTTOM: %d, RIGHT: %f, TOP: %d\n", discLeftXCoord, discBottomCoord, discLeftXCoord + discWidth, discBottomCoord - discHeight);
            gfx_filledRect(discLeftXCoord, discBottomCoord, discLeftXCoord + discWidth, discBottomCoord - discHeight, DISC_COLOR);
            discBottomCoord -= discHeight;
        }
    }
}
void handleKeyPress(int *stackToMoveFrom, int *stackToMoveTo)
{
    int key = gfx_pollkey();
    if (key == SDLK_ESCAPE)
    {
        exit(3);
    }
    // Numbers for keys from 0 to 1, have codes from 48 to 57,
    // thus when substracting 48 from key code, we get exact key number
    else if (key >= 49 && key <= 57)
    {
        if (*stackToMoveFrom == -1)
        {
            *stackToMoveFrom = (key + 1) % 10;
        }
        else
        {
            *stackToMoveTo = (key + 1) % 10;
        }
    }
    else if (key == SDLK_0)
    {
        if (*stackToMoveFrom == -1)
        {
            *stackToMoveFrom = 9;
        }
        else
        {

            *stackToMoveTo = 9;
        }
    }
}

bool endScreen()
{
    gfx_textout(gfx_screenWidth() / 3, gfx_screenHeight() / 2, END_SCREEN_MESSAGE, END_SCREEN_MESSAGE_COLOR);
    int key = gfx_pollkey();
    if (key == ENTER_KEY_CODE)
    {
        return true;
    }
    else if (key == SDLK_ESCAPE)
    {
        exit(3);
    }
    return false;
}

int main()
{
    if (gfx_init())
    {
        exit(3);
    }

    int stacks[PEGS_NUMBER][DISCS_NUMBER];
    int stackSizes[PEGS_NUMBER] = {0};
    int stackToMoveFrom = -1;
    int stackToMoveTo = -1;
    int gameState = PLAYING;

    preparePegsForGame(stacks[0], stackSizes);

    while (true)
    {
        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
                       BACKGROUND_COLOR);
        printf("LOOP RUNS\n");
        if (stackSizes[PEGS_NUMBER - 1] == DISCS_NUMBER)
        {
            printf("game won\n");
            gameState = FINISHED;
        }
        if (gameState == PLAYING)
        {
            printf("PLAYING\n");
            drawPegsAndBase();
            handleKeyPress(&stackToMoveFrom, &stackToMoveTo);
            tryMoving(&stackToMoveFrom, &stackToMoveTo, stacks, stackSizes);
            renderDiscs(stacks, stackSizes);
        }
        else if (gameState == FINISHED)
        {
            printf("game has finished\n");
            int shouldGameRestart = endScreen();
            if (shouldGameRestart)
            {
                preparePegsForGame(stacks[0], stackSizes);
                gameState = PLAYING;
            }
        }

        for (int i = 0; i < PEGS_NUMBER; i++)
        {
            printf("Stack %d (size %d): ", i, stackSizes[i]);
            for (int j = 0; j < stackSizes[i]; j++)
            {
                printf("%d ", stacks[i][j]);
            }
            printf("\n");
        }
        gfx_updateScreen();
        SDL_Delay(10);
    }

    return 0;
}