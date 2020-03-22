#include <tkgl/bgbrasw.h>

void Mat3F_Copy(float *a, float *b)
{
	b[0]=a[0]; b[1]=a[1]; b[2]=a[2];
	b[3]=a[3]; b[4]=a[4]; b[5]=a[5];
	b[6]=a[6]; b[7]=a[7]; b[8]=a[8];
}

void Mat3F_Transpose(float *a, float *b)
{
	b[0]=a[0]; b[1]=a[3]; b[2]=a[6];
	b[3]=a[1]; b[4]=a[4]; b[5]=a[7];
	b[6]=a[2]; b[7]=a[5]; b[8]=a[8];
}

void Mat3F_Identity(float *a)
{
	a[0]=1; a[1]=0; a[2]=0;
	a[3]=0; a[4]=1; a[5]=0;
	a[6]=0; a[7]=0; a[8]=1;
}

void Mat3F_Mat3Mult(float *a, float *b, float *c)
{
	c[0]=a[0]*b[0] + a[1]*b[3] + a[2]*b[6];
	c[1]=a[0]*b[1] + a[1]*b[4] + a[2]*b[7];
	c[2]=a[0]*b[2] + a[1]*b[5] + a[2]*b[8];
	c[3]=a[3]*b[0] + a[4]*b[3] + a[5]*b[6];
	c[4]=a[3]*b[1] + a[4]*b[4] + a[5]*b[7];
	c[5]=a[3]*b[2] + a[4]*b[5] + a[5]*b[8];
	c[6]=a[6]*b[0] + a[7]*b[3] + a[8]*b[6];
	c[7]=a[6]*b[1] + a[7]*b[4] + a[8]*b[7];
	c[8]=a[6]*b[2] + a[7]*b[5] + a[8]*b[8];
}

void Mat3F_Mat3MultT(float *a, float *b, float *c)
{
	//0 1 2  0 3 6
	//3 4 5  1 4 7
	//6 7 8  2 5 8

	c[0]=a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
	c[1]=a[0]*b[3] + a[1]*b[4] + a[2]*b[5];
	c[2]=a[0]*b[6] + a[1]*b[7] + a[2]*b[8];
	c[3]=a[3]*b[0] + a[4]*b[1] + a[5]*b[2];
	c[4]=a[3]*b[3] + a[4]*b[4] + a[5]*b[5];
	c[5]=a[3]*b[6] + a[4]*b[7] + a[5]*b[8];
	c[6]=a[6]*b[0] + a[7]*b[1] + a[8]*b[2];
	c[7]=a[6]*b[3] + a[7]*b[4] + a[8]*b[5];
	c[8]=a[6]*b[6] + a[7]*b[7] + a[8]*b[8];
}

void Mat3F_Mat3TMult(float *a, float *b, float *c)
{
	c[0]=a[0]*b[0] + a[3]*b[3] + a[6]*b[6];
	c[1]=a[0]*b[1] + a[3]*b[4] + a[6]*b[7];
	c[2]=a[0]*b[2] + a[3]*b[5] + a[6]*b[8];
	c[3]=a[1]*b[0] + a[4]*b[3] + a[7]*b[6];
	c[4]=a[1]*b[1] + a[4]*b[4] + a[7]*b[7];
	c[5]=a[1]*b[2] + a[4]*b[5] + a[7]*b[8];
	c[6]=a[2]*b[0] + a[5]*b[3] + a[8]*b[6];
	c[7]=a[2]*b[1] + a[5]*b[4] + a[8]*b[7];
	c[8]=a[2]*b[2] + a[5]*b[5] + a[8]*b[8];
}

void Mat3F_TransformVector(float *a, float *xform, float *c)
{
	c[0]=(a[0]*xform[0])+(a[1]*xform[3])+(a[2]*xform[6]);
	c[1]=(a[0]*xform[1])+(a[1]*xform[4])+(a[2]*xform[7]);
	c[2]=(a[0]*xform[2])+(a[1]*xform[5])+(a[2]*xform[8]);
}

void Mat3F_Inverse(float *a, float *b)
{
	float d;

	//determinant
	d=	a[0]*(a[4]*a[8]-a[7]*a[5])-
		a[1]*(a[3]*a[8]-a[6]*a[5])+
		a[2]*(a[3]*a[7]-a[6]*a[4]);

	if(fabs(d)<0.0001)
	{
		Mat3F_Identity(b);
		return;
	}
	if(fabs(d-1)<0.001)
	{
		Mat3F_Transpose(a, b);
		return;
	}

	b[0]= (a[4]*a[8]-a[5]*a[7])/d;
	b[1]=-(a[1]*a[8]-a[7]*a[2])/d;
	b[2]= (a[1]*a[5]-a[4]*a[2])/d;

	b[3]=-(a[3]*a[8]-a[5]*a[6])/d;
	b[4]= (a[0]*a[8]-a[6]*a[2])/d;
	b[5]=-(a[0]*a[5]-a[3]*a[2])/d;

	b[6]= (a[3]*a[7]-a[6]*a[4])/d;
	b[7]=-(a[0]*a[7]-a[6]*a[1])/d;
	b[8]= (a[0]*a[4]-a[1]*a[3])/d;
}

void Mat3F_AxisTo3Matrix(float *a, float th, float *b)
{
	float xx, xy, xz, xw, yy, yz, yw, zz, zw;
	float v[4];
	float sa, ca;
	int i;

	sa=sin(th/2);
	ca=cos(th/2);
	V3F_NORMALIZE(a, v);


	for(i=0; i<9; i++)b[i]=0;
	for(i=0; i<3; i++)b[(i*3)+i]=1;

	v[0]=v[0]*sa; v[1]=v[1]*sa; v[2]=v[2]*sa; v[3]=ca;

	xx=v[0]*v[0];	xy=v[0]*v[1];	xz=v[0]*v[2];	xw=v[0]*v[3];
			yy=v[1]*v[1];	yz=v[1]*v[2];	yw=v[1]*v[3];
					zz=v[2]*v[2];	zw=v[2]*v[3];

	b[0]=1-2*(yy+zz);	b[1]=  2*(xy+zw);	b[2]=  2*(xz-yw);
	b[3]=  2*(xy-zw);	b[4]=1-2*(xx+zz);	b[5]=  2*(yz+xw);
	b[6]=  2*(xz+yw);	b[7]=  2*(yz-xw);	b[8]=1-2*(xx+yy);
}

void Mat3F_Filter3Matrix(float *a, float *b)
{
	float v0[3], v1[3], v2[3];

	V3F_NORMALIZE(a+0, v0);

	V3F_ADDSCALE(a+3, v0, -V3F_DOT(a+3, v0), v1);
	V3F_NORMALIZE(v1, v1);

	V3F_ADDSCALE(a+6, v0, -V3F_DOT(a+6, v0), v2);
	V3F_ADDSCALE(v2, v1, -V3F_DOT(v2, v1), v2);
	V3F_NORMALIZE(v2, v2);

	V3F_COPY(v0, b+0);
	V3F_COPY(v1, b+3);
	V3F_COPY(v2, b+6);
}

void Mat3F_Rotate3Matrix(
	float *a, float *v, float th, float *b)
{
	float dm[9], tm[9];

	Mat3F_AxisTo3Matrix(v, th, dm);
	Mat3F_Mat3Mult(a, dm, tm);
	Mat3F_Filter3Matrix(tm, b);
}

int Mat3F_ArcPolate(
	float *v0, float *v1, float *v2, float t,
	float *rnv)
{
	float nv[3], nv1[3], f, g;

	//very similar or very far apart
	f=V3F_DOT(v0, v1);
	if((f>0.99) || (f<-0.99))
	{
		v2[0]=v0[0]*(1-t)+v1[0]*t;
		v2[1]=v0[1]*(1-t)+v1[1]*t;
		v2[2]=v0[2]*(1-t)+v1[2]*t;
		V3F_NORMALIZE(v2, v2);

		if(f>0)
		{
			if(rnv && (f>rnv[3]))
			{
				V3F_COPY(v2, rnv);
				rnv[3]=f;
			}
			return(0);
		}else
		{
			return(-1);
		}
	}

	V3F_CROSS(v0, v1, nv);
	g=V3F_NORMALIZE(nv, nv);
	f=acos(f)*t;

	if(rnv && (g>rnv[3]))
	{
		V3F_COPY(nv, rnv);
		rnv[3]=g;
	}

	V3F_CROSS(nv, v0, nv1);
	v2[0]=v0[0]*cos(f)+nv1[0]*sin(f);
	v2[1]=v0[1]*cos(f)+nv1[1]*sin(f);
	v2[2]=v0[2]*cos(f)+nv1[2]*sin(f);
	V3F_NORMALIZE(v2, v2);

	return(0);
}


void Mat3F_LinePolate(float *v0, float *v1, float *v2, float t,
	float *n)
{
	float nv[3], f;

	V3F_CROSS(v0, n, nv);
	V3F_NORMALIZE(nv, nv);
	f=M_PI*t;

	//try to make it go right direction
	if(V3F_DOT(v1, nv)<0) { V3F_SCALE(nv, -1, nv); }

	v2[0]=v0[0]*cos(f)+nv[0]*sin(f);
	v2[1]=v0[1]*cos(f)+nv[1]*sin(f);
	v2[2]=v0[2]*cos(f)+nv[2]*sin(f);
	V3F_NORMALIZE(v2, v2);
}

void Mat3F_Interpolate(float *m0, float *m1, float *m2, float t)
{
	float nv[4];
	int i;

	nv[3]=0;
	i=Mat3F_ArcPolate(m0+0, m1+0, m2+0, t, nv)?1:0;
	i|=Mat3F_ArcPolate(m0+3, m1+3, m2+3, t, nv)?2:0;
	i|=Mat3F_ArcPolate(m0+6, m1+6, m2+6, t, nv)?4:0;

	if(i)
	{
		if(i==7)
		{
			if(t<0.5)Mat3F_Copy(m0, m2);
				else Mat3F_Copy(m1, m2);
			return;
		}

		if(i&1)Mat3F_LinePolate(m0+0, m1+0, m2+0, t, nv);
		if(i&2)Mat3F_LinePolate(m0+3, m1+3, m2+3, t, nv);
		if(i&4)Mat3F_LinePolate(m0+6, m1+6, m2+6, t, nv);
	}
}
