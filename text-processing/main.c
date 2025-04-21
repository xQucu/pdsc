#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// char *getLines()
// {
//     char c;
//     char *lines = malloc(1);
//     if (lines == NULL)
//     {
//         printf("ERROR when allocating memory\n");
//         exit(EXIT_FAILURE);
//     }

//     while ((c = getchar()) != EOF)
//     {
//         if ((char)c == '\n')
//         {
//             printf("NEW LINE\n");
//             continue;
//         }

//         printf("The char is: %c\n", (char)c);
//     }

//     printf("EOF\n");
//     return NULL;
// }

char *getLine()
{
    char *line = NULL;
    int c;
    size_t bufSize = 0;
    size_t size = 0;

    while ((c = getchar()) != EOF)
    {
        if (c != '1' && c != '0' && c != '\n' && c != ' ')
        {
            printf("Input is incorrect\n");
            free(line);
            abort();
        }
        if (c == ' ' || (c == '\n' && size == 0))
        {
            continue;
        }
        if (c == '\n')
        {
            break;
        }
        if (size >= bufSize)
        {
            char *newBuf;
            if (bufSize == 0)
            {
                bufSize = 2;
            }
            else if (bufSize <= ((size_t)-1) / 2)
            {
                bufSize = 2 * size;
            }
            else
            {
                free(line);
                abort();
            }
            newBuf = realloc(line, bufSize);
            if (!newBuf)
            {
                free(line);
                abort();
            }
            line = newBuf;
        }
        line[size++] = c;
    }

    if ((c == EOF) && (size == 0))
    {
        return NULL;
    }
    if (size >= bufSize)
    {
        char *newBuf;
        if (size < ((size_t)-1))
        {
            bufSize = size + 1;
        }
        else
        {
            free(line);
            abort();
        }
        newBuf = realloc(line, bufSize);
        if (!newBuf)
        {
            free(line);
            abort();
        }
        line = newBuf;
    }
    line[size++] = '\0';
    return line;
}

char **readLines(size_t *noLinesPtr)
{
    char **lines = NULL;
    size_t noLines = 0;
    size_t noLinesMax = 0;
    char *line;
    size_t i;

    while ((line = getLine()))
    {
        if (noLines >= noLinesMax)
        {
            char **newLines;
            if (noLines == 0)
            {
                noLinesMax = 1;
            }
            else if (noLines <= ((size_t)-1) / 2 / sizeof(char *))
            {
                noLinesMax = 2 * noLines;
            }
            else
            {
                goto error;
            }
            newLines = realloc(lines, noLinesMax * sizeof(char *));
            if (newLines == NULL)
            {
                goto error;
            }
            lines = newLines;
        }
        lines[noLines++] = line;
    }

    // for (i = 0; i < noLines; i++)
    // {
    //     printf("%s", lines[i]);
    //     free(lines[i]);
    // }
    // free(lines);

    *noLinesPtr = noLines;
    return lines;

error:
    for (i = noLines; i > 0; i--)
    {
        free(lines[i - 1]);
        free(lines);
        abort();
    }
    return NULL;
}
int max(size_t a, size_t b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

void addLines(char *line, char **sum)
{
    int newSumSize = 0;
    for (int i = 0; i < strlen(line); i++)
    {
        if (line[i] == '1')
        {
            newSumSize = strlen(line) - i;
            break;
        }
    }

    if (*sum != NULL)
    {
        for (int i = 0; i < strlen(*sum); i++)
        {
            if ((*sum)[i] == '1')
            {
                newSumSize = strlen(*sum) - i;
                break;
            }
        }
    }

    printf("newSumSize: %d\n", newSumSize);

    int temp = 0;
    char *newSum = malloc((newSumSize + 1) * __SIZEOF_WCHAR_T__);
    if (newSum == NULL)
    {
        printf("Memory allocation failed\n");
        abort();
    }

    newSum[newSumSize + 1] = '\0';


    int lineLen = strlen(line)-1;
    int sumLen = strlen(*sum)-1;
    for (int i = newSumSize, j = lineLen, k = sumLen; i >= 0; i--, j--, k--)
    {
        char lineChar = j >= 0 ? line[j] : '0';
        char sumChar = k >= 0 ? (*sum)[k] : '0';

        printf("LineChar: %c, SumChar: %c\n", lineChar, sumChar);
        if (lineChar == '0' && sumChar == '0')
        {
            printf("RUNS1\n");
            if (temp != 0)
            {
                newSum[i] = '1';
                temp = 0;
            }
            else
            {
                newSum[i] = '0';
            }
        }
        if ((lineChar == '0' && sumChar == '1') || (lineChar == '1' && sumChar == '0'))
        {
            printf("RUNS2\n");
            if (temp != 0)
            {
                newSum[i] = '0';
            }
            else
            {
                newSum[i] = '1';
            }
        }
        if (lineChar == '1' && sumChar == '1')
        {
            printf("RUNS3\n");
            if (temp != 0)
            {
                newSum[i] = '1';
            }
            else
            {
                newSum[i] = '0';
                temp = 1;
            }
        }
    }
    if (temp == 1)
    {
        newSum[0] = '1';
    }

    free(*sum);
    *sum = newSum;
    // printf("Sum: %s\n", newSum);
}

int main()
{
    size_t noLines = 0;
    char **lines = readLines(&noLines);
    if (noLines == 0)
    {
        printf("empty input");
        abort();
    }
    char *sum = strdup(lines[0]);
    if (sum == NULL)
    {
        printf("Memory allocation failed\n");
        abort();
    }
    printf("%s\n", lines[0]);
    for (int i = 1; i < noLines; i++)
    {
        printf("%s\n", lines[i]);
        addLines(lines[i], &sum);
        free(lines[i]);
    }
    printf("sum: %s\n", sum);
    free(lines);
    free(sum);
}
