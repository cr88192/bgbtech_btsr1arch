#include <tkgl/bgbrasw.h>

#include <stdlib.h>

void *VecNF_Ralloc(int sz)
{
	static byte *buf=NULL;
	static byte *bufe, *bufr;
	void *p;

	if(!buf)
	{
		buf=(byte *)malloc(1<<16);
		bufe=buf+(1<<16);
		bufr=buf;
	}

	sz=(sz+15)&(~15);
	if((bufr+sz)>bufe)bufr=buf;
	p=bufr; bufr+=sz;
	memset(p, 0, sz);
	return(p);
}

// VECMATH_API void Vec2F_Copy(float *a, float *b)
//	{ b[0]=a[0]; b[1]=a[1]; }

// VECMATH_API void Vec4F_Copy(float *a, float *b)
//	{ b[0]=a[0]; b[1]=a[1]; b[2]=a[2]; b[3]=a[3]; }
