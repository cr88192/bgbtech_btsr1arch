void TKRA_Mat4F_Identity(float *a)
{
	a[0]=1;		a[1]=0;		a[2]=0;		a[3]=0;
	a[4]=0;		a[5]=1;		a[6]=0;		a[7]=0;
	a[8]=0;		a[9]=0;		a[10]=1;	a[11]=0;
	a[12]=0;	a[13]=0;	a[14]=0;	a[15]=1;
}

void TKRA_Mat4F_Copy(float *a, float *b)
{
	b[0]=a[0];   b[1]=a[1];   b[2]=a[2];   b[3]=a[3];
	b[4]=a[4];   b[5]=a[5];   b[6]=a[6];   b[7]=a[7];
	b[8]=a[8];   b[9]=a[9];   b[10]=a[10]; b[11]=a[11];
	b[12]=a[12]; b[13]=a[13]; b[14]=a[14]; b[15]=a[15];
}
void TKRA_Mat4F_MatMult(float *a, float *b, float *c)
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

void TKRA_Mat4F_MatMultT(float *a, float *b, float *c)
{
	TKRA_Mat4F_MatMult(b, a, c);
}

void TKRA_Vec3F_Add(float *a, float *b, float *c)
	{ c[0]=b[0]+a[0]; c[1]=b[1]+a[1]; c[2]=b[2]+a[2]; }
void TKRA_Vec3F_Sub(float *a, float *b, float *c)
	{ c[0]=a[0]-b[0]; c[1]=a[1]-b[1]; c[2]=a[2]-b[2]; }
void TKRA_Vec3F_Scale(float *a, float b, float *c)
	{ c[0]=a[0]*b; c[1]=a[1]*b; c[2]=a[2]*b; }
void TKRA_Vec3F_AddScale(float *a, float *b, float s, float *c)
	{ c[0]=b[0]*s+a[0]; c[1]=b[1]*s+a[1]; c[2]=b[2]*s+a[2]; }
void TKRA_Vec3F_ScaleAddScale(float *a, float t, float *b, float s, float *c)
	{ c[0]=b[0]*s+a[0]*t; c[1]=b[1]*s+a[1]*t; c[2]=b[2]*s+a[2]*t; }


void TKRA_Vec4F_Add(float *a, float *b, float *c)
	{ c[0]=b[0]+a[0]; c[1]=b[1]+a[1]; c[2]=b[2]+a[2]; c[3]=b[3]+a[3]; }
void TKRA_Vec4F_Sub(float *a, float *b, float *c)
	{ c[0]=a[0]-b[0]; c[1]=a[1]-b[1]; c[2]=a[2]-b[2]; c[3]=b[3]-a[3]; }
void TKRA_Vec4F_Scale(float *a, float b, float *c)
	{ c[0]=a[0]*b; c[1]=a[1]*b; c[2]=a[2]*b; c[3]=a[3]*b; }
void TKRA_Vec4F_AddScale(float *a, float *b, float s, float *c)
	{ c[0]=b[0]*s+a[0]; c[1]=b[1]*s+a[1]; c[2]=b[2]*s+a[2]; c[3]=b[3]*s+a[3]; }
void TKRA_Vec4F_ScaleAddScale(float *a, float t, float *b, float s, float *c)
	{ c[0]=b[0]*s+a[0]*t; c[1]=b[1]*s+a[1]*t; c[2]=b[2]*s+a[2]*t; c[3]=b[3]*s+a[3]*t; }


void TKRA_Vec2F_Zero(float *b)
	{ b[0]=0; b[1]=0; }
void TKRA_Vec3F_Zero(float *b)
	{ b[0]=0; b[1]=0; b[2]=0; }
void TKRA_Vec4F_Zero(float *b)
	{ b[0]=0; b[1]=0; b[2]=0; b[3]=0; }

void TKRA_Vec2F_Copy(float *a, float *b)
	{ b[0]=a[0]; b[1]=a[1]; }
void TKRA_Vec3F_Copy(float *a, float *b)
	{ b[0]=a[0]; b[1]=a[1]; b[2]=a[2]; }
void TKRA_Vec4F_Copy(float *a, float *b)
	{ b[0]=a[0]; b[1]=a[1]; b[2]=a[2]; b[3]=a[3]; }

void TKRA_Vec3F_Min(float *a, float *b, float *c)
{
	c[0]=fmin(b[0], a[0]);
	c[1]=fmin(b[1], a[1]);
	c[2]=fmin(b[2], a[2]);
}

void TKRA_Vec3F_Max(float *a, float *b, float *c)
{
	c[0]=fmax(b[0], a[0]);
	c[1]=fmax(b[1], a[1]);
	c[2]=fmax(b[2], a[2]);
}

void TKRA_Vec2F_Set(float *b, float x, float y)
	{ b[0]=x; b[1]=y; }
void TKRA_Vec3F_Set(float *b, float x, float y, float z)
	{ b[0]=x; b[1]=y; b[2]=z; }
void TKRA_Vec4F_Set(float *b, float x, float y, float z, float w)
	{ b[0]=x; b[1]=y; b[2]=z; b[3]=w; }


float TKRA_Vec3F_DotProduct(float *a, float *b)
	{ return((a[0]*b[0])+(a[1]*b[1])+(a[2]*b[2])); }
float TKRA_Vec4F_DotProduct(float *a, float *b)
	{ return((a[0]*b[0])+(a[1]*b[1])+(a[2]*b[2])+(a[3]*b[3])); }

float TKRA_Vec3F_NDotProduct(float *a, float *b)
	{ return((a[0]*b[0])+(a[1]*b[1])+(a[2]*b[2])-b[3]); }

float TKRA_Vec3F_Normalize(float *a, float *b)
{
	float f, g;
	f=sqrt(TKRA_Vec3F_DotProduct(a, a));
	g=(f==0)?1:f;
	b[0]=a[0]/g;
	b[1]=a[1]/g;
	b[2]=a[2]/g;
	return(f);
}

float TKRA_Vec4F_Normalize(float *a, float *b)
{
	float f, g;

	f=sqrt(TKRA_Vec4F_DotProduct(a, a));
	g=(f==0)?1:f;
	b[0]=a[0]/g;
	b[1]=a[1]/g;
	b[2]=a[2]/g;
	b[3]=a[3]/g;
	return(f);
}

void TKRA_Vec3F_CrossProduct(float *a, float *b, float *c)
{
	c[0]=(a[1]*b[2])-(a[2]*b[1]);
	c[1]=(a[2]*b[0])-(a[0]*b[2]);
	c[2]=(a[0]*b[1])-(a[1]*b[0]);
}

void TKRA_Mat4F_AxisTo4MatrixT(float *a, float th, float *b)
{
	float xx, xy, xz, xw, xv, yy, yz, yw, yv, zz, zw, zv, ww, wv;
	float v[5];
	float sa, ca;
	int i;

	sa=sin(th/2);
	ca=cos(th/2);
	TKRA_Vec4F_Normalize(a, v);

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

void TKRA_Mat4F_SetupFrustum(float *mat,
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

void TKRA_Mat4F_SetupOrtho(float *mat,
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

float TKRA_Vec4F_Distance(float *a, float *b)
{
	float tv[4];
	float f, g;

	TKRA_Vec4F_Sub(a, b, tv);
	f=sqrt(TKRA_Vec4F_DotProduct(tv, tv));
	return(f);
}
