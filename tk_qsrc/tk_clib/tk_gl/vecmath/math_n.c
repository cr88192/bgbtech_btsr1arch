#include <tkgl/bgbrasw.h>

float Vec3F_DistLine(float *pt, float *v0, float *v1)
{
	float nv[3], tv0[3], tv1[3];

	V3F_SUB(v1, v0, nv);
	V3F_NORMALIZE(nv, nv);

	V3F_ADDSCALE(v0, nv, -V3F_DOT(v0, nv), tv0);
	V3F_ADDSCALE(pt, nv, -V3F_DOT(pt, nv), tv1);

	return(V3F_DIST(tv0, tv1));
}

float Vec3F_DistLineSeg(float *pt, float *v0, float *v1)
{
	float nv[3], tv0[3], tv1[3];
	float m, n, d;

	V3F_SUB(v1, v0, nv);
	V3F_NORMALIZE(nv, nv);

	m=V3F_DOT(v0, nv);
	n=V3F_DOT(v1, nv);
	d=V3F_DOT(pt, nv);

	if(d<m)return(V3F_DIST(pt, v0));
	if(d>n)return(V3F_DIST(pt, v1));

	V3F_ADDSCALE(v0, nv, -V3F_DOT(v0, nv), tv0);
	V3F_ADDSCALE(pt, nv, -V3F_DOT(pt, nv), tv1);

	return(V3F_DIST(tv0, tv1));
}
