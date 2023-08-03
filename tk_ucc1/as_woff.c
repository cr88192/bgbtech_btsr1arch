void TKUCC_AsmObjFlattenWoff(TKUCC_MainContext *ctx,
	TKUCC_LinkObject *obj, byte **rbuf, int *rsize)
{
	TKUCC_LinkObjectLbl t_lbl;
	TKUCC_WoffHead *hd;
	TKUCC_WoffLump *de;
	TKUCC_WoffSymbol *sym;
	TKUCC_WoffSymbol *rlc;
	byte *tstrtab;
	byte *tbuf;
	int sz;
	int i, j, k;


	for(i=0; i<obj->n_sym; i++)
	{
//		TKUCC_AsmObjGetLabel(ctx, obj, idx, &t_lbl);
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
		
		TKUCC_AsmReadSectionBytes(ctx, obj, 0, i, tbuf+k, j);
		
		j=(j+15)&(~15);
		k+=j;
	}

	strncpy(de[i].name, "$symtab", 16);
	j=ob->n_lbl*12;
	de[i].ofs_data=k;
	de[i].csize=j;
	de[i].dsize=j;
	sym=(TKUCC_WoffSymbol *)(tbuf+k);
	j=(j+15)&(~15);
	k+=j;
	i++;

	strncpy(de[i].name, "$reloctab", 16);
	j=ob->n_rlc*12;
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
	TKUCC_AsmReadSecBufBytes(ctx, obj->strs, 0, tbuf+k, j);
	j=(j+15)&(~15);
	k+=j;
	i++;

	*rbuf=tbuf;
	*rsize=k;
}
