/*
Load PE/PEL4 PBO Images.

PBO allows multiple program instances to exist within a single address space.
Images are loaded with base relocations applied, with data sections accessed relative to GBR.

Each image will have an index, which will be used to find its associated data section (given GBR pointing to another data section within the same process).
Every data section within a process will point to every other data section within the process.

 */

typedef void X3PE_FILE;

char **tk_env_pathlst;
int tk_env_npathlst;

int x3pe_npboix=1;
int x3pe_nimgix=1;

X3PE_ImageInfo	*x3pe_pbo_image[256];
X3PE_ImageInfo	*x3pe_pbo_dllimg[256];

int X3PE_TryLoadProgramImage(char *imgname, char *cwd, int isdll);
x3vm_addr X3PE_LookupImageDllExport(X3PE_ImageInfo *img, char *name);
x3vm_addr X3PE_LookupImageElfExport(X3PE_ImageInfo *img, char *name);


int TK_Env_GetLibPathList(char ***rlst, int *rnlst);
X3PE_ImageInfo *X3PE_GetImageForIndex(int ix);

char *bccx_strdup(char *str);
char *x3vm_strdup(char *str);

X3VM_Context *x3pe_vmctx;
int x3pe_tlsgbl=1;

void *x3vm_vfsstdio_vfsOpen(X3VM_VfsGlue *ctx, char *name, char *mode)
{
	char tbuf[512];
	FILE *fd;
	
	if(ctx->uptr0)
	{
		sprintf(tbuf, "%s/%s", (char *)ctx->uptr0, name);
		fd=fopen(tbuf, mode);
	}else
	{
		fd=fopen(name, mode);
	}
	return(fd);
}

void x3vm_vfsstdio_vfsClose(X3VM_VfsGlue *ctx, void *fd)
{
	fclose(fd);
}

s64 x3vm_vfsstdio_vfsSeek(X3VM_VfsGlue *ctx,
	void *fd, s64 pos, int rel)
{
	fseek(fd, pos, rel);
	return(ftell(fd));
}

s64 x3vm_vfsstdio_vfsRead(X3VM_VfsGlue *ctx,
	void *buf, s64 sz1, s64 sz2, void *fd)
{
	return(fread(buf, sz1, sz2, fd));
}

s64 x3vm_vfsstdio_vfsWrite(X3VM_VfsGlue *ctx,
	void *buf, s64 sz1, s64 sz2, void *fd)
{
	return(fwrite(buf, sz1, sz2, fd));
}

X3VM_VfsGlueVt x3vm_vfsglue_stdio_vt = {
NULL,
NULL,
NULL,
NULL,
x3vm_vfsstdio_vfsOpen,
x3vm_vfsstdio_vfsClose,
x3vm_vfsstdio_vfsSeek,
x3vm_vfsstdio_vfsRead,
x3vm_vfsstdio_vfsWrite
};

X3VM_VfsGlue x3vm_vfsglue_stdio = {
&x3vm_vfsglue_stdio_vt,
NULL,
NULL,
NULL,
NULL
};



int X3PE_AllocNewTlsGbl()
{
	return(x3pe_tlsgbl++);
}

int X3PE_SetVfsBasePath(char *str)
{
	X3VM_VfsGlue *vfs;
	vfs=&x3vm_vfsglue_stdio;
	vfs->uptr0=x3vm_strdup(str);
	return(0);
}

int X3PE_BindVM(X3VM_Context *ctx)
{
	X3VM_VfsGlue *vfs;

	x3pe_vmctx=ctx;

	vfs=ctx->vfs;
	if(!vfs)
	{
		vfs=&x3vm_vfsglue_stdio;
		ctx->vfs=vfs;
	}

	return(0);
}

int x3pe_fseek(X3PE_FILE *fd, int ofs, int set)
{
	X3VM_VfsGlue *vfs;
	vfs=x3pe_vmctx->vfs;

//	fseek(fd, ofs, set);
	vfs->vt->vfsSeek(vfs, fd, ofs, set);
	return(0);
}

X3PE_FILE *x3pe_fopen(char *name, char *mode)
{
	X3PE_FILE *fd;
	X3VM_VfsGlue *vfs;
	vfs=x3pe_vmctx->vfs;
//	fd=fopen(name, mode);
	fd=vfs->vt->vfsOpen(vfs, name, mode);
	return(fd);
}

int x3pe_fclose(X3PE_FILE *fd)
{
	X3VM_VfsGlue *vfs;
	vfs=x3pe_vmctx->vfs;
//	fclose(fd);
	vfs->vt->vfsClose(vfs, fd);
	return(0);
}

int x3pe_fread(void *buf, int sz1, int sz2, X3PE_FILE *fd)
{
	int l;
	X3VM_VfsGlue *vfs;
	vfs=x3pe_vmctx->vfs;
//	l=fread(buf, sz1, sz2, fd);
	l=vfs->vt->vfsRead(vfs, buf, sz1, sz2, fd);
	return(l);
}

int x3pe_fgetc(X3PE_FILE *fd)
{
	byte buf[8];
	int l;
	l=x3pe_fread(buf, 1, 1, fd);
	if(l<1)
		return(-1);
	return(buf[0]);
}

int x3pe_fgets(char *buf, int msz, X3PE_FILE *fd)
{
	int c, n;
	memset(buf, 0, msz);
//	fgets(buf, msz, fd);

	n=0;
	while(n<msz)
	{
		c=x3pe_fgetc(fd);
		if(c<0)
			break;
		buf[n++]=c;
		if(c=='\n')
			break;
	}
	return(strlen(buf));
}

u16 x3pe_vmgetu16(x3vm_addr addr)
{
	return(X3VM_MemLoadU16(x3pe_vmctx, addr));
}

u32 x3pe_vmgetu32(x3vm_addr addr)
{
	return(X3VM_MemLoadU32(x3pe_vmctx, addr));
}

u64 x3pe_vmgetu64(x3vm_addr addr)
{
	return(X3VM_MemLoadU64(x3pe_vmctx, addr));
}

s64 x3pe_vmgets64(x3vm_addr addr)
{
	return(X3VM_MemLoadS64(x3pe_vmctx, addr));
}

int x3pe_vmsetu8(x3vm_addr addr, byte val)
{
	X3VM_MemStoreU8(x3pe_vmctx, addr, val);
	return(0);
}

int x3pe_vmsetu16(x3vm_addr addr, u16 val)
{
	X3VM_MemStoreU16(x3pe_vmctx, addr, val);
	return(0);
}

int x3pe_vmsetu32(x3vm_addr addr, u32 val)
{
	X3VM_MemStoreU32(x3pe_vmctx, addr, val);
	return(0);
}

int x3pe_vmsetu64(x3vm_addr addr, u64 val)
{
	X3VM_MemStoreU64(x3pe_vmctx, addr, val);
	return(0);
}

int x3pe_vmmemcpyin(x3vm_addr dst, void *src, int sz)
{
	X3VM_VMemcpyIn(x3pe_vmctx, dst, src, sz);
	return(0);
}

int x3pe_vmmemcpyout(void *dst, x3vm_addr src, int sz)
{
	X3VM_VMemcpyOut(x3pe_vmctx, dst, src, sz);
	return(0);
}

int x3pe_vmmemcpy(x3vm_addr dst, x3vm_addr src, int sz)
{
	X3VM_VMemcpy(x3pe_vmctx, dst, src, sz);
	return(0);
}

int x3pe_vmmemset(x3vm_addr dst, int val, int sz)
{
	X3VM_VMemset(x3pe_vmctx, dst, val, sz);
	return(0);
}

char *x3pe_vmgetstring(x3vm_addr addr)
{
	char tbuf[1024];
	char *p0, *p1, *pcs;

	p0=X3VM_MemMapAddress(x3pe_vmctx, addr+   0, 0);
	p1=X3VM_MemMapAddress(x3pe_vmctx, addr+1024, 0);
	if(p0 && p1 && ((p1-p0)==1024))
		return(p0);

//	p0=X3VM_MemMapAddress(x3pe_vmctx, addr+   0, 0);
	p1=X3VM_MemMapAddress(x3pe_vmctx, addr+256, 0);
	if(p0 && p1 && ((p1-p0)==256))
	{
		pcs=p0;
		while((pcs<p1) && *pcs) pcs++;
		if(pcs<p1)
			return(p0);
	}

//	p0=X3VM_MemMapAddress(x3pe_vmctx, addr+   0, 0);
	p1=X3VM_MemMapAddress(x3pe_vmctx, addr+64, 0);
	if(p0 && p1 && ((p1-p0)==64))
	{
		pcs=p0;
		while((pcs<p1) && *pcs) pcs++;
		if(pcs<p1)
			return(p0);
	}

	X3VM_VMemcpyOut(x3pe_vmctx, tbuf, addr, 1024);
	return(x3vm_strdup(tbuf));
}

int X3PE_GetRandom16ASLR()
{
	return(X3VM_RngGblRand16());
}

x3vm_addr X3PE_UnpackBufferL4(x3vm_addr dst, byte *ibuf, int isz)
{
	byte *cs, *cse;
	x3vm_addr ct;
	int tg, lr, ll, ld;
	int i;

	tg=0; lr=0; ll=0; ld=0;
	
	ct=dst;
	cs=ibuf; cse=ibuf+isz;
	while(cs<cse)
	{
		tg=*cs++;
		lr=(tg>>4)&15;
		if(lr==15)
		{
			i=*cs++;
			while(i==255)
				{ lr+=255; i=*cs++; }
			lr+=i;
		}

		x3pe_vmmemcpyin(ct, cs, lr);
		ct+=lr; cs+=lr;
		
		if((cs+1)>=cse)
			break;

		ll=(tg&15)+4;
		
//		ld=tkfat_getWord(cs);
		ld=*(u16 *)cs;
		cs+=2;
		if(!ld)
		{
			if(ll==5)
				continue;
			break;
		}
		if(ll==19)
		{
			i=*cs++;
			while(i==255)
				{ ll+=255; i=*cs++; }
			ll+=i;
		}

		x3pe_vmmemcpy(ct, ct-ld, ll);
		ct+=ll;
	}
	
	return(ct);
}

x3vm_addr X3PE_UnpackBuffer(x3vm_addr ct, void *tbuf, int sz, int cmp)
{
	if(cmp==4)
	{
		return(X3PE_UnpackBufferL4(ct, tbuf, sz));
	}
	
	return(0);
}

u32 X3PE_CalculateImagePel4BChecksum(x3vm_addr buf, int size)
{
	x3vm_addr cs, cse;
	u32 v, v0, v1, v2, v3;
	u64 acc_lo, acc_hi;
	u32 csum;
	
	cs=buf;
	cse=cs+size;
	acc_lo=1;
	acc_hi=0;

	while(cs<cse)
	{
		v0=x3pe_vmgetu32(cs+0);	v1=x3pe_vmgetu32(cs+ 4);
		v2=x3pe_vmgetu32(cs+8);	v3=x3pe_vmgetu32(cs+12);
		acc_lo=acc_lo+v0;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v1;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v2;	acc_hi=acc_hi+acc_lo;
		acc_lo=acc_lo+v3;	acc_hi=acc_hi+acc_lo;
		cs+=16;
	}

	acc_lo=((u32)acc_lo)+(acc_lo>>32);
	acc_lo=((u32)acc_lo)+(acc_lo>>32);
	acc_hi=((u32)acc_hi)+(acc_hi>>32);
	acc_hi=((u32)acc_hi)+(acc_hi>>32);
	csum=(u32)(acc_lo^acc_hi);

	return(csum);
}

/*
Apply base relocations.
disp: gives the displacement relative to the image base address.
pboix: PBO Index, gives the index used for images to find their associated data sections within a process.

If pboix is used, this skips over relocations in the areas covered by GBR. These relocations will be applied when a new program instance is created.
*/
int X3PE_ApplyBaseRelocs(x3vm_addr imgptr, x3vm_addr rlc, int szrlc,
	s64 disp, int pboix,
	s64 imgbase, int gbr_rva, int gbr_sz, int mach)
{
	x3vm_addr cs, cse, cs1, cs1e;
	x3vm_addr pdst;
	u32 pv, pv0, pv1, pv2, pv3;
	s64 v0, v1;
	int tgt_rva, gbr_end_rva;
	int rva_page, sz_blk;
	int isriscv, isbjx2, isbjx2xg2, isx3rv;
	int tg;

	isriscv=(mach==0x5064);
	isbjx2=(mach==0xB264);
	isbjx2xg2 = (mach==0xB265) || (mach==0xB250);
	isx3rv=(mach==0xB253);

	gbr_end_rva=gbr_rva+gbr_sz;

	cs=rlc;		cse=rlc+szrlc;
	while(cs<cse)
	{
		rva_page=x3pe_vmgetu32(cs);
		sz_blk=x3pe_vmgetu32(cs+4);
		cs1=cs+8; cs1e=cs+sz_blk;
		cs+=sz_blk;
		
		while(cs1<cs1e)
		{
			tg=x3pe_vmgetu16(cs1);
			cs1+=2;

			pdst=imgptr+rva_page+(tg&4095);

			tgt_rva=pdst-imgptr;
			if((tgt_rva>=gbr_rva) && (tgt_rva<gbr_end_rva))
			{
				/* Pointer is within data area. */
				if(((tg>>12)&15)==0)
					{ rva_page+=((s32)(tg<<20))>>8; }
				continue;
			}
			
			switch((tg>>12)&15)
			{
			case 0:
				rva_page+=((s32)(tg<<20))>>8;
				break;
			case 1:
				x3pe_vmsetu16(pdst, x3pe_vmgetu16(pdst)+(disp>>16));
				break;
			case 2:
				x3pe_vmsetu16(pdst, x3pe_vmgetu16(pdst)+(disp&65535));
				break;
			case 3:
				x3pe_vmsetu32(pdst, x3pe_vmgetu32(pdst)+disp);
				break;
			case 4:
				x3pe_vmsetu32(pdst, x3pe_vmgetu32(pdst)+(disp>>32));
				break;
			case 5:
				if(isriscv || isx3rv)
				{
					/* RISC-V: Disp High 20 */
					pv=x3pe_vmgetu32(pdst);
					pv+=disp&0xFFFFF000U;
					x3pe_vmsetu32(pdst, pv);
					break;
				}
			
				if(isbjx2 || isbjx2xg2)
				{
					pv=x3pe_vmgetu32(pdst);
					if((pv&0x0000FE00U)==0x0000FE00U)
					{
						pv0=x3pe_vmgetu32(pdst);
						pv1=x3pe_vmgetu32(pdst+4);
						v0=((pv0&0x00FF)<<16)|((pv0>>16)&0xFFFF);
						v1=((pv1&0x00FF)<<16)|((pv1>>16)&0xFFFF);
						v1+=disp;
						v0+=(disp>>24)+(v1>>24);
						pv0=(pv0&0x0000FF00U)|
							((v0>>16)&0x000000FFU)|
							((v0<<16)&0xFFFF0000U);
						pv1=(pv1&0x0000FF00U)|
							((v1>>16)&0x000000FFU)|
							((v1<<16)&0xFFFF0000U);
						x3pe_vmsetu32(pdst+0, pv0);
						x3pe_vmsetu32(pdst+4, pv1);
						break;
					}
				
					v0=((pv&0x01FF)<<16)|((pv>>16)&0xFFFF);
					v1=v0+disp;
					pv=(pv&0x0000FE00U)|
						((v1>>16)&0x1FF)|
						((v1<<16)&0xFFFF0000U);
					x3pe_vmsetu32(pdst, pv);
					break;
				}
				__debugbreak();
				break;

			case 6:
				if(isbjx2 || isbjx2xg2)
				{
					pv=x3pe_vmgetu16(pdst);
					if((pv==0xA000) && pboix)
					{
						pv=pv|((pboix*8)&0x1FFF);
						x3pe_vmsetu16(pdst, pv);
						break;
					}

					pv=x3pe_vmgetu32(pdst);
					if((pv==0x0000FA00UL) && pboix)
					{
						v0=((pboix*8)&0x01FFFFFF);
						pv=(pv&0x0000FF00U)|
							((v0>>16)&0x000000FFU)|
							((v0<<16)&0xFFFF0000U);
						x3pe_vmsetu32(pdst, pv);
						break;
					}

					pv0=pv;
					pv1=x3pe_vmgetu32(pdst+4);

					if(	(pv0==0x0000FE00UL) &&
						!(pv1&0x00FF0000) &&
						pboix)
					{
						v0=(pboix&0x00FFFFFF);

						pv0=(pv0&0x0000FF00U)|
							((v0>>24)&0x000000FFU)|
							((v0<< 8)&0xFFFF0000U);

						pv1=(pv1&0xFF00FFFF)|((v0<<16)&0x00FF0000);
						x3pe_vmsetu32(pdst+0, pv0);
						x3pe_vmsetu32(pdst+4, pv1);
						break;
					}

					if(pboix)
					{
						__debugbreak();
					}

					break;
				}
				__debugbreak();
				break;

			case 7:
				if(isriscv || isx3rv)
				{
					/* RISC-V: Low 12 I */
					pv=x3pe_vmgetu32(pdst);
					pv+=(disp<<20)&0xFFF00000U;
					x3pe_vmsetu32(pdst, pv);
					break;
				}
				__debugbreak();
				break;
			case 8:
				if(isriscv || isx3rv)
				{
					/* RISC-V: Low 12 S */
					pv=x3pe_vmgetu32(pdst);
					pv+=((disp<<20)&0xFE000000);
					pv+=((disp<< 7)&0x00000F80);
					x3pe_vmsetu32(pdst, pv);
					break;
				}
				__debugbreak();
				break;

			case 9:
				if(isbjx2 || isbjx2xg2)
				{
					pv=x3pe_vmgetu32(pdst);

					if((pv==0x0000FA00UL) && pboix)
					{
						v0=(((-pboix)*8)&0x01FFFFFF);
						pv=(pv&0x0000FF00U)|
							((v0>>16)&0x000000FFU)|
							((v0<<16)&0xFFFF0000U);
						x3pe_vmsetu32(pdst, pv);
						break;
					}

					pv0=pv;
					pv1=x3pe_vmgetu32(pdst+4);

					if(	(pv0==0x0000FE00UL) &&
						!(pv1&0x00FF0000) &&
						pboix)
					{
						v0=(pboix&0x00FFFFFF);

						pv0=(pv0&0x0000FF00U)|
							((v0>>24)&0x000000FFU)|
							((v0<< 8)&0xFFFF0000U);

						pv1=(pv1&0xFF00FFFF)|((v0<<16)&0x00FF0000);
						x3pe_vmsetu32(pdst+0, pv0);
						x3pe_vmsetu32(pdst+4, pv1);
						break;
					}

					break;
				}
				__debugbreak();
				break;
			case 10:
				v0=x3pe_vmgets64(pdst);
				v1=v0-imgbase;
//				if((v0&1) && (v1<gbr_rva))
//					{ v0|=(((s64)x3pe_magic_ubkey)<<56); }
				x3pe_vmsetu64(pdst, v0+disp);
				break;
			case 11:
				break;
			default:
				printf("X3PE_ApplyBaseRelocs: Unhandled Reloc %d\n",
					(tg>>12)&15);
				__debugbreak();
				break;
			}
		}
	}
	return(0);
}

/*
Apply relocations to data sections when creating a program instance.
Will ignore relocations outside of the GBR region.
 */
int X3PE_ApplyDataRelocs(
	x3vm_addr imgptr, x3vm_addr rlc, int szrlc,
	x3vm_addr dataptr,
	s64 disp_base, s64 disp_data, int pboix,
	s64 imgbase, int gbr_rva, int gbr_sz)
{
	x3vm_addr cs, cse, cs1, cs1e;
	x3vm_addr pdst;
	u32 pv;
	s64 v0, v1;
	int tgt_rva, gbr_end_rva;
	int rva_page, rva_dest, sz_blk;
	int tg;

	printf("X3PE_ApplyDataRelocs: disp_base=%X, disp_data=%X\n",
		(int)disp_base, (int)disp_data);

	gbr_end_rva=gbr_rva+gbr_sz;

	printf("X3PE_ApplyDataRelocs: gbr_rva=%X..%X\n",
		gbr_rva, gbr_end_rva);

	cs=rlc;		cse=rlc+szrlc;
	printf("X3PE_ApplyDataRelocs: relocs=%llX..%llX\n",
		cs, cse);

	while(cs<cse)
	{
		rva_page=x3pe_vmgetu32(cs+0);
		sz_blk=x3pe_vmgetu32(cs+4);
		cs1=cs+8; cs1e=cs+sz_blk;
		cs+=sz_blk;
		
		while(cs1<cs1e)
		{
			tg=x3pe_vmgetu16(cs1);
			cs1+=2;

			rva_dest=rva_page+(tg&4095);
			if((rva_dest<gbr_rva) || (rva_dest>=gbr_end_rva))
			{
				/* Pointer is not within data area. */
				if(((tg>>12)&15)==0)
					{ rva_page+=((s32)(tg<<20))>>8; }
				continue;
			}

			pdst=dataptr+(rva_dest-gbr_rva);
			
			switch((tg>>12)&15)
			{
			case 0:
				rva_page+=((s32)(tg<<20))>>8;
				break;
			case 3:
				tgt_rva=(x3pe_vmgetu32(pdst))-imgbase;				
				if((tgt_rva>=gbr_rva) && (tgt_rva<gbr_end_rva))
				{
					x3pe_vmsetu32(pdst, x3pe_vmgetu32(pdst)+disp_data);
					break;
				}
				x3pe_vmsetu32(pdst, x3pe_vmgetu32(pdst)+disp_base);
				break;

			case 10:
				tgt_rva=(x3pe_vmgets64(pdst))-imgbase;				
				if((tgt_rva>=gbr_rva) && (tgt_rva<gbr_end_rva))
				{
					x3pe_vmsetu64(pdst, x3pe_vmgets64(pdst)+disp_data);
					break;
				}

				v0=x3pe_vmgets64(pdst);
//				if(v0&1)
//					v0|=(((s64)x3pe_magic_ubkey)<<56);
				x3pe_vmsetu64(pdst, v0+disp_base);
				break;
			case 11:
				break;

			default:
				printf("X3PE_ApplyDataRelocs: Unhandled Reloc %d\n",
					(tg>>12)&15);
				__debugbreak();
				break;
			}
		}
	}
	return(0);
}

// X3PE_ImageInfo *X3PE_LoadDynELF(X3PE_FILE *fd, int fdoffs,
// 	char *imgname, char *cwd, int is_dll);

#if 1
X3PE_ImageInfo *X3PE_LoadDynPE(X3PE_FILE *fd, int fdoffs,
	char *imgname, char *cwd, int is_dll)
{
	byte tbuf[1024];
	X3PE_ImageInfo *img, *idll;
	x3vm_addr imgptr;
	x3vm_addr ct, cte;
//	x3vm_addr cs, cse;
	x3vm_addr p0, p1;
	char *s0, *s1;
	u64 imgbase, imgbase1;
	u32 imgsz, imgsz1, startrva, gbr_rva, gbr_sz;
	u64 entry;
	byte is64;
	byte is_pel4, cmp;
	u32 csum1, csum2;
	int sig_mz, sig_pe, mach, mmagic, mchar, mdllchar;
	int rva_rlc, sz_rlc;
	s64 rlc_disp;
	u32 tls_rva, tls_sz, tls_iptr, tls_key, tls_rds, tls_rde;
	int rva_imp, sz_imp, rva_ilt, rva_iat;
	int rva_exp, sz_exp;
	int ofs_pe, pboix, szcpy;
	int cb, nb, kb;
	int i, j, k, l, n;
	
//	if(!x3pe_magic_ubkey)
//	{
//		k=X3PE_GetRandom16ASLR();
//		k=(k^(k>>8))&0xFF;
//		x3pe_magic_ubkey=k;
//	}
	
	x3pe_fseek(fd, fdoffs, 0);
	x3pe_fread(tbuf, 1, 1024, fd);

	is_pel4=0;

	sig_mz=X3VM_GetU16LE(tbuf);
	if(sig_mz!=0x4550)
	{
		if(sig_mz==0x457F)
		{
//			return(X3PE_LoadDynELF(fd, fdoffs, imgname, cwd, is_dll));
			return(NULL);
		}

		if(sig_mz!=0x5A4D)
		{
//			printf("TKPE: MZ Sig Fail\n");
			return(NULL);
		}

		ofs_pe=X3VM_GetU32LE(tbuf+0x3C);
		sig_pe=X3VM_GetU16LE(tbuf+ofs_pe);
		if(sig_pe!=0x4550)
		{
//			printf("TKPE: PE Sig Fail\n");
			return(NULL);
		}
	}else
	{
		mach=X3VM_GetU16LE(tbuf+2);
		if(mach==0x364C)
			{ is_pel4=1; cmp=6; }
		if(mach==0x344C)
			{ is_pel4=1; cmp=4; }
		if(mach==0x334C)
			{ is_pel4=1; cmp=3; }
	
		ofs_pe=0;
		sig_pe=sig_mz;
	}

	mach=X3VM_GetU16LE(tbuf+ofs_pe+0x04);
	if(	(mach!=0xB264) && (mach!=0xB265) &&
		(mach!=0x5064) && (mach!=0xB250) &&
		(mach!=0xB296) && (mach!=0xB297) &&
		(mach!=0xB253))
	{
		printf("TKPE: Unexpected Arch %04X\n", mach);
		return(NULL);
	}

	printf("TKPE: PE Sig OK\n");
	
	mmagic=X3VM_GetU16LE(tbuf+ofs_pe+0x18);
	if(mmagic==0x020B)
	{
		is64=1;
	}else if(mmagic==0x010B)
	{
		is64=0;
	}else
	{
		printf("TKPE: Unexpected mMagic %04X\n", mach);
		return(NULL);
	}

	mchar=X3VM_GetU16LE(tbuf+ofs_pe+0x16);
	mdllchar=X3VM_GetU16LE(tbuf+ofs_pe+0x5E);

	if(is64)
	{
		startrva=X3VM_GetU32LE(tbuf+ofs_pe+0x28);
		imgbase=X3VM_GetU64LE(tbuf+ofs_pe+0x30);
		imgsz=X3VM_GetU32LE(tbuf+ofs_pe+0x50);
		gbr_rva=X3VM_GetU32LE(tbuf+ofs_pe+0xC8);
		gbr_sz=X3VM_GetU32LE(tbuf+ofs_pe+0xCC);

		rva_rlc=X3VM_GetU32LE(tbuf+ofs_pe+0xB0);
		sz_rlc=X3VM_GetU32LE(tbuf+ofs_pe+0xB4);

		tls_rva=X3VM_GetU32LE(tbuf+ofs_pe+0xD0);
		tls_sz=X3VM_GetU32LE(tbuf+ofs_pe+0xD4);

		rva_exp=X3VM_GetU32LE(tbuf+ofs_pe+0x88);
		sz_exp=X3VM_GetU32LE(tbuf+ofs_pe+0x8C);
		rva_imp=X3VM_GetU32LE(tbuf+ofs_pe+0x90);
		sz_imp=X3VM_GetU32LE(tbuf+ofs_pe+0x94);
	}else
	{
		startrva=X3VM_GetU32LE(tbuf+ofs_pe+0x28);
		imgbase=X3VM_GetU32LE(tbuf+ofs_pe+0x34);
		imgsz=X3VM_GetU32LE(tbuf+ofs_pe+0x50);
		gbr_rva=X3VM_GetU32LE(tbuf+ofs_pe+0xB8);
		gbr_sz=X3VM_GetU32LE(tbuf+ofs_pe+0xBC);

		rva_rlc=X3VM_GetU32LE(tbuf+ofs_pe+0xA0);
		sz_rlc=X3VM_GetU32LE(tbuf+ofs_pe+0xA4);

		tls_rva=X3VM_GetU32LE(tbuf+ofs_pe+0xC0);
		tls_sz=X3VM_GetU32LE(tbuf+ofs_pe+0xC4);

		rva_exp=X3VM_GetU32LE(tbuf+ofs_pe+0x78);
		sz_exp=X3VM_GetU32LE(tbuf+ofs_pe+0x7C);
		rva_imp=X3VM_GetU32LE(tbuf+ofs_pe+0x80);
		sz_imp=X3VM_GetU32LE(tbuf+ofs_pe+0x84);
	}
	
	printf("TKPE: Base=%08llX Sz=%d BootRVA=%08X GbrRVA=%08X\n",
		imgbase, imgsz, startrva, gbr_rva);
	
	img=malloc(sizeof(X3PE_ImageInfo));
	memset(img, 0, sizeof(X3PE_ImageInfo));

	imgsz1=(imgsz+16384+16383)&(~16383);

	i=(X3PE_GetRandom16ASLR()*64)&16383;

//	imgptr=x3pe_mmap(0, imgsz1, TKMM_PROT_RWX,
//		TKMM_MAP_SHARED|TKMM_MAP_32BIT|TKMM_MAP_DIRECT, -1, 0);
	imgptr=X3VM_VirtAllocPages(x3pe_vmctx, imgsz1);

//	memset(imgptr, 0, imgsz1);

//	X3PE_MProtectPages((u64)imgptr, imgsz1,
//		TKMM_PROT_READ|TKMM_PROT_WRITE|
//		TKMM_PROT_EXEC);

	img->dllflags=
		(mchar&0x0000FFFFULL) |
		((mdllchar&0x0000FFFFULL)<<16);

	img->imgbase=imgptr;
	img->imgname=x3vm_strdup(imgname);
	img->imgsz=imgsz;

//	imgbase1=(u64)imgptr;
//	printf("TKPE!LDA:%s=%04X_%08X\n", imgname,
//		(u16)(imgbase1>>32), (u32)imgbase1);

	rlc_disp=imgptr-imgbase;
	
	if(!is_pel4)
	{
		x3pe_fseek(fd, fdoffs, 0);

		cb=0; nb=imgsz>>10;
		ct=imgptr; cte=imgptr+imgsz;
		while((ct+1024)<=cte)
		{
			printf("%d/%dkB\r", cb, nb);
//			x3pe_fread(ct, 1, 1024, fd);
			memset(tbuf, 0, 1024);
			x3pe_fread(tbuf, 1, 1024, fd);
			x3pe_vmmemcpyin(ct, tbuf, 1024);
			ct+=1024; cb++;
		}
		printf("%d/%dkB\r", cb, nb);
//		x3pe_fread(ct, 1, cte-ct, fd);
		x3pe_fread(tbuf, 1, cte-ct, fd);
		x3pe_vmmemcpyin(ct, tbuf, cte-ct);
		printf("\n");
	}else
	{
		x3pe_vmmemcpyin(imgptr, tbuf, 1024);

		cb=0; nb=imgsz>>10;
		ct=imgptr+1024; cte=imgptr+imgsz;
		l=1024;
		while((ct+1024)<=cte)
		{
			kb=(ct-imgptr)>>10;
//			printf("%d/%dkB\r", cb, nb);
			printf("%d/%dkB\r", kb, nb);
			l=x3pe_fread(tbuf, 1, 1024, fd);
			ct=X3PE_UnpackBuffer(ct, tbuf, l, cmp);
			if(l<1024)
				break;
			cb++;
		}
		kb=(ct-imgptr)>>10;
//		printf("%d/%dkB\r", cb, nb);
		printf("%d/%dkB\r", kb, nb);
		if(l>=1024)
		{
			l=x3pe_fread(tbuf, 1, 1024, fd);
			ct=X3PE_UnpackBuffer(ct, tbuf, l, cmp);
		}
		printf("\n");
	}


	if(is_pel4)
	{
		csum1=x3pe_vmgetu32(imgptr+ofs_pe+0x58);
		
		if(csum1)
		{
			if(ct!=cte)
			{
				printf("Image Size Mismatch %08X -> %08X\n",
					(int)(cte-imgptr), (int)(ct-imgptr));
				__debugbreak();
			}
		
			x3pe_vmsetu16(imgptr+ofs_pe+0x02, 0);
			x3pe_vmsetu32(imgptr+ofs_pe+0x58, 0);
			
			csum2=X3PE_CalculateImagePel4BChecksum(imgptr, imgsz);
			if(csum1!=csum2)
			{
				printf("Image Checksum Mismatch %08X -> %08X\n",
					csum1, csum2);
				__debugbreak();
			}
		}
	}

#if 1
	pboix=0;
	if(gbr_sz!=0)
	{
		img->isdll=is_dll;

		i=x3pe_nimgix++;
		if(!i)
			i=x3pe_nimgix++;
		x3pe_pbo_image[i]=img;
		img->imgix=i;

//		x3pe_vmgetu32(imgptr+0x0C)=i;
		x3pe_vmsetu32(imgptr+0x0C, i);

		if(is_dll&1)
		{
			pboix=x3pe_npboix++;
			x3pe_pbo_dllimg[pboix]=img;
			img->pboix=pboix;
		}
		else
		{
			/* EXE images always remain as 0. */
			img->pboix=0;
			pboix=0;
		}
	}
	
	X3PE_ApplyBaseRelocs(imgptr, imgptr+rva_rlc, sz_rlc, rlc_disp, pboix,
		imgbase, gbr_rva, gbr_sz, mach);

//	TK_FlushCacheL1D();
//	TK_FlushCacheL1D_INVIC(NULL);

	entry=imgptr+startrva;

	if(mach==0xB264)
		{ entry|=0x0000000000000001ULL; }
	if(mach==0x5064)
		{ entry|=0x0004000000000001ULL; }
	if((mach==0xB265) || (mach==0xB297))
		{ entry|=0x0088000000000001ULL; }
	if(mach==0xB250)
		{ entry|=0x008C000000000001ULL; }
	if(mach==0xB253)
		{ entry|=0x000C000000000001ULL; }

//	entry|=((u64)x3pe_magic_ubkey)<<56;

	img->bootptr=entry;
	img->bootgbr=imgptr+gbr_rva;
	img->bootgbre=imgptr+imgsz;
	
	img->rlc_imgbase=imgbase;
	img->rlc_basedisp=rlc_disp;
	img->ptr_reloc=imgptr+rva_rlc;
	img->sz_reloc=sz_rlc;

	img->gbr_rva=gbr_rva;

	szcpy=imgsz-gbr_rva;
	if(szcpy>gbr_sz)
		szcpy=gbr_sz;
	img->gbr_szcpy=szcpy;
	img->gbr_sz=gbr_sz;
#endif

	img->tls_rva=tls_rva;

	if(tls_rva)
	{
		tls_key=X3PE_AllocNewTlsGbl();
		tls_iptr=x3pe_vmgetu32(imgptr+tls_rva+0x08);
		tls_rds=x3pe_vmgetu32(imgptr+tls_rva+0x00);
		tls_rde=x3pe_vmgetu32(imgptr+tls_rva+0x04);
		x3pe_vmsetu32(imgptr+tls_iptr, tls_key);
		
		img->tls_key=tls_key;
		img->tls_rvaraw=tls_rds;
		img->tls_szraw=tls_rde-tls_rds;
		img->tls_dsize=x3pe_vmgetu32(imgptr+tls_rva+0x10);
	}

	img->rva_imp=rva_imp;
	img->rva_exp=rva_exp;
	img->sz_imp=sz_imp;
	img->sz_exp=sz_exp;

	if(rva_imp)
	{
		n=sz_imp/20;
		for(i=0; i<n; i++)
		{
			j=rva_imp+(i*20);
			k=x3pe_vmgetu32(imgptr+j+0x0C);
			if(!k)
				break;
			s0=x3pe_vmgetstring(imgptr+k);
			l=X3PE_TryLoadProgramImage(s0, cwd, 1);
			idll=X3PE_GetImageForIndex(l);
			img->dll[img->n_dll++]=idll;
			
			rva_ilt=x3pe_vmgetu32(imgptr+j+0x00);
			rva_iat=x3pe_vmgetu32(imgptr+j+0x10);
			
			for(j=0;; j++)
			{
				k=x3pe_vmgetu64(imgptr+rva_ilt+j*8);
				if(!k)
					break;
				s0=x3pe_vmgetstring(imgptr+k);
				p0=X3PE_LookupImageDllExport(idll, s0);
				x3pe_vmsetu64(imgptr+rva_iat+j*8, p0);
			}
		}
	}

	return(img);
}

x3vm_addr X3PE_LookupImageDllExport(X3PE_ImageInfo *img, char *name)
{
	x3vm_addr imgptr, fcn;
	char *s0, *s1;
	int rva_exp, sz_exp;
	int rva_eat, rva_ent, rva_eot, n_ent, n_eob, n_exps;
	int is32;
	int i, j, k;
	
	if(img->iself)
	{
//		return(X3PE_LookupImageElfExport(img, name));
	}
	
	imgptr=img->imgbase;
	rva_exp=img->rva_exp;
	sz_exp=img->sz_exp;
	
	if(rva_exp<0x40)
	{
		printf("X3PE_LookupImageDllExport: Bad Exports, RVA=%08X Sz=%X\n",
			rva_exp, sz_exp);
		return(0);
	}
	
	rva_eat=x3pe_vmgetu32(imgptr+rva_exp+0x1C);

	rva_ent=x3pe_vmgetu32(imgptr+rva_exp+0x20);
	rva_eot=x3pe_vmgetu32(imgptr+rva_exp+0x24);
	n_ent=x3pe_vmgetu32(imgptr+rva_exp+0x18);
	n_eob=x3pe_vmgetu32(imgptr+rva_exp+0x10);
	n_exps=x3pe_vmgetu32(imgptr+rva_exp+0x14);

	is32=0;
	if(n_exps<0)
	{
		n_exps=-n_exps;
		is32=1;
	}else
		if((n_eob+n_exps)>65536)
	{
		is32=1;
	}

	if(n_ent<0)
	{
		n_ent=-n_ent;
		is32=1;
	}else
		if(n_ent>=65536)
	{
		is32=1;
	}

	printf("X3PE_LookupImageDllExport: "
		"ENT=%08X EOT=%08X N_Ent=%d N_EOB=%02X\n",
		rva_exp, sz_exp, n_ent, n_eob);
	
	for(i=0; i<n_ent; i++)
	{
		j=x3pe_vmgetu32(imgptr+rva_ent+i*4);
		s0=x3pe_vmgetstring(imgptr+j);

		printf("X3PE_LookupImageDllExport: Exp, Name RVA=%08X\n", j);
		printf("X3PE_LookupImageDllExport: Exp, Name=%s\n", s0);

		if(!strcmp(s0, name))
		{
			if(is32)
				{ j=(x3pe_vmgetu32(imgptr+rva_eot+i*4))-n_eob; }
			else
				{ j=(x3pe_vmgetu16(imgptr+rva_eot+i*2))-n_eob; }
			k=x3pe_vmgetu32(imgptr+rva_eat+j*4);

			printf("X3PE_LookupImageDllExport: Found Ord=%d RVA=%08X\n",
				j, k);

			fcn=imgptr+k;
			printf("X3PE_LookupImageDllExport: Found Name=%s Ptr=%ll012X\n",
				s0, fcn);

			return(fcn);
		}
	}

	return(0);
}
#endif

X3PE_FILE *X3PE_TryOpenImage(
	char *imgname, char *cwd, char **rpath, int isdll)
{
	char tbuf[512];
	X3PE_FILE *fd;
	char **path;
	int npath, isext;
	int i;

	isext=(strchr(imgname, '.')!=NULL);

	if((imgname[0]=='/') || (isdll&2))
	{
		fd=x3pe_fopen(imgname, "rb");
		if(!fd && !isext && (isdll&1))
		{
			sprintf(tbuf, "%s.dll", imgname);
			fd=x3pe_fopen(tbuf, "rb");
		}
		if(!fd && !isext && !(isdll&1))
		{
			sprintf(tbuf, "%s.exe", imgname);
			fd=x3pe_fopen(tbuf, "rb");
		}
		if(fd)
		{
			if(rpath)
				*rpath=x3vm_strdup(tbuf);
			return(fd);
		}
		
		return(NULL);
	}

	if(cwd)
	{
		sprintf(tbuf, "%s/%s", cwd, imgname);
		fd=x3pe_fopen(tbuf, "rb");
		if(!fd && !isext && (isdll&1))
		{
			sprintf(tbuf, "%s/%s.dll", cwd, imgname);
			fd=x3pe_fopen(tbuf, "rb");
		}
		if(!fd && !isext && !(isdll&1))
		{
			sprintf(tbuf, "%s/%s.exe", cwd, imgname);
			fd=x3pe_fopen(tbuf, "rb");
		}
		if(fd)
		{
			if(rpath)
				*rpath=x3vm_strdup(tbuf);
			return(fd);
		}
	}

#if 0
	path=NULL; npath=0;
	TK_Env_GetPathList(&path, &npath);
	for(i=0; i<npath; i++)
	{
		sprintf(tbuf, "%s/%s", path[i], imgname);
		printf("X3PE_TryOpenImage: try %s\n", tbuf);
		fd=x3pe_fopen(tbuf, "rb");
		if(!fd && !isext && (isdll&1))
		{
			sprintf(tbuf, "%s/%s.dll", path[i], imgname);
			printf("X3PE_TryOpenImage: try %s\n", tbuf);
			fd=x3pe_fopen(tbuf, "rb");
		}
		if(!fd && !isext && !(isdll&1))
		{
			sprintf(tbuf, "%s/%s.exe", path[i], imgname);
			printf("X3PE_TryOpenImage: try %s\n", tbuf);
			fd=x3pe_fopen(tbuf, "rb");
		}
		if(fd)
		{
			if(rpath)
				*rpath=x3vm_strdup(tbuf);
			return(fd);
		}
	}
	TK_Env_FreePathList(path);
#endif

#if 0
	path=NULL; npath=0;
	TK_Env_GetLibPathList(&path, &npath);
	for(i=0; i<npath; i++)
	{
		sprintf(tbuf, "%s/%s", path[i], imgname);
		printf("X3PE_TryOpenImage: try %s\n", tbuf);
		fd=x3pe_fopen(tbuf, "rb");
		if(!fd && !isext && (isdll&1))
		{
			sprintf(tbuf, "%s/%s.dll", path[i], imgname);
			printf("X3PE_TryOpenImage: try %s\n", tbuf);
			fd=x3pe_fopen(tbuf, "rb");
		}
		if(!fd && !isext && !(isdll&1))
		{
			sprintf(tbuf, "%s/%s.exe", path[i], imgname);
			printf("X3PE_TryOpenImage: try %s\n", tbuf);
			fd=x3pe_fopen(tbuf, "rb");
		}
		if(fd)
		{
			if(rpath)
				*rpath=x3vm_strdup(tbuf);
			return(fd);
		}
	}
	TK_Env_FreePathList(path);
#endif

	return(NULL);
}

int X3PE_LookupPboImageName(char *imgname, int isdll)
{
	X3PE_ImageInfo *img;
	int i;
	
	for(i=1; i<x3pe_nimgix; i++)
	{
		img=x3pe_pbo_image[i];

		printf("X3PE_LookupPboImageName: Check %s\n", img->imgname);

		if((isdll&1) && !(img->pboix))
			continue;
		if(!strcmp(img->imgname, imgname))
			return(i);
	}
	printf("X3PE_LookupPboImageName: Not Found %s\n", imgname);
	return(0);
}

int X3PE_LookupPboImagePath(char *imgpath, int isdll)
{
	X3PE_ImageInfo *img;
	int i;
	
	for(i=1; i<x3pe_nimgix; i++)
	{
		img=x3pe_pbo_image[i];

		if((isdll&1) && !(img->pboix))
			continue;
		if(!strcmp(img->imgpath, imgpath))
			return(i);
	}

	return(0);
}

int X3PE_LookupPboImagePathExt(char *imgpath, int isdll)
{
	char tbuf[512];
	int isext, ix;

	isext=(strchr(imgpath, '.')!=NULL);
	
	ix=X3PE_LookupPboImagePath(imgpath, isdll);
	if(ix>0)
		return(ix);
	if(isext)
		return(0);

	if(!isext && (isdll&1))
	{
		sprintf(tbuf, "%s.dll", imgpath);
		ix=X3PE_LookupPboImagePath(tbuf, isdll);
		if(ix>0)
			return(ix);
	}

	if(!isext && !(isdll&1))
	{
		sprintf(tbuf, "%s.exe", imgpath);
		ix=X3PE_LookupPboImagePath(tbuf, isdll);
		if(ix>0)
			return(ix);
	}
	
	return(0);
}

int X3PE_LookupPboImageRelPath(char *imgname, char *cwd, int isdll)
{
	char tbuf[512];
	X3PE_FILE *fd;
	char **path;
	int npath, isext;
	int i, j;

	if(*imgname=='/')
		imgname++;
	if(cwd && (*cwd=='/'))
		cwd++;

	isext=(strchr(imgname, '.')!=NULL);

	if((*imgname=='/') || (isdll&2))
	{
		i=X3PE_LookupPboImagePath(imgname, isdll);
		if(i>0)
			return(i);
		if(!isext && (isdll&1))
		{
			sprintf(tbuf, "%s.dll", imgname);
			i=X3PE_LookupPboImagePath(tbuf, isdll);
			if(i>0)
				return(i);
		}
		if(!isext && !(isdll&1))
		{
			sprintf(tbuf, "%s.exe", imgname);
			i=X3PE_LookupPboImagePath(tbuf, isdll);
			if(i>0)
				return(i);
		}
		
		return(0);
	}

	if(cwd)
	{
		sprintf(tbuf, "%s/%s", cwd, imgname);
		i=X3PE_LookupPboImagePath(tbuf, isdll);
		if(i>0)
			return(i);
		if(!isext && (isdll&1))
		{
			sprintf(tbuf, "%s/%s.dll", cwd, imgname);
			i=X3PE_LookupPboImagePath(tbuf, isdll);
			if(i>0)
				return(i);
		}
		if(!isext && !(isdll&1))
		{
			sprintf(tbuf, "%s/%s.exe", cwd, imgname);
			i=X3PE_LookupPboImagePath(tbuf, isdll);
			if(i>0)
				return(i);
		}
	}

#if 0
	path=NULL; npath=0;
	TK_Env_GetPathList(&path, &npath);
	for(i=0; i<npath; i++)
	{
		sprintf(tbuf, "%s/%s", path[i], imgname);
		j=X3PE_LookupPboImagePath(tbuf, isdll);
		if(j>0)
			return(j);
		if(!isext && (isdll&1))
		{
			sprintf(tbuf, "%s/%s.dll", path[i], imgname);
			j=X3PE_LookupPboImagePath(tbuf, isdll);
			if(j>0)
				return(j);
		}
		if(!isext && !(isdll&1))
		{
			sprintf(tbuf, "%s/%s.exe", path[i], imgname);
			j=X3PE_LookupPboImagePath(tbuf, isdll);
			if(j>0)
				return(j);
		}
	}
	TK_Env_FreePathList(path);
#endif

	return(0);
}

struct {
char *name;
int id;
}x3pe_stab_stynames[]={
{"GSYM",			0x20},		{"FNAME",			0x22},
{"FUM",				0x24},		{"STSYM",			0x26},
{"LCSYM",			0x28},		{"MAIN",			0x2A},
{"ROSYM",			0x2C},		{"PC",				0x30},
{"NSYMS",			0x32},		{"NOMAP",			0x34},
{"MAC_DEFINE",		0x36},		{"OBJ",				0x38},
{"MAC_UNDEF",		0x3A},		{"OPT",				0x3C},
{"RSYM",			0x40},		{"M2C",				0x42},
{"SLINE",			0x44},		{"DSLINE",			0x46},
{"BSLINE",			0x48},		{"DEFD",			0x4A},
{"FLINE",			0x4C},
{"EHDECL",			0x50},		{"MOD2",			0x52},
{"CATCH",			0x54},
{"SSYM",			0x60},		{"ENDM",			0x62},
{"SO",				0x64},
{"LSYM",			0x80},		{"BINCL",			0x82},
{"SOL",				0x84},
{"PSYM",			0xA0},		{"EINCL",			0xA2},
{"ENTRY",			0xA4},
{"LBRAC",			0xC0},		{"EXCL",			0xC2},
{"SCOPE",			0xC4},
{"RBRAC",			0xE0},		{"BCOMM",			0xE2},
{"ECOMM",			0xE4},
{"ECOML",			0xE8},		{"WITH",			0xEA},

{"NBTEXT",			0xF0},		{"NBDATA",			0xF2},
{"NBDATA",			0xF4},		{"NBBSS",			0xF6},
{"NBLCS",			0xF8},
{NULL, 0}
};

int X3PE_StabTypeToNumber(char *stn)
{
	int i;

	for(i=0; x3pe_stab_stynames[i].name; i++)
		if(!strcmp(x3pe_stab_stynames[i].name, stn))
			return(x3pe_stab_stynames[i].id);
	return(0);
}

int X3PE_ReadImageMapSyms(X3PE_ImageInfo *img, X3PE_FILE *fd)
{
	char tbuf[256];
	char nbuf[128];
	char stnbuf[32];
	int *map_rva;
	char **map_sym;
	char *map_sch;
	int *line_rva;
	char **line_sym;

	int *stab_rva;
	char **stab_sym;
	int *stab_tde;

	char sch;
	int oth, desc, stety, tdesc;
	int n, rva, nsym, msym, nline, mline, nstab, mstab;
	
	msym=1024;
	map_rva=malloc(msym*sizeof(int));
	map_sym=malloc(msym*sizeof(char *));
	map_sch=malloc(msym*sizeof(char));

	mline=1024;
	line_rva=malloc(mline*sizeof(int));
	line_sym=malloc(mline*sizeof(char *));

	mstab=1024;
	stab_rva=malloc(mstab*sizeof(int));
	stab_sym=malloc(mstab*sizeof(char *));
	stab_tde=malloc(mstab*sizeof(int));
	
	nsym=0;
	nline=0;
	nstab=0;
	while(1)
	{
		n=x3pe_fgets(tbuf, 255, fd);
		if(n<=0)
			break;
		sscanf(tbuf, "%X %c %s", &rva, &sch, nbuf);
		
		if((nsym+1)>=msym)
		{
			n=msym+(msym>>1);
			map_rva=realloc(map_rva, n*sizeof(int));
			map_sym=realloc(map_sym, n*sizeof(char *));
			map_sch=realloc(map_sch, n*sizeof(char));
			msym=n;
		}
		
		if((nline+1)>=mline)
		{
			n=mline+(mline>>1);
			line_rva=realloc(line_rva, n*sizeof(int));
			line_sym=realloc(line_sym, n*sizeof(char *));
			mline=n;
		}
		
		if((nstab+1)>=mstab)
		{
			n=mstab+(mstab>>1);
			stab_rva=realloc(stab_rva, n*sizeof(int));
			stab_tde=realloc(stab_tde, n*sizeof(int));
			stab_sym=realloc(stab_sym, n*sizeof(char *));
			mstab=n;
		}
		
		if(sch=='-')
		{
			sscanf(tbuf, "%X %c %X %X %s %s",
				&rva, &sch, &oth, &desc, stnbuf, nbuf);

			stety=X3PE_StabTypeToNumber(stnbuf);
			tdesc=stety|(oth<<8)|(desc<<16);

			n=nstab++;
			stab_rva[n]=rva;
			stab_tde[n]=tdesc;
			stab_sym[n]=x3vm_strdup(nbuf);
		}else
			if(sch=='L')
		{
			n=nline++;
			line_rva[n]=rva;
			line_sym[n]=x3vm_strdup(nbuf);
		}
		else
		{
			n=nsym++;
			map_rva[n]=rva;
			map_sym[n]=x3vm_strdup(nbuf);
			map_sch[n]=sch;
		}
	}
	
	img->map_rva=map_rva;
	img->map_sym=map_sym;
	img->map_sch=map_sch;
	img->map_nsym=nsym;

	img->map_lnrva=line_rva;
	img->map_lnsym=line_sym;
	img->map_nline=nline;

	img->stab_rva=stab_rva;
	img->stab_sym=stab_sym;
	img->stab_tde=stab_tde;
	img->map_nstab=nstab;

	return(0);
}

int X3PE_TryLoadProgramImage(char *imgname, char *cwd, int isdll)
{
	char tbuf[256];
	X3PE_ImageInfo *img;
	char *basename, *ext;
	char *path, *ls1, *ls2;
	X3PE_FILE *fd;
	byte ext_exe, ext_dll;
	int i;

	ext=strchr(imgname, '.');
	
	if(ext)
	{
		ext_exe = !stricmp(ext, ".exe") || !stricmp(ext, ".elf");
		ext_dll = !stricmp(ext, ".dll") ||
			!stricmp(ext, ".so") ||
			!strncmp(ext, ".so.", 4);

		if(ext_exe && (isdll&1))
			return(0);
		if(ext_dll && !(isdll&1))
			return(0);

		if(!ext_exe && !ext_dll)
			return(0);
	}
	
	
	ls1=strrchr(imgname, '/');
	if(ls1)
	{
		if(imgname[0]=='/')
		{
			i=X3PE_LookupPboImagePathExt(imgname, isdll);
			if(i>0)
				return(i);
		}else if(cwd)
		{
			i=X3PE_LookupPboImageRelPath(imgname, cwd, isdll);
			if(i>0)
				return(i);
		}
		
		strcpy_s(tbuf, 128, ls1+1);
		ls2=strchr(tbuf, '.');
		if(ls2)
			*ls2=0;
		basename=x3vm_strdup(tbuf);
	}else
	{
		strcpy_s(tbuf, 128, imgname);
		ls2=strchr(tbuf, '.');
		if(ls2)
		{
			*ls2=0;
			basename=x3vm_strdup(tbuf);
		}else
		{
			basename=imgname;
		}

		if(imgname!=basename)
		{
			i=X3PE_LookupPboImageName(imgname, isdll);
			if(i>0)
			{
				return(i);
			}
		}

		i=X3PE_LookupPboImageName(basename, isdll);
		if(i>0)
		{
			return(i);
		}
	}
	
	path=NULL;
	fd=X3PE_TryOpenImage(imgname, cwd, &path, isdll);
	if(!fd)
	{
		printf("X3PE_TryLoadProgramImage: Not Found %s\n", imgname);
		return(0);
	}

	img=X3PE_LoadDynPE(fd, 0, basename, cwd, isdll);
	if(!img)
	{
		printf("X3PE_TryLoadProgramImage: Failed to Load %s\n", imgname);
		x3pe_fclose(fd);
		return(0);
	}
	img->imgpath=path;
	x3pe_fclose(fd);
	
	sprintf(tbuf, "%s.map", path);
	fd=x3pe_fopen(tbuf, "rb");
	if(fd)
	{
		X3PE_ReadImageMapSyms(img, fd);
		x3pe_fclose(fd);
	}

	return(img->imgix);
}

X3PE_ImageInfo *X3PE_GetImageForIndex(int ix)
{
	X3PE_ImageInfo *img;
	img=x3pe_pbo_image[ix];
	return(img);
}

int X3PE_LookupSymbolForImageRVA(
	X3PE_ImageInfo *img, int rva,
	char **rname, int *rofs, int fl)
{
	int *map_rva;
	char **map_sym;
	int min, max, piv, n, map_nsym;
	int i;
	
	map_rva=img->map_rva;
	map_sym=img->map_sym;
	map_nsym=img->map_nsym;
	
	if(fl&1)
	{
		map_rva=img->map_lnrva;
		map_sym=img->map_lnsym;
		map_nsym=img->map_nline;
	}
	
	min=0; max=map_nsym; n=32;
	while(((max-min)>16) && n--)
	{
		piv=(min+max)>>1;
		if(map_rva[piv]>rva)
		{
			max=piv;
			continue;
		}
		if(map_rva[piv+1]<rva)
		{
			min=piv;
			continue;
		}
	}
	
	for(i=min; i<max; i++)
	{
		if((map_rva[i]<=rva) && (map_rva[i+1]>rva))
		{
			*rname=map_sym[i];
			*rofs=rva-map_rva[i];
			return(1);
		}
	}

	*rname=NULL;
	*rofs=0;
	return(-1);
}

int X3PE_LookupSymbolForAddr(
	X3VM_Context *ctx, x3vm_addr addr, char **rname, int *rofs, int fl)
{
	X3PE_ImageInfo *img;
	s64 relofs;
	int rva;
	int i;
	
	for(i=0; i<x3pe_nimgix; i++)
	{
		img=x3pe_pbo_image[i];
		if(!img)
			continue;
		if(addr<img->imgbase)
			continue;
		relofs=addr-img->imgbase;
		if(relofs>img->imgsz)
			continue;

		rva=addr-img->rlc_basedisp;
		return(X3PE_LookupSymbolForImageRVA(img, rva, rname, rofs, fl));
	}
	*rname=NULL;
	*rofs=0;
	return(-1);
}

#if 1
void X3PE_InstanceImageInTask(X3PE_TaskInfo *task, X3PE_ImageInfo *img)
{
	x3vm_addr gbrdat, tlsdat;
	int ix, gbrsz, tlssz, gbrszcpy;
	int i, j;

	gbrdat=X3PE_PboGbrGetB(task, img->pboix);
	if(gbrdat)
	{
		if(x3pe_vmgetu64(gbrdat)!=task->imggbrptrs)
			{ __debugbreak(); }
		return;
	}

	gbrdat=0;
	gbrsz=img->gbr_sz;

	if(gbrsz)
	{
//		gbrdat=TKMM_PageAllocVaMap(gbrsz, TKMM_PROT_RW,
//			TKMM_MAP_SHARED|TKMM_MAP_32BIT|TKMM_MAP_DIRECT);
//		gbrdat=X3VM_VirtAllocPages(x3pe_vmctx, gbrsz);
		gbrdat=(x3vm_addr)X3VM_AllocGlobalData(x3pe_vmctx, gbrsz);

		X3PE_TaskAddPageAlloc(task, gbrdat, gbrsz);
	}
	
	printf("TK_InstanceImageInTask: "
		"GBR RVA=%X sz_cpy=%d sz=%d pboix=%d\n",
		img->gbr_rva, img->gbr_szcpy, gbrsz, img->pboix);
	
	if(gbrdat)
	{
		x3pe_vmctx->reg[3]=gbrdat;

		gbrszcpy=img->gbr_szcpy;
		gbrszcpy&=~15;
		if(gbrsz>gbrszcpy)
		{
			x3pe_vmmemset(gbrdat, 0, gbrsz);
		}

		x3pe_vmmemcpy(gbrdat, img->imgbase+img->gbr_rva, img->gbr_szcpy);

		X3PE_ApplyDataRelocs(
			img->imgbase, img->ptr_reloc, img->sz_reloc,
			gbrdat,
			(s64)(img->imgbase)-img->rlc_imgbase,
			(s64)(gbrdat)-(img->rlc_imgbase+img->gbr_rva),
			img->pboix,
			img->rlc_imgbase, img->gbr_rva, img->gbr_sz);
	}

	X3PE_PboImgBaseSetB(task, img->pboix, (x3vm_addr)(img->imgbase));

	if(gbrdat)
	{
		X3PE_PboGbrSetB(task, img->pboix, (x3vm_addr)(gbrdat));
		x3pe_vmsetu64(gbrdat, task->imggbrptrs);
	}
	
	if(img->tls_rva)
	{
		tlssz=img->tls_dsize;
//		tlsdat=TKMM_PageAlloc(tlssz);
		tlsdat=X3VM_VirtAllocPages(x3pe_vmctx, tlssz);
		X3PE_TaskAddPageAlloc(task, tlsdat, tlssz);
		if(img->tls_rvaraw)
		{
			x3pe_vmmemcpy(tlsdat, img->imgbase+img->tls_rvaraw, img->tls_szraw);
		}
		X3PE_TlsSetB(task, img->tls_key, (s64)tlsdat);
	}
	
	for(i=0; i<img->n_dll; i++)
	{
		X3PE_InstanceImageInTask(task, img->dll[i]);
	}
}

int X3PE_SetupTaskForImage(X3PE_TaskInfo *task, X3PE_ImageInfo *img)
{
	X3PE_TaskInfoKern *tkrn;
	x3vm_addr gbrdat, gbrdatb, stkdat;
	int gbrsz, szpbix;
	int i, j, k;

	task->imgbaseptrs=(x3vm_addr)(task->img_baseptrs);
	task->imggbrptrs=(x3vm_addr)(task->img_gbrptrs);
	task->imgtlsrvas=(x3vm_addr)(task->img_tlsrvas);

	X3PE_InstanceImageInTask(task, img);

	gbrdat=X3PE_PboGbrGetB(task, img->pboix);

	printf("X3PE_SetupTaskForImage: GBR=%llX\n", gbrdat);

	task->baseptr=(x3vm_addr)(img->imgbase);
	task->bootptr=(x3vm_addr)(img->bootptr);
	task->basegbr=(x3vm_addr)gbrdat;
	task->boottbr=(x3vm_addr)(task->selfptr);

	tkrn=X3PE_TaskGetKernInfo(task);

	if(!tkrn->usr_boot_sps)
	{
//		stkdat=X3VM_VirtAllocPages(x3pe_vmctx, 1<<17);
		stkdat=(x3vm_addr)X3VM_AllocGlobalData(x3pe_vmctx, 1<<17);
		tkrn->usr_boot_sps=stkdat;
		tkrn->usr_boot_spe=stkdat+(1<<17)-256;
		
		X3PE_TaskSetReg(task, X3PE_REGSAVE_R2, tkrn->usr_boot_spe);
		X3PE_TaskSetReg(task, X3PE_REGSAVE_R3, task->basegbr);
		X3PE_TaskSetReg(task, X3PE_REGSAVE_R4, task->boottbr);

		X3PE_TaskSetReg(task, X3PE_REGSAVE_GBR, task->basegbr);
		X3PE_TaskSetReg(task, X3PE_REGSAVE_SPC, task->bootptr);
	}

	return(1);
}
#endif


#if 0
void *TK_DlOpenB(X3PE_TaskInfo *task, const char *path, int flags)
{
	char tbuf[256];
	X3PE_ImageInfo *img;
	byte *gbrdat, *tlsdat;
	int ix, gbrsz, tlssz;
	
	TK_TaskGetCwd(task, tbuf, 256);

	ix=X3PE_TryLoadProgramImage((char *)path, tbuf, 1);
	if(ix<=0)
		return(NULL);

	img=x3pe_pbo_image[ix];
	TK_InstanceImageInTask(task, img);
	
	return(img->imgbase);
}

void *TK_DlSymB(X3PE_TaskInfo *task,
	void *handle, const char *sym, int flags)
{
	X3PE_ImageInfo *img;
	void *p;
	int ix;
	
	if(handle && sym)
	{
		/* dlsym */
		ix=*(int *)(((byte *)handle)+0x0C);
		img=x3pe_pbo_image[ix];

//		img=handle;
		p=X3PE_LookupImageDllExport(img, (char *)sym);
		return(p);
	}

	if(handle && !sym && ((flags&7)==7))
	{
		/* dlclose */
		return(NULL);
	}

	if(!handle && !sym && ((flags&7)==5))
	{
		/* dlerror */
		return(NULL);
	}

	/* otherwise */
	return(NULL);
}
#endif
