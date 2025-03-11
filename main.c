#include "primlib.h"
#include <stdio.h>

int main()
{
    if (gfx_init())
    {
        exit(3);
    }
    printf("Hello world");
    return 0;
}