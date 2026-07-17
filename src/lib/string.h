#ifndef STRING_H
#define STRING_H

int memcmp(const void *ptr1, const void *ptr2, unsigned int num);
int strlen(const char *str);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, int n);
void memset(void *ptr, int value, int n);
void memcpy(void *dest, const void *src, int n);
void strsplit(const char *src, char *before, char *after, char delimiter);
void strreplace(const char *src, char *dest, char old_char, char new_char);
char *strncpy(char *dest, const char *src, unsigned int n);
void int_to_str(int num_to_convert, char *str);

#endif