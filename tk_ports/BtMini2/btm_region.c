/*
Copyright (C) 2022  Brendan G Bohannon

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

byte *FS_LoadFile(char *name, int *rsz, int pad);

byte * volatile btm_freelist_mul64k[256];
byte * volatile btm_freelist_mul4k[256];
byte * volatile btm_freelist_mul256b[256];
void * volatile btm_alloc_mutex;
void * volatile btm_alloctmp_mutex;

int btm_totalloc_mul64k[256];
int btm_totalloc_mul4k[256];
int btm_totalloc_mul256b[256];

int BTM_LockAlloc()
{
	if(!btm_alloc_mutex)
		btm_alloc_mutex=thMutex();
	thLockMutex(btm_alloc_mutex);
	return(0);
}

int BTM_UnlockAlloc()
{
	thUnlockMutex(btm_alloc_mutex);
	return(0);
}

int BTM_LockAllocTmp()
{
	if(!btm_alloctmp_mutex)
		btm_alloctmp_mutex=thMutex();
	thLockMutex(btm_alloctmp_mutex);
	return(0);
}

int BTM_UnlockAllocTmp()
{
	thUnlockMutex(btm_alloctmp_mutex);
	return(0);
}

void *BTM_AllocMul64K(int n)
{
	byte *ptr;
	
	BTM_LockAlloc();
	ptr=btm_freelist_mul64k[n];
	if(ptr)
	{
		btm_freelist_mul64k[n]=*(byte **)ptr;
		BTM_UnlockAlloc();
		return(ptr);
	}
	
	btm_totalloc_mul64k[n]+=n<<16;
	ptr=btm_malloc(n<<16);
	BTM_UnlockAlloc();
	return(ptr);
}

void *BTM_AllocMul4K(int n)
{
	byte *ptr;
	
	if(n>=64)
		return(BTM_AllocMul64K((n+15)>>4));
	
	BTM_LockAlloc();
	ptr=btm_freelist_mul4k[n];
	if(ptr)
	{
		btm_freelist_mul4k[n]=*(byte **)ptr;
		BTM_UnlockAlloc();
		return(ptr);
	}
	
	btm_totalloc_mul4k[n]+=n<<12;
	ptr=btm_malloc(n<<12);
	BTM_UnlockAlloc();
	return(ptr);
}

void *BTM_AllocMul256B(int n)
{
	byte *ptr;
	
	if(n>=64)
		return(BTM_AllocMul4K((n+15)>>4));
	
	BTM_LockAlloc();
	ptr=btm_freelist_mul256b[n];
	if(ptr)
	{
		btm_freelist_mul256b[n]=*(byte **)ptr;
		BTM_UnlockAlloc();
		return(ptr);
	}
	
	btm_totalloc_mul256b[n]+=n<<8;
	ptr=btm_malloc(n<<8);
	BTM_UnlockAlloc();
	return(ptr);
}

int BTM_FreeMul64K(void *ptr, int n)
{
	BTM_LockAlloc();
	*(byte **)ptr=btm_freelist_mul64k[n];
	btm_freelist_mul64k[n]=ptr;
	BTM_UnlockAlloc();
	return(0);
}

int BTM_FreeMul4K(void *ptr, int n)
{
	if(n>=64)
	{
		BTM_FreeMul64K(ptr, (n+15)>>4);
		return(0);
	}

	BTM_LockAlloc();
	*(byte **)ptr=btm_freelist_mul4k[n];
	btm_freelist_mul4k[n]=ptr;
	BTM_UnlockAlloc();
	return(0);
}

int BTM_FreeMul256B(void *ptr, int n)
{
	if(n>=64)
	{
		BTM_FreeMul4K(ptr, (n+15)>>4);
		return(0);
	}

	BTM_LockAlloc();
	*(byte **)ptr=btm_freelist_mul256b[n];
	btm_freelist_mul256b[n]=ptr;
	BTM_UnlockAlloc();
	return(0);
}

byte *btm_loadtemp_buf[8*32];
int btm_loadtemp_sz[8*32];
int btm_loadtemp_rov;

byte *BTM_AllocTempBuffer(int sz)
{
	void *ptr;
	int ix, sz1, tid, tix;

//	if(sz<=1024)
//	{
//		return(bccx_ralloc(sz));
//	}
	
	BTM_LockAllocTmp();
	
	tid=thGetThreadId();
	
	ix=btm_loadtemp_rov;
	btm_loadtemp_rov=(ix+1)&15;

#if 1
	if(sz>=262144)
		{ ix=0; }
	else if(sz>=65536)
		{ ix=2|(ix&1); }
	else if(sz>=16384)
		{ ix=4|(ix&3); }
	else if(sz>=4096)
		{ ix=8|(ix&7); }
	else
		{ ix=16|(ix&15); }
#endif

#if 0
	if(sz>=1048576)
		{ ix=0; }
	else if(sz>=262144)
		{ ix=2|(ix&1); }
	else if(sz>=65536)
		{ ix=4|(ix&3); }
	else if(sz>=16384)
		{ ix=8|(ix&7); }
	else
		{ ix=16|(ix&15); }
#endif

	tix=tid^(tid>>3)^(tid>>6);
//	ix=ix+((tid*31)&0xE0);
	ix=ix+((tix<<5)&0xE0);

	sz1=(sz+255)>>8;
	
	if(btm_loadtemp_buf[ix] && (sz1>btm_loadtemp_sz[ix]))
	{
		BTM_FreeMul256B(btm_loadtemp_buf[ix], btm_loadtemp_sz[ix]);
		btm_loadtemp_buf[ix]=NULL;
	}
	
	if(!btm_loadtemp_buf[ix])
	{
		btm_loadtemp_buf[ix]=BTM_AllocMul256B(sz1);
		btm_loadtemp_sz[ix]=sz1;
	}
	
	ptr=btm_loadtemp_buf[ix];
	
	BTM_UnlockAllocTmp();
	
	return(ptr);
}

byte *BTM_LoadFileI(char *name, int *rsz, int pad)
{
	char tb[256];
	byte *buf;
	FILE *fd;
	int sz, i;

	sprintf(tb, "resource/%s", name);
	
//	fd=fopen(name, "rb");
	fd=fopen(tb, "rb");
	if(!fd)
	{
		buf=FS_LoadFile(name, rsz, pad);
		if(buf)
			return(buf);
		
		printf("BTM_LoadFile: Failed open %s\n", name);
		return(NULL);
	}
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	if(pad&4)
		{ buf=BTM_AllocTempBuffer(sz+24); }
	else if(pad&2)
		{ buf=BTM_AllocMul64K((sz+65535)>>16); }
	else if(pad&1)
		{ buf=btm_malloc(sz+24); }
	else
		{ buf=btm_malloc(sz); }
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

byte *BTM_LoadFileNp(char *name, int *rsz)
{
	return(BTM_LoadFileI(name, rsz, 2));
}

byte *BTM_LoadFileTmp(char *name, int *rsz)
{
	return(BTM_LoadFileI(name, rsz, 5));
}

int BTM_StoreFile(char *name, void *buf, int sz)
{
	char tb[256];
	FILE *fd;

	sprintf(tb, "resource/%s", name);

//	fd=fopen(name, "wb");
	fd=fopen(tb, "wb");
	if(!fd)
		return(-1);
	
	fwrite(buf, 1, sz, fd);
	fclose(fd);
	return(0);
}

int BTM_RegionAllocGetCell(
	BTM_World *wrl, BTM_Region *rgn, int cix)
{
	u64 bm;
	int ix, v;
	
	if(cix<0)
		return(-1);
	if(cix>=rgn->img_ncell)
		return(-1);
	
	ix=cix&31;
	bm=rgn->img_bmp[cix>>5];
	v=(bm>>(ix*2))&3;
	return(v);
}

int BTM_RegionAllocSetCell(
	BTM_World *wrl, BTM_Region *rgn,
	int cix, int val)
{
	u64 bm;
	int ix;
	
	if(cix<0)
		return(-1);
	if(cix>=rgn->img_ncell)
		return(-1);
	
	ix=cix&31;
	bm=rgn->img_bmp[cix>>5];
	bm&=~(3ULL<<(ix*2));
	bm|=((val&3ULL)<<(ix*2));
	rgn->img_bmp[cix>>5]=bm;
	return(0);
}

int BTM_RegionMarkAllocCellSpan(
	BTM_World *wrl, BTM_Region *rgn,
	int base, int cnt)
{
	int i, j;

	if((base<0) || (base>=rgn->img_ncell))
		{ __debugbreak(); }
	if((cnt<=0) || ((base+cnt)>rgn->img_ncell))
		{ __debugbreak(); }

	for(i=0; i<cnt; i++)
	{
		j=BTM_RegionAllocGetCell(wrl, rgn, base+i);
		if(j!=0)
			{ __debugbreak(); }
	}
	
	for(i=0; i<cnt; i++)
		BTM_RegionAllocSetCell(wrl, rgn, base+i, 2);
	BTM_RegionAllocSetCell(wrl, rgn, base, 1);
	return(0);
}

int BTM_RegionMarkFreeCellSpan(
	BTM_World *wrl, BTM_Region *rgn,
	int base, int cnt)
{
	int i, j;
	
	if((base<0) || (base>=rgn->img_ncell))
		{ __debugbreak(); }
	if((cnt<=0) || ((base+cnt)>rgn->img_ncell))
		{ __debugbreak(); }

	j=BTM_RegionAllocGetCell(wrl, rgn, base);
	if(j!=1)
		{ __debugbreak(); }

	for(i=1; i<cnt; i++)
	{
		j=BTM_RegionAllocGetCell(wrl, rgn, base+i);
		if(j!=2)
			{ __debugbreak(); }
	}
	
	for(i=0; i<cnt; i++)
		BTM_RegionAllocSetCell(wrl, rgn, base+i, 0);
	return(0);
}

int BTM_RegionFindFreeSpan(
	BTM_World *wrl, BTM_Region *rgn, int cnt)
{
	int i, j, k, v, n, no, sz;
	
	if(!rgn->img_dat)
		return(-1);
	
	n=rgn->img_ncell;
	
	i=256;
	while(i<n)
	{
		v=BTM_RegionAllocGetCell(wrl, rgn, i);
		if(v)
			{ i++; continue; }
			
		j=i+1;
		k=i+cnt;

		while(j<n)
		{
			v=BTM_RegionAllocGetCell(wrl, rgn, j);
			if(v || (j>=k))
				break;
			j++;
		}
		
		k=j-i;
		if(k>=cnt)
			return(i);
		i=j;
	}

	return(-1);
}

int BTM_RegionCheckExpand(
	BTM_World *wrl, BTM_Region *rgn, int newsz)
{
	byte *dat1;
	u64 *bm1;
	int i, j, k, v, n, no, sz;
	
	if(!rgn->img_dat)
	{
//		sz=262144;
		sz=131072;
//		rgn->img_dat=btm_malloc(sz);
		rgn->img_dat=BTM_AllocMul64K(sz>>16);
		rgn->img_sz=sz;

		n=(sz+15)/16;
		k=(n+31)/32;
//		rgn->img_bmp=btm_malloc(k*8);
		rgn->img_bmp=BTM_AllocMul256B((k*8)>>8);
		memset(rgn->img_bmp, 0, k*8);
		rgn->img_ncell=n;
		
		for(i=0; i<8; i++)
			rgn->img_bmp[i]=-1;
	}
	
	if(newsz>rgn->img_sz)
	{
		no=rgn->img_ncell;

//		sz=rgn->img_sz;
		sz=131072;
		while(newsz>sz)
		{
//			sz=sz+(sz>>1);
			sz=sz+65536;
		}
//		rgn->img_dat=btm_realloc(rgn->img_dat, sz);
		dat1=BTM_AllocMul64K(sz>>16);
		memcpy(dat1, rgn->img_dat, rgn->img_sz);
		BTM_FreeMul64K(rgn->img_dat, rgn->img_sz>>16);
		rgn->img_dat=dat1;
		rgn->img_sz=sz;

		n=(sz+15)/16;
		j=(no+31)/32;
		k=(n+31)/32;
//		rgn->img_bmp=btm_realloc(rgn->img_bmp, k*8);
		bm1=BTM_AllocMul256B((k*8)>>8);
		memset(bm1, 0, k*8);
		memcpy(bm1, rgn->img_bmp, j*8);
		BTM_FreeMul256B(rgn->img_bmp, (j*8)>>8);
		rgn->img_bmp=bm1;
		
		if((k*8)&255)
			{ debug_break }
		if((j*8)&255)
			{ debug_break }
		
//		memset(((byte *)rgn->img_bmp)+(j*8), 0, (k-j)*8);
		rgn->img_ncell=n;
	}
	
	return(0);
}

int BTM_RegionAllocSpan(
	BTM_World *wrl, BTM_Region *rgn, int cnt)
{
	int i, j, k, v, n, no, sz;
	
	BTM_RegionCheckExpand(wrl, rgn, 0);
	
	while(1)
	{
		i=BTM_RegionFindFreeSpan(wrl, rgn, cnt);
		if(i>0)
			break;

		sz=rgn->img_sz;
//		sz=sz+(sz>>1);
		sz=sz+(cnt<<4);
		BTM_RegionCheckExpand(wrl, rgn, sz);
	}

	BTM_RegionMarkAllocCellSpan(wrl, rgn, i, cnt);
	return(i);
}

int BTM_TryLoadRegionImage(BTM_World *wrl, BTM_Region *rgn)
{
	char tb[256];
	BCCX_Node *lent;
	byte *buf, *bmbuf, *sebuf, *lebuf;
	int sz, bmsz, sesz, lesz, zrgn;
	int rx, ry;
	int i, j, k, n;

	rx=(rgn->rgnix>>0)&511;
	ry=(rgn->rgnix>>9)&511;
	ry|=((rgn->rgnix>>18)&7)<<9;
	sprintf(tb, "region/rg%03X%03X.dat", ry, rx);
	printf("BTM_TryLoadRegionImage: %s\n", tb);

	sz=0;
	buf=BTM_LoadFileNp(tb, &sz);

	if(!buf)
	{
		sprintf(tb, "region/rgn%05X.dat", rgn->rgnix);
		printf("BTM_TryLoadRegionImage: %s\n", tb);
		buf=BTM_LoadFileNp(tb, &sz);
	}

	if(sz<8192)
		buf=NULL;

	if(!buf)
	{
		rgn->voxbm=BTM_WorldAllocSq(wrl, 18);
		memset(rgn->voxbm, 0, 128*128*128/8);
		for(i=0; i<512; i++)
			rgn->voxbmix[i]=i;

		rgn->magic1=BTM_MAGIC1;
		rgn->magic2=BTM_MAGIC1;
		rgn->magic3=BTM_MAGIC1;
		rgn->magic4=BTM_MAGIC1;
		rgn->magic5=BTM_MAGIC1;

		return(-1);
	}
	
	if(sz<8192)
		{ __debugbreak(); }
	
	rgn->img_dat=buf;
	rgn->img_sz=sz;
	
	n=(sz+15)/16;
	k=(n+31)/32;
//	rgn->img_bmp=btm_malloc(k*8);
	rgn->img_bmp=BTM_AllocMul256B((k*8+255)>>8);
	memset(rgn->img_bmp, 0, k*8);
	rgn->img_ncell=n;
	
	for(i=0; i<8; i++)
		rgn->img_bmp[i]=-1;
	
	for(i=0; i<(512+64); i++)
	{
		rgn->chk_ofsz[i]=get_u32le(buf+(i*4)+16);
		
		if(rgn->chk_ofsz[i]>=256)
		{
			BTM_RegionMarkAllocCellSpan(wrl, rgn,
				rgn->chk_ofsz[i]&0x000FFFFFU,
				rgn->chk_ofsz[i]>>20);
		}
	}
	
	bmsz=0;
	bmbuf=BTM_RegionDoLoadBufferIx(wrl, rgn, 0, 0, &bmsz);
	if(bmbuf)
	{
		if(bmsz==262144)
		{
			rgn->voxbm=BTM_WorldAllocSq(wrl, 18);
//			memset(rgn->voxbm, 0, 128*128*128/8);

			memcpy(rgn->voxbm, bmbuf, 128*128*128/8);
			for(i=0; i<512; i++)
				rgn->voxbmix[i]=i;
			rgn->voxbm_n=0;
		}else
		{
			if(bmsz>(1<<16))
				{ debug_break }
		
			rgn->voxbm=BTM_WorldAllocSq(wrl, 16);

			memcpy(rgn->voxbm, bmbuf, bmsz);
			memcpy(rgn->voxbmix, bmbuf, 1024);
			
			memset(rgn->voxbm+0, 0, 512);
			memset(rgn->voxbm+64, 255, 512);
			
			rgn->voxbm_n=bmsz>>9;
		}
	}

	zrgn=((rgn->rgnix*65521)>>16)&32767;

	sesz=0;
	sebuf=BTM_RegionDoLoadBufferIx(wrl, rgn, 1, 0, &sesz);
	if(sebuf)
	{
		rgn->static_ent_tree=BCCX_AbxeParseBuffer(sebuf, sesz);
//		BCCX_MarkTreeZone(root, BCCX_ZTY_GLOBAL);
		BCCX_MarkTreeZone(rgn->static_ent_tree, BCCX_ZTY_REGION|(zrgn<<8));
		
		BTM_SpawnRegionEntities(wrl, rgn, rgn->static_ent_tree);
	}

	lesz=0;
	lebuf=BTM_RegionDoLoadBufferIx(wrl, rgn, 2, 0, &lesz);
	if(lebuf)
	{
		lent=BCCX_AbxeParseBuffer(lebuf, lesz);
		BTM_SpawnRegionEntities(wrl, rgn, lent);
	}

	BCCX_ClearZoneLevel(BCCX_ZTY_REDUCE);

	rgn->magic1=BTM_MAGIC1;
	rgn->magic2=BTM_MAGIC1;
	rgn->magic3=BTM_MAGIC1;
	rgn->magic4=BTM_MAGIC1;
	rgn->magic5=BTM_MAGIC1;

	return(0);
}

int btm_stat_malloc_szregion;

int BTM_PrintAllocStats()
{
	int i, j, k, p2tot, tot;
	printf("Region Headers: %dK\n", btm_stat_malloc_szregion>>10);
	printf("BCCX Node: %dK\n", bccx_stat_sznode>>10);
	printf("BCCX NodeList: %dK\n", bccx_stat_sznodelist>>10);
	
	p2tot=0;
	for(i=4; i<20; i++)
	{
		if(!btm_stat_szallocsq[i])
			continue;
		printf("Pow2: %d %dK\n", i, btm_stat_szallocsq[i]>>10);
		p2tot+=btm_stat_szallocsq[i];
	}

	printf("Pow2: Tot %dK\n", p2tot>>10);
	
	tot=0;
	for(i=0; i<256; i++)
	{
		if(!btm_totalloc_mul64k[i])
			continue;
		printf("Mul64K: %d %dK\n", i, btm_totalloc_mul64k[i]>>10);
		tot+=btm_totalloc_mul64k[i];
	}

	for(i=0; i<256; i++)
	{
		if(!btm_totalloc_mul4k[i])
			continue;
		printf("Mul4K: %d %dK\n", i, btm_totalloc_mul4k[i]>>10);
		tot+=btm_totalloc_mul4k[i];
	}

	for(i=0; i<256; i++)
	{
		if(!btm_totalloc_mul256b[i])
			continue;
		printf("Mul256B: %d %dK\n", i, btm_totalloc_mul256b[i]>>10);
		tot+=btm_totalloc_mul256b[i];
	}

	printf("Mul: Tot %dK\n", tot>>10);

	return(0);
}

BTM_Region *BTM_AllocRegion(BTM_World *wrl)
{
	BTM_Region *rgn;
	
	rgn=wrl->free_region;
	if(rgn)
	{
		wrl->free_region=rgn->next;
		memset(rgn, 0, sizeof(BTM_Region));
		return(rgn);
	}

	rgn=btm_malloc(sizeof(BTM_Region));
	memset(rgn, 0, sizeof(BTM_Region));
	btm_stat_malloc_szregion+=sizeof(BTM_Region);
	return(rgn);
}

BTM_Region *BTM_LookupRegionForRix(BTM_World *wrl, int rix)
{
	BTM_Region *rgn;
	int h;

	rgn=wrl->rgn_guess;
	if(rgn && (rgn->rgnix==rix))
	{
//		BTM_ValidateRegionMagics(wrl, rgn);
		rgn->useflag|=2;
		return(rgn);
	}

	h=((rix*65521)>>16)&63;
	rgn=wrl->rgn_luhash[h];
	if(rgn && (rgn->rgnix==rix))
	{
//		BTM_ValidateRegionMagics(wrl, rgn);
		wrl->rgn_guess=rgn;
		rgn->useflag|=2;
		return(rgn);
	}
	
	rgn=wrl->region;
	while(rgn)
	{
		BTM_ValidateRegionMagics(wrl, rgn);
		if(rgn->rgnix==rix)
		{
			wrl->rgn_luhash[h]=rgn;
			wrl->rgn_guess=rgn;
			rgn->useflag|=2;
			return(rgn);
		}
		rgn=rgn->next;
	}
	
	return(NULL);
}

BTM_Region *BTM_GetRegionForRix(BTM_World *wrl, int rix)
{
	BTM_Region *rgn;
	int h;
	
	rgn=wrl->rgn_guess;
	if(rgn && (rgn->rgnix==rix))
	{
//		BTM_ValidateRegionMagics(wrl, rgn);
		rgn->useflag|=1;
		return(rgn);
	}
	
	h=((rix*65521)>>16)&63;
	rgn=wrl->rgn_luhash[h];
	if(rgn && (rgn->rgnix==rix))
	{
//		BTM_ValidateRegionMagics(wrl, rgn);
		wrl->rgn_guess=rgn;
		rgn->useflag|=1;
		return(rgn);
	}
	
	rgn=wrl->region;
	while(rgn)
	{
		BTM_ValidateRegionMagics(wrl, rgn);
		if(rgn->rgnix==rix)
		{
			wrl->rgn_luhash[h]=rgn;
			wrl->rgn_guess=rgn;
			rgn->useflag|=1;
			return(rgn);
		}
		rgn=rgn->next;
	}

//	rgn=malloc(sizeof(BTM_Region));
//	memset(rgn, 0, sizeof(BTM_Region));
//	stat_malloc_szregion+=sizeof(sizeof(BTM_Region));
	rgn=BTM_AllocRegion(wrl);
	
//	rgn->vox=malloc(128*128*128*4);
//	rgn->voxbm=malloc(128*128*128/8);
//	rgn->voxbm=BTM_WorldAllocSq(wrl, 18);
//	memset(rgn->vox, 0, 128*128*128*4);
//	memset(rgn->voxbm, 0, 128*128*128/8);
	
	rgn->rgnix=rix;
	rgn->next=wrl->region;
	wrl->region=rgn;

	BTM_TryLoadRegionImage(wrl, rgn);

	wrl->rgn_luhash[h]=rgn;
	wrl->rgn_guess=rgn;

	return(rgn);
}

int BTM_DestroyRegion(BTM_World *wrl, BTM_Region *rgn)
{
	u32 *voxa;
	byte *voxb, *voxh;
	int zrgn;
	int i, j, k;
	
	printf("BTM_DestroyRegion: %05X\n", rgn->rgnix);

	for(i=0; i<512; i++)
	{
		voxa=rgn->voxa[i];
		voxb=rgn->voxb[i];
		voxh=rgn->voxh[i];
		if(!voxa)
			continue;
		
		if(voxb || voxh)
		{
			BTM_WorldFreeSq(wrl, voxa, rgn->vox_m[i]);
			if(voxb)
				BTM_WorldFreeSq(wrl, voxb, 12);
			if(voxh)
				BTM_WorldFreeSq(wrl, voxh, 11);
		}else if(voxa!=(rgn->voxu+i))
		{
			BTM_WorldFreeSq(wrl, voxa, 14);
		}
	}

	zrgn=((rgn->rgnix*65521)>>16)&32767;
	BCCX_ClearZoneLevel(BCCX_ZTY_REGION|(zrgn<<8));

	if(rgn->voxbm_n)
		{ BTM_WorldFreeSq(wrl, rgn->voxbm, 16); }
	else
		{ BTM_WorldFreeSq(wrl, rgn->voxbm, 18); }

	if(rgn->img_dat)
	{
		BTM_FreeMul64K(rgn->img_dat, rgn->img_sz>>16);
	//	free(rgn->img_dat);
	//	free(rgn->img_bmp);
		BTM_FreeMul256B(rgn->img_bmp, ((rgn->img_ncell/32)*8)>>8);
	}
	
	rgn->next=wrl->free_region;
	wrl->free_region=rgn;
	
	return(0);
}

int BTM_WorldCheckMinDecBuf(BTM_World *wrl, int decsz)
{
	int i, j, k;
	
	if(decsz>wrl->lz_szdecbuf)
	{
		if(wrl->lz_tdecbuf)
		{
			btm_free(wrl->lz_tdecbuf);
			wrl->lz_tdecbuf=NULL;
		}
	}
	
	if(!wrl->lz_tdecbuf)
	{
		k=16384;
		while(decsz>=k)
			k=k+(k>>1);
		wrl->lz_tdecbuf=btm_malloc(k);
		wrl->lz_szdecbuf=k;
	}
	return(0);
}

int BTM_WorldCheckMinEncBuf(BTM_World *wrl, int esz)
{
	int i, j, k;
	
	if(esz>wrl->lz_szencbuf)
	{
		if(wrl->lz_tencbuf)
		{
			btm_free(wrl->lz_tencbuf);
			wrl->lz_tencbuf=NULL;
		}
	}
	
	if(!wrl->lz_tencbuf)
	{
		k=16384;
		while(esz>=k)
			k=k+(k>>1);
		wrl->lz_tencbuf=btm_malloc(k);
		wrl->lz_szencbuf=k;
	}
	return(0);
}

int BTM_WorldCheckMinEnc2Buf(BTM_World *wrl, int esz)
{
	int i, j, k;
	
	if(esz>wrl->lz_szenc2buf)
	{
		if(wrl->lz_tenc2buf)
		{
			btm_free(wrl->lz_tenc2buf);
			wrl->lz_tenc2buf=NULL;
		}
	}
	
	if(!wrl->lz_tenc2buf)
	{
		k=16384;
		while(esz>=k)
			k=k+(k>>1);
		wrl->lz_tenc2buf=btm_malloc(k);
		wrl->lz_szenc2buf=k;
	}
	return(0);
}

u32 BTM_RegionCheckRepackLight(u32 blk)
{
	if((((blk>>12)&15)==0) && (((blk>>16)&15)!=0))
	{
		blk=blk&0xFF03FFFF|((blk<<2)&0x00F00000U);
	}
	return(blk);
}

int BTM_RegionDoLoadChunkCix(BTM_World *wrl, BTM_Region *rgn, int ch)
{
	byte *dat, *cs;
	u32 *voxa;
//	u64 *voxbm;
	byte *voxb, *voxh;
	u64 bm;
	u32 tag, blk;
	int ofs, sz, dsz, cm, cix, dsz1;
	int mfl, nvox, mi;
	int i, j, k;
	
	if(!rgn->chk_ofsz[ch])
		return(-1);

	j=rgn->chk_ofsz[ch];
	ofs=(j&0x0FFFFF)*16;
	sz=(j>>20)*16;
	
	dat=rgn->img_dat+ofs;
	tag=get_u32le(dat);
	
	if(((tag>>28)&15)!=12)
	{
		printf("BTM_RegionDoLoadChunkCix: Discard, Bad Tag\n");
		rgn->chk_ofsz[ch]=0;
		return(-1);
	}
	
	dsz=tag&0x00FFFFFF;
	cm=(tag>>24)&15;
	
	BTM_WorldCheckMinDecBuf(wrl, dsz+256);
	
	if(cm==3)
	{
		dsz1=TgvLz_DecodeBufferRP2(dat+4, wrl->lz_tdecbuf, sz-4, dsz);
		if(dsz1!=dsz)
		{
			printf("BTM_RegionDoLoadChunkCix: RP2 Size Mismatch %d->%d\n",
				dsz, dsz1);
		}
	}else if(cm==4)
	{
		dsz1=TgvLz_DecodeBufferLZ4(dat+4, wrl->lz_tdecbuf, sz-4, dsz);
		if(dsz1!=dsz)
		{
			printf("BTM_RegionDoLoadChunkCix: LZ4 Size Mismatch %d->%d\n",
				dsz, dsz1);
		}
	}else if(cm==0)
	{
		k=sz-4;
		if(dsz<k)k=dsz;
		memcpy(wrl->lz_tdecbuf, dat+4, k);
	}
	
//	voxbm=rgn->voxbm;
	
	cs=wrl->lz_tdecbuf;
	mfl=*cs++;
	if(mfl&0x80)
		mfl|=(*cs++)<<8;
	
	nvox=*cs++;
	if(nvox==1)
	{
		voxa=rgn->voxu+ch;
		voxb=NULL;
		voxh=NULL;

		voxa[0]=get_u32le(cs);
		voxa[0]=BTM_RegionCheckRepackLight(voxa[0]);
		cs+=4;

		rgn->voxa[ch]=voxa;
		rgn->voxb[ch]=NULL;
		rgn->voxh[ch]=NULL;
		rgn->vox_n[ch]=1;
		rgn->vox_m[ch]=0;
	}else
		if(nvox)
	{
		mi=4;
		while(nvox>=(1<<mi))
			mi++;
		voxa=BTM_WorldAllocSq(wrl, mi+2);
		voxb=NULL;
		voxh=NULL;
		
		if(mfl&2)
			voxh=BTM_WorldAllocSq(wrl, 11);
		else
			voxb=BTM_WorldAllocSq(wrl, 12);
		
		for(i=0; i<nvox; i++)
		{
			voxa[i]=get_u32le(cs);
			voxa[i]=BTM_RegionCheckRepackLight(voxa[i]);
			cs+=4;
		}
		
		if(voxb)
		{
			memcpy(voxb, cs, 4096);
		}

		if(voxh)
			memcpy(voxh, cs, 2048);

		rgn->voxa[ch]=voxa;
		rgn->voxb[ch]=voxb;
		rgn->voxh[ch]=voxh;
		rgn->vox_n[ch]=nvox;
		rgn->vox_m[ch]=mi;
	}else
	{
		voxa=BTM_WorldAllocSq(wrl, 14);

		for(i=0; i<4096; i++)
		{
			voxa[i]=get_u32le(cs);
			voxa[i]=BTM_RegionCheckRepackLight(voxa[i]);
			cs+=4;
		}

		rgn->voxa[ch]=voxa;
		rgn->voxb[ch]=NULL;
		rgn->voxh[ch]=NULL;
		rgn->vox_n[ch]=0;
		rgn->vox_m[ch]=0;
	}

#if 0
	for(i=0; i<4096; i++)
	{
		cix=(ch<<12)|i;
		bm=voxbm[cix>>6];
		
		if(nvox==1)
			{ blk=voxa[0]; }
		else if(voxh)
			{ blk=voxa[(voxh[i>>1]>>((i&1)*4))&15]; }
		else if(voxb)
			{ blk=voxa[voxb[i]]; }
		else
			{ blk=voxa[i]; }

		if((blk&255)>=4)
			bm|=1ULL<<(i&63);
		else
			bm&=~(1ULL<<(i&63));
		voxbm[cix>>6]=bm;
	}
#endif

	return(0);
}

byte *BTM_RegionDoLoadBufferIx(BTM_World *wrl, BTM_Region *rgn,
	int bn, int ldfl, int *rsz)
{
	byte *dat, *cs;
	u32 *voxa;
	u64 *voxbm;
	byte *voxb, *voxh;
	u64 bm;
	u32 tag, blk;
	int ofs, sz, dsz, cm, cix, ch;
	int mfl, nvox, mi;
	int i, j, k;
	
	ch=bn+512;
	
	if(!rgn->chk_ofsz[ch])
		return(NULL);

	j=rgn->chk_ofsz[ch];
	ofs=(j&0x0FFFFF)*16;
	sz=(j>>20)*16;
	
	dat=rgn->img_dat+ofs;
	tag=get_u32le(dat);
	
	if(((tag>>28)&15)!=14)
	{
		printf("BTM_RegionDoLoadBufferIx: Discard, Bad Tag\n");
		rgn->chk_ofsz[ch]=0;
		return(NULL);
	}
	
	dsz=tag&0x00FFFFFF;
	cm=(tag>>24)&15;
	
	BTM_WorldCheckMinDecBuf(wrl, dsz+256);
	
	if(cm==3)
	{
		TgvLz_DecodeBufferRP2(dat+4, wrl->lz_tdecbuf, sz-4, dsz);
	}else if(cm==4)
	{
		TgvLz_DecodeBufferLZ4(dat+4, wrl->lz_tdecbuf, sz-4, dsz);
	}else if(cm==0)
	{
		k=sz-4;
		if(dsz<k)k=dsz;
		memcpy(wrl->lz_tdecbuf, dat+4, k);
	}
	
	if(ldfl&1)
	{
		rgn->etb_dat[bn]=btm_malloc(dsz+16);
		rgn->etb_sz[bn]=dsz;
		
		memcpy(rgn->etb_dat[bn], wrl->lz_tdecbuf, dsz);

		*rsz=dsz;
		return(rgn->etb_dat[bn]);
	}
	
	*rsz=dsz;
	return(wrl->lz_tdecbuf);
}


int BTM_RegionDoCompactChunkCix(BTM_World *wrl, BTM_Region *rgn, int ch)
{
	u32 tvxa[256];
	byte tvxb[4096];
	byte tvxh[2050];
	u32 *voxa;
	u64 *voxbm;
	byte *voxb, *voxh;
	u32 blk;
	u64 bm;
	int nvox, nv1, mi;
	int i, j, k;

	voxa=rgn->voxa[ch];
	voxb=rgn->voxb[ch];
	voxh=rgn->voxh[ch];
	nvox=rgn->vox_n[ch];
	mi=rgn->vox_m[ch];

	voxbm=rgn->voxbm+(rgn->voxbmix[ch]*64);

	if(!voxa)
		return(0);
	
	if(voxh)
	{
		for(i=0; i<4096; i++)
		{
			j=voxh[i>>1];
			j=(j>>((i&1)*4))&15;
			if(j>=nvox)
				break;
			blk=voxa[j];
			
			if(!(blk&255))
				break;
	
			bm=voxbm[i>>6];
			if(bm&(1ULL<<(i&63)))
			{
				if((blk&255)<4)
					break;
			}else
			{
				if((blk&255)>=4)
					break;
			}
		}
		
		if(i<4096)
		{
			printf("BTM_RegionDoCompactChunkCix: Discard Damaged Chunk H\n");
			rgn->voxa[ch]=NULL;
			rgn->voxb[ch]=NULL;
			rgn->voxh[ch]=NULL;
			rgn->vox_n[ch]=0;
			rgn->vox_m[ch]=0;
			return(0);
		}
	
		k=voxh[0];
		if((k&15)==(k>>4))
		{
			for(i=0; i<2048; i++)
				if(voxh[i]!=k)
					break;
			
			if(i>=2048)
			{
				blk=voxa[k&15];
				BTM_WorldFreeSq(wrl, voxa, 4+2);
				BTM_WorldFreeSq(wrl, voxh, 11);

				voxa=rgn->voxu+ch;
				voxa[0]=blk;

				rgn->voxa[ch]=voxa;
				rgn->voxb[ch]=NULL;
				rgn->voxh[ch]=NULL;
				rgn->vox_n[ch]=1;
				rgn->vox_m[ch]=0;

				return(1);
			}
		}
	}

	if(voxb)
	{
		for(i=0; i<4096; i++)
		{
			j=voxb[i];
			if(j>=nvox)
				break;

			blk=voxa[j];
			
			if(!(blk&255))
				break;
	
			bm=voxbm[i>>6];
			if(bm&(1ULL<<(i&63)))
			{
				if((blk&255)<4)
					break;
			}else
			{
				if((blk&255)>=4)
					break;
			}
		}
		
		if(i<4096)
		{
			printf("BTM_RegionDoCompactChunkCix: Discard Damaged Chunk B\n");
			rgn->voxa[ch]=NULL;
			rgn->voxb[ch]=NULL;
			rgn->voxh[ch]=NULL;
			rgn->vox_n[ch]=0;
			rgn->vox_m[ch]=0;
			return(0);
		}
	
		k=voxb[0];
		for(i=0; i<4096; i++)
			if(voxb[i]!=k)
				break;
		
		if(i>=4096)
		{
			blk=voxa[k];
			BTM_WorldFreeSq(wrl, voxa, mi+2);
			BTM_WorldFreeSq(wrl, voxb, 12);

			voxa=rgn->voxu+ch;
			voxa[0]=blk;

			rgn->voxa[ch]=voxa;
			rgn->voxb[ch]=NULL;
			rgn->voxh[ch]=NULL;
			rgn->vox_n[ch]=1;
			rgn->vox_m[ch]=0;

			return(1);
		}
	}
	
	if(voxb || voxh)
		return(0);
	
	if(voxa==(rgn->voxu+ch))
		return(0);

	if(rgn->chk_ofsz[ch])
		return(0);


	for(i=0; i<4096; i++)
	{
		blk=voxa[i];
		if(!(blk&255))
			break;
	
		bm=voxbm[i>>6];
		if(bm&(1ULL<<(i&63)))
		{
			if((blk&255)<4)
				break;
		}else
		{
			if((blk&255)>=4)
				break;
		}
	}
	
	if(i<4096)
	{
		printf("BTM_RegionDoCompactChunkCix: Discard Damaged Chunk A\n");
		rgn->voxa[ch]=NULL;
		rgn->voxb[ch]=NULL;
		rgn->voxh[ch]=NULL;
		rgn->vox_n[ch]=0;
		rgn->vox_m[ch]=0;
		return(0);
	}

	nv1=0;
	for(i=0; i<4096; i++)
	{
		blk=voxa[i];
		for(j=0; j<nv1; j++)
		{
			if(tvxa[j]==blk)
				break;
		}
		
		tvxh[(i+1)>>1]=0;
		
		if(j<nv1)
		{
			tvxb[i]=j;
			tvxh[i>>1]|=j<<((i&1)*4);
			continue;
		}
		
		if(nv1>=255)
			break;

		j=nv1++;
		tvxa[j]=blk;
		tvxb[i]=j;
		tvxh[i>>1]|=j<<((i&1)*4);
	}
	
	if(i<4096)
		return(-1);

	mi=4;
	while(nv1>(1<<mi))
		mi++;

	BTM_WorldFreeSq(wrl, voxa, 14);

	voxa=BTM_WorldAllocSq(wrl, mi+2);
	
	for(i=0; i<nv1; i++)
		{ voxa[i]=tvxa[i]; }

	if(nv1<16)
	{
		voxh=BTM_WorldAllocSq(wrl, 11);
		memcpy(voxh, tvxh, 2048);
	}else
	{
		voxb=BTM_WorldAllocSq(wrl, 12);
		memcpy(voxb, tvxb, 4096);
	}

	rgn->voxa[ch]=voxa;
	rgn->voxb[ch]=voxb;
	rgn->voxh[ch]=voxh;
	rgn->vox_n[ch]=nv1;
	rgn->vox_m[ch]=mi;

	return(1);
}

int BTM_RegionDoStoreChunkCix(BTM_World *wrl, BTM_Region *rgn, int ch)
{
	static TgvLz_Context *lzctx=NULL;

	u32 *voxa;
	byte *voxb, *voxh;
	byte *ct, *cts, *cts2;
	int nvox, sz1, sz2, bcel, ncel, mfl;
	int i, j, k;

	voxa=rgn->voxa[ch];
	voxb=rgn->voxb[ch];
	voxh=rgn->voxh[ch];
	nvox=rgn->vox_n[ch];
	
	if(!voxa)
	{
		return(0);
	}
	
	if(rgn->chk_ofsz[ch])
		return(0);

	BTM_RegionDoCompactChunkCix(wrl, rgn, ch);

	voxa=rgn->voxa[ch];
	voxb=rgn->voxb[ch];
	voxh=rgn->voxh[ch];
	nvox=rgn->vox_n[ch];
	
	if(!voxa)
	{
		printf("BTM_RegionDoStoreChunkCix: Chunk Disappeared\n");
		return(-2);
	}
	
	BTM_WorldCheckMinEncBuf(wrl, 48*1024);
	BTM_WorldCheckMinEnc2Buf(wrl, 48*1024);
	
	cts=wrl->lz_tencbuf;
	ct=cts+4;
	
	mfl=0;
	if(voxh)
		mfl|=2;
	
	*ct++=mfl;
	
	if((nvox==1) || (voxa==(rgn->voxu+ch)))
	{
		*ct++=1;
		set_u32le(ct, voxa[0]);
		ct+=4;
	}else
		if(voxb || voxh)
	{
		*ct++=nvox;
		for(i=0; i<nvox; i++)
		{
			set_u32le(ct, voxa[i]);
			ct+=4;
		}
		
		if(voxh)
		{
			memcpy(ct, voxh, 2048);
			ct+=2048;
		}else
		{
			memcpy(ct, voxb, 4096);
			ct+=4096;
		}
	}else
	{
		*ct++=0;
		for(i=0; i<4096; i++)
		{
			set_u32le(ct, voxa[i]);
			ct+=4;
		}
	}

	memset(ct, 0, 32);
	
	sz1=(ct-cts)-4;
	set_u32le(cts, 0xC0000000U+sz1);
	
	if(!lzctx)
	{
		lzctx=TgvLz_CreateContext();
	}
	
	cts2=wrl->lz_tenc2buf;
//	TgvLz_SetLevel(lzctx, 4);
	TgvLz_SetLevel(lzctx, 3);
//	sz2=TgvLz_DoEncode(lzctx, cts+4, cts2+4, sz1);
	sz2=TgvLz_DoEncodeSafe(lzctx, cts+4, cts2+4, sz1);
	set_u32le(cts2, 0xC3000000U+sz1);
	
	if(((sz2*1.25)<sz1) && (sz2>0))
	{
		memcpy(cts, cts2, sz2+8);
		sz1=sz2;
	}
	
	ncel=((sz1+8)+15)/16;
	bcel=BTM_RegionAllocSpan(wrl, rgn, ncel);
	
	cts2=rgn->img_dat+bcel*16;
	memcpy(cts2, cts, sz1+8);
	
	if((ncel>=4096) || (bcel>=1<<20))
		{ __debugbreak(); }
	
	rgn->chk_ofsz[ch]=bcel|(ncel<<20);
	return(0);
}

int BTM_RegionDoStoreBufferIx(BTM_World *wrl,
	BTM_Region *rgn, int ch, byte *ibuf, int isz)
{
	static TgvLz_Context *lzctx=NULL;

	byte *ct, *cts, *cts2;
	int nvox, sz1, sz2, bcel, ncel, mfl;
	int i, j, k;
	
	if(isz>(1<<18))
	{
		printf("BTM_RegionDoStoreBufferIx: Lg %d %dK\n", ch, isz>>10);
	}
	
	BTM_WorldCheckMinEncBuf(wrl, 1024+1.5*isz);
	BTM_WorldCheckMinEnc2Buf(wrl, 1024+1.5*isz);
	
	cts=wrl->lz_tencbuf;
	ct=cts+4;
	
	memcpy(ct, ibuf, isz);
	ct+=isz;

	memset(ct, 0, 32);

	sz1=(ct-cts)-4;
	set_u32le(cts, 0xE0000000U+sz1);
	
	if(!lzctx)
		lzctx=TgvLz_CreateContext();	
	cts2=wrl->lz_tenc2buf;
	TgvLz_SetLevel(lzctx, 4);
//	sz2=TgvLz_DoEncode(lzctx, cts+4, cts2+4, sz1);
	sz2=TgvLz_DoEncodeSafe(lzctx, cts+4, cts2+4, sz1);
	set_u32le(cts2, 0xE3000000U+sz1);
	
	if(((sz2*1.25)<sz1) && (sz2>0))
	{
		memcpy(cts, cts2, sz2+8);
		sz1=sz2;
	}

	if(rgn->chk_ofsz[512+ch])
	{
		BTM_RegionMarkFreeCellSpan(wrl, rgn,
			rgn->chk_ofsz[512+ch]&0x000FFFFFU,
			rgn->chk_ofsz[512+ch]>>20);
		memset(
			rgn->img_dat+(rgn->chk_ofsz[512+ch]&0x000FFFFFU)*16,
			0, 
			(rgn->chk_ofsz[512+ch]>>20)*16);
		rgn->chk_ofsz[512+ch]=0;
	}

	ncel=((sz1+8)+15)/16;
	bcel=BTM_RegionAllocSpan(wrl, rgn, ncel);
	
	cts2=rgn->img_dat+bcel*16;
	memcpy(cts2, cts, sz1+8);
	
	if((ncel>=4096) || (bcel>=1<<20))
		{ __debugbreak(); }
	
	rgn->chk_ofsz[512+ch]=bcel|(ncel<<20);
	return(0);
}

int BTM_ValidateRegionMagics(BTM_World *wrl, BTM_Region *rgn)
{
	if(rgn->img_dat && memcmp(rgn->img_dat, "BTMRGN01", 8))
		{ debug_break }
	
	if(rgn->magic1!=BTM_MAGIC1)
		{ debug_break }
	if(rgn->magic2!=BTM_MAGIC1)
		{ debug_break }
	if(rgn->magic3!=BTM_MAGIC1)
		{ debug_break }
	if(rgn->magic4!=BTM_MAGIC1)
		{ debug_break }
	if(rgn->magic5!=BTM_MAGIC1)
		{ debug_break }
	
	return(0);
}

int BTM_FlattenRegion(BTM_World *wrl, BTM_Region *rgn)
{
	static byte *tentbuf=NULL;
	char tb[256];
	BCCX_Node *tnode;
	byte *buf, *cs;
	u16 *bmci;
	u64 *bmcs, *bmct;
	int rx, ry;
	int sz, tesz;
	int i, j, k, n;
	
// #ifdef BTM_TARGET_SMALL
//	return(0);		//BGB: Disable saving for now.
// #endif
	
//	if(!(rgn->dirty&1))
	if(!(rgn->dirty&1) && !(rgn->dirty&(16|32)))
		return(0);

	/* New region, not yet visited. */
	if((rgn->dirty&4) && !(rgn->dirty&2))
		return(0);
	
	if(!rgn->voxa[0] && !rgn->chk_ofsz[0])
	{
		/* Don't save if still void. */
		return(0);
	}
	
	BTM_ValidateRegionMagics(wrl, rgn);
	
	if(rgn->dirty&1)
	{
		rgn->rgnvseq++;
	
		for(i=0; i<512; i++)
		{
			k=BTM_RegionDoStoreChunkCix(wrl, rgn, i);
			if(k<0)
			{
				printf("BTM_FlattenRegion: Damaged Chunks, Abort\n");
				return(-1);
			}
			
			rgn->chk_vseq[i]=rgn->rgnvseq;
		}

		if(!tentbuf)
			tentbuf=btm_malloc(65536);

		if(rgn->voxbm_n>0)
		{
			n=rgn->voxbm_n;
			memcpy(tentbuf+1024, rgn->voxbm+128, n*512);
			memcpy(tentbuf, rgn->voxbmix, 1024);
			BTM_RegionDoStoreBufferIx(wrl, rgn, 0,
				(void *)tentbuf, n*512);
		}else
		{
			bmci=(u16 *)tentbuf;
			bmct=(u64 *)tentbuf;
			bmct+=128;

			n=2; bmcs=rgn->voxbm;
			for(i=0; (i<512) && (n<128); i++)
			{
				for(j=0; j<64; j++)
					if(bmcs[j]!=0x0000000000000000ULL)
						break;
				if(j>=64)
				{
					bmci[i]=0;
					bmcs+=64;
					continue;
				}
				for(j=0; j<64; j++)
					if(bmcs[j]!=0xFFFFFFFFFFFFFFFFULL)
						break;
				if(j>=64)
				{
					bmci[i]=1;
					bmcs+=64;
					continue;
				}

				j=n++;
				memcpy(bmct, bmcs, 64*8);
				bmct+=64;
				bmcs+=64;
				bmci[i]=j;
			}
			
			if(i>=512)
			{
				BTM_RegionDoStoreBufferIx(wrl, rgn, 0,
					(void *)tentbuf, n*512);
			}else
			{
		//		printf("Collapsible: %d/%d\n", n, 512);
			
				BTM_RegionDoStoreBufferIx(wrl, rgn, 0,
					(void *)(rgn->voxbm), 128*128*128/8);
			}
		}
	}

	if(rgn->dirty&16)
	{
		if(rgn->static_ent_tree)
		{
			if(!tentbuf)
				tentbuf=btm_malloc(65536);
			tesz=BCCX_AbxeEncodeNodeBuffer(
				rgn->static_ent_tree, tentbuf, 65536);
			BTM_RegionDoStoreBufferIx(wrl, rgn, 1,
				tentbuf, tesz);
		}
	}

	if(rgn->dirty&32)
	{
		tnode=BTM_FlattenRegionLiveEntities(wrl, rgn);
		if(tnode)
		{
			if(!tentbuf)
				tentbuf=btm_malloc(65536);
			tesz=BCCX_AbxeEncodeNodeBuffer(
				tnode, tentbuf, 65536);
			BTM_RegionDoStoreBufferIx(wrl, rgn, 2,
				tentbuf, tesz);
		}
	}

	if(rgn->dirty&(16|32))
	{
		BCCX_ClearZoneLevel(BCCX_ZTY_REDUCE);
	}


	rgn->dirty=0;
		
	buf=rgn->img_dat;
	memcpy(buf, "BTMRGN01", 8);
	set_u32le(buf+12, rgn->img_ncell*16);

	for(i=0; i<(512+64); i++)
	{
		set_u32le(buf+(i*4)+16, rgn->chk_ofsz[i]);
	}
	
	i=rgn->img_ncell;
//	while(i>4096)
	while(i>8192)
	{
		j=BTM_RegionAllocGetCell(wrl, rgn, i-1);
		if(j)break;
		i--;
	}
	
	j=8192;
	while(i>j)
	{
//		j=j+(j>>1);
		j=j+4096;
	}
	
//	rgn->img_lnzcell=i;
	rgn->img_lnzcell=j;

	rx=(rgn->rgnix>>0)&511;
	ry=(rgn->rgnix>>9)&511;
	ry|=((rgn->rgnix>>18)&7)<<9;
	sprintf(tb, "region/rg%03X%03X.dat", ry, rx);
//	sprintf(tb, "region/rgn%05X.dat", rgn->rgnix);

	printf("BTM_FlattenRegion: %s\n", tb);

	sz=rgn->img_lnzcell*16;
	BTM_StoreFile(tb, buf, sz);
	
	return(0);
}


int BTM_LoadWorldGlobals(BTM_World *wrl)
{
	BCCX_Node *lent;
	byte *ebuf;
	int esz;

	esz=NULL;
//	ebuf=BTM_RegionDoLoadBufferIx(wrl, rgn, 2, 0, &lesz);
	ebuf=BTM_LoadFileTmp("region/global.dat", &esz);

	if(ebuf)
	{
		lent=BCCX_AbxeParseBuffer(ebuf, esz);
		BTM_SpawnWorldGlobalState(wrl, lent);
	}
	
	return(0);
}

int BTM_CheckUnloadRegions(BTM_World *wrl)
{
	static byte *tentbuf;
	BTM_Region *rcur, *rnxt, *rulst, *rklst;
	BCCX_Node *tnode;
	u64 rpos;
	int cx, cy, vx, vy, dx, dy, d, tesz, rt, uld;

#ifdef BTM_TARGET_SMALL
	return(0);		//BGB: Disable saving for now.
#endif

	vx=(wrl->cam_org>> 8)&0xFFFF;
	vy=(wrl->cam_org>>32)&0xFFFF;

	tnode=BTM_FlattenWorldGlobalState(wrl);
	if(tnode)
	{
		if(!tentbuf)
			tentbuf=btm_malloc(65536);
		tesz=BCCX_AbxeEncodeNodeBuffer(
			tnode, tentbuf, 65536);
//		BTM_RegionDoStoreBufferIx(wrl, rgn, 2,
//			tentbuf, tesz);

		BTM_StoreFile("region/global.dat", tentbuf, tesz);

		BCCX_ClearZoneLevel(BCCX_ZTY_REDUCE);
	}

//	uld=(2*btm_drawdist);
	uld=(1.5*btm_drawdist);
	if(uld<192)
		uld=192;

	rulst=NULL;
	rklst=NULL;
	rcur=wrl->region;
	while(rcur)
	{
		BTM_ValidateRegionMagics(wrl, rcur);
		rt=BTM_FlattenRegion(wrl, rcur);

		BTM_ValidateRegionMagics(wrl, rcur);

//		rpos=BTM_ConvRixToBlkPos(rcur->rgnix);
		rpos=BTM_ConvRixToBlkPosCenter(rcur->rgnix);
		cx=(rpos>> 0)&65535;
		cy=(rpos>>16)&65535;

		dx=(s16)(vx-cx);
		dy=(s16)(vy-cy);
		dx=dx^(dx>>31);
		dy=dy^(dy>>31);
//		d=dx+dy;
		if(dx>dy)
			d=dx+(dy>>1);
		else
			d=dy+(dx>>1);

//		if(d>=384)
//		if(d>=192)
//		if((d>=244) || (rt<0))
//		if((d>=uld) || (rt<0))
		if(((d>=uld) && !(rcur->useflag&1)) || (rt<0))
//		if(d>=448)
//		if(d>=512)
		{
			rcur->unext=rulst;
			rulst=rcur;
		}else
		{
			rcur->unext=rklst;
			rklst=rcur;
		}

		rcur=rcur->next;
	}
	
	if(!rulst)
		return(0);
	
	for(d=0; d<64; d++)
		wrl->rgn_luhash[d]=NULL;
	wrl->rgn_guess=NULL;
	
	wrl->region=NULL;
	rcur=rklst;
	while(rcur)
	{
		BTM_ValidateRegionMagics(wrl, rcur);
		rcur->useflag&=~7;
		rcur->next=wrl->region;
		wrl->region=rcur;
		rcur=rcur->unext;
	}

	rcur=rulst;
	while(rcur)
	{
		rnxt=rcur->unext;
//		BTM_FlattenRegion(wrl, rcur);
		BTM_DestroyRegion(wrl, rcur);
		rcur=rnxt;
	}
	
	return(1);
}

u32 BTM_GetRegionBlockCix(BTM_World *wrl, BTM_Region *rgn, int cix)
{
	u32 *vox;
	byte *voxb, *voxh;
	u32 blk;
	int ch, vi;
	int i, j, k;

	if(cix>>21)
		{ __debugbreak(); }

	ch=(cix>>12)&511;
	vox=rgn->voxa[ch];
	if(!vox)
	{
		if(!rgn->chk_ofsz[ch])
			return(0);
		
		j=rgn->chk_ofsz[ch];
		if(j<256)
		{
			blk=j;
			return(blk);
		}
		
		BTM_RegionDoLoadChunkCix(wrl, rgn, ch);
		vox=rgn->voxa[ch];
		if(!vox)
			return(0);
	}
	
	if(vox==(rgn->voxu+ch))
	{
		return(vox[0]);
	}

	voxb=rgn->voxb[ch];
	voxh=rgn->voxh[ch];

	if(voxh)
	{
		vi=voxh[(cix>>1)&2047];
		vi=(vi>>((cix&1)*4))&15;
		blk=vox[vi];
		return(blk);
	}

	if(voxb)
	{
		vi=voxb[cix&4095];
		blk=vox[vi];
		return(blk);
	}

	blk=vox[cix&4095];
//	blk=rgn->vox[cix];
	return(blk);
}

int BTM_RegionUnpackVoxbm(BTM_World *wrl, BTM_Region *rgn)
{
	u64 *bm1;
	int i, j, k;
	
	if(!rgn->voxbm_n)
		return(0);
	
	bm1=BTM_WorldAllocSq(wrl, 18);
	for(i=0; i<512; i++)
	{
		j=rgn->voxbmix[i];
		memcpy(bm1+i*64, rgn->voxbm+j*64, 512);
	}

	BTM_WorldFreeSq(wrl, rgn->voxbm, 16);
	rgn->voxbm=bm1;
	rgn->voxbm_n=0;
	for(i=0; i<512; i++)
		rgn->voxbmix[i]=i;
	return(0);
}

int BTM_SetRegionBlockCix(BTM_World *wrl,
	BTM_Region *rgn, int cix, u32 blk)
{
	u32 *voxa, *vox1;
	byte *voxb, *voxh;
	u64 bm;
	int ch, n, m, mi, dn;
	int i, j, k;

	if(cix>>21)
		{ __debugbreak(); }

	ch=(cix>>12)&511;
	voxa=rgn->voxa[ch];

	if(!voxa && rgn->chk_ofsz[ch])
	{
		BTM_RegionDoLoadChunkCix(wrl, rgn, ch);
		voxa=rgn->voxa[ch];
	}

	if(!voxa)
	{
//		voxa=(rgn->voxu+ch);
		voxa=BTM_WorldAllocSq(wrl, 4+2);
		voxh=BTM_WorldAllocSq(wrl, 11);

		voxa[0]=blk;
		
		memset(voxh, 0, 2048);

		rgn->voxa[ch]=voxa;
		rgn->voxb[ch]=NULL;
		rgn->voxh[ch]=voxh;
		rgn->vox_n[ch]=1;
		rgn->vox_m[ch]=4;

		return(0);
	}

	if(voxa==(rgn->voxu+ch))
	{
		if(blk==voxa[0])
			return(0);
		voxa=NULL;
	}

	if(!voxa)
	{
		voxa=BTM_WorldAllocSq(wrl, 14);

//		memset(voxa, 0, 1<<14);
		for(i=0; i<4096; i++)
			voxa[i]=rgn->voxu[ch];

		rgn->voxa[ch]=voxa;
		rgn->voxb[ch]=NULL;
		rgn->voxh[ch]=NULL;
		rgn->vox_n[ch]=0;
		rgn->vox_m[ch]=0;
	}

	if(rgn->chk_ofsz[ch]>=256)
	{
		BTM_RegionMarkFreeCellSpan(wrl, rgn,
			rgn->chk_ofsz[ch]&0x000FFFFFU,
			rgn->chk_ofsz[ch]>>20);
		memset(
			rgn->img_dat+(rgn->chk_ofsz[ch]&0x000FFFFFU)*16,
			0, 
			(rgn->chk_ofsz[ch]>>20)*16);
	}
	rgn->chk_ofsz[ch]=0;
	
	rgn->dirty|=1;

	voxb=rgn->voxb[ch];
	voxh=rgn->voxh[ch];
	dn=0;

	if(voxh)
	{
		n=rgn->vox_n[ch];
		mi=rgn->vox_m[ch];
//		m=1<<mi;
//		if(m>=256)
//			m=255;

		for(i=0; i<n; i++)
		{
			if(voxa[i]==blk)
				break;
		}
		
		if((i>=n) && (n<15))
		{
			i=n++;
			rgn->vox_n[ch]=n;
			voxa[i]=blk;
		}
		
		if(i<n)
		{
			j=voxh[(cix>>1)&2047];
			if(cix&1)
				{ j=(j&0x0F)|((i<<4)&0xF0); }
			else
				{ j=(j&0xF0)|((i<<0)&0x0F); }
			voxh[(cix>>1)&2047]=j;
			dn=1;
		}else
		{
#if 0
			vox1=BTM_WorldAllocSq(wrl, 14);
			
			for(i=0; i<4096; i++)
				vox1[i]=voxa[(voxh[i/2]>>((i&1)*4))&15];
			vox1[cix&4095]=blk;
			
			BTM_WorldFreeSq(wrl, voxa, mi+2);
			BTM_WorldFreeSq(wrl, voxh, 11);
			
			rgn->voxa[ch]=vox1;
			rgn->voxb[ch]=NULL;
			rgn->voxh[ch]=NULL;
			rgn->vox_n[ch]=0;
			rgn->vox_m[ch]=0;
			
			voxa=vox1;
			voxb=NULL;
			voxh=NULL;
#endif

#if 1
			vox1=BTM_WorldAllocSq(wrl, 8+2);
			voxb=BTM_WorldAllocSq(wrl, 12);

			for(i=0; i<16; i++)
				vox1[i]=voxa[i];
			for(i=0; i<4096; i++)
				voxb[i]=(voxh[i/2]>>((i&1)*4))&15;
			
			i=n++;
			vox1[i]=blk;

			voxb[cix&4095]=i;
			
//			for(i=0; i<4096; i++)
//				vox1[i]=voxa[(voxh[i/2]>>((i&1)*4))&15];
//			vox1[cix&4095]=blk;
			
			BTM_WorldFreeSq(wrl, voxa, mi+2);
			BTM_WorldFreeSq(wrl, voxh, 11);
			
			rgn->voxa[ch]=vox1;
			rgn->voxb[ch]=voxb;
			rgn->voxh[ch]=NULL;
			rgn->vox_n[ch]=n;
			rgn->vox_m[ch]=8;
			
			voxa=vox1;
			voxh=NULL;

			dn=1;
#endif
		}
	}
	
	if(!dn && voxb)
	{
		n=rgn->vox_n[ch];
		mi=rgn->vox_m[ch];
		m=1<<mi;
		if(m>=256)
			m=255;
		
		for(i=0; i<n; i++)
		{
			if(voxa[i]==blk)
				break;
		}
		
		if(i>=n)
		{
			if((n>=m) && (n<255) && (m<255))
			{
				vox1=BTM_WorldAllocSq(wrl, 8+2);
				memcpy(vox1, voxa, m*4);
				m=255;

				BTM_WorldFreeSq(wrl, voxa, mi+2);
				rgn->voxa[ch]=vox1;
				rgn->vox_m[ch]=8;
				voxa=vox1;
			}
		
			if(n<m)
			{
				i=n++;
				rgn->vox_n[ch]=n;
				voxa[i]=blk;
			}
		}
		
		if(i<n)
		{
			voxb[cix&4095]=i;
			dn=1;
		}else
		{
			vox1=BTM_WorldAllocSq(wrl, 14);
			
			for(i=0; i<4096; i++)
				vox1[i]=voxa[voxb[i]];
			vox1[cix&4095]=blk;
			
			BTM_WorldFreeSq(wrl, voxa, mi+2);
			BTM_WorldFreeSq(wrl, voxb, 12);
			
			rgn->voxa[ch]=vox1;
			rgn->voxb[ch]=NULL;
			rgn->vox_n[ch]=0;
			rgn->vox_m[ch]=0;
			
			voxa=vox1;
			voxb=NULL;
		}
	}
	
	if(!dn && !voxb && !voxh)
	{
		voxa[cix&4095]=blk;
	}

	if(rgn->voxbm_n)
	{
		i=rgn->voxbmix[cix>>12];
		if(i<2)
		{
			if((i==0) && ((blk&255)<4))
				return(0);
			if((i==1) && ((blk&255)>=4))
				return(0);
				
			if(rgn->voxbm_n<127)
			{
				j=rgn->voxbm_n++;
				memcpy(rgn->voxbm+(j*64), rgn->voxbm+(i*64), 512);
				rgn->voxbmix[cix>>12]=j;
				i=j;
			}else
			{
				BTM_RegionUnpackVoxbm(wrl, rgn);
				i=cix>>12;
			}
		}

		j=(i<<6)|((cix>>6)&63);
		bm=rgn->voxbm[j];
		if((blk&255)>=4)
			bm|=1ULL<<(cix&63);
		else
			bm&=~(1ULL<<(cix&63));
		rgn->voxbm[j]=bm;
	}else
	{
		bm=rgn->voxbm[cix>>6];
		if((blk&255)>=4)
			bm|=1ULL<<(cix&63);
		else
			bm&=~(1ULL<<(cix&63));
		rgn->voxbm[cix>>6]=bm;
	}

	return(0);
}
