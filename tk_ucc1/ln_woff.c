TKUCC_LinkObject *TKUCC_LinkObjLoadWoff(TKUCC_MainContext *ctx,
	byte *ibuf, int size)
{
	TKUCC_LinkObjectLbl t_lbl, t_rlc;
	int sectab[64];
	TKUCC_LinkObjectLbl *lbl, *rlc;
	TKUCC_LinkObject *obj;
	TKUCC_WoffHead *hd;
	TKUCC_WoffLump *de, *ade;
	TKUCC_WoffSymbol *isym, *symtab;
	TKUCC_WoffSymbol *irlc, *rlctab;
	byte *strtab;
	byte *tbuf;
	char *s;
	int sz, nl, nsym, nrlc;
	int i, j, k;

	hd=(TKUCC_WoffHead *)ibuf;

	if(hd->fcc_magic!=TKUCC_FCC_WOFF)
		return(NULL);

	obj=TKUCC_AsmAllocObject(ctx);

	ade=(TKUCC_WoffLump *)(ibuf+hd->ofs_sec);
	
	nl=hd->num_sec;

	strtab=NULL;
	symtab=NULL;
	rlctab=NULL;
	
	for(i=0; i<nl; i++)
	{
		de=ade+i;
		if(de->name[0]!='.')
		{
			if(de->name[0]=='$')
			{
				if(!strcmp(de->name, "$symstr"))
				{
					strtab=ibuf+de->ofs_data;
				}

				if(!strcmp(de->name, "$symtab"))
				{
					symtab=(TKUCC_WoffSymbol *)(ibuf+de->ofs_data);
					nsym=de->dsize/12;
				}

				if(!strcmp(de->name, "$rlctab"))
				{
					rlctab=(TKUCC_WoffSymbol *)(ibuf+de->ofs_data);
					nrlc=de->dsize/12;
				}
			}
			continue;
		}

		j=TKUCC_ObjSetSection(ctx, obj, de->name);
		sectab[i+1]=j+1;
		TKUCC_ObjWriteSectionBytes(ctx, obj, 0, j,
			ibuf+de->ofs_data, de->dsize);
		obj->sec[j]->ct_size=de->dsize;
	}
	sectab[0]=0;
	
	lbl=&t_lbl;
	rlc=&t_rlc;

	if(symtab)
	{
		for(i=0; i<nsym; i++)
		{
			isym=symtab+i;
			lbl->sym_name=0;
			lbl->sym_ofs=isym->sym_ofs;
			lbl->sym_sec=sectab[isym->sym_sec];
			lbl->sym_type=isym->sym_sec;
			lbl->pad0=0;
			lbl->lbl_id=TKUCC_AsmGetNamedLabel(ctx, strtab+isym->sym_name);
			
			TKUCC_ObjSetLabel(ctx, obj, i, lbl);
		}
		obj->n_lbl=nsym;
	}

	if(rlctab)
	{
		for(i=0; i<nrlc; i++)
		{
			irlc=rlctab+i;

			TKUCC_ObjGetLabel(ctx, obj, irlc->sym_name, lbl);

			rlc->sym_name=0;
			rlc->sym_ofs=irlc->sym_ofs;
			rlc->sym_sec=sectab[irlc->sym_sec];
			rlc->sym_type=irlc->sym_sec;
			rlc->pad0=0;
			rlc->lbl_id=lbl->lbl_id;
			
			TKUCC_ObjSetReloc(ctx, obj, i, rlc);
		}
		obj->n_rlc=nrlc;
	}
	
	return(obj);
}


int TKUCC_LinkDoLoadObj(TKUCC_MainContext *ctx,
	byte *ibuf, int size)
{
	TKUCC_LinkObject *obj;

	obj=TKUCC_LinkObjLoadWoff(ctx, ibuf, size);
	if(!obj)
		return(-1);

	obj->next=ctx->link_inobj;
	ctx->link_inobj=obj;
	return(0);
}

int TKUCC_LinkDoLoadLib(TKUCC_MainContext *ctx,
	byte *ibuf, int size)
{
	TKUCC_LinkObject *obj;
	TKUCC_ArchFileHead *hd;
	byte *cs, *cse;
	int sz;
	int i, j, k;
	
	cs=ibuf; cse=ibuf+size;

	if(strncmp(cs, "!<arch>\n", 8))
		return(-1);
	cs+=8;

	while(cs<cse)
	{
		hd=(TKUCC_ArchFileHead *)cs;
		cs+=60;
		
		sz=0;
		for(i=0; i<10; i++)
		{
			if(hd->size[i]=' ')
				break;
			sz=(sz*10)+(hd->size[i]-'0');
		}
		
		TKUCC_LinkDoLoadObj(ctx, cs, sz);
		
		cs+=sz;
	}
	
//	obj=TKUCC_LinkObjLoadWoff(ctx, ibuf, size);
//	obj->next=ctx->link_inobj;
//	ctx->link_inobj=obj;
	return(0);
}
