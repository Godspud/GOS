
/*
- strlen: Returns the length of a null-terminated string.
- strcmp: Compares two null-terminated strings and returns an integer less than, equal to, or greater than zero if the first string is found, respectively, to be less than, to be equal to, or be greater than the second string.
- strncmp: Compares up to n characters of two null-terminated strings and returns an integer less than, equal to, or greater than zero if the first string is found, respectively, to be less than, to be equal to, or be greater than the second string.
- memset: Fills a block of memory with a specified value.
- memcpy: Copies a block of memory from a source to a destination.
- str_copy: Copies a string from src to dest, ensuring that it does not exceed max characters and is null-terminated.
*/

int memcmp(const void *ptr1, const void *ptr2, unsigned int num)
{
    const unsigned char *s1 = (const unsigned char *)ptr1;
    const unsigned char *s2 = (const unsigned char *)ptr2;

    while (num > 0)
    {
        if (*s1 != *s2)
        {
            return (*s1 - *s2); // Returns positive or negative difference
        }
        s1++;
        s2++;
        num--;
    }

    return 0; // All bytes matched up to 'num'
}

/**
 * strlen: Returns the length of a null-terminated string.
 */
int strlen(const char *str)
{
    int len = 0;
    // while str[len] != null
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}
/**
 * strcmp: Compares two null-terminated strings and returns an integer less than, equal to, or greater than zero if the first
 * string is found, respectively, to be less than, to be equal to, or be greater than the second string.
 */
int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    // return the difference between the first non-matching characters, or 0 if they are equal
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}
/**
 * strncmp: Compares up to n characters of two null-terminated strings and returns an integer less than, equal to, or greater than zero if the first
 * string is found, respectively, to be less than, to be equal to, or be greater than the second string.
 */
int strncmp(const char *s1, const char *s2, int n)
{
    while (n && *s1 && (*s1 == *s2))
    {
        // if n is not 0 and s1 is not null and s1 and s2 are equal, continue comparing the next characters
        s1++;
        s2++;
        n--;
    }
    // if n is 0, the strings are equal up to n characters, so return 0
    if (n == 0)
        return 0;
    // return the difference between the first non-matching characters, or 0 if they are equal up to n characters
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}
// memset: Fills a block of memory with a specified value.
void memset(void *ptr, int value, int n)
{
    unsigned char *p = (unsigned char *)ptr;
    while (n--)
    {
        *p++ = (unsigned char)value;
    }
}
// memcpy: Copies a block of memory from a source to a destination.
void memcpy(void *dest, const void *src, int n)
{
    // copy byte by byte from src to dest
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    while (n--)
    {
        *d++ = *s++;
    }
}
/**
 * str_copy: Copies a string from src to dest, ensuring that it does not exceed max characters and is null-terminated.
 */
static void str_copy(char *dest, const char *src, int max)
{
    int i = 0;
    while (i < max - 1 && src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}
/**
 * str_split: Splits a string into two parts based on a delimiter, copying the part before the delimiter into before and the behind part into after and ensuring it is null-terminated.
 */
void strsplit(const char *src, char *before, char *after, char delimiter)
{
    int i = 0;

    // copy before delimiter
    while (src[i] != '\0' && src[i] != delimiter)
    {
        before[i] = src[i];
        i++;
    }
    before[i] = '\0';

    // if delimiter found, move past it
    if (src[i] == delimiter)
    {
        i++;
    }

    // copy after delimiter
    int j = 0;
    while (src[i] != '\0')
    {
        after[j++] = src[i++];
    }
    after[j] = '\0';
}
/**
 * strreplace: Copies a string from src to dest, replacing all occurrences of old_char with new_char, and ensuring the result is null-terminated
 */
void strreplace(const char *src, char *dest, char old_char, char new_char)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = (src[i] == old_char) ? new_char : src[i];
        i++;
    }
    dest[i] = '\0';
}
/**
 * strncpy: Copies a string from src to dest, ensuring that it does not exceed n characters and is null-terminated. If src is shorter than n, the remainder of dest will be filled with null characters.
 */
char *strncpy(char *dest, const char *src, unsigned int n)
{
    char *ptr = dest;

    // Copy characters from src to dest
    while (n > 0 && *src != '\0')
    {
        *ptr++ = *src++;
        n--;
    }

    // fill the rest with null characters
    while (n > 0)
    {
        *ptr++ = '\0';
        n--;
    }

    return dest;
}

/**
 * int_to_str: Converts an integer to a string representation in a specified base (e.g., base 10 for decimal). The resulting string is null-terminated.
 */
void int_to_str(int num_to_convert, char *str)
{
    int counter = 0;

    if (num_to_convert == 0)
    {
        str[counter++] = '0';
        str[counter] = '\0';
        return;
    }

    // Convert digits to characters (results in reverse order)
    while (num_to_convert > 0)
    {
        str[counter++] = (num_to_convert % 10) + '0';
        num_to_convert /= 10;
    }
    str[counter] = '\0';

    // Reverse the string in-place
    int start = 0;
    int end = counter - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}