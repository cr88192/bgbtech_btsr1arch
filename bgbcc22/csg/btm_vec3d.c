btm_vec2f btm_mkvec2f(double x, double y)
{
	btm_vec2f t;
	t.x=x; t.y=y;
	return(t);
}

btm_vec3f btm_mkvec3f(double x, double y, double z)
{
	btm_vec3f t;
	t.x=x; t.y=y; t.z=z;
	return(t);
}

btm_vec4f btm_mkvec4f(double x, double y, double z, double w)
{
	btm_vec4f t;
	t.x=x; t.y=y; t.z=z; t.w=w;
	return(t);
}

btm_vec3f btm_mkvec3fv(float *fv)
{
	btm_vec3f t;
	t.x=fv[0]; t.y=fv[1]; t.z=fv[2];
	return(t);
}

void btm_stvec3fv(float *fv, btm_vec3f a)
{
	fv[0]=a.x;
	fv[1]=a.y;
	fv[2]=a.z;
}

double btm_v3f_x(btm_vec3f a)
	{ return(a.x); }
double btm_v3f_y(btm_vec3f a)
	{ return(a.y); }
double btm_v3f_z(btm_vec3f a)
	{ return(a.z); }

double btm_v4f_x(btm_vec4f a)
	{ return(a.x); }
double btm_v4f_y(btm_vec4f a)
	{ return(a.y); }
double btm_v4f_z(btm_vec4f a)
	{ return(a.z); }
double btm_v4f_w(btm_vec4f a)
	{ return(a.w); }

btm_vec3f btm_v4f_xyz(btm_vec4f a)
{
	btm_vec3f t;
	t.x=a.x;
	t.y=a.y;
	t.z=a.z;
	return(t);
}


btm_vec3f btm_v3f_add(btm_vec3f a, btm_vec3f b)
{
	btm_vec3f t;
	t.x=a.x+b.x;
	t.y=a.y+b.y;
	t.z=a.z+b.z;
	return(t);
}

btm_vec3f btm_v3f_sub(btm_vec3f a, btm_vec3f b)
{
	btm_vec3f t;
	t.x=a.x-b.x;
	t.y=a.y-b.y;
	t.z=a.z-b.z;
	return(t);
}

double btm_v3f_dot(btm_vec3f a, btm_vec3f b)
{
	double t;
	t=(a.x*b.x)+(a.y*b.y)+(a.z*b.z);
	return(t);
}

double btm_v3f_dot4(btm_vec3f a, btm_vec4f b)
{
	double t;
	t=(a.x*b.x)+(a.y*b.y)+(a.z*b.z);
	return(t);
}

double btm_v3f_ndot(btm_vec3f a, btm_vec4f b)
{
	double t;
	t=(a.x*b.x)+(a.y*b.y)+(a.z*b.z)-b.w;
	return(t);
}

btm_vec3f btm_v3f_cross(btm_vec3f a, btm_vec3f b)
{
	btm_vec3f t;
	t.x=a.x-b.x;
	t.y=a.y-b.y;
	t.z=a.z-b.z;

	t.x=(a.y*b.z)-(a.z*b.y);
	t.y=(a.z*b.x)-(a.x*b.z);
	t.z=(a.x*b.y)-(a.y*b.x);
	return(t);
}

btm_vec3f btm_v3f_scale(btm_vec3f a, double b)
{
	btm_vec3f t;
	t.x=a.x*b;
	t.y=a.y*b;
	t.z=a.z*b;
	return(t);
}

btm_vec3f btm_v3f_addscale(btm_vec3f a, btm_vec3f b, double c)
{
	btm_vec3f t;
	t.x=a.x+(b.x*c);
	t.y=a.y+(b.y*c);
	t.z=a.z+(b.z*c);
	return(t);
}

btm_vec3f btm_v3f_norm(btm_vec3f a)
{
	float d;
	d=sqrt(btm_v3f_dot(a, a));
	if(d==0)
		d=1;
	return(btm_v3f_scale(a, 1.0/d));
}

float btm_v2f_cross(btm_vec2f a, btm_vec2f b)
{
	float c;
	c=(a.x*b.y)-(a.y*b.x);
	return(c);
}


float btm_vec2fv_dist(float *xyz0, float *xyz1)
{
	float dx, dy, dz, d;
	dx=xyz0[0]-xyz1[0];
	dy=xyz0[1]-xyz1[1];
	d=dx*dx+dy*dy;
	d=sqrt(d);
	return(d);
}

float btm_vec3fv_dist(float *xyz0, float *xyz1)
{
	float dx, dy, dz, d;
	dx=xyz0[0]-xyz1[0];
	dy=xyz0[1]-xyz1[1];
	dz=xyz0[2]-xyz1[2];
	d=dx*dx+dy*dy+dz*dz;
	d=sqrt(d);
	return(d);
}

float btm_vec4fv_dist(float *xyz0, float *xyz1)
{
	float dx, dy, dz, dw, d;
	dx=xyz0[0]-xyz1[0];
	dy=xyz0[1]-xyz1[1];
	dz=xyz0[2]-xyz1[2];
	dw=xyz0[3]-xyz1[3];
	d=dx*dx+dy*dy+dz*dz+dw*dw;
	d=sqrt(d);
	return(d);
}

int btm_vec2fv_copy(float *xyzs, float *xyzd)
{
	xyzd[0]=xyzs[0];
	xyzd[1]=xyzs[1];
	return(0);
}

int btm_vec3fv_copy(float *xyzs, float *xyzd)
{
	xyzd[0]=xyzs[0];
	xyzd[1]=xyzs[1];
	xyzd[2]=xyzs[2];
	return(0);
}

int btm_vec4fv_copy(float *xyzs, float *xyzd)
{
	xyzd[0]=xyzs[0];
	xyzd[1]=xyzs[1];
	xyzd[2]=xyzs[2];
	xyzd[3]=xyzs[3];
	return(0);
}

int btm_vec3fv_cross(float *xyza, float *xyzb, float *xyzd)
{
	xyzd[0]=(xyza[1]*xyzb[2])-(xyza[2]*xyzb[1]);
	xyzd[1]=(xyza[2]*xyzb[0])-(xyza[0]*xyzb[2]);
	xyzd[2]=(xyza[0]*xyzb[1])-(xyza[1]*xyzb[0]);
	return(0);
}

float btm_vec3fv_dot(float *xyza, float *xyzb)
{
	float d;
	d=(xyza[0]*xyzb[0])+(xyza[1]*xyzb[1])+(xyza[2]*xyzb[2]);
	return(d);
}

float btm_vec3fv_norm(float *xyzs, float *xyzd)
{
	float d, f;

	d=btm_vec3fv_dot(xyzs, xyzs);
	d=sqrt(d); f=1;
	if(d>0)
		f=1.0/d;
	xyzd[0]=xyzs[0]*f;
	xyzd[1]=xyzs[1]*f;
	xyzd[2]=xyzs[2]*f;
	return(d);
}

int btm_vec3fv_add(float *xyza, float *xyzb, float *xyzd)
{
	xyzd[0]=xyza[0]+xyzb[0];
	xyzd[1]=xyza[1]+xyzb[1];
	xyzd[2]=xyza[2]+xyzb[2];
	return(0);
}

int btm_vec3fv_sub(float *xyza, float *xyzb, float *xyzd)
{
	xyzd[0]=xyza[0]-xyzb[0];
	xyzd[1]=xyza[1]-xyzb[1];
	xyzd[2]=xyza[2]-xyzb[2];
	return(0);
}
