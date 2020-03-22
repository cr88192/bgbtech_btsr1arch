#include <tkgl/bgbrasw.h>

void Mat4F_Identity(float *a)
{
	a[0]=1;		a[1]=0;		a[2]=0;		a[3]=0;
	a[4]=0;		a[5]=1;		a[6]=0;		a[7]=0;
	a[8]=0;		a[9]=0;		a[10]=1;	a[11]=0;
	a[12]=0;	a[13]=0;	a[14]=0;	a[15]=1;
}

void Mat4F_Copy(float *a, float *b)
{
	b[0]=a[0];   b[1]=a[1];   b[2]=a[2];   b[3]=a[3];
	b[4]=a[4];   b[5]=a[5];   b[6]=a[6];   b[7]=a[7];
	b[8]=a[8];   b[9]=a[9];   b[10]=a[10]; b[11]=a[11];
	b[12]=a[12]; b[13]=a[13]; b[14]=a[14]; b[15]=a[15];
}

int Mat4F_Equal(float *a, float *b)
{
	int i;

	i=(b[0] ==a[0]) &&(b[1] ==a[1]) &&(b[2] ==a[2]) &&(b[3] ==a[3]) &&
	  (b[4] ==a[4]) &&(b[5] ==a[5]) &&(b[6] ==a[6]) &&(b[7] ==a[7]) &&
	  (b[8] ==a[8]) &&(b[9] ==a[9]) &&(b[10]==a[10])&&(b[11]==a[11]) &&
	  (b[12]==a[12])&&(b[13]==a[13])&&(b[14]==a[14])&&(b[15]==a[15]);
	return(i);
}

void Mat4F_MatMult(float *a, float *b, float *c)
{
	c[0] =a[0]*b[0]  + a[1]*b[4]  + a[2]*b[8]   + a[3]*b[12];
	c[1] =a[0]*b[1]  + a[1]*b[5]  + a[2]*b[9]   + a[3]*b[13];
	c[2] =a[0]*b[2]  + a[1]*b[6]  + a[2]*b[10]  + a[3]*b[14];
	c[3] =a[0]*b[3]  + a[1]*b[7]  + a[2]*b[11]  + a[3]*b[15];
	c[4] =a[4]*b[0]  + a[5]*b[4]  + a[6]*b[8]   + a[7]*b[12];
	c[5] =a[4]*b[1]  + a[5]*b[5]  + a[6]*b[9]   + a[7]*b[13];
	c[6] =a[4]*b[2]  + a[5]*b[6]  + a[6]*b[10]  + a[7]*b[14];
	c[7] =a[4]*b[3]  + a[5]*b[7]  + a[6]*b[11]  + a[7]*b[15];
	c[8] =a[8]*b[0]  + a[9]*b[4]  + a[10]*b[8]  + a[11]*b[12];
	c[9] =a[8]*b[1]  + a[9]*b[5]  + a[10]*b[9]  + a[11]*b[13];
	c[10]=a[8]*b[2]  + a[9]*b[6]  + a[10]*b[10] + a[11]*b[14];
	c[11]=a[8]*b[3]  + a[9]*b[7]  + a[10]*b[11] + a[11]*b[15];
	c[12]=a[12]*b[0] + a[13]*b[4] + a[14]*b[8]  + a[15]*b[12];
	c[13]=a[12]*b[1] + a[13]*b[5] + a[14]*b[9]  + a[15]*b[13];
	c[14]=a[12]*b[2] + a[13]*b[6] + a[14]*b[10] + a[15]*b[14];
	c[15]=a[12]*b[3] + a[13]*b[7] + a[14]*b[11] + a[15]*b[15];
}

void Mat4F_MatMultT(float *a, float *b, float *c)
{
	Mat4F_MatMult(b, a, c);
//	c[0]=a[0]*b[0]
//	c[4]=a[0]*b[4]
//	c[8]=a[0]*b[8]
//	c[12]=a[0]*b[12]
}

void Mat4F_Transpose(float *a, float *b)
{
	b[0]=a[0];  b[1]=a[4];  b[2]=a[8];   b[3]=a[12];
	b[4]=a[1];  b[5]=a[5];  b[6]=a[9];   b[7]=a[13];
	b[8]=a[2];  b[9]=a[6];  b[10]=a[10]; b[11]=a[14];
	b[12]=a[3]; b[13]=a[7]; b[14]=a[11]; b[15]=a[15];
}

void Mat4F_Inverse(float *a, float *b)
{
	static float at[8*4];
	float *a2p[4], *a3p;
	int i, j, k;

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		at[i*8+j]=a[i*4+j];
		at[i*8+j+4]=((i==j)?1:0);
	}
	for(i=0; i<4; i++)a2p[i]=&at[i*4*2];

	for(i=0; i<4; i++)
	{
		if(a2p[i][i]==0)
		{
			for(j=3; j>=0; j--)
				if(a2p[j][i]!=0)
			{
				a3p=a2p[i];
				a2p[i]=a2p[j];
				a2p[j]=a3p;
				break;
			}
			if(a2p[i][i]==0)return;
		}

		for(j=0; j<8; j++)a2p[i][j]/=a2p[i][i];
		for(j=0; j<4; j++)if(i!=j)
		{
			for(k=0; k<8; k++)
				a2p[j][k]-=a2p[i][k]*a2p[j][i];
		}
	}

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			b[i*4+j]=a2p[i][4+j];
}

#if 0
void Mat4F_Inverse(float *a, float *b)
{
	// a b c d	1/a	-b	-c	0
	// e f g h	-e/f	1/f	-g	0
	// i j k l	-i/k	-j/k 	1/k	0
	// m n o p	-m	-n	-o	1

	// 1 b/a c/a d/a	1/a+(e/f)*(b/a)	0		0		0
	// 0 1 g/f h/f		-e/f	(1-e(b/a))/f	(-e(c/a))/f	(-e(d/a))/f
	// 0 j k l		-i	-i(b/a)		1-i(c/a)	-i(d/a)
	// 0 n o p		-m	-m(b/a)		-m(c/a)		1-m(d/a)

	b[0]=
	b[1]=
	b[2]=
	b[3]=

	b[4]=
	b[5]=
	b[6]=
	b[7]=

	b[8]=
	b[9]=
	b[10]=
	b[11]=

	b[12]=
	b[13]=
	b[14]=
	b[15]=
}
#endif

void Mat4F_InverseTransform(float *a, float *b)
{
	b[0]=a[0];	b[1]=a[4];	b[2]=a[8];	b[3]=0;
	b[4]=a[1];	b[5]=a[5];	b[6]=a[9];	b[7]=0;
	b[8]=a[2];	b[9]=a[6];	b[10]=a[10];	b[11]=0;
	b[12]=-a[12];	b[13]=-a[13];	b[14]=-a[14];	b[15]=1;

	b[12]=-V3F_DOT(a+3*4, b+0*4);
	b[13]=-V3F_DOT(a+3*4, b+1*4);
	b[14]=-V3F_DOT(a+3*4, b+2*4);
}

void Mat4F_InterpolateTransform(
	float *m0, float *m1, float *m2, float t)
{
	float nv[4];
	int i;

	nv[3]=0;
	i=Mat3F_ArcPolate(m0+0, m1+0, m2+0, t, nv)?1:0;
	i|=Mat3F_ArcPolate(m0+4, m1+4, m2+4, t, nv)?2:0;
	i|=Mat3F_ArcPolate(m0+8, m1+8, m2+8, t, nv)?4:0;

	V3F_SCALEADDSCALE(m0+12, 1-t, m1+12, t, m2+12);

	if(i)
	{
		if(i==7)
		{
			if(t<0.5)Mat3F_Copy(m0, m2);
				else Mat3F_Copy(m1, m2);
			return;
		}

		if(i&1)Mat3F_LinePolate(m0+0, m1+0, m2+0, t, nv);
		if(i&2)Mat3F_LinePolate(m0+4, m1+4, m2+4, t, nv);
		if(i&4)Mat3F_LinePolate(m0+8, m1+8, m2+8, t, nv);
	}
}

void Mat4F_TransformVector4(float *a, float *b, float *c)
{
	c[0]=a[0]*b[0]+a[1]*b[4]+a[2]*b[ 8]+a[3]*b[12];
	c[1]=a[0]*b[1]+a[1]*b[5]+a[2]*b[ 9]+a[3]*b[13];
	c[2]=a[0]*b[2]+a[1]*b[6]+a[2]*b[10]+a[3]*b[14];
	c[3]=a[0]*b[3]+a[1]*b[7]+a[2]*b[11]+a[3]*b[15];
}

void Mat4F_TransformVector4T(float *a, float *b, float *c)
{
	c[0]=a[0]*b[ 0]+a[1]*b[ 1]+a[2]*b[ 2]+a[3]*b[ 3];
	c[1]=a[0]*b[ 4]+a[1]*b[ 5]+a[2]*b[ 6]+a[3]*b[ 7];
	c[2]=a[0]*b[ 8]+a[1]*b[ 9]+a[2]*b[10]+a[3]*b[11];
	c[3]=a[0]*b[12]+a[1]*b[13]+a[2]*b[14]+a[3]*b[15];
}

void Mat4F_TransformPoint34G(
	float *point, float *xform, float *org)
{
	org[0]=(point[0]*xform[0])+(point[1]*xform[4])+
		(point[2]*xform[8])+xform[12];
	org[1]=(point[0]*xform[1])+(point[1]*xform[5])+
		(point[2]*xform[9])+xform[13];
	org[2]=(point[0]*xform[2])+(point[1]*xform[6])+
		(point[2]*xform[10])+xform[14];
	org[3]=(point[0]*xform[3])+(point[1]*xform[7])+
		(point[2]*xform[11])+xform[15];
}


void Mat4F_TransformVector(float *dir, float *xform, float *v)
{
	v[0]=(dir[0]*xform[0])+(dir[1]*xform[4])+(dir[2]*xform[8]);
	v[1]=(dir[0]*xform[1])+(dir[1]*xform[5])+(dir[2]*xform[9]);
	v[2]=(dir[0]*xform[2])+(dir[1]*xform[6])+(dir[2]*xform[10]);
}

void Mat4F_TransformVectorT(float *dir, float *xform, float *v)
{
	v[0]=(dir[0]*xform[0])+(dir[1]*xform[1])+(dir[2]*xform[2]);
	v[1]=(dir[0]*xform[4])+(dir[1]*xform[5])+(dir[2]*xform[6]);
	v[2]=(dir[0]*xform[8])+(dir[1]*xform[9])+(dir[2]*xform[10]);
}

void Mat4F_TransformNormal(
	float *dir, float *xform, float *norm)
{
	float f;

	Mat4F_TransformVector(dir, xform, norm);
	f=(norm[0]*xform[12])+(norm[1]*xform[13])+(norm[2]*xform[14]);
	norm[3]=dir[3]+f;
}

void Mat4F_TransformNormalT(
	float *dir, float *xform, float *norm)
{
	float f;

	Mat4F_TransformVectorT(dir, xform, norm);
	f=(dir[0]*xform[12])+(dir[1]*xform[13])+(dir[2]*xform[14]);
	norm[3]=dir[3]-f;
}

void Mat4F_TransformNormalN(
	float *dir, float *xform, float *norm)
{
	float f;

	Mat4F_TransformVector(dir, xform, norm);
	f=(norm[0]*xform[12])+(norm[1]*xform[13])+(norm[2]*xform[14]);
	norm[3]=dir[3]-f;
}

void Mat4F_TransformNormalNT(
	float *dir, float *xform, float *norm)
{
	float f;

	Mat4F_TransformVectorT(dir, xform, norm);
	f=(norm[0]*xform[12])+(norm[1]*xform[13])+(norm[2]*xform[14]);
	norm[3]=dir[3]-f;
}

void Mat4F_TransformPoint(
	float *point, float *xform, float *org)
{
	org[0]=(point[0]*xform[0])+(point[1]*xform[4])+
		(point[2]*xform[8])+xform[12];
	org[1]=(point[0]*xform[1])+(point[1]*xform[5])+
		(point[2]*xform[9])+xform[13];
	org[2]=(point[0]*xform[2])+(point[1]*xform[6])+
		(point[2]*xform[10])+xform[14];
}

void Mat4F_TransformPointT(
	float *point, float *xform, float *org)
{
	org[0]=(point[0]*xform[0])+(point[1]*xform[1])+
		(point[2]*xform[2])+xform[12];
	org[1]=(point[0]*xform[4])+(point[1]*xform[5])+
		(point[2]*xform[6])+xform[13];
	org[2]=(point[0]*xform[8])+(point[1]*xform[9])+
		(point[2]*xform[10])+xform[14];
}

void Mat4F_TransformPointN(
	float *point, float *xform, float *org)
{
	org[0]=(point[0]*xform[0])+(point[1]*xform[4])+
		(point[2]*xform[8])-xform[12];
	org[1]=(point[0]*xform[1])+(point[1]*xform[5])+
		(point[2]*xform[9])-xform[13];
	org[2]=(point[0]*xform[2])+(point[1]*xform[6])+
		(point[2]*xform[10])-xform[14];
}

void Mat4F_TransformPointNT(
	float *point, float *xform, float *org)
{
	org[0]=(point[0]*xform[0])+(point[1]*xform[1])+
		(point[2]*xform[2])-xform[12];
	org[1]=(point[0]*xform[4])+(point[1]*xform[5])+
		(point[2]*xform[6])-xform[13];
	org[2]=(point[0]*xform[8])+(point[1]*xform[9])+
		(point[2]*xform[10])-xform[14];
}

void Mat4F_TransformPointLocal(
	float *point, float *xform, float *org)
{
	float pt[3];

	V3F_SUB(point, xform+3*4, pt);
	org[0]=(pt[0]*xform[0])+(pt[1]*xform[1])+(pt[2]*xform[2]);
	org[1]=(pt[0]*xform[4])+(pt[1]*xform[5])+(pt[2]*xform[6]);
	org[2]=(pt[0]*xform[8])+(pt[1]*xform[9])+(pt[2]*xform[10]);
}

void Mat4F_TransformPointLocalT(
	float *point, float *xform, float *org)
{
	float pt[3];

	V3F_SUB(point, xform+3*4, pt);
	org[0]=(pt[0]*xform[0])+(pt[1]*xform[4])+(pt[2]*xform[8]);
	org[1]=(pt[0]*xform[1])+(pt[1]*xform[5])+(pt[2]*xform[9]);
	org[2]=(pt[0]*xform[2])+(pt[1]*xform[6])+(pt[2]*xform[10]);
}


void Mat4F_AxisTo4Matrix(float *a, float th, float *b)
{
	float xx, xy, xz, xw, xv, yy, yz, yw, yv, zz, zw, zv, ww, wv;
	float v[5];
	float sa, ca;
	int i;

	sa=sin(th/2);
	ca=cos(th/2);
	V4F_NORMALIZE(a, v);


	for(i=0; i<16; i++)b[i]=0;
	for(i=0; i<4; i++)b[(i*4)+i]=1;

	v[0]=v[0]*sa; v[1]=v[1]*sa; v[2]=v[2]*sa; v[3]=v[3]*sa; v[4]=ca;

	xx=v[0]*v[0];	xy=v[0]*v[1];	xz=v[0]*v[2];	xw=v[0]*v[3];	xv=v[0]*v[4];
			yy=v[1]*v[1];	yz=v[1]*v[2];	yw=v[1]*v[3];	yv=v[1]*v[4];
					zz=v[2]*v[2];	zw=v[2]*v[3];	zv=v[2]*v[4];
							ww=v[2]*v[3];	wv=v[3]*v[4];

	//I have no idea...
//	b[ 0]=1-3*(yy+zz+ww);	b[ 1]=  2*(xy+wv);	b[ 2]=  2*(xz-zv);	b[ 3]=  2*(xw+yv);
//	b[ 4]=  2*(xy-wv);	b[ 5]=1-3*(xx+zz+ww);	b[ 6]=  2*(yz+xv);	b[ 7]=  2*(yw-yv);
//	b[ 8]=  2*(xz+zv);	b[ 9]=  2*(yz-xv);	b[10]=1-3*(xx+yy+ww);	b[11]=  2*(zw+xv);
//	b[12]=  2*(xw-yv);	b[13]=  2*(yw-yv);	b[14]=  2*(zw+xv);	b[15]=1-3*(xx+yy+zz);

	b[ 0]=1-2*(yy+zz);	b[ 1]=  2*(xy+zv);	b[ 2]=  2*(xz-yv);
	b[ 4]=  2*(xy-zv);	b[ 5]=1-2*(xx+zz);	b[ 6]=  2*(yz+xv);
	b[ 8]=  2*(xz+yv);	b[ 9]=  2*(yz-xv);	b[10]=1-2*(xx+yy);
}

void Mat4F_AxisTo4MatrixT(float *a, float th, float *b)
{
	float xx, xy, xz, xw, xv, yy, yz, yw, yv, zz, zw, zv, ww, wv;
	float v[5];
	float sa, ca;
	int i;

	sa=sin(th/2);
	ca=cos(th/2);
	V4F_NORMALIZE(a, v);

	for(i=0; i<16; i++)b[i]=0;
	for(i=0; i<4; i++)b[(i*4)+i]=1;

	v[0]=v[0]*sa; v[1]=v[1]*sa; v[2]=v[2]*sa; v[3]=v[3]*sa; v[4]=ca;

	xx=v[0]*v[0];	xy=v[0]*v[1];	xz=v[0]*v[2];
					xw=v[0]*v[3];	xv=v[0]*v[4];
	yy=v[1]*v[1];	yz=v[1]*v[2];	yw=v[1]*v[3];	yv=v[1]*v[4];
	zz=v[2]*v[2];	zw=v[2]*v[3];	zv=v[2]*v[4];
	ww=v[2]*v[3];	wv=v[3]*v[4];

	b[ 0]=1-2*(yy+zz);	b[ 4]=  2*(xy+zv);	b[ 8]=  2*(xz-yv);
	b[ 1]=  2*(xy-zv);	b[ 5]=1-2*(xx+zz);	b[ 9]=  2*(yz+xv);
	b[ 2]=  2*(xz+yv);	b[ 6]=  2*(yz-xv);	b[10]=1-2*(xx+yy);
}

void Mat4F_Filter4Matrix(float *a, float *b)
{
	float v0[4], v1[4], v2[4], v3[4];

	V4F_NORMALIZE(a+0, v0);

	V4F_ADDSCALE(a+4, v0, -V4F_DOT(a+4, v0), v1);
	V4F_NORMALIZE(v1, v1);

	V4F_ADDSCALE(a+8, v0, -V4F_DOT(a+8, v0), v2);
	V4F_ADDSCALE(v2, v1, -V4F_DOT(v2, v1), v2);
	V4F_NORMALIZE(v2, v2);

	V4F_ADDSCALE(a+12, v0, -V4F_DOT(a+12, v0), v3);
	V4F_ADDSCALE(v3, v1, -V4F_DOT(v3, v1), v3);
	V4F_ADDSCALE(v3, v2, -V4F_DOT(v3, v2), v3);
	V4F_NORMALIZE(v3, v3);

	V4F_COPY(v0, b+0);
	V4F_COPY(v1, b+4);
	V4F_COPY(v2, b+8);
	V4F_COPY(v3, b+12);
}

void Mat4F_Rotate4Matrix(
	float *a, float *v, float th, float *b)
{
	float dm[16], tm[16];

	Mat4F_AxisTo4Matrix(v, th, dm);
	Mat4F_MatMult(a, dm, tm);
	Mat4F_Filter4Matrix(tm, b);
}


void Mat4F_Set4MatrixRotXY(float *a, float th)
{
	Mat4F_Identity(a);
	a[0]=cos(th); a[1]=-sin(th);
	a[4]=sin(th); a[5]=cos(th);
}

void Mat4F_Set4MatrixRotXZ(float *a, float th)
{
	Mat4F_Identity(a);
	a[ 0]=cos(th); a[ 2]=-sin(th);
	a[ 8]=sin(th); a[10]=cos(th);
}

void Mat4F_Set4MatrixRotYZ(float *a, float th)
{
	Mat4F_Identity(a);
	a[ 5]=cos(th); a[ 6]=-sin(th);
	a[ 9]=sin(th); a[10]=cos(th);
}

void Mat4F_Set4MatrixRotXW(float *a, float th)
{
	Mat4F_Identity(a);
	a[ 0]=cos(th); a[ 3]=-sin(th);
	a[12]=sin(th); a[15]=cos(th);
}

void Mat4F_Set4MatrixRotYW(float *a, float th)
{
	Mat4F_Identity(a);
	a[ 5]=cos(th); a[ 7]=-sin(th);
	a[13]=sin(th); a[15]=cos(th);
}

void Mat4F_Set4MatrixRotZW(float *a, float th)
{
	Mat4F_Identity(a);
	a[10]=cos(th); a[11]=-sin(th);
	a[14]=sin(th); a[15]=cos(th);
}

void Mat4F_Rotate4MatrixXY(float *a, float th, float *b)
	{ float t[16]; Mat4F_Set4MatrixRotXY(t, th); Mat4F_MatMult(a, t, b); }
void Mat4F_Rotate4MatrixXZ(float *a, float th, float *b)
	{ float t[16]; Mat4F_Set4MatrixRotXZ(t, th); Mat4F_MatMult(a, t, b); }
void Mat4F_Rotate4MatrixYZ(float *a, float th, float *b)
	{ float t[16]; Mat4F_Set4MatrixRotYZ(t, th); Mat4F_MatMult(a, t, b); }
void Mat4F_Rotate4MatrixXW(float *a, float th, float *b)
	{ float t[16]; Mat4F_Set4MatrixRotXW(t, th); Mat4F_MatMult(a, t, b); }
void Mat4F_Rotate4MatrixYW(float *a, float th, float *b)
	{ float t[16]; Mat4F_Set4MatrixRotYW(t, th); Mat4F_MatMult(a, t, b); }
void Mat4F_Rotate4MatrixZW(float *a, float th, float *b)
	{ float t[16]; Mat4F_Set4MatrixRotZW(t, th); Mat4F_MatMult(a, t, b); }

void Mat4F_Rotate4MatrixAngles(float *a, float *ang, float *b)
{
	float t[16], r[16];

	Mat4F_Copy(a, r);

	Mat4F_Set4MatrixRotXY(t, ang[2]*(M_PI/180));
	Mat4F_MatMult(r, t, b);
	Mat4F_Copy(b, r);

	Mat4F_Set4MatrixRotYZ(t, ang[0]*(M_PI/180));
	Mat4F_MatMult(r, t, b);
	Mat4F_Copy(b, r);

	Mat4F_Set4MatrixRotXZ(t, ang[1]*(M_PI/180));
	Mat4F_MatMult(r, t, b);
	Mat4F_Copy(b, r);

	Mat4F_Set4MatrixRotXW(t, ang[3]*(M_PI/180));
	Mat4F_MatMult(r, t, b);
	Mat4F_Copy(b, r);

	Mat4F_Set4MatrixRotYW(t, ang[4]*(M_PI/180));
	Mat4F_MatMult(r, t, b);
	Mat4F_Copy(b, r);

	Mat4F_Set4MatrixRotZW(t, ang[5]*(M_PI/180));
	Mat4F_MatMult(r, t, b);
	Mat4F_Copy(b, r);
}

void Mat4F_SetupFrustum(float *mat,
	float left, float right,
	float bottom, float top,
	float nearval, float farval)
{
	float x, y, a, b, c, d;

	x=(2.0F*nearval) / (right-left);
	y=(2.0F*nearval) / (top-bottom);
	a=(right+left) / (right-left);
	b=(top+bottom) / (top-bottom);
	c=-(farval+nearval) / ( farval-nearval);
	d=-(2.0F*farval*nearval) / (farval-nearval);

	mat[0+4*0]=   x; mat[0+4*1]=0.0F; mat[0+4*2]=    a; mat[0+4*3]=0.0F;
	mat[1+4*0]=0.0F; mat[1+4*1]=   y; mat[1+4*2]=    b; mat[1+4*3]=0.0F;
	mat[2+4*0]=0.0F; mat[2+4*1]=0.0F; mat[2+4*2]=    c; mat[2+4*3]=   d;
	mat[3+4*0]=0.0F; mat[3+4*1]=0.0F; mat[3+4*2]=-1.0F; mat[3+4*3]=0.0F;
}

void Mat4F_SetupOrtho(float *mat,
	float left, float right,
	float bottom, float top,
	float nearval, float farval)
{
	float x, y, a, b, c, d;

	x=2.0F / (right-left);
	y=2.0F / (top-bottom);
	a=-(right+left) / (right-left);
	b=-(top+bottom) / (top-bottom);
	c=-2.0F / (farval-nearval);
	d=-(farval+nearval) / (farval-nearval);

	mat[0+4*0]=   x; mat[0+4*1]=0.0F; mat[0+4*2]=0.0F; mat[0+4*3]=   a;
	mat[1+4*0]=0.0F; mat[1+4*1]=   y; mat[1+4*2]=0.0F; mat[1+4*3]=   b;
	mat[2+4*0]=0.0F; mat[2+4*1]=0.0F; mat[2+4*2]=   c; mat[2+4*3]=   d;
	mat[3+4*0]=0.0F; mat[3+4*1]=0.0F; mat[3+4*2]=0.0F; mat[3+4*3]=1.0F;
}
