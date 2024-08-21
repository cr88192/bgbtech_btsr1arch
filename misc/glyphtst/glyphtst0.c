#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <time.h>


#ifndef BTIC2F_BYTE
#define BTIC2F_BYTE
typedef unsigned char			byte;
typedef unsigned short		u16;
typedef unsigned int			u32;
typedef unsigned long long	u64;

typedef signed char			sbyte;
typedef signed short			s16;
typedef signed int			s32;
typedef signed long long		s64;
#endif

#define BTMGL_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define BGTF_MAGIC	BTMGL_FOURCC('B', 'G', 'T', 'F')

#define debug_break __debugbreak();

#include "cca_ast2.h"

#include "bt1h_targa.c"

#include "cca_node.c"
#include "cca_print.c"
#include "cca_parse.c"
#include "cca_abxe.c"


typedef struct {
short x;
short y;
byte tag;	//tag bits
byte cnix;	//component name index
byte xsc;
byte ysc;
}BGTF_Vert2;

typedef struct BGTF_Glyph_s BGTF_Glyph;

struct BGTF_Glyph_s {
BGTF_Glyph *next;
BGTF_Vert2 *verts;
BGTF_Vert2 *spans;
int nvert;
int nspan;
int utf;
int adv_x;
int org_x, org_y;
int min_x, min_y;
int max_x, max_y;
char *keyname;
char *name;
char **comp_name;
};


void *btm_malloc(int sz)
{
	void *ptr;
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

void btm_free(void *ptr)
{
	free(ptr);
}

void *btm_realloc(void *ptr, int sz)
{
	return(realloc(ptr, sz));
}

byte *BTM_LoadFileI(char *name, int *rsz, int pad)
{
	char tb[256];
	byte *buf;
	FILE *fd;
	int sz, i;

//	sprintf(tb, "resource/%s", name);
	strcpy(tb, name);
	
//	fd=fopen(name, "rb");
	fd=fopen(tb, "rb");
	if(!fd)
	{
//		buf=FS_LoadFile(name, rsz, pad);
//		if(buf)
//			return(buf);
		
		printf("BTM_LoadFile: Failed open %s\n", name);
		return(NULL);
	}
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
//	if(pad&4)
//		{ buf=BTM_AllocTempBuffer(sz+24); }
//	else if(pad&2)
//		{ buf=BTM_AllocMul64K((sz+65535)>>16); }
//	else if(pad&1)
//		{ buf=btm_malloc(sz+24); }
//	else
//		{ buf=btm_malloc(sz); }

	buf=btm_malloc(sz+24);
	i=fread(buf, 1, sz, fd);
	fclose(fd);
	
	if(i!=sz)
	{
		printf("BTM_LoadFile: Size mismatch %s exp=%d got=%d\n",
			name, sz, i);
		if(i>0)
		{
			sz=i;
		}else
		{
			if(!(pad&6))
				btm_free(buf);
			return(NULL);
		}
	}
	
	if(pad&1)
	{
//		*(u64 *)(buf+sz+0)=0;
//		*(u64 *)(buf+sz+8)=0;
		memset(buf+sz, 0, 16);
	}
	
	*rsz=sz;
	return(buf);
}

byte *BTM_LoadFile(char *name, int *rsz)
{
	return(BTM_LoadFileI(name, rsz, 1));
}

int BTM_StoreFile(char *name, void *buf, int sz)
{
	char tb[256];
	FILE *fd;

//	sprintf(tb, "resource/%s", name);
	strcpy(tb, name);

//	fd=fopen(name, "wb");
	fd=fopen(tb, "wb");
	if(!fd)
		return(-1);
	
	fwrite(buf, 1, sz, fd);
	fclose(fd);
	return(0);
}


BCCX_Node *BCCX_LoadXmlFile(char *name)
{
	BCCX_Node *node;
	char *buf;
	int sz;
	
	sz=0;
	buf=(char *)BTM_LoadFile(name, &sz);
	if(!buf)
		return(NULL);
	node=BCCX_ParseExprStr(buf);
	BCCX_MarkTreeZone(node, BCCX_ZTY_MODULE);
	btm_free(buf);
	return(node);
}

byte BGTF_Float2Fp8u(float fv)
{
	int e, v;
	float fr;
	
	fr=fv;
	if(fr<0)
		fr=-fr;
		
	e=7;
	while(fr<1.0)
		{ e--; fr=fr*2.0; }
	while(fr>=2.0)
		{ e++; fr=fr*0.5; }

	v=fr*16.0;

	if(v>=32)
		{ e++; v=v>>1; }
	v=(e<<4)|(v&15);
	
	if(e<0)
		return(0x00);
	if(e>15)
		return(0xFF);
	
	return(v);
}

float BGTF_Fp8u2Float(byte v)
{
	int e;
	float fr;
	
	e=((v>>4)&15)-7;
	fr=(16.0+(v&15))*(1.0/16.0);
	while(e>0)
		{ e--; fr=fr*2.0; }
	while(e<0)
		{ e++; fr=fr*0.5; }
	return(fr);
}


BGTF_Glyph *BGTF_LoadGlif(char *fname)
{
	static BGTF_Vert2 *verts;
	static int sz_verts;
	static BGTF_Vert2 spans[64];
	static char *cname[64];

	BCCX_Node *root, *n0, *n1, *n2, *n3;
	BCCX_Node *glyph, *outline;
	BGTF_Glyph *tmp;
	char *s_ty, *s_sm, *s0, *s1;
	int ni0, ni1, ni2, vi0, vi1;
	float xsc, ysc;
	int i, j, k, n;
	int vx, vy, vt, nv, ns, nv0, ncn;
	int bb_mx, bb_my, bb_nx, bb_ny, org_x, org_y;
	
	root=BCCX_LoadXmlFile(fname);
	if(!root)
		return(NULL);
	
	if(!verts)
	{
		k=512;
		verts=malloc(k*sizeof(BGTF_Vert2));
		sz_verts=k;
	}
	
	glyph=BCCX_FindTag(root, "glyph");
	outline=BCCX_FindTag(glyph, "outline");
	
	nv=0;
	ns=0;
	ncn=0;
	
	bb_mx= 999999;
	bb_my= 999999;
	bb_nx=-999999;
	bb_ny=-999999;
	org_x=0;
	org_y=0;
	
	ni0=BCCX_GetNodeChildCount(outline);
	for(i=0; i<ni0; i++)
	{
		n0=BCCX_GetNodeIndex(outline, i);

		if(BCCX_TagIsP(n0, "contour"))
		{
			ni1=BCCX_GetNodeChildCount(n0);
			nv0=nv;		vi0=nv;		vi1=nv;
			
			for(j=0; j<ni1; j++)
			{
				n1=BCCX_GetNodeIndex(n0, j);
				if(BCCX_TagIsP(n1, "point"))
				{
					vx=BCCX_GetInt(n1, "x");
					vy=BCCX_GetInt(n1, "y");
					s_ty=BCCX_Get(n1, "type");
					s_sm=BCCX_Get(n1, "smooth");
					if(!s_ty)
						s_ty="offcurve";
					if(!s_sm)
						s_sm="no";
					
					vt=0x10;
					if(j==0)
					{
						vt=0x20;
						vi0=nv;
					}
					if(j==(ni1-1))
					{
						vt|=0x30;
						vi1=nv;
					}

					if(!strcmp(s_ty, "curve"))
						vt|=1;

					if(!strcmp(s_ty, "qcurve"))
					{
						/* curve if last point is offcurve, else line. */
						if(verts[nv-1].tag&4)
							vt|=1;
					}

					if(!strcmp(s_sm, "yes"))
						vt|=2;
					if(!strcmp(s_ty, "offcurve"))
						vt|=4;
					
					if(vx<bb_mx)
						bb_mx=vx;
					if(vy<bb_my)
						bb_my=vy;

					if(vx>bb_nx)
						bb_nx=vx;
					if(vy>bb_ny)
						bb_ny=vy;
										
					verts[nv].x=vx;
					verts[nv].y=vy;
					verts[nv].tag=vt;
					verts[nv].cnix=0;
					nv++;
				}
			}
			
			if(nv!=nv0)
			{
				spans[ns].x=vi0;
				spans[ns].y=vi1+1;
				ns++;
			}

			continue;
		}

		if(BCCX_TagIsP(n0, "anchor"))
		{
			vx=BCCX_GetInt(n0, "x");
			vy=BCCX_GetInt(n0, "y");
			s0=BCCX_Get(n0, "name");
			continue;
		}

		if(BCCX_TagIsP(n0, "component"))
		{
			vx=BCCX_GetInt(n0, "xOffset");
			vy=BCCX_GetInt(n0, "yOffset");
			xsc=BCCX_GetFloat(n0, "xScale");
			ysc=BCCX_GetFloat(n0, "yScale");
			s0=BCCX_Get(n0, "base");

			if(xsc==0)
				xsc=1.0;
			if(ysc==0)
				ysc=1.0;

			for(j=0; j<ncn; j++)
			{
				if(!strcmp(cname[j], s0))
					break;
			}
			if(j>=ncn)
			{
				j=ncn++;
				cname[j]=bccx_strdup(s0);
			}

			vt=0x10;
			
			if(xsc<0)
				vt|=0x01;
			if(ysc<0)
				vt|=0x02;

			verts[nv].x=vx;
			verts[nv].y=vy;
			verts[nv].tag=vt;
			verts[nv].cnix=j+1;
			verts[nv].xsc=BGTF_Float2Fp8u(xsc);
			verts[nv].ysc=BGTF_Float2Fp8u(ysc);
			nv++;
			continue;
		}
	}

	tmp=btm_malloc(sizeof(BGTF_Glyph));

	if(nv)
	{
		tmp->verts=btm_malloc(nv*sizeof(BGTF_Vert2));
		memcpy(tmp->verts, verts, nv*sizeof(BGTF_Vert2));
	}

	if(ns)
	{
		tmp->spans=btm_malloc(ns*sizeof(BGTF_Vert2));
		memcpy(tmp->spans, spans, ns*sizeof(BGTF_Vert2));
	}

	tmp->nvert=nv;
	tmp->nspan=ns;

	if(ncn)
	{
		tmp->comp_name=btm_malloc((ncn+1)*sizeof(char *));
		memcpy(tmp->comp_name, cname, ncn*sizeof(char *));
	}

	tmp->min_x=bb_mx;
	tmp->min_y=bb_my;
	tmp->max_x=bb_nx;
	tmp->max_y=bb_ny;

	tmp->utf=-1;
	n0=BCCX_FindTag(glyph, "unicode");
	if(n0)
	{
		s0=BCCX_Get(n0, "hex");
		if(s0)
			{ tmp->utf=strtoll(s0, NULL, 16); }
	}

	n0=BCCX_FindTag(glyph, "advance");
	if(n0)
	{
		tmp->adv_x=BCCX_GetInt(n0, "width");
	}
	
//	BCCX_FreeNode(root);
	BCCX_ClearZoneLevel(BCCX_ZTY_MODULE);
	
	
	return(tmp);
}

BGTF_Glyph *BGTF_LookupGlifKeyList(BGTF_Glyph *list, char *name)
{
	BGTF_Glyph *cur;
	
	cur=list;
	while(cur)
	{
		if(cur->keyname && !strcmp(cur->keyname, name))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

int BGTF_CopyTranslateVerts(
	BGTF_Vert2 *dst, BGTF_Vert2 *src, int cnt, BGTF_Vert2 xlat)
{
//	u32 v0, v1, v2;
	BGTF_Vert2 v0, v1, v2;
	float xsc, ysc;
	int i, j, k;
	
	xsc=BGTF_Fp8u2Float(xlat.xsc);
	ysc=BGTF_Fp8u2Float(xlat.ysc);
	
	if((xlat.tag^(xlat.tag>>1))&1)
	{
		for(i=0; i<cnt; i++)
		{
			v0=src[cnt-i-1];
			v1=v0;
			
			if((i==0) || ((i+1)==cnt))
			{
				v1.tag^=0x10;
			}
			
			if(xlat.tag&0x01)
				v0.x=-v0.x;
			if(xlat.tag&0x02)
				v0.y=-v0.y;
			
			v1.x=v0.x*xsc+xlat.x;
			v1.y=v0.y*ysc+xlat.y;
			dst[i]=v1;
		}
		return(0);
	}
	
	for(i=0; i<cnt; i++)
	{
		v0=src[i];
		v1=v0;
		
		if(xlat.tag&0x01)
			v0.x=-v0.x;
		if(xlat.tag&0x02)
			v0.y=-v0.y;
		
		v1.x=v0.x+xlat.x;
		v1.y=v0.y+xlat.y;
		dst[i]=v1;
	}
	return(0);
}

int BGTF_BuildGlifComponents(BGTF_Glyph *glif, BGTF_Glyph *list)
{
	static BGTF_Vert2 *verts;
	static int sz_verts;
	static BGTF_Vert2 spans[64];
	static BGTF_Vert2 comps[64];

	BGTF_Glyph *gref;
	char *s_ty, *s_sm, *s0, *s1;
	int ni0, ni1, ni2, vi0, vi1;
	int i, j, k, n;
	int vx, vy, vt, nv, ns, nv0, nc, ncn;
	int bb_mx, bb_my, bb_nx, bb_ny, org_x, org_y;
		
	if(!verts)
	{
		k=512;
		verts=malloc(k*sizeof(BGTF_Vert2));
		sz_verts=k;
	}
	
	ncn=0;
	for(i=0; i<glif->nvert; i++)
	{
		if(glif->verts[i].cnix)
			{ comps[ncn++]=glif->verts[i]; }
	}
	
	nv=0;
	ns=0;

	for(i=0; i<glif->nspan; i++)
	{
		vx=glif->spans[i].x;
		vy=glif->spans[i].y;
		k=vy-vx;
		memcpy(verts+nv, glif->verts+vx, k*sizeof(BGTF_Vert2));
		spans[ns].x=nv;
		spans[ns].y=nv+k;
		nv+=k;
		ns++;
	}
	
	for(i=0; i<ncn; i++)
	{
		gref=BGTF_LookupGlifKeyList(list, glif->comp_name[comps[i].cnix-1]);
		if(!gref)
			continue;
		if(gref->comp_name)
		{
			BGTF_BuildGlifComponents(gref, list);
		}

		for(j=0; j<gref->nspan; j++)
		{
			vx=gref->spans[j].x;
			vy=gref->spans[j].y;
			k=vy-vx;
			BGTF_CopyTranslateVerts(
				verts+nv, gref->verts+vx, k, comps[i]);
			spans[ns].x=nv;
			spans[ns].y=nv+k;
			nv+=k;
			ns++;
		}
	}

	bb_mx= 999999;
	bb_my= 999999;
	bb_nx=-999999;
	bb_ny=-999999;

	for(i=0; i<nv; i++)
	{
		vx=verts[i].x;
		vy=verts[i].y;

		if(vx<bb_mx)
			bb_mx=vx;
		if(vy<bb_my)
			bb_my=vy;
		if(vx>bb_nx)
			bb_nx=vx;
		if(vy>bb_ny)
			bb_ny=vy;
	}

	
	if(nv)
	{
		glif->verts=btm_malloc(nv*sizeof(BGTF_Vert2));
		memcpy(glif->verts, verts, nv*sizeof(BGTF_Vert2));
	}

	if(ns)
	{
		glif->spans=btm_malloc(ns*sizeof(BGTF_Vert2));
		memcpy(glif->spans, spans, ns*sizeof(BGTF_Vert2));
	}

	glif->nvert=nv;
	glif->nspan=ns;
	glif->comp_name=NULL;

	glif->min_x=bb_mx;
	glif->min_y=bb_my;
	glif->max_x=bb_nx;
	glif->max_y=bb_ny;

	return(0);
}

BGTF_Glyph *BGTF_LoadGlifContents(char *base)
{
	static char *names[65536+256];
	static char *keys[65536+256];
	char tb[256];
	char *s0, *s1;
	char *key;
	BCCX_Node *root, *n0, *n1, *n2, *n3;
	BCCX_Node *plist, *dict;
	BGTF_Glyph *tmp, *lst;
	int ng, ni0, ni1;
	int i, j, k;
	
	sprintf(tb, "%s/contents.plist", base);

	root=BCCX_LoadXmlFile(tb);
	if(!root)
		return(NULL);

	plist=BCCX_FindTag(root, "plist");
	if(!plist)
		return(NULL);
	dict=BCCX_FindTag(plist, "dict");
	if(!dict)
		return(NULL);
	
	lst=NULL;
	ng=0;
	key=NULL;

	ni0=BCCX_GetNodeChildCount(dict);
	for(i=0; i<ni0; i++)
	{
		n0=BCCX_GetNodeIndex(dict, i);

		if(BCCX_TagIsP(n0, "key"))
		{
			n1=BCCX_Child(n0);
			if(!n1)
				continue;
			s0=BCCX_Text(n1);
			if(!s0)
				continue;
			
			key=bccx_strdup(s0);
		}

		if(BCCX_TagIsP(n0, "string"))
		{
			n1=BCCX_Child(n0);
			if(!n1)
				continue;
			s0=BCCX_Text(n1);
			if(!s0)
				continue;
			
			keys[ng]=key;
			names[ng]=bccx_strdup(s0);
			ng++;
		}
	}

	for(i=0; i<ng; i++)
	{
		sprintf(tb, "%s/%s", base, names[i]);
		tmp=BGTF_LoadGlif(tb);
		
		if(tmp)
		{
			tmp->name=names[i];
			tmp->keyname=keys[i];
			tmp->next=lst;
			lst=tmp;
		}
	}
	
	tmp=lst;
	while(tmp)
	{
		if(tmp->comp_name)
		{
			BGTF_BuildGlifComponents(tmp, lst);
		}
		tmp=tmp->next;
	}
	
	return(lst);
}


// BGTF_Vert2 vertspan[8];
// int vertspcnt;

int clamp(int x, int m, int n)
{
	if(x<m)
		return(m);
	if(x>n)
		return(n);
	return(x);
}

int dot2(int px, int py, int vx, int vy)
{
	int d;
	
	d=vx*px+vy*py;
	return(d);
}

int checkline(
	int rx0, int ry0,
	int rx1, int ry1,
	int vx0, int vy0,
	int vx1, int vy1)
{
	int rdx, rdy, vdx, vdy;
	int rd0, vd0, rd1, vd1;
	int d0, d1, d2, d3;
	
	rdx=rx1-rx0;	rdy=ry1-ry0;
	vdx=vx1-vx0;	vdy=vy1-vy0;
	
	rd0=dot2(rx0, ry0, -rdy, rdx);
	vd0=dot2(vx0, vy0, -vdy, vdx);
	rd1=dot2(rx1, ry1, -rdy, rdx);
	vd1=dot2(vx1, vy1, -vdy, vdx);
	
	if(rd0!=rd1)
	{
		printf("checkline: Dot R %d %d\n", rd0, rd1);
	}
	
	if(vd0!=vd1)
	{
		printf("checkline: Dot V %d %d\n", vd0, vd1);
	}
	
	d0=dot2(rx0, ry0, -vdy, vdx)-vd0;
	d1=dot2(rx1, ry1, -vdy, vdx)-vd0;
	d2=dot2(vx0, vy0, -rdy, rdx)-rd0;
	d3=dot2(vx1, vy1, -rdy, rdx)-rd0;
	
	if((d0^d1)>=0)
		return(0);
	if((d2^d3)>=0)
		return(0);
	
	if(d0<0)
		return(3);
	
	return(1);
}

#if 1
int checkcurve(
	int rx0, int ry0, int rx1, int ry1,
	int vx0, int vy0, int vx1, int vy1,
	int vx2, int vy2, int vx3, int vy3,
	byte tg0, byte tg1, byte tg2, byte tg3)
{
	int px0, py0, px1, py1, px, py, d;
	int i0, i1, i2, i3;
	
	i0=vx2-vx1;
	i1=vy2-vy1;
	i0=i0^(i0>>31);
	i1=i1^(i1>>31);
//	d=i0+i1;
	if(i0>i1)
		d=i0+(i1>>1);
	else
		d=i1+(i0>>1);

	if(d<96)
	{
		return(checkline(rx0, ry0, rx1, ry1, vx1, vy1, vx2, vy2));
	}
	
//	px0=2*vx1-vx0;
//	py0=2*vy1-vy0;
//	px1=2*vx2-vx3;
//	py1=2*vy2-vy3;

//	px0=(3*vx1-vx0)/2;
//	py0=(3*vy1-vy0)/2;
//	px1=(3*vx2-vx3)/2;
//	py1=(3*vy2-vy3)/2;

	px0=(5*vx1-vx0)/4;
	py0=(5*vy1-vy0)/4;
	px1=(5*vx2-vx3)/4;
	py1=(5*vy2-vy3)/4;

	px=(px0+px1)/2;
	py=(py0+py1)/2;

//	px=(vx1+vx2)/2;
//	py=(vy1+vy2)/2;

//	i0=checkline(vx1, vy1, px, py, rx0, ry0, rx1, ry1);
//	i1=checkline(px, py, vx2, vy2, rx0, ry0, rx1, ry1);
//	if(!i0 && !i1)
//		return(0);

	px0=(5*vx1-vx0)/4;
	py0=(5*vy1-vy0)/4;
	px1=(5*vx2-vx3)/4;
	py1=(5*vy2-vy3)/4;

	px0=(3*px0+px)/4;
	py0=(3*py0+py)/4;
	px1=(3*px1+px)/4;
	py1=(3*py1+py)/4;

//	px=(px+vx2)/2;
//	py=(py+vy2)/2;
//	px=(2*vx1+px+vx2)/4;
//	py=(2*vy1+py+vy2)/4;
//	px=(4*vx1+px+3*vx2)/8;
//	py=(4*vy1+py+3*vy2)/8;
	
//	if(d>=192)
	if(0)
	{
		i0=checkline(rx0, ry0, rx1, ry1,
			vx1, vy1,
			px0, py0);
		i1=checkline(rx0, ry0, rx1, ry1,
			px0, py0,
			px, py);
		i2=checkline(rx0, ry0, rx1, ry1,
			px, py,
			px1, py1);
		i3=checkline(rx0, ry0, rx1, ry1,
			px1, py1,
			vx2, vy2);

		if(i0)
			return(i0);
		if(i1)
			return(i1);
		if(i2)
			return(i2);
		if(i3)
			return(i3);

		return(0);
	}else
	{
		i0=checkline(rx0, ry0, rx1, ry1,
			vx1, vy1,
			px, py);
		i1=checkline(rx0, ry0, rx1, ry1,
			px, py,
			vx2, vy2);

		if(i0)
			return(i0);
		if(i1)
			return(i1);

		return(0);
	}
}
#endif

int checkpoint2b(
	BGTF_Glyph *glif,
	int cn, int x0, int y0, int x1, int y1)
{
	int vm, vn, i0, i1, i2, i3;
	int i, j, k, lc, lp, ln, pp;
	
	vm=glif->spans[cn].x;
	vn=glif->spans[cn].y;
	lc=0;
	lp=0; ln=0;
	for(j=vm; j<vn; j++)
	{
		i0=j;
		i1=j+1;
		if(i1>=vn)
			i1=vm;

		i2=j-1;
		if(i2<0)
//			i2+=(vn-vm);
			i2=vn+i2;

		i3=j+2;
		if(i3>=vn)
//			i3-=(vn-vm);
			i3=vm+(i3-vn);
		
		if(!(glif->verts[i2].tag&0x06))
			i2=i0;
		if(!(glif->verts[i3].tag&0x06))
			i3=i1;

//		if(glif->verts[i0].tag&0x01)
		if(glif->verts[i1].tag&0x05)
//		if(	(glif->verts[i0].tag&0x05) &&
//			(glif->verts[i1].tag&0x05))
//		if(0)
		{
			k=checkcurve(x0, y0, x1, y1,
				glif->verts[i2].x, glif->verts[i2].y,
				glif->verts[i0].x, glif->verts[i0].y,
				glif->verts[i1].x, glif->verts[i1].y,
				glif->verts[i3].x, glif->verts[i3].y,
				glif->verts[i2].tag, glif->verts[i0].tag,
				glif->verts[i1].tag, glif->verts[i3].tag);
		}else
		{
			k=checkline(x0, y0, x1, y1,
				glif->verts[i0].x, glif->verts[i0].y,
				glif->verts[i1].x, glif->verts[i1].y);
		}
		if(k>0)
		{
			lc++;
			if(k&2)
				ln++;
			else
				lp++;
		}
	}
	
//	return(lp!=ln);
//	return(lp>ln);
//	return(lp<ln);

	return(lp-ln);
	
//	return(lc);
}

int checkpoint2(
	BGTF_Glyph *glif,
	int x0, int y0, int x1, int y1)
{
	int vm, vn, i0, i1;
	int i, j, k, lc, pp;
	
	pp=0;
	for(i=0; i<glif->nspan; i++)
	{
		lc=checkpoint2b(glif, i, x0, y0, x1, y1);
//		if(lc&1)
//		if(lc>=2)
		if(lc>0)
			pp=1;

		if(lc<0)
			pp=0;
		
//		if(lc==4)
//			pp=1;
	}
	return(pp);
}

int checkpoint2n(
	BGTF_Glyph *glif,
	int x0, int y0, int x1, int y1)
{
	int vm, vn, i0, i1;
	int i, j, k, lc, pp;
	
	pp=0;
	for(i=0; i<glif->nspan; i++)
	{
		lc=checkpoint2b(glif, i, x0, y0, x1, y1);
		if(lc!=0)
			pp=1;
	}
	return(pp);
}

int checkpoint3n(
	BGTF_Glyph *glif,
	int cn, int x0, int y0, int x1, int y1)
{
	int vm, vn, i0, i1;
	int i, j, k, lc, pp;
	
	pp=0;
	lc=checkpoint2b(glif, cn, x0, y0, x1, y1);
	if(lc!=0)
		pp=1;
	return(pp);
}

int checkpoint(BGTF_Glyph *glif, int x, int y)
{
	int pp, ppn, bi, cld, cldx, cldy;
	int i;
	
	bi=6;
	pp=0;
	cld=2048;
	
	cldx=cld^(x>>31);
	cldy=cld^(y>>31);

	if(checkpoint2(glif, x, y, x+cldx, y+cldy))
		pp|=15;

#if 0
	if(checkpoint2(glif, x-bi, y-bi, x+cld, y+cld))
		pp|=1;
	if(checkpoint2(glif, x-bi, y+bi, x+cld, y-cld))
		pp|=2;
	if(checkpoint2(glif, x+bi, y-bi, x-cld, y+cld))
		pp|=4;
	if(checkpoint2(glif, x+bi, y+bi, x-cld, y-cld))
		pp|=8;
#endif
	
#if 0
	if(pp==0)
	{
		for(i=0; i<glif->nspan; i++)
		{
			if((x!=0) && (y!=0))
			{
				if(!checkpoint3n(i, x, y, x*1024, y*1024))
					continue;
			}
		
			ppn=0;
			if(checkpoint3n(i, x+bi, y+bi, x+1024, y+1024))
				ppn|=1;
			if(checkpoint3n(i, x+bi, y-bi, x+1024, y-1024))
				ppn|=2;
			if(checkpoint3n(i, x-bi, y+bi, x-1024, y+1024))
				ppn|=4;
			if(checkpoint3n(i, x-bi, y-bi, x-1024, y-1024))
				ppn|=8;

			if(ppn!=15)
				continue;

			if(checkpoint3n(i, x+bi, y, x+1024, y))
				ppn|=16;
			if(checkpoint3n(i, x-bi, y, x-1024, y))
				ppn|=32;
			if(checkpoint3n(i, x, y+bi, x, y+1024))
				ppn|=64;
			if(checkpoint3n(i, x, y-bi, x, y-1024))
				ppn|=128;
			
	//		if((ppn==1) || (ppn==2) || (ppn==4) || (ppn==8))
	//			ppn=0;
			
			if(ppn==255)
				pp=15;
		}
	}
#endif

	return(pp);
}

u32 *bgtf_image;
int bgtf_szimage;
int bgtf_maxszimage;

int bgtf_ofs_pagedir;
int bgtf_ofs_pagetab[256];

int BGTF_ImageCheckExpand(int nw)
{
	int k;

	if(nw<=0)
		return(0);

	if((bgtf_szimage+nw)<bgtf_maxszimage)
		return(0);

	k=bgtf_maxszimage+(bgtf_maxszimage>>1);
	bgtf_image=btm_realloc(bgtf_image, k*sizeof(u32));
	bgtf_maxszimage=k;
	return(1);
}

int BGTF_ImageAllocWords(int nw)
{
	int ofs;
	BGTF_ImageCheckExpand(nw);
	ofs=bgtf_szimage;
	bgtf_szimage+=nw;
	
	memset(bgtf_image+ofs, 0, nw*sizeof(u32));
	return(ofs);
}

int BGTF_ImageSetGlyphOffset(int utf, int ofs)
{
	int pto, ixh, ixl;
	
	ixh=(utf>>8)&255;
	ixl=(utf>>0)&255;
	pto=bgtf_ofs_pagetab[ixh];
	if(!pto)
	{
		pto=BGTF_ImageAllocWords(256);
		bgtf_image[bgtf_ofs_pagedir+ixh]=pto;
		bgtf_ofs_pagetab[ixh]=pto;
	}

	bgtf_image[pto+ixl]=ofs;
	return(0);
}

int BGTF_ImageAddGlif(int utf, BGTF_Glyph *glif)
{
	u32 *img;
	int baseofs, szi;
	int vx, vy, vt;
	int i, j, k;
	
	if(utf<0)
	{
		printf("BGTF_ImageAddGlif: Reject %s\n", glif->name);
		return(0);
	}

	BGTF_ImageSetGlyphOffset(utf, 0);	//pre-warm table
	
	BGTF_ImageCheckExpand(glif->nvert+64);
	
	img=bgtf_image;
	szi=bgtf_szimage;
	baseofs=szi;
	
	vx=glif->adv_x;
	vy=glif->max_y-glif->min_y;
	vt=0x04;
	img[szi]=
		((vx&0xFFF)<< 0) |
		((vy&0xFFF)<<12) |
		((vt&0xFF)<<24);
	szi++;

	vx=glif->org_x;
	vy=glif->org_y;
	vt=0x05;
	if((vx|vy)!=0)
	{
		img[szi]=
			((vx&0xFFF)<< 0) |
			((vy&0xFFF)<<12) |
			((vt&0xFF)<<24);
		szi++;
	}

	vx=glif->min_x;
	vy=glif->min_y;
	vt=0x06;
	img[szi]=
		((vx&0xFFF)<< 0) |
		((vy&0xFFF)<<12) |
		((vt&0xFF)<<24);
	szi++;

	vx=glif->max_x;
	vy=glif->max_y;
	vt=0x07;
	img[szi]=
		((vx&0xFFF)<< 0) |
		((vy&0xFFF)<<12) |
		((vt&0xFF)<<24);
	szi++;
	
	for(i=0; i<glif->nvert; i++)
	{
		vx=glif->verts[i].x;
		vy=glif->verts[i].y;
		vt=glif->verts[i].tag;
		img[szi]=
			((vx&0xFFF)<< 0) |
			((vy&0xFFF)<<12) |
			((vt&0xFF)<<24);
		szi++;
	}
	
	img[szi]=0;
	szi++;
	
	bgtf_szimage=szi;

	BGTF_ImageSetGlyphOffset(utf, baseofs);	//pre-warm table
	
	printf("BGTF_ImageAddGlif: Add U%04X at %d  %s\n", utf, baseofs, glif->name);
	
	return(0);
}

int main()
{
	char tb[256];
	BGTF_Glyph *glif, *glst, *gcur;
	byte *ibuf, *obuf;
	char *ifn;
	int xs, ys, cxs, cys;
	int cr, cg, cb, cco, ccb;
	int cy0, cy1, cy2, cy3;
	int p, x, y, cx, cy, x0, y0, x1, y1;
	int bmx, bmy, bnx, bny;
	int i, j, k;
	
	k=4096;
	bgtf_image=btm_malloc(k*sizeof(u32));
	bgtf_maxszimage=k;

	bgtf_szimage=16;
	bgtf_ofs_pagedir=BGTF_ImageAllocWords(256);

	bgtf_image[0]=BGTF_MAGIC;
	bgtf_image[1]=bgtf_ofs_pagedir;
	bgtf_image[2]=8;

//	glif=BGTF_LoadGlif("Cantarell-Bold.ufo/glyphs/A_.glif");
//	glst=BGTF_LoadGlifContents("Cantarell-Bold.ufo/glyphs");
	glst=BGTF_LoadGlifContents("Cantarell-Regular.ufo/glyphs");
	
//	BGTF_ImageAddGlif(0x0041, glif);

	bmx= 999999;
	bmy= 999999;
	bnx=-999999;
	bny=-999999;

	gcur=glst;
	while(gcur)
	{
		glif=gcur;
		if(glif->utf>=0)
		{
			BGTF_ImageSetGlyphOffset(glif->utf, 0);
			
			if(glif->min_x<bmx)
				bmx=glif->min_x;
			if(glif->min_y<bmy)
				bmy=glif->min_y;
			if(glif->max_x>bnx)
				bnx=glif->max_x;
			if(glif->max_y>bny)
				bny=glif->max_y;
		}
		gcur=gcur->next;
	}

	cx=bnx-bmx;
	cy=bny-bmy;
//	bgtf_image[8]=(0x01<<24)|(1024<<12)|1024;
	bgtf_image[ 8]=(0x01<<24)|(cy<<12)|cx;
	bgtf_image[ 9]=(0x06<<24)|((bmy&0xFFF)<<12)|(bmx&0xFFF);
	bgtf_image[10]=(0x07<<24)|((bny&0xFFF)<<12)|(bnx&0xFFF);

	gcur=glst;
	while(gcur)
	{
		glif=gcur;
		BGTF_ImageAddGlif(glif->utf, glif);
		gcur=gcur->next;
	}

	
//	BTM_StoreFile("test0.gtf", bgtf_image, bgtf_szimage*4);
//	BTM_StoreFile("Cantarell-Bold.gtf", bgtf_image, bgtf_szimage*4);
	BTM_StoreFile("Cantarell-Regular.gtf", bgtf_image, bgtf_szimage*4);
	
	xs=2048;
	ys=2048;
	cxs=128;
	cys=128;
	
//	ifn="IMG_20230114_005526779_ds0.tga";
//	ibuf=BTIC1H_Img_LoadTGA(ifn, &xs, &ys);
	obuf=malloc(xs*ys*4);
	
	k=xs*ys;
	
	for(i=0; i<k; i++)
	{
		cr=128;
		cg=128;
		cb=128;
	
		obuf[i*4+0]=cr;
		obuf[i*4+1]=cg;
		obuf[i*4+2]=cb;
		obuf[i*4+3]=255;
	}
	
//	buildspan();
	
	gcur=glst;
	while(gcur)
	{
		glif=gcur;
		
		if(glif->utf<0)
		{
			gcur=gcur->next;
			continue;
		}

		if(glif->utf>=0x0100)
		{
			gcur=gcur->next;
			continue;
		}
		
		cx=(glif->utf>>0)&15;
		cy=(glif->utf>>4)&15;
		
		cy=15-cy;
		
		for(y=0; y<cys; y++)
			for(x=0; x<cxs; x++)
		{
			j=checkpoint(glif, (x-16)*12, (y-16)*12);
			if(j)
			{
				cg=0;
			}else
			{
				cg=255;
			}
			cr=cg;
			cb=cg;
			
			if(j && (j!=15))
				cb=255-cg;
			
			i=((cy*cys+y)*xs)+(cx*cxs+x);
			
			obuf[i*4+0]=cr;
			obuf[i*4+1]=cg;
			obuf[i*4+2]=cb;
			obuf[i*4+3]=255;
		}
		

//		sprintf(tb, "gldump/U%04X.tga", glif->utf);
//		BTIC1H_Img_SaveTGA(tb, obuf, xs, ys);
		
//		printf("wrote %s\n", tb);

		gcur=gcur->next;
	}

	BTIC1H_Img_SaveTGA("glyph0a.tga", obuf, xs, ys);
}
