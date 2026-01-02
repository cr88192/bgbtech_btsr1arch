int BTM_ObjGetVertex(float **rvtxbuf, int *rpos, float *vec)
{
	float *vtxbuf;
	int pos, maxpos, minpos;
	int i, j, k;
	
	vtxbuf=*rvtxbuf;
	pos=rpos[0];
	maxpos=rpos[1];
	minpos=rpos[2];
	
	for(i=minpos; i<pos; i++)
	{
		j=i;
		if(	(vtxbuf[j*4+0]==vec[0]) &&
			(vtxbuf[j*4+1]==vec[1]) &&
			(vtxbuf[j*4+2]==vec[2]))
				return(i+1);
	}

	if((pos+1)>=maxpos)
	{
		maxpos=maxpos+(maxpos>>1);
		vtxbuf=realloc(vtxbuf, maxpos*4*sizeof(float));
		*rvtxbuf=vtxbuf;
		rpos[1]=maxpos;
	}
	
	j=pos++;
	vtxbuf[j*4+0]=vec[0];
	vtxbuf[j*4+1]=vec[1];
	vtxbuf[j*4+2]=vec[2];
	vtxbuf[j*4+3]=0;
	rpos[0]=pos;
	return(0);
}

int BTM_ObjDumpTrisBuf(
	char **robuf, int *robsz,
	float **vtxbuf, int *vtxpos,
	char *sldname, float *tris, int ntris, u32 clr)
{
	char buf[256];
//	float vtxbuf[256*4];
//	btm_vec3f v0, v1, v2, v3, v4, v5, v6;
	char *mtlname;
	int i0, i1, i2;
	float x, y, z, vcr, vcg, vcb;
	float xo, yo, zo;
//	int vtxpos;
	int i, j, k;

	xo=0; yo=0; zo=0;

	if(!sldname)
		sldname="default";

//	for(i=0; i<256*4; i++)
//		vtxbuf[i]=-999999999;
//	vtxpos=0;
	
	if(clr)
	{
		vcr=((clr>>16)&255)/255.0;
		vcg=((clr>> 8)&255)/255.0;
		vcb=((clr>> 0)&255)/255.0;
	}else
	{
		vcr=1.0;
		vcg=1.0;
		vcb=1.0;
	}

	
	if(clr>=0x040000)
	{
		k=	(((clr>>20)&15)<<8) |
			(((clr>>12)&15)<<4) |
			(((clr>> 4)&15)<<0) ;
		sprintf(buf, "U%03X", k);
		mtlname=bgbcc_strdup(buf);
	}else if(clr)
	{
		mtlname=bgbcc_strtab_i(clr);
	}else
	{
		mtlname="UFFF";
	}
//	BTM_MeshBufPrintf(robuf, "usemtl U%03X\n", k);

	BTM_MeshBufPrintf(robuf, "o %s\n", sldname);

//	BTM_MeshBufPrintf(robuf, "usemtl U%03X\n", k);
	BTM_MeshBufPrintf(robuf, "usemtl %s\n", mtlname);
	
	for(i=0; i<ntris; i++)
	{
//		v0=btm_mkvec3f(tris[i*9+0], tris[i*9+1], tris[i*9+2]);
//		v1=btm_mkvec3f(tris[i*9+3], tris[i*9+4], tris[i*9+5]);
//		v2=btm_mkvec3f(tris[i*9+6], tris[i*9+7], tris[i*9+8]);

		j=BTM_ObjGetVertex(vtxbuf, vtxpos, tris+i*9+0);
		if(!j)
		{
			BTM_MeshBufPrintf(robuf,
				"v %f %f %f %.2f %.2f %.2f\n",
				tris[i*9+0]-xo, tris[i*9+2]-yo, -tris[i*9+1]-zo,
				vcr, vcg, vcb);

//			BTM_MeshBufPrintf(robuf,
//				"v %f %f %f\n",
//				tris[i*9+0]-xo, tris[i*9+1]-yo, tris[i*9+2]-zo);
		}
		j=BTM_ObjGetVertex(vtxbuf, vtxpos, tris+i*9+3);
		if(!j)
		{
			BTM_MeshBufPrintf(robuf,
				"v %f %f %f %.2f %.2f %.2f\n",
				tris[i*9+3]-xo, tris[i*9+5]-yo, -tris[i*9+4]-zo,
				vcr, vcg, vcb);
//			BTM_MeshBufPrintf(robuf,
//				"v %f %f %f\n",
//				tris[i*9+3]-xo, tris[i*9+4]-yo, tris[i*9+5]-zo);
		}
		j=BTM_ObjGetVertex(vtxbuf, vtxpos, tris+i*9+6);
		if(!j)
		{
			BTM_MeshBufPrintf(robuf,
				"v %f %f %f %.2f %.2f %.2f\n",
				tris[i*9+6]-xo, tris[i*9+8]-yo, -tris[i*9+7]-zo,
				vcr, vcg, vcb);
//			BTM_MeshBufPrintf(robuf,
//				"v %f %f %f\n",
//				tris[i*9+6]-xo, tris[i*9+7]-yo, tris[i*9+8]-zo);
		}

		i0=BTM_ObjGetVertex(vtxbuf, vtxpos, tris+i*9+0);
		i1=BTM_ObjGetVertex(vtxbuf, vtxpos, tris+i*9+3);
		i2=BTM_ObjGetVertex(vtxbuf, vtxpos, tris+i*9+6);

		BTM_MeshBufPrintf(robuf, "f %d %d %d\n", i0, i1, i2);
	}

//	BTM_MeshBufPrintf(robuf,
//		"endsolid %s\n", sldname);
	
	return(0);
}

int BTM_ExportMeshListObjBuf(BTM_SolidMesh *mesh,
	byte **robuf, int *robsz)
{
	float *vtxbuf;
	int vtxpos[4];
	char *oct[4];
	BTM_SolidMesh *mcur;

	oct[0]=*robuf;
	oct[1]=*robuf+(*robsz);
	oct[2]=*robuf;
	oct[3]=NULL;

	vtxbuf=malloc(4096*4*sizeof(float));
	vtxpos[0]=0;
	vtxpos[1]=4096;
	vtxpos[2]=0;
	vtxpos[3]=0;

	BTM_MeshBufPrintf(oct, "mtllib colors.mtl\n");
	
	mcur=mesh;
	while(mcur)
	{
		BTM_ObjDumpTrisBuf(oct, robsz,
			&vtxbuf, vtxpos,
			mcur->name,
			mcur->tris, mcur->nTris, mcur->clrmat&0xFFFFFF);
		mcur=mcur->next;
	}
	
	*robuf=oct[0];
	*robsz=oct[2]-oct[0];
	return(0);
}
