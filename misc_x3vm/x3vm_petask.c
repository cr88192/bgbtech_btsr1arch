X3VM_Context *x3pe_vmctx;

u32 *x3pe_tcptr;
u64 *x3pe_tcvta;

X3PE_TaskInfo *X3PE_VMAllocTask()
{
	X3PE_TaskInfo *task;
	X3PE_TaskInfoKern *tkrn;
	X3PE_TaskInfoUser *tusr;
	u32 *tcptr;
	u64 *tcvta;
	u64 a_tuser, a_tbase, a_tkern;
	int sz_user, sz_base, sz_kern;
	int i, j, k;
	
	sz_user=sizeof(X3PE_TaskInfoUser);
	sz_base=sizeof(X3PE_TaskInfo);
	sz_kern=sizeof(X3PE_TaskInfoKern);

//	a_tuser=X3VM_VirtAllocPages(x3pe_vmctx, sz_user);
//	a_tbase=X3VM_VirtAllocPages(x3pe_vmctx, sz_base);
//	a_tkern=X3VM_VirtAllocPages(x3pe_vmctx, sz_kern);
	
	a_tuser=(x3vm_addr)X3VM_AllocGlobalData(x3pe_vmctx, sz_user);
	a_tbase=(x3vm_addr)X3VM_AllocGlobalData(x3pe_vmctx, sz_base);
	a_tkern=(x3vm_addr)X3VM_AllocGlobalData(x3pe_vmctx, sz_kern);
	
	task=X3VM_MemMapAddress(x3pe_vmctx, a_tbase, 0);
	tkrn=X3VM_MemMapAddress(x3pe_vmctx, a_tkern, 0);
	tusr=X3VM_MemMapAddress(x3pe_vmctx, a_tuser, 0);
	
	task->regsave=a_tkern+((byte *)(tkrn->ctx_regsave)-(byte *)tkrn);
	task->tlsptr=a_tuser+((byte *)(tusr->tlsdat)-(byte *)tusr);
	task->tls_freelist=a_tuser+((byte *)(tusr->tls_freelistarray)-(byte *)tusr);

	task->selfptr=a_tbase;
	task->krnlptr=a_tkern;
	task->usrptr=a_tuser;

//	task->SysCall=a_tuser+0x400;
	
	if(!x3pe_tcptr)
	{
		x3pe_tcptr=X3VM_AllocGlobalData(x3pe_vmctx, 512*4);
		tcptr=x3pe_tcptr;
	
//		tcptr=(u32 *)(((byte *)tusr)+0x400);
		tcptr[0]=0xFFF00893U;		//ADD  X17, X0, -1
		tcptr[1]=0x00000073U;		//ECALL
		tcptr[2]=0x00008067U;		//JALR X0, X1, 0
		tcptr[3]=0x00000013U;		//NOP
	//	tcptr[4]=0x00000013U;		//NOP

		tcptr[ 4]=0x0000100AU|( 2<<6)|( 2<<16)|(-128<<22);	//ADD  X2, X2, -128
		tcptr[ 5]=0x00005026U|(10<<6)|( 2<<16)|(   8<<22);	//SDP  X10,  64(SP)
		tcptr[ 6]=0x00005026U|(12<<6)|( 2<<16)|(  10<<22);	//SDP  X12,  80(SP)
		tcptr[ 7]=0x00005026U|(14<<6)|( 2<<16)|(  12<<22);	//SDP  X14,  96(SP)
		tcptr[ 8]=0x00005026U|(16<<6)|( 2<<16)|(  14<<22);	//SDP  X16, 112(SP)
		tcptr[ 9]=0x0000000AU|(13<<6)|( 2<<16)|(  64<<22);	//ADD  X12, X2, 64
		tcptr[10]=0x0000000AU|(12<<6)|( 2<<16)|(  32<<22);	//ADD  X12, X2, 32
		tcptr[11]=0x0000000AU|(11<<6)|(30<<16)|(   0<<22);	//ADD  X11, X30, 0
		tcptr[12]=0xFFF00893U;								//ADD  X17, X0, -1
		tcptr[13]=0x00000073U;								//ECALL
		tcptr[14]=0x00007026U|(10<<6)|(2<<16)|(   4<<22);	//LDP  X10,  64(SP)
		tcptr[15]=0x0000000AU|( 2<<6)|(2<<16)|( 128<<22);	//ADD  X2, X2, 128
		tcptr[16]=0x00008067U;								//JALR X0, X1, 0

//		for(i=0; i<60; i++)
		for(i=0; i<126; i++)
		{
			j=24+i*2;

			//LI  X30, 0x1204+i
			tcptr[j+0]=0x00000012U|(30<<6)|((0x1202+i)<<16);

			//BRA $(tcptr+4)
			tcptr[j+1]=0x0000CFE2U|((4-(j+1))<<16);
		}

	}

	task->SysCall=(u64)x3pe_tcptr;

	if(!x3pe_tcvta)
	{
		x3pe_tcvta=X3VM_AllocGlobalData(x3pe_vmctx, 128*8);
		tcvta=x3pe_tcvta;

//		tcvta=(u32 *)(((byte *)tusr)+0x800);
		tcvta[0]=0;
		tcvta[1]=0;
		tcvta[2]=0;
		tcvta[3]=0;
		
		for(i=0; i<126; i++)
		{
//			tcvta[2+i]=a_tuser+0x400+(24*4)+(i*8);
//			tcvta[2+i]=((u64)x3pe_tcptr)+(24*4)+(i*8);
			tcvta[2+i]=((u64)x3pe_tcptr)+((24+(i*2))*4);
		}
	}

	return(task);
}

X3PE_TaskInfoUser *X3PE_TaskGetUserInfo(X3PE_TaskInfo *task)
{
	X3PE_TaskInfoUser *tusr;
	tusr=X3VM_MemMapAddress(x3pe_vmctx, task->usrptr, 0);
	return(tusr);
}

X3PE_TaskInfoKern *X3PE_TaskGetKernInfo(X3PE_TaskInfo *task)
{
	X3PE_TaskInfoKern *tkrn;
	tkrn=X3VM_MemMapAddress(x3pe_vmctx, task->krnlptr, 0);
	return(tkrn);
}

x3vm_addr X3PE_PboGbrGetB(X3PE_TaskInfo *task, int pboix)
{
	return(task->img_gbrptrs[pboix]);
}

x3vm_addr X3PE_PboGbrSetB(X3PE_TaskInfo *task, int pboix, x3vm_addr dat)
{
	task->img_gbrptrs[pboix]=dat;
	return(0);
}

x3vm_addr X3PE_PboImgBaseSetB(X3PE_TaskInfo *task, int pboix, x3vm_addr dat)
{
	task->img_baseptrs[pboix]=dat;
	return(0);
}

x3vm_addr X3PE_TlsSetB(X3PE_TaskInfo *task, int tlsix, x3vm_addr dat)
{
	task->img_tlsrvas[tlsix]=dat;
	return(0);
}

x3vm_addr X3PE_TaskAddPageAlloc(X3PE_TaskInfo *task, x3vm_addr base, int sz)
{
	return(0);
}

x3vm_addr X3PE_TaskSetReg(X3PE_TaskInfo *task, int regix, x3vm_addr dat)
{
	u64 *regsave;

	regsave=X3VM_MemMapAddress(x3pe_vmctx, task->regsave, 0);
	regsave[regix]=dat;
	return(0);
}

x3vm_addr X3PE_TaskGetReg(X3PE_TaskInfo *task, int regix)
{
	u64 *regsave;

	regsave=X3VM_MemMapAddress(x3pe_vmctx, task->regsave, 0);
	return(regsave[regix]);
}


int X3PE_TaskSetupRunVM(X3PE_TaskInfo *task)
{
	void *psp, *pgp;
	x3vm_addr vsp, vgp, spc;
	int i;

	x3pe_vmctx->reg[0]=0;
	
	for(i=1; i<64; i++)
		x3pe_vmctx->reg[i]=X3PE_TaskGetReg(task, i);
	spc=X3PE_TaskGetReg(task, X3PE_REGSAVE_SPC);
	x3pe_vmctx->reg[X3VM_REG_PC]=spc;
	x3pe_vmctx->pc_hibits=spc>>48;

	vsp=x3pe_vmctx->reg[2];
	vgp=x3pe_vmctx->reg[3];
	psp=X3VM_MemMapAddress(x3pe_vmctx, vsp, 0);
	pgp=X3VM_MemMapAddress(x3pe_vmctx, vgp, 0);
	
	x3pe_vmctx->opt_faststack=0;

	if(psp && (((x3vm_addr)psp)==vsp))
	{
		/* If stack is shared, make it fast... */
		x3pe_vmctx->opt_faststack|=1;
	}

	if(pgp && (((x3vm_addr)pgp)==vgp))
	{
		/* If GBR is shared, make it fast... */
		x3pe_vmctx->opt_faststack|=2;
	}

	return(0);
}

void *x3pe_hostif_vtab[256];
int x3pe_hostif_szvtab[256];
int x3pe_hostif_szdat[256];
int x3pe_hostif_shcpy[256];
int x3pe_hostif_ehcpy[256];
u64 x3pe_hostif_tag1[256];
u64 x3pe_hostif_tag2[256];
int x3pe_n_hostif;

int X3PE_LookupHostInterfaceForTag(u64 tag1, u64 tag2)
{
	int i;
	
	for(i=0; i<x3pe_n_hostif; i++)
	{
		if(	(tag1==x3pe_hostif_tag1[i]) &&
			(tag2==x3pe_hostif_tag2[i]))
				return(i);
	}
	
	for(i=0; i<x3pe_n_hostif; i++)
	{
		if(	(tag1==x3pe_hostif_tag1[i]) &&
			(x3pe_hostif_tag2[i]==0))
				return(i);
	}
	
	return(-1);
}

int X3PE_RegisterHostInterface(
	u64 tag1, u64 tag2,
	void *vtab, int szVtab, int szDat, int shCpy, int ehCpy)
{
	int i;
	
	i=x3pe_n_hostif++;
	x3pe_hostif_vtab[i]=vtab;
	x3pe_hostif_szvtab[i]=szVtab;
	x3pe_hostif_szdat[i]=szDat;
	x3pe_hostif_shcpy[i]=shCpy;
	x3pe_hostif_ehcpy[i]=ehCpy;
	x3pe_hostif_tag1[i]=tag1;
	x3pe_hostif_tag2[i]=tag2;
	return(i);
}

void *x3pe_ifobj[1024];
void *x3pe_ifhob[1024];
void *x3pe_ifhvt[1024];
int x3pe_ifsvt[1024];
int x3pe_ifscp[1024];
int x3pe_ifecp[1024];
int x3pe_n_ifobj;

int X3PE_CheckPointerIsMagic(void *ptr)
{
	if(((u64)ptr)==0)
		return(1);
	if(((u64)ptr)==0x12345678)
		return(1);

	return(0);
}

x3vm_addr X3PE_InstanceHostInterface(
	void *vtab, int szVtab, int szDat, int shCpy, int ehCpy)
{
	void *obj, *hob, *vt1;
	int i, j, k;
	
	for(k=2; k<szVtab; k++)
	{
		if((k<4) && !((void **)vtab)[k])
			continue;
		if(X3PE_CheckPointerIsMagic(((void **)vtab)[k]))
			break;
	}
	
	if(k>=szVtab)
	{
		obj=X3VM_AllocGlobalData(x3pe_vmctx, szDat);
		*(void **)obj=x3pe_tcvta;
	}else
	{
		k=(szDat+15)&(~15);
		obj=X3VM_AllocGlobalData(x3pe_vmctx, k+(szVtab*8));
		vt1=((byte *)obj)+k;
		*(void **)obj=vt1;

		for(i=2; i<szVtab; i++)
		{
			if(X3PE_CheckPointerIsMagic(((void **)vtab)[i]))
			{
				((void **)vt1)[i]=((void **)vtab)[i];
			}else
			{
				((void **)vt1)[i]=(void *)(x3pe_tcvta[i]);
			}
		}
		
	}
	
	hob=obj;
	if(ehCpy)
	{
		hob=X3VM_AllocGlobalData(x3pe_vmctx, szDat);
		*(void **)hob=vtab;
	}
	
	i=x3pe_n_ifobj++;
	x3pe_ifobj[i]=obj;
	x3pe_ifhob[i]=hob;
	x3pe_ifhvt[i]=vtab;
	x3pe_ifsvt[i]=szVtab;
	x3pe_ifscp[i]=shCpy;
	x3pe_ifecp[i]=ehCpy;
	
	return((x3vm_addr)obj);
}

x3vm_addr X3PE_InstanceHostInterfaceForIndex(int ix)
{
	x3vm_addr tmp;
	tmp=X3PE_InstanceHostInterface(
		x3pe_hostif_vtab[ix], x3pe_hostif_szvtab[ix],
		x3pe_hostif_szdat[ix],
		x3pe_hostif_shcpy[ix],
		x3pe_hostif_ehcpy[ix]);
	return(tmp);
}

x3vm_addr X3PE_CheckMapVmPointerToHost(x3vm_addr vma)
{
	void *ptr;

	if(vma>>46)
		return(vma);
	if(vma<(1LL<<40))
		return(vma);

	ptr=X3VM_MemMapAddress(x3pe_vmctx, vma, 0);
	if(ptr && (((x3vm_addr)ptr)!=vma))
		return((x3vm_addr)ptr);
	return(vma);
}

int X3PE_InvokeMethodCallToHost(u64 obj, int vmt, u64 *args, u64 *pret)
{
	u64 (*fcn)(
		void *obj, u64 arg1, u64 arg2, u64 arg3,
		u64 arg4, u64 arg5, u64 arg6, u64 arg7,
		u64 arg8, u64 arg9, u64 arg10, u64 arg11,
		u64 arg12, u64 arg13, u64 arg14, u64 arg15);
	void *hob, *gob;
	u64 tmp;
	int scp, ecp;
	int i, j, k;
	
	for(i=0; i<x3pe_n_ifobj; i++)
	{
		if(obj==((u64)(x3pe_ifobj[i])))
			break;
	}
	
	if(i>=x3pe_n_ifobj)
		return(-1);
	if(vmt<2)
		return(-1);
	if(vmt>=x3pe_ifsvt[i])
		return(-1);
	
	fcn=((void **)(x3pe_ifhvt[i]))[vmt];
	
	gob=x3pe_ifobj[i];
	hob=x3pe_ifhob[i];
	scp=x3pe_ifscp[i];
	ecp=x3pe_ifecp[i];
	if(scp<8)
		scp=8;

	if(ecp && (ecp>scp))
		{ memcpy(((byte *)hob)+scp, ((byte *)gob)+scp, ecp-scp); }
	
	tmp=fcn(hob,
		X3PE_CheckMapVmPointerToHost(args[ 1]),
		X3PE_CheckMapVmPointerToHost(args[ 2]),
		X3PE_CheckMapVmPointerToHost(args[ 3]),
		X3PE_CheckMapVmPointerToHost(args[ 4]),
		X3PE_CheckMapVmPointerToHost(args[ 5]),
		X3PE_CheckMapVmPointerToHost(args[ 6]),
		X3PE_CheckMapVmPointerToHost(args[ 7]),
		X3PE_CheckMapVmPointerToHost(args[ 8]),
		X3PE_CheckMapVmPointerToHost(args[ 9]),
		X3PE_CheckMapVmPointerToHost(args[10]),
		X3PE_CheckMapVmPointerToHost(args[11]),
		args[12], args[13], args[14], args[15]);
	*pret=tmp;

	if(ecp && (ecp>scp))
		{ memcpy(((byte *)gob)+scp, ((byte *)hob)+scp, ecp-scp); }
	
	return(1);
}

int X3VM_TagIsFourcc(u64 tag)
{
	if(tag!=((u32)tag))
		return(0);
	if(tag&0x80808080U)
		return(0);
	if((tag-0x1F1F1F1FU)&0x80808080U)
		return(0);
	return(1);
}

int X3VM_TagIsEightcc(u64 tag)
{
	if(tag!=((u32)tag))
		return(0);
	if(tag&0x8080808080808080ULL)
		return(0);
	if((tag-0x1F1F1F1F1F1F1F1FULL)&0x8080808080808080ULL)
		return(0);
	return(1);
}

char *X3VM_InterfaceTagsToString(u64 tag1, u64 tag2)
{
	char tbuf[256];

	tbuf[0]=0;
	if(X3VM_TagIsEightcc(tag1))
	{
		if(!tag2)
		{
			memcpy(tbuf, &tag1, 8);
			tbuf[8]=0;
		}else
			if(X3VM_TagIsEightcc(tag2))
		{
			memcpy(tbuf+0, &tag1, 8);
			memcpy(tbuf+8, &tag2, 8);
			tbuf[16]=0;
		}else
			if(X3VM_TagIsFourcc(tag2))
		{
			memcpy(tbuf+0, &tag1, 8);
			tbuf[8]='-';
			memcpy(tbuf+9, &tag2, 4);
			tbuf[13]=0;
		}
	}
	else
		if(X3VM_TagIsFourcc(tag1))
	{
		if(!tag2)
		{
			memcpy(tbuf, &tag1, 4);
			tbuf[4]=0;
		}else
			if(X3VM_TagIsEightcc(tag2))
		{
			memcpy(tbuf+0, &tag1, 4);
			tbuf[4]='-';
			memcpy(tbuf+5, &tag2, 8);
			tbuf[13]=0;
		}else
			if(X3VM_TagIsFourcc(tag2))
		{
			memcpy(tbuf+0, &tag1, 4);
			tbuf[4]='-';
			memcpy(tbuf+5, &tag2, 4);
			tbuf[9]=0;
		}
	}
	
	if(!tbuf[0])
	{
		sprintf(tbuf,
			"{%08X-%04X-%04X-%04X-%02X%02X%02X%02X%02X%02X}",
			(u32)(tag1>> 0),
			(u16)(tag1>>32),
			(u16)(tag1>>48),
			(u16)(tag2>> 0),
			(byte)(tag2>>16), (byte)(tag2>>24),
			(byte)(tag2>>32), (byte)(tag2>>40),
			(byte)(tag2>>48), (byte)(tag2>>56)
			);
	}
	
	return(x3vm_strdup(tbuf));
}
