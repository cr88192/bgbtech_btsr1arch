/*
Aim to support VFS capabilities for WAD2A and WAD4.
 */

TK_WadZBlock	*tk_wad4_zlive;
TK_WadZBlock	*tk_wad4_zfree;
int				tk_wad4_zmemuse;
int				tk_wad4_zmemlim = 4*(1<<20);

void *TK_Wad4_GetCacheLumpNum(TK_WadImage *img, int lump, int *rsz);
byte *TKPE_UnpackBuffer(byte *ct, byte *ibuf, int isz, int cmp);

TK_WadZBlock *TK_Wad4_ZAllocBlock()
{
	TK_WadZBlock *tmp;
	
	tmp=tk_wad4_zfree;
	if(tmp)
	{
		tk_wad4_zfree=tmp->next;
		return(tmp);
	}
	
	tmp=tk_malloc_krn(sizeof(TK_WadZBlock));
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
	if(cur)
	{
		if(prv)
			{ prv->next=cur->next; }
		else
			{ tk_wad4_zlive=cur->next; }
	}

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
	ptr=tk_malloc_krn(sz+sizeof(void *));
	blk->data=ptr;
	blk->size=sz;
	blk->user=user;
	blk->tag=tag;
	blk->evptr=NULL;
	blk->EvFunc=NULL;
	
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

void TK_Wad4_ZChangeTag(void *ptr, int tag)
{
	TK_WadZBlock *blk;
	blk=*(((void **)ptr)-1);
	blk->tag=tag;
//	TK_Wad4_ZFreeBlock(blk);
}

void TK_Wad4_ZChangeTagEvict(void *ptr, int tag,
	void *evfunc, void *evptr)
{
	TK_WadZBlock *blk;
	blk=*(((void **)ptr)-1);
	blk->tag=tag;
//	TK_Wad4_ZFreeBlock(blk);
	
	blk->EvFunc=evfunc;
	blk->evptr=evptr;
}

void TK_Wad4_ZEvictCache()
{
	TK_WadZBlock *blk, *nxt;
	void *ptr, *ptr1;
	int rt;
	
	blk=tk_wad4_zlive;
	while(blk)
	{
		nxt=blk->next;
		if(blk->tag>=TK_W4PU_CACHE)
		{
			TK_Wad4_ZFreeBlock(blk);
		}else if(blk->tag>=TK_W4PU_CACHEDIRTY)
		{
			if(blk->EvFunc)
			{
				ptr=blk->data;
				ptr1=(void *)(((void **)ptr)+1);

				rt=blk->EvFunc(ptr1, blk->evptr);
				if(rt>0)
					{ TK_Wad4_ZFreeBlock(blk); }
			}
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

	img=tk_malloc_krn(sizeof(TK_WadImage));
	memset(img, 0, sizeof(TK_WadImage));

//	if(__offsetof(TK_WadImage, mntbase)>=sizeof(TK_WadImage))
//		__debugbreak();

//	tk_printf("TK_Wad4_AllocImage: p=%p, sz=%d\n", img, sizeof(TK_WadImage));

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
	int wadver, hbits, hsz, nl0, nl1;

	img=TK_Wad4_AllocImage();
	img->img_fd=fd;
	
	inf=tk_malloc_krn(sizeof(TK_Wad4Info));
	img->w2inf=(TK_Wad2Info *)inf;
	img->w4inf=(TK_Wad4Info *)inf;
	
	tk_fseek(fd, 0, 0);
	tk_fread(img->w4inf, 1, sizeof(TK_Wad4Info), fd);

	if(	(inf->ident[0]!='W') ||
		(inf->ident[1]!='A') ||
		(inf->ident[2]!='D'))
	{
		tk_printf("TK_Wad4_OpenImage: Not WAD\n");
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
		tk_printf("TK_Wad4_OpenImage: Not WAD2 or WAD4\n");

		tk_free(inf);
		TK_Wad4_FreeImage(img);
		return(NULL);
	}

	if(wadver==2)
	{
		tk_dbg_printf("TK_Wad4_OpenImage: WAD2\n");

		img->w2dir=tk_malloc_krn(inf->numlumps*sizeof(TK_Wad2Lump));
		img->w4dir=NULL;
		tk_fseek(fd, inf->diroffs, 0);
		tk_fread(img->w2dir, 1, inf->numlumps*sizeof(TK_Wad2Lump), fd);

		img->hashsz=64;
		hsz=64;
		
		img->hash2=tk_malloc_krn(hsz*2);
		tk_fread(img->hash2, 1, hsz*2, fd);

		if(img->w4dir)
			__debugbreak();
	}

	if(wadver==4)
	{
//		tk_printf("TK_Wad4_OpenImage: WAD4\n");

		nl0=inf->numlumps;
		nl1=nl0+256;

//		img->w4dir=tk_malloc_krn(inf->numlumps*sizeof(TK_Wad4Lump));
		img->w4dir=tk_malloc_krn(nl1*sizeof(TK_Wad4Lump));
		memset(img->w4dir, 0, nl1*sizeof(TK_Wad4Lump));
		img->w2dir=NULL;
		
		tk_fseek(fd, inf->diroffs*64, 0);
//		tk_fread(img->w4dir, 1, inf->numlumps*sizeof(TK_Wad4Lump), fd);
		tk_fread(img->w4dir, 1, nl0*sizeof(TK_Wad4Lump), fd);
		
		hbits=tk_log2u(inf->numlumps)-4;
		if(hbits<6)hbits=6;
		if(hbits>16)hbits=16;
		hsz=1<<hbits;
		img->hashsz=hsz;

//		tk_printf("TK_Wad4_OpenImage: hbits=%d / %d\n", hbits, hsz);
		
		if(inf->numlumps>=65536)
		{
//			tk_printf("TK_Wad4_OpenImage: Hash4\n");

			img->hash4=tk_malloc_krn(hsz*4);
			tk_fseek(fd, inf->hashoffs*64, 0);
			tk_fread(img->hash4, 1, hsz*4, fd);
		}else
		{
//			tk_printf("TK_Wad4_OpenImage: Hash2\n");

			img->hash2=tk_malloc_krn(hsz*2);
			tk_fseek(fd, inf->hashoffs*64, 0);
			tk_fread(img->hash2, 1, hsz*2, fd);
		}
		
		inf->numlumps=nl1;

		if(img->w2dir)
			__debugbreak();
	}
	
	img->lca_data=tk_malloc_krn(inf->numlumps*sizeof(void *));
//	img->lca_sz=tk_malloc_krn(inf->numlumps*sizeof(int));

	memset(img->lca_data, 0, inf->numlumps*sizeof(void *));

	return(img);
}

TK_WadImage *TK_Wad4_CreateTempRamImage(int dirsz)
{
	TK_WadImage *img;
	TK_Wad4Info *inf;
	TK_Wad4Lump *lmp;
	int wadver, hbits, hsz;

	img=TK_Wad4_AllocImage();
//	img->img_fd=fd;
	
	inf=tk_malloc_krn(sizeof(TK_Wad4Info));
	img->w4inf=(TK_Wad4Info *)inf;
	img->wadver=4;
	img->readwrite=1;
	
	inf->numlumps=dirsz;

//	if(wadver==4)
	if(1)
	{
		img->w4dir=tk_malloc_krn(dirsz*sizeof(TK_Wad4Lump));
		memset(img->w4dir, 0, dirsz*sizeof(TK_Wad4Lump));

		lmp=(img->w4dir)+0;
		strcpy(lmp->name, "$ROOT");
		lmp->ety=TK_W4ETY_DIR;

		lmp=(img->w4dir)+1;
		strcpy(lmp->name, "$BITMAP");
		lmp->ety=TK_W4ETY_NORMAL;

//		tk_fseek(fd, inf->diroffs*64, 0);
//		tk_fread(img->w4dir, 1, inf->numlumps*sizeof(TK_Wad4Lump), fd);
		
		hbits=tk_log2u(dirsz)-4;
		if(hbits<6)hbits=6;
		if(hbits>16)hbits=16;
		hsz=1<<hbits;
		img->hashsz=hsz;
		
		if(inf->numlumps>=65536)
		{
			img->hash4=tk_malloc_krn(hsz*4);
			memset(img->hash4, 0, hsz*4);

//			tk_fseek(fd, inf->hashoffs*64, 0);
//			tk_fread(img->hash4, 1, hsz*4, fd);
		}else
		{
			img->hash2=tk_malloc_krn(hsz*2);
			memset(img->hash2, 0, hsz*2);

//			tk_fseek(fd, inf->hashoffs*64, 0);
//			tk_fread(img->hash2, 1, hsz*2, fd);
		}
	}
	
	img->lca_data=tk_malloc_krn(inf->numlumps*sizeof(void *));
//	img->lca_sz=tk_malloc_krn(inf->numlumps*sizeof(int));

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
	byte *cs, *cse, *cs0, *ct, *ct0;
	int i, j, k;
	
	if(img->w4dir)
	{
		cs=img->w4dir[lump].name;
		cse=cs+32;
		cs0=cs;
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
		
//		tk_printf("TK_Wad4_GetLumpDirName: %s %s\n", cs0, ct0);
		
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

int TK_Wad4_GetLumpDirRawLink(TK_WadImage *img,
	int lump, char *name)
{
	byte *cs, *cse, *ct, *ct0, *ptr;
	int sz;
	int i, j, k;
	
	sz=0;
	if(img->w4dir)
	{
		ptr=TK_Wad4_GetCacheLumpNum(img, lump, &sz);

		ct=(byte *)name;
		cs=ptr;
		cse=cs+sz;
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
	}

	if(img->w2dir)
	{
	}
	
	return(-1);
}

int TK_Wad4_GetLinkPBase(TK_WadImage *img,
	char *bpath, char *ppath, char *obuf)
{
	char *cs, *ct;

	ct=obuf;
	*ct++='/';
	cs=img->mntbase;
	if(cs)
	{
		while(*cs)
			{ *ct++=*cs++; }
		*ct++='/';
	}
	cs=bpath;
	while(*cs && (cs!=ppath))
		{ *ct++=*cs++; }
	while((ct>bpath) && (*(ct-1)=='/'))	ct--;
	while((ct>bpath) && (*(ct-1)!='/'))	ct--;
	while((ct>bpath) && (*(ct-1)=='/'))	ct--;
	*ct++=0;
	return(ct-obuf);
}

int TK_Wad4_GetLumpDirBaseLink(TK_WadImage *img, int lump,
	char *pbase, char *oname)
{
	char tb[256];
	byte *cs, *cse, *ct, *ct0, *ptr;
	int sz, isuri;
	int i, j, k;
	
	sz=TK_Wad4_GetLumpDirRawLink(img, lump, tb);
	ptr=tb;

	cs=ptr; i=*cs++;
	while(((i>='a') && (i<='z')) || ((i>='A') && (i<='Z')))
		i=*cs++;
	isuri=(i==':');

	ct=(byte *)oname;
	ct0=ct;
	
	if(ptr[0]==':')
	{
		cs=img->mntbase;
		if(cs)
		{
			while(*cs)
				{ *ct++=*cs++; }
		}
		*ct++='/';
		ptr[0]++;
	}else if((ptr[0]!='/') && !isuri)
	{
		cs=pbase;
		if(cs)
		{
			while(*cs)
				{ *ct++=*cs++; }
		}
		*ct++='/';
	}

	cs=ptr;
	cse=cs+sz;
	
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
}

byte *tk_wad4_tcbuf=NULL;
int tk_wad4_tcsz;

void TK_Wad4_ReadLumpBuffer(TK_WadImage *img, int lump, void *buf)
{
	TK_Wad4Lump	*w4l;
	TK_Wad2Lump	*w2l;
	TK_FILE		*fd;
	byte		*tptr, *ct1;
	s64 offs;
	u32 csum, xcsum, csum1;
	int csz, lmp1, offs0, ofs1, csz1;
	int dsz, dsz1;
	int cmp;

	csz1=0;

	if(img->w4dir)
	{
		w4l=img->w4dir+lump;
		offs0=w4l->offs;
		offs=offs0<<6;
		csz=w4l->csize;
		dsz=w4l->dsize;
		cmp=w4l->cmp;
	}else if(img->w2dir)
	{
		w2l=img->w2dir+lump;
		offs0=w2l->offs;
		offs=offs0;
		csz=w2l->csize;
		dsz=w2l->dsize;
		cmp=w2l->cmp;
	}

	if(cmp==TK_W4CMP_PACKED)
	{
		lmp1=(offs0>>16)&65535;
		ofs1=(offs0&65535)<<6;
		tptr=TK_Wad4_GetCacheLumpNum(img, lmp1, &csz1);
		memcpy(buf, tptr+ofs1, dsz);
		return;
	}
	
	if(cmp==TK_W4CMP_FRAG)
	{
		/* Base lump of fragmented file is not read this way. */
		return;
	}

	fd=img->img_fd;
	
	if(!fd)
	{
		tk_printf("TK_Wad4_ReadLumpBuffer: Read with no image file\n");
		memset(buf, 0, dsz);
		return;
	}
	

	if(cmp==TK_W4CMP_NONE)
	{
		tk_fseek(fd, offs, 0);
		tk_fread(buf, 1, dsz, fd);
		return;
	}

	if(!tk_wad4_tcbuf)
	{
		csz1=65536;
		while(csz>csz1)
			csz1=csz1+(csz1>>1);
		tk_wad4_tcbuf=tk_malloc_krn(csz1);
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

		ct1=TKPE_UnpackBuffer(buf, tk_wad4_tcbuf, csz, cmp);
		dsz1=ct1-buf;
		if(dsz1!=dsz)
		{
			tk_printf("TK_Wad4_ReadLumpBuffer: "
				"Unpack Size Error %d!=%d cm=%d\n",
				dsz1, dsz, cmp);
			return;
		}
		if(cmp==TK_W4CMP_RP2)
		{
			csum=*(u32 *)(tk_wad4_tcbuf+(csz-8)+0);
			xcsum=*(u32 *)(tk_wad4_tcbuf+(csz-8)+4);
			if(csum==(~xcsum))
			{
//				csum1=TKPE_CalculateImagePel4BChecksum(buf, dsz);
				csum1=TKPE_CalculateImagePel4BChecksumAc(buf, dsz);
				if(csum1!=csum)
				{
					tk_printf("TK_Wad4_ReadLumpBuffer: "
						"RP2 Checksum Error %X!=%X\n",
						csum1, csum);
				}
			}
		}
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

//	if(cmp==TK_W4CMP_PACK)
//	{
//		ptr=TK_Wad4_GetCacheLumpNum()
//	}

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
//	int		fraga[64];
	int		*fraga;
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
		*rrofs=ofs;
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

	fraga=img->lca_data[lump];
	if(!fraga)
	{
		fraga=TK_Wad4_ZMalloc(64*sizeof(int),
			TK_W4PU_CACHE, img->lca_data+lump);
		img->lca_data[lump]=fraga;

		fd=img->img_fd;
		tk_fseek(fd, offs, 0);
		tk_fread(fraga, 1, csz, fd);
	}
	
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

int	TK_Wad4_LumpEvFunc(void *blkptr, void *vfsptr)
{
	TK_WadImage *img;
	
	img=vfsptr;
	if(!(img->img_fd))
		return(0);

	return(0);
}

void *TK_Wad4_GetCacheExpandLumpNum(TK_WadImage *img,
	int lump, int newsz, int *rsz)
{
	void *ptr, *ptr1;
	int sz, fxi, sz1, cmp;

	sz=TK_Wad4_GetLumpSize(img, lump);
	if(sz>=newsz)
	{
		ptr=TK_Wad4_GetCacheLumpNum(img, lump, &sz);
		if(ptr)
		{
//			TK_Wad4_ZChangeTag(ptr, TK_W4PU_CACHEDIRTY);
			TK_Wad4_ZChangeTagEvict(ptr, TK_W4PU_CACHEDIRTY, 
				TK_Wad4_LumpEvFunc, img);
		}
		if(rsz)
			*rsz=sz;
		return(ptr);
	}
	
	ptr=img->lca_data[lump];

	fxi=TK_Wad4_SizeToFxiU(newsz);
	sz1=TK_Wad4_FxiToSize(fxi);
	if(sz1<256)sz1=256;

	ptr1=TK_Wad4_ZMalloc(sz1, TK_W4PU_CACHEDIRTY, img->lca_data+lump);
	TK_Wad4_ZChangeTagEvict(ptr1, TK_W4PU_CACHEDIRTY, TK_Wad4_LumpEvFunc, img);
	img->lca_data[lump]=ptr1;
	TK_Wad4_SetLumpSize(img, lump, newsz);

	if(ptr)
	{
		memcpy(ptr1, ptr, sz);
		TK_Wad4_ZFree(ptr);
	}

	if(rsz)
		*rsz=newsz;	
	return(ptr1);

//	if(sz<=0)
//	{
//		if(rsz)
//			*rsz=0;
//		return(NULL);
//	}
}

int TK_Wad4_AllocateNewLump(TK_WadImage *img)
{
	TK_Wad4Lump *lmp;
	int nl;
	int i, j, k;

	if(!img->w4dir)
		return(-1);

	nl=img->w4inf->numlumps;
	for(i=2; i<nl; i++)
	{
		lmp=img->w4dir+i;
		if(!(lmp->ety))
			break;
	}
	
	if(i<nl)
	{
		lmp=img->w4dir+i;
		lmp->offs=0;
		lmp->csize=0;
		lmp->dsize=0;
		lmp->ety=TK_W4ETY_NORMAL;
		lmp->cmp=TK_W4CMP_NONE;
		return(i);
	}

	return(-1);
}

int TK_Wad4_AllocateNewFragLump(TK_WadImage *img)
{
	return(TK_Wad4_AllocateNewLump(img));
}

void *TK_Wad4_GetCacheExpandLumpNumOffs(TK_WadImage *img,
	int lump, int ofs, int wsz, int *rrofs, int *rsz)
{
//	int		fraga[64];
	int		*fraga;
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
//		ptr=TK_Wad4_GetCacheLumpNum(img, lump, rsz);
		ptr=TK_Wad4_GetCacheExpandLumpNum(img, lump, ofs+wsz, rsz);
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

	fraga=img->lca_data[lump];
	if(!fraga)
	{
		fraga=TK_Wad4_ZMalloc(64*sizeof(int),
			TK_W4PU_CACHE, img->lca_data+lump);
		TK_Wad4_ZChangeTagEvict(fraga, TK_W4PU_CACHEDIRTY,
			TK_Wad4_LumpEvFunc, img);
		img->lca_data[lump]=fraga;

		fd=img->img_fd;
		if(fd)
		{
			tk_fseek(fd, offs, 0);
			tk_fread(fraga, 1, csz, fd);
		}else
		{
			memset(fraga, 0, 64*sizeof(int));
		}
	}
	
	n=csz>>2;
	i=0; offs1=ofs;
	while(i<n)
	{
		lmp1=fraga[i];
		if(lmp1<=0)
		{
			lmp1=TK_Wad4_AllocateNewFragLump(img);
			fraga[i]=lmp1;
		}
		sz1=TK_Wad4_GetLumpSize(img, lmp1);
		if(offs1<sz1)
			break;
		if((offs1<TK_W4_FRAGSZ) && (fraga[i+1]<=0))
			break;
		offs1-=sz1;
		i++;
	}
	
	if(i>=n)
		return(NULL);

	*rrofs=offs1;
	
	sz1=offs1+wsz;
	if(sz1>TK_W4_FRAGSZ)
		sz1=TK_W4_FRAGSZ;
	
//	ptr=TK_Wad4_GetCacheLumpNum(img, lmp1, rsz);
//	ptr=TK_Wad4_GetCacheExpandLumpNum(img, lmp1, ofs1+wsz, rsz);
	ptr=TK_Wad4_GetCacheExpandLumpNum(img, lmp1, sz1, rsz);
	return(ptr);
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
	
//	printf("TK_Wad4_LookupLumpNameW4: %s %d %X\n", name, pfx, h);
	
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
	int h, i, di, nl;

	memset(tn, 0, 17);
	TK_Wad4_Wad2BuildPfxName(tn, name, pfx);
	
//	tk_dbg_printf("TK_Wad4_LookupLumpNameW2: tn=%s\n", tn);
	
	h=TK_Wad4_HashIndexForName16(tn);
	u0=((u64 *)tn)[0];	u1=((u64 *)tn)[1];
	
	nl=img->w2inf->numlumps;
	
	i=(s16)(img->hash2[h]);
	while(i>=0)
	{
		lmp=img->w2dir+i;
		pli=(u64 *)(lmp->name);
		v0=pli[0];	v1=pli[1];
		if((u0==v0) && (u1==v1))
			return(i);
		i=(s16)(lmp->chn);
	}
	
	tk_dbg_printf("TK_Wad4_LookupLumpNameW2: tn=%s h=%02X, h2i=%04X\n",
		tn, h, img->hash2[h]);
	
	for(i=0; i<nl; i++)
	{
		lmp=img->w2dir+i;
		pli=(u64 *)(lmp->name);
		v0=pli[0];	v1=pli[1];
		if((u0==v0) && (u1==v1))
			return(i);
	}

	tk_dbg_printf("TK_Wad4_LookupLumpNameW2: Not Found, tn=%s\n", tn);

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
		id=TK_Wad4_LookupLumpNameW2(img, name, pfx);
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
	int pfx1, id, ety;
	
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
		img->tmp_ppath=NULL;
		id=TK_Wad4_LookupLumpName(img, tn, pfx);
		return(id);
	}

	pfx1=TK_Wad4_LookupDirName(img, tn, pfx);
	if(pfx1<=0)
		return(-1);
	
	ety=TK_Wad4_GetLumpEntType(img, pfx1);
	if(ety==TK_W4ETY_SYMLINK)
	{
		img->tmp_ppath=s;
		return(pfx1);
	}
	
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

int TK_Wad4_CreateNewLumpBasic(TK_WadImage *img, char *name, int pfx)
{
	TK_Wad4Lump *lmp, *plmp;
	int id, hi;

//	tk_printf("TK_Wad4_CreateNewLumpBasic: name=%s pfx=%X\n", name, pfx);

	id=TK_Wad4_AllocateNewLump(img);
	if(id<0)
		return(-1);

	lmp=img->w4dir+id;
	plmp=img->w4dir+pfx;

	memset(lmp->name, 0, 32);
	memcpy(lmp->name, name, strlen(name));

//	tk_printf("TK_Wad4_CreateNewLumpBasic: lname=%s\n", lmp->name, pfx);

	hi=TK_Wad4_HashIndexForName32(lmp->name, pfx);
	hi=hi&(img->hashsz-1);

	lmp->ety=TK_W4ETY_NORMAL;
	lmp->cmp=TK_W4CMP_NONE;
	lmp->dirid=pfx;
	lmp->dirnext=plmp->offs;
	plmp->offs=id;
	
	if(img->hash2)
	{
		lmp->chn=img->hash2[hi];
		img->hash2[hi]=id;
	}else if(img->hash4)
	{
		lmp->chn=img->hash4[hi];
		img->hash4[hi]=id;
	}
	
	return(id);
}

int TK_Wad4_CreateNewLumpDir(TK_WadImage *img, char *name, int pfx)
{
	TK_Wad4Lump *lmp;
	int id;

	id=TK_Wad4_CreateNewLumpBasic(img, name, pfx);
	if(id<0)
		return(-1);

	lmp=img->w4dir+id;
	lmp->ety=TK_W4ETY_DIR;
	lmp->cmp=TK_W4CMP_NONE;
	
	return(id);
}

int TK_Wad4_CreateLumpPathI(TK_WadImage *img, char *path, int pfx)
{
	char tn[48];
	char *s, *t;
	int pfx1, id, pety;
	
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
		img->tmp_ppath=NULL;
		id=TK_Wad4_LookupLumpName(img, tn, pfx);
		if(id>0)
			return(id);
		id=TK_Wad4_CreateNewLumpBasic(img, tn, pfx);
		return(id);
	}

	pfx1=TK_Wad4_LookupDirName(img, tn, pfx);
	if(pfx1>0)
	{
		pety=TK_Wad4_GetLumpEntType(img, pfx1);
		if(pety==TK_W4ETY_SYMLINK)
		{
			img->tmp_ppath=s;
			return(pfx1);
		}

		id=TK_Wad4_CreateLumpPathI(img, s, pfx1);
		return(id);
	}
	
	pfx1=TK_Wad4_CreateNewLumpDir(img, tn, pfx);
	id=TK_Wad4_CreateLumpPathI(img, s, pfx1);
	return(id);
}

int TK_Wad4_CreateLumpPath(TK_WadImage *img, char *path)
	{ return(TK_Wad4_CreateLumpPathI(img, path, 0)); }

int TK_Wad4_CreateDirPath(TK_WadImage *img, char *path)
{
	TK_Wad4Lump *lmp;
	int id;

	id=TK_Wad4_CreateLumpPath(img, path);
	if(id<=0)
		return(id);

	lmp=img->w4dir+id;
	if(!(lmp->offs) && !(lmp->dsize))
	{
		lmp->ety=TK_W4ETY_DIR;
		lmp->cmp=TK_W4CMP_NONE;
		return(id);
	}
	return(-1);
}

int TK_Wad4_CreateLink(TK_WadImage *img, char *path, char *dest)
{
	TK_Wad4Lump *lmp;
	char *ptr;
	int id, sz;

	tk_printf("TK_Wad4_CreateLink: %s %s\n", path, dest);

	id=TK_Wad4_CreateLumpPath(img, path);
	if(id<=0)
		return(id);

	lmp=img->w4dir+id;
	if(!(lmp->offs) && !(lmp->dsize))
	{
		sz=strlen(dest)+1;
		ptr=TK_Wad4_GetCacheExpandLumpNum(img, id, sz, &sz);
		strcpy(ptr, dest);
		lmp->ety=TK_W4ETY_SYMLINK;
		return(id);
	}

	return(-1);
}

int TK_Wad4_UnlinkDirLump(TK_WadImage *img, int id)
{
	TK_Wad4Lump *lmp, *lmp1, *plmp;
	int id1, pfx, hi;

	lmp=img->w4dir+id;
	pfx=lmp->dirid;
	plmp=img->w4dir+pfx;

	hi=TK_Wad4_HashIndexForName32(lmp->name, pfx);
	hi=hi&(img->hashsz-1);

	id1=plmp->offs;
	if(id1==id)
	{
		plmp->offs=lmp->dirnext;
	}else
	{
		while(id1>0)
		{
			lmp1=img->w4dir+id1;
			if(lmp1->dirnext==id)
			{
				lmp1->dirnext=lmp->dirnext;
				break;
			}
			id1=lmp1->dirnext;
		}
	}
	
	id1=img->hash2[hi];
	if(id==id1)
	{
		img->hash2[hi]=lmp->chn;
	}else
	{
		while(id1>0)
		{
			lmp1=img->w4dir+id1;
			if(lmp1->chn==id)
			{
				lmp1->chn=lmp->chn;
				break;
			}
			id1=lmp1->chn;
		}
	}
	
	lmp->dirid=1;
}

int TK_Wad4_UnlinkLumpPath(TK_WadImage *img, char *path)
{
	int id;
	
	id=TK_Wad4_LookupLumpPath(img, path);
	if(id<=0)
		return(-1);
	TK_Wad4_UnlinkDirLump(img, id);
	return(id);
}
