#include <dirent.h>

#if 1
int	closedir(DIR *dir)
{
	tk_closedir((TK_DIR *)dir);
}

DIR	*opendir(const char *path)
{
	return((DIR *)tk_opendir((char *)path));
}

struct dirent *readdir(DIR *dir)
{
	TK_DIR *dir1;
	TK_DIRENT *tde;
	struct dirent *de;
	
	dir1=(TK_DIR *)dir;
	tde=tk_readdir(dir1);
	if(!tde)
		return(NULL);

//	de=(struct dirent *)tde;

#if 1
	de=tk_ralloc(sizeof(struct dirent));
	de->d_ino=tde->d_ino;
	de->d_off=tde->d_off;
	de->d_reclen=tde->d_reclen;
	de->d_type=tde->d_type;
	strncpy(de->d_name, tde->d_name, 256);
#endif

	return(de);
}

void rewinddir(DIR *dir)
{
}

void seekdir(DIR *dir, long offs)
{
}

long telldir(DIR *dir)
{
}
#endif
