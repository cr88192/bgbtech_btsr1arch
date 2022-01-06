#ifndef STD_DIRENT_H
#define STD_DIRENT_H

#include <stdint.h>
#include <sys/types.h>

typedef struct DIR_s DIR;

struct dirent {
ino_t		d_ino;
off_t		d_off;
uint16_t	d_reclen;
uint8_t		d_type;
char		d_name[256];
};

int	closedir(DIR *dir);
DIR	*opendir(const char *path);
struct dirent *readdir(DIR *dir);
int readdir_r(DIR * restrict dir,
	struct dirent * restrict entry,
	struct dirent ** restrict result);
void rewinddir(DIR *dir);
void seekdir(DIR *dir, long offs);
long telldir(DIR *dir);

#endif
