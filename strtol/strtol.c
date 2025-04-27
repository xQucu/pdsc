#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>
#include <limits.h>
#include <errno.h>

int toInt(char c)
{
	int number = 0;
	if (c >= 'a' && c <= 'z')
	{
		number = (int)c - 87;
	}
	else if (c >= 'A' && c <= 'Z')
	{
		number = (int)c - 55;
	}
	else if (c >= '0' && c <= '9')
	{
		number = (int)c - 48;
	}
	else
	{
		return -1;
	}
	return number;
}

long strtol(const char *nPtr, char **endPtr, int base)
{
	int counter = 0;
	char c = *nPtr;
	int signHelper = 1;
	long result = 0;

	if ((base < 2 || base > 36) && base != 0)
	{
		errno = EINVAL;
		if (endPtr != NULL)
		{
			*endPtr = (char *)nPtr;
		}
		return result;
	}

	// check spaces
	while (c != '\0' && isspace(c))
	{
		counter++;
		c = *(nPtr + counter);
	}
	if (c == '\0')
	{
		if (endPtr != NULL)
		{
			*endPtr = (char *)nPtr;
		}
		return result;
	}

	// check sign
	if (c == '-' && *(nPtr + counter + 1) != '\0')
	{
		signHelper = -1;
		counter++;
		c = *(nPtr + counter);
	}
	else if (c == '+' && *(nPtr + counter + 1) != '\0')
	{
		signHelper = 1;
		counter++;
		c = *(nPtr + counter);
	}
	else if (c == '+' || c == '-')
	{
		if (endPtr != NULL)
		{
			*endPtr = (char *)nPtr;
		}
		return result;
	}

	// check prefix
	if (base == 0)
	{
		if (c == '0' && *(nPtr + counter + 1) == 'x')
		{
			base = 16;
			counter += 2;
			c = *(nPtr + counter);
			if (endPtr != NULL && c == '\0')
			{
				*endPtr = (char *)(nPtr + counter - 1);
				return result;
			}
			if (toInt(c) == -1 || toInt(c) >= base || c == '\0')
			{
				*endPtr = (char *)(nPtr + counter - 1);
				return result;
			}
		}
		else if (c == '0')
		{
			counter++;
			c = *(nPtr + counter);
			base = 8;

			if (endPtr != NULL && c == '\0')
			{
				*endPtr = (char *)(nPtr + counter);
				return result;
			}
			if (toInt(c) == -1 || toInt(c) >= base)
			{
				*endPtr = (char *)(nPtr + counter);
				return result;
			}
		}
		else
		{
			base = 10;
		}
	}
	else if (base == 8)
	{
		if (c == '0')
		{
			counter++;
			c = *(nPtr + counter);
			base = 8;
		}
		if (toInt(c) == -1 || toInt(c) >= base)
		{
			if (endPtr != NULL)
			{
				*endPtr = (char *)(nPtr + counter);
			}
			return 0;
		}
	}
	else if (base == 16)
	{
		if (c == '0' && *(nPtr + counter + 1) == 'x')
		{
			counter += 2;
			c = *(nPtr + counter);
			base = 16;
		}

		if (toInt(c) == -1 || toInt(c) >= base || c == '\0')
		{
			if (endPtr != NULL)
			{
				*endPtr = (char *)(nPtr + counter - 1);
			}
			return result;
		}
	}

	if (toInt(c) == -1 || toInt(c) >= base || c == '\0')
	{
		if (endPtr != NULL)
		{
			*endPtr = (char *)nPtr;
		}
		return 0;
	}

	while (c != '\0')
	{
		if (toInt(c) == -1 || toInt(c) >= base)
		{
			if (endPtr != NULL)
			{
				*endPtr = (char *)(nPtr + counter);
			}
			return result;
		}
		if (result >= 0 && (LONG_MAX - toInt(c)) / base < result)
		{
			errno = ERANGE;
			result = LONG_MAX;
		}
		else if (result < 0 && (LONG_MIN + toInt(c)) / base > result)
		{
			errno = ERANGE;
			result = LONG_MIN;
		}
		else
		{
			result = result * base + toInt(c) * signHelper;
		}

		counter++;
		c = *(nPtr + counter);
	}

	if (endPtr != NULL)
	{
		*endPtr = (char *)(nPtr + counter);
	}
	return result;
}
