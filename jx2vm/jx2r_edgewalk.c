#define BJX2_EDGEWALK_FEATUREMASK	0x0000FFF0

u64 bjx2_edgewalk_regs[64];

u64 bjx2_edgewalk_regs0l;

u32 bjx2_edgewalk_addr_cbuf;
u32 bjx2_edgewalk_addr_zbuf;
u32 bjx2_edgewalk_addr_ubuf;

s16 bjx2_edgewalk_var_scany;
s16 bjx2_edgewalk_var_scanymax;
u16 bjx2_edgewalk_var_scanystr;

int bjx2_edgewalk_var_scanlfx;
int bjx2_edgewalk_var_scanrtx;
int bjx2_edgewalk_var_steplfx;
int bjx2_edgewalk_var_steprtx;

int bjx2_edgewalk_var_scanlfz;
int bjx2_edgewalk_var_scanrtz;
int bjx2_edgewalk_var_steplfz;
int bjx2_edgewalk_var_steprtz;

int bjx2_edgewalk_var_scanlfs;
int bjx2_edgewalk_var_scanrts;
int bjx2_edgewalk_var_steplfs;
int bjx2_edgewalk_var_steprts;

int bjx2_edgewalk_var_scanlft;
int bjx2_edgewalk_var_scanrtt;
int bjx2_edgewalk_var_steplft;
int bjx2_edgewalk_var_steprtt;

u16 bjx2_edgewalk_var_scanlfr;
u16 bjx2_edgewalk_var_scanrtr;
u16 bjx2_edgewalk_var_steplfr;
u16 bjx2_edgewalk_var_steprtr;

u16 bjx2_edgewalk_var_scanlfg;
u16 bjx2_edgewalk_var_scanrtg;
u16 bjx2_edgewalk_var_steplfg;
u16 bjx2_edgewalk_var_steprtg;

u16 bjx2_edgewalk_var_scanlfb;
u16 bjx2_edgewalk_var_scanrtb;
u16 bjx2_edgewalk_var_steplfb;
u16 bjx2_edgewalk_var_steprtb;

u16 bjx2_edgewalk_var_scanlfa;
u16 bjx2_edgewalk_var_scanrta;
u16 bjx2_edgewalk_var_steplfa;
u16 bjx2_edgewalk_var_steprta;

u16 bjx2_edgewalk_var_clip_x0;
u16 bjx2_edgewalk_var_clip_y0;
u16 bjx2_edgewalk_var_clip_x1;
u16 bjx2_edgewalk_var_clip_y1;

u64 tkra_pmuluhw(u64 a, u64 b);
u64 tkra_rgbupck64(u16 a);
u64 tkra_rgb32upck64(u32 a);
u32 BJX2_PMORT_U16(u16 x);
u64 TKRA_CachedBlkUtx2(u64 blk, int ix);

u32 bjx2_edgewalk_var_pixtot;
u32 bjx2_edgewalk_var_pixzpass;

u64 tkra_rgbupck64b(u16 a)
{
	int ax, ay, az, aw, av;
	u64 c;

//	av=(a&0x0001)|((a&0x0020)>>5)|((a&0x0400)>>10);
	av=(a&0x0001)|((a&0x0020)>>4)|((a&0x0400)>>8);
//	av=av<<13;
//	ax=(a&0x001F)<<11;
//	ay=(a&0x03E0)<< 6;
//	az=(a&0x7C00)<< 1;

	av=av<<5;
	ax=(a&0x001F)<<3;
	ay=(a&0x03E0)>>2;
	az=(a&0x7C00)>>7;

#if 0
	if(a&0x8000)
	{
		ax&=0xF0;
		ay&=0xF0;
		az&=0xF0;
	}
#endif
	
	ax|=(ax>>5);	ay|=(ay>>5);	az|=(az>>5);
	ax=0x80|(ax<<8);	ay=0x80|(ay<<8);
	az=0x80|(az<<8);	av=0x80|(av<<8);

	aw=(a&0x8000)?av:0xFF80;
	c=	(((u64)ax)<< 0) | (((u64)ay)<<16) |
		(((u64)az)<<32) | (((u64)aw)<<48) ;
	return(c);
	
}

u16 bjx2_rgb64pck16b(u64 vs)
{
	u16 vn, av;

	vn=	((vs>>11)&0x001F)|
		((vs>>22)&0x03E0)|
		((vs>>33)&0x7C00);

#if 1
//	av=(vs>>60)&15;
	av=(vs>>60)&0xE;
//	if(av!=15)
	if(av!=0xE)
	{
		vn&=0x7BDE;
		vn|=0x8000;
		if(av&8)
			vn|=0x0400;
		if(av&4)
			vn|=0x0020;
		if(av&2)
			vn|=0x0001;
	}
#endif
	
	return(vn);
}

u32 bjx2_rgb64pck32(u64 vs)
{
	u32 tv;
	tv=	(((vs>>56)&0xFF)<<24) |
		(((vs>>40)&0xFF)<<16) |
		(((vs>>24)&0xFF)<< 8) |
		(((vs>> 8)&0xFF)<< 0) ;
	return(tv);
}

// #define JX2_EDGEWALK_XSUB	0
// #define JX2_EDGEWALK_ZSUB	0
#define JX2_EDGEWALK_STSUB	4
#define JX2_EDGEWALK_STHI	6

#define JX2_EDGEWALK_XSUB	4
#define JX2_EDGEWALK_XHI	0

#define JX2_EDGEWALK_ZSUB	4
#define JX2_EDGEWALK_ZHI	0

static u16 jx2_edgewalk_rcptab[1024];

u64 BJX2_MemEdgeWalk_LerpRgbFrac16(u64 v0, u64 v1, int fr)
{
	u64 v2;
	int cr0, cr1, cg0, cg1, cb0, cb1, ca0, ca1;
	int cr2, cg2, cb2, ca2;
	
	ca0=(v0>>48)&0xFFFF;	cr0=(v0>>32)&0xFFFF;
	cg0=(v0>>16)&0xFFFF;	cb0=(v0>> 0)&0xFFFF;

	ca1=(v1>>48)&0xFFFF;	cr1=(v1>>32)&0xFFFF;
	cg1=(v1>>16)&0xFFFF;	cb1=(v1>> 0)&0xFFFF;
	
	fr&=15;
	cr2=(cr0*(16-fr))+(cr1*fr)>>4;
	cg2=(cg0*(16-fr))+(cg1*fr)>>4;
	cb2=(cb0*(16-fr))+(cb1*fr)>>4;
	ca2=(ca0*(16-fr))+(ca1*fr)>>4;
	
	v2=
		(((u64)ca2)<<48) |
		(((u64)cr2)<<32) |
		(((u64)cg2)<<16) |
		(((u64)cb2)<< 0) ;
	return(v2);
}

u64 jx2_edgewalk_utxcache_dt[1024];
u16 jx2_edgewalk_utxcache_ix[1024];
u16 jx2_edgewalk_utxcache_ax[1024];

s64 jx2_edgewalk_busycyc;

int BJX2_MemEdgeWalk_ProbeUpdateXspan(BJX2_Context *ctx)
{
	u64 blk_u, rgb_u, rgb_m, rgb_c, rgb_bl;
	u64 rgb_ms, rgb_md, rgb_bs, rgb_bd;
	u32 blk_c, blk_z;
	u64 addr_o, addr_c, addr_z, addr_u;
	int x0, x1, z0, z1, s0, s1, t0, t1, rcp;
	int r0, r1, g0, g1, b0, b1, a0, a1;
	int x, z, s, t, zs, ss, ts, z2, s_p, t_p, z_p;
	int r, g, b, a, rs, gs, bs, as, ix, ax;
	int txs, txt, txi, ztest, atest, zmask, cmask, islin;
	int bfs, bfd, txm, zfn, zpn, ispersp, isrcpz, isdword, issten;
	int blk_st, ref_st, upd_st, stfn, st_op, zupd;
	int stpn;
	int i, j, k;

//	x0=((s16)bjx2_edgewalk_var_scanlfx)>>6;
//	x1=((s16)bjx2_edgewalk_var_scanrtx)>>6;

	x0=bjx2_edgewalk_var_scanlfx>>(6+JX2_EDGEWALK_XSUB);
	x1=bjx2_edgewalk_var_scanrtx>>(6+JX2_EDGEWALK_XSUB);

	bfs=(bjx2_edgewalk_regs[0]>> 8)&15;
	bfd=(bjx2_edgewalk_regs[0]>>12)&15;

	zmask=(bjx2_edgewalk_regs[0]>>7)&1;
	ztest=(bjx2_edgewalk_regs[0]>>6)&1;
	cmask=(bjx2_edgewalk_regs[0]>>5)&1;
	atest=(bjx2_edgewalk_regs[0]>>4)&1;

	islin=(bjx2_edgewalk_regs[0]>>16)&1;
	zfn=(bjx2_edgewalk_regs[0]>>17)&7;

	ispersp=(bjx2_edgewalk_regs[0]>>20)&1;
	isrcpz=(bjx2_edgewalk_regs[0]>>21)&1;
	issten=(bjx2_edgewalk_regs[0]>>22)&1;
	isdword=(bjx2_edgewalk_regs[0]>>23)&1;

	stfn	= (bjx2_edgewalk_regs[17]>>4)&7;
	ref_st	= (bjx2_edgewalk_regs[17]>>16)&0xFF;

	ax=(bjx2_edgewalk_addr_ubuf^(bjx2_edgewalk_addr_ubuf>>16))&0xFFFF;

	x=x1-x0;
	if(x<0)
		return(0);
	
	if(!ctx->no_memcost)
	{
		jx2_edgewalk_busycyc+=(x+1)*8+10;
	}
		
//	rcp=0x10000/(x+1);
	rcp=jx2_edgewalk_rcptab[x+1];

	z0=bjx2_edgewalk_var_scanlfz;
	z1=bjx2_edgewalk_var_scanrtz;
	s0=bjx2_edgewalk_var_scanlfs;
	s1=bjx2_edgewalk_var_scanrts;
	t0=bjx2_edgewalk_var_scanlft;
	t1=bjx2_edgewalk_var_scanrtt;

	r0=bjx2_edgewalk_var_scanlfr;
	r1=bjx2_edgewalk_var_scanrtr;
	g0=bjx2_edgewalk_var_scanlfg;
	g1=bjx2_edgewalk_var_scanrtg;
	b0=bjx2_edgewalk_var_scanlfb;
	b1=bjx2_edgewalk_var_scanrtb;
	a0=bjx2_edgewalk_var_scanlfa;
	a1=bjx2_edgewalk_var_scanrta;

	zs=(((s64)(z1-z0))*rcp)>>16;
	ss=(((s64)(s1-s0))*rcp)>>16;
	ts=(((s64)(t1-t0))*rcp)>>16;

	rs=(((s64)(r1-r0))*rcp)>>16;
	gs=(((s64)(g1-g0))*rcp)>>16;
	bs=(((s64)(b1-b0))*rcp)>>16;
	as=(((s64)(a1-a0))*rcp)>>16;

	if(
		(bjx2_edgewalk_var_scany<bjx2_edgewalk_var_clip_y0) ||
		(bjx2_edgewalk_var_scany>bjx2_edgewalk_var_clip_y1))
	{
//		printf("BJX2_MemEdgeWalk_ProbeUpdateXspan: Scan Y out of range %d\n",
//			bjx2_edgewalk_var_scany);
		return(0);
	}

//	addr_o=(bjx2_edgewalk_var_scany*bjx2_edgewalk_var_scanystr+x0)&(~3);
	addr_o=bjx2_edgewalk_var_scany*bjx2_edgewalk_var_scanystr;
	
	txm=(bjx2_edgewalk_regs[3]>>52)&31;
	txm=(1<<txm)-1;
	
//	blkx=-1;
	
	x=x0; z=z0; s=s0; t=t0;
	r=r0; g=g0; b=b0; a=a0;
	while(x<=x1)
	{
		if((x<bjx2_edgewalk_var_clip_x0) || (x>=bjx2_edgewalk_var_clip_x1))
		{
//			printf("BJX2_MemEdgeWalk_ProbeUpdateXspan: Scan X out of range %d\n",
//				x);

			z+=zs;
			s+=ss;	t+=ts;
			r+=rs;	g+=gs;
			b+=bs;	a+=as;
			x++;
//			return(0);
			continue;
		}

		if(isdword)
		{
			addr_c=bjx2_edgewalk_addr_cbuf+(addr_o+x)*4;
			addr_z=bjx2_edgewalk_addr_zbuf+(addr_o+x)*4;
			blk_c=BJX2_MemGetDWord_NoAT(ctx, addr_c, 0);
			blk_z=BJX2_MemGetDWord_NoAT(ctx, addr_z, 0);
			
			blk_st=blk_z&0xFF;
			blk_z>>=16;
		}else
		{
			addr_c=bjx2_edgewalk_addr_cbuf+(addr_o+x)*2;
			addr_z=bjx2_edgewalk_addr_zbuf+(addr_o+x)*2;
			blk_c=BJX2_MemGetWord_NoAT(ctx, addr_c, 0);
			blk_z=BJX2_MemGetWord_NoAT(ctx, addr_z, 0);
			blk_st=blk_z&0x000F;
		}
		
		upd_st=blk_st;

		if(isrcpz)
		{
			z_p=(1LL<<(2*(16+JX2_EDGEWALK_ZSUB)))/z;
		}else
		{
			z_p=z;
		}

		if(ispersp)
		{
			s_p=((s64)s*z_p)>>(16+JX2_EDGEWALK_ZSUB);
			t_p=((s64)t*z_p)>>(16+JX2_EDGEWALK_ZSUB);
		}else
		{
			s_p=s;
			t_p=t;
		}

		txs=s_p>>(6+JX2_EDGEWALK_STSUB);
		txt=t_p>>(6+JX2_EDGEWALK_STSUB);
		txi=	(BJX2_PMORT_U16(txt)<<1)|
				(BJX2_PMORT_U16(txs)   );
		txi&=txm;

		ix=(txi>>4)&0xFFFF;
		j=ix&1023;
		if(	(jx2_edgewalk_utxcache_ix[j]==ix) &&
			(jx2_edgewalk_utxcache_ax[j]==ax))
		{
			blk_u=jx2_edgewalk_utxcache_dt[j];
		}else
		{
			addr_u=bjx2_edgewalk_addr_ubuf+((txi>>4)<<3);
			blk_u=BJX2_MemGetQWord_NoAT(ctx, addr_u, 0);
			jx2_edgewalk_utxcache_dt[j]=blk_u;
			jx2_edgewalk_utxcache_ix[j]=ix;
			jx2_edgewalk_utxcache_ax[j]=ax;
		}

		if(isdword)
		{
			rgb_c=tkra_rgb32upck64(blk_c);
		}else
		{
			rgb_c=tkra_rgbupck64b(blk_c);
		}
		rgb_u=TKRA_CachedBlkUtx2(blk_u, txi);
		
//		if(((rgb_u>>61)&7)==7)
//		{
//			rgb_u|=0x1C00000000000000ULL;
//		}

		if(islin)
		{
			txi=	(BJX2_PMORT_U16(txt+0)<<1)|
					(BJX2_PMORT_U16(txs+1)   );
			txi&=txm;
			addr_u=bjx2_edgewalk_addr_ubuf+((txi>>4)<<3);
			blk_u=BJX2_MemGetQWord_NoAT(ctx, addr_u, 0);

			rgb_bs=TKRA_CachedBlkUtx2(blk_u, txi);

			txi=	(BJX2_PMORT_U16(txt+1)<<1)|
					(BJX2_PMORT_U16(txs+0)   );
			txi&=txm;
			addr_u=bjx2_edgewalk_addr_ubuf+((txi>>4)<<3);
			blk_u=BJX2_MemGetQWord_NoAT(ctx, addr_u, 0);

			rgb_bd=TKRA_CachedBlkUtx2(blk_u, txi);
			
			rgb_bs=BJX2_MemEdgeWalk_LerpRgbFrac16(rgb_u, rgb_bs,
				(s_p>>(2+JX2_EDGEWALK_STSUB))&15);
			rgb_bd=BJX2_MemEdgeWalk_LerpRgbFrac16(rgb_u, rgb_bd,
				(t_p>>(2+JX2_EDGEWALK_STSUB))&15);
			rgb_u=BJX2_MemEdgeWalk_LerpRgbFrac16(rgb_bs, rgb_bd, 8);

//			if(((rgb_u>>61)&7)==7)
//			{
//				rgb_u|=0x1C00000000000000ULL;
//			}

		}

		rgb_m=(((u64)a)<<48)|(((u64)r)<<32)|(((u64)g)<<16)|(((u64)b)<<0);

//		rgb_u=0xFFFF7FFF7FFF7FFFULL;
//		rgb_m=0xFFFF7FFF7FFF7FFFULL;
		
		rgb_m=tkra_pmuluhw(rgb_m, rgb_u);

		switch(bfs)
		{
			case 0:		case 1:
			case 2:		case 3:
			case 4:		case 5:
				rgb_ms=0xFFFFFFFFFFFFFFFFULL;
				break;
			case 6: rgb_ms=0x0000000000000000ULL; break;
			case 7: rgb_ms=0xFFFFFFFFFFFFFFFFULL; break;

			case 8:		case 12:
				rgb_ms=rgb_m>>48;
				rgb_ms|=rgb_ms<<16;		rgb_ms|=rgb_ms<<32;
				if(bfs&4)rgb_ms=~rgb_ms;
				break;
			case 9:		case 13:
				rgb_ms=(rgb_m>>16)&0xFFFF;
				rgb_ms|=rgb_ms<<16;		rgb_ms|=rgb_ms<<32;
				if(bfs&4)rgb_ms=~rgb_ms;
//				rgb_ms&=~0xFFFF000000000000ULL;
				break;
			case 10:	case 14:
				rgb_ms=rgb_c>>48;
				rgb_ms|=rgb_ms<<16;		rgb_ms|=rgb_ms<<32;
				if(bfs&4)rgb_ms=~rgb_ms;
				break;
			case 11:	case 15:
				rgb_ms=(rgb_c>>16)&0xFFFF;
				rgb_ms|=rgb_ms<<16;		rgb_ms|=rgb_ms<<32;
				if(bfs&4)rgb_ms=~rgb_ms;
//				rgb_ms&=~0xFFFF000000000000ULL;
				break;
		}

		switch(bfd)
		{
			case 0:		case 1:
			case 2:		case 3:
			case 4:		case 5:
				rgb_md=0x0000000000000000ULL;
				break;
			case 6: rgb_md=0x0000000000000000ULL; break;
			case 7: rgb_md=0xFFFFFFFFFFFFFFFFULL; break;

			case 8:		case 12:
				rgb_md=rgb_m>>48;
				rgb_md|=rgb_md<<16;		rgb_md|=rgb_md<<32;
				if(bfd&4)rgb_md=~rgb_md;
				break;
			case 9:		case 13:
				rgb_md=(rgb_m>>16)&0xFFFF;
				rgb_md|=rgb_md<<16;		rgb_md|=rgb_md<<32;
				if(bfd&4)rgb_md=~rgb_md;
				rgb_md|=0xFFFF000000000000ULL;
				break;
			case 10:	case 14:
				rgb_md=rgb_c>>48;
				rgb_md|=rgb_md<<16;		rgb_md|=rgb_md<<32;
				if(bfd&4)rgb_md=~rgb_md;
				break;
			case 11:	case 15:
				rgb_md=(rgb_c>>16)&0xFFFF;
				rgb_md|=rgb_md<<16;		rgb_md|=rgb_md<<32;
				if(bfd&4)rgb_md=~rgb_md;
				rgb_md|=0xFFFF000000000000ULL;
				break;
		}

		rgb_bs=tkra_pmuluhw(rgb_m, rgb_ms);
		rgb_bd=tkra_pmuluhw(rgb_c, rgb_md);
		rgb_bl=rgb_bs+rgb_bd;

//		rgb_bl=0xFFFF7FFF7FFF7FFFULL;

		if(isdword)
		{
			blk_c=bjx2_rgb64pck32(rgb_bl);
		}else
		{
#if 0
			if(((rgb_bl>>60)&15)!=15)
			{
				printf("EdgeWalk: Alpha in Output\n");
				printf(	"  Out=%016llX Mod-Src=%016llX\n"
						"  Dst=%016llX Tex=%016llX\n"
						"  UTX=%016llX\n",
					rgb_bl, rgb_m,
					rgb_c, rgb_u,
					blk_u);
			}
#endif

			blk_c=bjx2_rgb64pck16b(rgb_bl);
		}

		z2=z>>JX2_EDGEWALK_ZSUB;
//		z2=z;

		if(issten && !isdword)
		{
			z2&=0xFFF0;
			blk_z&=0xFFF0;
		}

		switch(zfn)
		{
			case 0: zpn=(z2< blk_z); break;
			case 1: zpn=(z2>=blk_z); break;
			case 2: zpn=(z2<=blk_z); break;
			case 3: zpn=(z2> blk_z); break;
			case 4: zpn=(z2==blk_z); break;
			case 5: zpn=(z2!=blk_z); break;
			case 6: zpn=1; break;
			case 7: zpn=0; break;
			default: zpn=1; break;
		}
		
		stpn=1;
		if(issten)
		{
			switch(stfn)
			{
				case 0: stpn=ref_st< blk_st;	break;
				case 1: stpn=ref_st>=blk_st;	break;
				case 2: stpn=ref_st<=blk_st;	break;
				case 3: stpn=ref_st> blk_st;	break;
				case 4: stpn=ref_st==blk_st;	break;
				case 5: stpn=ref_st!=blk_st;	break;
				case 6: stpn=1; break;
				case 7: stpn=0; break;
			}

			if(stpn)
			{
				if(zpn)
					st_op=(bjx2_edgewalk_regs[17]>>13)&7;
				else
					st_op=(bjx2_edgewalk_regs[17]>>10)&7;
			}else
			{
				st_op=(bjx2_edgewalk_regs[17]>>7)&7;
			}
		}

		switch(st_op)
		{
			case 0: upd_st=blk_st;		break;
			case 1: upd_st=0;			break;
			case 2: upd_st=~blk_st;		break;
			case 3: upd_st=ref_st;		break;
			case 4:
				upd_st=blk_st+1;
				if(upd_st>255)
					upd_st=255;
				break;
			case 5:
				upd_st=blk_st-1;
				if(upd_st<0)
					upd_st=0;
				break;
			case 6: upd_st=blk_st+1;	break;
			case 7: upd_st=blk_st-1;	break;
		}

		bjx2_edgewalk_var_pixtot++;
		if(zpn)
			bjx2_edgewalk_var_pixzpass++;

		zupd=0;
//		if((z2<=blk_z) || !ztest)
		if(zpn || !ztest)
		{
			if(((rgb_bl>>63)&1) || !atest)
			{
				if(cmask && stpn)
				{
					if(isdword)
					{
						BJX2_MemSetDWord_NoAT(ctx, addr_c, 0, blk_c);
					}else
					{
						BJX2_MemSetWord_NoAT(ctx, addr_c, 0, blk_c);
					}
				}
				if(zmask && stpn)
				{
					zupd=1;
					if(isdword)
					{
						BJX2_MemSetWord_NoAT(ctx, addr_z, 0, (z2<<16)|upd_st);
					}else
					{
						if(issten)
						{
							BJX2_MemSetWord_NoAT(ctx,
								addr_z, 0, (z2&0xFFF0)|upd_st);
						}else
						{
							BJX2_MemSetWord_NoAT(ctx,
								addr_z, 0, z2);
						}
					}
				}
			}
		}

		if(issten && !zupd)
		{
			if(isdword)
			{
				BJX2_MemSetWord_NoAT(ctx,
					addr_z, 0, (blk_z<<16)|upd_st);
			}else
			{
				if(issten)
				{
					BJX2_MemSetWord_NoAT(ctx,
						addr_z, 0, (blk_z&0xFFF0)|upd_st);
				}else
				{
					BJX2_MemSetWord_NoAT(ctx,
						addr_z, 0, blk_z);
				}
			}
		}

		z+=zs;
		s+=ss;	t+=ts;
		r+=rs;	g+=gs;
		b+=bs;	a+=as;
		x++;
	}

	if(ctx->status)
	{
		printf("BJX2_MemEdgeWalk_ProbeUpdateXspan: Bad Mem\n");
		ctx->status=0;
		return(0);
	}

	return(0);
}

int BJX2_MemEdgeWalk_ProbeUpdateYstep(BJX2_Context *ctx)
{
	BJX2_MemEdgeWalk_ProbeUpdateXspan(ctx);

	bjx2_edgewalk_var_scanlfx+=bjx2_edgewalk_var_steplfx;
	bjx2_edgewalk_var_scanrtx+=bjx2_edgewalk_var_steprtx;

	bjx2_edgewalk_var_scanlfz+=bjx2_edgewalk_var_steplfz;
	bjx2_edgewalk_var_scanrtz+=bjx2_edgewalk_var_steprtz;

	bjx2_edgewalk_var_scanlfs+=bjx2_edgewalk_var_steplfs;
	bjx2_edgewalk_var_scanrts+=bjx2_edgewalk_var_steprts;

	bjx2_edgewalk_var_scanlft+=bjx2_edgewalk_var_steplft;
	bjx2_edgewalk_var_scanrtt+=bjx2_edgewalk_var_steprtt;

	bjx2_edgewalk_var_scanlfr+=bjx2_edgewalk_var_steplfr;
	bjx2_edgewalk_var_scanrtr+=bjx2_edgewalk_var_steprtr;

	bjx2_edgewalk_var_scanlfg+=bjx2_edgewalk_var_steplfg;
	bjx2_edgewalk_var_scanrtg+=bjx2_edgewalk_var_steprtg;

	bjx2_edgewalk_var_scanlfb+=bjx2_edgewalk_var_steplfb;
	bjx2_edgewalk_var_scanrtb+=bjx2_edgewalk_var_steprtb;

	bjx2_edgewalk_var_scanlfa+=bjx2_edgewalk_var_steplfa;
	bjx2_edgewalk_var_scanrta+=bjx2_edgewalk_var_steprta;

	bjx2_edgewalk_var_scany++;

	return(0);
}

u32 BJX2_ValExtractZx(u64 ref, int ofs, int width)
{
	return((ref>>ofs)&((1U<<width)-1));
}

s32 BJX2_ValExtractSx(u64 ref, int ofs, int width)
{
	s32 tv;
	tv=(s32)(ref>>ofs);
	tv=(s32)(tv<<(32-width));
	tv=tv>>(32-width);
	return(tv);
}

s64 jx2_edgewalk_lastcyc;
int jx2_edgewalk_busyqueue;

int BJX2_MemEdgeWalk_ProbeUpdate(BJX2_Context *ctx)
{
	int i, j, k, stadj;
	s64 dcyc;
	if(ctx->status)
		return(0);

	if(!jx2_edgewalk_rcptab[0])
	{
		jx2_edgewalk_rcptab[0]=0xFFFF;
		jx2_edgewalk_rcptab[1]=0xFFFF;
		
		for(i=2; i<64; i++)
		{
			jx2_edgewalk_rcptab[i]=0x10000/i;
		}

		for(i=64; i<1024; i++)
		{
			j=i; k=0;
			while(j>=64)
				{ j=j>>1; k++; }
			jx2_edgewalk_rcptab[i]=jx2_edgewalk_rcptab[32+(j&31)]>>k;
		}
	}

	dcyc=ctx->tot_cyc-jx2_edgewalk_lastcyc;
	jx2_edgewalk_lastcyc=ctx->tot_cyc;
	
	if(dcyc<0)
		dcyc=0;
	jx2_edgewalk_busycyc-=dcyc;
	if(jx2_edgewalk_busycyc<0)
		jx2_edgewalk_busycyc=0;

	if(bjx2_edgewalk_regs[0]&2)
	{
		bjx2_edgewalk_regs[0]&=BJX2_EDGEWALK_FEATUREMASK;
		return(0);
	}

//	if((bjx2_edgewalk_regs[0]&1) && !(bjx2_edgewalk_regs0l&1))
	if((bjx2_edgewalk_regs[0]&1) && !(bjx2_edgewalk_regs0l&1) &&
		(jx2_edgewalk_busyqueue<31))
	{
		if(bjx2_edgewalk_regs[0]&(1<<3))
		{
			bjx2_edgewalk_regs0l=bjx2_edgewalk_regs[0];
			return(0);
		}
	
		bjx2_edgewalk_var_scany		=(s16)((bjx2_edgewalk_regs[1]    )&0xFFFF);
		bjx2_edgewalk_var_scanymax	=(s16)((bjx2_edgewalk_regs[1]>>16)&0xFFFF);
//		bjx2_edgewalk_var_scanystr	=(bjx2_edgewalk_regs[0]>>16)&0x03FF; 
		bjx2_edgewalk_var_scanystr	=(bjx2_edgewalk_regs[1]>>32)&0x03FF; 

		bjx2_edgewalk_addr_cbuf=(u32)(bjx2_edgewalk_regs[2]>> 0);
		bjx2_edgewalk_addr_zbuf=(u32)(bjx2_edgewalk_regs[2]>>32);
		bjx2_edgewalk_addr_ubuf=(u32)(bjx2_edgewalk_regs[3]>> 0);

#if 1
		bjx2_edgewalk_var_scanlfx=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[ 4],
				42-JX2_EDGEWALK_XSUB, 16+JX2_EDGEWALK_XSUB+JX2_EDGEWALK_XHI);
		bjx2_edgewalk_var_scanrtx=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[ 5],
				42-JX2_EDGEWALK_XSUB, 16+JX2_EDGEWALK_XSUB+JX2_EDGEWALK_XHI);

		bjx2_edgewalk_var_steplfx=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[ 6],
				42-JX2_EDGEWALK_XSUB, 16+JX2_EDGEWALK_XSUB+JX2_EDGEWALK_XHI);
		bjx2_edgewalk_var_steprtx=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[ 7],
				42-JX2_EDGEWALK_XSUB, 16+JX2_EDGEWALK_XSUB+JX2_EDGEWALK_XHI);
#endif

		if((s32)(bjx2_edgewalk_var_steplfx^(bjx2_edgewalk_regs[ 6]>>32))<0)
		{
			printf("EdgeWalk: X Step Range Issue\n");
//			bjx2_edgewalk_var_steplfx=0;
		}

		if((s32)(bjx2_edgewalk_var_steprtx^(bjx2_edgewalk_regs[ 7]>>32))<0)
		{
			printf("EdgeWalk: X Step Range Issue\n");
//			bjx2_edgewalk_var_steprtx=0;
		}
		
//		bjx2_edgewalk_var_steplfx<<=1;
//		bjx2_edgewalk_var_steprtx<<=1;

//		bjx2_edgewalk_var_steplfx=0;
//		bjx2_edgewalk_var_steprtx=0;

#if 1
		bjx2_edgewalk_var_scanlfz=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[ 4],
				16-JX2_EDGEWALK_XSUB, 16+JX2_EDGEWALK_ZSUB+JX2_EDGEWALK_ZHI);
		bjx2_edgewalk_var_scanrtz=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[ 5],
				16-JX2_EDGEWALK_XSUB, 16+JX2_EDGEWALK_ZSUB+JX2_EDGEWALK_ZHI);

		bjx2_edgewalk_var_steplfz=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[ 6],
				16-JX2_EDGEWALK_XSUB, 16+JX2_EDGEWALK_ZSUB+JX2_EDGEWALK_ZHI);
		bjx2_edgewalk_var_steprtz=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[ 7],
				16-JX2_EDGEWALK_XSUB, 16+JX2_EDGEWALK_ZSUB+JX2_EDGEWALK_ZHI);
		
#endif

//		bjx2_edgewalk_var_steplfz=
//			((s32)(bjx2_edgewalk_var_steplfz<<(32-(16+JX2_EDGEWALK_ZSUB))))>>
//			(32-(16+JX2_EDGEWALK_ZSUB));
//		bjx2_edgewalk_var_steprtz=
//			((s32)(bjx2_edgewalk_var_steprtz<<(32-(16+JX2_EDGEWALK_ZSUB))))>>
//			(32-(16+JX2_EDGEWALK_ZSUB));

#if 1
		stadj=0;
		if(bjx2_edgewalk_regs[0]&(1<<20))
		{
			stadj=4;
		}

		bjx2_edgewalk_var_scanlfs=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[ 8],
				10-JX2_EDGEWALK_STSUB-stadj,
				16+JX2_EDGEWALK_STSUB+JX2_EDGEWALK_STHI);
		bjx2_edgewalk_var_scanrts=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[ 9],
				10-JX2_EDGEWALK_STSUB-stadj,
				16+JX2_EDGEWALK_STSUB+JX2_EDGEWALK_STHI);
		bjx2_edgewalk_var_steplfs=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[10],
				10-JX2_EDGEWALK_STSUB-stadj,
				16+JX2_EDGEWALK_STSUB+JX2_EDGEWALK_STHI);
		bjx2_edgewalk_var_steprts=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[11],
				10-JX2_EDGEWALK_STSUB-stadj,
				16+JX2_EDGEWALK_STSUB+JX2_EDGEWALK_STHI);

		bjx2_edgewalk_var_scanlft=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[ 8],
				42-JX2_EDGEWALK_STSUB-stadj,
				16+JX2_EDGEWALK_STSUB+JX2_EDGEWALK_STHI);
		bjx2_edgewalk_var_scanrtt=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[ 9],
				42-JX2_EDGEWALK_STSUB-stadj,
				16+JX2_EDGEWALK_STSUB+JX2_EDGEWALK_STHI);
		bjx2_edgewalk_var_steplft=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[10],
				42-JX2_EDGEWALK_STSUB-stadj,
				16+JX2_EDGEWALK_STSUB+JX2_EDGEWALK_STHI);
		bjx2_edgewalk_var_steprtt=
			BJX2_ValExtractSx(bjx2_edgewalk_regs[11],
				42-JX2_EDGEWALK_STSUB-stadj,
				16+JX2_EDGEWALK_STSUB+JX2_EDGEWALK_STHI);
#endif

		bjx2_edgewalk_var_scanlfa=(bjx2_edgewalk_regs[12]>>48)&0xFFFF;
		bjx2_edgewalk_var_scanrta=(bjx2_edgewalk_regs[13]>>48)&0xFFFF;
		bjx2_edgewalk_var_steplfa=(bjx2_edgewalk_regs[14]>>48)&0xFFFF;
		bjx2_edgewalk_var_steprta=(bjx2_edgewalk_regs[15]>>48)&0xFFFF;

		bjx2_edgewalk_var_scanlfr=(bjx2_edgewalk_regs[12]>>32)&0xFFFF;
		bjx2_edgewalk_var_scanrtr=(bjx2_edgewalk_regs[13]>>32)&0xFFFF;
		bjx2_edgewalk_var_steplfr=(bjx2_edgewalk_regs[14]>>32)&0xFFFF;
		bjx2_edgewalk_var_steprtr=(bjx2_edgewalk_regs[15]>>32)&0xFFFF;

		bjx2_edgewalk_var_scanlfg=(bjx2_edgewalk_regs[12]>>16)&0xFFFF;
		bjx2_edgewalk_var_scanrtg=(bjx2_edgewalk_regs[13]>>16)&0xFFFF;
		bjx2_edgewalk_var_steplfg=(bjx2_edgewalk_regs[14]>>16)&0xFFFF;
		bjx2_edgewalk_var_steprtg=(bjx2_edgewalk_regs[15]>>16)&0xFFFF;

		bjx2_edgewalk_var_scanlfb=(bjx2_edgewalk_regs[12]>> 0)&0xFFFF;
		bjx2_edgewalk_var_scanrtb=(bjx2_edgewalk_regs[13]>> 0)&0xFFFF;
		bjx2_edgewalk_var_steplfb=(bjx2_edgewalk_regs[14]>> 0)&0xFFFF;
		bjx2_edgewalk_var_steprtb=(bjx2_edgewalk_regs[15]>> 0)&0xFFFF;

		bjx2_edgewalk_var_clip_x0=(bjx2_edgewalk_regs[16]>> 0)&0xFFFF;
		bjx2_edgewalk_var_clip_y0=(bjx2_edgewalk_regs[16]>>16)&0xFFFF;
		bjx2_edgewalk_var_clip_x1=(bjx2_edgewalk_regs[16]>>32)&0xFFFF;
		bjx2_edgewalk_var_clip_y1=(bjx2_edgewalk_regs[16]>>48)&0xFFFF;
		
		if(jx2_edgewalk_busycyc>0)
			jx2_edgewalk_busyqueue++;
	}

#if 1
	while(bjx2_edgewalk_var_scany<=bjx2_edgewalk_var_scanymax)
		BJX2_MemEdgeWalk_ProbeUpdateYstep(ctx);
	bjx2_edgewalk_regs[0]&=~1;
#endif

#if 0
	if(bjx2_edgewalk_var_scany<=bjx2_edgewalk_var_scanymax)
	{
		bjx2_edgewalk_regs[0]|=1;
		BJX2_MemEdgeWalk_ProbeUpdateYstep(ctx);
	}else
	{
		bjx2_edgewalk_regs[0]&=~1;
	}
#endif

	bjx2_edgewalk_regs[0]&=~4;
	if(jx2_edgewalk_busycyc>0)
	{
		bjx2_edgewalk_regs[0]|=4;
	}else
	{
		jx2_edgewalk_busyqueue=0;
	}
	
	if(jx2_edgewalk_busyqueue>=28)
		bjx2_edgewalk_regs[0]|=1;

	bjx2_edgewalk_regs[0x1C]	=
	((u64)bjx2_edgewalk_var_pixtot) |
	(((u64)bjx2_edgewalk_var_pixzpass)<<32);


	bjx2_edgewalk_regs0l=bjx2_edgewalk_regs[0];
	return(0);
}

s64 BJX2_MemEdgeWalk_GetQWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	u32 lo, hi;
	int ra;

	if(((s64)addr)>0)
	{
		lo=-1;
	}
	
	if(!(ctx->do_edgewalk))
		return(0);

	BJX2_MemEdgeWalk_ProbeUpdate(ctx);

	ra=addr-sp->addr_base;
	ra=(ra&511);

	return(bjx2_edgewalk_regs[ra>>3]);
}

int BJX2_MemEdgeWalk_SetQWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s64 val)
{
	int ra;

	BJX2_MemEdgeWalk_ProbeUpdate(ctx);

	ra=addr-sp->addr_base;
	ra=(ra&511);
	bjx2_edgewalk_regs[ra>>3]=val;

	BJX2_MemEdgeWalk_ProbeUpdate(ctx);

	return(0);
}

int BJX2_MemDefineEdgeWalk(BJX2_Context *ctx,
	char *name, bjx2_addr base, bjx2_addr lim)
{
	BJX2_MemSpan *sp;

	sp=BJX2_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->addr_sz=lim-base;
//	sp->data=malloc((lim-base)+8);
	
	sp->GetByte=BJX2_MemRamCb_GetFault;
	sp->GetWord=BJX2_MemRamCb_GetFault;
	sp->GetDWord=BJX2_MemRamCb_GetFault;
	sp->GetQWord=BJX2_MemEdgeWalk_GetQWord;
	sp->GetXWord=BJX2_MemRamCb_GetFaultXW;
	
	sp->SetByte=BJX2_MemRamCb_SetFault;
	sp->SetWord=BJX2_MemRamCb_SetFault;
	sp->SetDWord=BJX2_MemRamCb_SetFault;
	sp->SetQWord=BJX2_MemEdgeWalk_SetQWord;
	sp->SetXWord=BJX2_MemRamCb_SetFaultXW;

	BJX2_MemAddSpan(ctx, sp);
	return(0);
}
