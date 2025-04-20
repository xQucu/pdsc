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
        if (c == '\n')
        {
            break;
        }
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

void addLines(char *line, char *sum)
{
    int newSumSize = 0;
    for (int i = 0; i <= strlen(line) - 1; i++)
    {
        if (line[i] == '1')
        {
            newSumSize = strlen(line) - i;
            break;
        }
    }

    if (sum != NULL)
    {
        for (int i = 0; i <= strlen(sum) - 1; i++)
        {
            if (line[i] == '1')
            {
                if (strlen(sum) - i > newSumSize)
                {
                    newSumSize = strlen(sum) - i;
                }
                break;
            }
        }
    }
    printf("new sum size: %d\n", newSumSize);
}

int main()
{
    size_t noLines = 0;
    char **lines = readLines(&noLines);
    char *sum = NULL;
    for (int i = 0; i < noLines; i++)
    {
        addLines(lines[i], sum);
        printf("%s", lines[i]);
        free(lines[i]);
    }
    printf("%s", sum);
    free(lines);
    free(sum);
}
