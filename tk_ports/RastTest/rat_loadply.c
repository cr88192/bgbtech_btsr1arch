byte *bgbcc_rlcbuf=NULL;
int bgbcc_rlcpos;

void *bgbcc_ralloc(int sz)
{
	void *p;

	if(!bgbcc_rlcbuf)
	{
		bgbcc_rlcbuf=(byte *)malloc(1<<18);
		bgbcc_rlcpos=0;
	}

	if((bgbcc_rlcpos+sz)>=((1<<18)-4096))
		bgbcc_rlcpos=0;
	p=(void *)(bgbcc_rlcbuf+bgbcc_rlcpos);
	bgbcc_rlcpos=(bgbcc_rlcpos+sz+15)&(~15);

	return(p);
}

char *bgbcc_rstrdup(char *s)
{
	char *t;
	t=(char *)bgbcc_ralloc(strlen(s)+1);
	strcpy(t, s);
	return(t);
}

char **bgbcc_split(char *s)
{
	char **a, *t;
	int i;

	a=bgbcc_ralloc(64*sizeof(char *));
	i=0;
	while(*s)
	{
		while(*s && (*s<=' '))s++;
		if(!*s)break;
		if(*s=='#')break;
		if(*s==';')break;
		if((s[0]=='/') && (s[1]=='/'))
			break;

		t=bgbcc_ralloc(256);
		a[i++]=t;

		if(*s=='"')
		{
			s++;
			while(*s && (*s!='"'))*t++=*s++;
			if(*s=='"')s++;

			*t++=0;
			continue;
		}

		while(*s && (*s>' '))*t++=*s++;
		*t++=0;
	}
	a[i]=NULL;

	return(a);
}


int TKRA_LoadPly(char *fname, float **rtris, int *rntris)
{
	char tb[256];
	char **a;
	int nvtx, nface;
	FILE *fd;
	float *vtx;
	float *tris;
	float vx, vy, vz, vs, vt;
	int i, j, k;
	
	*rtris=NULL;
	*rntris=0;
	
	nvtx=0;
	nface=0;
	fd=fopen(fname, "rt");
	if(!fd)
	{
		return(-1);
	}
	
	while(!feof(fd))
	{
		fgets(tb, 255, fd);
		a=bgbcc_split(tb);
		if(!a[0])
			continue;
			
		if(!strcmp(a[0], "end_header"))
			break;

		if(!strcmp(a[0], "element"))
		{
			if(!strcmp(a[1], "vertex"))
			{
				nvtx=atoi(a[2]);
			}

			if(!strcmp(a[1], "face"))
			{
				nface=atoi(a[2]);
			}
			continue;
		}
	}
	
	vtx=malloc((nvtx+3)*3*sizeof(float));
	memset(vtx, 0, (nvtx+3)*3*sizeof(float));
	for(i=0; i<nvtx; i++)
	{
		fgets(tb, 255, fd);
		a=bgbcc_split(tb);
		vtx[i*3+0]=atof(a[0]);
		vtx[i*3+1]=atof(a[1]);
		vtx[i*3+2]=atof(a[2]);
	}

	tris=malloc((nface+10)*3*6*sizeof(float));
	memset(tris, 0, (nface+10)*3*6*sizeof(float));

	for(i=0; i<nface; i++)
	{
		fgets(tb, 255, fd);
		a=bgbcc_split(tb);
		
		for(j=0; j<3; j++)
		{
			k=atoi(a[j+1]);
//			vx=vtx[k*3+0];
//			vy=vtx[k*3+1];
//			vz=vtx[k*3+2];

			vx=vtx[k*3+0]*200;
			vy=vtx[k*3+1]*200;
			vz=vtx[k*3+2]*200;

			vs=atan2(vy, vx);
			vt=atan2(vz, sqrt(vx*vx+vy*vy));
			tris[(i*3+j)*6+0]=vx;
			tris[(i*3+j)*6+1]=vy;
			tris[(i*3+j)*6+2]=vz;
			tris[(i*3+j)*6+3]=vs;
			tris[(i*3+j)*6+4]=vt;
			
			k=128+vt*127;
			if(k<0)k=0;
			if(k>255)k=255;
			((u32 *)tris)[(i*3+j)*6+5]=0xFF000000+(k<<16)+(k<<8)+k;
		}
	}
	
	fclose(fd);
	free(vtx);
	
	*rtris=tris;
	*rntris=nface;
	return(1);
}


double stl_atod(char *str)
{
	double f, g, v, sc;
	char *s;
	int sg, e, esg;
	
	s=str; sg=0;
	if(*s=='+') { sg=0; s++; }
	if(*s=='-') { sg=1; s++; }
	
	v=0;
	while((*s>='0') && (*s<='9'))
		{ v=(v*10)+((*s++)-'0'); }
	
	if(*s=='.')
	{
		s++; sc=0.1;
		while((*s>='0') && (*s<='9'))
			{ v=v+(((*s++)-'0')*sc); sc=sc*0.1; }
	}
	
	if((*s=='e') || (*s=='E'))
	{
		s++;
		e=0;	esg=0;
		if(*s=='+') { esg=0; s++; }
		if(*s=='-') { esg=1; s++; }
		while((*s>='0') && (*s<='9'))
			{ e=(e*10)+((*s++)-'0'); }
		if(esg)e=-e;
		v=v*pow(10, e);
	}
	
	if(sg)v=-v;
	return(v);
}

int EmitTriangle(float *tris, int *rntris,
	float *v0, float *v1, float *v2)
{
	int ntris;
	float vx, vy, vz, vs, vt;
	u32 px;
	int k;
	
	ntris=*rntris;

	vx=v0[0]; vy=v0[1]; vz=v0[2];
	vs=atan2(vy, vx);
	vt=atan2(vz, sqrt(vx*vx+vy*vy));
	k=128+vt*127;
	if(k<0)k=0;
	if(k>255)k=255;
	px=0xFF000000+(k<<16)+(k<<8)+k;
	tris[ntris*(3*6)+(0*6)+0]=vx;
	tris[ntris*(3*6)+(0*6)+1]=vy;
	tris[ntris*(3*6)+(0*6)+2]=vz;
	tris[ntris*(3*6)+(0*6)+3]=vs;
	tris[ntris*(3*6)+(0*6)+4]=vt;
	((u32 *)tris)[ntris*(3*6)+(0*6)+5]=px;

	vx=v1[0]; vy=v1[1]; vz=v1[2];
	vs=atan2(vy, vx);
	vt=atan2(vz, sqrt(vx*vx+vy*vy));
	k=128+vt*127;
	if(k<0)k=0;
	if(k>255)k=255;
	px=0xFF000000+(k<<16)+(k<<8)+k;
	tris[ntris*(3*6)+(1*6)+0]=vx;
	tris[ntris*(3*6)+(1*6)+1]=vy;
	tris[ntris*(3*6)+(1*6)+2]=vz;
	tris[ntris*(3*6)+(1*6)+3]=vs;
	tris[ntris*(3*6)+(1*6)+4]=vt;
	((u32 *)tris)[ntris*(3*6)+(1*6)+5]=px;

	vx=v2[0]; vy=v2[1]; vz=v2[2];
	vs=atan2(vy, vx);
	vt=atan2(vz, sqrt(vx*vx+vy*vy));
	k=128+vt*127;
	if(k<0)k=0;
	if(k>255)k=255;
	px=0xFF000000+(k<<16)+(k<<8)+k;
	tris[ntris*(3*6)+(2*6)+0]=vx;
	tris[ntris*(3*6)+(2*6)+1]=vy;
	tris[ntris*(3*6)+(2*6)+2]=vz;
	tris[ntris*(3*6)+(2*6)+3]=vs;
	tris[ntris*(3*6)+(2*6)+4]=vt;
	((u32 *)tris)[ntris*(3*6)+(2*6)+5]=px;

	*rntris=ntris+1;
	return(0);
}

int EmitPolygon(float *tris, int *rntris,
	float *vec, int npts)
{
	float *v0, *v1, *v2;
	int i, j, k;

	v0=vec+0;
	for(i=1; i<(npts-1); i++)
	{
		v1=vec+(i+0)*3;
		v2=vec+(i+1)*3;
		EmitTriangle(tris, rntris, v0, v1, v2);
//		EmitTriangle(tris, rntris, v2, v1, v0);
	}
	return(0);
}


int LoadTrisStlAscii(char *fname, float **rtris, int *rntris)
{
	float pts[16*3];
	char tb[256];
	FILE *fd;
	char **a;
	double x, y, z;
	double xo, yo, zo;
	int ntris, npts, mtris;
	int i, j, k;
	float *tris;

	mtris=8192;
	tris=malloc(mtris*3*6*4);

//	printf("LoadTrisStl %s\n", fname);
	printf("LoadTrisStl:Ascii %s\n", fname);
	
	fd=fopen(fname, "rt");
	if(!fd)
	{
		printf("Can't open input STL %s\n", fname);
		if(rntris)
			{ *rntris=0; }
		return(-1);
	}

	ntris=0;
	while(!feof(fd))
	{
		fgets(tb, 256, fd);
		a=bgbcc_split(tb);
		if(!strcmp(a[0], "facet"))
		{
			npts=0;
			continue;
		}

		if(!strcmp(a[0], "vertex"))
		{
			pts[npts*3+0]=stl_atod(a[1]);
			pts[npts*3+1]=stl_atod(a[2]);
			pts[npts*3+2]=stl_atod(a[3]);
			npts++;
			continue;
		}

		if(!strcmp(a[0], "endfacet"))
		{
			if((ntris+npts)>=mtris)
			{
				while((ntris+npts)>mtris)
					mtris=mtris+(mtris>>1);
				tris=realloc(tris, mtris*3*6*4);
			}

			EmitPolygon(tris, &ntris, pts, npts);
			npts=0;
			continue;
		}
	}

	fclose(fd);
	
	if(rtris)
		{ *rtris=tris; }
	if(rntris)
		{ *rntris=ntris; }
	return(ntris);
}

int LoadTrisStl(char *fname, float **rtris, int *rntris)
{
	float pts[16*3];
	unsigned char tb[128];
	FILE *fd;
	byte *cs;
	float *tris;
	double x, y, z;
	double xo, yo, zo;
	int ntris, npts, ntri2;
	int i, j, k;
	
//	printf("LoadTrisStl %s\n", fname);
	
	fd=fopen(fname, "rb");
	if(!fd)
	{
		printf("Can't open input STL %s\n", fname);
		if(rntris)
			{ *rntris=0; }
		return(-1);
	}

	fread(tb, 1, 84, fd);

	if(!memcmp(tb, "solid ", 6))
	{
		fclose(fd);
		return(LoadTrisStlAscii(fname, rtris, rntris));
	}

	printf("LoadTrisStl:Binary %s\n", fname);
	
	ntris=tb[80]|(tb[81]<<8)|(tb[82]<<16)|(tb[83]<<24);
	tris=malloc(ntris*3*6*4);
	ntri2=0;

	for(i=0; i<ntris; i++)
	{
		fread(tb, 1, 50, fd);
		j=tb[48]|(tb[49]<<8);
		
		//0-32767: atribute data size
		//32768-65535: RGB555 color
		if((j>0) && (j<32768))
			{ fseek(fd, j, 1); }

		for(j=0; j<9; j++)
		{
			cs=tb+12+(j*4);
			k=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24);
//			pts[j]=*(float *)(&k);
			pts[j]=*(float *)(&k)*2;
		}
		EmitPolygon(tris, &ntri2, pts, 3);
	}

	fclose(fd);
	
	if(rtris)
		{ *rtris=tris; }
	if(rntris)
		{ *rntris=ntris; }
	return(ntris);
}
