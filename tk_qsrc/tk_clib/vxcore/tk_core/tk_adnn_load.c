TK_AdNn2_Layer *TK_AdNn_AllocLayer(int isz, int osz, int wszfl)
{
	TK_AdNn2_Layer *tmp;
	int j, k, l, l4;
	
	tmp=tk_malloc(sizeof(TK_AdNn2_Layer));
	tmp->nn_isz=isz;
	tmp->nn_osz=osz;
	tmp->nn_afn=0;

	j=isz+1;
	j=(j+7)&(~7);
	l=osz*j;
	k=((l+127)/64)*8;
	tmp->nn_stzm=(j/8);
	tmp->nn_szzm=k;

	l=osz*(isz+1);
	
	j=(isz+1)/2;
	tmp->nn_stw4=2+j;
	l4=osz*(j+2);

	if((wszfl&3)==0)
		tmp->wght4=tk_malloc(l4);
	if((wszfl&3)==1)
		tmp->wght8=tk_malloc(l);

	tmp->wght_zm=tk_malloc(k);
	tmp->act_o=tk_malloc(osz);
	tmp->act_i=tk_malloc(isz);
	
	return(tmp);
}

TK_AdNn2_Net *TK_AdNn_AllocNet(int nl, int *lasz, int wszfl)
{
	TK_AdNn2_Net *tmp;
	int i, j, k;
	
	tmp=tk_malloc(sizeof(TK_AdNn2_Net));
	
	tmp->n_layer=nl;
	for(i=0; i<nl; i++)
	{
		tmp->layer[i]=TK_AdNn_AllocLayer(lasz[i+0], lasz[i+1], wszfl);
	}
	return(tmp);
}


byte *TK_AdNn_LoadNetLayer(TK_AdNn2_Net *net, byte *cs0, int lidx, int wszfl)
{
	TK_AdNn2_Layer *lyr;
	byte *cs, *ct;
	int isz, osz, lsz;
	
	cs=cs0;
	lyr=net->layer[lidx];
	isz=lyr->nn_isz;
	osz=lyr->nn_osz;

	if((wszfl&0xE0)==0x00)
	{
		if((wszfl&3)==0)
		{
			lsz=osz*lyr->nn_stw4;
			ct=lyr->wght4;
			memcpy(ct, cs, lsz);
			return(cs+lsz);
		}

		if((wszfl&3)==1)
		{
			lsz=osz*(isz+1);
			ct=lyr->wght8;
			memcpy(ct, cs, lsz);
			return(cs+lsz);
		}
	}
}

TK_AdNn2_Net *TK_AdNn_LoadNetBuffer(byte *buf, int sz)
{
	int la_isz[64];
	int la_osz[64];
	int la_afn[64];
	int la_sz[65];
	TK_AdNn2_Net *tmp;
	byte *tcs, *p_lyr, *p_dat;
	int nl, wszfl;
	int ofs_lyr, ofs_dat, sz_lhdr, ofs_opt;
	int i, j, k;

	if((*(u32 *)buf)!=TK_FCC_NN2)
		return(NULL);
	wszfl=*(u16 *)(buf+4);
	nl=*(u16 *)(buf+6);
	
	sz_lhdr=4;
	if(wszfl&0x10)
		sz_lhdr=8;

	ofs_lyr=8;
	ofs_dat=8+(nl*sz_lhdr);
	ofs_opt=0;
	if(wszfl&0x0100)
	{
		ofs_lyr=*(u16 *)(buf+0xA);
		ofs_dat=*(u16 *)(buf+0xC);
		ofs_opt=ofs_lyr+(nl*sz_lhdr);
	}
	
	p_lyr=buf+ofs_lyr;
	p_dat=buf+ofs_dat;
	
	for(i=0; i<nl; i++)
	{
		tcs=p_lyr+i*sz_lhdr;
		la_isz[i]=*(u16 *)(tcs+0);
		la_osz[i]=*(u16 *)(tcs+2);
		la_afn[i]=0;
		if(wszfl&0x10)
		{
			la_afn[i]=*(u16 *)(tcs+4);
			la_isz[i]|=(*(tcs+6))<<16;
			la_osz[i]|=(*(tcs+7))<<16;
		}
	}
	
	la_sz[0]=la_isz[0];
	for(i=0; i<nl; i++)
		{ la_sz[i+1]=la_osz[i]; }
		
	tmp=TK_AdNn_AllocNet(nl, la_sz, wszfl);

	tcs=p_dat;
	for(i=0; i<nl; i++)
	{
		tmp->layer[i]->nn_afn=la_afn[i]&63;
		tcs=TK_AdNn_LoadNetLayer(tmp, tcs, i, wszfl);
	}
	
	return(tmp);
}
