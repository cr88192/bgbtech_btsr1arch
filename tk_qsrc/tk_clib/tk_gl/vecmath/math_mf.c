#include <tkgl/bgbrasw.h>

//3x3 matrices inside 3x4 matrices

void Mat34F_MatMult(float *a, float *b, float *c)
{
	c[0] =a[0]*b[0] + a[1]*b[4] + a[2]*b[8];
	c[1] =a[0]*b[1] + a[1]*b[5] + a[2]*b[9];
	c[2] =a[0]*b[2] + a[1]*b[6] + a[2]*b[10];
	c[4] =a[4]*b[0] + a[5]*b[4] + a[6]*b[8];
	c[5] =a[4]*b[1] + a[5]*b[5] + a[6]*b[9];
	c[6] =a[4]*b[2] + a[5]*b[6] + a[6]*b[10];
	c[8] =a[8]*b[0] + a[9]*b[4] + a[10]*b[8];
	c[9] =a[8]*b[1] + a[9]*b[5] + a[10]*b[9];
	c[10]=a[8]*b[2] + a[9]*b[6] + a[10]*b[10];
}

void Mat34F_Transpose(float *a, float *b)
{
	b[0]=a[0];  b[1]=a[4];  b[2]=a[8];
	b[4]=a[1];  b[5]=a[5];  b[6]=a[9];
	b[8]=a[2];  b[9]=a[6];  b[10]=a[10];
}

void Mat3to4F_Copy(float *a, float *b)
{
	b[0]=a[0]; b[1]=a[1]; b[2]=a[2]; b[3]=0;
	b[4]=a[3]; b[5]=a[4]; b[6]=a[5]; b[7]=0;
	b[8]=a[6]; b[9]=a[7]; b[10]=a[8]; b[11]=0;
	b[12]=0; b[13]=0; b[14]=0; b[15]=1;
}

void Mat3to4F_Transpose(float *a, float *b)
{
	b[0]=a[0]; b[1]=a[3]; b[2]=a[6]; b[3]=0;
	b[4]=a[1]; b[5]=a[4]; b[6]=a[7]; b[7]=0;
	b[8]=a[2]; b[9]=a[5]; b[10]=a[8]; b[11]=0;
	b[12]=0; b[13]=0; b[14]=0; b[15]=1;
}
