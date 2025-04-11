#include <stdio.h>
#include <math.h>

void *bsearch(const void *key, const void *base, size_t num, size_t size, int (*compar)(const void *, const void *))
{
    const char *lo = base;
    const char *hi = lo + size * num;
    do
    {
        const char *mid = (int)floor((hi - lo) / size / 2) * size + lo;
        if (compar(key, mid) < 0)
        {
            hi = mid;
        }
        else if (compar(key, mid) > 0)
        {
            lo = mid + size;
        }
        else
        {
            return (void *)mid;
        }

    } while (hi > lo);

    return NULL;
}

int compareInt(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int compareString(const void *a, const void *b)
{
    char firstLetter = *(char *)a;
    char secondLetter = *(char *)b;

    if (firstLetter < secondLetter)
    {
        return -1;
    }

    if (firstLetter > secondLetter)
    {
        return 1;
    }

    return 0;
}

int compareFloat(const void *a, const void *b)
{
    return *(float *)a - *(float *)b;
}

int main()
{
    int hayStack1[] = {4, 5, 25, 29, 32, 49, 49, 61, 65, 69, 72, 76, 83, 93, 97};
    int hayStack1Size = sizeof(hayStack1) / sizeof(hayStack1[0]);

    char hayStack2[] = {'b', 'e', 'e', 'g', 'i', 'k', 'p', 'q', 's', 't', 'u', 'v', 'v', 'v', 'w'};
    int hayStack2Size = sizeof(hayStack2) / sizeof(hayStack2[0]);

    float hayStack3[] = {25.63, 26.69, 26.75, 30.26, 37.48, 39.14, 56.72, 61.16, 62.96, 80.66, 83.84, 86.75, 92.19, 92.49, 95.62};
    int hayStack3Size = sizeof(hayStack3) / sizeof(hayStack3[0]);

    void *result1;
    void *result2;
    void *result3;

    int needle1 = 651;
    char needle2 = 'a';
    float needle3 = 11.26;

    result1 = bsearch(&needle1, hayStack1, hayStack1Size, sizeof(hayStack1[0]), compareInt);
    result2 = bsearch(&needle2, hayStack2, hayStack2Size, sizeof(hayStack2[0]), compareString);
    result3 = bsearch(&needle3, hayStack3, hayStack3Size, sizeof(hayStack3[0]), compareFloat);

    result1 != NULL ? printf("Value1: %p\n", result1) : printf("No match found\n");
    result2 != NULL ? printf("Value1: %p\n", result2) : printf("No match found\n");
    result3 != NULL ? printf("Value1: %p\n", result3) : printf("No match found\n");

    return 0;
}