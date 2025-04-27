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

bool isValidBase(const char *nPtr, char **endPtr, int base)
{
	if ((base < 2 || base > 36) && base != 0)
	{
		errno = EINVAL;
		if (endPtr != NULL)
		{
			*endPtr = (char *)nPtr;
		}
		return false;
	}
	return true;
}

void checkSpaces(const char *nPtr, int *counter, char *c)
{
	while (*c != '\0' && isspace(*c))
	{
		(*counter)++;
		*c = *(nPtr + *counter);
	}
}

bool checkSign(const char *nPtr, int *counter, char *c, int *signHelper, char **endPtr, long *result)
{
	if (*c == '-' && *(nPtr + *counter + 1) != '\0')
	{
		*signHelper = -1;
		(*counter)++;
		*c = *(nPtr + *counter);
	}
	else if (*c == '+' && *(nPtr + *counter + 1) != '\0')
	{
		*signHelper = 1;
		(*counter)++;
		*c = *(nPtr + *counter);
	}
	else if (*c == '+' || *c == '-')
	{
		if (endPtr != NULL)
		{
			*endPtr = (char *)nPtr;
		}
		*result = 0;
		return false;
	}
	return true;
}

bool checkBasePrefix(const char *nPtr, int *counter, char *c, int *base, char **endPtr, long *result)
{
	if (*base == 0)
	{
		if (*c == '0' && *(nPtr + *counter + 1) == 'x')
		{
			*base = 16;
			*counter += 2;
			*c = *(nPtr + *counter);
			if (endPtr != NULL && *c == '\0')
			{
				*endPtr = (char *)(nPtr + *counter - 1);
				return false;
			}
			if (toInt(*c) == -1 || toInt(*c) >= *base || *c == '\0')
			{
				if (endPtr != NULL)
				{
					*endPtr = (char *)(nPtr + *counter - 1);
				}
				return false;
			}
		}
		else if (*c == '0')
		{
			(*counter)++;
			*c = *(nPtr + *counter);
			*base = 8;

			if (endPtr != NULL && *c == '\0')
			{
				*endPtr = (char *)(nPtr + *counter);
				return false;
			}
			if (toInt(*c) == -1 || toInt(*c) >= *base)
			{
				if (endPtr != NULL)
				{
					*endPtr = (char *)(nPtr + *counter);
				}
				return false;
			}
		}
		else
		{
			*base = 10;
		}
	}
	else if (*base == 8)
	{
		if (*c == '0')
		{
			(*counter)++;
			*c = *(nPtr + *counter);
		}
		if (toInt(*c) == -1 || toInt(*c) >= *base)
		{
			if (endPtr != NULL)
			{
				*endPtr = (char *)(nPtr + *counter);
			}
			return false;
		}
	}
	else if (*base == 16)
	{
		if (*c == '0' && *(nPtr + *counter + 1) == 'x')
		{
			*counter += 2;
			*c = *(nPtr + *counter);
		}

		if (toInt(*c) == -1 || toInt(*c) >= *base || *c == '\0')
		{
			if (endPtr != NULL)
			{
				*endPtr = (char *)(nPtr + *counter - 1);
			}
			return false;
		}
	}

	return true;
}

long updateResult(long result, char c, int base, int signHelper)
{
	if (result >= 0 && (LONG_MAX - toInt(c)) / base < result)
	{
		errno = ERANGE;
		return LONG_MAX;
	}
	if (result < 0 && (LONG_MIN + toInt(c)) / base > result)
	{
		errno = ERANGE;
		return LONG_MIN;
	}
	return result * base + toInt(c) * signHelper;
}

long strtol(const char *nPtr, char **endPtr, int base)
{
	int counter = 0;
	char c = *nPtr;
	int signHelper = 1;
	long result = 0;
	bool ok = false;

	ok = isValidBase(nPtr, endPtr, base);
	if (!ok)
	{
		return result;
	}

	checkSpaces(nPtr, &counter, &c);

	// check for end of string
	if (c == '\0')
	{
		if (endPtr != NULL)
		{
			*endPtr = (char *)nPtr;
		}
		return result;
	}

	ok = checkSign(nPtr, &counter, &c, &signHelper, endPtr, &result);
	if (!ok)
	{
		return result;
	}

	ok = checkBasePrefix(nPtr, &counter, &c, &base, endPtr, &result);
	if (!ok)
	{
		return result;
	}

	if (toInt(c) == -1 || toInt(c) >= base || c == '\0')
	{
		if (endPtr != NULL)
		{
			*endPtr = (char *)nPtr;
		}
		return result;
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
		result = updateResult(result, c, base, signHelper);

		counter++;
		c = *(nPtr + counter);
	}

	if (endPtr != NULL)
	{
		*endPtr = (char *)(nPtr + counter);
	}

	return result;
}
