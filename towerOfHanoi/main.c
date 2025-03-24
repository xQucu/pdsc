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

#define PEG_WIDTH 10

#define BASE_HEIGHT 10

#define END_SCREEN_MESSAGE "Congratulations! You have won! Exit(escape) or Play again(enter)"
#define END_SCREEN_MESSAGE_COLOR RED

#define BACKGROUND_COLOR BLACK
#define EXTRA_SPACING 4
#define ANIMATION_SPEED 2

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
    int spaceBetweenPegs = gfx_screenWidth() / (PEGS_NUMBER + 1);
    int discHeight = (gfx_screenHeight() - BASE_HEIGHT) / (DISCS_NUMBER + EXTRA_SPACING);
    int pegTopYCoord = gfx_screenHeight() - (DISCS_NUMBER + 1) * discHeight;
    for (int i = 1; i <= PEGS_NUMBER; i++)
    {
        int pegLeftXCoord = i * spaceBetweenPegs - (PEG_WIDTH / 2);
        gfx_filledRect(pegLeftXCoord, gfx_screenHeight() - BASE_HEIGHT - 1, pegLeftXCoord + PEG_WIDTH, pegTopYCoord, PEG_COLOR);
    }
    return;
}

bool animate(int *movingPieceX, int *movingPieceY, int movingPieceSize, int celling, int finalDiscX, int finalDiscY, int stacks[PEGS_NUMBER][DISCS_NUMBER], int stackSizes[PEGS_NUMBER], int stackToMoveTo)
{
    gfx_filledRect(*movingPieceX, *movingPieceY, *movingPieceX + movingPieceSize * (gfx_screenWidth() / (PEGS_NUMBER + 1) - PEG_WIDTH / 2) / DISCS_NUMBER, *movingPieceY + (gfx_screenHeight() - BASE_HEIGHT) / (DISCS_NUMBER + 4), DISC_COLOR);
    printf("movingPieceX: %d, movingPieceY: %d, movingPieceSize: %d, celling: %d, finalDiscX: %d, finalDiscY: %d\n", *movingPieceX, *movingPieceY, movingPieceSize, celling, finalDiscX, finalDiscY);

    int key = gfx_pollkey();
    if (key == SDLK_ESCAPE)
    {
        push(movingPieceSize, stacks[stackToMoveTo], &stackSizes[stackToMoveTo]);
        return true;
    }

    if (*movingPieceX != finalDiscX && *movingPieceY > celling)
    {
        (*movingPieceY) -= ANIMATION_SPEED;
        if (*movingPieceY < celling)
        {
            (*movingPieceY) = celling;
        }
    }
    else if (*movingPieceX != finalDiscX && *movingPieceY >= celling)
    {
        if (*movingPieceX < finalDiscX)
        {
            (*movingPieceX) += ANIMATION_SPEED;
        }
        else
        {
            (*movingPieceX) -= ANIMATION_SPEED;
        }
    }
    else if (*movingPieceX == finalDiscX && *movingPieceY < finalDiscY)
    {
        (*movingPieceY) += ANIMATION_SPEED;
    }
    else
    {
        push(movingPieceSize, stacks[stackToMoveTo], &stackSizes[stackToMoveTo]);
        return true;
    }
    return false;
}

bool tryMoving(int *stackIdxToMoveFrom, int *stackIdxToMoveTo, int stacks[PEGS_NUMBER][DISCS_NUMBER], int sizes[PEGS_NUMBER], int *movingPieceX, int *movingPieceY, int *movingPieceSize, int *celling, int *finalDiscX, int *finalDiscY)
{
    if (*stackIdxToMoveFrom == -1 || *stackIdxToMoveTo == -1 || *stackIdxToMoveTo >= PEGS_NUMBER || *stackIdxToMoveFrom >= PEGS_NUMBER)
    {
        return false;
    }

    if ((peek(stacks[*stackIdxToMoveFrom], &sizes[*stackIdxToMoveFrom]) < peek(stacks[*stackIdxToMoveTo], &sizes[*stackIdxToMoveTo]) || peek(stacks[*stackIdxToMoveTo], &sizes[*stackIdxToMoveTo]) == 0) && peek(stacks[*stackIdxToMoveFrom], &sizes[*stackIdxToMoveFrom]) != 0)
    {

        // pop disc and save it to movingDiscSize
        *movingPieceSize = pop(stacks[*stackIdxToMoveFrom], &sizes[*stackIdxToMoveFrom]);
        int discHeight = (gfx_screenHeight() - BASE_HEIGHT) / (DISCS_NUMBER + 4);

        // calculate current cords and set it to movingDiscX and movingDiscY
        *movingPieceY = gfx_screenHeight() - BASE_HEIGHT - 1 - (sizes[*stackIdxToMoveFrom] + 1) * discHeight;
        *movingPieceX = (*stackIdxToMoveFrom + 1) * gfx_screenWidth() / (PEGS_NUMBER + 1) - *movingPieceSize * (gfx_screenWidth() / (PEGS_NUMBER + 1) - PEG_WIDTH / 2) / DISCS_NUMBER / 2;

        // calculate end coords and set it to finalDiscX and finalDiscX
        *finalDiscY = gfx_screenHeight() - BASE_HEIGHT - 1 - (sizes[*stackIdxToMoveTo] + 1) * discHeight;
        *finalDiscX = (*stackIdxToMoveTo + 1) * gfx_screenWidth() / (PEGS_NUMBER + 1) - *movingPieceSize * (gfx_screenWidth() / (PEGS_NUMBER + 1) - PEG_WIDTH / 2) / DISCS_NUMBER / 2;

        // calculate celling and set it to celling
        *celling = discHeight;

        // int pegTopYCoord = gfx_screenHeight() - (DISCS_NUMBER + 1) * discHeight;

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
    int discHeight = (gfx_screenHeight() - BASE_HEIGHT) / (DISCS_NUMBER + 4);
    int discScale = (gfx_screenWidth() / (PEGS_NUMBER + 1) - PEG_WIDTH / 2) / DISCS_NUMBER;
    for (int p = 1; p <= PEGS_NUMBER; p++)
    {
        int discBottomCoord = gfx_screenHeight() - BASE_HEIGHT - 1;
        int pegCenterXCoord = p * gfx_screenWidth() / (PEGS_NUMBER + 1);
        for (int d = 1; d <= sizes[p - 1]; d++)
        {
            int discWidth = stacks[p - 1][d - 1] * discScale;
            float discLeftXCoord = pegCenterXCoord - discWidth / 2.0;
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
    // thus instead of long switch, simple math is used to get peg number
    // eg. 1 is pressed, then key value is 49; (49+1)%10=0 <- first peg.

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

    int movingPieceX = 0;
    int movingPieceY = 0;
    int movingPieceSize = 0;
    int celling = 0;
    int finalDiscX = 0;
    int finalDiscY = 0;

    preparePegsForGame(stacks[0], stackSizes);

    while (true)
    {

        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
                       BACKGROUND_COLOR);
        if (stackSizes[PEGS_NUMBER - 1] == DISCS_NUMBER)
        {
            gameState = FINISHED;
        }

        if (gameState == PLAYING || gameState == PIECE_MOVING)
        {
            drawPegsAndBase();
            renderDiscs(stacks, stackSizes);
        }

        if (gameState == PLAYING)
        {
            handleKeyPress(&stackToMoveFrom, &stackToMoveTo);
            bool isMoving = tryMoving(&stackToMoveFrom, &stackToMoveTo, stacks, stackSizes, &movingPieceX, &movingPieceY, &movingPieceSize, &celling, &finalDiscX, &finalDiscY);
            if (isMoving)
            {
                gameState = PIECE_MOVING;
            }
        }
        else if (gameState == PIECE_MOVING)
        {
            int isAnimationFinished = animate(&movingPieceX, &movingPieceY, movingPieceSize, celling, finalDiscX, finalDiscY, stacks, stackSizes, stackToMoveTo);
            if (isAnimationFinished)
            {
                gameState = PLAYING;
            }
        }
        else if (gameState == FINISHED)
        {
            int shouldGameRestart = endScreen();
            if (shouldGameRestart)
            {
                stackToMoveFrom = -1;
                stackToMoveTo = -1;
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