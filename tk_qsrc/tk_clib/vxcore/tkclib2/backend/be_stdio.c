#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <tkclib2/ci_stdio.h>

FILE c2be_fileAlloc(_stdio_context_t *ctx)
{
}

void c2be_fileFree(FILE *fd)
{
	fd->ctx->vt->free(fd->ctx, fd);
}

FILE *c2be_fopen(_stdio_context_t *ctx,
	const char *filename, const char *mode)
{
}

int c2be_remove(_stdio_context_t *ctx,
	const char *filename);
int c2be_rename(_stdio_context_t *ctx,
	const char *oldname, const char *newname);

FILE **c2be_get_stream_fd(_stdio_context_t *ctx, int fd)
{
	return(ctx->openfile+fd);
}


int c2be_fclose(FILE *stream)
{
	if(stream->tkfd)
		tk_fclose(stream->tkfd);

	stream->ctx->openfile[stream->ifd]=NULL;
	c2be_fileFree(stream);
}

int c2be_fflush(FILE *stream)
{
}

int c2be_feof(FILE *stream)
{
	return(stream->is_eof);
}

long c2be_ftell(FILE *stream)
{
	if(stream->tkfd)
		return(tk_ftell(stream->tkfd));

	return(-1);
}

int c2be_fseek(FILE *stream, long offset, int whence)
{
	if(stream->tkfd)
	{
		stream->is_eof=0;
		return(tk_fseek(stream->tkfd, offset, whence));
	}

	return(-1);
}

size_t c2be_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	int sz1, sz2;
	if(stream->tkfd)
	{
		sz1=size*nmemb;
		sz2=tk_fread(ptr, size, nmemb, stream->tkfd);
		if(sz1!=sz2)
			stream->is_eof=1;
		return(sz2);
	}
	return(-1);
}

size_t c2be_fwrite(
	const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	if(stream->tkfd)
		return(tk_fwrite(ptr, size, nmemb, stream->tkfd));
	return(-1);
}

int c2be_fgetc(FILE *stream)
{
	int c;

	if(stream->ugn>0)
	{
		c=stream->ugch&255;
		stream->ugch=stream->ugch>>8;
		stream->ugn--;
		return(c);
	}

	if(stream->tkfd)
	{
		c=tk_fgetc(stream->tkfd);
		if(c<0)
			stream->is_eof=1;
		return(c);
	}
	return(-1);
}

int c2be_fputc(int c, FILE *stream)
{
	if(stream->tkfd)
		return(tk_fputc(c, stream->tkfd));
	return(-1);
}

char *c2be_fgets(char *s, int n, FILE *stream)
{
	if(stream->tkfd)
		return(tk_fgets(s, n, stream->tkfd));
	return(-1);
}

int c2be_fputs(const char *s, FILE *stream)
{
	if(stream->tkfd)
		return(tk_fputs(s, stream->tkfd));
	return(-1);
}

int c2be_ungetc(int c, FILE *stream)
{
	stream->ugch=(stream->ugch<<8)|(c&255);
	stream->ugn++;
	return(0);
}

int c2be_ferror((FILE *stream)
{
}

void c2be_clearerr(FILE *stream)
{
}

int c2be_setvbuf(FILE *stream, char *buf, int mode, size_t size)
{
}

FILE *c2be_freopen(const char *filename, const char *mode, FILE *stream)
{
}
