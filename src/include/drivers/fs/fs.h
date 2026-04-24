#ifndef FS_H
#define FS_H

void fs_init(void);
int fs_create_file(const char *filename, const char *extension, const void *data, unsigned int size);

#endif