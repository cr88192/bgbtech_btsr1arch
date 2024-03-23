/*
VFILE implementing a FIFO pipe.
 */

int tk_pipe_fread(void *buf, int sz1, int sz2, TK_FILE *fd);
int tk_pipe_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd);
s64 tk_pipe_fseek(TK_FILE *fd, s64 ofs, int rel);
s64 tk_pipe_ftell(TK_FILE *fd);
int tk_pipe_fclose(TK_FILE *fd);
int tk_pipe_fgetc(TK_FILE *fd);
int tk_pipe_fputc(int ch, TK_FILE *fd);
int tk_pipe_fioctl(TK_FILE *fd, int cmd, void *ptr);

TK_FILE_VT tk_vfile_fat_vt={
"pipe",				//fsname
NULL,				//next
NULL,				//mount
NULL,				//fopen
NULL,				//fopendir
NULL,				//unlink
NULL,				//rename
NULL,				//fstat

NULL,				//mkdir
NULL,				//rmdir
NULL,				//fsctl

/* FILE Ops */
tk_pipe_fread,		//fread
tk_pipe_fwrite,		//fwrite
tk_pipe_fseek,		//fseek
tk_pipe_ftell,		//ftell
tk_pipe_fclose,		//fclose
tk_pipe_fgetc,		//fgetc
tk_pipe_fputc,		//fputc
tk_pipe_fioctl,		//ioctl

/* DIR ops */
NULL,				//readdir
NULL,				//closedir

/* Socket/Device Ops */
NULL,				//fsend
NULL				//frecv

};

int tk_pipe_fread(void *buf, int sz1, int sz2, TK_FILE *fd);
int tk_pipe_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd);

s64 tk_pipe_fseek(TK_FILE *fd, s64 ofs, int rel)
{
	return(0);
}

s64 tk_pipe_ftell(TK_FILE *fd)
{
	return(0);
}

int tk_pipe_fclose(TK_FILE *fd)
{
}

int tk_pipe_fgetc(TK_FILE *fd)
{
	byte ch;
	int i;
	
	i=tk_pipe_fread(&ch, 1, 1, fd);
	if(i!=1)
		return(-1);
	return(ch);
}

int tk_pipe_fputc(int ch, TK_FILE *fd)
{
	byte v;
	v=ch;
	return(tk_pipe_fwrite(&v, 1, 1, fd));
}

int tk_pipe_fioctl(TK_FILE *fd, int cmd, void *ptr)
{
	return(-1);
}
