#include "string.h"

/*
- strlen: Returns the length of a null-terminated string.
- strcmp: Compares two null-terminated strings and returns an integer less than, equal to, or greater than zero if the first string is found, respectively, to be less than, to be equal to, or be greater than the second string.
- strncmp: Compares up to n characters of two null-terminated strings and returns an integer less than, equal to, or greater than zero if the first string is found, respectively, to be less than, to be equal to, or be greater than the second string.
- memset: Fills a block of memory with a specified value.
- memcpy: Copies a block of memory from a source to a destination.
- str_copy: Copies a string from src to dest, ensuring that it does not exceed max characters and is null-terminated.
*/
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
 * str_split: Splits a string into two parts based on a delimiter, copying the part before the delimiter into dest and ensuring it is null-terminated.
 */
void strsplit(const char *src, char *dest, char delimiter)
{
    int i = 0;
    while (src[i] != '\0' && src[i] != delimiter)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
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
