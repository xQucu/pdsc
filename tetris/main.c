#include "primlib.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define BACKGROUND_COLOR BLACK
// It is assumed that the X number will always be set smaller than Y
#define TILES_NUMBER_X 20
#define TILES_NUMBER_Y 20
#define SIDEBAR_RATIO (1.0 / 3)
#define PICE_SIZE 4

struct tile
{
    int x;
    int y;
    int val;
};

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

void loadPiece(int kind, int rotation)
{
    int centerOfNewPiece = (int)ceil(TILES_NUMBER_X / 2);
    printf("TILES_NUMBER_X / 2: %d\n", TILES_NUMBER_X / 2);
    // int startLoadingAt = centerOfNewPiece - PICE_SIZE / 2;
    int startLoadingAt = centerOfNewPiece - 1;
    // int startLoadingAt = 2;
    for (int x = 0; x < 4; x++)
    {
        printf("startLoadingAt + x: %d\n", startLoadingAt + x);
        printf("startLoadingAt second + x: %d\n", x + 1);
        for (int y = 0; y < 4; y++)
        {
            // board[x + startLoadingAt][TILES_NUMBER_Y - 1].val = pieces[kind][rotation][x][y];
            board[x + startLoadingAt][TILES_NUMBER_Y - y - 1].val = pieces[kind][rotation][x][y];
            printf("%d ", board[x + startLoadingAt][TILES_NUMBER_Y - 1].val);
            if (y == 3)
                printf("\n");
        }
    }

    return;
}

void drawTiles()
{
    for (int y = 0; y < TILES_NUMBER_Y; y++)
    {
        for (int x = 0; x < TILES_NUMBER_X; x++)
        {
            struct tile currentTile = board[x][y];
            if (currentTile.val > 0)
            {
                gfx_filledRect(currentTile.x, currentTile.y, currentTile.x + tileSize, currentTile.y + tileSize, RED);
            }
            else
            {
                gfx_rect(currentTile.x, currentTile.y, currentTile.x + tileSize, currentTile.y + tileSize, BLUE);
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

    loadPiece(6, 0);
    while (true)
    {
        clearScreen();
        gfx_line(sideBarLinePosition, 0, sideBarLinePosition, gfx_screenHeight(), WHITE);
        drawTiles();

        // for (int y = 0; y < TILES_NUMBER_Y; y++)
        // {
        //     for (int x = 0; x < TILES_NUMBER_X; x++)
        //     {
        //         printf("%d ", board[x][y].val);
        //     }
        //     printf("\n");
        // }
        gfx_updateScreen();
        SDL_Delay(10);
    }

    return 0;
}