/*
Aim to support VFS capabilities for WAD2A and WAD4.
 */

TK_WadZBlock	*tk_wad4_zlive;
TK_WadZBlock	*tk_wad4_zfree;
int				tk_wad4_zmemuse;
int				tk_wad4_zmemlim = 4*(1<<20);

TK_WadZBlock *TK_Wad4_ZAllocBlock()
{
	TK_WadZBlock *tmp;
	
	tmp=tk_wad4_zfree;
	if(tmp)
	{
		tk_wad4_zfree=tmp->next;
		return(tmp);
	}
	
	tmp=tk_malloc(sizeof(TK_WadZBlock));
	return(tmp);
}

void TK_Wad4_ZFreeBlock(TK_WadZBlock *blk)
{
	TK_WadZBlock *cur, *prv;

	if(!blk)
		return;

	cur=tk_wad4_zlive; prv=NULL;
	while(cur && (cur!=blk))
		{ prv=cur; cur=cur->next; }
	if(!prv)
		{ tk_wad4_zlive=cur->next; }
	else if(cur)
		{ prv->next=cur->next; }

	if(blk->user)
		{ *(blk->user)=NULL; }
	if(blk->data)
	{
		tk_wad4_zmemuse-=blk->size;
		tk_free(blk->data);
	}
	blk->next=tk_wad4_zfree;
	tk_wad4_zfree=blk;
}

void *TK_Wad4_ZMalloc(int sz, int tag, void **user)
{
	TK_WadZBlock *blk;
	void *ptr, *ptr1;

	blk=TK_Wad4_ZAllocBlock();
	ptr=tk_malloc(sz+sizeof(void *));
	blk->data=ptr;
	blk->size=sz;
	blk->user=user;
	blk->tag=tag;
	
	blk->next=tk_wad4_zlive;
	tk_wad4_zlive=blk;
	tk_wad4_zmemuse+=sz;
	
	*(void **)ptr=blk;
	ptr1=(void *)(((void **)ptr)+1);
	return(ptr1);
}

void TK_Wad4_ZFree(void *ptr)
{
	TK_WadZBlock *blk;
	blk=*(((void **)ptr)-1);
	TK_Wad4_ZFreeBlock(blk);
}

void TK_Wad4_ZEvictCache()
{
	TK_WadZBlock *blk, *nxt;
	
	blk=tk_wad4_zlive;
	while(blk)
	{
		nxt=blk->next;
		if(blk->tag>=TK_W4PU_CACHE)
		{
			TK_Wad4_ZFreeBlock(blk);
		}
		blk=nxt;
	}
}

void TK_Wad4_ZCheckEvictCache()
{
	if(tk_wad4_zmemuse<tk_wad4_zmemlim)
		return;
	TK_Wad4_ZEvictCache();
}

TK_WadImage *TK_Wad4_AllocImage()
{
	TK_WadImage *img;
	img=tk_malloc(sizeof(TK_WadImage));
	memset(img, 0, sizeof(TK_WadImage));
	return(img);
}

void TK_Wad4_FreeImage(TK_WadImage *img)
{
	tk_free(img);
}

int tk_log2u(int val)
{
	int v, k;
	v=val; k=0;
	while(k>1)
		{ v=(v+1)>>1; k++; }
	return(k);
}

TK_WadImage *TK_Wad4_OpenImage(TK_FILE *fd)
{
	TK_WadImage *img;
	TK_Wad4Info *inf;
	int wadver, hbits, hsz;

	img=TK_Wad4_AllocImage();
	img->img_fd=fd;
	
	inf=tk_malloc(sizeof(TK_Wad4Info));
	img->w2inf=(TK_Wad2Lump *)inf;
	img->w4inf=(TK_Wad4Lump *)inf;
	
	tk_fseek(fd, 0, 0);
	tk_fread(img->w4inf, 1, sizeof(TK_Wad4Info), fd);

	if(	(inf->ident[0]!='W') ||
		(inf->ident[1]!='A') ||
		(inf->ident[2]!='D'))
	{
		tk_free(inf);
		TK_Wad4_FreeImage(img);
		return(NULL);
	}

	wadver=0;
	if(inf->ident[3]=='2')
		wadver=2;
	if(inf->ident[3]=='4')
		wadver=4;

	img->wadver=wadver;

	if(!wadver)
	{
		tk_free(inf);
		TK_Wad4_FreeImage(img);
		return(NULL);
	}

	if(wadver==2)
	{
		img->w2dir=tk_malloc(inf->numlumps*sizeof(TK_Wad2Lump));
		tk_fseek(fd, inf->diroffs, 0);
		tk_fread(img->w2dir, 1, inf->numlumps*sizeof(TK_Wad2Lump), fd);

		img->hashsz=64;
		img->hash2=tk_malloc(hsz*2);
		tk_fread(img->hash2, 1, hsz*2, fd);
	}

	if(wadver==4)
	{
		img->w4dir=tk_malloc(inf->numlumps*sizeof(TK_Wad4Lump));
		tk_fseek(fd, inf->diroffs*64, 0);
		tk_fread(img->w4dir, 1, inf->numlumps*sizeof(TK_Wad4Lump), fd);
		
		hbits=tk_log2u(inf->numlumps)-4;
		if(hbits<6)hbits=6;
		if(hbits>16)hbits=16;
		hsz=1<<hbits;
		img->hashsz=hsz;
		
		if(inf->numlumps>=65536)
		{
			img->hash4=tk_malloc(hsz*4);
			tk_fseek(fd, inf->hashoffs*64, 0);
			tk_fread(img->hash4, 1, hsz*4, fd);
		}else
		{
			img->hash2=tk_malloc(hsz*2);
			tk_fseek(fd, inf->hashoffs*64, 0);
			tk_fread(img->hash2, 1, hsz*2, fd);
		}
	}
	
	img->lca_data=tk_malloc(inf->numlumps*sizeof(void *));
//	img->lca_sz=tk_malloc(inf->numlumps*sizeof(int));

	memset(img->lca_data, 0, inf->numlumps*sizeof(void *));

	return(img);
}

TK_WadImage *TK_Wad4_CreateTempRamImage(int dirsz)
{
	TK_WadImage *img;
	TK_Wad4Info *inf;
	int wadver, hbits, hsz;

	img=TK_Wad4_AllocImage();
//	img->img_fd=fd;
	
	inf=tk_malloc(sizeof(TK_Wad4Info));
	img->w4inf=(TK_Wad4Lump *)inf;
	img->wadver=4;
	
	inf->numlumps=dirsz;

	if(wadver==4)
	{
		img->w4dir=tk_malloc(inf->numlumps*sizeof(TK_Wad4Lump));
		memset(img->w4dir, 0, inf->numlumps*sizeof(TK_Wad4Lump));

//		tk_fseek(fd, inf->diroffs*64, 0);
//		tk_fread(img->w4dir, 1, inf->numlumps*sizeof(TK_Wad4Lump), fd);
		
		hbits=tk_log2u(inf->numlumps)-4;
		if(hbits<6)hbits=6;
		if(hbits>16)hbits=16;
		hsz=1<<hbits;
		img->hashsz=hsz;
		
		if(inf->numlumps>=65536)
		{
			img->hash4=tk_malloc(hsz*4);
			memset(img->hash4, 1, hsz*4);

//			tk_fseek(fd, inf->hashoffs*64, 0);
//			tk_fread(img->hash4, 1, hsz*4, fd);
		}else
		{
			img->hash2=tk_malloc(hsz*2);
			memset(img->hash2, 1, hsz*2);

//			tk_fseek(fd, inf->hashoffs*64, 0);
//			tk_fread(img->hash2, 1, hsz*2, fd);
		}
	}
	
	img->lca_data=tk_malloc(inf->numlumps*sizeof(void *));
//	img->lca_sz=tk_malloc(inf->numlumps*sizeof(int));

	memset(img->lca_data, 0, inf->numlumps*sizeof(void *));

	return(img);
}

int TK_Wad4_GetLumpSize(TK_WadImage *img, int lump)
{
	if(img->w4dir)
	{
		return(img->w4dir[lump].dsize);
	}

	if(img->w2dir)
	{
		return(img->w2dir[lump].dsize);
	}
	
	return(-1);
}

int TK_Wad4_SetLumpSize(TK_WadImage *img, int lump, int newsz)
{
	if(img->w4dir)
	{
		img->w4dir[lump].dsize=newsz;
		return(newsz);
	}

	if(img->w2dir)
	{
		img->w2dir[lump].dsize=newsz;
		return(newsz);
	}
	
	return(-1);
}

int TK_Wad4_GetLumpEntType(TK_WadImage *img, int lump)
{
	if(img->w4dir)
	{
		return(img->w4dir[lump].ety);
	}

	if(img->w2dir)
	{
		return(img->w2dir[lump].ety);
	}
	
	return(-1);
}

int TK_Wad4_SetLumpEntType(TK_WadImage *img, int lump, int newety)
{
	if(img->w4dir)
	{
		img->w4dir[lump].ety=newety;
		return(newety);
	}

	if(img->w2dir)
	{
		img->w2dir[lump].ety=newety;
		return(newety);
	}
	
	return(-1);
}

int TK_Wad4_GetLumpEntCmp(TK_WadImage *img, int lump)
{
	if(img->w4dir)
	{
		return(img->w4dir[lump].cmp);
	}

	if(img->w2dir)
	{
		return(img->w2dir[lump].cmp);
	}
	
	return(-1);
}

int TK_Wad4_GetLumpDirChild(TK_WadImage *img, int lump)
{
	if(img->w4dir)
	{
		return(img->w4dir[lump].offs);
	}

	if(img->w2dir)
	{
//		return(img->w2dir[lump].cmp);
	}
	
	return(-1);
}

int TK_Wad4_GetLumpDirNext(TK_WadImage *img, int lump)
{
	TK_Wad4Lump *lmp;
	int id;

	if(img->w4dir)
	{
		lmp=img->w4dir+lump;
		id=(lmp->dirnext)|(((lmp->hibits)&0x00F0)<<12);
		return(id);
	}

	if(img->w2dir)
	{
//		return(img->w2dir[lump].cmp);
	}
	
	return(-1);
}

int TK_Wad4_GetLumpDirName(TK_WadImage *img, int lump, char *name)
{
	byte *cs, *cse, *ct, *ct0;
	int i, j, k;
	
	if(img->w4dir)
	{
		cs=img->w4dir[lump].name;
		cse=cs+32;
		ct=(byte *)name;
		ct0=ct;
		
		while(cs<cse)
		{
			i=*cs++;
			if(!i)
				break;
			j=tkfat_asc2ucs(i);
			ct=tkfat_emitUtf8(ct, j);
		}
		*ct++=0;
		return(ct-ct0);
		
//		return(img->w4dir[lump].dirnext);
	}

	if(img->w2dir)
	{
//		return(img->w2dir[lump].cmp);
	}
	
	return(-1);
}

int TK_Wad4_GetLumpDirMode(TK_WadImage *img, int lump)
{
	if(img->w4dir)
	{
		return(img->w4dir[lump].mode);
	}

	return(-1);
}

int TK_Wad4_GetLumpDirUid(TK_WadImage *img, int lump)
{
	if(img->w4dir)
	{
		return(img->w4dir[lump].uid);
	}

	return(-1);
}

int TK_Wad4_GetLumpDirGid(TK_WadImage *img, int lump)
{
	if(img->w4dir)
	{
		return(img->w4dir[lump].gid);
	}

	return(-1);
}

s64 TK_Wad4_GetLumpDirMtime(TK_WadImage *img, int lump)
{
	TK_Wad4Lump *lmp;
	s64 tv;

	if(img->w4dir)
	{
		lmp=img->w4dir+lump;
		tv=(u64)(lmp->mtime)|(((lmp->hibits)&0xF000ULL)<<20);
		tv=(tv<<44);
		tv=(tv>>44);
		return(tv);
	}

	return(-1);
}

int TK_Wad4_GetLumpDirLink(TK_WadImage *img, int lump, char *name)
{
	byte *cs, *cse, *ct, *ct0, *ptr;
	int sz;
	int i, j, k;
	
	if(img->w4dir)
	{
		ptr=TK_Wad4_GetCacheLumpNum(img, lump, &sz);

		cs=ptr;
		cse=cs+sz;
		ct=(byte *)name;
		ct0=ct;
		
		while(cs<cse)
		{
			i=*cs++;
			if(!i)
				break;
			j=tkfat_asc2ucs(i);
			ct=tkfat_emitUtf8(ct, j);
		}
		*ct++=0;
		return(ct-ct0);
		
//		return(img->w4dir[lump].dirnext);
	}

	if(img->w2dir)
	{
//		return(img->w2dir[lump].cmp);
	}
	
	return(-1);
}

byte *tk_wad4_tcbuf=NULL;
int tk_wad4_tcsz;

void TK_Wad4_ReadLumpBuffer(TK_WadImage *img, int lump, void *buf)
{
	TK_Wad4Lump	*w4l;
	TK_Wad2Lump	*w2l;
	TK_FILE		*fd;
	byte		*tptr;
	s64 offs;
	int csz, lmp1, ofs1, csz1;
	int dsz;
	int cmp;

	if(img->w4dir)
	{
		w4l=img->w4dir+lump;
		offs=(w4l->offs)<<6;
		csz=w4l->csize;
		dsz=w4l->dsize;
		cmp=w4l->cmp;
	}else if(img->w2dir)
	{
		w2l=img->w2dir+lump;
		offs=w2l->offs;
		csz=w2l->csize;
		dsz=w2l->dsize;
		cmp=w2l->cmp;
	}

	fd=img->img_fd;

	if(cmp==TK_W4CMP_NONE)
	{
		tk_fseek(fd, offs, 0);
		tk_fread(buf, 1, dsz, fd);
		return;
	}

	if(cmp==TK_W4CMP_PACKED)
	{
		lmp1=(offs>>16)&65535;
		ofs1=(offs&65535)<<6;
		tptr=TK_Wad4_GetCacheLumpNum(img, lmp1, &csz1);
		memcpy(buf, tptr+ofs1, dsz);
		return;
	}
	
	if(cmp!=TK_W4CMP_FRAG)
	{
		/* Base lump of fragmented file is not read this way. */
		return;
	}

	if(!tk_wad4_tcbuf)
	{
		csz1=65536;
		while(csz>csz1)
			csz1=csz1+(csz1>>1);
		tk_wad4_tcbuf=tk_malloc(csz1);
		tk_wad4_tcsz=csz1;
	}

	if(csz>tk_wad4_tcsz)
	{
		csz1=tk_wad4_tcsz;
		while(csz>csz1)
			csz1=csz1+(csz1>>1);
		tk_wad4_tcbuf=tk_realloc(tk_wad4_tcbuf, csz1);
		tk_wad4_tcsz=csz1;
	}
	
	if((cmp==TK_W4CMP_RP2) || (cmp==TK_W4CMP_LZ4))
	{
		tk_fseek(fd, offs, 0);
		tk_fread(tk_wad4_tcbuf, 1, csz, fd);

		TKPE_UnpackBuffer(buf, tk_wad4_tcbuf, csz, cmp);
		return;
	}
	
	return;
}

/** Convert size into a 5.1 minifloat, Round Up */
int TK_Wad4_SizeToFxiU(int sz)
{
	int i;
	i=TKMM_SizeToFxiU(sz);	/* 5.3 Minifloat */
	i=(i+3)>>2;
	return(i);
}

int TK_Wad4_FxiToSize(int sz)
{
	return(TKMM_FxiToSize(sz<<2));
}


void *TK_Wad4_GetCacheLumpNum(TK_WadImage *img, int lump, int *rsz)
{
	void *ptr;
	int sz, fxi, sz1, cmp;

	sz=TK_Wad4_GetLumpSize(img, lump);
	if(sz<=0)
	{
		if(rsz)
			*rsz=0;
		return(NULL);
	}

	ptr=img->lca_data[lump];
	if(ptr)
	{
		if(rsz)
			*rsz=sz;
		return(ptr);
	}

	cmp=TK_Wad4_GetLumpEntCmp(img, lump);
	if(cmp==TK_W4CMP_FRAG)
	{
		/* Not the right interface for fragmented lumps. */
		return(NULL);
	}

	TK_Wad4_ZCheckEvictCache();

	/* Further quantize size to reduce fragmentation. */
	fxi=TK_Wad4_SizeToFxiU(sz);
	sz1=TK_Wad4_FxiToSize(fxi);
	if(sz1<256)sz1=256;

	ptr=TK_Wad4_ZMalloc(sz1, TK_W4PU_CACHE, img->lca_data+lump);
	img->lca_data[lump]=ptr;
	TK_Wad4_ReadLumpBuffer(img, lump, ptr);
	
	if(rsz)
		*rsz=sz;
	return(ptr);
}

void *TK_Wad4_GetCacheLumpNumOffs(TK_WadImage *img,
	int lump, int ofs, int *rrofs, int *rsz)
{
	int		fraga[64];
	TK_Wad4Lump	*w4l;
	TK_Wad2Lump	*w2l;
	TK_FILE		*fd;
	byte		*tptr;
	s64 offs;
	int csz, lmp1, offs1, csz1;
	int dsz, sz1;
	void *ptr;
	int cmp;
	int i, j, k, n;
	
	cmp=TK_Wad4_GetLumpEntCmp(img, lump);
	if(cmp<0)
		return(NULL);
	
	if(cmp!=TK_W4CMP_FRAG)
	{
		*rrofs=0;
		ptr=TK_Wad4_GetCacheLumpNum(img, lump, rsz);
		return(ptr);
	}

	if(img->w4dir)
	{
		w4l=img->w4dir+lump;
		offs=(w4l->offs)<<6;
		csz=w4l->csize;
		dsz=w4l->dsize;
		cmp=w4l->cmp;
	}else if(img->w2dir)
	{
		w2l=img->w2dir+lump;
		offs=w2l->offs;
		csz=w2l->csize;
		dsz=w2l->dsize;
		cmp=w2l->cmp;
	}
	
	if(ofs>=dsz)
		return(NULL);

	fd=img->img_fd;
	tk_fseek(fd, offs, 0);
	tk_fread(fraga, 1, csz, fd);
	
	n=csz>>2;
	i=0; offs1=ofs;
	while(i<n)
	{
		lmp1=fraga[i];
		sz1=TK_Wad4_GetLumpSize(img, lmp1);
		if(offs1<sz1)
			break;
		offs1-=sz1;
		i++;
	}
	
	if(i>=n)
		return(NULL);

	*rrofs=offs1;
	ptr=TK_Wad4_GetCacheLumpNum(img, lmp1, rsz);
	return(ptr);
}

void *TK_Wad4_GetCacheExpandLumpNum(TK_WadImage *img, int lump, int newsz)
{
	void *ptr, *ptr1;
	int sz, fxi, sz1, cmp;

	sz=TK_Wad4_GetLumpSize(img, lump);
	if(sz>=newsz)
	{
		ptr=TK_Wad4_GetCacheLumpNum(img, lump, &sz);
		return(ptr);
	}
	
	ptr=img->lca_data[lump];

	fxi=TK_Wad4_SizeToFxiU(newsz);
	sz1=TK_Wad4_FxiToSize(fxi);
	if(sz1<256)sz1=256;

	ptr1=TK_Wad4_ZMalloc(sz1, TK_W4PU_CACHE, img->lca_data+lump);
	img->lca_data[lump]=ptr1;
	TK_Wad4_SetLumpSize(img, lump, newsz);

	if(ptr)
	{
		memcpy(ptr1, ptr, sz);
		TK_Wad4_ZFree(ptr);
	}
	
	return(ptr1);

//	if(sz<=0)
//	{
//		if(rsz)
//			*rsz=0;
//		return(NULL);
//	}
}

int TK_Wad4_HashIndexForName16(char *s)
{
	int j, h;
	j = (((int *)(s))[0])+(((int *)(s))[1])+
		(((int *)(s))[2])+(((int *)(s))[3]);
	h = ((j*0xF14A83)>>24)&63;
	return(h);
}

int TK_Wad4_HashIndexForName32(char *s, int pfx)
{
	u64 v0, v1, v2, v3, v;
	v0=((u64 *)s)[0];
	v1=((u64 *)s)[1];
	v2=((u64 *)s)[2];
	v3=((u64 *)s)[3];
	v=v0+v1+v2+v3+pfx;
	v=((u32)v)+(v>>32);
//	v=((u32)v)+(v>>32);
	v=(u32)v;
	v=v*0xE20B7AC6ULL;
//	v=(v>>48)&65535;
	v=(v>>32)&65535;
	return(v);
}

int TK_Wad4_LookupLumpNameW4(TK_WadImage *img, char *name, int pfx)
{
	char tn[35];
	TK_Wad4Lump *lmp;
	u64 *pli;
	u64 v0, v1, v2, v3;
	u64 u0, u1, u2, u3;
	int h, i, di;

	memset(tn, 0, 33);
	strncpy(tn, name, 32);
	
	h=TK_Wad4_HashIndexForName32(tn, pfx);
	h=h&(img->hashsz-1);
	u0=((u64 *)tn)[0];	u1=((u64 *)tn)[1];
	u2=((u64 *)tn)[2];	u3=((u64 *)tn)[3];
	
	i=img->hash2[h];
	while(i>0)
	{
		lmp=img->w4dir+i;
		pli=(u64 *)(lmp->name);
		v0=pli[0];	v1=pli[1];
		v2=pli[2];	v3=pli[3];
		di=(lmp->dirid)|((lmp->hibits&0x0F00)<<8);
		if(		(u0==v0) && (u1==v1) &&
				(u2==v2) && (u3==v3) &&
				(pfx==di))
			return(i);
		i=(lmp->chn)|((lmp->hibits&0x000F)<<16);
	}
	return(-1);
}

int TK_Wad4_DigitBase32(int v)
{
	if((v>=0) && (v<=9))
		return('0'+v);
	return('a'+(v-10));
}

int TK_Wad4_Wad2BuildPfxName(char *tn, char *name, int pfx)
{
	char *s, *t;

	memset(tn, 0, 16);

	if(!pfx)
	{
//		w_strlwr_n(tn, name, 16);
		s=name; t=tn;
		while(*s && (*s!='.'))
			*t++=tolower(*s++);
		*t++=0;

		return(0);
	}

	if(pfx<32)
	{
		tn[0]=TK_Wad4_DigitBase32(pfx&31);
		tn[1]='|';
//		w_strlwr_n(tn+2, name, 12);
		s=name; t=tn+2;
		while(*s && (*s!='.'))
			*t++=tolower(*s++);
		*t++=0;
		return(0);
	}

	if(pfx<1024)
	{
		tn[0]=TK_Wad4_DigitBase32((pfx>> 5)&31);
		tn[1]=TK_Wad4_DigitBase32((pfx>> 0)&31);
		tn[2]='|';
//		w_strlwr_n(tn+2, name, 12);
		s=name; t=tn+3;
		while(*s && (*s!='.'))
			*t++=tolower(*s++);
		*t++=0;
		return(0);
	}

	tn[0]=TK_Wad4_DigitBase32((pfx>>10)&31);
	tn[1]=TK_Wad4_DigitBase32((pfx>> 5)&31);
	tn[2]=TK_Wad4_DigitBase32((pfx>> 0)&31);
	tn[3]='|';
//	w_strlwr_n(tn+4, name, 12);
	s=name; t=tn+4;
	while(*s && (*s!='.'))
		*t++=tolower(*s++);
	*t++=0;
	return(0);
}

int TK_Wad4_LookupLumpNameW2(TK_WadImage *img, char *name, int pfx)
{
	char tn[35];
	TK_Wad2Lump *lmp;
	u64 *pli;
	u64 v0, v1, v2, v3;
	u64 u0, u1, u2, u3;
	int h, i, di;

	memset(tn, 0, 17);
	TK_Wad4_Wad2BuildPfxName(tn, name, pfx);
	
	h=TK_Wad4_HashIndexForName16(tn);
	u0=((u64 *)tn)[0];	u1=((u64 *)tn)[1];
	
	i=img->hash2[h];
	while(i>0)
	{
		lmp=img->w2dir+i;
		pli=(u64 *)(lmp->name);
		v0=pli[0];	v1=pli[1];
		if((u0==v0) && (u1==v1))
			return(i);
		i=lmp->chn;
	}
	return(-1);
}

int TK_Wad4_LookupLumpName(TK_WadImage *img, char *name, int pfx)
{
	int id;

	if(img->w4dir)
	{
		id=TK_Wad4_LookupLumpNameW4(img, name, pfx);
		return(id);
	}

	if(img->w2dir)
	{
		id=TK_Wad4_LookupLumpNameW4(img, name, pfx);
		return(id);
	}

	return(-1);
}

int TK_Wad4_LookupDirName(TK_WadImage *img, char *name, int pfx)
{
	int id;

	if(img->w4dir)
	{
		id=TK_Wad4_LookupLumpNameW4(img, name, pfx);
		return(id);
	}

	if(img->w2dir)
	{
		id=TK_Wad4_LookupLumpNameW2(img, name, pfx);
		if(id<0)return(id);
		id=(img->w2dir[id].offs);
		return(id);
	}

	return(-1);
}

int TK_Wad4_LookupLumpPathI(TK_WadImage *img, char *path, int pfx)
{
	char tn[48];
	char *s, *t;
	int pfx1, id;
	
	s=path;
	if((s[0]=='.') && (s[1]=='/'))
		s++;
	while((*s=='/') || (*s=='\\'))
		s++;
	t=tn;
	while(*s && (*s!='/') && (*s!='\\'))
		*t++=*s++;
	*t++=0;
	
	if(!(*s))
	{
		id=TK_Wad4_LookupLumpName(img, tn, pfx);
		return(id);
	}

	pfx1=TK_Wad4_LookupDirName(img, tn, pfx);
	id=TK_Wad4_LookupLumpPathI(img, s, pfx1);
	return(id);
}

int TK_Wad4_LookupLumpPath(TK_WadImage *img, char *path)
	{ return(TK_Wad4_LookupLumpPathI(img, path, 0)); }

void *TK_Wad4_CacheLumpPath(TK_WadImage *img, char *path, int *rsz)
{
	void *ptr;
	int id;
	
	id=TK_Wad4_LookupLumpPath(img, path);
	if(id<0)
	{
		if(rsz)*rsz=0;
		return(NULL);
	}
	
	ptr=TK_Wad4_GetCacheLumpNum(img, id, rsz);
	return(ptr);
}
