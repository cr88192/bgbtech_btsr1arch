void TKUCC_LinkError(TKUCC_MainContext *ctx, char *strs, ...)
{
	char tb[256];
	va_list lst;
	
	va_start(lst, strs);
	vsprintf(tb, strs, lst);
	printf("Link Error: %s\n", tb);
	
	va_end(lst);
}

char *TKUCC_LinkGetSectionName(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int sec)
{
	return(obj->sec[sec]->name);
}

int TKUCC_LinkGetSectionCategory(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, int sec)
{
	char *sn;
	
	sn=TKUCC_LinkGetSectionName(ctx, obj, sec);
	if(!sn)
		return(0);

	if(	!strcmp(sn, ".text") ||
		!strcmp(sn, ".rdata") ||
		!strcmp(sn, ".rodata") ||
		!strcmp(sn, ".strtab") ||
		!strcmp(sn, ".rsrc"))
	{
		return(1);
	}

	if(!strcmp(sn, ".data"))
	{
		return(2);
	}

	if(!strcmp(sn, ".bss"))
	{
		return(3);
	}
	
	if(!strcmp(sn, ".reloc"))
	{
		return(4);
	}
	
	return(0);
}

int TKUCC_LinkCombineObjects(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *dstobj, TKUCC_LinkObject *srcobj)
{
	int sectab[64];
	int secofs[64];
	TKUCC_LinkObjectLbl t_lbl, t_rlc;
	TKUCC_LinkObjectLbl *lbl, *rlc;
	byte *tbuf;
	int sztbuf, sz;
	int i, j, k;

	lbl=&t_lbl;
	rlc=&t_rlc;

	sztbuf=0;
	for(i=0; i<srcobj->n_sec; i++)
	{
		k=srcobj->sec[i]->ct_size;
		if(k>sztbuf)
			sztbuf=k;
	}
	
	tbuf=tkucc_malloc(sztbuf);
	
	for(i=0; i<srcobj->n_sec; i++)
	{
		j=TKUCC_ObjSetSection(ctx, dstobj, srcobj->sec[i]->name);
		sectab[i+1]=j+1;

		sz=srcobj->sec[i]->ct_size;
		TKUCC_ObjReadSectionBytes(ctx, srcobj, 0, i,
			tbuf, sz);

		k=dstobj->sec[j]->ct_size;
		k=(k+15)&(~15);

		TKUCC_ObjWriteSectionBytes(ctx, dstobj, k, j,
			tbuf, sz);
		secofs[i+1]=k;

		k+=sz;
		dstobj->sec[j]->ct_size=k;
	}

	sectab[0]=0;
	secofs[0]=0;


	for(i=0; i<srcobj->n_lbl; i++)
	{
		TKUCC_ObjGetLabel(ctx, srcobj, i, lbl);

		if(!lbl->sym_sec)
		{
			/* skip extern symbols here. */
			continue;
		}

		lbl->sym_ofs=lbl->sym_ofs+secofs[lbl->sym_sec];
		lbl->sym_sec=sectab[lbl->sym_sec];

		j=dstobj->n_lbl++;
		TKUCC_ObjSetLabel(ctx, dstobj, j, lbl);
	}

	for(i=0; i<srcobj->n_rlc; i++)
	{
		TKUCC_ObjGetReloc(ctx, srcobj, i, rlc);

		if(!rlc->sym_sec)
		{
			/* skip null-section relocs. */
			continue;
		}

		rlc->sym_ofs=rlc->sym_ofs+secofs[rlc->sym_sec];
		rlc->sym_sec=sectab[rlc->sym_sec];

		j=dstobj->n_rlc++;
		TKUCC_ObjSetReloc(ctx, dstobj, j, rlc);
	}
	
	tkucc_free(tbuf);

	return(0);
}

int TKUCC_ObjLookupLabelIndexForLabelId(TKUCC_MainContext *ctx, 
	TKUCC_LinkObject *obj, int lblid)
{
	TKUCC_LinkObjectLbl t_lbl;
	TKUCC_LinkObjectLbl *lbl;
	int j;

	for(j=0; j<obj->n_lbl; j++)
	{
		TKUCC_ObjGetLabel(ctx, obj, j, &t_lbl);
		if(t_lbl.lbl_id==lblid)
		{
			return(j);
		}
	}
	return(-1);
}

int TKUCC_ObjLookupLabelIndexForName(TKUCC_MainContext *ctx, 
	TKUCC_LinkObject *obj, char *name)
{
	int lblid;
	
	lblid=TKUCC_AsmGetNamedLabel(ctx, name);
	return(TKUCC_ObjLookupLabelIndexForLabelId(ctx, obj, lblid));
}

int TKUCC_LinkLookupLabelRvaForName(TKUCC_MainContext *ctx, 
	TKUCC_LinkObject *obj, char *name)
{
	TKUCC_LinkObjectLbl t_lbl;
	u32 rva;
	int ix;

	ix=TKUCC_ObjLookupLabelIndexForName(ctx, obj, name);
	if(ix<0)
		return(0);

	TKUCC_ObjGetLabel(ctx, obj, ix, &t_lbl);
	rva=obj->sec[t_lbl.sym_sec-1]->ct_rva+t_lbl.sym_ofs;
	return(rva);
}

void TKUCC_LinkApplyImageRelocs(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, byte *imgbuf, int imgsize, u64 imgbase)
{
	TKUCC_LinkObjectLbl t_lbl, t_rlc;
	TKUCC_LinkObjectLbl *lbl, *rlc;
	u32 rva_rlc, rva_lbl;
	s64 addr, disp, addr0, addr1;
	byte *p_rlc, *p_lbl;
	char *s0;
	int badrange, opw0, opw1, opw2, opw3;
	int i, j, k;

	lbl=&t_lbl;
	rlc=&t_rlc;

	for(i=0; i<obj->n_rlc; i++)
	{
		TKUCC_ObjGetReloc(ctx, obj, i, &t_rlc);
		rlc->sym_name=-1;

#if 0
		for(j=0; j<obj->n_lbl; j++)
		{
			TKUCC_ObjGetLabel(ctx, obj, j, &t_lbl);
			if(t_lbl.lbl_id==t_rlc.lbl_id)
			{
				t_rlc.sym_name=j;
				break;
			}
		}
		if(j>=obj->n_lbl)
		{
//			t_rlc.sym_name=j;
		}
#endif

		j=TKUCC_ObjLookupLabelIndexForLabelId(ctx, obj, t_rlc.lbl_id);
		if(j>=0)
			{ t_rlc.sym_name=j; }

		TKUCC_ObjSetReloc(ctx, obj, i, &t_rlc);
	}


	for(i=0; i<obj->n_rlc; i++)
	{
		TKUCC_ObjGetReloc(ctx, obj, i, rlc);
		j=rlc->sym_name;
		if(j<0)
		{
			s0=TKUCC_AsmGetNameForLabel(ctx, rlc->lbl_id);
			if(!s0)
				s0="?";
			TKUCC_LinkError(ctx,
				"Reloc Missing Label %02X %08X %s",
				rlc->sym_type, rlc->lbl_id, s0);
			continue;
		}

		TKUCC_ObjGetLabel(ctx, obj, j, lbl);
		
		rva_rlc=obj->sec[rlc->sym_sec-1]->ct_rva+rlc->sym_ofs;
		rva_lbl=obj->sec[lbl->sym_sec-1]->ct_rva+lbl->sym_ofs;
		p_rlc=imgbuf+rva_rlc;
		p_lbl=imgbuf+rva_lbl;
		
		addr=imgbase+rva_lbl;
		disp=((s64)rva_lbl)-((s64)rva_rlc);
		badrange=0;
		
		switch(rlc->sym_type)
		{
		case TKUCC_RELOC_DIR32:
			addr1=addr+(*(u32 *)p_rlc);
			*(u32 *)p_rlc=addr1;
			if(((u32)addr1)!=addr1)
				badrange=1;
			break;
		case TKUCC_RELOC_DIR64:
			addr1=addr+(*(u64 *)p_rlc);
			*(u64 *)p_rlc=addr1;
			break;
		case TKUCC_RELOC_JX2_RELW8S:
			opw0=*(u16 *)p_rlc;
			addr1=(((sbyte)opw0)<<1)+disp;
			*(u16 *)p_rlc=(opw0&0xFF00)|((addr1>>1)&0x00FF);
			if((addr1&1) || (((sbyte)(addr1>>1))!=(addr1>>1)))
				badrange=1;
			break;
		case TKUCC_RELOC_JX2_RELW20S:
			opw0=((u16 *)p_rlc)[0];
			opw1=((u16 *)p_rlc)[1];
			addr0=((opw0&0x00FF)<<13)|((opw1&0x0FFF)<<1);
			addr0=(((s32)(addr0<<11))>>11);
			addr1=addr0+disp;
			if(addr1&1)
				badrange=1;
			opw0=(opw0&0xFF00)|((addr1>>13)&0x00FF);
			opw1=(opw1&0xF000)|((addr1>> 1)&0x0FFF);
			((u16 *)p_rlc)[0]=opw0;
			((u16 *)p_rlc)[1]=opw1;
			if(addr1!=(((s32)(addr1<<11))>>11))
				badrange=1;
			break;

		case TKUCC_RELOC_JX2_RELW24A:
			opw0=((u16 *)p_rlc)[0];
			opw1=((u16 *)p_rlc)[1];
			addr0=((opw0&0x01FF)<<17)|((opw1&0xFFFF)<<1);
			addr0=(((s32)(addr0<<6))>>6);
			addr1=addr0+disp;
			if(addr1&1)
				badrange=1;
			opw0=(opw0&0xFE00)|((addr1>>17)&0x01FF);
			opw1=(opw1&0x0000)|((addr1>> 1)&0xFFFF);
			((u16 *)p_rlc)[0]=opw0;
			((u16 *)p_rlc)[1]=opw1;
			if(addr1!=(((s32)(addr1<<6))>>6))
				badrange=1;
			break;

		default:
			badrange=2;
			break;
		}
		
		if(badrange)
		{
			if(badrange==1)
			{
				TKUCC_LinkError(ctx,
					"Reloc out of Range %02X %lld",
					rlc->sym_type, disp);
			}else
				if(badrange==2)
			{
				TKUCC_LinkError(ctx,
					"Invalid Reloc %02X %lld",
					rlc->sym_type, disp);
			}
		}
	}
}

TKUCC_LinkObject *TKUCC_DoLinkObjectsStage(TKUCC_MainContext *ctx)
{
	TKUCC_LinkObject *dst_obj;
	TKUCC_LinkObject *c_obj;
	
	if(!ctx->link_inobj)
		return(NULL);

	c_obj=ctx->link_inobj;
	if(!c_obj->next)
	{
		ctx->link_inobj=NULL;
		return(c_obj);
	}

	dst_obj=TKUCC_AsmAllocObject(ctx);
	ctx->cur_obj=dst_obj;
	
	c_obj=ctx->link_inobj;
	while(c_obj)
	{
		TKUCC_LinkCombineObjects(ctx, dst_obj, c_obj);
		c_obj=c_obj->next;
	}
	
	return(dst_obj);
}
