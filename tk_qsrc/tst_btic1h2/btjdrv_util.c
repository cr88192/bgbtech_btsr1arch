/*
Copyright (C) 2015 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// #include <bgbbtj.h>

void *bgbv86_ralloc(int sz)
{
	static byte *buf=NULL;
	static byte *bufe, *bufr;
	void *p;

	if(!buf)
	{
		buf=malloc(1<<18);
		bufe=buf+(1<<18);
		bufr=buf;
	}

	sz=(sz+15)&(~15);
	if((bufr+sz)>bufe)bufr=buf;
	p=bufr; bufr+=sz;
	memset(p, 0, sz);
	return(p);
}

char *bgbv86_rstrdup(char *str)
{
	char *t;
	t=bgbv86_ralloc(strlen(str)+1);
	strcpy(t, str);
	return(t);
}

char *bgbv86_rsprint(char *str, ...)
{
	char buf[256];
	va_list lst;
	char *t;

	va_start(lst, str);
	vsprintf(buf, str, lst);
	va_end(lst);

	t=bgbv86_rstrdup(buf);
	return(t);
}

char **bgbv86_rsplit(char *s)
{
	char **a, *t;
	int i;

	a=bgbv86_ralloc(64*sizeof(char *));
	i=0;
	while(*s)
	{
		while(*s && (*s<=' '))s++;
		if(!*s)break;
		t=bgbv86_ralloc(256);
		a[i++]=t;

		if(*s=='"')
		{
			s++;

			while(*s && (*s!='"'))
			{
				if(*s=='\\')
				{
					*t++=*s++;

					if(*s=='x')
					{
						*t++=*s++;
						*t++=*s++; *t++=*s++;
						continue;
					}
					if(*s=='u')
					{
						*t++=*s++;
						*t++=*s++; *t++=*s++;
						*t++=*s++; *t++=*s++;
						continue;
					}
					if(*s=='U')
					{
						*t++=*s++;
						*t++=*s++; *t++=*s++;
						*t++=*s++; *t++=*s++;
						*t++=*s++; *t++=*s++;
						*t++=*s++; *t++=*s++;
						continue;
					}

					*t++=*s++;
					continue;
				}

				*t++=*s++;
			}

			if(*s=='"')s++;

			*t++=0;
			*t++='\"';
			*t++=0;
			continue;
		}

		while(*s && (*s>' '))*t++=*s++;
		*t++=0;
		*t++=0;
	}
	a[i]=NULL;

	return(a);
}

int imgstat(byte *simg, byte *dimg, int n)
{
	float er, eg, eb, ea;
	int i, j;
	
	er=0; eg=0; eb=0; ea=0;
	for(i=0; i<n; i++)
	{
		j=dimg[i*4+0]-simg[i*4+0];	er+=j*j;
		j=dimg[i*4+1]-simg[i*4+1];	eg+=j*j;
		j=dimg[i*4+2]-simg[i*4+2];	eb+=j*j;
		j=dimg[i*4+3]-simg[i*4+3];	ea+=j*j;
	}
	
	printf("%f %f %f %f\n", sqrt(er/n), sqrt(eg/n), sqrt(eb/n), sqrt(ea/n));
	return(0);
}

int bgbbtj_imgflip(byte *simg, byte *dimg, int xs, int ys)
{
	int i, j;
	
	for(i=0; i<ys; i++)
		for(j=0; j<xs; j++)
	{
		dimg[((ys-i-1)*xs+j)*4+0]=simg[(i*xs+j)*4+0];
		dimg[((ys-i-1)*xs+j)*4+1]=simg[(i*xs+j)*4+1];
		dimg[((ys-i-1)*xs+j)*4+2]=simg[(i*xs+j)*4+2];
		dimg[((ys-i-1)*xs+j)*4+3]=simg[(i*xs+j)*4+3];
	}
	return(0);
}

int bgbbtj_imgflip_single(byte *img, int xs, int ys)
{
	byte *timg;
	
	timg=malloc(xs*ys*4);
	bgbbtj_imgflip(img, timg, xs, ys);
	memcpy(img, timg, xs*ys*4);
	free(timg);
	return(0);
}
