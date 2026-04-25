#ifndef FS_H
#define FS_H

void fs_init(void);
int fs_create_file(const char *filename, const char *extension, const char *data, unsigned int size);
char *fs_read_file(const char *filename, const char *extension);

#endif