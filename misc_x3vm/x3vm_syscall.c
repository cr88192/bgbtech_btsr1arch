void *x3vm_hopen_fd[256];

int x3vm_hfopen(X3VM_Context *ctx, char *name, char *mode)
{
//	char tbuf[256];
	X3VM_VfsGlue *vfs;
	char *basedif;
	void *fd;
	int ix;
	
	vfs=ctx->vfs;
//	if(!vfs)
//		vfs=&x3vm_vfsglue_stdio;
	
	for(ix=4; ix<256; ix++)
		if(!x3vm_hopen_fd[ix])
			break;
	if(ix>=256)
		return(-1);
	
//	sprintf(tbuf, "resource/test/%s", name);	
//	fd=fopen(name, mode);
//	fd=fopen(tbuf, mode);
	fd=vfs->vt->vfsOpen(vfs, name, mode);
	if(!fd)
		return(-1);
	x3vm_hopen_fd[ix]=fd;
	return(ix);
}

int x3vm_hclose(X3VM_Context *ctx, int ix)
{
	X3VM_VfsGlue *vfs;

	if((ix<4) || (ix>=256))
		return(-1);
	if(!x3vm_hopen_fd[ix])
		return(-1);

	vfs=ctx->vfs;
//	if(!vfs)
//		vfs=&x3vm_vfsglue_stdio;
	
//	fclose(x3vm_hopen_fd[ix]);
	vfs->vt->vfsClose(vfs, x3vm_hopen_fd[ix]);
	x3vm_hopen_fd[ix]=NULL;
	return(1);
}

int x3vm_hread(X3VM_Context *ctx, int ix, x3vm_addr ptr, int sz)
{
	byte tbuf[1024];
	X3VM_VfsGlue *vfs;
	x3vm_addr cs;
	int csz, rt, tot;
	
	if((ix<4) || (ix>=256))
		return(-1);
	if(!x3vm_hopen_fd[ix])
		return(-1);
	
	vfs=ctx->vfs;
//	if(!vfs)
//		vfs=&x3vm_vfsglue_stdio;
	
	cs=ptr; csz=sz; tot=0;
	while(csz>=1024)
	{
//		rt=fread(tbuf, 1, 1024, x3vm_hopen_fd[ix]);
		rt=vfs->vt->vfsRead(vfs, tbuf, 1, 1024, x3vm_hopen_fd[ix]);
		if(rt<1024)
		{
			tot+=rt;
			X3VM_VMemcpyIn(ctx, cs, tbuf, rt);
			return(tot);
		}
		
		X3VM_VMemcpyIn(ctx, cs, tbuf, 1024);
		cs+=1024; csz-=1024; tot+=1024;
	}

//	rt=fread(tbuf, 1, csz, x3vm_hopen_fd[ix]);
	rt=vfs->vt->vfsRead(vfs, tbuf, 1, csz, x3vm_hopen_fd[ix]);
	if(rt<=0)
		return(tot);
	X3VM_VMemcpyIn(ctx, cs, tbuf, rt);
	tot+=rt;
	return(tot);
}

int x3vm_hwrite(X3VM_Context *ctx, int ix, x3vm_addr ptr, int sz)
{
	byte tbuf[1024];
	X3VM_VfsGlue *vfs;
	x3vm_addr cs;
	int csz, rt, tot;
	
	if((ix<4) || (ix>=256))
		return(-1);
	if(!x3vm_hopen_fd[ix])
		return(-1);
	
	vfs=ctx->vfs;
//	if(!vfs)
//		vfs=&x3vm_vfsglue_stdio;
	
	cs=ptr; csz=sz; tot=0;
	while(csz>=1024)
	{
		X3VM_VMemcpyOut(ctx, tbuf, cs, 1024);
//		rt=fwrite(tbuf, 1, 1024, x3vm_hopen_fd[ix]);
		rt=vfs->vt->vfsWrite(vfs, tbuf, 1, 1024, x3vm_hopen_fd[ix]);
		if(rt<1024)
		{
			tot+=rt;
			return(tot);
		}
		cs+=1024; csz-=1024; tot+=1024;
	}

	X3VM_VMemcpyOut(ctx, tbuf, cs, csz);
//	rt=fwrite(tbuf, 1, csz, x3vm_hopen_fd[ix]);
	rt=vfs->vt->vfsWrite(vfs, tbuf, 1, csz, x3vm_hopen_fd[ix]);
	if(rt<=0)
		return(tot);
	tot+=rt;
	return(tot);
}

int x3vm_hseek(X3VM_Context *ctx, int ix, int ofs, int rel)
{
	X3VM_VfsGlue *vfs;

	if((ix<4) || (ix>=256))
		return(-1);
	if(!x3vm_hopen_fd[ix])
		return(-1);
	
	vfs=ctx->vfs;
//	if(!vfs)
//		vfs=&x3vm_vfsglue_stdio;
	
	return(vfs->vt->vfsSeek(vfs, x3vm_hopen_fd[ix], ofs, rel));
	
//	fseek(x3vm_hopen_fd[ix], ofs, rel);
//	return(ftell(x3vm_hopen_fd[ix]));
}

x3vm_addr X3VM_GetVirtApiContext(X3VM_Context *ctx,
	u64 tag1, u64 tag2)
{
	int ix;

	ix=X3PE_LookupHostInterfaceForTag(tag1, tag2);
	if(ix>=0)
	{
		return(X3PE_InstanceHostInterfaceForIndex(ix));
	}else
	{
		printf("X3VM_GetVirtApiContext: Fail tag=\"%s\"\n",
			X3VM_InterfaceTagsToString(tag1, tag2));
	}

	return(0);
}

int X3VM_HandleSyscall(X3VM_Context *ctx)
{
	char tbuf[1024];
	x3vm_addr obj, parm1, parm2, tmp;
	u64 *args, *pret;
	char *s0, *s1;
	int umsg, uscn, sz, msz, prot, fl, fd, ofs;
	
	obj=ctx->reg[10];
	umsg=ctx->reg[11];
	parm1=ctx->reg[12];
	parm2=ctx->reg[13];
	uscn=ctx->reg[17];
	
	if(uscn>=0)
	{
		/* Linux style */
		return(-1);
	}
	
	pret=X3VM_MemMapAddress(ctx, parm1, 1);
	args=X3VM_MemMapAddress(ctx, parm2, 0);
	
	if((umsg>=X3PE_UMSG_ROMFCN) && (umsg<X3PE_UMSG_COMGLUE_VMT0))
	{
		if(umsg==X3PE_UMSG_PAGEALLOC)
		{
			sz=args[0];
			if(sz<=0)
				return(-1);
			tmp=X3VM_VirtAllocPages(ctx, sz);
			*pret=tmp;
			return(1);
		}

		if(umsg==X3PE_UMSG_MPROTECT)
		{
			return(1);
		}

		if(umsg==X3PE_UMSG_CONPUTSN)
		{
			tmp=args[0];
			sz=args[1];
			
			if(sz<1024)
			{
				X3VM_VMemcpyOut(ctx, tbuf, tmp, sz);
				tbuf[sz]=0;
				fputs(tbuf, stdout);
				fflush(stdout);
				return(1);
			}
			return(-1);
		}

		if(umsg==X3PE_UMSG_CONPUTC)
		{
			tmp=args[0];
			fputc(tmp&255, stdout);
//			fflush(stdout);
			return(1);
		}
		
		if(umsg==X3PE_UMSG_CONKBHIT)
		{
			return(0);
		}
		
		if(umsg==X3PE_UMSG_CONGETCH)
		{
			return(0);
		}
		
		if(umsg==X3PE_UMSG_GETENVVAR)
		{
			s0=X3VM_MemMapAddress(ctx, args[0], 0);
			s1=X3VM_MemMapAddress(ctx, args[1], 1);
			msz=args[2];

			if(s0 && !strcmp(s0, "CWD") || !strcmp(s0, "PWD"))
			{
				strcpy(s1, "/");
				sz=strlen(s1);
			}
			*pret=sz;

			return(1);
		}

		if(umsg==X3PE_UMSG_VFOPEN)
		{
			s0=X3VM_MemMapAddress(ctx, args[0], 0);
			s1=X3VM_MemMapAddress(ctx, args[1], 1);

			printf("X3VM_HandleSyscall: Try open %s\n", s0);

			fd=x3vm_hfopen(ctx, s0, s1);
			if(fd>0)
			{
				*pret=fd;
				return(fd);
			}

			*pret=-1;
			return(0);
		}

		if(umsg==X3PE_UMSG_VFCLOSE)
		{
			x3vm_hclose(ctx, args[0]);
			*pret=1;
			return(1);
		}

		if(umsg==X3PE_UMSG_VFREAD)
		{
			sz=x3vm_hread(ctx, args[0], args[1], args[2]);
			if(sz<0)
				sz=0;
			return(sz);
		}

		if(umsg==X3PE_UMSG_VFWRITE)
		{
			sz=x3vm_hwrite(ctx, args[0], args[1], args[2]);
			if(sz<0)
				sz=0;
			return(sz);
		}

		if(umsg==X3PE_UMSG_VFSEEK)
		{
			sz=x3vm_hseek(ctx, args[0], args[1], args[2]);
			if(sz<0)
				sz=0;
			*pret=sz;
			return(1);
		}

		if(umsg==X3PE_UMSG_MMAP)
		{
			tmp=args[0];
			sz=args[1];
			prot=args[2];
			fl=args[3];
			fd=args[4];
			ofs=args[5];

			if(sz<=0)
				return(-1);

			if(fl&0x0200)
			{
				tmp=(x3vm_addr)X3VM_AllocGlobalBulkData(ctx, sz);
			}else
			{
				tmp=X3VM_VirtAllocPages(ctx, sz);
			}
			*pret=tmp;
			return(1);
		}

		if(umsg==X3PE_UMSG_GETAPICTX)
		{
			tmp=X3VM_GetVirtApiContext(ctx, args[0], args[1]);
			*pret=tmp;
			if(!tmp)
				return(0);
			return(1);
		}
	}
	
	if((umsg>=X3PE_UMSG_COMGLUE_VMT0) && umsg<(X3PE_UMSG_COMGLUE_VMT0+512))
	{
		sz=X3PE_InvokeMethodCallToHost(
			obj, umsg-X3PE_UMSG_COMGLUE_VMT0, args, pret);
		return(sz);
	}
	
	return(-1);
}
