/*********************************************************************/
/*																   */
/*  This Program Written by Paul Edwards.							*/
/*  Released to the Public Domain									*/
/*																   */
/*  Modifications by Dave Edwards, released to the Public Domain	 */
/*																   */
/*********************************************************************/
/*********************************************************************/
/*																   */
/*  stdio.c - implementation of stuff in stdio.h					 */
/*																   */
/*  The philosophy of this module is explained here.				 */
/*  There is a static array containing pointers to file objects.	 */
/*  This is required in order to close all the files on program	  */
/*  termination.													 */
/*																   */
/*  In order to give speed absolute priority, so that people don't   */
/*  resort to calling DosRead themselves, there is a special flag	*/
/*  in the FILE object called "quickbin".  If this flag is set to 1  */
/*  it means that it is a binary file and there is nothing in the	*/
/*  buffer and there are no errors, so don't stuff around, just call */
/*  DosRead.														 */
/*																   */
/*  When a buffer exists, which is most of the time, fbuf will point */
/*  to it.  The size of the buffer is given by szfbuf.  upto will	*/
/*  point to the next character to be read.  endbuf will point PAST  */
/*  the last valid character in the buffer.  bufStartR represents	*/
/*  the position in the file that the first character in the buffer  */
/*  is at.  This is only updated when a new buffer is read in.	   */
/*																   */
/*  After file open, for a file being read, bufStartR will actually  */
/*  be a negative number, which if added to the position of upto	 */
/*  will get to 0.  On a file being written, bufStartR will be set   */
/*  to 0, and upto will point to the start of the buffer.  The	   */
/*  reason for the difference on the read is in order to tell the	*/
/*  difference between an empty buffer and a buffer with data in it, */
/*  but which hasn't been used yet.  The alternative would be to	 */
/*  either keep track of a flag, or make fopen read in an initial	*/
/*  buffer.  But we want to avoid reading in data that no-one has	*/
/*  yet requested.												   */
/*																   */
/*  The buffer is organized as follows...							*/
/*  What we have is an internal buffer, which is 8 characters		*/
/*  longer than the actually used buffer.  E.g. say BUFSIZ is		*/
/*  512 bytes, then we actually allocate 520 bytes.  The first	   */
/*  2 characters will be junk, the next 2 characters set to NUL,	 */
/*  for protection against some backward-compares.  The fourth-last  */
/*  character is set to '\n', to protect against overscan.  The	  */
/*  last 3 characters will be junk, to protect against memory		*/
/*  violation.  intBuffer is the internal buffer, but everyone	   */
/*  refers to fbuf, which is actually set to the &intBuffer[4].	  */
/*  Also, szfbuf is the size of the "visible" buffer, not the		*/
/*  internal buffer.  The reason for the 2 junk characters at the	*/
/*  beginning is to align the buffer on a 4-byte boundary.		   */
/*																   */
/*																   */
/*********************************************************************/

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include "ctype.h"
#include "errno.h"
#include "float.h"
#include "limits.h"
#include "stddef.h"

// int __open(const char *a, int b, int c);
int __open(const char *a, int b, int *rc);

int __read(int handle, void *buf, size_t len, int *errind);
int __write(int handle, const void *buf, size_t len, int *errind);
void __seek(int handle, long offset, int whence);
void __close(int handle);
void __remove(const char *filename);
void __rename(const char *oldfn, const char *newfn);
long __tell(int handle);

// static FILE permFiles[3];
static FILE permFiles[3];

#define unused(x) ((void)(x))
// #define outch(ch) ((fq == NULL) ? *s++ = (char)ch : putc(ch, fq))
#define outch(ch) ((fq == NULL) ? ((s<dste) ? (*s++ = (char)ch) : 0) : putc(ch, fq))
#define inch() ((fp == NULL) ? \
	(ch = (unsigned char)*s++) : (ch = getc(fp)))

/* We need to choose whether we are doing move mode or
   locate mode */
#if !LOCMODE /* move mode */
#define begwrite(stream, len) (lenwrite = (len), dptr = (stream)->asmbuf)
#define finwrite(stream) (__awrite((stream)->hfile, NULL, lenwrite))
#else /* locate mode */
#define begwrite(stream, len) (__awrite((stream)->hfile, &dptr, len))
#define finwrite(stream)
#endif

void tk_printf(char *str, ...);


// FILE *__stdin = &permFiles[0];
// FILE *__stdout = &permFiles[1];
// FILE *__stderr = &permFiles[2];

FILE *__stdin;
FILE *__stdout;
FILE *__stderr;

FILE *__userFiles[__NFILE];
static FILE  *myfile;
static int	spareSpot;
static int	err;
static int	inreopen = 0;

static const char *fnm;
static const char *modus;
static int modeType;

void __init_stdin(void)
{
	__stdin = &permFiles[0];
	__stdout = &permFiles[1];
	__stderr = &permFiles[2];
}

__PDPCLIB_API__ FILE **__get_stdin()
	{ return(&__stdin); }
__PDPCLIB_API__ FILE **__get_stdout()
	{ return(&__stdout); }
__PDPCLIB_API__ FILE **__get_stderr()
	{ return(&__stderr); }

static void dblcvt(double num, char cnvtype, size_t nwidth,
				   int nprecision, char *result);
static int vvprintf(const char *format, va_list arg,
	FILE *fq, char *s, int nmax);
static int vvscanf(const char *format, va_list arg, FILE *fp, const char *s);
static void fopen2(void);
static void fopen3(void);
static void findSpareSpot(void);
static void checkMode(void);
static void osfopen(void);

#if !defined(__MVS__) && !defined(__CMS__)
static void fwriteSlow(const void *ptr,
					   size_t size,
					   size_t nmemb,
					   FILE *stream,
					   size_t towrite,
					   size_t *elemWritten);
static void fwriteSlowT(const void *ptr,
						FILE *stream,
						size_t towrite,
						size_t *actualWritten);
static void fwriteSlowB(const void *ptr,
						FILE *stream,
						size_t towrite,
						size_t *actualWritten);
static void freadSlowT(void *ptr,
					   FILE *stream,
					   size_t toread,
					   size_t *actualRead);
static void freadSlowB(void *ptr,
					   FILE *stream,
					   size_t toread,
					   size_t *actualRead);
#endif

static int examine(const char **formt, FILE *fq, char *s, va_list *arg,
				   int chcount, char *dste);


__PDPCLIB_API__ int printf(const char *format, ...)
{
	char tbuf[1024];
	va_list arg;
	int ret;

	va_start(arg, format);
//	ret = vfprintf(stdout, format, arg);
	ret = vsprintf(tbuf, format, arg);
	va_end(arg);
//	fflush(stdout);
	tk_puts(tbuf);

	return (ret);
}

__PDPCLIB_API__ int fprintf(FILE *stream, const char *format, ...)
{
	va_list arg;
	int ret;

	va_start(arg, format);
	ret = vfprintf(stream, format, arg);
	va_end(arg);
	return (ret);
}

__PDPCLIB_API__ int vfprintf(FILE *stream, const char *format, va_list arg)
{
	int ret;

	stream->quickText = 0;
	ret = vvprintf(format, arg, stream, NULL, 99999999);
	return (ret);
}

__PDPCLIB_API__ FILE *fopen(const char *filename, const char *mode)
{
	if((*filename=='$') && !mode)
	{
		if(!strcmp(filename, "$STDIN"))
			return(stdin);
		if(!strcmp(filename, "$STDOUT"))
			return(stdout);
		if(!strcmp(filename, "$STDERR"))
			return(stderr);
	}

	fnm = filename;
	modus = mode;
	err = 0;
	findSpareSpot();
	if (!err)
	{
		myfile = malloc(sizeof(FILE));
		if (myfile == NULL)
		{
			tk_puts("fopen: malloc fail\n");
			err = 1;
		}
		else
		{
			fopen2();
			if (err)
			{
				free(myfile);
			}
		}
	}
	if (err)
	{
		myfile = NULL;
	}
	return (myfile);
}

static void fopen2(void)
{
	checkMode();
	if (!err)
	{
		strcpy(myfile->modeStr, modus);
		osfopen();
		if (!err)
		{
			__userFiles[spareSpot] = myfile;
			myfile->intFno = spareSpot;
			fopen3();
		}else
		{
			tk_puts("fopen2: error status B\n");
		}
	}else
	{
		tk_puts("fopen2: error status A\n");
	}
	return;
}

static void fopen3(void)
{
	myfile->intBuffer = malloc(BUFSIZ + 8);
	if (myfile->intBuffer == NULL)
	{
		tk_puts("fopen3: intBuffer malloc fail\n");
		err = 1;
	}
	else
	{
		myfile->theirBuffer = 0;
		myfile->fbuf = myfile->intBuffer + 2;
		*myfile->fbuf++ = '\0';
		*myfile->fbuf++ = '\0';
		myfile->szfbuf = BUFSIZ;
		myfile->quickText = 0;
		myfile->noNl = 0;
		myfile->endbuf = myfile->fbuf + myfile->szfbuf;
		*myfile->endbuf = '\n';

		myfile->upto = myfile->endbuf;
		myfile->bufStartR = -(long)myfile->szfbuf;
		myfile->errorInd = 0;
		myfile->eofInd = 0;
		myfile->ungetCh = -1;
		myfile->update = 0;
		if (!inreopen)
		{
			myfile->permfile = 0;
		}
		myfile->isopen = 1;

		if (!myfile->textMode)
		{
			myfile->quickBin = 1;
		}
		else
		{
			myfile->quickBin = 0;
		}

		myfile->mode = __READ_MODE;
		switch (modeType)
		{
			case 2:
			case 3:
			case 5:
			case 6:
			case 8:
			case 9:
			case 11:
			case 12:
				myfile->bufStartR = 0;
				myfile->upto = myfile->fbuf;
				myfile->mode = __WRITE_MODE;
				break;
		}
		switch (modeType)
		{
			case 7:
			case 8:
			case 10:
			case 11:
			case 12:
				myfile->update = 1;
				break;
		}
	}
	return;
}

static void findSpareSpot(void)
{
	int x;

	for (x = 0; x < __NFILE; x++)
	{
		if (__userFiles[x] == NULL)
		{
			break;
		}
	}
	if (x == __NFILE)
	{
		err = 1;
		tk_puts("findSpareSpot: find spot fail\n");
	}
	else
	{
		spareSpot = x;
	}
	return;
}

/* checkMode - interpret mode string */
/* r = 1 */
/* w = 2 */
/* a = 3 */
/* rb = 4 */
/* wb = 5 */
/* ab = 6 */
/* r+ = 7 */
/* w+ = 8 */
/* a+ = 9 */
/* r+b or rb+ = 10 */
/* w+b or wb+ = 11 */
/* a+b or ab+ = 12 */

static void checkMode(void)
{
	if (strncmp(modus, "r+b", 3) == 0)
	{
		modeType = 10;
	}
	else if (strncmp(modus, "rb+", 3) == 0)
	{
		modeType = 10;
	}
	else if (strncmp(modus, "w+b", 3) == 0)
	{
		modeType = 11;
	}
	else if (strncmp(modus, "wb+", 3) == 0)
	{
		modeType = 11;
	}
	else if (strncmp(modus, "a+b", 3) == 0)
	{
		modeType = 12;
	}
	else if (strncmp(modus, "ab+", 3) == 0)
	{
		modeType = 12;
	}
	else if (strncmp(modus, "r+", 2) == 0)
	{
		modeType = 7;
	}
	else if (strncmp(modus, "w+", 2) == 0)
	{
		modeType = 8;
	}
	else if (strncmp(modus, "a+", 2) == 0)
	{
		modeType = 9;
	}
	else if (strncmp(modus, "rb", 2) == 0)
	{
		modeType = 4;
	}
	else if (strncmp(modus, "wb", 2) == 0)
	{
		modeType = 5;
	}
	else if (strncmp(modus, "ab", 2) == 0)
	{
		modeType = 6;
	}
	else if (strncmp(modus, "r", 1) == 0)
	{
		modeType = 1;
	}
	else if (strncmp(modus, "w", 1) == 0)
	{
		modeType = 2;
	}
	else if (strncmp(modus, "a", 1) == 0)
	{
		modeType = 3;
	}
	else
	{
		err = 1;
		return;
	}
	if ((modeType == 4)
		|| (modeType == 5)
		|| (modeType == 6)
		|| (modeType == 10)
		|| (modeType == 11)
		|| (modeType == 12))
	{
		myfile->textMode = 0;
	}
	else
	{
		myfile->textMode = 1;
	}

	myfile->textMode = 0;

	return;
}

static void osfopen(void)
{
	int mode;
	int errind;

//	if ((modeType == 1) || (modeType == 4))
//	{
//		mode = 0; /* read */
//	}
	if (modeType == 1)
	{
		mode = 0; /* read */
	}
	else if (modeType == 4)
	{
		mode = 0; /* read */
	}
	else if ((modeType == 2) || (modeType == 5))
	{
		mode = 1; /* write */
	}
//	else if (modeType == 10)
	else if ((modeType == 10) || (modeType == 7))
	{
		mode = 2; /* read+write */
	}
	else
	{
		mode = 2; /* update or otherwise unsupported */
		/* because we don't have the ability to update files
		   at the moment on MSDOS, just return with an
		   error immediately */
		err = 1;
		errno = 2;
//		tk_puts("osfopen: bad mode\n");
		tk_printf("osfopen: bad mode %d\n", modeType);
		return;
	}
	
	errind = 0;

//	myfile->hfile = __open(fnm, mode, (int)&errind);
//	myfile->hfile = __open(fnm, mode, (long)(&errind));
	myfile->hfile = __open(fnm, mode, &errind);
	if (errind)
	{
		err = 1;
		errno = myfile->hfile;
		tk_puts("osfopen: got error status\n");
	}
	return;
}

__PDPCLIB_API__ int fclose(FILE *stream)
{
	if(!stream)
		return(-1);

	if (!stream->isopen)
	{
		return (EOF);
	}
	fflush(stream);
	__close(stream->hfile);
	return (0);
}

__PDPCLIB_API__ size_t fread(void *ptr,
							 size_t size,
							 size_t nmemb,
							 FILE *stream)
{
	size_t toread;
	size_t elemRead;
	size_t actualRead;
	int errind;
	size_t tempRead;

	if(!stream)
		return(-1);

	if (nmemb == 1)
	{
		toread = size;
	}
	else if (size == 1)
	{
		toread = nmemb;
	}
	else
	{
		toread = size * nmemb;
	}
	if (toread < stream->szfbuf)
	{
		stream->quickBin = 0;
	}

//	if(!stream->textMode)
		stream->quickBin = 1;

	if (stream->ungetCh != -1)
	{
		*--stream->upto = (char)stream->ungetCh;
		stream->ungetCh = -1;
	}
	if (!stream->quickBin)
	{
		if (stream->textMode)
		{
			freadSlowT(ptr, stream, toread, &actualRead);
		}
		else
		{
			if (toread <= (stream->endbuf - stream->upto))
			{
				memcpy(ptr, stream->upto, toread);
				actualRead = toread;
				stream->upto += toread;
			}
			else
			{
				freadSlowB(ptr, stream, toread, &actualRead);
			}
		}
		if (nmemb == 1)
		{
			if (actualRead == size)
			{
				elemRead = 1;
			}
			else
			{
				elemRead = 0;
			}
		}
		else if (size == 1)
		{
			elemRead = actualRead;
		}
		else
		{
			if (size == 0)
			{
				elemRead = 0;
			}
			else
			{
				elemRead = actualRead / size;
			}
		}
		return (elemRead);
	}
	else
	{
		tempRead = __read(stream->hfile, ptr, toread, &errind);
		if (errind)
		{
			errno = tempRead;
			actualRead = 0;
			stream->errorInd = 1;
		}
		else
		{
			actualRead = tempRead;
		}

		if (nmemb == 1)
		{
			if (actualRead == size)
			{
				elemRead = 1;
			}
			else
			{
				elemRead = 0;
				stream->eofInd = 1;
			}
		}
		else if (size == 1)
		{
			elemRead = actualRead;
			if (nmemb != actualRead)
			{
				stream->eofInd = 1;
			}
		}
		else
		{
			if (size == 0)
			{
				elemRead = 0;
			}
			else
			{
				elemRead = actualRead / size;
			}
			if (toread != actualRead)
			{
				stream->eofInd = 1;
			}
		}
		stream->bufStartR += actualRead;
		return (elemRead);
	}
}


/*
while toread has not been satisfied
{
	scan stuff out of buffer, replenishing buffer as required
}
*/

static void freadSlowT(void *ptr,
					   FILE *stream,
					   size_t toread,
					   size_t *actualRead)
{
	int finReading = 0;
	size_t avail;
	size_t need;
	char *p;
	size_t got;
	size_t tempRead;
	int errind;

	*actualRead = 0;
	while (!finReading)
	{
		if (stream->upto == stream->endbuf)
		{
			tempRead = __read(stream->hfile,
							  stream->fbuf,
							  stream->szfbuf,
							  &errind);
			if (errind)
			{
				errno = tempRead;
				tempRead = 0;
				stream->errorInd = 1;
			}

			if (tempRead == 0)
			{
				stream->eofInd = 1;
				break;
			}
			stream->bufStartR += (stream->upto - stream->fbuf);
			stream->endbuf = stream->fbuf + tempRead;
			*stream->endbuf = '\n';
			stream->upto = stream->fbuf;
		}
		avail = (size_t)(stream->endbuf - stream->upto) + 1;
		need = toread - *actualRead;
		p = memchr(stream->upto, '\n', avail);
		got = (size_t)(p - stream->upto);
		if (need < got)
		{
			memcpy((char *)ptr + *actualRead, stream->upto, need);
			stream->upto += need;
			*actualRead += need;
		}
		else
		{
			memcpy((char *)ptr + *actualRead, stream->upto, got);
			stream->upto += got;
			*actualRead += got;
			if (p != stream->endbuf)
			{
				if (*(stream->upto - 1) == '\r')
				{
					*((char *)ptr + *actualRead - 1) = '\n';
					stream->upto++;
				}
				else if (need != got)
				{
					*((char *)ptr + *actualRead) = '\n';
					*actualRead += 1;
					stream->upto++;
				}
			}
			else
			{
				if (*(stream->upto - 1) == '\r')
				{
					*actualRead -= 1;
				}
			}
		}
		if (*actualRead == toread)
		{
			finReading = 1;
		}
	}
	return;
}

static void freadSlowB(void *ptr,
					   FILE *stream,
					   size_t toread,
					   size_t *actualRead)
{
	size_t avail;
	size_t tempRead;
	int errind;

	avail = (size_t)(stream->endbuf - stream->upto);
	memcpy(ptr, stream->upto, avail);
	*actualRead = avail;
	stream->bufStartR += (stream->endbuf - stream->fbuf);
	if (toread >= stream->szfbuf)
	{
		stream->upto = stream->endbuf;
		stream->quickBin = 1;

		tempRead = __read(stream->hfile,
						  (char *)ptr + *actualRead,
						  toread - *actualRead,
						  &errind);
		if (errind)
		{
			errno = tempRead;
			tempRead = 0;
			stream->errorInd = 1;
		}
		else if (tempRead != (toread - *actualRead))
		{
			stream->eofInd = 1;
		}
		*actualRead += tempRead;
		stream->bufStartR += tempRead;
	}
	else
	{
		size_t left;

		stream->upto = stream->fbuf;

		tempRead = __read(stream->hfile,
						  stream->fbuf,
						  stream->szfbuf,
						  &errind);
		left = toread - *actualRead;
		if (errind)
		{
			errno = tempRead;
			tempRead = 0;
			stream->errorInd = 1;
		}
		else if (tempRead < left)
		{
			stream->eofInd = 1;
		}
		stream->endbuf = stream->fbuf + tempRead;
		*stream->endbuf = '\n';
		avail = (size_t)(stream->endbuf - stream->upto);
		if (avail > left)
		{
			avail = left;
		}
		memcpy((char *)ptr + *actualRead,
			   stream->upto,
			   avail);
		stream->upto += avail;
		*actualRead += avail;
	}
	return;
}

__PDPCLIB_API__ size_t fwrite(const void *ptr,
							  size_t size,
							  size_t nmemb,
							  FILE *stream)
{
	size_t towrite;
	size_t elemWritten;
	size_t actualWritten;
	int errind;

	if(!stream)
		return(-1);

	if (nmemb == 1)
	{
		towrite = size;
	}
	else if (size == 1)
	{
		towrite = nmemb;
	}
	else
	{
		towrite = size * nmemb;
	}
	if (towrite < stream->szfbuf)
	{
		stream->quickBin = 0;
		if ((stream->bufTech == _IONBF) && !stream->textMode)
		{
			stream->quickBin = 1;
		}
	}
	if (!stream->quickBin)
	{
		fwriteSlow(ptr, size, nmemb, stream, towrite, &elemWritten);
		return (elemWritten);
	}
	else
	{
		actualWritten = __write(stream->hfile,
								ptr,
								towrite,
								&errind);
		if (errind)
		{
			stream->errorInd = 1;
			actualWritten = 0;
			errno = actualWritten;
		}

		if (nmemb == 1)
		{
			if (actualWritten == size)
			{
				elemWritten = 1;
			}
			else
			{
				elemWritten = 0;
			}
		}
		else if (size == 1)
		{
			elemWritten = actualWritten;
		}
		else
		{
			elemWritten = actualWritten / size;
		}
		stream->bufStartR += actualWritten;
		return (elemWritten);
	}
}

static void fwriteSlow(const void *ptr,
					   size_t size,
					   size_t nmemb,
					   FILE *stream,
					   size_t towrite,
					   size_t *elemWritten)
{
	size_t actualWritten;

	/* Normally, on output, there will never be a situation where
	   the write buffer is full, but it hasn't been written out.
	   If we find this to be the case, then it is because we have
	   done an fseek, and didn't know whether we were going to do
	   a read or a write after it, so now that we know, we switch
	   the buffer to being set up for write.  We could use a flag,
	   but I thought it would be better to just put some magic
	   code in with a comment */
	if (stream->upto == stream->endbuf)
	{
		stream->bufStartR += (stream->endbuf - stream->fbuf);
		stream->upto = stream->fbuf;
		stream->mode = __WRITE_MODE;
	}
	if ((stream->textMode) || (stream->bufTech == _IOLBF))
	{
		fwriteSlowT(ptr, stream, towrite, &actualWritten);
	}
	else
	{
		fwriteSlowB(ptr, stream, towrite, &actualWritten);
	}
	if (nmemb == 1)
	{
		if (actualWritten == size)
		{
			*elemWritten = 1;
		}
		else
		{
			*elemWritten = 0;
		}
	}
	else if (size == 1)
	{
		*elemWritten = actualWritten;
	}
	else
	{
		*elemWritten = actualWritten / size;
	}
	return;
}


/* can still be called on binary files, if the binary file is
   line buffered  */

static void fwriteSlowT(const void *ptr,
						FILE *stream,
						size_t towrite,
						size_t *actualWritten)
{
	char *p;
	char *tptr;
	char *oldp;
	size_t diffp;
	size_t rem;
	int fin;
	size_t tempWritten;
	int errind;

	*actualWritten = 0;
	tptr = (char *)ptr;
	p = tptr;
	oldp = p;
	p = (char *)memchr(oldp, '\n', towrite - (size_t)(oldp - tptr));
	while (p != NULL)
	{
		diffp = (size_t)(p - oldp);
		fin = 0;
		while (!fin)
		{
			rem = (size_t)(stream->endbuf - stream->upto);
			if (diffp < rem)
			{
				memcpy(stream->upto, oldp, diffp);
				stream->upto += diffp;
				*actualWritten += diffp;
				fin = 1;
			}
			else
			{
				memcpy(stream->upto, oldp, rem);
				oldp += rem;
				diffp -= rem;

				tempWritten = __write(stream->hfile,
									  stream->fbuf,
									  stream->szfbuf,
									  &errind);
				if (errind)
				{
					stream->errorInd = 1;
					return;
				}
				else
				{
					*actualWritten += rem;
					stream->upto = stream->fbuf;
					stream->bufStartR += tempWritten;
				}
			}
		}
		rem = (size_t)(stream->endbuf - stream->upto);
		if (rem < 3)
		{
			tempWritten = __write(stream->hfile,
								  stream->fbuf,
								  (size_t)(stream->upto - stream->fbuf),
								  &errind);
			if (errind)
			{
				stream->errorInd = 1;
				errno = tempWritten;
				return;
			}

			stream->upto = stream->fbuf;
			stream->bufStartR += tempWritten;
		}

		memcpy(stream->upto, "\n", 1);
		stream->upto += 1;

		*actualWritten += 1;
		oldp = p + 1;
		p = (char *)memchr(oldp, '\n', towrite - (size_t)(oldp - tptr));
	}

	if ((stream->bufTech == _IOLBF)
		&& (stream->upto != stream->fbuf)
		&& (oldp != tptr))
	{
		tempWritten = __write(stream->hfile,
							  stream->fbuf,
							  (size_t)(stream->upto - stream->fbuf),
							  &errind);
		if (errind)
		{
			stream->errorInd = 1;
			errno = tempWritten;
			return;
		}

		stream->upto = stream->fbuf;
		stream->bufStartR += tempWritten;
	}

	diffp = towrite - *actualWritten;
	while (diffp != 0)
	{
		rem = (size_t)(stream->endbuf - stream->upto);
		if (diffp < rem)
		{
			memcpy(stream->upto, oldp, diffp);
			stream->upto += diffp;
			*actualWritten += diffp;
		}
		else
		{
			memcpy(stream->upto, oldp, rem);

			tempWritten = __write(stream->hfile,
								  stream->fbuf,
								  stream->szfbuf,
								  &errind);
			if (errind)
			{
				stream->errorInd = 1;
				errno = tempWritten;
				return;
			}
			else
			{
				*actualWritten += rem;
				stream->upto = stream->fbuf;
			}
			stream->bufStartR += tempWritten;
			oldp += rem;
		}
		diffp = towrite - *actualWritten;
	}
	if ((stream->bufTech == _IONBF)
		&& (stream->upto != stream->fbuf))
	{
		tempWritten = __write(stream->hfile,
							  stream->fbuf,
							  (size_t)(stream->upto - stream->fbuf),
							  &errind);
		if (errind)
		{
			stream->errorInd = 1;
			errno = tempWritten;
			return;
		}

		stream->upto = stream->fbuf;
		stream->bufStartR += tempWritten;
	}
	return;
}

/* whilst write requests are smaller than a buffer, we do not turn
   on quickbin */

static void fwriteSlowB(const void *ptr,
						FILE *stream,
						size_t towrite,
						size_t *actualWritten)
{
	size_t spare;
	size_t tempWritten;
	int errind;

	spare = (size_t)(stream->endbuf - stream->upto);
	if (towrite < spare)
	{
		memcpy(stream->upto, ptr, towrite);
		*actualWritten = towrite;
		stream->upto += towrite;
		return;
	}
	memcpy(stream->upto, ptr, spare);

	tempWritten = __write(stream->hfile,
						  stream->fbuf,
						  stream->szfbuf,
						  &errind);
	if (errind)
	{
		stream->errorInd = 1;
		errno = tempWritten;
		return;
	}

	*actualWritten = spare;
	stream->upto = stream->fbuf;
	stream->bufStartR += tempWritten;
	if (towrite > stream->szfbuf)
	{
		stream->quickBin = 1;

		tempWritten = __write(stream->hfile,
							  (char *)ptr + *actualWritten,
							  towrite - *actualWritten,
							  &errind);
		if (errind)
		{
			stream->errorInd = 1;
			errno = tempWritten;
			return;
		}

		*actualWritten += tempWritten;
		stream->bufStartR += tempWritten;
	}
	else
	{
		memcpy(stream->fbuf,
			   (char *)ptr + *actualWritten,
			   towrite - *actualWritten);
		stream->upto += (towrite - *actualWritten);
		*actualWritten = towrite;
	}
	stream->bufStartR += *actualWritten;
	return;
}

static int vvprintf(const char *format, va_list arg,
	FILE *fq, char *s, int nmax)
{
	int fin;
//	int vint;
	long long vint;
	double vdbl;
//	unsigned int uvint;
	unsigned long long uvint;
	const char *vcptr;
	int chcount;
	size_t len;
	char numbuf[50];
	char pad, ljust, llong;
	int width, prec;
	char *nptr, *dste;
	int *viptr;

	dste=s+nmax;

	fin=0;
	chcount=0;

	while (!fin)
	{
		if (*format == '\0')
		{
			fin = 1;
		}
		else if (*format == '%')
		{
			format++;
			pad=' ';

#if 0
			ljust=0;
			llong=0;
			width=0;
			prec=0;
			
			if(*format=='0')
				{ pad='0'; format++; }
			if(*format==' ')
				{ pad=' '; format++; }
			if(*format=='-')
				{ ljust|=1; format++; }
			if(*format=='+')
				{ ljust|=2; format++; }
			
			while((*format>='0') && (*format<='9'))
				{ width=(width*10)+((*format++)-'0'); }

			if(*format=='*')
			{
				format++;
				width = va_arg(arg, int);
			}

			if(*format=='.')
			{
				format++;
				while((*format>='0') && (*format<='9'))
					{ prec = (prec*10)+((*format++)-'0'); }

				if(*format=='*')
				{
					format++;
					prec = va_arg(arg, int);
				}
			}
			
			if(*format=='l')
			{
				format++;
				if(*format=='l')
					format++;
				llong=1;
			}
#endif
			
//			if (*format == 'd')
			if ((*format == 'd') || (*format == 'i'))
			{
				if(llong)
				{
					vint = va_arg(arg, long long);
				}else
				{
					vint = va_arg(arg, int);
				}

				if (vint < 0)
					{ uvint = -vint; }
				else
					{ uvint = vint; }

				nptr = numbuf;
				do
				{
					*nptr++ = (char)('0' + uvint % 10);
					uvint /= 10;
				} while (uvint > 0);

				if (vint < 0)
					{ *nptr++ = '-'; }
//				else if(ljust&2)
//					{ *nptr++ = '+'; }

				do
				{
					nptr--;
					outch(*nptr);
					chcount++;
				} while (nptr != numbuf);

			}
			else if (*format == 's')
			{
				vcptr = va_arg(arg, const char *);
//				if (vcptr == NULL)
				if(!vcptr)
				{
					vcptr = "(null)";
				}

//				if (fq == NULL)
				if(!fq)
				{
					len = strlen(vcptr);
					if((s+len)<dste)
						memcpy(s, vcptr, len);
					s += len;
					chcount += len;
				}
				else
				{
					fputs(vcptr, fq);
					chcount += strlen(vcptr);
				}
			}
			else if (*format == 'c')
			{
				vint = va_arg(arg, int);
				outch(vint);
				chcount++;
			}
			else if (*format == 'n')
			{
				viptr = va_arg(arg, int *);
				*viptr = chcount;
			}
			else if (*format == '%')
			{
				outch('%');
				chcount++;
			}
			else if (strchr("eEgGfF", *format) != NULL && *format != 0)
			{

				vdbl = va_arg(arg, double);
				dblcvt(vdbl, *format, 0, 6, numbuf);   /* 'e','f' etc. */
				len = strlen(numbuf);

				if (fq == NULL)
				{
					if((s+len)<dste)
						memcpy(s, numbuf, len);
					s += len;
				}
				else
				{
					fputs(numbuf, fq);
				}
				chcount += len;
			}
			else
			{
				int extraCh;

				extraCh = examine(&format, fq, s, &arg, chcount, dste);
				chcount += extraCh;
				if (s != NULL)
				{
					s += extraCh;
				}
			}
		}
		else
		{
			outch(*format);
			chcount++;
		}
		format++;
	}

	return (chcount);
}

static int examine(const char **formt, FILE *fq, char *s, va_list *arg,
				   int chcount, char *dste)
{
	int extraCh = 0;
	int flagMinus = 0;
	int flagPlus = 0;
	int flagSpace = 0;
	int flagHash = 0;
	int flagZero = 0;
	int width = 0;
	int precision = -1;
	int half = 0;
	int lng = 0;
	int llng = 0;
	int xlng = 0;
	int specifier = 0;
	int fin;
#if defined(__BJX2__)
// #if 0
	__int128 lvalue;
	__uint128 ulvalue;
	unsigned long long ui64value;
#else
//	long lvalue;
//	unsigned long ulvalue;
	long long lvalue;
	unsigned long long ulvalue;
	unsigned long long ui64value;
#endif
	short int hvalue;
	int ivalue;
	double vdbl;
	char *svalue;
	char work[50];
	int x;
	int y;
	int rem;
	const char *format;
	int base;
	int fillCh;
	int neg, isunsgn;
	int length;
	size_t slen;
	int i, n;

#if 0
	extraCh = 0;
	flagMinus = 0;
	flagPlus = 0;
	flagSpace = 0;
	flagHash = 0;
	flagZero = 0;
	width = 0;
	precision = -1;
	half = 0;
	lng = 0;
	specifier = 0;
#endif

	unused(chcount);
	format = *formt;
	/* processing flags */
	fin = 0;
	while (!fin)
	{
		switch (*format)
		{
			case '-': flagMinus = 1;
					  break;
			case '+': flagPlus = 1;
					  break;
			case ' ': flagSpace = 1;
					  break;
			case '#': flagHash = 1;
					  break;
			case '0': flagZero = 1;
					  break;
			case '*': width = va_arg(*arg, int);
					  if (width < 0)
					  {
						  flagMinus = 1;
						  width = -width;
					  }
					  break;
			default:  fin = 1;
					  break;
		}
		if (!fin)
		{
			format++;
		}
		else
		{
			if (flagSpace && flagPlus)
			{
				flagSpace = 0;
			}
			if (flagMinus)
			{
				flagZero = 0;
			}
		}
	}

	/* processing width */
	if (isdigit((unsigned char)*format))
	{
		while (isdigit((unsigned char)*format))
		{
			width = width * 10 + (*format - '0');
			format++;
		}
	}

	/* processing precision */
	if (*format == '.')
	{
		format++;
		if (*format == '*')
		{
			precision = va_arg(*arg, int);
			format++;
		}
		else
		{
			precision = 0;
			while (isdigit((unsigned char)*format))
			{
				precision = precision * 10 + (*format - '0');
				format++;
			}
		}
	}

	/* processing h/l/L */
	if (*format == 'h')
	{
		/* all environments should promote shorts to ints,
		   so we should be able to ignore the 'h' specifier.
		   It will create problems otherwise. */
		/* half = 1; */
	}
	else if (*format == 'l')
	{
		lng = 1;
		if(format[1]=='l')
		{
			llng = 1;
			format++;
		}
	}
	else if (*format == 't')
	{
		lng = 1;
	}
	else if (*format == 'L')
//	else if ((*format == 'L') || (*format == 't'))
	{
		lng = 1;
	}
	else if (*format == 'j')
	{
		xlng = 1;
	}
	else if ((*format == 'I') && (format[1]>='0') && (format[1]<='9'))
	{
//		xlng = 1;
		format++;
		i=0;
		while((*format>='0') && (*format<='9'))
			i=(i*10)+((*format++)-'0');
		format--;
		
		if((i>32) && (i<=64))
			{ llng = 1; }
		else if((i>64) && (i<=128))
			{ xlng = 1; }
	}
	else
	{
		format--;
	}
	format++;

	/* processing specifier */
	specifier = *format;

	if (strchr("dxXuiop", specifier) != NULL && specifier != 0)
	{
		if (precision < 0)
		{
			precision = 1;
		}

		isunsgn = 0;
		if (	(specifier == 'u') ||
				(specifier == 'p') ||
				(specifier == 'x') ||
				(specifier == 'X')	)
			isunsgn = 1;

#if defined(__MSDOS__) && !defined(__PDOS__) && !defined(__gnu_linux__)
		if (specifier == 'p')
		{
			lng = 1;
		}
#endif

#if defined(__BJX2__)
		if (specifier == 'p')
		{
			lng = 1;
		}
#endif

#if defined(__BJX2__)
		if (xlng)
		{
			lvalue = va_arg(*arg, __int128);
		}
		else
			if (llng)
		{
			lvalue = va_arg(*arg, long long);

//			__debugbreak();

//			if (	(specifier == 'u') ||
//					(specifier == 'x') ||
//					(specifier == 'X')	)
			if(isunsgn)
				lvalue = (unsigned long long)lvalue;
		}else
#endif
			if (lng)
		{
			lvalue = va_arg(*arg, long);

//			if (	(specifier == 'u') ||
//					(specifier == 'x') ||
//					(specifier == 'X')	)
			if(isunsgn)
				lvalue = (unsigned long)lvalue;
		}
		else if (half)
		{
/*			hvalue = va_arg(*arg, short); */  /* BGB: gcc warnings */
			hvalue = va_arg(*arg, int);
//			if (specifier == 'u')
//			if (	(specifier == 'u') ||
//					(specifier == 'x') ||
//					(specifier == 'X')	)
			if(isunsgn)
				lvalue = (unsigned short)hvalue;
			else
				lvalue = hvalue;
		}
		else
		{
			ivalue = va_arg(*arg, int);
//			if (specifier == 'u')
//			if (	(specifier == 'u') ||
//					(specifier == 'x') ||
//					(specifier == 'X')	)
			if(isunsgn)
				lvalue = (unsigned int)ivalue;
			else
				lvalue = ivalue;
		}
//		ulvalue = (unsigned long)lvalue;
		ulvalue = lvalue;
//		if ((lvalue < 0) && ((specifier == 'd') || (specifier == 'i')))
		if ((lvalue < 0) && !isunsgn)
		{
			neg = 1;
			ulvalue = -lvalue;
		}
		else
		{
			neg = 0;
		}
#if defined(__MSDOS__) && !defined(__PDOS__) && !defined(__gnu_linux__)
		if (!lng)
		{
			ulvalue &= 0xffff;
		}
#endif
		if ((specifier == 'X') || (specifier == 'x') || (specifier == 'p'))
		{
			base = 16;
		}
		else if (specifier == 'o')
		{
			base = 8;
		}
		else
		{
			base = 10;
		}
		if (specifier == 'p')
		{
#if defined(__MSDOS__) && !defined(__PDOS__) && !defined(__gnu_linux__)
			precision = 9;
#else
			precision = 8;
#endif

#if defined(__BJX2__)
			precision = 12;
#endif

		}
		x = 0;
		n = 64;
//		while (ulvalue > 0)
		while ((ulvalue > 0) && ((n--)>0))
		{
			if(base == 16)
			{
				rem = (int)(ulvalue & 15);
				ulvalue = ulvalue >> 4;
			}else
			{
				rem = (int)(ulvalue % base);
				ulvalue = ulvalue / base;
			}

			if (rem < 10)
			{
				work[x] = (char)('0' + rem);
			}
			else
			{
				if ((specifier == 'X') || (specifier == 'p'))
				{
					work[x] = (char)('A' + (rem - 10));
				}
				else
				{
					work[x] = (char)('a' + (rem - 10));
				}
			}
			x++;
#if defined(__MSDOS__) && !defined(__PDOS__) && !defined(__gnu_linux__)
			if ((x == 4) && (specifier == 'p'))
			{
				work[x] = ':';
				x++;
			}
#endif
		}
#if defined(__MSDOS__) && !defined(__PDOS__) && !defined(__gnu_linux__)
		if (specifier == 'p')
		{
			while (x < 5)
			{
				work[x] = (x == 4) ? ':' : '0';
				x++;
			}
		}
#endif
		while (x < precision)
		{
			work[x] = '0';
			x++;
		}
		if (neg)
		{
			work[x++] = '-';
		}
		else if (flagPlus)
		{
			work[x++] = '+';
		}
		if (flagZero)
		{
			fillCh = '0';
		}
		else
		{
			fillCh = ' ';
		}
		y = x;
		if (!flagMinus)
		{
			while (y < width)
			{
				outch(fillCh);
				extraCh++;
				y++;
			}
		}
		if (flagHash && (toupper((unsigned char)specifier) == 'X'))
		{
			outch('0');
			outch('x');
			extraCh += 2;
		}
		x--;
		while (x >= 0)
		{
			outch(work[x]);
			extraCh++;
			x--;
		}
		if (flagMinus)
		{
			while (y < width)
			{
				outch(fillCh);
				extraCh++;
				y++;
			}
		}
	}
	else if (strchr("eEgGfF", specifier) != NULL && specifier != 0)
	{
		if (precision < 0)
		{
			precision = 6;
		}
		if(lng || xlng)
		{
			vdbl = va_arg(*arg, long double);
		}else
		{
			vdbl = va_arg(*arg, double);
		}
		dblcvt(vdbl, specifier, width, precision, work);   /* 'e','f' etc. */
		slen = strlen(work);
		if (fq == NULL)
		{
			memcpy(s, work, slen);
			s += slen;
		}
		else
		{
			fputs(work, fq);
		}
		extraCh += slen;
	}
	else if (specifier == 's')
	{
		if (precision < 0)
		{
			precision = 1;
		}
		svalue = va_arg(*arg, char *);
		fillCh = ' ';
		if (precision > 1)
		{
			char *p;

			p = memchr(svalue, '\0', precision);
			if (p != NULL)
			{
				length = (int)(p - svalue);
			}
			else
			{
				length = precision;
			}
		}
		else
		{
			length = strlen(svalue);
		}
		if (!flagMinus)
		{
			if (length < width)
			{
				extraCh += (width - length);
				for (x = 0; x < (width - length); x++)
				{
					outch(fillCh);
				}
			}
		}
		for (x = 0; x < length; x++)
		{
			outch(svalue[x]);
		}
		extraCh += length;
		if (flagMinus)
		{
			if (length < width)
			{
				extraCh += (width - length);
				for (x = 0; x < (width - length); x++)
				{
					outch(fillCh);
				}
			}
		}
	}
	else if ((specifier == 'v') || (specifier == 'V'))
	{
		if (precision < 0)
		{
			precision = 1;
		}

		ui64value = va_arg(*arg, unsigned long long);
		svalue = __lva_conv_tostring(ui64value);

		fillCh = ' ';
		if (precision > 1)
		{
			char *p;

			p = memchr(svalue, '\0', precision);
			if (p != NULL)
			{
				length = (int)(p - svalue);
			}
			else
			{
				length = precision;
			}
		}
		else
		{
			length = strlen(svalue);
		}
		if (!flagMinus)
		{
			if (length < width)
			{
				extraCh += (width - length);
				for (x = 0; x < (width - length); x++)
				{
					outch(fillCh);
				}
			}
		}
		for (x = 0; x < length; x++)
		{
			outch(svalue[x]);
		}
		extraCh += length;
		if (flagMinus)
		{
			if (length < width)
			{
				extraCh += (width - length);
				for (x = 0; x < (width - length); x++)
				{
					outch(fillCh);
				}
			}
		}
	}
	*formt = format;
	return (extraCh);
}

__PDPCLIB_API__ int fputc(int c, FILE *stream)
{
	char buf[1];

#if !defined(__MVS__) && !defined(__CMS__)
	stream->quickBin = 0;
	if ((stream->upto < (stream->endbuf - 2))
		&& (stream->bufTech != _IONBF))
	{
		if (stream->textMode)
		{
			if (c == '\n')
			{
				if (stream->bufTech == _IOFBF)
				{
					*stream->upto++ = '\r';
					*stream->upto++ = '\n';
				}
				else
				{
					buf[0] = (char)c;
					if (fwrite(buf, 1, 1, stream) != 1)
					{
						return (EOF);
					}
				}
			}
			else
			{
				*stream->upto++ = (char)c;
			}
		}
		else
		{
			*stream->upto++ = (char)c;
		}
	}
	else
#endif
	{
		buf[0] = (char)c;
		if (fwrite(buf, 1, 1, stream) != 1)
		{
			return (EOF);
		}
	}
	return (c);
}

#if !defined(__MVS__) && !defined(__CMS__)
__PDPCLIB_API__ int fputs(const char *s, FILE *stream)
{
	size_t len;
	size_t ret;

	len = strlen(s);
	ret = fwrite(s, len, 1, stream);
	if (ret != 1) return (EOF);
	else return (0);
}
#endif

__PDPCLIB_API__ int remove(const char *filename)
{
	int ret;
	__remove(filename);
	ret = 0;
	return (ret);
}

__PDPCLIB_API__ int rename(const char *oldfn, const char *newfn)
{
	int ret;
	__rename(oldfn, newfn);
	ret = 0;
	return (ret);
}

__PDPCLIB_API__ int sprintf(char *s, const char *format, ...)
{
	va_list arg;
	int ret;

	va_start(arg, format);
	ret = vsprintf(s, format, arg);
	va_end(arg);
	return (ret);
}

__PDPCLIB_API__ int vsprintf(char *s, const char *format, va_list arg)
{
	int ret;

	ret = vvprintf(format, arg, NULL, s, 99999999);
	if (ret >= 0)
	{
		*(s + ret) = '\0';
	}
	return (ret);
}


int vsnprintf(char *s, size_t n, const char *format, va_list arg)
{
	int ret;

	ret = vvprintf(format, arg, NULL, s, n);
	if (ret >= 0)
	{
		*(s + ret) = '\0';
	}
	return (ret);
}

int snprintf(char *s, size_t n, const char *format, ...)
{
	va_list arg;
	int ret;

	va_start(arg, format);
	ret = vsnprintf(s, n, format, arg);
	va_end(arg);
	return (ret);
}


/*

In fgets, we have the following possibilites...

1. we found a genuine '\n' that terminated the search.
2. we hit the '\n' at the endbuf.
3. we hit the '\n' sentinel.

*/
#if !defined(__MVS__) && !defined(__CMS__)
__PDPCLIB_API__ char *fgets(char *s, int n, FILE *stream)
{
	char *p;
	register char *t;
	register char *u = s;
	int c;
	int processed;
	size_t actualRead;
	int errind;

	if (stream->quickText)
	{
		p = stream->upto + n - 1;
		t = stream->upto;
		if (p < stream->endbuf)
		{
			c = *p;
			*p = '\n';
				while ((*u++ = *t++) != '\n') ; /* tight inner loop */

			*p = (char)c;
			if (t <= p)
			{
				if (*(t - 2) == '\r') /* t is protected, u isn't */
				{
					*(u - 2) = '\n';
					*(u - 1) = '\0';
				}
				else
				{
					*u = '\0';
				}
				stream->upto = t;
				return (s);
			}
			else
			{
				processed = (int)(t - stream->upto) - 1;
				stream->upto = t - 1;
				u--;
			}
		}
		else
		{
			while ((*u++ = *t++) != '\n') ; /* tight inner loop */
			if (t <= stream->endbuf)
			{
				if (*(t - 2) == '\r') /* t is protected, u isn't */
				{
					*(u - 2) = '\n';
					*(u - 1) = '\0';
				}
				else
				{
					*u = '\0';
				}
				stream->upto = t;
				return (s);
			}
			else
			{
				processed = (int)(t - stream->upto) - 1;
				stream->upto = t - 1;
				u--;
			}
		}
	}
	else
	{
		processed = 0;
	}

	if (n < 1)
	{
		return (NULL);
	}
	if (n < 2)
	{
		*u = '\0';
		return (s);
	}
	if (stream->ungetCh != -1)
	{
		processed++;
		*u++ = (char)stream->ungetCh;
		stream->ungetCh = -1;
	}
	while (1)
	{
		t = stream->upto;
		p = stream->upto + (n - processed) - 1;
		if (p < stream->endbuf)
		{
			c = *p;
			*p = '\n';
		}
		if (stream->noNl)
		{
			while (((*u++ = *t) != '\n') && (*t++ != '\r')) ;
			if (*(u - 1) == '\n')
			{
				t++;
			}
			else
			{
				u--;
				while ((*u++ = *t++) != '\n') ;
			}
		}
		else
		{
			while ((*u++ = *t++) != '\n') ; /* tight inner loop */
		}
		if (p < stream->endbuf)
		{
			*p = (char)c;
		}
		if (((t <= p) && (p < stream->endbuf))
		   || ((t <= stream->endbuf) && (p >= stream->endbuf)))
		{
			if (stream->textMode)
			{
				if (stream->noNl)
				{
					if ((*(t - 1) == '\r') || (*(t - 1) == '\n'))
					{
						*(u - 1) = '\0';
					}
					else
					{
						*u = '\0';
					}
				}
				else if (*(t - 2) == '\r') /* t is protected, u isn't */
				{
					*(u - 2) = '\n';
					*(u - 1) = '\0';
				}
				else
				{
					*u = '\0';
				}
			}
			stream->upto = t;
			if (stream->textMode)
			{
				stream->quickText = 1;
			}
			return (s);
		}
		else if (((t > p) && (p < stream->endbuf))
				 || ((t > stream->endbuf) && (p >= stream->endbuf)))
		{
			int leave = 1;

			if (stream->textMode)
			{
				if (t > stream->endbuf)
				{
					if ((t - stream->upto) > 1)
					{
						if (*(t - 2) == '\r') /* t is protected, u isn't */
						{
							processed -= 1; /* preparation for add */
						}
					}
					leave = 0;
				}
				else
				{
					if ((*(t - 2) == '\r') && (*(t - 1) == '\n'))
					{
						*(u - 2) = '\n';
						*(u - 1) = '\0';
					}
					else
					{
						t--;
						*(u - 1) = '\0';
					}
				}
			}
			else if (t > stream->endbuf)
			{
				leave = 0;
			}
			else
			{
				*u = '\0';
			}
			if (leave)
			{
				stream->upto = t;
				if (stream->textMode)
				{
					stream->quickText = 1;
				}
				return (s);
			}
		}
		processed += (int)(t - stream->upto) - 1;
		u--;
		stream->bufStartR += (stream->endbuf - stream->fbuf);

		actualRead = __read(stream->hfile,
							stream->fbuf,
							stream->szfbuf,
							&errind);
		if (errind)
		{
			errno = actualRead;
			actualRead = 0;
			stream->errorInd = 1;
		}

		stream->endbuf = stream->fbuf + actualRead;
		*stream->endbuf = '\n';
		if (actualRead == 0)
		{
			*u = '\0';
			if ((u - s) <= 1)
			{
				stream->eofInd = 1;
				return (NULL);
			}
			else
			{
				return (s);
			}
		}
		stream->upto = stream->fbuf;
	}
}
#endif

__PDPCLIB_API__ int ungetc(int c, FILE *stream)
{
	if ((stream->ungetCh != -1) || (c == EOF))
	{
		return (EOF);
	}
	stream->ungetCh = (unsigned char)c;
	stream->quickText = 0;
	stream->quickBin = 0;
	return ((unsigned char)c);
}

__PDPCLIB_API__ int fgetc(FILE *stream)
{
	unsigned char x[1];
	size_t ret;

	ret = fread(x, 1, 1, stream);
	if (ret == 0)
	{
		return (EOF);
	}
	return ((int)(x[0]));
}

__PDPCLIB_API__ int fseek(FILE *stream, long int offset, int whence)
{
	long oldpos;
	long newpos;

	oldpos = stream->bufStartR + (stream->upto - stream->fbuf);
	if (stream->mode == __WRITE_MODE)
	{
		fflush(stream);
	}
	if (whence == SEEK_SET)
	{
		newpos = offset;
	}
	else if (whence == SEEK_CUR)
	{
		newpos = oldpos + offset;
	}

	if (whence == SEEK_END)
	{
		__seek(stream->hfile, offset, 2);
		newpos=__tell(stream->hfile);
		
		stream->endbuf = stream->fbuf + stream->szfbuf;
		stream->upto = stream->endbuf;
		stream->bufStartR = newpos - stream->szfbuf;

//		char buf[1000];
//		while (fread(buf, sizeof buf, 1, stream) == 1)
//		{
//		   /* do nothing */
//		}
	}
//	else if ((newpos >= stream->bufStartR)
//		&& (newpos < (stream->bufStartR + (stream->endbuf - stream->fbuf)))
//		&& (stream->update || (stream->mode == __READ_MODE)))
	else if(0)
	{
		tk_printf("%d %d %d\n",
			((newpos >= stream->bufStartR)),
			(newpos < (stream->bufStartR + (stream->endbuf - stream->fbuf))),
			(stream->update || (stream->mode == __READ_MODE)));

		stream->upto = stream->fbuf + (size_t)(newpos - stream->bufStartR);
	}
	else
	{
		__seek(stream->hfile, newpos, whence);
		stream->endbuf = stream->fbuf + stream->szfbuf;
		stream->upto = stream->endbuf;
		stream->bufStartR = newpos - stream->szfbuf;
	}

	stream->quickBin = 0;
	stream->quickText = 0;
	stream->ungetCh = -1;
	return (0);
}

__PDPCLIB_API__ long int ftell(FILE *stream)
{
	return (stream->bufStartR + (stream->upto - stream->fbuf));
}

__PDPCLIB_API__ int fsetpos(FILE *stream, const fpos_t *pos)
{
	fseek(stream, *pos, SEEK_SET);
	return (0);
}

__PDPCLIB_API__ int fgetpos(FILE *stream, fpos_t *pos)
{
	*pos = ftell(stream);
	return (0);
}

__PDPCLIB_API__ void rewind(FILE *stream)
{
	fseek(stream, 0L, SEEK_SET);
	return;
}

__PDPCLIB_API__ void clearerr(FILE *stream)
{
	stream->errorInd = 0;
	stream->eofInd = 0;
	return;
}

__PDPCLIB_API__ void perror(const char *s)
{
	if ((s != NULL) && (*s != '\0'))
	{
		printf("%s: %s\n", s, strerror(errno));
	}else
	{
		printf("%s\n", strerror(errno));
	}

	return;
}

/*
NULL + F = allocate, setup
NULL + L = allocate, setup
NULL + N = ignore, return success
buf  + F = setup
buf  + L = setup
buf  + N = ignore, return success
*/

__PDPCLIB_API__ int setvbuf(FILE *stream, char *buf, int mode, size_t size)
{
	char *mybuf;

	if (mode == _IONBF)
	{
		stream->bufTech = mode;
		return (0);
	}
	if (buf == NULL)
	{
		if (size < 2)
		{
			return (-1);
		}
		mybuf = malloc(size + 8);
		if (mybuf == NULL)
		{
			return (-1);
		}
	}
	else
	{
		if (size < 10)
		{
			return (-1);
		}
		mybuf = buf;
		stream->theirBuffer = 1;
		size -= 8;
	}
	if (!stream->permfile)
	{
		free(stream->intBuffer);
	}
	stream->intBuffer = mybuf;
	stream->fbuf = stream->intBuffer + 2;
	*stream->fbuf++ = '\0';
	*stream->fbuf++ = '\0';
	stream->szfbuf = size;
	stream->endbuf = stream->fbuf + stream->szfbuf;
	*stream->endbuf = '\n';
	if (stream->mode == __WRITE_MODE)
	{
		stream->upto = stream->fbuf;
	}
	else
	{
		stream->upto = stream->endbuf;
	}
	stream->bufTech = mode;
	if (!stream->textMode && (stream->bufTech == _IOLBF))
	{
		stream->quickBin = 0;
	}
	return (0);
}

__PDPCLIB_API__ int setbuf(FILE *stream, char *buf)
{
	int ret;

	if (buf == NULL)
	{
		ret = setvbuf(stream, NULL, _IONBF, 0);
	}
	else
	{
		ret = setvbuf(stream, buf, _IOFBF, BUFSIZ);
	}
	return (ret);
}

__PDPCLIB_API__ FILE *freopen(
	const char *filename, const char *mode, FILE *stream)
{
	inreopen = 1;
	fclose(stream);

	myfile = stream;
	fnm = filename;
	modus = mode;
	err = 0;
	spareSpot = stream->intFno;
	fopen2();
	if (err && !stream->permfile)
	{
		__userFiles[stream->intFno] = NULL;
		free(stream);
	}

	inreopen = 0;
	if (err)
	{
		return (NULL);
	}
	return (stream);
}

__PDPCLIB_API__ int fflush(FILE *stream)
{
	int errind;
	size_t actualWritten;

	if ((stream->upto != stream->fbuf) && (stream->mode == __WRITE_MODE))
	{
		actualWritten = __write(stream->hfile,
								stream->fbuf,
								(size_t)(stream->upto - stream->fbuf),
								&errind);
		if (errind)
		{
			stream->errorInd = 1;
			errno = actualWritten;
			return (EOF);
		}

		stream->bufStartR += actualWritten;
		stream->upto = stream->fbuf;
	}
	return (0);
}

__PDPCLIB_API__ char *tmpnam(char *s)
{
	static char buf[] = "ZZZZZZZA.$$$";
	buf[7]++;

	if (s == NULL)
	{
		return (buf);
	}
	strcpy(s, buf);
	return (s);
}

__PDPCLIB_API__ FILE *tmpfile(void)
{
	return (fopen("ZZZZZZZA.$$$", "wb+"));
}

__PDPCLIB_API__ int vfscanf(FILE *stream, const char *format, va_list arg)
{
	int ret;
	ret = vvscanf(format, arg, stream, NULL);
	return (ret);
}

__PDPCLIB_API__ int fscanf(FILE *stream, const char *format, ...)
{
	va_list arg;
	int ret;

	va_start(arg, format);
	ret = vvscanf(format, arg, stream, NULL);
	va_end(arg);
	return (ret);
}

__PDPCLIB_API__ int scanf(const char *format, ...)
{
	va_list arg;
	int ret;

	va_start(arg, format);
	ret = vvscanf(format, arg, stdin, NULL);
	va_end(arg);
	return (ret);
}

__PDPCLIB_API__ int vscanf(const char *format, va_list arg)
{
	int ret;

	ret = vvscanf(format, arg, stdin, NULL);
	return (ret);
}

__PDPCLIB_API__ int sscanf(const char *s, const char *format, ...)
{
	va_list arg;
	int ret;

	va_start(arg, format);
	ret = vvscanf(format, arg, NULL, s);
	va_end(arg);
	return (ret);
}

__PDPCLIB_API__ int vsscanf(const char *s, const char *format, va_list arg)
{
	int ret;
	ret = vvscanf(format, arg, NULL, s);
	return (ret);
}

/* vvscanf - the guts of the input scanning */
/* several mods by Dave Edwards */
static int vvscanf(const char *format, va_list arg, FILE *fp, const char *s)
{
	int ch;
	int fin = 0;
	int cnt = 0;
	char *cptr;
	int *iptr;
	unsigned int *uptr;
	long *lptr;
	unsigned long *luptr;
	short *hptr;
	unsigned short *huptr;
	double *dptr;
	float *fptr;
	long startpos;
	const char *startp;
	int skipvar; /* nonzero if we are skipping this variable */
	int modlong;   /* nonzero if "l" modifier found */
	int modshort;   /* nonzero if "h" modifier found */
	int informatitem;  /* nonzero if % format item started */
		   /* informatitem is 1 if we have processed "%l" but not the
			  type letter (s,d,e,f,g,...) yet. */

	if (fp != NULL)
	{
		startpos = ftell(fp);
	}
	else
	{
		startp = s;
	}
	inch();
	informatitem = 0;   /* init */
	if ((fp != NULL && ch == EOF) || (fp == NULL && ch == 0)) return EOF;
							   /* initially at EOF or end of string */
	while (!fin)
	{
		if (*format == '\0')
		{
			fin = 1;
		}
		else if (*format == '%' || informatitem)
		{
			if(*format=='%')   /* starting a format item */
			{
				format++;
				modlong=0;   /* init */
				modshort=0;
				skipvar = 0;
				if (*format == '*')
				{
					skipvar = 1;
					format++;
				}
			}
			if (*format == '%')   /* %% */
			{
				if (ch != '%') return (cnt);
				inch();
				informatitem=0;
			}
			else if (*format == 'l')
			{
				/* Type modifier: l  (e.g. %ld or %lf) */
				modlong=1;
				informatitem=1;
			}
			else if (*format == 'h')
			{
				/* Type modifier: h (short int) */
				modshort=1;
				informatitem=1;
			}
			else	/* process a type character: */
			{
				informatitem=0;   /* end of format item */
				if (*format == 's')
				{
					if (!skipvar)
					{
						cptr = va_arg(arg, char *);
					}
					/* Skip leading whitespace: */
					while (ch>=0 && isspace(ch)) inch();
					if ((fp != NULL && ch == EOF) || (fp == NULL && ch == 0))
										 /* at EOF or end of string */
					{
						fin = 1;
						if (!skipvar)
						{
							*cptr = 0;   /* give a null string */
						}
					}
					else
					{
						for(;;)
						{
							if (isspace(ch)) break;
							if ((fp != NULL && ch == EOF)
								|| (fp == NULL && ch == 0))
							{
								fin = 1;
								break;
							}
							if (!skipvar)
							{
								*cptr++ = (char)ch;
							}
							inch();
						}
						if (!skipvar)
						{
							*cptr = '\0';
						}
						cnt++;
					}
				}
				else if (*format == '[')
				{
					int reverse = 0;
					int found;
					const char *first;
					const char *last;
					size_t size;
					size_t mcnt = 0;
					
					if (!skipvar)
					{
						cptr = va_arg(arg, char *);
					}
					format++;
					if (*format == '^')
					{
						reverse = 1;
						format++;
					}
					if (*format == '\0') break;
					first = format;
					format++;
					last = strchr(format, ']');					
					if (last == NULL) return (cnt);
					size = (size_t)(last - first);
					while (1)
					{
						/* note that C90 doesn't require special
						   processing for '-' so it hasn't been
						   added */
						found = (memchr(first, ch, size) != NULL);
						if (found && reverse) break;
						if (!found && !reverse) break;
						if (!skipvar)
						{
							*cptr++ = (char)ch;
						}
						mcnt++;
						inch();
						/* if at EOF or end of string, bug out */
						if ((fp != NULL && ch == EOF) 
							|| (fp == NULL && ch == 0))
						{
							break;
						}
					}
					if (mcnt > 0)
					{
						if (!skipvar)
						{
							*cptr++ = '\0';
						}
						cnt++;
					}
					else
					{
						break;
					}
					format = last + 1;
				}
				else if (*format == 'c')
				{
					if (!skipvar)
					{
						cptr = va_arg(arg, char *);
					}
					if ((fp != NULL && ch == EOF)
						|| (fp == NULL && ch == 0)) fin = 1;
										  /* at EOF or end of string */
					else
					{
						if (!skipvar)
						{
							*cptr = ch;
						}
						cnt++;
						inch();
					}
				}
				else if (*format == 'n')
				{
					uptr = va_arg(arg, unsigned int *);
					if (fp != NULL)
					{
						*uptr = (unsigned int)(ftell(fp) - startpos);
					}
					else
					{
						*uptr = (unsigned int)(startp - s);
					}					
				}
				else if (*format == 'd' || *format == 'u'
						 || *format == 'x' || *format == 'o'
						 || *format == 'p'
						 || *format == 'i')						 
				{
					int neg = 0;
					unsigned long x = 0;
					int undecided = 0;
					int base = 10;
					int mcnt = 0;

					if (*format == 'x') base = 16;
					else if (*format == 'p') base = 16;
					else if (*format == 'o') base = 8;
					else if (*format == 'i') base = 0;
					if (!skipvar)
					{
						if ((*format == 'd') || (*format == 'i'))
						{
							if (modlong) lptr = va_arg(arg, long *);
							else if (modshort) hptr = va_arg(arg, short *);
							else iptr = va_arg(arg, int *);
						}
						else
						{
							if (modlong) luptr = va_arg(arg, unsigned long *);
							else if (modshort) huptr =
									 va_arg(arg, unsigned short *);
							else uptr = va_arg(arg, unsigned int *);
						}
					}
					/* Skip leading whitespace: */
					while (ch>=0 && isspace(ch)) inch();
					if (ch == '-')
					{
						neg = 1;
						inch();
					}
					else if(ch == '+') inch();
					
					/* this logic is the same as strtoul so if you
					   change this, change that one too */

					if (base == 0)
					{
						undecided = 1;
					}
					while (!((fp != NULL && ch == EOF)
							 || (fp == NULL && ch == 0)))
					{
						if (isdigit((unsigned char)ch))
						{
							if (base == 0)
							{
								if (ch == '0')
								{
									base = 8;
								}
								else
								{
									base = 10;
									undecided = 0;
								}
							}
							x = x * base + (ch - '0');
							inch();
						}
/* DOS has a ':' in the pointer - skip that */
#if defined(__MSDOS__) && !defined(__PDOS__) && !defined(__gnu_linux__)
						else if ((*format == 'p') && (ch == ':'))
						{
							inch();
						}
#endif
						else if (isalpha((unsigned char)ch))
						{
							if ((ch == 'X') || (ch == 'x'))
							{
								if ((base == 0) || ((base == 8) && undecided))
								{
									base = 16;
									undecided = 0;
									inch();
								}
								else if (base == 16)
								{
									/* hex values are allowed to have an 
									   optional 0x */
									inch();
								}
								else
								{
									break;
								}
							}
							else if (base <= 10)
							{
								break;
							}
							else
							{
								x = x * base + 
									(toupper((unsigned char)ch) - 'A') + 10;
								inch();
							}
						}
						else
						{
							break;
						}
						mcnt++;
					}
					
					/* end of strtoul logic */
					
					/* If we didn't get any characters, don't go any
					   further */
					if (mcnt == 0)
					{
						break;
					}

					
					if (!skipvar)
					{
						if ((*format == 'd') || (*format == 'i'))
						{
							long lval;
							
							if (neg)
							{
								lval = (long)-x;
							}
							else
							{
								lval = (long)x;
							}
							if (modlong) *lptr=lval;
								/* l modifier: assign to long */
							else if (modshort) *hptr = (short)lval;
								/* h modifier */
							else *iptr=(int)lval;
						}
						else
						{
							if (modlong) *luptr = (unsigned long)x;
							else if (modshort) *huptr = (unsigned short)x;
							else *uptr = (unsigned int)x;
						}
					}
					cnt++;
				}
				else if (*format=='e' || *format=='f' || *format=='g' ||
						 *format=='E' || *format=='G')
				{
					/* Floating-point (double or float) input item */
					int negsw1,negsw2,dotsw,expsw,ndigs1,ndigs2,nfdigs;
					int ntrailzer,expnum,expsignsw;
					double fpval,pow10;

					if (!skipvar)					
					{
						if (modlong) dptr = va_arg(arg, double *);
						else fptr = va_arg(arg, float *);
					}
					negsw1=0;   /* init */
					negsw2=0;
					dotsw=0;
					expsw=0;
					ndigs1=0;
					ndigs2=0;
					nfdigs=0;
					ntrailzer=0;  /* # of trailing 0's unaccounted for */
					expnum=0;
					expsignsw=0;  /* nonzero means done +/- on exponent */
					fpval=0.0;
					/* Skip leading whitespace: */
					while (ch>=0 && isspace(ch)) inch();
					if (ch=='-')
					{
						negsw1=1;
						inch();
					}
					else if (ch=='+') inch();
					while (ch>0)
					{
						if (ch=='.' && dotsw==0 && expsw==0) dotsw=1;
						else if (isdigit(ch))
						{
							if(expsw)
							{
								ndigs2++;
								expnum=expnum*10+(ch-'0');
							}
							else
							{
								/* To avoid overflow or loss of precision,
								   skip leading and trailing zeros unless
								   really needed. (Automatic for leading
								   0's, since 0.0*10.0 is 0.0) */
								ndigs1++;
								if (dotsw) nfdigs++;
								if (ch=='0' && fpval!=0.)
								{
									/* Possible trailing 0 */
									ntrailzer++;
								}
								else
								{
									/* Account for any preceding zeros */
									while (ntrailzer>0)
									{
										fpval*=10.;
										ntrailzer--;
									}
									fpval=fpval*10.0+(ch-'0');
								}
							}
						}
						else if ((ch=='e' || ch=='E') && expsw==0) expsw=1;
						else if ((ch=='+' || ch=='-') && expsw==1
								 && ndigs2==0 && expsignsw==0)
						{
							expsignsw=1;
							if (ch=='-') negsw2=1;
						}
						else break;   /* bad char: end of input item */
						inch();
					}
					if ((fp != NULL && ch == EOF)
						|| (fp == NULL && ch == 0)) fin=1;
					/* Check for a valid fl-pt value: */
					if (ndigs1==0 || (expsw && ndigs2==0)) return(cnt);
					/* Complete the fl-pt value: */
					if (negsw2) expnum=-expnum;
					expnum+=ntrailzer-nfdigs;
					if (expnum!=0 && fpval!=0.0)
					{
						negsw2=0;
						if (expnum<0)
						{
							expnum=-expnum;
							negsw2=1;
						}
						/* Multiply or divide by 10.0**expnum, using
						   bits of expnum (fast method) */
						pow10=10.0;
						for (;;)
						{
							if (expnum & 1)	 /* low-order bit */
							{
								if (negsw2) fpval/=pow10;
								else fpval*=pow10;
							}
							expnum>>=1;   /* shift right 1 bit */
							if (expnum==0) break;
							pow10*=pow10;   /* 10.**n where n is power of 2 */
						}
					}
					if (negsw1) fpval=-fpval;
					if (!skipvar)
					{
						/* l modifier: assign to double */
						if (modlong) *dptr=fpval;
						else *fptr=(float)fpval;
					}
					cnt++;
				}
			}
		}
		else if (isspace((unsigned char)(*format)))
		{
			/* Whitespace char in format string: skip next whitespace
			   chars in input data. This supports input of multiple
			   data items. */
			while (ch>=0 && isspace(ch))
			{
				inch();
			}
		}
		else   /* some other character in the format string */
		{
			if (ch != *format) return (cnt);
			inch();
		}
		format++;
		if ((fp != NULL && ch == EOF) || (fp == NULL && ch == 0)) fin = 1;
			/* EOF */
	}
	if (fp != NULL) ungetc(ch, fp);
	return (cnt);
}

__PDPCLIB_API__ char *gets(char *s)
{
	char *ret;

	stdin->quickText = 0;
	stdin->noNl = 1;
	ret = fgets(s, INT_MAX, stdin);
	stdin->noNl = 0;
	stdin->quickText = 1;
	return (ret);
}

__PDPCLIB_API__ int puts(const char *s)
{
	int ret;

	ret = fputs(s, stdout);
	if (ret == EOF)
	{
		return (ret);
	}
	return (putc('\n', stdout));
}

/* The following functions are implemented as macros */

#undef getchar
#undef putchar
#undef getc
#undef putc
#undef feof
#undef ferror

__PDPCLIB_API__ int getc(FILE *stream)
{
	return (fgetc(stream));
}

__PDPCLIB_API__ int putc(int c, FILE *stream)
{
	return (fputc(c, stream));
}

__PDPCLIB_API__ int getchar(void)
{
	return (getc(stdin));
}

__PDPCLIB_API__ int putchar(int c)
{
	return (putc(c, stdout));
}

__PDPCLIB_API__ int feof(FILE *stream)
{
	return (stream->eofInd);
}

__PDPCLIB_API__ int ferror(FILE *stream)
{
	return (stream->errorInd);
}


/*

 The truely cludged piece of code was concocted by Dave Wade

 His erstwhile tutors are probably turning in their graves.

 It is however placed in the Public Domain so that any one
 who wishes to improve is free to do so

*/

static void dblcvt(double num, char cnvtype, size_t nwidth,
			int nprecision, char *result)
{
	double b,round;
	int i,j,exp,pdigits,format;
	char sign, work[45];

	/* save original data & set sign */

	if ( num < 0 )
	{
		b = -num;
		sign = '-';
	}
	else
	{
		b = num;
		sign = ' ';
	}

	/*
	  Now scale to get exponent
	*/

	exp = 0;
	if( b > 1.0 )
	{
		while ((b >= 10.0) && (exp < 35))
		{
			++exp;
//			b=b / 10.0;
			b=b * 0.1;
		}
	}
	else if ( b == 0.0 )
	{
		exp=0;
	}
	/* 1.0 will get exp = 0 */
	else if ( b < 1.0 )
	{
		while ((b < 1.0) && (exp > -35))
		{
			--exp;
			b=b*10.0;
		}
	}
	if ((exp <= -35) || (exp >= 35))
	{
		exp = 0;
		b = 0.0;
	}
	
	/*
	  now decide how to print and save in FORMAT.
		 -1 => we need leading digits
		  0 => print in exp
		 +1 => we have digits before dp.
	*/

	switch (cnvtype)
	{
		case 'E':
		case 'e':
			format = 0;
			break;
		case 'f':
		case 'F':
			if ( exp >= 0 )
			{
				format = 1;
			}
			else
			{
				format = -1;
			}
			break;
		default:
			/* Style e is used if the exponent from its
			   conversion is less than -4 or greater than
			   or equal to the precision.
			*/
			if ( exp >= 0 )
			{
				if ( nprecision > exp )
				{
					format=1;
				}
				else
				{
					format=0;
				}
			}
			else
			{
				/*  if ( nprecision > (-(exp+1) ) ) { */
				if ( exp >= -4)
				{
					format=-1;
				}
				else
				{
					format=0;
				}
			}
			break;
	}
	/*
	Now round
	*/
	switch (format)
	{
		case 0:	/* we are printing in standard form */
			if (nprecision < DBL_MANT_DIG) /* we need to round */
			{
				j = nprecision;
			}
			else
			{
				j=DBL_MANT_DIG;
			}
			round = 1.0/2.0;
			i = 0;
			while (++i <= j)
			{
//				round = round/10.0;
				round = round*0.1;

			}
			b = b + round;
			if (b >= 10.0)
			{
//				b = b/10.0;
				b = b*0.1;
				exp = exp + 1;
			}
			break;

		case 1:	  /* we have a number > 1  */
						 /* need to round at the exp + nprecisionth digit */
				if (exp + nprecision < DBL_MANT_DIG) /* we need to round */
				{
					j = exp + nprecision;
				}
				else
				{
					j = DBL_MANT_DIG;
				}
				round = 0.5;
				i = 0;
				while (i++ < j)
				{
//					round = round/10;
					round = round*0.1;
				}
				b = b + round;
				if (b >= 10.0)
				{
//					b = b/10.0;
					b = b*0.1;
					exp = exp + 1;
				}
				break;

		case -1:   /* we have a number that starts 0.xxxx */
			if (nprecision < DBL_MANT_DIG) /* we need to round */
			{
				j = nprecision + exp + 1;
			}
			else
			{
				j = DBL_MANT_DIG;
			}
			round = 5.0;
			i = 0;
			while (i++ < j)
			{
//				round = round/10;
				round = round*0.1;
			}
			if (j >= 0)
			{
				b = b + round;
			}
			if (b >= 10.0)
			{
//				b = b/10.0;
				b = b*0.1;
				exp = exp + 1;
			}
			if (exp >= 0)
			{
				format = 1;
			}
			break;
	}
	/*
	   Now extract the requisite number of digits
	*/

	if (format==-1)
	{
		/*
			 Number < 1.0 so we need to print the "0."
			 and the leading zeros...
		*/
		result[0]=sign;
		result[1]='0';
		result[2]='.';
		result[3]=0x00;
		while (++exp)
		{
			--nprecision;
			strcat(result,"0");
		}
		i=b;
		--nprecision;
		work[0] = (char)('0' + i % 10);
		work[1] = 0x00;
		strcat(result,work);

		pdigits = nprecision;

		while (pdigits-- > 0)
		{
			b = b - i;
			b = b * 10.0;
			i = b;
			work[0] = (char)('0' + i % 10);
			work[1] = 0x00;
			strcat(result,work);
		}
	}
	/*
	   Number >= 1.0 just print the first digit
	*/
	else if (format==+1)
	{
		i = b;
		result[0] = sign;
		result[1] = '\0';
		work[0] = (char)('0' + i % 10);
		work[1] = 0x00;
		strcat(result,work);
		nprecision = nprecision + exp;
		pdigits = nprecision ;

		while (pdigits-- > 0)
		{
			if ( ((nprecision-pdigits-1)==exp)  )
			{
				strcat(result,".");
			}
			b = b - i;
			b = b * 10.0;
			i = b;
			work[0] = (char)('0' + i % 10);
			work[1] = 0x00;
			strcat(result,work);
		}
	}
	/*
	   printing in standard form
	*/
	else
	{
		i = b;
		result[0] = sign;
		result[1] = '\0';
		work[0] = (char)('0' + i % 10);
		work[1] = 0x00;
		strcat(result,work);
		strcat(result,".");

		pdigits = nprecision;

		while (pdigits-- > 0)
		{
			b = b - i;
			b = b * 10.0;
			i = b;
			work[0] = (char)('0' + i % 10);
			work[1] = 0x00;
			strcat(result,work);
		}
	}

	if (format==0)
	{ /* exp format - put exp on end */
		work[0] = 'E';
		if ( exp < 0 )
		{
			exp = -exp;
			work[1]= '-';
		}
		else
		{
			work[1]= '+';
		}
		work[2] = (char)('0' + (exp/10) % 10);
		work[3] = (char)('0' + exp % 10);
		work[4] = 0x00;
		strcat(result, work);
	}
	else
	{
		/* get rid of trailing zeros for g specifier */
		if (cnvtype == 'G' || cnvtype == 'g')
		{
			char *p;
			
			p = strchr(result, '.');
			if (p != NULL)
			{
				p++;
				p = p + strlen(p) - 1;				
				while (*p != '.' && *p == '0')
				{
					*p = '\0';
					p--;
				}
				if (*p == '.')
				{
					*p = '\0';
				}
			}
		}
	 }
	/* printf(" Final Answer = <%s> fprintf gives=%g\n",
				result,num); */
	/*
	 do we need to pad
	*/
	if(result[0] == ' ')strcpy(work,result+1); else strcpy(work,result);
	pdigits=nwidth-strlen(work);
	result[0]= 0x00;
	while(pdigits>0)
	{
		strcat(result," ");
		pdigits--;
	}
	strcat(result,work);
	return;
}
