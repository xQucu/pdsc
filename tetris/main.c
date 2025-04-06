#include "primlib.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define BACKGROUND_COLOR BLACK
// It is assumed that the X number will always be set smaller than Y
#define TILES_NUMBER_X 10
#define TILES_NUMBER_Y 20
#define SIDEBAR_RATIO (1.0 / 3)
#define PICE_SIZE 4
#define FALLING_SPEED 1

struct tile
{
    int x;
    int y;
    int val;
};

struct piece
{
    int Left;
    int Top;
    int kind;
    int rotation;
    int speedFactor;
};

struct piece fallingPiece;
struct tile board[TILES_NUMBER_X][TILES_NUMBER_Y];
int tileSize = 0;
int sideBarLinePosition = 0;
int sideBarWidth = 0;
// pieces definition
char pieces[7 /*kind */][4 /* rotation */][4][4] = {
    /* square */
    {
        {{2, 1, 0, 0},
         {1, 1, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        {{2, 1, 0, 0},
         {1, 1, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        {{2, 1, 0, 0},
         {1, 1, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        {{2, 1, 0, 0},
         {1, 1, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}}},
    /* I */

    {
        {{1, 2, 1, 1},
         {0, 0, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        {{1, 0, 0, 0},
         {2, 0, 0, 0},
         {1, 0, 0, 0},
         {1, 0, 0, 0}},
        {{1, 1, 2, 1},
         {0, 0, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        {{1, 0, 0, 0},
         {1, 0, 0, 0},
         {2, 0, 0, 0},
         {1, 0, 0, 0}}},
    /* L */
    {
        {{1, 0, 0, 0},
         {2, 0, 0, 0},
         {1, 1, 0, 0},
         {0, 0, 0, 0}},
        {{1, 2, 1, 0},
         {1, 0, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        {{1, 1, 0, 0},
         {0, 2, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 0}},
        {{0, 0, 1, 0},
         {1, 2, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}}},
    /* L mirrored */
    {
        {{0, 1, 0, 0},
         {0, 2, 0, 0},
         {1, 1, 0, 0},
         {0, 0, 0, 0}},
        {{1, 0, 0, 0},
         {1, 2, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        {{1, 1, 0, 0},
         {2, 0, 0, 0},
         {1, 0, 0, 0},
         {0, 0, 0, 0}},
        {{1, 2, 1, 0},
         {0, 0, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}}},
    /* N */
    {
        {{0, 1, 0, 0},
         {2, 1, 0, 0},
         {1, 0, 0, 0},
         {0, 0, 0, 0}},
        {{1, 2, 0, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        {{0, 1, 0, 0},
         {1, 2, 0, 0},
         {1, 0, 0, 0},
         {0, 0, 0, 0}},
        {{1, 1, 0, 0},
         {0, 2, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}}},
    /* N mirrored */
    {
        {{1, 0, 0, 0},
         {2, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 0}},
        {{0, 2, 1, 0},
         {1, 1, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        {{1, 0, 0, 0},
         {1, 2, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 0}},
        {{0, 1, 1, 0},
         {1, 2, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}}},
    /* T */
    {
        {{1, 0, 0, 0},
         {2, 1, 0, 0},
         {1, 0, 0, 0},
         {0, 0, 0, 0}},
        {{1, 2, 1, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        {{0, 1, 0, 0},
         {1, 2, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 0}},
        {{0, 1, 0, 0},
         {1, 2, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}}}};

int min(int a, int b)
{
    if (a < b)
    {
        return a;
    }

    return b;
}

int calculateTileSize()
{
    return gfx_screenHeight() / TILES_NUMBER_Y;
}
void calculateTilesPosition()
{
    int boardLeftSidePosition = (gfx_screenWidth() - sideBarWidth) / 2 - TILES_NUMBER_X * tileSize / 2;
    int tileBottomPosition = gfx_screenHeight();
    for (int y = 0; y < TILES_NUMBER_Y; y++)
    {
        tileBottomPosition -= tileSize;
        for (int x = 0; x < TILES_NUMBER_X; x++)
        {
            board[x][y].x = boardLeftSidePosition + x * tileSize;
            board[x][y].y = tileBottomPosition;
        }
    }
    return;
}
void clearScreen()
{
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BACKGROUND_COLOR);
    return;
}

void loadNewPiece(int kind, int rotation)
{
    int centerOfNewPiece = (int)ceil(TILES_NUMBER_X / 2);
    int startLoadingAt = centerOfNewPiece - 1;

    fallingPiece.kind = kind;
    fallingPiece.rotation = rotation;
    fallingPiece.Top = TILES_NUMBER_Y - 1;
    fallingPiece.Left = startLoadingAt;

    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            board[x + startLoadingAt][fallingPiece.Top - y].val = pieces[kind][rotation][y][x];
        }
    }

    return;
}
void rotate(int newRotation) {}

void updateFallingPiecePosition(int pressedKey)
{
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y <= 4; y++)
        {
            board[x + fallingPiece.Left][fallingPiece.Top - y].val = 0;
        }
    }

    // printf("Falling piece position before update: TopY=%d, LeftX=%d\n", fallingPiece.Top, fallingPiece.Left);
    fallingPiece.speedFactor += FALLING_SPEED;
    if (fallingPiece.speedFactor / 100 > 1)
    {
        fallingPiece.speedFactor -= 100;
        fallingPiece.Top--;
    }
    // printf("Falling piece position after update: TopY=%d, LeftX=%d\n", fallingPiece.Top, fallingPiece.Left);

    printf("Falling piece Left position: %d\n", fallingPiece.Left);
    switch (pressedKey)
    {
        // todo left and right move boundary check
    case SDLK_LEFT:
        if (fallingPiece.Left > 0)
        {
            fallingPiece.Left--;
        }
        break;
    case SDLK_RIGHT:

        fallingPiece.Left++;

        break;
    case SDLK_SPACE:
        // rotate(fallingPiece.rotation + 1);
        break;
    case SDLK_DOWN:
        /* code */
        break;

    default:
        break;
    }

    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            board[x + fallingPiece.Left][fallingPiece.Top - y - 1].val = pieces[fallingPiece.kind][fallingPiece.rotation][y][x];
        }
    }
}

void drawTiles()
{
    for (int y = 0; y < TILES_NUMBER_Y; y++)
    {
        for (int x = 0; x < TILES_NUMBER_X; x++)
        {
            struct tile currentTile = board[x][y];
            if (currentTile.val == 1)
            {
                gfx_filledRect(currentTile.x, currentTile.y, currentTile.x + tileSize, currentTile.y + tileSize - 1, BLUE);
            }
            else if (currentTile.val == 2)
            {
                gfx_filledRect(currentTile.x, currentTile.y, currentTile.x + tileSize, currentTile.y + tileSize - 1, MAGENTA);
            }
            else
            {
                // todo remove this grid
                gfx_rect(currentTile.x, currentTile.y, currentTile.x + tileSize, currentTile.y + tileSize, RED);
            }
        }
    }
}

int main()
{
    if (gfx_init())
    {
        exit(3);
    }
    sideBarWidth = gfx_screenWidth() * SIDEBAR_RATIO;
    sideBarLinePosition = gfx_screenWidth() - sideBarWidth;
    tileSize = calculateTileSize();
    calculateTilesPosition();

    loadNewPiece(1, 1);
    while (true)
    {
        int pressedKey = gfx_pollkey();
        if (pressedKey == SDLK_ESCAPE)
        {
            exit(3);
        }
        clearScreen();
        gfx_line(sideBarLinePosition, 0, sideBarLinePosition, gfx_screenHeight(), WHITE);
        drawTiles();
        updateFallingPiecePosition(pressedKey);

        for (int y = 0; y < TILES_NUMBER_Y; y++)
        {
            for (int x = 0; x < TILES_NUMBER_X; x++)
            {
                printf("%d ", board[x][y].val);
            }
            printf("\n");
        }
        gfx_updateScreen();
        SDL_Delay(10);
    }

    return 0;
}