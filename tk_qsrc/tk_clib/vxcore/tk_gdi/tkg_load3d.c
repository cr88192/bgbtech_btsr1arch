float TKGDI_TKO3D_Lerp(float f0, float f1, float frac)
{
	return((f0*(1.0-frac))+(f1*frac));
}

int TKGDI_TKO3D_VecLerp2(float *iv0, float *iv1, float *ov, float frac)
{
	ov[0]=TKGDI_TKO3D_Lerp(iv0[0], iv1[0], frac);
	ov[1]=TKGDI_TKO3D_Lerp(iv0[1], iv1[1], frac);
	return(0);
}

int TKGDI_TKO3D_VecLerp3(float *iv0, float *iv1, float *ov, float frac)
{
	ov[0]=TKGDI_TKO3D_Lerp(iv0[0], iv1[0], frac);
	ov[1]=TKGDI_TKO3D_Lerp(iv0[1], iv1[1], frac);
	ov[2]=TKGDI_TKO3D_Lerp(iv0[2], iv1[2], frac);
	return(0);
}

int TKGDI_TKO3D_VecLerp4(float *iv0, float *iv1, float *ov, float frac)
{
	ov[0]=TKGDI_TKO3D_Lerp(iv0[0], iv1[0], frac);
	ov[1]=TKGDI_TKO3D_Lerp(iv0[1], iv1[1], frac);
	ov[2]=TKGDI_TKO3D_Lerp(iv0[2], iv1[2], frac);
	ov[3]=TKGDI_TKO3D_Lerp(iv0[3], iv1[3], frac);
	return(0);
}

void TKGDI_TKO3D_VecCopy2(float *iv, float *ov)
	{ memcpy(ov, iv, 2*sizeof(float)); }
void TKGDI_TKO3D_VecCopy3(float *iv, float *ov)
	{ memcpy(ov, iv, 3*sizeof(float)); }
void TKGDI_TKO3D_VecCopy4(float *iv, float *ov)
	{ memcpy(ov, iv, 4*sizeof(float)); }

float TKGDI_TKO3D_VecDist3(float *iv0, float *iv1)
{
	float dx, dy, dz, d;
	
	dx=iv0[0]-iv1[0];
	dy=iv0[1]-iv1[1];
	dz=iv0[2]-iv1[2];
	d=sqrt(dx*dx+dy*dy+dz*dz);
	return(d);
}

u64 TKGDI_TKO3D_ReadVLN(byte **rcs)
{
	byte *cs;
	u64 v;
	int i;
	
	cs=*rcs;
	i=*cs++;
	if(!(i&0x80))
	{
		v=i;
		return(i);
	}else if(!(i&0x40))
	{
		v=((i&0x3F)<<8)|(*cs++);
	}else if(!(i&0x20))
	{
		v=((i&0x1F)<<8)|(*cs++);	v=(v<<8)|(*cs++);
	}else if(!(i&0x10))
	{
		v=((i&0x0F)<<8)|(*cs++);	v=(v<<8)|(*cs++);
		v=(v<<8)|(*cs++);
	}else if(!(i&0x08))
	{
		v=((i&0x07)<<8)|(*cs++);	v=(v<<8)|(*cs++);
		v=(v<<8)|(*cs++);			v=(v<<8)|(*cs++);
	}else if(!(i&0x04))
	{
		v=((i&0x03)<<8)|(*cs++);	v=(v<<8)|(*cs++);
		v=(v<<8)|(*cs++);			v=(v<<8)|(*cs++);
		v=(v<<8)|(*cs++);
	}else if(!(i&0x02))
	{
		v=((i&0x01)<<8)|(*cs++);	v=(v<<8)|(*cs++);
		v=(v<<8)|(*cs++);			v=(v<<8)|(*cs++);
		v=(v<<8)|(*cs++);			v=(v<<8)|(*cs++);
	}else if(!(i&0x01))
	{
		v=(*cs++);			v=(v<<8)|(*cs++);
		v=(v<<8)|(*cs++);	v=(v<<8)|(*cs++);
		v=(v<<8)|(*cs++);	v=(v<<8)|(*cs++);
		v=(v<<8)|(*cs++);
	}else
	{
		v=(*cs++);			v=(v<<8)|(*cs++);
		v=(v<<8)|(*cs++);	v=(v<<8)|(*cs++);
		v=(v<<8)|(*cs++);	v=(v<<8)|(*cs++);
		v=(v<<8)|(*cs++);	v=(v<<8)|(*cs++);
	}
	
	*rcs=cs;
	return(v);
}

s64 TKGDI_TKO3D_ReadSVLI(byte **rcs)
{
	u64 v0, v1;
	
	v0=TKGDI_TKO3D_ReadVLN(rcs);
	v1=(v0>>1)^(((s64)(v0<<63))>>63);
	return(v1);
}


s32 TKGDI_TKO3D_ReadSCoord(byte **rcs)
{
	u64 v0, v1;
	int i;
	
	v0=TKGDI_TKO3D_ReadVLN(rcs);
	v1=(v0>>3)^(((s64)(v0<<61))>>63);
	i=v0&3;
	if(i>=1)	v1=v1*10;
	if(i>=2)	v1=v1*10;
	if(i>=3)	v1=v1*10;
	return(v1);
}

TKGDI_Object3D *TKGDI_TKO3D_ReadObjectBinary(byte **rcs)
{
	char *mata[64];
	int *vtxa;
	int *polya;
	int n_vtxa, m_vtxa;
	int n_poly, m_poly;
	int parm[32];
	int n_mat;

	TKGDI_Object3D *obj;
	byte *cs, *ct;
	char *s0, *objnam;
	float sc;
	int cmd, ixvtx, is_ijk;
	int i, j, k;

	cs=*rcs;
	cmd=*cs;

	objnam=NULL;
	if(cmd=='N')
	{
		cs++;
		s0=(char *)cs;
		cs+=strlen(s0)+1;
		objnam=tk_strdup_in(s0);

		cmd=*cs;
	}
	

	if(cmd!=0x7B)
		return(NULL);
	
	vtxa=malloc(1024*16*sizeof(int));
	polya=malloc(256*4*sizeof(int));
	n_vtxa=0;
	n_poly=0;
	m_vtxa=1024;
	m_poly=256;
	n_mat=0;

	is_ijk=0;
	ixvtx=0;
	memset(parm, 0, 32*sizeof(int));
	parm['A'-'A']=10000;
	parm['B'-'A']=10000;
	parm['C'-'A']=10000;
	parm['D'-'A']=10000;
	
	while(1)
	{
		cmd=*cs++;
		
		if(cmd==0x40)
		{
			if((n_vtxa+1)>=m_vtxa)
			{
				i=m_vtxa+(m_vtxa>>1);
				vtxa=realloc(vtxa, i*16*sizeof(int));
				m_vtxa=i;
			}
		
			i=n_vtxa*16;
			vtxa[i+ 0]=parm['X'-'A'];	vtxa[i+ 1]=parm['Y'-'A'];
			vtxa[i+ 2]=parm['Z'-'A'];	vtxa[i+ 3]=parm['I'-'A'];
			vtxa[i+ 4]=parm['J'-'A'];	vtxa[i+ 5]=parm['K'-'A'];
			vtxa[i+ 6]=parm['S'-'A'];	vtxa[i+ 7]=parm['T'-'A'];
			vtxa[i+ 8]=parm['D'-'A'];	vtxa[i+ 9]=parm['C'-'A'];
			vtxa[i+10]=parm['B'-'A'];	vtxa[i+11]=parm['A'-'A'];
			vtxa[i+12]=parm['G'-'A'];
			n_vtxa++;
			
			if(parm['G'-'A']>=2)
				is_ijk=1;
			
			continue;
		}
		
		if(cmd=='M')
		{
			s0=(char *)cs;
			cs+=strlen(s0)+1;
			
			for(i=0; i<n_mat; i++)
			{
				if(!strcmp(s0, mata[i]))
					break;
			}
			
			if(i<n_mat)
			{
				parm['M'-'A']=i;
				continue;
			}
			
			i=n_mat++;
			mata[i]=tk_strdup_in(s0);
			parm['M'-'A']=i;
			
			continue;
		}

		if((cmd>='A') && (cmd<='Z'))
		{
			parm[cmd-'A']=TKGDI_TKO3D_ReadSCoord(&cs);
			continue;
		}
		
		if(cmd==0x5D)
		{
			/* ENDPOLY */
			if((n_poly+1)>=m_poly)
			{
				i=m_poly+(m_poly>>1);
				polya=realloc(polya, i*4*sizeof(int));
				m_poly=i;
			}
		
			i=n_poly*4;
			polya[i+0]=ixvtx;
			polya[i+1]=n_vtxa-ixvtx;
			polya[i+2]=parm['M'-'A'];
			ixvtx=n_vtxa;
			n_poly++;
			continue;
		}
		
		if(cmd==0x5B)
		{
			/* POLY */
			continue;
		}
		
		break;
	}
	
	obj=malloc(sizeof(TKGDI_Object3D));
	memset(obj, 0, sizeof(TKGDI_Object3D));

	obj->name=objnam;

	obj->vtxa_xyz=malloc(n_vtxa*3*sizeof(float));
	if(is_ijk)
		obj->vtxa_ijk=malloc(n_vtxa*4*sizeof(float));
	obj->vtxa_st=malloc(n_vtxa*2*sizeof(float));
	obj->vtxa_rgba=malloc(n_vtxa*4*sizeof(float));
	obj->vtxa_tag=malloc(n_vtxa*sizeof(byte));
	obj->n_vtxa=n_vtxa;

	obj->poly_vidx=malloc(n_poly*sizeof(int));
	obj->poly_vcnt=malloc(n_poly*sizeof(short));
	obj->poly_mat=malloc(n_poly*sizeof(byte));
	obj->n_poly=n_poly;
	
	sc=1.0/10000;
	for(i=0; i<n_vtxa; i++)
	{
		j=i*16;
		k=i*3;
		obj->vtxa_xyz[k+0]=vtxa[j+0]*sc;
		obj->vtxa_xyz[k+1]=vtxa[j+1]*sc;
		obj->vtxa_xyz[k+2]=vtxa[j+2]*sc;
		if(is_ijk)
		{
			k=i*4;
			obj->vtxa_ijk[k+0]=vtxa[j+3]*sc;
			obj->vtxa_ijk[k+1]=vtxa[j+4]*sc;
			obj->vtxa_ijk[k+2]=vtxa[j+5]*sc;
			obj->vtxa_ijk[k+3]=TKGDI_TKO3D_VecDist3(
				obj->vtxa_xyz+i*3, obj->vtxa_ijk+i*4);
		}
		k=i*2;
		obj->vtxa_st[k+0]=vtxa[j+6]*sc;
		obj->vtxa_st[k+1]=vtxa[j+7]*sc;
		k=i*4;
		obj->vtxa_rgba[k+0]=vtxa[j+ 8]*sc;
		obj->vtxa_rgba[k+1]=vtxa[j+ 9]*sc;
		obj->vtxa_rgba[k+2]=vtxa[j+10]*sc;
		obj->vtxa_rgba[k+3]=vtxa[j+11]*sc;
		obj->vtxa_tag[i]=vtxa[j+12];
	}

	for(i=0; i<n_poly; i++)
	{
		obj->poly_vidx[i]=polya[i*4+0];
		obj->poly_vcnt[i]=polya[i*4+1];
		obj->poly_mat[i]=polya[i*4+2];
	}
	
	obj->mata=malloc(n_mat*sizeof(char *));
	obj->n_mat=n_mat;
	for(i=0; i<n_mat; i++)
		obj->mata[i]=mata[i];
	
	free(vtxa);
	free(polya);
	
	*rcs=cs;
	
	return(obj);
}

TKGDI_Object3D *TKGDI_TKO3D_ReadObjectFileBinary(byte *buf, int szbuf)
{
	byte *cs, *cse;
	TKGDI_Object3D *obj, *lst;
	
	cs=buf;
	cse=buf+szbuf;
	if(*cs=='@')cs++;
	if(*cs=='@')cs++;
	
	lst=NULL;
	while((cs<cse) && (*cs))
	{
		obj=TKGDI_TKO3D_ReadObjectBinary(&cs);
		if(!obj)
			break;
		obj->next=lst;
		lst=obj;
	}
	return(lst);
}


s64 TKGDI_TKO3D_ParseCoordNumber(char *tok)
{
	char *s;
	int isdec;

	s=tok; isdec=0;
	while(*s && (*s!='.'))s++;
	if(*s=='.')
		isdec=1;
	
	if(isdec)
	{
		return(atof(tok)*10000);
	}else
	{
		return(tk_atoi(tok));
	}
}

TKGDI_Object3D *TKGDI_TKO3D_ReadObjectAscii(byte **rcs)
{
	char *mata[64];
	int *vtxa;
	int *polya;
	int n_vtxa, m_vtxa;
	int n_poly, m_poly;
	int parm[32];
	int n_mat;
	char tbuf[128], tb1[128];

	TKGDI_Object3D *obj;
	char *cs, *ct;
	char *s0, *objnam;
	char **a, **a1;
	float sc;
	int cmd, ixvtx, islvtx;
	int i, j, k;

	cs=(char *)(*rcs);
	
	while(1)
	{
		cs=tk_getstrline(tbuf, 128, cs);
		if(!*cs)
			break;
		a=tk_rsplit(tbuf);
		if(!a[0])
			continue;
		if(a[0][0]=='/')
			continue;
	}

	if(stricmp(a[0], "OBJECT"))
		return(NULL);
	for(i=1; a[i]; i++)
	{
		if(a[i][0]=='N')
		{
			a1=tk_rsplit(a[i]+1);
			objnam=tk_strdup_in(a1[0]);
		}
	}

	vtxa=malloc(1024*16*sizeof(int));
	polya=malloc(256*4*sizeof(int));
	n_vtxa=0;
	n_poly=0;
	m_vtxa=1024;
	m_poly=256;
	n_mat=0;

	ixvtx=0;
	memset(parm, 0, 32*sizeof(int));
	parm['A'-'A']=10000;
	parm['B'-'A']=10000;
	parm['C'-'A']=10000;
	parm['D'-'A']=10000;
	
	while(1)
	{
		cs=tk_getstrline(tbuf, 128, cs);
		if(!*cs)
			break;
		a=tk_rsplit(tbuf);
		if(!a[0])
			continue;
		if(a[0][0]=='/')
			continue;

		islvtx=0;
		for(i=0; a[i]; i++)
		{
			if(a[i][0]=='M')
			{
				a1=tk_rsplit(a[i]+1);
				s0=a1[0];

				for(i=0; i<n_mat; i++)
				{
					if(!strcmp(s0, mata[i]))
						break;
				}
				
				if(i<n_mat)
				{
					parm['M'-'A']=i;
					continue;
				}
				
				i=n_mat++;
				mata[i]=tk_strdup_in(s0);
				parm['M'-'A']=i;
				
				continue;
			}

			if((a[i][0]>='A') && (a[i][0]<='Z') &&
				(a[i][1]>='0') && (a[i][1]<='9'))
			{
				if((a[i][0]>='X') && (a[i][0]<='Z'))
					islvtx=1;
				parm[a[i][0]-'A']=TKGDI_TKO3D_ParseCoordNumber(a[i]+1);
				continue;
			}
		}
		
		if(islvtx)
		{
			if((n_vtxa+1)>=m_vtxa)
			{
				i=m_vtxa+(m_vtxa>>1);
				vtxa=realloc(vtxa, i*16*sizeof(int));
				m_vtxa=i;
			}
		
			i=n_vtxa*16;
			vtxa[i+ 0]=parm['X'-'A'];	vtxa[i+ 1]=parm['Y'-'A'];
			vtxa[i+ 2]=parm['Z'-'A'];	vtxa[i+ 3]=parm['I'-'A'];
			vtxa[i+ 4]=parm['J'-'A'];	vtxa[i+ 5]=parm['K'-'A'];
			vtxa[i+ 6]=parm['S'-'A'];	vtxa[i+ 7]=parm['T'-'A'];
			vtxa[i+ 8]=parm['D'-'A'];	vtxa[i+ 9]=parm['C'-'A'];
			vtxa[i+10]=parm['B'-'A'];	vtxa[i+11]=parm['A'-'A'];
			vtxa[i+12]=parm['G'-'A'];
			n_vtxa++;
			continue;
		}
		
		if(!stricmp(a[0], "ENDPOLY"))
		{
			/* ENDPOLY */
			if((n_poly+1)>=m_poly)
			{
				i=m_poly+(m_poly>>1);
				polya=realloc(polya, i*4*sizeof(int));
				m_poly=i;
			}
		
			i=n_poly*4;
			polya[i+0]=ixvtx;
			polya[i+1]=n_vtxa-ixvtx;
			polya[i+2]=parm['M'-'A'];
			ixvtx=n_vtxa;
			n_poly++;
			continue;
		}
		
		if(!stricmp(a[0], "POLY"))
		{
			/* POLY */
			continue;
		}
		
		break;
	}
	
	obj=malloc(sizeof(TKGDI_Object3D));
	memset(obj, 0, sizeof(TKGDI_Object3D));

	obj->name=objnam;

	obj->vtxa_xyz=malloc(n_vtxa*3*sizeof(float));
	obj->vtxa_ijk=malloc(n_vtxa*4*sizeof(float));
	obj->vtxa_st=malloc(n_vtxa*2*sizeof(float));
	obj->vtxa_rgba=malloc(n_vtxa*4*sizeof(float));
	obj->vtxa_tag=malloc(n_vtxa*sizeof(byte));
	obj->n_vtxa=n_vtxa;

	obj->poly_vidx=malloc(n_poly*sizeof(int));
	obj->poly_vcnt=malloc(n_poly*sizeof(short));
	obj->poly_mat=malloc(n_poly*sizeof(byte));
	obj->n_poly=n_poly;
	
	sc=1.0/10000;
	for(i=0; i<n_vtxa; i++)
	{
		j=i*16;
		k=i*3;
		obj->vtxa_xyz[k+0]=vtxa[j+0]*sc;
		obj->vtxa_xyz[k+1]=vtxa[j+1]*sc;
		obj->vtxa_xyz[k+2]=vtxa[j+2]*sc;

		k=i*4;
		obj->vtxa_ijk[k+0]=vtxa[j+3]*sc;
		obj->vtxa_ijk[k+1]=vtxa[j+4]*sc;
		obj->vtxa_ijk[k+2]=vtxa[j+5]*sc;

		k=i*2;
		obj->vtxa_st[k+0]=vtxa[j+6]*sc;
		obj->vtxa_st[k+1]=vtxa[j+7]*sc;
		k=i*4;
		obj->vtxa_rgba[k+0]=vtxa[j+ 8]*sc;
		obj->vtxa_rgba[k+1]=vtxa[j+ 9]*sc;
		obj->vtxa_rgba[k+2]=vtxa[j+10]*sc;
		obj->vtxa_rgba[k+3]=vtxa[j+11]*sc;
		obj->vtxa_tag[i]=vtxa[j+12];
	}

	for(i=0; i<n_poly; i++)
	{
		obj->poly_vidx[i]=polya[i*4+0];
		obj->poly_vcnt[i]=polya[i*4+1];
		obj->poly_mat[i]=polya[i*4+2];
	}
	
	obj->mata=malloc(n_mat*sizeof(char *));
	obj->n_mat=n_mat;
	for(i=0; i<n_mat; i++)
		obj->mata[i]=mata[i];
	
	free(vtxa);
	free(polya);
	
	*rcs=cs;
	
	return(obj);
}

TKGDI_Object3D *TKGDI_TKO3D_ReadObjectFileAscii(byte *buf, int szbuf)
{
	byte *cs, *cse;
	TKGDI_Object3D *obj, *lst;
	
	cs=buf;
	cse=buf+szbuf;
	
	lst=NULL;
	while((cs<cse) && (*cs))
	{
		obj=TKGDI_TKO3D_ReadObjectAscii(&cs);
		if(!obj)
			break;
		obj->next=lst;
		lst=obj;
	}
	return(lst);
}

TKGDI_Object3D *TKGDI_TKO3D_ReadObjectFile(byte *buf, int szbuf)
{
	if((buf[0]=='@') && (buf[1]=='@'))
	{
		return(TKGDI_TKO3D_ReadObjectFileBinary(buf, szbuf));
	}
	return(TKGDI_TKO3D_ReadObjectFileAscii(buf, szbuf));
}

void TKGDI_TKO3D_VertexCopy(
	float *ixyz, float *iijk, float *ist, float *irgb, byte *itag,
	float *oxyz, float *oijk, float *ost, float *orgb, byte *otag,
	int ix, int ox)
{
	TKGDI_TKO3D_VecCopy3(ixyz+ix*3, oxyz+ox*3);
	TKGDI_TKO3D_VecCopy4(iijk+ix*4, oijk+ox*4);
	TKGDI_TKO3D_VecCopy2(ist +ix*2, ost +ox*2);
	TKGDI_TKO3D_VecCopy4(irgb+ix*4, orgb+ox*4);
	otag[ox]=itag[ix];
}

void TKGDI_TKO3D_VertexFixupDistance(
	float *xyz, float *ijk, byte tag)
{
	float d0, d1, d2;
	
	if(tag<2)
		return;
	
	d0=TKGDI_TKO3D_VecDist3(xyz, ijk);
	d1=ijk[3];
	if(d0>0)
		d2=d1/d0;

	xyz[0]=ijk[0]+((xyz[0]-ijk[0])*d2);
	xyz[1]=ijk[1]+((xyz[1]-ijk[1])*d2);
	xyz[2]=ijk[2]+((xyz[2]-ijk[2])*d2);
}

int TKGDI_TKO3D_SubdivideCurvedSurfaceToTriangles(
	float *ixyz, float *iijk, float *ist, float *irgb, byte *itag,
	int n_vtx,
	float *oxyz, float *ost, float *orgb,
	int ndiv)
{
	float t_xyz[16*3], t_ijk[16*4], t_st[16*2], t_rgb[16*4];
	float s_xyz[4*3], s_ijk[4*4], s_st[4*2], s_rgb[4*4];
	byte t_tag[16], s_tag[4];

	int i, j, k;

	for(i=0; i<n_vtx; i++)
		if(itag[i]>=2)
			break;
	if((i>=n_vtx) || (ndiv<=0))
	{
		memcpy(oxyz, ixyz, n_vtx*3*sizeof(float));
		memcpy(ost , ist , n_vtx*2*sizeof(float));
		memcpy(orgb, irgb, n_vtx*4*sizeof(float));
		return(n_vtx);
	}
	
	if(n_vtx==3)
	{
		for(i=0; i<3; i++)
		{
			j=i+1;
			if(j>=3)j-=3;

			TKGDI_TKO3D_VecCopy3(ixyz+i*3, t_xyz+(i*2+0)*3);
			TKGDI_TKO3D_VecCopy4(iijk+i*4, t_ijk+(i*2+0)*4);
			TKGDI_TKO3D_VecCopy2(ist +i*2, t_st +(i*2+0)*2);
			TKGDI_TKO3D_VecCopy4(irgb+i*4, t_rgb+(i*2+0)*4);

			TKGDI_TKO3D_VecLerp3(ixyz+i*3, ixyz+j*3, t_xyz+(i*2+1)*3, 0.5);
			TKGDI_TKO3D_VecLerp4(iijk+i*4, iijk+j*4, t_ijk+(i*2+1)*4, 0.5);
			TKGDI_TKO3D_VecLerp2(ist +i*2, ist +j*2, t_st +(i*2+1)*2, 0.5);
			TKGDI_TKO3D_VecLerp4(irgb+i*4, irgb+j*4, t_rgb+(i*2+1)*4, 0.5);

			t_tag[i*2+0]=t_tag[i];
			t_tag[i*2+1]=t_tag[i];
			if(t_tag[j]>=2)
				t_tag[i*2+1]=t_tag[2];

			TKGDI_TKO3D_VertexFixupDistance(
				t_xyz+(i*2+1)*3, t_ijk+(i*2+1)*4, t_tag[i*2+1]);
		}

		//0 1 5
		//1 2 3
		//3 4 5
		//1 3 5

		i=0;
		
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 0, 0);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 1, 1);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 5, 2);
		i=TKGDI_TKO3D_SubdivideCurvedSurfaceToTriangles(
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 3,
			oxyz+i*3, ost+i*2, orgb+i*4, ndiv-1);
		
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 1, 0);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 2, 1);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 3, 2);
		i+=TKGDI_TKO3D_SubdivideCurvedSurfaceToTriangles(
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 3,
			oxyz+i*3, ost+i*2, orgb+i*4, ndiv-1);
		
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 3, 0);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 4, 1);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 5, 2);
		i+=TKGDI_TKO3D_SubdivideCurvedSurfaceToTriangles(
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 3,
			oxyz+i*3, ost+i*2, orgb+i*4, ndiv-1);
		
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 1, 0);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 3, 1);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 5, 2);
		i+=TKGDI_TKO3D_SubdivideCurvedSurfaceToTriangles(
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 3,
			oxyz+i*3, ost+i*2, orgb+i*4, ndiv-1);

		return(i);
	}

	
	if(n_vtx==4)
	{
		for(i=0; i<4; i++)
		{
			j=i+1;
			if(j>=4)j-=4;
			TKGDI_TKO3D_VecCopy3(ixyz+i*3, t_xyz+(i*2+0)*3);
			TKGDI_TKO3D_VecCopy4(iijk+i*4, t_ijk+(i*2+0)*4);
			TKGDI_TKO3D_VecCopy2(ist +i*2, t_st +(i*2+0)*2);
			TKGDI_TKO3D_VecCopy4(irgb+i*4, t_rgb+(i*2+0)*4);

			TKGDI_TKO3D_VecLerp3(ixyz+i*3, ixyz+j*3, t_xyz+(i*2+1)*3, 0.5);
			TKGDI_TKO3D_VecLerp4(iijk+i*4, iijk+j*4, t_ijk+(i*2+1)*4, 0.5);
			TKGDI_TKO3D_VecLerp2(ist +i*2, ist +j*2, t_st +(i*2+1)*2, 0.5);
			TKGDI_TKO3D_VecLerp4(irgb+i*4, irgb+j*4, t_rgb+(i*2+1)*4, 0.5);

			t_tag[i*2+0]=t_tag[i];
			t_tag[i*2+1]=t_tag[i];
			if(t_tag[j]>=2)
				t_tag[i*2+1]=t_tag[2];

			TKGDI_TKO3D_VertexFixupDistance(
				t_xyz+(i*2+1)*3, t_ijk+(i*2+1)*4, t_tag[i*2+1]);
		}

		//0 1 8 7
		//1 2 3 8
		//3 4 5 8
		//5 6 7 8

		i=0;
		
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 0, 0);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 1, 1);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 8, 2);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 7, 3);
		i+=TKGDI_TKO3D_SubdivideCurvedSurfaceToTriangles(
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 4,
			oxyz+i*3, ost+i*2, orgb+i*4, ndiv-1);
		
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 1, 0);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 2, 1);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 3, 2);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 8, 3);
		i+=TKGDI_TKO3D_SubdivideCurvedSurfaceToTriangles(
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 4,
			oxyz+i*3, ost+i*2, orgb+i*4, ndiv-1);
		
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 3, 0);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 4, 1);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 5, 2);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 8, 3);
		i+=TKGDI_TKO3D_SubdivideCurvedSurfaceToTriangles(
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 4,
			oxyz+i*3, ost+i*2, orgb+i*4, ndiv-1);
		
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 5, 0);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 6, 1);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 7, 2);
		TKGDI_TKO3D_VertexCopy(t_xyz, t_ijk, t_st, t_rgb, t_tag,
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 8, 3);
		i+=TKGDI_TKO3D_SubdivideCurvedSurfaceToTriangles(
			s_xyz, s_ijk, s_st, s_rgb, s_tag, 4,
			oxyz+i*3, ost+i*2, orgb+i*4, ndiv-1);

		return(i);
	}
	
	return(0);
}
