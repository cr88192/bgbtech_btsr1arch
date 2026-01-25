BTM_CsgBrush *BTM_AllocBrush(int np)
{
	BTM_CsgBrush *brush;
	
	brush=btm_malloc(sizeof(BTM_CsgBrush)+(np+1)*4*sizeof(float));
	memset(brush, 0, sizeof(BTM_CsgBrush)+(np+1)*4*sizeof(float));
	brush->n_planes=np;
	brush->planes=(float *)(brush+1);
	return(brush);
}

BTM_CsgBrush *BTM_AllocBrushPlanes(float *planes, int np)
{
	BTM_CsgBrush *brush;

	brush=BTM_AllocBrush(np);
	memcpy(brush->planes, planes, np*4*sizeof(float));
	return(brush);
}

int btm_csg_checkfloatsane(float f)
{
	if(f==0)
		return(1);
	if(!(f==f))
		return(1);
//	if(fabs(f)<0.0001)
//		return(0);
	if(fabs(f)>10000.0)
		return(0);
	return(1);
}

int btm_csg_checkvec3sane(float *fv)
{
	if(	!btm_csg_checkfloatsane(fv[0]) ||
		!btm_csg_checkfloatsane(fv[1]) ||
		!btm_csg_checkfloatsane(fv[2]) )
	{
		return(0);
	}
	return(1);
}

int btm_csg_checkvec4sane(float *fv)
{
	if(	!btm_csg_checkfloatsane(fv[0]) ||
		!btm_csg_checkfloatsane(fv[1]) ||
		!btm_csg_checkfloatsane(fv[2]) ||
		!btm_csg_checkfloatsane(fv[3]) )
	{
		return(0);
	}
	return(1);
}

BTM_CsgBrush *BTM_AllocBrushAabb(float *mins, float *maxs)
{
	BTM_CsgBrush *brush;
	
	if(	!btm_csg_checkvec3sane(mins) ||
		!btm_csg_checkvec3sane(maxs) )
	{
		brush=NULL;
	}

	brush=BTM_AllocBrush(6);
	TKRA_Vec4F_Set(brush->planes+0*4, -1,  0,  0, -mins[0]);
	TKRA_Vec4F_Set(brush->planes+1*4,  1,  0,  0,  maxs[0]);
	TKRA_Vec4F_Set(brush->planes+2*4,  0, -1,  0, -mins[1]);
	TKRA_Vec4F_Set(brush->planes+3*4,  0,  1,  0,  maxs[1]);
	TKRA_Vec4F_Set(brush->planes+4*4,  0,  0, -1, -mins[2]);
	TKRA_Vec4F_Set(brush->planes+5*4,  0,  0,  1,  maxs[2]);
	return(brush);
}

BTM_CsgBrush *BTM_AllocBrushCylinder(
	float *org,
	float zhgt, float ratbot, float rattop, int fn)
{
	BTM_CsgBrush *brush;
	float nv0[4], nv1[4], pt0[3];
	float sinth1, costh1;
	float th, sinth2, costh2, ath, sinz, ra, rasc, d;
	int i;
	
	brush=BTM_AllocBrush(2+fn);
	TKRA_Vec4F_Set(brush->planes+0*4,  0,  0, -1, -org[2]+0);
	TKRA_Vec4F_Set(brush->planes+1*4,  0,  0,  1,  org[2]+zhgt);
	
	rasc=cos(M_PI/fn);
//	rasc*=0.995;
	
	for(i=0; i<fn; i++)
	{
//		ra=(ratbot+rattop)/2;
		ra=ratbot*rasc;
		th=(i+0.5)*((2*M_PI)/fn);
//		th=th+M_PI/4;
		
//		ath=atan2(ratbot-rattop, zhgt);
		ath=atan2(ratbot-rattop, zhgt*1.414);
//		ath=asin((ratbot-rattop)/zhgt);
		
		sinth1=sin(th);
		costh1=cos(th);
		sinth2=sinth1*cos(ath);
		costh2=costh1*cos(ath);
		sinz=sin(ath);

		nv0[0]=sinth1;
		nv0[1]=costh1;
		nv0[2]=0;
		
		TKRA_Vec3F_AddScale(org, nv0, ra, pt0);
//		TKRA_Vec3F_AddScale(org, nv0, ratbot, pt0);
//		d=(org[0]*sinth)+(org[1]*costh)+(org[2]*sinz);

		nv1[0]=sinth2;
		nv1[1]=costh2;
		nv1[2]=sinz;
		nv1[3]=TKRA_Vec3F_DotProduct(pt0, nv1);

//		TKRA_Vec4F_Set(brush->planes+(i+2)*4, sinth, costh, sinz, ra+d);
		TKRA_Vec4F_Set(brush->planes+(i+2)*4, nv1[0], nv1[1], nv1[2], nv1[3]);
	}
	
	return(brush);
}

BTM_CsgBrush *BTM_AllocBrushSphere(float rad, int fn)
{
	BTM_CsgBrush *brush;
	float th, sinth, costh, ath, sinz, ra;
	int i, j, fn2, fnn2;

	fn2=fn/2;
	if(fn2<3)
		fn2=3;
	fnn2=fn2-2;
	
	brush=BTM_AllocBrush(2+fn*fnn2);
	TKRA_Vec4F_Set(brush->planes+0*4,  0,  0, -1, rad);
	TKRA_Vec4F_Set(brush->planes+1*4,  0,  0,  1, rad);
	
	for(j=0; j<fnn2; j++)
		for(i=0; i<fn; i++)
	{
		th=i*((2*M_PI)/fn);
		ath=(j+1)*(M_PI/fn2)-(M_PI/2);
		sinth=sin(th)*cos(ath);
		costh=cos(th)*cos(ath);
		sinz=sin(ath);
		TKRA_Vec4F_Set(brush->planes+(i+2)*4, sinth, costh, sinz, rad);
	}
	
	return(brush);
}

BTM_CsgBrush *BTM_AllocBrushSphereScale(
	float *org, float *size, int fn)
{
	float nv0[4], nv1[4], pt0[4];
	BTM_CsgBrush *brush;
	float th, sinth, costh, ath, sinz, ra, d;
	int i, j, fn2, fnn2;

//	fn2=fn/2;
//	if(fn2<3)
//		fn2=3;
//	fnn2=fn2-2;

	fnn2=(fn-2)/2;
	fn2=fnn2+2;
	
	brush=BTM_AllocBrush(2+fn*fnn2);
	TKRA_Vec4F_Set(brush->planes+0*4,  0,  0, -1, -(org[2]-size[2]));
	TKRA_Vec4F_Set(brush->planes+1*4,  0,  0,  1, org[2]+size[2]);
	
	for(i=0; i<fn; i++)
		for(j=0; j<fnn2; j++)
	{
//		th=(i+0.5)*((2*M_PI)/fn);
		th=(i+0.0)*((2*M_PI)/fn);
//		ath=(j+1)*(M_PI/fn2)-(M_PI/2);
//		ath=(j+0.5)*(M_PI/fn2)-(M_PI/2);
//		ath=(j+1.5)*(M_PI/fn2)-(M_PI/2);
		ath=(j+1)*(M_PI/(fn2-1))-(M_PI/2);
//		ath=(j+1.5)*(M_PI/fnn2)-(M_PI/2);
		sinth=sin(th)*cos(ath);
		costh=cos(th)*cos(ath);
		sinz=sin(ath);

//		ra=(size[0]*fabs(sinth))+(size[1]*fabs(costh))+(size[2]*fabs(sinz));
		ra=size[0];

		nv0[0]=sinth;
		nv0[1]=costh;
		nv0[2]=sinz;
		TKRA_Vec3F_Normalize(nv0, nv0);
		
		d=fabs(nv0[0])+fabs(nv0[1])+fabs(nv0[2]);
		nv1[0]=fabs(nv0[0])/d;
		nv1[1]=fabs(nv0[1])/d;
		nv1[2]=fabs(nv0[2])/d;
		ra=(size[0]*nv1[0])+(size[1]*nv1[1])+(size[2]*nv1[2]);
		
		TKRA_Vec3F_AddScale(org, nv0, ra, pt0);
		nv0[3]=TKRA_Vec3F_DotProduct(pt0, nv0);

//		ra=(size[0]*sinth)+(size[1]*costh)+(size[2]*sinz);
//		d=(org[0]*sinth)+(org[1]*costh)+(org[2]*sinz);
		
		TKRA_Vec4F_Set(brush->planes+(2+i*fnn2+j)*4,
			nv0[0], nv0[1], nv0[2], nv0[3]);
	}
	
	return(brush);
}

BTM_CsgBrush *BTM_CloneBrush(BTM_CsgBrush *inbrush)
{
	BTM_CsgBrush *brush;
	int np;
	
	np=inbrush->n_planes;
	brush=btm_malloc(sizeof(BTM_CsgBrush)+np*4*sizeof(float));
	brush->n_planes=np;
	brush->planes=(float *)(brush+1);
	memcpy(brush->planes, inbrush->planes, np*4*sizeof(float));
	return(brush);
}

BTM_CsgBrush *BTM_CloneBrushesList(BTM_CsgBrush *inblst, BTM_CsgBrush *outblst)
{
	BTM_CsgBrush *bcur, *btmp, *olst;
	
	olst=outblst;
	bcur=inblst;
	while(bcur)
	{
		btmp=BTM_CloneBrush(bcur);
		btmp->next=olst;
		olst=btmp;
		bcur=bcur->next;
	}
	return(olst);
}

BTM_CsgNode *BTM_MakeCsgNodeFromBrushList(BTM_CsgBrush *blst)
{
	BTM_CsgNode *tmp;
	
	tmp=btm_malloc(sizeof(BTM_CsgNode));
	memset(tmp, 0, sizeof(BTM_CsgNode));
	TKRA_Mat4F_Identity(tmp->trans);
	tmp->brush=blst;
	tmp->opr=0;
	return(tmp);
}

BTM_CsgNode *BTM_MakeCsgNodeSubtract(BTM_CsgNode *ltcsg, BTM_CsgNode *rtcsg)
{
	BTM_CsgNode *tmp;
	
	tmp=btm_malloc(sizeof(BTM_CsgNode));
	memset(tmp, 0, sizeof(BTM_CsgNode));

	TKRA_Mat4F_Identity(tmp->trans);
	tmp->ltree=ltcsg;
	tmp->rtree=rtcsg;
	tmp->opr=1;
	
	return(tmp);
}

BTM_CsgNode *BTM_MakeCsgNodeUnion(BTM_CsgNode *ltcsg, BTM_CsgNode *rtcsg)
{
	BTM_CsgNode *tmp;
	
	tmp=btm_malloc(sizeof(BTM_CsgNode));
	memset(tmp, 0, sizeof(BTM_CsgNode));

	TKRA_Mat4F_Identity(tmp->trans);
	tmp->ltree=ltcsg;
	tmp->rtree=rtcsg;
	tmp->opr=0;

#if 0
	if((ltcsg->opr==0) && (rtcsg->opr==0) &&
		!ltcsg->ltree && !ltcsg->rtree &&
		!rtcsg->ltree && !rtcsg->rtree &&
		HullF_Mat4F_IdentityP(ltcsg->trans) &&
		HullF_Mat4F_IdentityP(rtcsg->trans) &&
		(ltcsg->colormat==rtcsg->colormat))
	{
		tmp->ltree=NULL;
		tmp->rtree=NULL;
		tmp->brush=BTM_CloneBrushesList(rtcsg->brush, NULL);
		tmp->brush=BTM_CloneBrushesList(ltcsg->brush, tmp->brush);
		tmp->color=ltcsg->color;
	}
#endif

	return(tmp);
}

BTM_CsgNode *BTM_MakeCsgNodeUnionMulti(BTM_CsgNode **prims, int nprim)
{
	BTM_CsgNode *tmp, *tmp1;
	
	if(nprim==2)
	{
		tmp=BTM_MakeCsgNodeUnion(prims[0], prims[1]);
		return(tmp);
	}

	if(nprim>2)
	{
		tmp1=BTM_MakeCsgNodeUnionMulti(prims+1, nprim-1);
		tmp=BTM_MakeCsgNodeUnion(prims[0], tmp1);
		return(tmp);
	}
	
	return(NULL);
}

BTM_CsgNode *BTM_MakeCsgNodeDifference(BTM_CsgNode *ltcsg, BTM_CsgNode *rtcsg)
{
	BTM_CsgNode *tmp;
	
	tmp=btm_malloc(sizeof(BTM_CsgNode));
	memset(tmp, 0, sizeof(BTM_CsgNode));

	TKRA_Mat4F_Identity(tmp->trans);
	tmp->ltree=ltcsg;
	tmp->rtree=rtcsg;
	tmp->opr=1;

	return(tmp);
}

BTM_CsgNode *BTM_MakeCsgNodeIntersection(BTM_CsgNode *ltcsg, BTM_CsgNode *rtcsg)
{
	BTM_CsgNode *tmp;
	
	tmp=btm_malloc(sizeof(BTM_CsgNode));
	memset(tmp, 0, sizeof(BTM_CsgNode));

	TKRA_Mat4F_Identity(tmp->trans);
	tmp->ltree=ltcsg;
	tmp->rtree=rtcsg;
	tmp->opr=2;

	return(tmp);
}

#define BTM_CSGTAG_FREE		0x123456785A5A5A5A

BTM_CsgPoly *btm_csgpoly_freelist[128];

BTM_CsgPoly *BTM_MakeCsgPolyForPoints(
	float *pts, int npts,
	u64 clrmat, BTM_CsgPoly *lst)
{
	BTM_CsgPoly *tmp;
	int i;

	if(!npts)
		return(lst);
	
	if((npts<=0) || (npts>=128))
	{
		__debugbreak();
	}
	
	if(clrmat==BTM_CSGTAG_FREE)
		{ __debugbreak(); }
	
	for(i=0; i<npts; i++)
	{
		if(!btm_csg_checkvec3sane(pts+i*3))
			{ __debugbreak(); }
	}
	
	tmp=btm_csgpoly_freelist[npts];
	if(tmp)
	{
		btm_csgpoly_freelist[npts]=tmp->next;
		if(tmp->pts!=((float *)(tmp+1)))
			{ __debugbreak(); }
		if(tmp->clrmat!=BTM_CSGTAG_FREE)
			{ __debugbreak(); }
	}else
	{	
		tmp=btm_malloc(sizeof(BTM_CsgPoly)+(npts+1)*3*sizeof(float));
		memset(tmp, 0, sizeof(BTM_CsgPoly));
		tmp->pts=(float *)(tmp+1);
	}

	memcpy(tmp->pts, pts, npts*3*sizeof(float));
	tmp->npts=npts;

	tmp->clrmat=clrmat;
	HullF_CalcPolyBBox(pts, npts, tmp->bbox+0, tmp->bbox+3);

	tmp->next=lst;
	return(tmp);
}

void BTM_FreeCsgPolyList(BTM_CsgPoly *lst)
{
	BTM_CsgPoly *pcur, *pnxt;

	if(!lst)
		return;

	if(lst->clrmat==BTM_CSGTAG_FREE)
	{
		printf("BTM_FreeCsgPolyList: Try free already-freed poly list\n");
		return;
	}
	
	pcur=lst;
	while(pcur)
	{
		pnxt=pcur->next;

		if(pcur->clrmat==BTM_CSGTAG_FREE)
		{
			printf("BTM_FreeCsgPolyList: Try free already-freed poly\n");
			break;
//			{ __debugbreak(); }
		}
		if((pcur->npts<=0) || (pcur->npts>=128))
			{ __debugbreak(); }

//		btm_free(pcur);
		pcur->clrmat=BTM_CSGTAG_FREE;
		pcur->next=btm_csgpoly_freelist[pcur->npts];
		btm_csgpoly_freelist[pcur->npts]=pcur;

		pcur=pnxt;
	}
}

void BTM_CheckSaneCsgPolyList(BTM_CsgPoly *lst)
{
	BTM_CsgPoly *pcur, *pnxt;

	if(!lst)
		return;

	if(lst->clrmat==BTM_CSGTAG_FREE)
		{ __debugbreak(); }
	
	pcur=lst;
	while(pcur)
	{
		pnxt=pcur->next;
		if(pcur->clrmat==BTM_CSGTAG_FREE)
			{ __debugbreak(); }
		if((pcur->npts<=0) || (pcur->npts>=128))
			{ __debugbreak(); }
		pcur=pnxt;
	}
}

BTM_CsgPoly *BTM_CloneCsgPolyList(BTM_CsgPoly *lst)
{
	BTM_CsgPoly *pcur, *olst;
	
	olst=NULL;
	pcur=lst;
	while(pcur)
	{
		if(pcur->clrmat==BTM_CSGTAG_FREE)
			{ __debugbreak(); }
		olst=BTM_MakeCsgPolyForPoints(pcur->pts, pcur->npts, pcur->clrmat, olst);
		pcur=pcur->next;
	}
	BTM_CheckSaneCsgPolyList(olst);
	return(olst);
}

BTM_CsgPoly *BTM_MergeCsgPolyList(BTM_CsgPoly *plst1, BTM_CsgPoly *plst2)
{
	BTM_CsgPoly *pcur, *pnxt, *olst;
	
	olst=plst2;
	pcur=plst1;
	while(pcur)
	{
		if(pcur->clrmat==BTM_CSGTAG_FREE)
			{ __debugbreak(); }
		pnxt=pcur->next;
		pcur->next=olst;
		olst=pcur;
		pcur=pnxt;
	}
	BTM_CheckSaneCsgPolyList(olst);
	return(olst);
}

BTM_CsgPoly *BTM_GetCsgPolysForBrush(BTM_CsgBrush *bru,
	BTM_CsgPoly *orglst, float *trans, u64 iclrmat, int flag)
{
	static float pts0[16*3];
	static float pts1[16*3];
	BTM_CsgPoly *tmp, *lst;

	float tv0[3], tv1[3], tv2[3], tnv[4], tnvi[4], nvi[4], nvj[4];
	float *norm;
	float sx, sy, f;
	u64 clrmat;
	int i, j, k, l, t, num;

	norm=bru->planes;
	num=bru->n_planes;
	lst=orglst;

	clrmat=iclrmat;
	if(flag&1)
	{
		if(!clrmat)
			clrmat=0xFFFFFF;
		clrmat|=1<<25;
	}

	for(i=0; i<num; i++)
	{
		TKRA_Vec4F_Copy(norm+i*4, nvi);
		HullF_TransformPlane(nvi, tnv, trans);
		if(flag&1)
			{ TKRA_Vec4F_Scale(tnv, -1, tnv); }

		TKRA_Vec4F_Copy(tnv, tnvi);

		if(!btm_csg_checkvec3sane(tnv))
		{
			k=-1;
		}
		
//		HullF_MakePlaneFace(norm+i*4, pts0);
		HullF_MakePlaneFace(tnv, pts0);
		l=4;

		for(j=0; j<num; j++)
		{
			if(i==j)continue;

			TKRA_Vec4F_Copy(norm+j*4, nvj);
			HullF_TransformPlane(nvj, tnv, trans);

			if(!btm_csg_checkvec4sane(tnv))
			{
				k=-1;
			}

//			l=HullF_ClipFace(norm+j*4, pts0, pts1, l);
			l=HullF_ClipFace(tnv, pts0, pts1, l);
			
			if(l<=0)
			{
				k=-1;
				break;
			}
			
			for(k=0; k<(l*3); k++)pts0[k]=pts1[k];
		}
		if(l<=0)
			continue;

		lst=BTM_MakeCsgPolyForPoints(pts0, l, clrmat, lst);

//		for(j=0; j<l; j++)
//		{
//			TKRA_Vec3F_Min(mins, pts0+j*3, mins);
//			TKRA_Vec3F_Max(maxs, pts0+j*3, maxs);
//		}
	}

	BTM_CheckSaneCsgPolyList(lst);
	return(lst);
}

BTM_CsgPoly *BTM_ClipCsgPolysByBrush(BTM_CsgBrush *bru,
	BTM_CsgPoly *orglst, float *trans, int flag)
{
	static float pts0[192*3];
	static float pts1[192*3];
	BTM_CsgPoly *tmp, *plst, *plst2, *olst, *pcur, *pnxt;

	float tv0[3], tv1[3], tv2[3], tnv[4], tnnv[4];
	float *norm;
	float sx, sy, f;
	u64 pcmat;
	int i, j, k, l, t, num;

	norm=bru->planes;
	num=bru->n_planes;

	olst=NULL;
	pcur=orglst;
	plst=NULL;
	while(pcur)
	{
		if(pcur->clrmat==BTM_CSGTAG_FREE)
			{ __debugbreak(); }
		memcpy(pts0, pcur->pts, pcur->npts*3*sizeof(float));
		l=pcur->npts;
		pcmat=pcur->clrmat;
		pcur=pcur->next;

		for(i=0; i<num; i++)
		{
			HullF_TransformPlane(norm+i*4, tnv, trans);
			HullF_PolyPlaneExtents(pts0, l, tnv, &sx, &sy);
//			if(sx>=0)
			if(sx>=(-0.999))
				break;
		}
		
		if(i<num)
		{
			/* entirely outside of brush. */
//			if(flag&1)
//				continue;
			olst=BTM_MakeCsgPolyForPoints(pts0, l, pcmat, olst);
		}else
		{
			/* crosses into brush. */
			plst=BTM_MakeCsgPolyForPoints(pts0, l, pcmat, plst);
		}
	}

	if(olst && !plst)
	{
		BTM_CheckSaneCsgPolyList(olst);
		return(olst);
	}

	for(i=0; i<num; i++)
	{
		HullF_TransformPlane(norm+i*4, tnv, trans);
		TKRA_Vec4F_Scale(tnv, -1, tnnv);

		pcur=plst;
		plst2=NULL;
		while(pcur)
		{
			if(pcur->clrmat==BTM_CSGTAG_FREE)
				{ __debugbreak(); }
			memcpy(pts0, pcur->pts, pcur->npts*3*sizeof(float));
			l=pcur->npts;
			pcmat=pcur->clrmat;
			pcur=pcur->next;

			HullF_PolyPlaneExtents(pts0, l, tnv, &sx, &sy);
			if(sx>=0)
			{
				if(flag&1)
					continue;
				olst=BTM_MakeCsgPolyForPoints(pts0, l, pcmat, olst);
				continue;
			}

			if(sy<=0)
			{
				plst2=BTM_MakeCsgPolyForPoints(pts0, l, pcmat, plst2);
				continue;
			}

			t=HullF_ClipFace(tnv, pts0, pts1, l);
			if(t>=3)
				plst2=BTM_MakeCsgPolyForPoints(pts1, t, pcmat, plst2);

			if(flag&1)
				continue;
			t=HullF_ClipFace(tnnv, pts0, pts1, l);
			if(t>=3)
				olst=BTM_MakeCsgPolyForPoints(pts1, t, pcmat, olst);
		}
		
		BTM_FreeCsgPolyList(plst);
		plst=plst2;
		BTM_CheckSaneCsgPolyList(plst);
	}
	
	if(flag&1)
	{
		pcur=plst;
		while(pcur)
		{
			pnxt=pcur->next;
			pcur->clrmat|=(1<<24);
			pcur->next=olst;
			olst=pcur;
			pcur=pnxt;
		}
	
//		BTM_FreeCsgPolyList(olst);
		BTM_CheckSaneCsgPolyList(olst);
		return(olst);
	}

	BTM_FreeCsgPolyList(plst);
	BTM_CheckSaneCsgPolyList(olst);
	return(olst);
}

BTM_CsgPoly *BTM_ClipCsgPolysOutsideByBrushList(
	BTM_CsgBrush *brulst,
	BTM_CsgPoly *plylst,
	float *trans)
{
	BTM_CsgBrush *bcur;
	BTM_CsgPoly *plst, *plst2;
	
	plst=BTM_CloneCsgPolyList(plylst);
	bcur=brulst;

	while(bcur)
	{
		plst2=BTM_ClipCsgPolysByBrush(bcur, plst, trans, 0);
		bcur=bcur->next;
		BTM_FreeCsgPolyList(plst);
		plst=plst2;
	}
	
	BTM_CheckSaneCsgPolyList(plst);
	return(plst);
}

BTM_CsgPoly *BTM_ClipCsgPolysInsideByBrushList(
	BTM_CsgBrush *brulst,
	BTM_CsgPoly *plylst,
	float *trans)
{
	BTM_CsgBrush *bcur;
	BTM_CsgPoly *plst, *plst2;
	
	plst=BTM_CloneCsgPolyList(plylst);
	bcur=brulst;

	while(bcur)
	{
		plst2=BTM_ClipCsgPolysByBrush(bcur, plst, trans, 1);
		bcur=bcur->next;
		BTM_FreeCsgPolyList(plst);
		plst=plst2;
	}
	
	BTM_CheckSaneCsgPolyList(plst);
	return(plst);
}

/* Cull negative exterior polygons. */
BTM_CsgPoly *BTM_ClipCsgPolysCullNegExteriorN(BTM_CsgPoly *plylst)
{
	BTM_CsgPoly *pcur, *pnxt, *olst, *plst;
	
	pcur=plylst; olst=NULL; plst=NULL;
	while(pcur)
	{
		if(pcur->clrmat==BTM_CSGTAG_FREE)
			{ __debugbreak(); }
		pnxt=pcur->next;
		if(pcur->clrmat&(1<<25))
		{
			/* negative poly */
			if(pcur->clrmat&(1<<24))
			{
				/* interior */
				pcur->next=olst;
				olst=pcur;
			}else
			{
				/* exterior */
				pcur->next=plst;
				plst=pcur;
			}
		}else
		{
			/* exterior */
			pcur->next=olst;
			olst=pcur;
		}
		pcur=pnxt;
	}
	BTM_FreeCsgPolyList(plst);
	BTM_CheckSaneCsgPolyList(olst);
	return(olst);
}

/* Cull all exterior polys, keeping only interior polys. */
BTM_CsgPoly *BTM_ClipCsgPolysCullExteriorN(BTM_CsgPoly *plylst)
{
	BTM_CsgPoly *pcur, *pnxt, *olst, *plst;
	
	pcur=plylst; olst=NULL; plst=NULL;
	while(pcur)
	{
		if(pcur->clrmat==BTM_CSGTAG_FREE)
			{ __debugbreak(); }
		pnxt=pcur->next;
		if(pcur->clrmat&(1<<24))
		{
			/* interior */
			pcur->next=olst;
			olst=pcur;
		}else
		{
			/* exterior */
			pcur->next=plst;
			plst=pcur;
		}
		pcur=pnxt;
	}
	BTM_FreeCsgPolyList(plst);
	BTM_CheckSaneCsgPolyList(olst);
	return(olst);
}

BTM_CsgPoly *BTM_ClipCsgPolysForCsgNode(BTM_CsgNode *tree,
	BTM_CsgPoly *orglst, float *trans, int flag)
{
	float tmat[16];
	BTM_CsgPoly *plst, *plst2, *plstb, *pcur, *pnxt;
	BTM_CsgBrush *bcur;
	
//	TKRA_Mat4F_MatMult(trans, tree->trans, tmat);
	TKRA_Mat4F_MatMultT(trans, tree->trans, tmat);
	
	if(tree->opr==0)
	{
		if(flag&1)
		{
			plst=BTM_ClipCsgPolysInsideByBrushList(
				tree->brush, orglst, tmat);
			BTM_CheckSaneCsgPolyList(plst);
		}else
		{
			plst=BTM_ClipCsgPolysOutsideByBrushList(
				tree->brush, orglst, tmat);
			BTM_CheckSaneCsgPolyList(plst);
		}

		if(tree->ltree)
		{
			plst2=BTM_ClipCsgPolysForCsgNode(tree->ltree, plst, tmat, flag);
			BTM_CheckSaneCsgPolyList(plst2);
			BTM_FreeCsgPolyList(plst);
			plst=plst2;
			BTM_CheckSaneCsgPolyList(plst);
		}

		if(tree->rtree)
		{
			plst2=BTM_ClipCsgPolysForCsgNode(tree->rtree, plst, tmat, flag);
			BTM_CheckSaneCsgPolyList(plst2);
			BTM_FreeCsgPolyList(plst);
			plst=plst2;
			BTM_CheckSaneCsgPolyList(plst);
		}
		
		BTM_CheckSaneCsgPolyList(plst);
		return(plst);
	}
	
	if(tree->opr==1)
	{
		plst=orglst;

		if(tree->ltree)
		{
			plst2=BTM_ClipCsgPolysForCsgNode(tree->ltree, plst, tmat, flag);
//			BTM_FreeCsgPolyList(plst);
			plst=plst2;
			BTM_CheckSaneCsgPolyList(plst);
		}

		if(tree->rtree)
		{
			plst2=BTM_ClipCsgPolysForCsgNode(tree->rtree, plst, tmat, flag^1);
			BTM_FreeCsgPolyList(plst);
			plst=plst2;
//			plst=BTM_ClipCsgPolysCullNegExteriorN(plst2);
		}
		
		BTM_CheckSaneCsgPolyList(plst);
		return(plst);
	}
	
	return(NULL);
}

BTM_CsgPoly *BTM_GetCsgPolysForCsgNode(BTM_CsgNode *tree,
	BTM_CsgPoly *orglst, float *trans, u64 iclrmat, int flag)
{
	float tmat[16];
	BTM_CsgPoly *plst, *plst2, *plsta, *plsta2, *plstb, *plstb2, *pcur, *pnxt;
	BTM_CsgBrush *bcur;
	u64 clrmat;
	
//	TKRA_Mat4F_MatMult(trans, tree->trans, tmat);
	TKRA_Mat4F_MatMultT(trans, tree->trans, tmat);
	clrmat=iclrmat;
	if((tree->clrmat) && (!(flag&1) || !clrmat))
	{
//		clrmat=tree->clrmat;
		if(tree->clrmat&0xFFFFFF)
			clrmat=
				(clrmat&(~0xFFFFFF)) |
				(tree->clrmat&0xFFFFFF) ;
		if(tree->clrmat&0x000000FF00000000ULL)
			clrmat=
				(clrmat&(~0x000000FF00000000ULL)) |
				(tree->clrmat&0x000000FF00000000ULL) ;
	}
	
	if(!HullF_Mat4F_IdentityP(tmat))
	{
		pnxt=NULL;
	}
	
	if(tree->opr==0)
	{
		if(!tree->ltree && !tree->rtree &&
			tree->brush && !tree->brush->next)
		{
			/* union simply holding a brush. */
			plst=BTM_GetCsgPolysForBrush(
				tree->brush, orglst, tmat, clrmat, flag);
			BTM_CheckSaneCsgPolyList(plst);
			return(plst);
		}
	
		plst=NULL;
		if(tree->ltree)
			plst=BTM_GetCsgPolysForCsgNode(
				tree->ltree, plst, tmat, clrmat, flag);
		if(tree->rtree)
			plst=BTM_GetCsgPolysForCsgNode(
				tree->rtree, plst, tmat, clrmat, flag);
		bcur=tree->brush;
		while(bcur)
		{
			plst=BTM_GetCsgPolysForBrush(bcur, plst, tmat, clrmat, flag);
			bcur=bcur->next;
		}
		
//		plst2=BTM_ClipCsgPolysOutsideByBrushList(tree->brush, plst, tmat);
		plst2=BTM_ClipCsgPolysForCsgNode(tree, plst, trans, flag);
		BTM_FreeCsgPolyList(plst);

		plst=BTM_MergeCsgPolyList(plst2, orglst);
		BTM_CheckSaneCsgPolyList(plst);
		return(plst);
	}

	if(tree->opr==1)
	{
		plst=BTM_GetCsgPolysForCsgNode(
			tree->ltree, NULL, tmat, clrmat, flag);
		plstb=BTM_GetCsgPolysForCsgNode(
			tree->rtree, NULL, tmat, clrmat, flag^1);
		
//		plst2=BTM_ClipCsgPolysOutsideByBrushList(tree->brush, plst, tmat);
//		BTM_FreeCsgPolyList(plst);

		plst2=BTM_ClipCsgPolysForCsgNode(
			tree->rtree, plst, tmat, flag);
		plstb2=BTM_ClipCsgPolysForCsgNode(
			tree->ltree, plstb, tmat, flag^1);
		BTM_FreeCsgPolyList(plst);
		BTM_FreeCsgPolyList(plstb);

		BTM_CheckSaneCsgPolyList(plst2);
		BTM_CheckSaneCsgPolyList(plstb2);

		plstb2=BTM_ClipCsgPolysCullNegExteriorN(plstb2);

		plst=BTM_MergeCsgPolyList(plst2, orglst);
		plst=BTM_MergeCsgPolyList(plstb2, plst);
		BTM_CheckSaneCsgPolyList(plst);
		return(plst);
	}

	if(tree->opr==2)
	{
		plsta=BTM_GetCsgPolysForCsgNode(
			tree->ltree, NULL, tmat, clrmat, flag);
		plstb=BTM_GetCsgPolysForCsgNode(
			tree->rtree, NULL, tmat, clrmat, flag);
		
		plsta2=BTM_ClipCsgPolysForCsgNode(
			tree->rtree, plsta, tmat, flag^1);
		plstb2=BTM_ClipCsgPolysForCsgNode(
			tree->ltree, plstb, tmat, flag^1);
		BTM_FreeCsgPolyList(plsta);
		BTM_FreeCsgPolyList(plstb);

		plsta2=BTM_ClipCsgPolysCullExteriorN(plsta2);
		plstb2=BTM_ClipCsgPolysCullExteriorN(plstb2);

		plst=BTM_MergeCsgPolyList(plsta2, orglst);
		plst=BTM_MergeCsgPolyList(plstb2, plst);
		BTM_CheckSaneCsgPolyList(plst);
		return(plst);
	}

	return(NULL);
}

