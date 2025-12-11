
BTM_SolidMesh *mesh_root;

#if 0
int BTM_DumpMeshListStl()
{
	BTM_SolidMesh *cur;
	
	cur=mesh_root;
	while(cur)
	{
		if(cur->fname)
			BTM_DumpTrisStl(cur->fname, cur->tris, cur->nTris);
		cur=cur->next;
	}
}
#endif

BTM_SolidMesh *BTM_AllocMesh()
{
	BTM_SolidMesh *tmp;
	tmp=btm_malloc(sizeof(BTM_SolidMesh));
	memset(tmp, 0, sizeof(BTM_SolidMesh));
	return(tmp);
}

int BTM_FreeMesh(BTM_SolidMesh *mesh)
{
	if(mesh->tris)
		btm_free(mesh->tris);
	free(mesh);
	return(0);
}

int BTM_DestroyMeshList()
{
	BTM_SolidMesh *cur, *nxt;
	
	cur=mesh_root;
	mesh_root=NULL;
	while(cur)
	{
		nxt=cur->next;
		BTM_FreeMesh(cur);
		cur=nxt;
	}
	return(0);
}

BTM_SolidMesh *BTM_NewMesh(char *name, char *fname)
{
	BTM_SolidMesh *tmp;
	
	tmp=BTM_AllocMesh();
	tmp->name=bccx_strdup(name);
	tmp->fname=bccx_strdup(fname);

	tmp->next=mesh_root;
	mesh_root=tmp;
	return(tmp);
}

BTM_SolidMesh *BTM_LookupMesh(char *name)
{
	BTM_SolidMesh *cur;
	
	cur=mesh_root;
	while(cur)
	{
		if(cur->name && !strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BTM_SolidMesh *BTM_LookupMeshList(
	BTM_SolidMesh *lst, char *name)
{
	BTM_SolidMesh *cur;
	
	cur=lst;
	while(cur)
	{
		if(cur->name && !strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}


int BTM_DeleteMesh(char *name)
{
	BTM_SolidMesh *cur, *nxt, *prv;
	
	cur=mesh_root; prv=NULL;
	while(cur)
	{
		nxt=cur->next;
		if(cur->name && !strcmp(cur->name, name))
		{
			if(prv)
				{ prv->next=nxt; }
			else
				{ mesh_root=nxt; }
			BTM_FreeMesh(cur);
			cur=nxt;
			continue;
		}
		prv=cur; cur=nxt;
	}
	return(0);
}

int BTM_TransformMesh(
	BTM_SolidMesh *mesh, float *mat)
{
	float x, y, z;
	float x1, y1, z1;
	int ntris;
	int i, j, k;

	ntris=mesh->nTris;
	for(i=0; i<(ntris*3); i++)
	{
		x=mesh->tris[i*3+0];
		y=mesh->tris[i*3+1];
		z=mesh->tris[i*3+2];

		x1=(x*mat[0])+(y*mat[4])+(z*mat[ 8]);
		y1=(x*mat[1])+(y*mat[5])+(z*mat[ 9]);
		z1=(x*mat[2])+(y*mat[6])+(z*mat[10]);

		mesh->tris[i*3+0]=x1;
		mesh->tris[i*3+1]=y1;
		mesh->tris[i*3+2]=z1;
	}

	return(0);
}

int BTM_InvertMesh(BTM_SolidMesh *mesh,
	float x, float y, float z)
{
	btm_vec3f v0, v1, v2, nv;
	float d0, d1, d2;
	int ntris;
	int i, j, k;

	nv=btm_mkvec3f(x, y, z);
	nv=btm_v3f_norm(nv);

	ntris=mesh->nTris;
	for(i=0; i<ntris; i++)
	{
		v0=btm_mkvec3fv(mesh->tris+i*9+0);
		v1=btm_mkvec3fv(mesh->tris+i*9+3);
		v2=btm_mkvec3fv(mesh->tris+i*9+6);
	
		d0=btm_v3f_dot(v0, nv);
		d1=btm_v3f_dot(v1, nv);
		d2=btm_v3f_dot(v2, nv);
	
		v0=btm_v3f_addscale(v0, nv, -2*d0);
		v1=btm_v3f_addscale(v1, nv, -2*d1);
		v2=btm_v3f_addscale(v2, nv, -2*d2);
	
		btm_stvec3fv(mesh->tris+i*9+0, v2);
		btm_stvec3fv(mesh->tris+i*9+3, v1);
		btm_stvec3fv(mesh->tris+i*9+6, v0);
	}

	return(0);
}

int BTM_ProjectMeshAxis(BTM_SolidMesh *mesh,
	btm_vec3f vec, float *rm, float *rn)
{
	float dv[3];
	float x, y, z;
	float f, g, m, n;
	int ntris;
	int i, j, k;

	btm_stvec3fv(dv, vec);

	ntris=mesh->nTris; m=999999999; n=-999999999;
	for(i=0; i<(ntris*3); i++)
	{
		x=mesh->tris[i*3+0];
		y=mesh->tris[i*3+1];
		z=mesh->tris[i*3+2];

		f=(x*dv[0])+(y*dv[1])+(z*dv[2]);
		if(f<m)m=f;
		if(f>n)n=f;
	}
	
	if(rm)*rm=m;
	if(rn)*rn=n;
	return(0);
}

int BTM_ProjectMeshAxisPlane(
	BTM_SolidMesh *mesh,
	btm_vec4f plane, float *rm, float *rn)
{
	btm_vec3f norm;
	float d, m, n;
	
	norm=btm_v4f_xyz(plane);
	d=btm_v4f_w(plane);
	BTM_ProjectMeshAxis(mesh, norm, &m, &n);
	m=m-d;	n=n-d;
	
	if(rm)*rm=m;
	if(rn)*rn=n;
	return(0);
}

btm_vec4f BTM_CalcTrianglePlane(
	btm_vec3f v0, btm_vec3f v1, btm_vec3f v2)
{
	btm_vec3f dv0, dv1, dvn;
	btm_vec4f pl;
	float d;
	
	dv0=btm_v3f_sub(v1, v0);
	dv1=btm_v3f_sub(v2, v0);
	dvn=btm_v3f_cross(dv0, dv1);
	dvn=btm_v3f_norm(dvn);
	d=btm_v3f_dot(v0, dvn);
	
	pl=btm_mkvec4f(btm_v3f_x(dvn), btm_v3f_y(dvn), btm_v3f_z(dvn), d);
	return(pl);
}

btm_vec3f BTM_LinePlaneIntersect(
	btm_vec4f pl, btm_vec3f vs, btm_vec3f ve)
{
	btm_vec3f vv, vp;
	float d, ds, de;
	
	vv=btm_v3f_sub(ve, vs);
	ds=btm_v3f_ndot(vs, pl);
	de=btm_v3f_ndot(ve, pl);
	
	//if above plane, return closer point
	if((ds>=0) && (de>=0))
	{
		if(ds<de)
			return(vs);
		return(ve);
	}

	//if below plane, return closer point
	if((ds<=0) && (de<=0))
	{
		if(ds>de)
			return(vs);
		return(ve);
	}
	
	d=btm_v3f_dot4(vv, pl);
	vp=btm_v3f_add(vs, btm_v3f_scale(vv, -ds/d));
	return(vp);
}

void BTM_CalcTrianglePlaneExtents(
	btm_vec4f pl, btm_vec3f v0, btm_vec3f v1, btm_vec3f v2,
	float *rm, float *rn)
{
	float d0, d1, d2;
	float m, n;
	
	d0=btm_v3f_ndot(v0, pl);
	d1=btm_v3f_ndot(v1, pl);
	d2=btm_v3f_ndot(v2, pl);
	
	m=d0; n=d0;
	if(d1<m)m=d1;
	if(d1>n)n=d1;
	if(d2<m)m=d2;
	if(d2>n)n=d2;
	if(rm)*rm=m;
	if(rn)*rn=n;
}

int BTM_CheckTriangleLine(
	btm_vec3f v0, btm_vec3f v1, btm_vec3f v2,
	btm_vec3f vs, btm_vec3f ve, float rad)
{
	btm_vec3f dv0, dv1, dvn;
	btm_vec3f ev0, ev1, ev2;
	btm_vec3f vn0, vn1, vn2;
	btm_vec3f vpi;
	btm_vec4f pl, epl0, epl1, epl2;
	float d0, d1, d2;
	float d, ds, de;
	
	dv0=btm_v3f_sub(v1, v0);
	dv1=btm_v3f_sub(v2, v0);
	dvn=btm_v3f_cross(dv0, dv1);
	dvn=btm_v3f_norm(dvn);
	d=btm_v3f_dot(v0, dvn);
	pl=btm_mkvec4f(btm_v3f_x(dvn), btm_v3f_y(dvn), btm_v3f_z(dvn), d);

	ds=btm_v3f_ndot(vs, pl);
	de=btm_v3f_ndot(ve, pl);
	
	//check if above or below plane
	if(((ds+rad)<=0) && ((de+rad)<=0))	return(0);
	if(((ds-rad)>=0) && ((de-rad)>=0))	return(0);
	
	//calculate intersection
	vpi=BTM_LinePlaneIntersect(pl, vs, ve);

	//calculate edge vectors
	ev0=btm_v3f_sub(v1, v0);
	ev1=btm_v3f_sub(v2, v1);
	ev2=btm_v3f_sub(v0, v2);
	vn0=btm_v3f_norm(btm_v3f_cross(ev0, dvn));
	vn1=btm_v3f_norm(btm_v3f_cross(ev1, dvn));
	vn2=btm_v3f_norm(btm_v3f_cross(ev2, dvn));

	//calculate edge planes
	epl0=btm_mkvec4f(btm_v3f_x(vn0), btm_v3f_y(vn0), btm_v3f_z(vn0), btm_v3f_dot(v0, vn0));
	epl1=btm_mkvec4f(btm_v3f_x(vn1), btm_v3f_y(vn1), btm_v3f_z(vn1), btm_v3f_dot(v1, vn1));
	epl2=btm_mkvec4f(btm_v3f_x(vn2), btm_v3f_y(vn2), btm_v3f_z(vn2), btm_v3f_dot(v2, vn2));
	
	//check intersection against edges
	d0=btm_v3f_ndot(vpi, epl0)-rad;
	d1=btm_v3f_ndot(vpi, epl1)-rad;
	d2=btm_v3f_ndot(vpi, epl2)-rad;
	if(d0>0)return(0);
	if(d1>0)return(0);
	if(d2>0)return(0);
	return(1);
}

int BTM_CheckTriangleTriangleA(
	btm_vec3f v0, btm_vec3f v1, btm_vec3f v2,
	btm_vec3f v0b, btm_vec3f v1b, btm_vec3f v2b)
{
	btm_vec3f dv0, dv1, dvn;
	btm_vec3f ev0, ev1, ev2;
	btm_vec3f vn0, vn1, vn2;
	btm_vec4f pl, epl0, epl1, epl2;
	float d0, d1, d2;
	float d, ds, de;
	
	dv0=btm_v3f_sub(v1, v0);
	dv1=btm_v3f_sub(v2, v0);
	dvn=btm_v3f_cross(dv0, dv1);
	dvn=btm_v3f_norm(dvn);
	d=btm_v3f_dot(v0, dvn);
	pl=btm_mkvec4f(btm_v3f_x(dvn), btm_v3f_y(dvn), btm_v3f_z(dvn), d);

	BTM_CalcTrianglePlaneExtents(pl, v0b, v1b, v2b, &ds, &de);
	
	//check if above or below plane
	if(de<=0)	return(0);
	if(ds>=0)	return(0);
	
	//calculate edge vectors
	ev0=btm_v3f_sub(v1, v0);
	ev1=btm_v3f_sub(v2, v1);
	ev2=btm_v3f_sub(v0, v2);
	vn0=btm_v3f_norm(btm_v3f_cross(ev0, dvn));
	vn1=btm_v3f_norm(btm_v3f_cross(ev1, dvn));
	vn2=btm_v3f_norm(btm_v3f_cross(ev2, dvn));

	//calculate edge planes
	epl0=btm_mkvec4f(btm_v3f_x(vn0), btm_v3f_y(vn0), btm_v3f_z(vn0), btm_v3f_dot(v0, vn0));
	epl1=btm_mkvec4f(btm_v3f_x(vn1), btm_v3f_y(vn1), btm_v3f_z(vn1), btm_v3f_dot(v1, vn1));
	epl2=btm_mkvec4f(btm_v3f_x(vn2), btm_v3f_y(vn2), btm_v3f_z(vn2), btm_v3f_dot(v2, vn2));
	
	//check intersection against edges

	BTM_CalcTrianglePlaneExtents(epl0, v0b, v1b, v2b, &d0, NULL);
	BTM_CalcTrianglePlaneExtents(epl1, v0b, v1b, v2b, &d1, NULL);
	BTM_CalcTrianglePlaneExtents(epl2, v0b, v1b, v2b, &d2, NULL);
	if(d0>0)return(0);
	if(d1>0)return(0);
	if(d2>0)return(0);
	return(1);
}

int BTM_CheckTriangleTriangle(
	btm_vec3f v0a, btm_vec3f v1a, btm_vec3f v2a,
	btm_vec3f v0b, btm_vec3f v1b, btm_vec3f v2b)
{
	if(!BTM_CheckTriangleTriangleA(v0a, v1a, v2a, v0b, v1b, v2b))
		return(0);
	if(!BTM_CheckTriangleTriangleA(v0b, v1b, v2b, v0a, v1a, v2a))
		return(0);
	return(1);
}

int BTM_ClipFace(btm_vec4f norm,
	btm_vec3f *ipts, btm_vec3f *opts, int num)
{
	int i, j, k, l;

	//first outside
	for(i=0; i<num; i++)
		if(btm_v3f_ndot(ipts[i], norm)>0)
			break;

	if(i==num)	//nothing to clip
	{
		for(i=0; i<num; i++)
			opts[i]=ipts[i];
		return(num);	//nothing to clip
	}

	//first inside
	j=i;
	while(1)
	{
		if(btm_v3f_ndot(ipts[j], norm)<=0)
			break;
		j=(j+1)%num;
		if(j==i)return(0);	//everything clipped
	}

	//copy inside
	i=j;
	l=0;
	while(1)
	{
		opts[l++]=ipts[j];

		k=(j+1)%num;
		if(btm_v3f_ndot(ipts[k], norm)>0)
			break;
		j=k;
	}

	//exit point
	if(btm_v3f_ndot(ipts[j], norm)<0)
		{ opts[l++]=BTM_LinePlaneIntersect(norm, ipts[j], ipts[k]); }

	j=k;
	while(1)
	{
		k=(j+1)%num;
		if(btm_v3f_ndot(ipts[k], norm)<=0)
			break;
		j=k;
	}

	//entry point
	if(btm_v3f_ndot(ipts[k], norm)<0)
		{ opts[l++]=BTM_LinePlaneIntersect(norm, ipts[j], ipts[k]); }

	return(l);
}

int BTM_MeshCheckLineCrossings(BTM_SolidMesh *mesh,
	btm_vec3f vs, btm_vec3f ve, float rad)
{
	btm_vec3f v0, v1, v2;
	btm_vec4f pl;
	float m, n;
	int ntris, nc;
	int i, j, k;

	if(mesh->status&BTM_STATUS_CONVEX)
		return(1);
	if(mesh->status&BTM_STATUS_CONCAVE)
		return(0);

	ntris=mesh->nTris; nc=0;
	for(i=0; i<ntris; i++)
	{
		v0=btm_mkvec3fv(mesh->tris+i*9+0);
		v1=btm_mkvec3fv(mesh->tris+i*9+3);
		v2=btm_mkvec3fv(mesh->tris+i*9+6);
		
		j=BTM_CheckTriangleLine(v0, v1, v2, vs, ve, 0.1);
		if(j)nc++;
	}
	return(nc);
}

int BTM_MeshIsConvexP(BTM_SolidMesh *mesh)
{
	btm_vec3f v0, v1, v2;
	btm_vec4f pl;
	float m, n;
	int ntris;
	int i, j, k;

	if(mesh->status&BTM_STATUS_CONVEX)
		return(1);
	if(mesh->status&BTM_STATUS_CONCAVE)
		return(0);

	ntris=mesh->nTris;
	for(i=0; i<ntris; i++)
	{
		v0=btm_mkvec3fv(mesh->tris+i*9+0);
		v1=btm_mkvec3fv(mesh->tris+i*9+3);
		v2=btm_mkvec3fv(mesh->tris+i*9+6);
		pl=BTM_CalcTrianglePlane(v0, v1, v2);
		BTM_ProjectMeshAxisPlane(mesh, pl, &m, &n);
		
		if(n>0)
		{
			mesh->status|=BTM_STATUS_CONCAVE;
			return(0);
		}
	}
	
	mesh->status|=BTM_STATUS_CONVEX;
	return(1);
}

//0: Completely Outside
//1: Completely Inside
//2: Crosses Edge
int BTM_CheckSphereInMeshConvexP(BTM_SolidMesh *mesh,
	btm_vec3f org, float rad)
{
	btm_vec3f v0, v1, v2;
	btm_vec4f pl;
	float d, m, n;
	int ntris, fl;
	int i, j, k;

	ntris=mesh->nTris; fl=0;
	for(i=0; i<ntris; i++)
	{
		v0=btm_mkvec3fv(mesh->tris+i*9+0);
		v1=btm_mkvec3fv(mesh->tris+i*9+3);
		v2=btm_mkvec3fv(mesh->tris+i*9+6);
		pl=BTM_CalcTrianglePlane(v0, v1, v2);
//		BTM_ProjectMeshAxisPlane(mesh, pl, &m, &n);
		
		d=btm_v3f_ndot(org, pl);
		m=d-rad; n=d+rad;
		
		if(m>0)		{ return(0); }
		if(n>0)		{ fl|=2; }
		if(n<0)		{ fl|=1; }
	}
	return(fl);
}

//0: Outside
//!0: Inside
int BTM_CheckSphereInMeshP(BTM_SolidMesh *mesh,
	btm_vec3f org, float rad)
{
	btm_vec3f v0, v1, v2;
	btm_vec4f pl;
	float d, m, n;
	int ntris, fl;
	int i, j, k;

	if(BTM_MeshIsConvexP(mesh))
	{
		i=BTM_CheckSphereInMeshConvexP(mesh, org, rad);
		return(i);
	}

	//check if ray-cast has an odd number of hits.
	//if outside, ray will cross an even number of times.
	//if inside, ray will cross an odd number of times.
	v0=btm_mkvec3f(999999, 999999, 999999);
	i=BTM_MeshCheckLineCrossings(mesh, org, v0, rad);
	return(i&1);
}

int BTM_CheckTriangleInMeshP(BTM_SolidMesh *mesh,
	btm_vec3f v0, btm_vec3f v1, btm_vec3f v2)
{
	int i, j, k;
	
	i=BTM_CheckSphereInMeshP(mesh, v0, 0.1);
	j=BTM_CheckSphereInMeshP(mesh, v1, 0.1);
	k=BTM_CheckSphereInMeshP(mesh, v2, 0.1);
	
	if(!i && !j && !k)
		return(0);
	if(i && j && k)
		return(1);
	return(2);
}

int BTM_RebuildMesh_GetVertexIndexBuf(
	float **rxy, float **rst, float **rnv,
	int *rnvtx, int *rmvtx,
	float *vxy, float *vst, float *vnv)
{
	float *xya, *sta, *nva;
	float dxy, dst, dnv, d, bd;
	int nvtx, mvtx;
	int i, j, k, bi;
	
	xya=*rxy;
	sta=*rst;
	nva=*rnv;
	nvtx=*rnvtx;
	mvtx=*rmvtx;
	
	bi=0; bd=999999999.0;
	for(i=0; i<nvtx; i++)
	{
		dxy=btm_vec3fv_dist(xya+i*3, vxy);
		dst=btm_vec2fv_dist(sta+i*2, vst);
		dnv=btm_vec3fv_dist(nva+i*3, vnv);
		d=dxy+dst*0.25+dnv*0.1;
		if(d<bd)
			{ bi=i; bd=d; }
	}
	
	if(bd<0.01)
		return(bi);
	
	if((nvtx+3)>=mvtx)
	{
		k=mvtx+(mvtx>>1);
		xya=btm_realloc(xya, k*3*sizeof(float));
		sta=btm_realloc(sta, k*2*sizeof(float));
		nva=btm_realloc(nva, k*3*sizeof(float));
		mvtx=k;
		*rxy=xya;	*rst=sta;
		*rnv=nva;	*rmvtx=mvtx;
	}
	
	i=nvtx++;
	btm_vec3fv_copy(vxy, xya+i*3);
	btm_vec2fv_copy(vst, sta+i*2);
	btm_vec3fv_copy(vnv, nva+i*3);
	*rnvtx=nvtx;
	return(i);
}

int BTM_RebuildMeshProjectTexture(BTM_SolidMesh *mesh)
{
	float	*tvt_xyz;
	float	*tvt_st;
	float	*tvt_norm;
	u32		*tvt_rgba;
	byte	*tvt_bone;
	int *tvtris;

	float xyz0[3], st0[2];
	float xyz1[3], st1[2];
	float xyz2[3], st2[2];
	float nv[4], tsv[4], ttv[4], bnorg[3];
	u32 meshclr;
	float f, g;
	int nvtx, mvtx, ntris, bnid;
	int i0, i1, i2;
	int i, j, k;
	
	if(mesh->t_vidx)
		return(0);
	
	tsv[0]=mesh->texvec_s[0];
	tsv[1]=mesh->texvec_s[1];
	tsv[2]=mesh->texvec_s[2];
	tsv[3]=mesh->texvec_s[3];
	
	ttv[0]=mesh->texvec_t[0];
	ttv[1]=mesh->texvec_t[1];
	ttv[2]=mesh->texvec_t[2];
	ttv[3]=mesh->texvec_t[3];

	if((mesh->baseclr[3]>0.25) && (mesh->baseclr[3]<=1.0))
	{
		meshclr=
			(((int)(mesh->baseclr[3]*255.0))<<24) |
			(((int)(mesh->baseclr[0]*255.0))<<16) |
			(((int)(mesh->baseclr[1]*255.0))<< 8) |
			(((int)(mesh->baseclr[2]*255.0))<< 0) ;
	}else
	{
		meshclr=0xFFFFFFFFU;
	}

	mvtx=mesh->nTris*2;
	tvt_xyz=btm_malloc(mvtx*3*sizeof(float));
	tvt_st=btm_malloc(mvtx*2*sizeof(float));
	tvt_norm=btm_malloc(mvtx*3*sizeof(float));
	tvt_rgba=btm_malloc(mvtx*1*sizeof(u32));
	tvt_bone=btm_malloc(mvtx*1*sizeof(byte));
	nvtx=0;

	tvtris=btm_malloc(mesh->nTris*3*sizeof(int));
	
	bnorg[0]=0;
	bnorg[1]=0;
	bnorg[2]=0;
	bnid=0;
	
	if(mesh->skel)
	{
		bnid=(mesh->clrmat>>32)&255;
		
		bnorg[0]=mesh->skel->bone_baseorg[bnid*3+0];
		bnorg[1]=mesh->skel->bone_baseorg[bnid*3+1];
		bnorg[2]=mesh->skel->bone_baseorg[bnid*3+2];
	}

	ntris=0;
	for(i=0; i<mesh->nTris; i++)
	{
		xyz0[0]=mesh->tris[(i*3+0)*3+0]*mesh->scale[0];
		xyz0[1]=mesh->tris[(i*3+0)*3+1]*mesh->scale[1];
		xyz0[2]=mesh->tris[(i*3+0)*3+2]*mesh->scale[2];

		xyz1[0]=mesh->tris[(i*3+1)*3+0]*mesh->scale[0];
		xyz1[1]=mesh->tris[(i*3+1)*3+1]*mesh->scale[1];
		xyz1[2]=mesh->tris[(i*3+1)*3+2]*mesh->scale[2];

		xyz2[0]=mesh->tris[(i*3+2)*3+0]*mesh->scale[0];
		xyz2[1]=mesh->tris[(i*3+2)*3+1]*mesh->scale[1];
		xyz2[2]=mesh->tris[(i*3+2)*3+2]*mesh->scale[2];

		xyz0[0]-=bnorg[0];	xyz0[1]-=bnorg[1];	xyz0[2]-=bnorg[2];
		xyz1[0]-=bnorg[0];	xyz1[1]-=bnorg[1];	xyz1[2]-=bnorg[2];
		xyz2[0]-=bnorg[0];	xyz2[1]-=bnorg[1];	xyz2[2]-=bnorg[2];

		nv[0]=mesh->norm[i*4+0];
		nv[1]=mesh->norm[i*4+1];
		nv[2]=mesh->norm[i*4+2];
		nv[3]=mesh->norm[i*4+3];
		
		g=-999; k=0;
		for(j=0; j<mesh->n_texvec; j++)
		{
			f=btm_vec3fv_dot(mesh->texvec_n+j*4, nv);
			if(f>0)
				f=4*f;
			else
				f=-f;
			if(f>g)
				{ g=f; k=j; }
		}
		btm_vec4fv_copy(mesh->texvec_s+k*4, tsv);
		btm_vec4fv_copy(mesh->texvec_t+k*4, ttv);

		st0[0]=(xyz0[0]*tsv[0])+(xyz0[1]*tsv[1])+(xyz0[2]*tsv[2])-tsv[3];
		st0[1]=(xyz0[0]*ttv[0])+(xyz0[1]*ttv[1])+(xyz0[2]*ttv[2])-ttv[3];
		st1[0]=(xyz1[0]*tsv[0])+(xyz1[1]*tsv[1])+(xyz1[2]*tsv[2])-tsv[3];
		st1[1]=(xyz1[0]*ttv[0])+(xyz1[1]*ttv[1])+(xyz1[2]*ttv[2])-ttv[3];
		st2[0]=(xyz2[0]*tsv[0])+(xyz2[1]*tsv[1])+(xyz2[2]*tsv[2])-tsv[3];
		st2[1]=(xyz2[0]*ttv[0])+(xyz2[1]*ttv[1])+(xyz2[2]*ttv[2])-ttv[3];

		while((st0[0]>=1) && (st1[0]>=1) && (st2[0]>=1))
			{ st0[0]-=1; st1[0]-=1; st2[0]-=1; }
		while((st0[1]>=1) && (st1[1]>=1) && (st2[1]>=1))
			{ st0[1]-=1; st1[1]-=1; st2[1]-=1; }

		while((st0[0]<=-1) && (st1[0]<=-1) && (st2[0]<=-1))
			{ st0[0]+=1; st1[0]+=1; st2[0]+=1; }
		while((st0[1]<=-1) && (st1[1]<=-1) && (st2[1]<=-1))
			{ st0[1]+=1; st1[1]+=1; st2[1]+=1; }

		i0=BTM_RebuildMesh_GetVertexIndexBuf(
			&tvt_xyz, &tvt_st, &tvt_norm,
			&nvtx, &mvtx,
			xyz0, st0, nv);
		i1=BTM_RebuildMesh_GetVertexIndexBuf(
			&tvt_xyz, &tvt_st, &tvt_norm,
			&nvtx, &mvtx,
			xyz1, st1, nv);
		i2=BTM_RebuildMesh_GetVertexIndexBuf(
			&tvt_xyz, &tvt_st, &tvt_norm,
			&nvtx, &mvtx,
			xyz2, st2, nv);
		
		if((i0==i1) || (i1==i2) || (i2==i0))
			continue;
		
		tvt_rgba[i0]=meshclr;
		tvt_rgba[i1]=meshclr;
		tvt_rgba[i2]=meshclr;

		tvt_bone[i0]=bnid;
		tvt_bone[i1]=bnid;
		tvt_bone[i2]=bnid;
		
		j=ntris++;
		tvtris[j*3+0]=i0;
		tvtris[j*3+1]=i1;
		tvtris[j*3+2]=i2;
	}

	for(i=0; i<nvtx; i++)
	{
		tvt_rgba[i]=meshclr;
		tvt_bone[i]=bnid;
	}
	
	mesh->v_xyz=tvt_xyz;
	mesh->v_st=tvt_st;
	mesh->v_nv=tvt_norm;
	mesh->v_cl=tvt_rgba;
	mesh->v_bn=tvt_bone;
	mesh->t_vidx=tvtris;
	mesh->n_vtx=nvtx;
	mesh->n_tris=ntris;

	return(0);
}

int BTM_MeshAdjustForSkeleton(BTM_SolidMesh *mesh)
{
	if(!mesh->skel)
		return(0);

	return(0);
}
