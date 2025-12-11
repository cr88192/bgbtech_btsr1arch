int btm_vartag_solidmesh;
int btm_vartag_csgnode;

int btm_pgmtok_csgaabb;
int btm_pgmtok_csgcube;
int btm_pgmtok_csgsphere;
int btm_pgmtok_csgcylinder;
int btm_pgmtok_csgunion;
int btm_pgmtok_csgdifference;
int btm_pgmtok_csgintersection;
int btm_pgmtok_csgdiff;
int btm_pgmtok_csgisec;

int btm_pgmtok_mins;
int btm_pgmtok_maxs;
int btm_pgmtok_size;
int btm_pgmtok_org;
int btm_pgmtok_rot;
int btm_pgmtok_angles;
int btm_pgmtok_color;
int btm_pgmtok_bone;

int btm_pgmtok_h;
int btm_pgmtok_r;
int btm_pgmtok_r1;
int btm_pgmtok_r2;
int btm_pgmtok_hgt;
int btm_pgmtok_rad;
int btm_pgmtok_fn;

u64 BTM_PgmCmd_CsgAabb(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs);
u64 BTM_PgmCmd_CsgCube(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs);
u64 BTM_PgmCmd_CsgSphere(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs);
u64 BTM_PgmCmd_CsgCylinder(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs);
u64 BTM_PgmCmd_CsgUnion(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs);

int BTM_PgmCsgInit()
{
	if(btm_vartag_solidmesh>0)
		return(0);
	btm_vartag_solidmesh=BCCX_StringToStridx("solidmesh");
	btm_vartag_csgnode=BCCX_StringToStridx("csgnode");

	btm_pgmtok_csgaabb			=BTM_PgmInternToken("Icsgaabb");
	btm_pgmtok_csgcube			=BTM_PgmInternToken("Icsgcube");
	btm_pgmtok_csgsphere		=BTM_PgmInternToken("Icsgsphere");
	btm_pgmtok_csgcylinder		=BTM_PgmInternToken("Icsgcylinder");
	btm_pgmtok_csgunion			=BTM_PgmInternToken("Icsgunion");
	btm_pgmtok_csgdifference	=BTM_PgmInternToken("Icsgdifference");
	btm_pgmtok_csgintersection	=BTM_PgmInternToken("Icsgintersection");
	btm_pgmtok_csgdiff			=BTM_PgmInternToken("Icsgdiff");
	btm_pgmtok_csgisec			=BTM_PgmInternToken("Icsgisec");

	btm_pgmtok_mins			=BTM_PgmInternToken("Imins");
	btm_pgmtok_maxs			=BTM_PgmInternToken("Imaxs");
	btm_pgmtok_size			=BTM_PgmInternToken("Isize");
	btm_pgmtok_org			=BTM_PgmInternToken("Iorg");
	btm_pgmtok_rad			=BTM_PgmInternToken("Irad");
	btm_pgmtok_hgt			=BTM_PgmInternToken("Ihgt");
	btm_pgmtok_rot			=BTM_PgmInternToken("Irot");
	btm_pgmtok_angles		=BTM_PgmInternToken("Iangles");
	btm_pgmtok_color		=BTM_PgmInternToken("Icolor");
	btm_pgmtok_bone			=BTM_PgmInternToken("Ibone");

	btm_pgmtok_h			=BTM_PgmInternToken("Ih");
	btm_pgmtok_r			=BTM_PgmInternToken("Ir");
	btm_pgmtok_r1			=BTM_PgmInternToken("Ir1");
	btm_pgmtok_r2			=BTM_PgmInternToken("Ir2");
	btm_pgmtok_fn			=BTM_PgmInternToken("Ifn");

	BTM_PgmAddExt(btm_pgmtok_csgaabb, 1, BTM_PgmCmd_CsgAabb);
	BTM_PgmAddExt(btm_pgmtok_csgcube, 1, BTM_PgmCmd_CsgAabb);
	BTM_PgmAddExt(btm_pgmtok_csgsphere, 1, BTM_PgmCmd_CsgSphere);
	BTM_PgmAddExt(btm_pgmtok_csgcylinder, 1, BTM_PgmCmd_CsgCylinder);

	BTM_PgmAddExt(btm_pgmtok_csgunion, 1, BTM_PgmCmd_CsgUnion);
	BTM_PgmAddExt(btm_pgmtok_csgdifference, 1, BTM_PgmCmd_CsgUnion);
	BTM_PgmAddExt(btm_pgmtok_csgintersection, 1, BTM_PgmCmd_CsgUnion);
	BTM_PgmAddExt(btm_pgmtok_csgdiff, 1, BTM_PgmCmd_CsgUnion);
	BTM_PgmAddExt(btm_pgmtok_csgisec, 1, BTM_PgmCmd_CsgUnion);

	return(1);
}

struct {
char *name;
u32 rgb;
}btm_csg_colornames[]={

{"black",		0x101010},
{"blue",		0x0000AA},
{"green",		0x00AA00},
{"cyan",		0x00AAAA},
{"red",			0xAA0000},
{"magenta",		0xAA00AA},
{"brown",		0xAA5500},
{"higray",		0xAAAAAA},
{"logray",		0x555555},
{"hiblue",		0x555555},
{"higreen",		0x55FF55},
{"hicyan",		0x55FFFF},
{"hired",		0xFF5555},
{"himagenta",	0xFF55FF},
{"hiyellow",	0xFFFF55},
{"white",		0xFFFFFF},

{"yellow",		0xAAAA00},

{"blue2",		0x0000FF},
{"lime",		0x00FF00},
{"aqua",		0x00FFFF},
{"red2",		0xFF0000},
{"fuchsia",		0xFF00FF},
{"yellow2",		0xFFFF00},

{"navy",		0x000080},
{"green2",		0x008000},
{"teal",		0x008080},
{"maroon",		0x800000},
{"purple",		0x800080},
{"olive",		0x808000},
{"gray",		0x808080},
{"silver",		0xC0C0C0},

{"pink",		0xFFC0CB},
{"violet",		0xEE82EE},

{NULL, 0}
};

u32 BTM_PgmCsg_NameToColor(u64 val)
{
	char *s0;
	int i, j;

	if(BTM_VarIsInt(val))
	{
		i=BTM_VarUnwrapInt(val)&0xFFFFFF;
		if(((i>>16)&255)<4)		i|=(4<<16);
		if(((i>> 8)&255)<4)		i|=(4<< 8);
		if(((i>> 0)&255)<4)		i|=(4<< 0);
		return(i);
	}
	
	if(BTM_VarIsString(val))
	{
		s0=BTM_VarUnwrapString(val);
		
		if(!s0 || !(*s0))
			return(0xFFFFFF);
		
		if(s0 && (*s0=='#'))
		{
			i=strtol(s0+1, NULL, 16)&0xFFFFFF;
			if(((i>>16)&255)<4)		i|=(4<<16);
			if(((i>> 8)&255)<4)		i|=(4<< 8);
			if(((i>> 0)&255)<4)		i|=(4<< 0);
			return(i);
		}
		
		for(i=0; btm_csg_colornames[i].name; i++)
		{
			if(!strcmp(btm_csg_colornames[i].name, s0))
			{
				j=btm_csg_colornames[i].rgb;
				if(((j>>16)&255)<4)		j|=(4<<16);
				if(((j>> 8)&255)<4)		j|=(4<< 8);
				if(((j>> 0)&255)<4)		j|=(4<< 0);
				return(j);
			}
		}
		
		i=bccx_strdup_i(s0);
		return(i);
	}
	
	return(0xFFFFFF);
}

u64 BTM_PgmCmd_CsgAabb(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs)
{
	u16 argn[32];
	u64 argv[32];
	float org[3], size[3];
	float mins[3], maxs[3];
	float ang[3], rot[4];
	float mat[16];
	u16 *tcs;
	BTM_CsgBrush *bru;
	BTM_CsgNode *csgn;
	u64 clrmat;
	u64 val;
	int i, j, k, narg;
	int has_orgsz, has_rot, has_aabb;

	tcs=*rtcs;
	tcs++;
	narg=BTM_PgmDoEvalNamedArgsList(wrl, pgm, &tcs, argn, argv);
	*rtcs=tcs;
	
	TKRA_Vec3F_Zero(mins);
	TKRA_Vec3F_Zero(maxs);
	TKRA_Vec3F_Zero(org);
	TKRA_Vec3F_Zero(size);
	TKRA_Vec3F_Zero(ang);
//	TKRA_Vec4F_Zero(rot);
	QuatF_Identity(rot);
	has_orgsz=0;
	has_rot=0;
	has_aabb=0;
	clrmat=0;
	
	for(i=0; i<narg; i++)
	{
		if(argn[i]==btm_pgmtok_mins)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], mins, 3);
			has_aabb=1;
		}

		if(argn[i]==btm_pgmtok_maxs)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], maxs, 3);
			has_aabb=1;
		}

		if(argn[i]==btm_pgmtok_org)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], org, 3);
			has_orgsz=1;
		}

		if(argn[i]==btm_pgmtok_rot)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], rot, 4);
			has_rot=1;
		}

		if(argn[i]==btm_pgmtok_angles)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], ang, 3);
			ang[0]=ang[0]*(M_PI/180.0);
			ang[1]=ang[1]*(M_PI/180.0);
			ang[2]=ang[2]*(M_PI/180.0);
			QuatF_FromAngles(ang, rot);
			has_rot=1;
		}

		if(argn[i]==btm_pgmtok_size)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], size, 3);
			has_orgsz=1;
		}

		if(argn[i]==btm_pgmtok_color)
		{
//			clrmat=BTM_VarUnwrapInt(argv[i])&0xFFFFFF;
			clrmat=(BTM_PgmCsg_NameToColor(argv[i])&0xFFFFFF)|
				(clrmat&(~0xFFFFFF));
		}

		if(argn[i]==btm_pgmtok_bone)
		{
			clrmat=
				clrmat&(~0x000000FF00000000ULL)|
				((BTM_VarUnwrapInt(argv[i])&0xFFULL)<<32);
		}
	}

	if(has_orgsz && !has_aabb)
	{
		if(has_rot)
		{
			TKRA_Vec3F_Zero(mins);
			TKRA_Vec3F_Copy(size, maxs);
//			TKRA_Vec3F_Scale(size, -0.5, mins);
//			TKRA_Vec3F_Scale(size,  0.5, maxs);
		}else
		{
			TKRA_Vec3F_Copy(org, mins);
			TKRA_Vec3F_Add(org, size, maxs);
//			TKRA_Vec3F_AddScale(org, size, -0.5, mins);
//			TKRA_Vec3F_AddScale(org, size,  0.5, maxs);
		}
	}

	if(has_rot)
	{
		QuatF_ToMatrix(rot, mat);
		mat[12]+=org[0];
		mat[13]+=org[1];
		mat[14]+=org[2];
	}else
	{
		TKRA_Mat4F_Identity(mat);
	}

	bru=BTM_AllocBrushAabb(mins, maxs);
	csgn=BTM_MakeCsgNodeFromBrushList(bru);
	csgn->clrmat=clrmat;
	TKRA_Mat4F_Copy(mat, csgn->trans);
	val=BTM_VarWrapPointer(csgn, btm_vartag_csgnode);
	return(val);
	
}

u64 BTM_PgmCmd_CsgSphere(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs)
{
	u16 argn[32];
	u64 argv[32];
	float org[3], size[3];
	float mins[3], maxs[3];
	u16 *tcs;
	BTM_CsgBrush *bru;
	BTM_CsgNode *csgn;
	u64 val;
	u64 clrmat;
	float rad;
	int was_orgsz, was_aabb, was_rad;
	int i, j, k, narg, fn;

	tcs=*rtcs;
	tcs++;
	narg=BTM_PgmDoEvalNamedArgsList(wrl, pgm, &tcs, argn, argv);
	*rtcs=tcs;
	
	TKRA_Vec3F_Zero(mins);
	TKRA_Vec3F_Zero(maxs);
	TKRA_Vec3F_Zero(org);
	TKRA_Vec3F_Zero(size);
	was_orgsz=0;
	was_aabb=0;
	was_rad=0;
	rad=1.0;
	fn=16;
	clrmat=0;
	
	for(i=0; i<narg; i++)
	{
		if(argn[i]==btm_pgmtok_mins)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], mins, 3);
			was_aabb=1;
		}

		if(argn[i]==btm_pgmtok_maxs)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], maxs, 3);
			was_aabb=1;
		}

		if(argn[i]==btm_pgmtok_org)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], org, 3);
		}

		if(argn[i]==btm_pgmtok_size)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], size, 3);
			was_orgsz=1;
		}

		if((argn[i]==btm_pgmtok_rad) || (argn[i]==btm_pgmtok_r))
		{
			rad=BTM_VarUnwrapFloat(argv[i]);
			was_rad=1;
		}

		if(argn[i]==btm_pgmtok_color)
		{
//			clrmat=BTM_VarUnwrapInt(argv[i])&0xFFFFFF;
			clrmat=(BTM_PgmCsg_NameToColor(argv[i])&0xFFFFFF)|
				(clrmat&(~0xFFFFFF));
		}

		if(argn[i]==btm_pgmtok_bone)
		{
			clrmat=
				clrmat&(~0x000000FF00000000ULL)|
				((BTM_VarUnwrapInt(argv[i])&0xFFULL)<<32);
		}
	}

	if(was_aabb)
	{
		TKRA_Vec3F_ScaleAddScale(mins, 0.5, maxs, 0.5, org);
		TKRA_Vec3F_Sub(maxs, org, size);
	}else
		if(was_rad)
	{
		size[0]=rad;
		size[1]=rad;
		size[2]=rad;
	}
	
//	bru=BTM_AllocBrushAabb(mins, maxs);
	bru=BTM_AllocBrushSphereScale(org, size, fn);
	csgn=BTM_MakeCsgNodeFromBrushList(bru);
	csgn->clrmat=clrmat;
	val=BTM_VarWrapPointer(csgn, btm_vartag_csgnode);
	return(val);
}

u64 BTM_PgmCmd_CsgCylinder(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs)
{
	u16 argn[32];
	u64 argv[32];
	float org[3], size[3], orgb[3];
	float mins[3], maxs[3];
	float rot[4], ang[3];
	float mat[16];
	u16 *tcs;
	BTM_CsgBrush *bru;
	BTM_CsgNode *csgn;
	u64 val, clrmat;
	float rad, hgt, r1, r2;
	int was_orgsz, was_aabb, was_rad, has_rot;
	int i, j, k, narg, fn;

	tcs=*rtcs;
	tcs++;
	narg=BTM_PgmDoEvalNamedArgsList(wrl, pgm, &tcs, argn, argv);
	*rtcs=tcs;
	
	TKRA_Vec3F_Zero(mins);
	TKRA_Vec3F_Zero(maxs);
	TKRA_Vec3F_Zero(org);
	TKRA_Vec3F_Zero(size);
	TKRA_Vec3F_Zero(ang);
	QuatF_Identity(rot);
	was_orgsz=0;
	was_aabb=0;
	was_rad=0;
	has_rot=0;
	rad=1.0;
	hgt=1.0;
	fn=16;
	clrmat=0;
	
	for(i=0; i<narg; i++)
	{
		if(argn[i]==btm_pgmtok_mins)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], mins, 3);
			was_aabb=1;
		}

		if(argn[i]==btm_pgmtok_maxs)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], maxs, 3);
			was_aabb=1;
		}

		if(argn[i]==btm_pgmtok_org)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], org, 3);
		}

		if(argn[i]==btm_pgmtok_size)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], size, 3);
			was_orgsz=1;
		}

		if((argn[i]==btm_pgmtok_hgt) || (argn[i]==btm_pgmtok_h))
		{
			hgt=BTM_VarUnwrapFloat(argv[i]);
		}

		if((argn[i]==btm_pgmtok_rad) || (argn[i]==btm_pgmtok_r))
		{
			rad=BTM_VarUnwrapFloat(argv[i]);
			r1=rad;
			r2=rad;
			was_rad=1;
		}

		if(argn[i]==btm_pgmtok_r1)
		{
			r1=BTM_VarUnwrapFloat(argv[i]);
			was_rad=1;
		}

		if(argn[i]==btm_pgmtok_r2)
		{
			r2=BTM_VarUnwrapFloat(argv[i]);
			was_rad=1;
		}

		if(argn[i]==btm_pgmtok_fn)
		{
			fn=BTM_VarUnwrapInt(argv[i]);
		}

		if(argn[i]==btm_pgmtok_rot)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], rot, 4);
			has_rot=1;
		}

		if(argn[i]==btm_pgmtok_angles)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], ang, 3);
			ang[0]=ang[0]*(M_PI/180.0);
			ang[1]=ang[1]*(M_PI/180.0);
			ang[2]=ang[2]*(M_PI/180.0);
			QuatF_FromAngles(ang, rot);
			has_rot=1;
		}

		if(argn[i]==btm_pgmtok_color)
		{
//			clrmat=BTM_VarUnwrapInt(argv[i])&0xFFFFFF;
			clrmat=(BTM_PgmCsg_NameToColor(argv[i])&0xFFFFFF)|
				(clrmat&(~0xFFFFFF));
		}

		if(argn[i]==btm_pgmtok_bone)
		{
			clrmat=
				clrmat&(~0x000000FF00000000ULL)|
				((BTM_VarUnwrapInt(argv[i])&0xFFULL)<<32);
		}
	}

	if(was_aabb)
	{
		TKRA_Vec3F_ScaleAddScale(mins, 0.5, maxs, 0.5, org);
		TKRA_Vec3F_Sub(maxs, org, size);
		
		hgt=size[2];
		rad=size[0];
		r1=rad;
		r2=rad;
	}else
		if(!was_rad)
	{
//		size[0]=rad;
//		size[1]=rad;
//		size[2]=rad;
	}

	if(has_rot)
	{
		QuatF_ToMatrix(rot, mat);
		mat[12]+=org[0];
		mat[13]+=org[1];
		mat[14]+=org[2];
		TKRA_Vec4F_Zero(orgb);
	}else
	{
		TKRA_Mat4F_Identity(mat);
		TKRA_Vec4F_Copy(org, orgb);
	}

//	bru=BTM_AllocBrushAabb(mins, maxs);
//	bru=BTM_AllocBrushSphereScale(org, size, fn);
	bru=BTM_AllocBrushCylinder(orgb, hgt, r1, r2, fn);
	csgn=BTM_MakeCsgNodeFromBrushList(bru);
	csgn->clrmat=clrmat;
	TKRA_Mat4F_Copy(mat, csgn->trans);
	val=BTM_VarWrapPointer(csgn, btm_vartag_csgnode);
	return(val);
}

u64 BTM_PgmCmd_CsgUnion(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs)
{
	u16 argn[32];
	u64 argv[32];
	BTM_CsgNode *prims[32];
	float org[3], rot[4], scale[3], ang[3];
	float mat[16];
	u16 *tcs;
	BTM_CsgBrush *bru;
	BTM_CsgNode *csgn;
	u64 val, clrmat;
	int i, j, k, narg, nprim, orgsz, unopr;

	unopr=0;
	tcs=*rtcs;
	if(tcs[0]==btm_pgmtok_csgdifference)
		unopr=1;
	if(tcs[0]==btm_pgmtok_csgintersection)
		unopr=2;
	if(tcs[0]==btm_pgmtok_csgdiff)
		unopr=1;
	if(tcs[0]==btm_pgmtok_csgisec)
		unopr=2;

	tcs++;
	narg=BTM_PgmDoEvalNamedArgsList(wrl, pgm, &tcs, argn, argv);
	*rtcs=tcs;
	nprim=0;
	clrmat=0;

	TKRA_Vec3F_Zero(org);
//	TKRA_Vec3F_Zero(rot);
	TKRA_Vec3F_Zero(ang);
	QuatF_Identity(rot);
	
	for(i=0; i<narg; i++)
	{
		if(argn[i]==0)
		{
			if(!BTM_VarIsPointerTag(argv[i], btm_vartag_csgnode))
				continue;
			prims[nprim++]=BTM_VarUnwrapPointer(argv[i]);
			continue;
		}

		if(argn[i]==btm_pgmtok_org)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], org, 3);
		}
		if(argn[i]==btm_pgmtok_rot)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], rot, 4);
		}

		if(argn[i]==btm_pgmtok_angles)
		{
			BTM_VarUnwrapVectorAsVf(argv[i], ang, 3);
			ang[0]=ang[0]*(M_PI/180.0);
			ang[1]=ang[1]*(M_PI/180.0);
			ang[2]=ang[2]*(M_PI/180.0);
			QuatF_FromAngles(ang, rot);
		}

		if(argn[i]==btm_pgmtok_color)
		{
//			clrmat=(clrmat&(~0xFFFFFF))|
//				(BTM_VarUnwrapInt(argv[i])&0xFFFFFF);
			clrmat=(BTM_PgmCsg_NameToColor(argv[i])&0xFFFFFF)|
				(clrmat&(~0xFFFFFF));
		}

		if(argn[i]==btm_pgmtok_bone)
		{
			clrmat=
				clrmat&(~0x000000FF00000000ULL)|
				((BTM_VarUnwrapInt(argv[i])&0xFFULL)<<32);
		}
	}

	QuatF_ToMatrix(rot, mat);
	mat[12]+=org[0];
	mat[13]+=org[1];
	mat[14]+=org[2];

	csgn=BTM_MakeCsgNodeUnionMulti(prims, nprim);
	csgn->clrmat=clrmat;
	csgn->opr=unopr;
	TKRA_Mat4F_Copy(mat, csgn->trans);
	val=BTM_VarWrapPointer(csgn, btm_vartag_csgnode);
	return(val);
}

BTM_SolidMesh *BTM_ProcCsgToMesh(u64 val, BTM_SolidMesh *olst)
{
	char tb[256];
	float tmat[16];
	BTM_CsgPoly *plmat[256];
	u64 plclr[256];
	int ntv[4];
	btm_vec3f v0, v1, v2, v3;
	float *tris;
	BTM_SolidMesh *mtmp, *mlst;
	BTM_CsgNode *csgn;
	BTM_CsgPoly *plst, *plcur, *plnxt;
	char *matname;
	float f;
	int i, j, k, nplmat, plix, ntris;
	
	if(!BTM_VarIsPointerTag(val, btm_vartag_csgnode))
		return(olst);
	
	csgn=BTM_VarUnwrapPointer(val);
	if(!val)
		return(olst);
	
	TKRA_Mat4F_Identity(tmat);
	plst=BTM_GetCsgPolysForCsgNode(csgn, NULL, tmat, 0, 0);
	
	nplmat=0;
	plcur=plst;
	while(plcur)
	{
		for(plix=0; plix<nplmat; plix++)
			if(plclr[plix]==plcur->clrmat)
				break;
		if(plix>=nplmat)
		{
			plix=nplmat++;
			plmat[plix]=NULL;
			plclr[plix]=plcur->clrmat;
		}
	
		plnxt=plcur->next;
		
		plcur->next=plmat[plix];
		plmat[plix]=plcur;
		
		plcur=plnxt;
	}
	
	mlst=olst;
	for(plix=0; plix<nplmat; plix++)
	{
		tris=btm_malloc(256*9*sizeof(float));
		ntv[0]=0;
		ntv[1]=256;

		ntris=0;
		plcur=plmat[plix];
		while(plcur)
		{
			BTM_Mesh_TrisEmitPolygon(&tris, ntv, plcur->pts, plcur->npts);
			plcur=plcur->next;
		}
		
		mtmp=BTM_AllocMesh();
		mtmp->tris=tris;
		mtmp->nTris=ntv[0];
		mtmp->mTris=ntv[1];

		mtmp->bbox[0]= 999999999.0;
		mtmp->bbox[1]= 999999999.0;
		mtmp->bbox[2]= 999999999.0;
		mtmp->bbox[3]=-999999999.0;
		mtmp->bbox[4]=-999999999.0;
		mtmp->bbox[5]=-999999999.0;

		mtmp->scale[0]=1.0;
		mtmp->scale[1]=1.0;
		mtmp->scale[2]=1.0;
		
		mtmp->clrmat=plclr[plix];
		
		sprintf(tb, "#%06llX", mtmp->clrmat&0xFFFFFF);
		mtmp->usetex[0]=bccx_strdup(tb);
		
		if(plclr[plix])
		{
			mtmp->baseclr[0]=((plclr[plix]>>16)&255)*(1.0/255.0);
			mtmp->baseclr[1]=((plclr[plix]>> 8)&255)*(1.0/255.0);
			mtmp->baseclr[2]=((plclr[plix]>> 0)&255)*(1.0/255.0);
			mtmp->baseclr[3]=1.0;
		}

		mtmp->norm=btm_malloc(mtmp->mTris*4*sizeof(float));
		for(i=0; i<mtmp->nTris; i++)
		{
			for(j=0; j<3; j++)
				for(k=0; k<3; k++)
			{
				f=mtmp->tris[i*9+j*3+k];
				if(f<mtmp->bbox[0+k])
					mtmp->bbox[0+k]=f;
				if(f>mtmp->bbox[3+k])
					mtmp->bbox[3+k]=f;
			}
		
			v0=btm_v3f_sub(
				btm_mkvec3fv(mtmp->tris+i*9+3),
				btm_mkvec3fv(mtmp->tris+i*9+0));
			v1=btm_v3f_sub(
				btm_mkvec3fv(mtmp->tris+i*9+6),
				btm_mkvec3fv(mtmp->tris+i*9+3));
			v2=btm_v3f_cross(v0, v1);
			v3=btm_v3f_norm(v2);

			mtmp->norm[i*4+0]=btm_v3f_x(v3);
			mtmp->norm[i*4+1]=btm_v3f_y(v3);
			mtmp->norm[i*4+2]=btm_v3f_z(v3);
			mtmp->norm[i*4+3]=btm_v3f_dot(btm_mkvec3fv(mtmp->tris+i*9+0), v3);
		}

		mtmp->next=mlst;
		mlst=mtmp;
	}
	
	return(mlst);
}
