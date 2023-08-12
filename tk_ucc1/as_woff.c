void TKUCC_LinkFlattenObjWoff(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, byte **rbuf, int *rsize)
{
	TKUCC_LinkObjectLbl t_lbl, t_rlc;
	TKUCC_WoffHead *hd;
	TKUCC_WoffLump *de;
	TKUCC_WoffSymbol *sym;
	TKUCC_WoffSymbol *rlc;
	byte *tstrtab;
	byte *tbuf;
	char *s;
	int sz;
	int i, j, k;

	for(i=0; i<obj->n_rlc; i++)
	{
		TKUCC_ObjGetReloc(ctx, obj, i, &t_rlc);
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
			/* reloc to undefined symbol, add as extern */
			memset(&t_lbl, 0, sizeof(TKUCC_LinkObjectLbl));
			t_lbl.lbl_id=t_rlc.lbl_id;
			
			j=obj->n_lbl++;
			TKUCC_ObjSetLabel(ctx, obj, j, &t_lbl);

			t_rlc.sym_name=j;
		}

		TKUCC_ObjSetReloc(ctx, obj, i, &t_rlc);
	}

	for(i=0; i<obj->n_lbl; i++)
	{
		TKUCC_ObjGetLabel(ctx, obj, i, &t_lbl);
		
		if(t_lbl.lbl_id && !t_lbl.sym_name)
		{
			s=TKUCC_AsmGetNameForLabel(ctx, t_lbl.lbl_id);
			j=TKUCC_ObjInternName(ctx, obj, s);
			t_lbl.sym_name=j;

			TKUCC_ObjSetLabel(ctx, obj, i, &t_lbl);
		}
	}
	
	sz=64;
	for(i=0; i<obj->n_sec; i++)
	{
		j=obj->sec[i]->ct_size;
		j=(j+15)&(~15);
		sz+=j;
		sz+=32;
	}

	sz+=obj->n_lbl*12;
	sz+=obj->n_rlc*12;
	
	j=obj->strs->ct_size;
	j=(j+15)&(~15);
	sz+=j;

	sz+=4*32;
	
	tbuf=tkucc_malloc(sz);
	
	hd=(TKUCC_WoffHead *)(tbuf+ 0);
	de=(TKUCC_WoffLump *)(tbuf+32);
	
	hd->fcc_magic=TKUCC_FCC_WOFF;
	hd->num_sec=obj->n_sec+3;
	hd->ofs_sec=32;
	hd->arch=TKUCC_FCC_B264;

	k=32+(obj->n_sec+3)*32;
	for(i=0; i<obj->n_sec; i++)
	{
		strncpy(de[i].name, obj->sec[i]->name, 16);

		j=obj->sec[i]->ct_size;
		de[i].ofs_data=k;
		de[i].csize=j;
		de[i].dsize=j;
		
		TKUCC_ObjReadSectionBytes(ctx, obj, 0, i, tbuf+k, j);
		
		j=(j+15)&(~15);
		k+=j;
	}

	strncpy(de[i].name, "$symtab", 16);
	j=obj->n_lbl*12;
	de[i].ofs_data=k;
	de[i].csize=j;
	de[i].dsize=j;
	sym=(TKUCC_WoffSymbol *)(tbuf+k);
	j=(j+15)&(~15);
	k+=j;
	i++;

	strncpy(de[i].name, "$rlctab", 16);
	j=obj->n_rlc*12;
	de[i].ofs_data=k;
	de[i].csize=j;
	de[i].dsize=j;
	rlc=(TKUCC_WoffSymbol *)(tbuf+k);
	j=(j+15)&(~15);
	k+=j;
	i++;

	strncpy(de[i].name, "$symstr", 16);
	j=obj->strs->ct_size;
	de[i].ofs_data=k;
	de[i].csize=j;
	de[i].dsize=j;
	TKUCC_SecBufReadBytes(ctx, obj->strs, 0, tbuf+k, j);
	j=(j+15)&(~15);
	k+=j;
	i++;
	
	hd->num_sec=i;

	for(i=0; i<obj->n_lbl; i++)
	{
		TKUCC_ObjGetLabel(ctx, obj, i, &t_lbl);
		sym[i].sym_name=t_lbl.sym_name;
		sym[i].sym_ofs=t_lbl.sym_ofs;
		sym[i].sym_sec=t_lbl.sym_sec;
		sym[i].sym_type=t_lbl.sym_type;
		sym[i].pad0=0;
	}

	for(i=0; i<obj->n_rlc; i++)
	{
		TKUCC_ObjGetReloc(ctx, obj, i, &t_lbl);
		rlc[i].sym_name=t_lbl.sym_name;
		rlc[i].sym_ofs=t_lbl.sym_ofs;
		rlc[i].sym_sec=t_lbl.sym_sec;
		rlc[i].sym_type=t_lbl.sym_type;
		rlc[i].pad0=0;
	}

	*rbuf=tbuf;
	*rsize=k;
}
