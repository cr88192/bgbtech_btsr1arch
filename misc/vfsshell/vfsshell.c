#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "tk_vfile.h"
#include "tk_fatfs.h"
#include "tk_wadimg.h"

#include "tk_mmutil.c"
#include "tk_bdev.c"
#include "tk_lzunpack.c"

#include "tk_fat.c"
#include "tk_wad4.c"

#include "tk_vfile.c"

#if 0
typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#endif

#define BGBCC_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

void *LoadFile(char *name, int *rsz)
{
	FILE *fd;
	void *buf;
	int sz;
	
	fd=fopen(name, "rb");
	if(!fd)return(NULL);
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz+16);
	fread(buf, 1, sz, fd);
	fclose(fd);
	*rsz=sz;
	return(buf);
}

void StoreFile(char *name, void *buf, int sz)
{
	FILE *fd;
	
	fd=fopen(name, "wb");
	if(!fd)return;
	fwrite(buf, 1, sz, fd);
	fclose(fd);
}

int cmd_ls(char **args)
{
	char tb[256], tb2[256];
	char cwd[256];
	TK_DIR *dir;
	TK_DIRENT *de;
	char *path;
	int i, j, k;
	
	TK_Env_GetCwd(cwd, 255);
	
	path=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		if(!path)
			path=args[i];
	}
	
	if(path)
	{
		if(*path=='/')
		{
			strcpy(tb, path);
		}else
		{
			strcpy(tb, cwd);
			strcat(tb, "/");
			strcat(tb, path);			
		}
	}else
	{
		strcpy(tb, cwd);
	}
	
	TKSH_NormalizePath(tb2, tb);
	dir=tk_opendir(tb2);
	
	if(!dir)
	{
		printf("%s: Can't open dir %s\n", args[0], tb2);
		return(-1);
	}
	
	de=tk_readdir(dir);
	while(de)
	{
		printf("%s\n", de->d_name);
		de=tk_readdir(dir);
	}
	tk_closedir(dir);
	return(0);
}

int cmd_rm(char **args)
{
	char tb[256], tb2[256];
	char cwd[256];
	TK_DIR *dir;
	TK_DIRENT *de;
	char *path;
	int i, j, k;
	
	TK_Env_GetCwd(cwd, 255);
	
	path=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		if(!path)
			path=args[i];
	}
	
	if(path)
	{
		if(*path=='/')
		{
			strcpy(tb, path);
		}else
		{
			strcpy(tb, cwd);
			strcat(tb, "/");
			strcat(tb, path);			
		}
	}else
	{
		return(-1);
//		strcpy(tb, cwd);
	}
	
	TKSH_NormalizePath(tb2, tb);
//	dir=tk_opendir(tb2);

	tk_unlink(tb2);
	return(0);
}

int cmd_rmdir(char **args)
{
	char tb[256], tb2[256];
	char cwd[256];
	TK_DIR *dir;
	TK_DIRENT *de;
	char *path;
	int i, j, k;
	
	TK_Env_GetCwd(cwd, 255);
	
	path=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		if(!path)
			path=args[i];
	}
	
	if(path)
	{
		if(*path=='/')
		{
			strcpy(tb, path);
		}else
		{
			strcpy(tb, cwd);
			strcat(tb, "/");
			strcat(tb, path);			
		}
	}else
	{
		return(-1);
//		strcpy(tb, cwd);
	}
	
	TKSH_NormalizePath(tb2, tb);
//	dir=tk_opendir(tb2);

	tk_rmdir(tb2);
	return(0);
}

int cmd_mkdir(char **args)
{
	char tb[256], tb2[256];
	char cwd[256];
	TK_DIR *dir;
	TK_DIRENT *de;
	char *path;
	int i, j, k;
	
	TK_Env_GetCwd(cwd, 255);
	
	path=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		if(!path)
			path=args[i];
	}
	
	if(path)
	{
		if(*path=='/')
		{
			strcpy(tb, path);
		}else
		{
			strcpy(tb, cwd);
			strcat(tb, "/");
			strcat(tb, path);			
		}
	}else
	{
		return(-1);
//		strcpy(tb, cwd);
	}
	
	TKSH_NormalizePath(tb2, tb);
//	dir=tk_opendir(tb2);

	tk_mkdir(tb2, "");
	return(0);
}

int cmd_chdir(char **args)
{
	char tb[256], tb2[256];
	char cwd[256];
	TK_DIR *dir;
	TK_DIRENT *de;
	char *path;
	int i, j, k;
	
	TK_Env_GetCwd(cwd, 255);
	
	path=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		if(!path)
			path=args[i];
	}
	
	if(path)
	{
		if(*path=='/')
		{
			strcpy(tb, path);
		}else
		{
			strcpy(tb, cwd);
			strcat(tb, "/");
			strcat(tb, path);			
		}
	}else
	{
		strcpy(tb, "/");
	}
	
	TKSH_NormalizePath(tb2, tb);
	dir=tk_opendir(tb2);

	if(!dir)
	{
		printf("%s: Can't open dir %s\n", args[0], tb2);
		return(-1);
	}

	TK_Env_SetCwd(tb2);
	
	tk_closedir(dir);
	return(0);
}

int cmd_copyin(char **args)
{
	char tb[1024], tb2[256];
	char cwd[256];
//	TK_DIR *dir;
//	TK_DIRENT *de;
	TK_FILE *ifd;
	FILE *efd;
	
	char *path, *extpath;
	int i, j, k;
	
	TK_Env_GetCwd(cwd, 255);
	
	path=NULL;
	extpath=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}

		if(!extpath)
		{
			extpath=args[i];
			continue;
		}

		if(!path)
		{
			path=args[i];
			continue;
		}
	}
	
	if(extpath && !path)
		path=extpath;
	
	if(path)
	{
		if(*path=='/')
		{
			strcpy(tb, path);
		}else
		{
			strcpy(tb, cwd);
			strcat(tb, "/");
			strcat(tb, path);			
		}
	}else
	{
		strcpy(tb, "/");
	}
	
	TKSH_NormalizePath(tb2, tb);

	ifd=tk_fopen(tb2, "wb");
	efd=fopen(extpath, "rb");
	
	while(!feof(efd))
	{
		j=fread(tb, 1, 1024, efd);
		if(j!=1024)
		{
			if(j>0)
				{ tk_fwrite(tb, 1, j, ifd); }
			break;
		}
		tk_fwrite(tb, 1, 1024, ifd);
	}

	tk_fclose(ifd);
	fclose(efd);

	return(0);
}

int cmd_copyout(char **args)
{
	char tb[1024], tb2[256];
	char cwd[256];
//	TK_DIR *dir;
//	TK_DIRENT *de;
	TK_FILE *ifd;
	FILE *efd;
	
	char *path, *extpath;
	int i, j, k;
	
	TK_Env_GetCwd(cwd, 255);
	
	path=NULL;
	extpath=NULL;
	for(i=1; args[i]; i++)
	{
		if(args[i][0]=='-')
		{
			continue;
		}
		if(!path)
		{
			path=args[i];
			continue;
		}

		if(!extpath)
		{
			extpath=args[i];
			continue;
		}
	}
	
	if(path && !extpath)
		extpath=path;
	
	if(path)
	{
		if(*path=='/')
		{
			strcpy(tb, path);
		}else
		{
			strcpy(tb, cwd);
			strcat(tb, "/");
			strcat(tb, path);			
		}
	}else
	{
		strcpy(tb, "/");
	}
	
	TKSH_NormalizePath(tb2, tb);

	ifd=tk_fopen(tb2, "rb");
	efd=fopen(extpath, "wb");
	
	while(!feof(efd))
	{
		j=tk_fread(tb, 1, 1024, ifd);
		if(j!=1024)
		{
			if(j>0)
				{ fwrite(tb, 1, j, efd); }
			break;
		}
		fwrite(tb, 1, 1024, efd);
	}
	
	tk_fclose(ifd);
	fclose(efd);

	return(0);
}

int main(int argc, char *argv[])
{
	char tbuf[256];
	char cwd[256];
	char **a;
	int i;
	
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "--initsz"))
			{
				TKSPI_SetInitSizeString(argv[i+1]);
				i++;
				continue;
			}

			if(!strcmp(argv[i], "--sdimg"))
			{
				tkspi_sdimg_name=tk_strdup_in(argv[i+1]);
				i++;
				continue;
			}

			if(!strncmp(argv[i], "--initsz=", 9))
			{
				TKSPI_SetInitSizeString(argv[i]+9);
				continue;
			}

			if(!strncmp(argv[i], "--sdimg=", 8))
			{
				tkspi_sdimg_name=tk_strdup_in(argv[i]+8);
				continue;
			}
		}
	}


	tk_vfile_init();

	while(1)
	{
		TK_Env_GetCwd(cwd, 255);
	
		printf("%s$ ", cwd);
		fgets(tbuf, 256, stdin);
		a=tk_split(tbuf);
		
		if(!a || !a[0] || !(*a[0]))
			continue;

		if(!strcmp(a[0], "exit"))
			break;
		if(!strcmp(a[0], "quit"))
			break;

		if(!strcmp(a[0], "ls"))
		{
			cmd_ls(a);
			continue;
		}

		if(!strcmp(a[0], "rm"))
		{
			cmd_rm(a);
			continue;
		}

		if(!strcmp(a[0], "rmdir"))
		{
			cmd_rmdir(a);
			continue;
		}

		if(	!strcmp(a[0], "md") ||
			!strcmp(a[0], "mkdir"))
		{
			cmd_mkdir(a);
			continue;
		}

		if(	!strcmp(a[0], "cd") ||
			!strcmp(a[0], "chdir"))
		{
			cmd_chdir(a);
			continue;
		}

		if(	!strcmp(a[0], "copyin") ||
			!strcmp(a[0], "put"))
		{
			cmd_copyin(a);
			continue;
		}

		if(	!strcmp(a[0], "copyout") ||
			!strcmp(a[0], "get"))
		{
			cmd_copyout(a);
			continue;
		}
	}

	return(0);
}
