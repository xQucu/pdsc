#include "primlib.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define BACKGROUND_COLOR BLACK
// It is assumed that the X number will always be set smaller than Y
#define TILES_NUMBER_X 10
#define TILES_NUMBER_Y 20
#define SIDEBAR_RATIO (1.0 / 3)
#define NEXT_PIECE_DISPLAY_POSITION 3

#define PIECE_SIZE 4
#define PIECE_ROTATIONS 4
#define PIECE_KINDS 7

#define FALLING_SPEED 1
#define BOOST 50

#define END_SCREEN_MESSAGE "You have failed terribly! Maybe reconsider your live choices!!! Exit(escape) or Play again(enter)"

#define END_SCREEN_MESSAGE_COLOR RED
#define FALLING_PIECE_COLOR CYAN
#define RESTING_PIECE_COLOR RED
#define FALLING_PIECE_CENTER_COLOR MAGENTA
#define SIDE_LINES_COLOR BLUE

enum gameState
{
    IN_PROGRESS,
    END_SCREEN,
    RESTARTING
};
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
    int width;
    int height;
};

struct coords
{
    int x;
    int y;
};

struct piece fallingPiece;
struct tile board[TILES_NUMBER_X][TILES_NUMBER_Y];
struct coords smallGrid[PIECE_SIZE][PIECE_SIZE];
int tileSize = 0;
int sideBarLinePosition = 0;
int sideBarWidth = 0;
int gameState = IN_PROGRESS;
int nextPiece = 0;
int nextPieceRotation = 0;

char pieces[PIECE_KINDS /*kind */][PIECE_ROTATIONS /* rotation */][PIECE_SIZE][PIECE_SIZE] = {
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

void selectNextPiece()
{
    nextPiece = rand() % PIECE_KINDS;
    nextPieceRotation = rand() % PIECE_ROTATIONS;

    return;
}

void loadNewPiece()
{
    int centerOfNewPiece = (int)ceil(TILES_NUMBER_X / 2);
    int startLoadingAt = centerOfNewPiece - 1;

    fallingPiece.kind = nextPiece;
    fallingPiece.rotation = nextPieceRotation;
    fallingPiece.Top = TILES_NUMBER_Y - 1;
    fallingPiece.Left = startLoadingAt;

    int width = 0;
    int height = 0;

    for (int x = 0; x < PIECE_SIZE; x++)
    {
        for (int y = 0; y < PIECE_SIZE; y++)
        {
            int pieceCellValue = pieces[nextPiece][nextPieceRotation][x][y];
            if (pieceCellValue != 0 && width < x)
            {
                width = x;
            }
            if (pieceCellValue != 0 && height < y)
            {
                height = y;
            }
            if (board[x + startLoadingAt][fallingPiece.Top - y].val != 0 && pieceCellValue)
            {
                gameState = END_SCREEN;
                return;
            }
            if (board[x + startLoadingAt][fallingPiece.Top - y].val == 0)
            {
                board[x + startLoadingAt][fallingPiece.Top - y].val = pieceCellValue;
            }
        }
    }
    fallingPiece.width = width + 1;
    fallingPiece.height = height + 1;

    return;
}

void restartGame()
{
    for (int y = 0; y < TILES_NUMBER_Y; y++)
    {
        for (int x = 0; x < TILES_NUMBER_X; x++)
        {
            board[x][y].val = 0;
        }
    }
    gameState = IN_PROGRESS;

    fallingPiece.Left = 0;
    fallingPiece.Top = 0;
    fallingPiece.kind = 0;
    fallingPiece.rotation = 0;
    fallingPiece.speedFactor = 0;
    fallingPiece.width = 0;
    fallingPiece.height = 0;

    loadNewPiece();
    selectNextPiece();

    return;
}

void drawTile(int tileValue, int x, int y, int xCoord, int yCoord)
{
    switch (tileValue)
    {
    case 1:
        gfx_filledRect(xCoord, yCoord, xCoord + tileSize, yCoord + tileSize - 1, FALLING_PIECE_COLOR);
        break;
    case 2:
        gfx_filledRect(xCoord, yCoord, xCoord + tileSize, yCoord + tileSize - 1, FALLING_PIECE_CENTER_COLOR);
        break;
    case 3:
        gfx_filledRect(xCoord, yCoord, xCoord + tileSize, yCoord + tileSize - 1, RESTING_PIECE_COLOR);
        break;
    }
}

void displayFuturePiece()
{
    for (int x = 0; x < PIECE_SIZE; x++)
    {
        for (int y = 0; y < PIECE_SIZE; y++)
        {
            int xCoord = smallGrid[x][PIECE_SIZE - 1 - y].x;
            int yCoord = smallGrid[x][PIECE_SIZE - 1 - y].y;

            drawTile(pieces[nextPiece][nextPieceRotation][x][y], x, y, xCoord, yCoord);
        }
    }

    return;
}

void calculateSmallGridPosition()
{
    int leftSide = sideBarLinePosition + sideBarWidth / 2 - PIECE_SIZE / 2 * tileSize;
    int tileBottomPosition = gfx_screenHeight() / NEXT_PIECE_DISPLAY_POSITION;
    for (int y = 0; y < PIECE_SIZE; y++)
    {
        tileBottomPosition -= tileSize;
        for (int x = 0; x < PIECE_SIZE; x++)
        {
            smallGrid[x][y].x = leftSide + x * tileSize;
            smallGrid[x][y].y = tileBottomPosition;
        }
    }

    return;
}

void endScreen(int key)
{
    gfx_textout(gfx_screenWidth() / PIECE_SIZE, gfx_screenHeight() / 2, END_SCREEN_MESSAGE, END_SCREEN_MESSAGE_COLOR);

    if (key == SDLK_RETURN)
    {
        restartGame();
    }
    else if (key == SDLK_ESCAPE)
    {
        exit(3);
    }

    return;
}

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

void rotate(int newRotation)
{
    newRotation = newRotation > PIECE_ROTATIONS - 1 ? 0 : newRotation;
    int prevCenterX = 0;
    int prevCenterY = 0;
    int newCenterX = 0;
    int newCenterY = 0;
    for (int x = 0; x < PIECE_SIZE; x++)
    {
        for (int y = 0; y < PIECE_SIZE; y++)
        {
            if (pieces[fallingPiece.kind][fallingPiece.rotation][x][y] == 2)
            {
                prevCenterX = x;
                prevCenterY = y;
            }
            if (pieces[fallingPiece.kind][newRotation][x][y] == 2)
            {
                newCenterX = x;
                newCenterY = y;
            }
        }
    }
    int newLeft = fallingPiece.Left + prevCenterX - newCenterX;
    int newTop = fallingPiece.Top - prevCenterY - newCenterY;
    if (newLeft < 0 || newLeft + fallingPiece.height > TILES_NUMBER_X)
    {
        return;
    }

    if (newTop + fallingPiece.width > TILES_NUMBER_Y || newTop <= 0)
    {
        return;
    }

    for (int x = 0; x < PIECE_SIZE; x++)
    {
        for (int y = 0; y < PIECE_SIZE; y++)
        {
            int newX = newLeft + x;
            int newY = newTop - y;

            if (pieces[fallingPiece.kind][newRotation][x][y] != 0)
            {
                if (newX < 0 || newX >= TILES_NUMBER_X || newY < 0 || newY >= TILES_NUMBER_Y || board[newX][newY].val == 3)
                {
                    return;
                }
            }
        }
    }
    fallingPiece.Left += prevCenterX - newCenterX;
    fallingPiece.Top -= prevCenterY - newCenterY;
    fallingPiece.rotation = newRotation;
    int tmp = fallingPiece.height;
    fallingPiece.height = fallingPiece.width;
    fallingPiece.width = tmp;

    return;
}

void clearRowAndShiftElementsDown(int removedRow)
{
    for (int y = removedRow; y < TILES_NUMBER_Y - 1; y++)
    {
        for (int x = 0; x < TILES_NUMBER_X; x++)
        {
            board[x][y].val = board[x][y + 1].val;
        }
    }
}

void checkFullRows()
{
    bool rowsClear = false;
    while (!rowsClear)
    {
        rowsClear = true;
        for (int y = 0; y < TILES_NUMBER_Y; y++)
        {
            int sum = 0;
            for (int x = 0; x < TILES_NUMBER_X; x++)
            {
                int tileValue = board[x][y].val;
                if (tileValue != 0)
                {
                    sum++;
                }
            }
            if (sum == TILES_NUMBER_X)
            {
                clearRowAndShiftElementsDown(y);
                rowsClear = false;
                break;
            }
        }
    }

    return;
}

void stopPiece()
{
    printf("Pice is Stopped\n");
    for (int x = 0; x < PIECE_SIZE; x++)
    {
        for (int y = 0; y < PIECE_SIZE; y++)
        {
            if (x + fallingPiece.Left < TILES_NUMBER_X && !(fallingPiece.Top - y < 0 || board[x + fallingPiece.Left][fallingPiece.Top - y].val))
            {
                board[x + fallingPiece.Left][fallingPiece.Top - y].val = pieces[fallingPiece.kind][fallingPiece.rotation][x][y] != 0 ? 3 : 0;
            }
        }
    }

    checkFullRows();
    loadNewPiece();
    selectNextPiece();
    selectNextPiece();
    return;
}
void redrawPiece()
{
    for (int x = 0; x < PIECE_SIZE; x++)
    {
        for (int y = 0; y < PIECE_SIZE; y++)
        {
            if (x + fallingPiece.Left < TILES_NUMBER_X && (!(fallingPiece.Top - y < 0 || board[x + fallingPiece.Left][fallingPiece.Top - y].val)))
            {
                board[x + fallingPiece.Left][fallingPiece.Top - y].val = pieces[fallingPiece.kind][fallingPiece.rotation][x][y];
            }
        }
    }
}
void updateFallingPiecePosition(int pressedKey)
{
    for (int x = 0; x < PIECE_SIZE; x++)
    {
        for (int y = 0; y <= PIECE_SIZE; y++)
        {
            if (fallingPiece.Top - y > 0 && x + fallingPiece.Left < TILES_NUMBER_X &&
                fallingPiece.Top - y < TILES_NUMBER_Y && board[x + fallingPiece.Left][fallingPiece.Top - y].val != 3)
            {
                board[x + fallingPiece.Left][fallingPiece.Top - y].val = 0;
            }
        }
    }

    switch (pressedKey)
    {
    case SDLK_LEFT:
        if (fallingPiece.Left > 0)
        {
            for (int x = 0; x < PIECE_SIZE; x++)
            {
                for (int y = 0; y < PIECE_SIZE; y++)
                {
                    if (pieces[fallingPiece.kind][fallingPiece.rotation][x][y] != 0)
                    {
                        int newX = fallingPiece.Left + x - 1;
                        int newY = fallingPiece.Top - y;

                        if (newX >= 0 && newY >= 0 && board[newX][newY].val == 3)
                        {
                            redrawPiece();
                            return;
                        }
                    }
                }
            }
            fallingPiece.Left--;
        }
        break;
    case SDLK_RIGHT:

        if (fallingPiece.Left + fallingPiece.width < TILES_NUMBER_X)
        {
            for (int x = 0; x < PIECE_SIZE; x++)
            {
                for (int y = 0; y < PIECE_SIZE; y++)
                {
                    if (pieces[fallingPiece.kind][fallingPiece.rotation][x][y] != 0)
                    {
                        int newX = fallingPiece.Left + x + 1;
                        int newY = fallingPiece.Top - y;

                        if (newX < TILES_NUMBER_X && newY >= 0 && board[newX][newY].val == 3)
                        {
                            redrawPiece();
                            return;
                        }
                    }
                }
            }
            fallingPiece.Left++;
        }

        break;
    case SDLK_SPACE:
        rotate(fallingPiece.rotation + 1);
        break;
    case SDLK_DOWN:

        fallingPiece.speedFactor += FALLING_SPEED * BOOST;
        break;

    default:
        break;
    }

    fallingPiece.speedFactor += FALLING_SPEED;
    if (fallingPiece.speedFactor / 100 > 1)
    {
        fallingPiece.speedFactor -= 100;
        fallingPiece.Top--;
    }
    if (fallingPiece.Top - fallingPiece.height == -1)
    {
        stopPiece();
        return;
    }

    for (int x = 0; x < PIECE_SIZE; x++)
    {
        for (int y = 0; y < PIECE_SIZE; y++)
        {
            if (fallingPiece.Top - y - 1 >= 0 && x + fallingPiece.Left < TILES_NUMBER_X &&
                board[x + fallingPiece.Left][fallingPiece.Top - y - 1].val == 3 && pieces[fallingPiece.kind][fallingPiece.rotation][x][y] != 0)
            {
                stopPiece();
                return;
            }
        }
    }

    redrawPiece();

    return;
}

void drawSideLines()
{
    gfx_line(board[0][0].x, board[0][0].y + tileSize, board[0][TILES_NUMBER_Y - 1].x, board[0][TILES_NUMBER_Y - 1].y, SIDE_LINES_COLOR);
    gfx_line(board[TILES_NUMBER_X - 1][0].x + tileSize, board[0][0].y + tileSize,
             board[TILES_NUMBER_X - 1][TILES_NUMBER_Y - 1].x + tileSize, board[0][TILES_NUMBER_Y - 1].y, SIDE_LINES_COLOR);
}
void drawTiles()
{
    for (int x = 0; x < TILES_NUMBER_X; x++)
    {
        for (int y = 0; y < TILES_NUMBER_Y; y++)
        {
            struct tile currentTile = board[x][y];

            drawTile(currentTile.val, x, y, currentTile.x, currentTile.y);
        }
    }
    drawSideLines();

    return;
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
    calculateSmallGridPosition();

    srand(time(NULL));
    selectNextPiece();
    loadNewPiece();
    selectNextPiece();

    while (true)
    {
        int pressedKey = gfx_pollkey();
        if (pressedKey == SDLK_ESCAPE)
        {
            exit(3);
        }
        clearScreen();

        switch (gameState)
        {

        case IN_PROGRESS:
            drawTiles();
            updateFallingPiecePosition(pressedKey);
            displayFuturePiece();
            break;
        case END_SCREEN:
            endScreen(pressedKey);
            break;
        default:
            break;
        }

        gfx_updateScreen();
        SDL_Delay(10);
    }

    return 0;
}