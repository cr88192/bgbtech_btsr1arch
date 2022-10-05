TkMod_Info *tkm_loadmods;

int TkMod_GetU16BE(byte *buf)
{
	return((buf[0]<<8)|buf[1]);
}

int TkMod_LoadFromBufferS3Ctx(TkMod_Info *ctx, byte *data, int dsz)
{
	TkMod_S3Head *head;
	TkMod_S3SampHead *samp;
	byte *cs, *ct, *cs1, *ct1;
	int maxpat, n_pat, pat;
	int cmda, cmdb, cmdc, cmds, cmdx, cmdy;
	int songlen;
	int i, j, k, l, ls, le;

	head=(TkMod_S3Head *)data;

	memcpy(ctx->modname, head->name, 28);
	
	l=0;
	for(i=0; i<32; i++)
	{
		j=head->chanSetting[i];
		if(j&128)
		{
			ctx->chan_remap[i]=255;
			continue;
		}
		ctx->chan_remap[i]=l;
		l++;
	}
	
	l=(l+3)&(~3);
	
	ctx->n_chan_remap=l;
	
	cs=data+0x60;
	
	ctx->hed2.num_positions=head->orderCnt[0];
	memcpy(ctx->hed2.pattern_tab, cs, head->orderCnt[0]);
	cs+=head->orderCnt[0];

	ctx->nPat=get_u16le(head->patternCnt);

	for(i=0; i<ctx->hed2.num_positions; i++)
	{
		j=ctx->hed2.pattern_tab[i];
		if(j>ctx->nPat)
		{
			ctx->hed2.pattern_tab[i]=ctx->nPat;
		}
	}
	
	ctx->nSamp=get_u16le(head->instrumCnt);
	for(i=0; i<ctx->nSamp; i++)
	{
		ctx->rvaSamp[i]=get_u16le(cs+i*2)*16;
	}
	cs+=ctx->nSamp*2;

	ctx->nPat=get_u16le(head->patternCnt);
	for(i=0; i<ctx->nPat; i++)
	{
		ctx->rvaPat[i]=get_u16le(cs+i*2)*16;
	}
	cs+=ctx->nPat*2;

	ctx->colstride=6;
	ctx->rowstride=ctx->n_chan_remap*6;
	ctx->patstride=ctx->rowstride*64;

	ctx->patterns=malloc((ctx->nPat+1)*ctx->patstride);
	memset(ctx->patterns, 0xFF, (ctx->nPat+1)*ctx->patstride);

	for(i=0; i<ctx->nPat; i++)
	{
		cs=data+ctx->rvaPat[i];
		ct=ctx->patterns+(i*ctx->patstride);
		
		cs+=2;
		for(j=0; j<64; j++)
		{
			k=*cs++;
			while(k)
			{
				ct1=ct+(ctx->chan_remap[k&31]*6);
//				memset(ct1, 0, 6);
//				memset(ct1, 0xFF, 6);
				if(k&32)
				{
					ct1[0]=*cs++;	//note
					ct1[1]=*cs++;	//instrument
				}
				if(k&64)
				{
					ct1[4]=*cs++;	//volume
				}
				if(k&128)
				{
					ct1[2]=*cs++;	//command
					ct1[3]=*cs++;	//info
				}

				k=*cs++;
			}
			
			ct+=ctx->rowstride;
		}
	}

	for(i=0; i<256; i++)
	{
		ctx->samp_samplen[i]=0;
		ctx->samp_volume[i]=0;
		ctx->samp_c2hz[i]=8000;
		ctx->samp_repeat_start[i]=0;
		ctx->samp_repeat_len[i]=0;
		
		ctx->sampofs[i]=0;
		ctx->sampdat[i]="\0";
	}

	l=0;
	for(i=0; i<ctx->nSamp; i++)
	{
		cs=data+ctx->rvaSamp[i];
		samp=(TkMod_S3SampHead *)cs;
		k=get_u32le(samp->szDataLen);
		
		if(samp->flags&4)
			k/=2;
		
		ctx->samp_samplen[i]=k;
		ctx->samp_volume[i]=samp->volume;
		ctx->samp_c2hz[i]=get_u32le(samp->c2spd);
//		ctx->samp_repeat_start[i]=get_u16le(samp->repeat_start);
//		ctx->samp_repeat_len[i]=get_u16le(samp->repeat_len);

		ls=get_u16le(samp->repeat_start);
		le=get_u16le(samp->repeat_len);

		if(samp->flags&4)
		{
			ls=ls/2;
			le=le/2;
		}

		ctx->samp_repeat_start[i]=ls;
		ctx->samp_repeat_len[i]=le-ls;

//		if(!ctx->samp_repeat_len[i])
//		{
//			ctx->samp_repeat_start[i]=k-1;
//			ctx->samp_repeat_len[i]=k;
//		}
		
		ctx->sampofs[i]=l;
		l+=k+4;
	}
	
	ctx->sampbuf=malloc(l+384);
	memset(ctx->sampbuf, 0, l+384);

	for(i=0; i<ctx->nSamp; i++)
	{
		cs=data+ctx->rvaSamp[i];
		samp=(TkMod_S3SampHead *)cs;
		k=get_u32le(samp->szDataLen);
		
		cs1=data+(get_u16le(samp->rvaDataLo)+(samp->rvaDataHi<<16))*16;
		ct1=ctx->sampbuf+ctx->sampofs[i];
		ctx->sampdat[i]=ct1;

		if(samp->flags&4)
			k/=2;

		if(!(samp->flags&1))
		{
			ctx->samp_repeat_start[i]=k-1;
			ctx->samp_repeat_len[i]=1;
		}
		
		if(samp->flags&4)
		{
			for(j=0; j<k; j++)
			{
//				ct1[j]=cs1[j*2+1];
				ct1[j]=cs1[j*2+1]^0x80;
//				ct1[j]=0;
			}
		}else
		{
			for(j=0; j<k; j++)
			{
				ct1[j]=cs1[j]^0x80;
//				ct1[j]=cs1[j];
			}
//			memcpy(ct1, cs1, k);
		}
	}
	
//	ctx->tick_bpm=125;
	ctx->tick_bpm=head->initTempo;
//	ctx->tick_div=6;
	ctx->tick_div=head->initSpeed;
	ctx->mixrate=16000;

	ctx->tick_divmin=(24*ctx->tick_bpm)/ctx->tick_div;
	ctx->tick_divsamp=(ctx->mixrate*60)/ctx->tick_divmin;
	ctx->tick_samp=ctx->tick_divsamp/ctx->tick_div;
	
	printf("divsamp=%d\n", ctx->tick_divsamp);

	songlen=(ctx->hed2.num_positions*64*60)/ctx->tick_divmin;
	printf("length: %ds (%02d:%02d)\n", songlen,
		(songlen/60), (songlen%60));

	ctx->songlen=songlen;

	ctx->magic1=BTM_MAGIC1;
	ctx->magic2=BTM_MAGIC1;
	ctx->magic3=BTM_MAGIC1;
	ctx->magic4=BTM_MAGIC1;

	return(0);
}

int TkMod_LoadFromBufferCtx(TkMod_Info *ctx, byte *data, int dsz)
{
	byte *cs;
	int maxpat, n_pat, pat;
	int cmda, cmdb, cmdc, cmds, cmdx, cmdy;
	int songlen;

	int ls, le;
	int i, j, k, l;
	
	if(	(data[0x1C]==0x1A) &&
		!memcmp(data+0x2C, "SCRM", 4))
	{
		return(TkMod_LoadFromBufferS3Ctx(ctx, data, dsz));
	}
	
	memcpy(ctx->modname, data, 20);
	memcpy(&(ctx->samphead), data+20, 31*30);
	memcpy(&(ctx->hed2), data+20+(31*30), 134);

	for(i=0; i<4; i++)
		ctx->chan_remap[i]=i;
	ctx->n_chan_remap=4;

	ctx->colstride=4;
	ctx->rowstride=16;
	ctx->patstride=1024;

//	printf("%04s\n", ctx->hed2.sig);

	maxpat=0;
	for(i=0; i<ctx->hed2.num_positions; i++)
	{
		j=ctx->hed2.pattern_tab[i];
		if(j>maxpat)
			maxpat=j;
	}
	
	n_pat=maxpat+1;
	printf("n_pat=%d\n", n_pat);

	cs=data+0x438+4;

	ctx->patterns=malloc(n_pat*1024);
	memcpy(ctx->patterns, cs, n_pat*1024);
	cs+=n_pat*1024;
//	cs+=maxpat*1024;
//	cs+=(maxpat+2)*1024;

	k=0;
	for(i=0; i<31; i++)
	{
//		l=	(ctx->samphead[i].samplen[0]<<8) |
//			(ctx->samphead[i].samplen[1]<<0) ;
		l=TkMod_GetU16BE(ctx->samphead[i].samplen);

//		if(l&1)l++;
		
//		l--;
		ctx->sampofs[i]=k;
		k+=l*2;
//		ctx->sampdat[i]=malloc(l*2);
	}
	
	ctx->sampbuf=malloc(k+(64*4));
	memset(ctx->sampbuf, 0, k+(64*4));
//	memcpy(ctx->sampbuf, cs, k);

	for(i=0; i<31; i++)
	{
		l=TkMod_GetU16BE(ctx->samphead[i].samplen);
//		ctx->sampdat[i]=ctx->sampbuf+ctx->sampofs[i];
		ctx->sampdat[i]=ctx->sampbuf+ctx->sampofs[i]+(i*4);
		memcpy(ctx->sampdat[i], cs+ctx->sampofs[i], l*2);

		ls=TkMod_GetU16BE(ctx->samphead[i].repeat_start)*2;
		le=TkMod_GetU16BE(ctx->samphead[i].repeat_len)*2;
		j=TkMod_GetU16BE(ctx->sampdat[i]);
		printf("Sample %d Tag %04X Len=%d Ls=%d Ld=%d\n", i+1, j, l, ls, le);
		
		ctx->samp_volume[i]=ctx->samphead[i].volume;
//		ctx->samp_volume[i]=ctx->samphead[i].volume;
		ctx->samp_samplen[i]=l*2;
		ctx->samp_c2hz[i]=8000;
		ctx->samp_repeat_start[i]=ls;
		ctx->samp_repeat_len[i]=le;
	}

	ctx->tick_bpm=125;
	ctx->tick_div=6;
	ctx->mixrate=16000;

	ctx->tick_divmin=(24*ctx->tick_bpm)/ctx->tick_div;
	ctx->tick_divsamp=(ctx->mixrate*60)/ctx->tick_divmin;
	ctx->tick_samp=ctx->tick_divsamp/ctx->tick_div;
	
	printf("divsamp=%d\n", ctx->tick_divsamp);

	songlen=(ctx->hed2.num_positions*64*60)/ctx->tick_divmin;
	printf("length: %ds (%02d:%02d)\n", songlen,
		(songlen/60), (songlen%60));

	ctx->songlen=songlen;

	ctx->magic1=BTM_MAGIC1;
	ctx->magic2=BTM_MAGIC1;
	ctx->magic3=BTM_MAGIC1;
	ctx->magic4=BTM_MAGIC1;

	return(0);
}

int TkMod_ValidateSongMagic(TkMod_Info *ctx)
{
	if(		(ctx->magic1!=BTM_MAGIC1) ||
			(ctx->magic2!=BTM_MAGIC1) ||
			(ctx->magic3!=BTM_MAGIC1) ||
			(ctx->magic4!=BTM_MAGIC1) )
	{
		debug_break
	}
	return(0);
}

TkMod_Info *TkMod_GetMod(char *name)
{
	TkMod_Info *cur;
	byte *buf;
	int sz, len;

	cur=tkm_loadmods;
	while(cur)
	{
		TkMod_ValidateSongMagic(cur);
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	cur=btm_malloc(sizeof(TkMod_Info));
	memset(cur, 0, sizeof(TkMod_Info));

	cur->name=bccx_strdup(name);
	cur->next=tkm_loadmods;
	tkm_loadmods=cur;

	sz=0;
	buf=BTM_LoadFileTmp(name, &sz);
	if(buf)
	{
		TkMod_LoadFromBufferCtx(cur, buf, sz);
		TkMod_ValidateSongMagic(cur);
	}

	len=16384;
	cur->mixbuf=malloc(len*2);
	cur->mixpos=cur->mixbuf;
	cur->mixpos_cs=cur->mixbuf;
	cur->mixbufs=cur->mixbuf;
	cur->mixbufe=cur->mixbuf+len;

	return(cur);
}

int TkMod_UpdateSongCol(TkMod_Info *ctx, int col, int cmda, int cmdb)
{
	static short s3m_noteperiod[16]={
		1712,1616,1524,1440,1356,1280,1208,1140,1076,1016,960,907};
	int cmdc, cmds, cmdx, cmdy, cmdv, hz, step, ix;
	int i, j, k;

	TkMod_ValidateSongMagic(ctx);

	if(ctx->colstride==4)
	{
		cmds=((cmda>>12)<<4)|(cmdb>>12);
		cmdx=cmda&0x0FFF;
		cmdy=cmdb&0x00FF;
		cmdc=(cmdb>>8)&15;
		cmdv=255;
	}else
		if(ctx->colstride==6)
	{
		cmds=(cmda>>8)&255;
		cmdx=(cmda>>0)&255;
		cmdy=(cmda>>24)&255;
		cmdc=0x100|((cmda>>16)&255);
		
		if(cmdc==0x1FF)
			cmdc=0x100;
		
//		cmds+=1;
		if(cmds==255)
			cmds=0;

		if(cmds!=0)
		{
			ctx->sampc2hz[col]=ctx->samp_c2hz[cmds-1];
		}

		if(cmdx!=0xFF)
		{
			j=(cmdx>>4)&15;
			k=(cmdx&15);
//			hz=262+(k*22);
//			hz=262+(k*22);
//			if(j>=4)
//				hz<<=(j-4);
//			else
//				hz>>=(3-j);
//			cmdx=7093789/(hz*8);

//			hz=8363*16*(s3m_noteperiod[k]>>j)/8363;
//			hz=8363*16*(s3m_noteperiod[k]>>j)/ctx->sampc2hz[col];
			hz=(16*s3m_noteperiod[k])>>j;
			cmdx=hz/4;

		}else
		{
			cmdx=0;
		}

		cmdv=(cmdb>>0)&255;

#if 0
		cmdc=0;
		switch(k)
		{
			case 0x00: cmdc=0;		break;
			case 0x01: cmdc=0x101;	break;
			case 0x02: cmdc=0x102;	break;
			case 0x03: cmdc=0x103;	break;
			case 0x07: cmdc=0x107;	break;
			case 0x14: cmdc=0x114;	break;
			default: cmdc=0;		break;
		}
#endif
	}

//	printf("%02X-%01X-%03X-%02X ", cmds, cmdc, cmdx, cmdy);

	if(cmds!=0)
	{
		if(cmdx!=0)
		{
			ctx->step_fq[col]=cmdx;
			ctx->step_fq_slide[col]=0;
			ctx->step_fq_stgt[col]=0;
		}

		ix=cmds-1;

//		j=TkMod_GetU16BE(ctx->samphead[ix].samplen)*2;
		j=ctx->samp_samplen[ix];
		j=(j*15)/16;
		
//		if((ctx->sampidx[col]!=ix) || !ctx->samp_isloop[col])
//		if(ctx->sampidx[col]!=ix)
//		if((ctx->sampidx[col]!=ix) ||
//			(!ctx->samp_isloop[col] &&
//				((ctx->stepfrac[col]>>12)==ctx->samp_lbeg[col])))
//		if((ctx->sampidx[col]!=ix) ||
//			(!ctx->samp_isloop[col] && (cmdx!=0)))
		if((ctx->sampidx[col]!=ix) || (cmdx!=0))
		{
			ctx->sampidx[col]=ix;
//			ctx->sampvol[col]=ctx->samphead[ix].volume;
			ctx->stepfrac[col]=0;
		}

//		ctx->sampvol[col]=ctx->samphead[ix].volume;
		if(cmdv==255)
			cmdv=ctx->samp_volume[ix];

		if(cmdv>63)
			cmdv=63;
		ctx->sampvol[col]=cmdv;
		ctx->samprefvol[col]=cmdv;

//		j=TkMod_GetU16BE(ctx->samphead[ix].repeat_start)*2;
//		k=TkMod_GetU16BE(ctx->samphead[ix].repeat_len)*2;
		j=ctx->samp_repeat_start[ix];
		k=ctx->samp_repeat_len[ix];

		ctx->sampc2hz[col]=ctx->samp_c2hz[ix];

		if(k<=2)
		{
//			j=TkMod_GetU16BE(ctx->samphead[ix].samplen)*2;
			j=ctx->samp_samplen[ix];
			k=1;
		}

		ctx->samp_lbeg[col]=j;
		ctx->samp_lend[col]=j+k;
		ctx->samp_isloop[col]=(k!=1);
	}else if(cmdv!=255)
	{
		if(cmdv>63)
			cmdv=63;
		ctx->sampvol[col]=cmdv;
		ctx->samprefvol[col]=cmdv;
	}

	switch(cmdc)
	{
	case 0x0:
		break;
	case 0x1:
		ctx->step_fq_slide[col]=-cmdy;
		break;
	case 0x2:
		ctx->step_fq_slide[col]=cmdy;
		break;
	case 0x3:
		if(cmdx>=ctx->step_fq[col])
			ctx->step_fq_slide[col]=cmdy;
		else
			ctx->step_fq_slide[col]=-cmdy;
		ctx->step_fq_stgt[col]=cmdx;
		break;
	case 0x9:
		ctx->stepfrac[col]=cmdy<<8;
		break;
	case 0xA:
		j=(cmdy>>4)&15;
		k=(cmdy>>0)&15;
		if(j!=0)
		{
			ctx->sampvol[col]+=(j-1);
			if(ctx->sampvol[col]>=64)
				ctx->sampvol[col]=63;
		}
		if(k!=0)
		{
			ctx->sampvol[col]-=(k-1);
			if(ctx->sampvol[col]<1)
				ctx->sampvol[col]=1;
		}
		break;

	case 0xB:
		ctx->row+=cmdy;
		break;

	case 0xC:
		ctx->sampvol[col]=cmdy;
		break;

	case 0xE:
//		printf("CMD E: %02X\n", cmdy);

		if((cmdy&0xF0)==0x10)
			ctx->step_fq[col]-=cmdy&0x0F;
		if((cmdy&0xF0)==0x20)
			ctx->step_fq[col]+=cmdy&0x0F;
		break;

	case 0xF:
//		ctx->sampvol[col]=cmdy;
		if(!cmdy)
			cmdy=1;
		if(cmdy<=32)
			ctx->tick_div=cmdy;
		else
			ctx->tick_bpm=cmdy;
		ctx->tick_divmin=(24*ctx->tick_bpm)/ctx->tick_div;
		ctx->tick_divsamp=(ctx->mixrate*60)/ctx->tick_divmin;
		ctx->tick_samp=ctx->tick_divsamp/ctx->tick_div;
		break;

	case 0x100:
		break;

	case 0x101:
//		ctx->sampvol[col]=cmdy;
		if(!cmdy)
			break;
		if(cmdy<=32)
			ctx->tick_div=cmdy;
		ctx->tick_divmin=(24*ctx->tick_bpm)/ctx->tick_div;
		ctx->tick_divsamp=(ctx->mixrate*60)/ctx->tick_divmin;
		ctx->tick_samp=ctx->tick_divsamp/ctx->tick_div;
		break;

	case 0x102:
		ctx->row=cmdy<<6;
		break;

	case 0x103:
		k=((cmdy>>4)*10)+(cmdy&15);
		if(k>=64)
			break;
		
		ctx->row=(ctx->row+63)&(~63);
		ctx->row+=k;
		break;
	case 0x104:
		if(cmdy==0)
			cmdy=ctx->sampeffvol[col];
		ctx->sampeffvol[col]=cmdy;
	
		j=(cmdy>>4)&15;
		k=(cmdy>>0)&15;
		if((j>0) && (j<15) && ((k==0) || k==15))
		{
			ctx->sampvol[col]+=(j-1);
			if(ctx->sampvol[col]>=64)
				ctx->sampvol[col]=63;
		}
		if((k>0) && (k<15) && ((j==0) || j==15))
		{
			ctx->sampvol[col]-=(k-1);
			if(ctx->sampvol[col]<1)
				ctx->sampvol[col]=1;
		}
		break;

	case 0x107:

//		j=(cmdy>>4)&15;
//		k=(cmdy&15);
//		hz=(16*s3m_noteperiod[k])>>j;
//		cmdy=hz/4;

		if(cmdx>=ctx->step_fq[col])
			ctx->step_fq_slide[col]=cmdy;
		else
			ctx->step_fq_slide[col]=-cmdy;
		ctx->step_fq_stgt[col]=cmdx;
		break;

	case 0x114:
		if(cmdy<=32)
			break;

		ctx->tick_bpm=cmdy;
		ctx->tick_divmin=(24*ctx->tick_bpm)/ctx->tick_div;
		ctx->tick_divsamp=(ctx->mixrate*60)/ctx->tick_divmin;
		ctx->tick_samp=ctx->tick_divsamp/ctx->tick_div;
		break;

	default:
		printf("Unhandled Effect %X\n", cmdc);
		break;
	}

	cmdx=ctx->step_fq[col];
	if(cmdx)
	{
//		hz=7093789/(cmdx*2);
//		step=(hz<<12)/ctx->mixrate;

		hz=7093789/(cmdx*4);
		step=(((s64)hz)*ctx->sampc2hz[col])/ctx->mixrate;

		ctx->steprate[col]=step;
	//	ctx->step_fq[col]=cmdx;
	}

	TkMod_ValidateSongMagic(ctx);
	return(0);
}

int TkMod_StepSongRow(TkMod_Info *ctx)
{
	byte *cs;
	int pat, row, cmda, cmdb, cmdc;
	int i, j, k;
	
	if(!(ctx->hed2.num_positions))
		return(0);
	
	TkMod_ValidateSongMagic(ctx);

	row=ctx->row;
	ctx->row=row+1;
	i=row>>6;
	
	while(i>=ctx->hed2.num_positions)
	{
		row-=(ctx->hed2.num_positions<<6);
		ctx->row=row+1;
		i=row>>6;
	}

	pat=ctx->hed2.pattern_tab[i];
//	printf("Pattern %d\n", pat);

//	cs=ctx->patterns + pat*1024;
//	cs+=(row&63)*16;

	cs=ctx->patterns + (pat*ctx->patstride);
	cs+=(row&63)*(ctx->rowstride);

//	for(k=0; k<4; k++)
	for(k=0; k<ctx->n_chan_remap; k++)
	{
		if(ctx->colstride==4)
		{
			cmda=(cs[0]<<8)|cs[1];
			cmdb=(cs[2]<<8)|cs[3];
			cs+=4;
		}else if(ctx->colstride==6)
		{
			cmda=get_u32le(cs+0);
			cmdb=get_u16le(cs+4);
			cs+=6;
		}
		TkMod_UpdateSongCol(ctx, k, cmda, cmdb);
//		printf("%02X-%01X-%03X-%02X ", cmds, cmdc, cmdx, cmdy);
	}

	TkMod_ValidateSongMagic(ctx);
//	printf("\n");
	return(0);
}

int TkMod_StepMixCtx4W(TkMod_Info *ctx, int cnt, int chofs)
{
//	sbyte *sd0, *sd1, *sd2, *sd3;
	byte *sd0, *sd1, *sd2, *sd3;
	s16 *ct;
	int sf0, sf1, sf2, sf3;
	int sr0, sr1, sr2, sr3;
	int sv0, sv1, sv2, sv3;
	int slb0, slb1, slb2, slb3;
	int sle0, sle1, sle2, sle3;
	int sld0, sld1, sld2, sld3;
	int j0, j1, j2, j3;
	int j4, j5, j6, j7;
	int i0, i1, i2, i3;
	int i4, i5, i6, i7;
	int ch0, ch1, ch2, ch3;
	int cmdx, hz, step;
	int i, j, k;

	if(!(ctx->hed2.num_positions))
		return(0);

	ct=ctx->mixpos;

	TkMod_ValidateSongMagic(ctx);

//	return(0);

	if(ct<(ctx->mixbufs))
		{ debug_break }
	if((ct+cnt)>(ctx->mixbufe))
		{ debug_break }
//		return(-1);

	ch0=chofs+0;
	ch1=chofs+1;
	ch2=chofs+2;
	ch3=chofs+3;

	i0=ctx->sampidx[ch0];	i1=ctx->sampidx[ch1];
	i2=ctx->sampidx[ch2];	i3=ctx->sampidx[ch3];
	sd0=ctx->sampdat[i0];	sd1=ctx->sampdat[i1];
	sd2=ctx->sampdat[i2];	sd3=ctx->sampdat[i3];
	sf0=ctx->stepfrac[ch0];	sf1=ctx->stepfrac[ch1];
	sf2=ctx->stepfrac[ch2];	sf3=ctx->stepfrac[ch3];
	sr0=ctx->steprate[ch0];	sr1=ctx->steprate[ch1];
	sr2=ctx->steprate[ch2];	sr3=ctx->steprate[ch3];
	sv0=ctx->sampvol[ch0];	sv1=ctx->sampvol[ch1];
	sv2=ctx->sampvol[ch2];	sv3=ctx->sampvol[ch3];
	
	slb0=ctx->samp_lbeg[ch0]<<12;	sle0=ctx->samp_lend[ch0]<<12;
	slb1=ctx->samp_lbeg[ch1]<<12;	sle1=ctx->samp_lend[ch1]<<12;
	slb2=ctx->samp_lbeg[ch2]<<12;	sle2=ctx->samp_lend[ch2]<<12;
	slb3=ctx->samp_lbeg[ch3]<<12;	sle3=ctx->samp_lend[ch3]<<12;

	sld0=sle0-slb0;	sld1=sle1-slb1;
	sld2=sle2-slb2;	sld3=sle3-slb3;

	if((	sv0|sv1|sv2|sv3|
			sr0|sr1|sr2|sr3|
			sld0|sld1|sld2|sld3)==0)
	{
		if(!chofs)
		{
			memset(ct, 0, cnt*sizeof(s16));
		}
		ctx->mixpos=ct+cnt;
		return(0);
	}
	
#if 1

	for(i=0; i<cnt; i++)
	{
		j0=sf0>>12;		j1=sf1>>12;
		j2=sf2>>12;		j3=sf3>>12;
//		i0=sd0[i0];		i1=sd1[i1];
//		i2=sd2[i2];		i3=sd3[i3];

//		i0=sd0[i0]-128;		i1=sd1[i1]-128;
//		i2=sd2[i2]-128;		i3=sd3[i3]-128;

		j4=j0+1;
		j5=j1+1;
		j6=j2+1;
		j7=j3+1;
		
		if(j4>(sle0>>12))j4-=(sld0>>12);
		if(j5>(sle1>>12))j5-=(sld1>>12);
		if(j6>(sle2>>12))j6-=(sld2>>12);
		if(j7>(sle3>>12))j7-=(sld3>>12);

		i0=(sbyte)(sd0[j0]);		i1=(sbyte)(sd1[j1]);
		i2=(sbyte)(sd2[j2]);		i3=(sbyte)(sd3[j3]);
		i4=(sbyte)(sd0[j4]);		i5=(sbyte)(sd1[j5]);
		i6=(sbyte)(sd2[j6]);		i7=(sbyte)(sd3[j7]);

		j4=(sf0&4095);
		j5=(sf1&4095);
		j6=(sf2&4095);
		j7=(sf3&4095);

		i0=((i0*(4096-j4))+(i4*j4))>>12;
		i1=((i1*(4096-j5))+(i5*j5))>>12;
		i2=((i2*(4096-j6))+(i6*j6))>>12;
		i3=((i3*(4096-j7))+(i7*j7))>>12;

		k=(sv0*i0)+(sv1*i1)+(sv2*i2)+(sv3*i3);
		
		if(ch0)
			{ k+=ct[i]; }
		
		if(k<-32767)
			k=-32767;
		if(k>32767)
			k=32767;
		
		ct[i]=k;
		sf0+=sr0;	sf1+=sr1;
		sf2+=sr2;	sf3+=sr3;
		
		while(sf0>sle0)		sf0-=sld0;
		while(sf1>sle1)		sf1-=sld1;
		while(sf2>sle2)		sf2-=sld2;
		while(sf3>sle3)		sf3-=sld3;
	}

#endif

	ctx->stepfrac[ch0]=sf0;	ctx->stepfrac[ch1]=sf1;
	ctx->stepfrac[ch2]=sf2;	ctx->stepfrac[ch3]=sf3;
	ctx->mixpos=ct+cnt;
	
//	for(i=0; i<4; i++)
	for(i=ch0; i<=ch3; i++)
	{
		if(ctx->step_fq_slide[i])
		{
			cmdx=ctx->step_fq[i];
			cmdx+=ctx->step_fq_slide[i];
			if(ctx->step_fq_stgt[i])
			{
				if(ctx->step_fq_slide[i]<0)
				{
					if(cmdx<ctx->step_fq_stgt[i])
					{
						cmdx=ctx->step_fq_stgt[i];
						ctx->step_fq_slide[i]=0;
					}
				}else
				{
					if(cmdx>ctx->step_fq_stgt[i])
					{
						cmdx=ctx->step_fq_stgt[i];
						ctx->step_fq_slide[i]=0;
					}
				}
			}else
			{
				if(cmdx<113)cmdx=113;
				if(cmdx>856)cmdx=856;
			}
			hz=7093789/(cmdx*2);
			step=(hz<<12)/ctx->mixrate;
			ctx->steprate[i]=step;
			ctx->step_fq[i]=cmdx;
		}
	}

	if(ct<(ctx->mixbufs))
		{ debug_break }
	if(ctx->mixpos>(ctx->mixbufe))
		{ debug_break }

	TkMod_ValidateSongMagic(ctx);

	return(0);
}

int TkMod_StepMixCtx(TkMod_Info *ctx, int cnt)
{
	u16 *mp0;
	int i, j, k;

	if(ctx->n_chan_remap<=4)
	{
		TkMod_StepMixCtx4W(ctx, cnt, 0);
		return(0);
	}
	
	mp0=ctx->mixpos;
	for(i=0; i<ctx->n_chan_remap; i+=4)
	{
		ctx->mixpos=mp0;
		TkMod_StepMixCtx4W(ctx, cnt, i);
	}
	return(0);
}

#if 0
int main(int argc, char *argv[])
{
	TkMod_Info tctx;
	TkMod_Info *ctx;
	byte *ibuf;
	int isz, len;
	int i, j, k;

	ibuf=TkMod_LoadFile("musix-shine.mod", &isz);
//	ibuf=TkMod_LoadFile("musix-kids.mod", &isz);
//	ibuf=TkMod_LoadFile("BGB_Noise1.mod", &isz);
	
	ctx=&tctx;
	memset(ctx, 0, sizeof(TkMod_Info));
	TkMod_LoadFromBufferCtx(ctx, ibuf, isz);

	len=(ctx->songlen+1)*16000;
	ctx->mixbuf=malloc(len*2);
	ctx->mixpos=ctx->mixbuf;
	ctx->mixbufe=ctx->mixbuf+len;

	while((ctx->mixpos+ctx->tick_divsamp)<ctx->mixbufe)
	{
		TkMod_StepSongRow(ctx);
//		TkMod_StepMixCtx(ctx, ctx->tick_divsamp);
		for(i=0; i<ctx->tick_div; i++)
			TkMod_StepMixCtx(ctx, ctx->tick_samp);
	}

	BGBMID_StoreWAV("out0.wav", (byte *)(ctx->mixbuf), 1, 16000, 16, len);

}
#endif
