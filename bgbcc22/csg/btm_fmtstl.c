BTM_SolidMesh *BTM_ProcCsgToMesh(u64 val, BTM_SolidMesh *olst);
void QuatF_Identity(float *a);

BTM_SolidMesh *BTM_LoadMeshListScadBuffer(byte *ibuf, int ibsz);


double btm_atod(char *s)
{
	return(atof(s));
}

int BTM_MeshBufPuts(char **robuf, char *str)
{
	char *obuf, *obufe, *oct;
	int obsz, oofs, osz1;

	obuf =robuf[0];
	obufe=robuf[1];
	oct  =robuf[2];
	obsz=obufe-obuf;
	
	if(!obuf)
	{
		obsz=4096;
		obuf=btm_malloc(obsz);
		robuf[0]=obuf;
		robuf[1]=obuf+obsz;
		robuf[2]=obuf;
	}
	
	if((oct+strlen(str)+1)>=obufe)
	{
		osz1=obsz+(obsz>>1);
		oofs=oct-obuf;
		obuf=btm_realloc(obuf, osz1);
		obsz=osz1;
		obufe=obuf+obsz;
		oct=obuf+oofs;

		robuf[0]=obuf;
		robuf[1]=obufe;
		robuf[2]=oct;
	}

	strcpy(oct, str);
	oct+=strlen(oct);
	robuf[2]=oct;
	return(0);
}

int BTM_MeshBufPrintf(char **robuf, char *str, ...)
{
	char tbuf[256];
	va_list lst;
	
	va_start(lst, str);
	vsprintf(tbuf, str, lst);
	va_end(lst);
	BTM_MeshBufPuts(robuf, tbuf);
	return(0);
}


int BTM_Mesh_TrisEmitTriangle(float **rtris, int *rntris,
	btm_vec3f v0, btm_vec3f v1, btm_vec3f v2)
{
	float *tris;
	int ntris, mtris;
	int i;

	tris=*rtris;
	ntris=rntris[0];
	mtris=rntris[1];

	if((ntris+1)>=mtris)
	{
		i=mtris+(mtris>>1);
		tris=btm_realloc(tris, i*9*sizeof(float));
		*rtris=tris;
		mtris=i;
		rntris[1]=i;
	}

	tris[ntris*9+0]=v0.x;
	tris[ntris*9+1]=v0.y;
	tris[ntris*9+2]=v0.z;
	tris[ntris*9+3]=v1.x;
	tris[ntris*9+4]=v1.y;
	tris[ntris*9+5]=v1.z;
	tris[ntris*9+6]=v2.x;
	tris[ntris*9+7]=v2.y;
	tris[ntris*9+8]=v2.z;
	rntris[0]=ntris+1;

	return(0);
}

int BTM_Mesh_TrisEmitQuad(float **rtris, int *rntris,
	btm_vec3f v0, btm_vec3f v1, btm_vec3f v2, btm_vec3f v3)
{
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v1, v2);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v2, v3);
	return(0);
}

int BTM_Mesh_TrisEmitPentagon(float **rtris, int *rntris,
	btm_vec3f v0, btm_vec3f v1, btm_vec3f v2, btm_vec3f v3, btm_vec3f v4)
{
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v1, v2);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v2, v3);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v3, v4);
	return(0);
}

int BTM_Mesh_TrisEmitHexagon(float **rtris, int *rntris,
	btm_vec3f v0, btm_vec3f v1, btm_vec3f v2, btm_vec3f v3, btm_vec3f v4, btm_vec3f v5)
{
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v1, v2);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v2, v3);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v3, v4);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v4, v5);
	return(0);
}

int BTM_Mesh_TrisEmitHeptagon(float **rtris, int *rntris,
	btm_vec3f v0, btm_vec3f v1, btm_vec3f v2, btm_vec3f v3, btm_vec3f v4,
	btm_vec3f v5, btm_vec3f v6)
{
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v1, v2);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v2, v3);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v3, v4);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v4, v5);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v5, v6);
	return(0);
}

int BTM_Mesh_TrisEmitOctagon(float **rtris, int *rntris,
	btm_vec3f v0, btm_vec3f v1, btm_vec3f v2, btm_vec3f v3, btm_vec3f v4,
	btm_vec3f v5, btm_vec3f v6, btm_vec3f v7)
{
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v1, v2);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v2, v3);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v3, v4);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v4, v5);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v5, v6);
	BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v6, v7);
	return(0);
}

int BTM_Mesh_TrisEmitPolygon(float **rtris, int *rntris,
	float *vec, int npts)
{
	btm_vec3f v0, v1, v2;
	int i, j, k;

	v0=btm_mkvec3f(vec[0], vec[1], vec[2]);
	for(i=1; i<(npts-1); i++)
	{
		v1=btm_mkvec3f(vec[(i+0)*3+0], vec[(i+0)*3+1], vec[(i+0)*3+2]);
		v2=btm_mkvec3f(vec[(i+1)*3+0], vec[(i+1)*3+1], vec[(i+1)*3+2]);
		BTM_Mesh_TrisEmitTriangle(rtris, rntris, v0, v1, v2);
	}

	return(0);
}

int BTM_DumpTrisRawBuf(char **robuf, float *tris, int ntris)
{
	char buf[256];
	int i, j, k;
	
	for(i=0; i<ntris; i++)
	{
		sprintf(buf, "%f %f %f  %f %f %f  %f %f %f 0xFFFFFF\n",
			tris[i*9+0], tris[i*9+1], tris[i*9+2],
			tris[i*9+3], tris[i*9+4], tris[i*9+5],
			tris[i*9+6], tris[i*9+7], tris[i*9+8]);
		BTM_MeshBufPuts(robuf, buf);
	}
	return(0);
}

int BTM_DumpTrisStlBuf(char **robuf, int *robsz,
	char *sldname, float *tris, int ntris)
{
	char buf[256];
	btm_vec3f v0, v1, v2, v3, v4, v5, v6;
	float x, y, z;
	float xo, yo, zo;
	int i, j, k;

	xo=0; yo=0; zo=0;

	if(!sldname)
		sldname="default";

	BTM_MeshBufPrintf(robuf, "solid %s\n", sldname);
	
	for(i=0; i<ntris; i++)
	{
		v0=btm_mkvec3f(tris[i*9+0], tris[i*9+1], tris[i*9+2]);
		v1=btm_mkvec3f(tris[i*9+3], tris[i*9+4], tris[i*9+5]);
		v2=btm_mkvec3f(tris[i*9+6], tris[i*9+7], tris[i*9+8]);

		v3=btm_v3f_sub(v1, v0);
		v4=btm_v3f_sub(v2, v0);
		v5=btm_v3f_cross(v3, v4);
		v6=btm_v3f_norm(v5);
	
		BTM_MeshBufPrintf(robuf,
			"  facet normal %f %f %f\n", v6.x, v6.y, v6.z);
		BTM_MeshBufPrintf(robuf,
			"    outer loop\n");
		BTM_MeshBufPrintf(robuf,
			"      vertex %f %f %f\n",
			tris[i*9+0]-xo, tris[i*9+1]-yo, tris[i*9+2]-zo);
		BTM_MeshBufPrintf(robuf,
			"      vertex %f %f %f\n",
			tris[i*9+3]-xo, tris[i*9+4]-yo, tris[i*9+5]-zo);
		BTM_MeshBufPrintf(robuf,
			"      vertex %f %f %f\n",
			tris[i*9+6]-xo, tris[i*9+7]-yo, tris[i*9+8]-zo);
		BTM_MeshBufPrintf(robuf,
			"    endloop\n");
		BTM_MeshBufPrintf(robuf,
			"  endfacet\n");
	}

	BTM_MeshBufPrintf(robuf,
		"endsolid %s\n", sldname);
	
	return(0);
}

int BTM_ExportMeshListStlBuf(BTM_SolidMesh *mesh,
	byte **robuf, int *robsz)
{
	BTM_SolidMesh *mcur;

	mcur=mesh;
	while(mcur)
	{
		BTM_DumpTrisStlBuf(robuf, robsz, mcur->name,
			mcur->tris, mcur->nTris);
		mcur=mcur->next;
	}
	return(0);
}

int btm_bufgets(char *dst, int max, char **rcs)
{
	char *cs, *ct;
	cs=*rcs;
	ct=dst;
	while(*cs && (*cs!='\r') && (*cs!='\n'))
	{
		if((*cs=='\\') && ((cs[1]=='\r') || (cs[1]=='\n')))
		{
			cs++;
			if(*cs=='\r')	cs++;
			if(*cs=='\n')	cs++;
			continue;
		}
		*ct++=*cs++;
	}
	*ct=0;
	if(*cs=='\r')	cs++;
	if(*cs=='\n')	cs++;
	*rcs=cs;
	return(0);
}

int BTM_LoadTrisStlAsciiBuf(byte *sbuf, float **rtris, int *rntris)
{
	float pts[16*3];
	char tb[256];
	int ntv[4];
	char **a;
	char *cs;
	float x, y, z;
	float xo, yo, zo;
	int ntris, npts;
	int i, j, k;

	cs=(char *)sbuf;

	if(!rntris)
		rntris=ntv;

	ntris=0;
	while(*cs)
	{
		btm_bufgets(tb, 256, &cs);
		a=bccx_split(tb);
		if(!strcmp(a[0], "facet"))
		{
			npts=0;
			continue;
		}

		if(!strcmp(a[0], "vertex"))
		{
			pts[npts*3+0]=btm_atod(a[1]);
			pts[npts*3+1]=btm_atod(a[2]);
			pts[npts*3+2]=btm_atod(a[3]);
			npts++;
			continue;
		}

		if(!strcmp(a[0], "endfacet"))
		{
			BTM_Mesh_TrisEmitPolygon(rtris, rntris, pts, npts);
			npts=0;
			continue;
		}
	}

	ntris=*rntris;

	return(ntris);
}

int BTM_LoadTrisStlBuf(byte *sbuf, float **rtris, int *rntris)
{
	float pts[16*3];
	unsigned char tb[128];
	int ntv[2];
	byte *cs, *scs;
	float x, y, z;
	float xo, yo, zo;
	int ntris, npts;
	int i, j, k;

	memcpy(tb, sbuf, 84);
	scs=sbuf+84;

	if(!memcmp(tb, "solid ", 6))
	{
		return(BTM_LoadTrisStlAsciiBuf(sbuf, rtris, rntris));
	}

//	printf("LoadTrisStl:Binary %s\n", fname);
	
	ntris=tb[80]|(tb[81]<<8)|(tb[82]<<16)|(tb[83]<<24);
	
	for(i=0; i<ntris; i++)
	{
//		vfread(tb, 1, 50, fd);
		memcpy(tb, scs, 50);
		scs+=50;
		j=tb[48]|(tb[49]<<8);
		
		//0-32767: atribute data size
		//32768-65535: RGB555 color
		if((j>0) && (j<32768))
			scs+=j;

		for(j=0; j<9; j++)
		{
			cs=tb+12+(j*4);
			k=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24);
			pts[j]=*(float *)(&k);
		}
		BTM_Mesh_TrisEmitPolygon(rtris, rntris, pts, 3);
	}

	return(ntris);
}

BTM_SolidMesh *BTM_LoadMeshStlBuf(byte *ibuf, int ibsz, char *fname)
{
//	float dv0[3], dv1[3], dv2[3], dv3[3];
	btm_vec3f v0, v1, v2, v3;
	int ntv[4];
	BTM_SolidMesh *mesh;
	float *tris;
	float f, g;
	int i, j, k;

	tris=btm_malloc(256*9*sizeof(float));
	ntv[0]=0;
	ntv[1]=256;
	
	BTM_LoadTrisStlBuf(ibuf, &tris, ntv);

	mesh=BTM_AllocMesh();
	mesh->fname=bccx_strdup(fname);
	mesh->tris=tris;
	mesh->nTris=ntv[0];
	mesh->mTris=ntv[1];

	mesh->bbox[0]= 999999999.0;
	mesh->bbox[1]= 999999999.0;
	mesh->bbox[2]= 999999999.0;
	mesh->bbox[3]=-999999999.0;
	mesh->bbox[4]=-999999999.0;
	mesh->bbox[5]=-999999999.0;

	mesh->scale[0]=1.0;
	mesh->scale[1]=1.0;
	mesh->scale[2]=1.0;

	mesh->norm=btm_malloc(mesh->mTris*4*sizeof(float));
	for(i=0; i<mesh->nTris; i++)
	{
		for(j=0; j<3; j++)
			for(k=0; k<3; k++)
		{
			f=mesh->tris[i*9+j*3+k];
			if(f<mesh->bbox[0+k])
				mesh->bbox[0+k]=f;
			if(f>mesh->bbox[3+k])
				mesh->bbox[3+k]=f;
		}
	
		v0=btm_v3f_sub(
			btm_mkvec3fv(mesh->tris+i*9+3),
			btm_mkvec3fv(mesh->tris+i*9+0));
		v1=btm_v3f_sub(
			btm_mkvec3fv(mesh->tris+i*9+6),
			btm_mkvec3fv(mesh->tris+i*9+3));
		v2=btm_v3f_cross(v0, v1);
		v3=btm_v3f_norm(v2);

		mesh->norm[i*4+0]=btm_v3f_x(v3);
		mesh->norm[i*4+1]=btm_v3f_y(v3);
		mesh->norm[i*4+2]=btm_v3f_z(v3);
		mesh->norm[i*4+3]=btm_v3f_dot(btm_mkvec3fv(mesh->tris+i*9+0), v3);
	}
	
	return(mesh);
}

BTM_SolidMesh *BTM_LoadMeshStl(char *fname)
{
	byte *ibuf;
	int i, j, k, ibsz;

	ibuf=BTM_LoadFile(fname, &ibsz);
	return(BTM_LoadMeshStlBuf(ibuf, ibsz, fname));
}

BTM_SolidSkel *BTM_LoadBonesI(
	BTM_SolidSkel *skel,
	char *fname)
{
	char tb[256];
	float org[3], ang[3], rot[4], rot9[10];
	BTM_BtModelAnim *acur, *anim;
	byte *ibuf;
	char **a;
	char *tn, *ics, *icse;
	u32 pv;
	int i, j, k, ibsz, id, fr, bn;

	ibuf=BTM_LoadFile(fname, &ibsz);
//	return(BTM_LoadMeshStlBuf(ibuf, ibsz, fname));

	if(!ibuf)
		return(NULL);

	ics=ibuf;
	icse=ics+ibsz;
	anim=NULL;

	while(ics<icse)
	{
		btm_bufgets(tb, 256, &ics);
		a=bccx_split(tb);
		
		if(!a[0])
			continue;


		/* bone id name parent ( ox oy oz ) */
		if(!strcmp(a[0], "bone"))
		{
			id=atoi(a[1]);
			if(id>=skel->n_bones)
				skel->n_bones=id+1;
			skel->bone_name[id]=bccx_strdup(a[2]);
			skel->bone_parent[id]=atoi(a[3]);
			skel->bone_baseorg[id*3+0]=atof(a[5]);
			skel->bone_baseorg[id*3+1]=atof(a[6]);
			skel->bone_baseorg[id*3+2]=atof(a[7]);
			
			QuatF_Identity(skel->bone_baserot+id*4);
			continue;
		}

		/* include filename */
		if(!strcmp(a[0], "include"))
		{
			BTM_LoadBonesI(skel, bccx_strdup(a[1]));
			continue;
		}

		/* animation name frames hz */
		if(!strcmp(a[0], "animation"))
		{
			acur=skel->anim;
			while(acur)
			{
				if(!strcmp(acur->name, a[1]))
					break;
				acur=acur->next;
			}

			if(!acur)
			{
				acur=btm_malloc(sizeof(BTM_BtModelAnim));
				memset(acur, 0, sizeof(BTM_BtModelAnim));
				acur->name=bccx_strdup(a[1]);

				acur->next=skel->anim;
				skel->anim=acur;
				
				acur->n_frames=atoi(a[2]);

				j=atof(a[3])*4;
				if(j<1)		j=1;
				if(j>255)	j=255;
				acur->framerate=j;
				
				acur->n_bones=skel->n_bones;
				
				k=(acur->n_frames+1)*acur->n_bones*sizeof(u32);
				acur->frm_rot=btm_malloc(k);
				acur->frm_org=btm_malloc(k);
				acur->frm_scl=btm_malloc(k);
				memset(acur->frm_rot, 0xFF, k);
				memset(acur->frm_org, 0xFF, k);
				memset(acur->frm_scl, 0xFF, k);
			}

			anim=acur;
			continue;
		}

		/* frane framenum */
		if(!strcmp(a[0], "frame"))
		{
			fr=atoi(a[1]);
			continue;
		}

		/*
		 * arot bone ( ax ay az )
		 * arot bone [ rx ry rz rw ]
		 * arot bone { xx xy xz yx yy yz zx zy zz }
		 */
		if(!strcmp(a[0], "arot"))
		{
			bn=atoi(a[1]);
			
			if(!strcmp(a[2], "("))
			{
				ang[0]=atof(a[3]);
				ang[1]=atof(a[4]);
				ang[2]=atof(a[5]);

				ang[0]=ang[0]*(M_PI/180.0);
				ang[1]=ang[1]*(M_PI/180.0);
				ang[2]=ang[2]*(M_PI/180.0);
				QuatF_FromAnglesB(ang, rot);	
			}

			if(!strcmp(a[2], "["))
			{
				rot[0]=atof(a[3]);
				rot[1]=atof(a[4]);
				rot[2]=atof(a[5]);
				rot[3]=atof(a[6]);
				QuatF_Normalize(rot, rot);
			}
			
			if(!strcmp(a[2], "{"))
			{
				rot9[0]=atof(a[ 3]);
				rot9[1]=atof(a[ 4]);
				rot9[2]=atof(a[ 5]);
				rot9[3]=atof(a[ 6]);
				rot9[4]=atof(a[ 7]);
				rot9[5]=atof(a[ 8]);
				rot9[6]=atof(a[ 9]);
				rot9[7]=atof(a[10]);
				rot9[8]=atof(a[11]);

				QuatF_From3Matrix(rot9, rot);
				QuatF_Normalize(rot, rot);
			}
			
			pv=BTM_MeshEncodePackQuat32(rot);
			
			anim->frm_rot[fr*anim->n_bones+bn]=pv;
			continue;
		}

		if(!strcmp(a[0], "aorg"))
		{
			bn=atoi(a[1]);
			
			if(!strcmp(a[2], "("))
			{
				org[0]=atof(a[3]);
				org[1]=atof(a[4]);
				org[2]=atof(a[5]);
			}

			pv=BTM_MeshEncodePackSe32(org);
			anim->frm_org[fr*anim->n_bones+bn]=pv;
			continue;
		}


		/*
		 * aorot bone ( ox oy oz ) ( ax ay az )
		 * aorot bone ( ox oy oz ) [ rx ry rz rw ]
		 */
		if(!strcmp(a[0], "arot"))
		{
			bn=atoi(a[1]);

			if(!strcmp(a[2], "("))
			{
				org[0]=atof(a[3]);
				org[1]=atof(a[4]);
				org[2]=atof(a[5]);
			}

			if(!strcmp(a[7], "("))
			{
				ang[0]=atof(a[8]);
				ang[1]=atof(a[9]);
				ang[2]=atof(a[10]);

				ang[0]=ang[0]*(M_PI/180.0);
				ang[1]=ang[1]*(M_PI/180.0);
				ang[2]=ang[2]*(M_PI/180.0);
				QuatF_FromAnglesB(ang, rot);	
			}

			if(!strcmp(a[7], "["))
			{
				rot[0]=atof(a[ 8]);
				rot[1]=atof(a[ 9]);
				rot[2]=atof(a[10]);
				rot[3]=atof(a[11]);
				QuatF_Normalize(rot, rot);
			}

			pv=BTM_MeshEncodePackSe32(org);
			anim->frm_org[fr*anim->n_bones+bn]=pv;
			
			pv=BTM_MeshEncodePackQuat32(rot);
			anim->frm_rot[fr*anim->n_bones+bn]=pv;
			continue;
		}
	}

	for(i=0; i<skel->n_bones; i++)
	{
		j=skel->bone_parent[i];
		if(j<0)
			continue;
		TKRA_Vec3F_Sub(
			skel->bone_baseorg+i*3,
			skel->bone_baseorg+j*3,
			skel->bone_relorg +i*3);
	}

	return(skel);
}

BTM_SolidSkel *BTM_LoadBones(char *fname)
{
	BTM_SolidSkel *skel;

	skel=btm_malloc(sizeof(BTM_SolidSkel));
	memset(skel, 0, sizeof(BTM_SolidSkel));

	BTM_LoadBonesI(skel, fname);

	return(skel);
}

#if 1
BTM_SolidMesh *BTM_LoadMeshListBufferMdef(byte *ibuf, int ibsz)
{
	static BTM_ProgCtx *csg_wrl=NULL;
	char tb[256];
	int ntv[4];
	BTM_SolidMesh *mlst, *mcur, *mc1;
	BTM_SolidSkel *skel;
	char **a;
	char *tn, *ics, *icse;
	float x, y, z, w;
	float xo, yo, zo;
	u64 vala;
	int ntris, npts;
	int i, j, k, ix;
	
	ics=ibuf;
	icse=ics+ibsz;

	skel=NULL;
	mlst=NULL;
//	while(!vfeof(fd))
	while(ics<icse)
	{
//		vfgets(tb, 256, fd);
		btm_bufgets(tb, 256, &ics);
		a=bccx_split(tb);
		
		if(!a[0])
			continue;

		if(!strcmp(a[0], "bones"))
		{
			skel=BTM_LoadBones(a[1]);
			continue;
		}
		
		if(!strcmp(a[0], "mesh"))
		{
			tn=bccx_strdup(a[1]);
			mcur=BTM_LoadMeshStl(a[2]);
			mcur->name=tn;
			
			mcur->next=mlst;
			mlst=mcur;
			continue;
		}

		if(!strcmp(a[0], "csgprog"))
		{
#if 1
			if(!csg_wrl)
			{
//				csg_wrl=BTM_AllocWorld(7, 7);
				csg_wrl=BTM_PgmAllocContext();
			}
			tn=bccx_strdup(a[1]);
			BTM_PgmCsgInit();
			vala=BTM_PgmRun(csg_wrl, a[2], a[3]);
			mcur=BTM_ProcCsgToMesh(vala, mlst);

			mcur->name=tn;
			mcur->skel=skel;
			
			mc1=mcur->next; k=1;
			while(mc1 && mc1!=mlst)
			{
				sprintf(tb, "%s.%d", tn, k);
				mc1->name=bccx_strdup(tb);
				mc1->skel=skel;
				mc1=mc1->next; k++;
			}
			mlst=mcur;
			continue;
#endif
		}

		if(!strcmp(a[0], "texture"))
		{
			mcur=BTM_LookupMeshList(mlst, a[1]);
			if(mcur)
			{
				ix=mcur->n_texvec++;
				mcur->usetex[ix]=bccx_strdup(a[2]);
				
				if(	a[3] && !strcmp(a[ 3], "(") &&
					a[9] && !strcmp(a[ 9], "[") &&
					a[15] && !strcmp(a[15], "("))
				{
					//a[3]="("
					mcur->texvec_n[ix*4+0]=btm_atod(a[4]);
					mcur->texvec_n[ix*4+1]=btm_atod(a[5]);
					mcur->texvec_n[ix*4+2]=btm_atod(a[6]);
					mcur->texvec_n[ix*4+3]=btm_atod(a[7]);
					//a[8]=")"

					//a[9]="["
					mcur->texvec_s[ix*4+0]=btm_atod(a[10]);
					mcur->texvec_s[ix*4+1]=btm_atod(a[11]);
					mcur->texvec_s[ix*4+2]=btm_atod(a[12]);
					mcur->texvec_s[ix*4+3]=btm_atod(a[13]);
					//a[14]="]"

					//a[15]="["
					mcur->texvec_t[ix*4+0]=btm_atod(a[16]);
					mcur->texvec_t[ix*4+1]=btm_atod(a[17]);
					mcur->texvec_t[ix*4+2]=btm_atod(a[18]);
					mcur->texvec_t[ix*4+3]=btm_atod(a[19]);
					//a[20]="]"
				}else
					if(a[3] && !strcmp(a[3], "["))
				{
					//a[3]="["
					mcur->texvec_s[ix*4+0]=btm_atod(a[4]);
					mcur->texvec_s[ix*4+1]=btm_atod(a[5]);
					mcur->texvec_s[ix*4+2]=btm_atod(a[6]);
					mcur->texvec_s[ix*4+3]=btm_atod(a[7]);
					//a[8]="]"

					//a[9]="["
					mcur->texvec_t[ix*4+0]=btm_atod(a[10]);
					mcur->texvec_t[ix*4+1]=btm_atod(a[11]);
					mcur->texvec_t[ix*4+2]=btm_atod(a[12]);
					mcur->texvec_t[ix*4+3]=btm_atod(a[13]);
					//a[14]="]"
					
					btm_vec3fv_cross(
						mcur->texvec_s+ix*4,
						mcur->texvec_t+ix*4,
						mcur->texvec_n+ix*4);
					btm_vec3fv_norm(mcur->texvec_n+ix*4, mcur->texvec_n+ix*4);
					mcur->texvec_n[ix*4+3]=0;
				}else
				{
					if(a[3])
						x=btm_atod(a[3]);
					else
						x=1;

					mcur->texvec_s[ix*4+0]=x;
					mcur->texvec_s[ix*4+1]=0;
					mcur->texvec_s[ix*4+2]=0;
					mcur->texvec_s[ix*4+3]=0;

					mcur->texvec_t[ix*4+0]=0;
					mcur->texvec_t[ix*4+1]=0;
					mcur->texvec_t[ix*4+2]=x;
					mcur->texvec_t[ix*4+3]=0;
				
//					mcur->texscale_s=btm_atod(a[3]);
//					mcur->texscale_t=mcur->texscale_s;
				}
			}
		}

		if(!strcmp(a[0], "origin"))
		{
			mcur=BTM_LookupMeshList(mlst, a[1]);
			if(mcur)
			{
				mcur->baseorg[0]=btm_atod(a[2]);
				mcur->baseorg[1]=btm_atod(a[3]);
				mcur->baseorg[2]=btm_atod(a[4]);
			}
			continue;
		}

		if(!strcmp(a[0], "color"))
		{
			mcur=BTM_LookupMeshList(mlst, a[1]);
			if(mcur)
			{
				mcur->baseclr[0]=btm_atod(a[2]);
				mcur->baseclr[1]=btm_atod(a[3]);
				mcur->baseclr[2]=btm_atod(a[4]);
				mcur->baseclr[3]=btm_atod(a[5]);
			}
			continue;
		}

		if(!strcmp(a[0], "scale"))
		{
			mcur=BTM_LookupMeshList(mlst, a[1]);
			if(mcur)
			{
				if(!strcmp(a[2], "["))
				{
					mcur->scale[0]=btm_atod(a[3]);
					mcur->scale[1]=btm_atod(a[4]);
					mcur->scale[2]=btm_atod(a[5]);
				}else
				{
					w=btm_atod(a[2]);
					mcur->scale[0]=w;
					mcur->scale[1]=w;
					mcur->scale[2]=w;
				}
			}
			continue;
		}
	}

	return(mlst);
}

BTM_SolidMesh *BTM_LoadMeshListBuffer(byte *ibuf, int ibsz, u32 fcc)
{
	if(fcc==BGBCC_FMT_MDEF)
		{ return(BTM_LoadMeshListBufferMdef(ibuf, ibsz)); }

	if(fcc==BGBCC_FMT_STL)
		{ return(BTM_LoadMeshStlBuf(ibuf, ibsz, NULL)); }

	if(fcc==BGBCC_FMT_SCAD)
		{ return(BTM_LoadMeshListScadBuffer(ibuf, ibsz)); }

	return(NULL);
}

BTM_SolidMesh *BTM_LoadMeshListStl(char *fname)
{
	byte *ibuf;
	int ibsz;

	printf("BTM_LoadMeshListStl %s\n", fname);

	ibuf=BTM_LoadFile(fname, &ibsz);
	if(!ibuf)
	{
		printf("Can't open input list %s\n", fname);
		return(NULL);
	}

	return(BTM_LoadMeshListBufferMdef(ibuf, ibsz));
}
#endif
