#if 1
/*
R4=src
R5=lsrc
R6=rdsta
R7=rdstb
*/

// void	TK_EncBlock16P(u16 *src, u16 *lsrc, u32 *rdsta, u32 *rdstb)

TK_EncBlock16P_Ret0:
	rts

TK_EncBlock16P:
//	u16 *cs, *ct;
//	int cmin, cmax;
//	register int cavg, calo, cahi;
//	int clra, clrb;
//	u32 px2;
//	u32 dxa, dxb;
//	u64 lpx0, lpx1, lpx2, lpx3, lpx4;
//	register int k0, k1;
//	int i, j, k;

#if 1

#if 0

//	ct=lsrc;
//	cs=src;
//	i=0;

	mov r4, r16		//cs
	mov r5, r17		//ct
	xor r2, r2		//i
	
//	lpx0=*(u64 *)cs;
//	lpx4=*(u64 *)ct;

	mov.q @r16, r18
	mov.q @r17, r19

//	cs+=BASEWIDTH;
//	ct+=BASEWIDTH;
	add #640, r16
	add #640, r17

//	i|=(lpx0!=lpx4);
	cmpqeq r18, r19
	movnt r3
	or r3, r2

//	lpx1=*(u64 *)cs;
//	lpx4=*(u64 *)ct;
	mov.q @r16, r19
	mov.q @r17, r20

//	cs+=BASEWIDTH;
//	ct+=BASEWIDTH;
	add #640, r16
	add #640, r17

//	i|=(lpx1!=lpx4);
	cmpqeq r19, r20
	movnt r3
	or r3, r2

//	lpx2=*(u64 *)cs;
//	lpx4=*(u64 *)ct;
	mov.q @r16, r20
	mov.q @r17, r21

//	cs+=BASEWIDTH;
//	ct+=BASEWIDTH;
	add #640, r16
	add #640, r17

//	i|=(lpx2!=lpx4);
	cmpqeq r20, r21
	movnt r3
	or r3, r2

//	lpx3=*(u64 *)cs;
//	lpx4=*(u64 *)ct;
	mov.q @r16, r21
	mov.q @r17, r22

//	i|=(lpx3!=lpx4);
	cmpqeq r21, r22
	movnt r3
	or r3, r2

//	if(!i)	return;
	tst r2, r2
	bt TK_EncBlock16P_Ret0
#endif

#if 1

	mov r4, r16		//cs
	mov r5, r17		//ct
	
	mov.q @r16, r18
	add #640, r16
	mov.q @r16, r19
	add #640, r16
	mov.q @r16, r20
	add #640, r16
	mov.q @r16, r21

	mov.q @r17, r22
	add #640, r17

	xor r2, r2		//i
	cmpqeq r18, r22
	movnt r2

	bf TK_EncBlock16P_Go0

	mov.q @r17, r23
	add #640, r17

	cmpqeq r19, r23
	movnt r3
	or r3, r2

//	tst r2, r2
//	bf TK_EncBlock16P_Go0

	mov.q @r17, r22
	add #640, r17
	mov.q @r17, r23

	cmpqeq r20, r22
	movnt r3
	or r3, r2

	cmpqeq r21, r23
	movnt r3
	or r3, r2

	tst r2, r2
	bt TK_EncBlock16P_Ret0
#endif

	TK_EncBlock16P_Go0:

//	ct=lsrc; cs=src;
//	*(u64 *)ct=lpx0;
//	ct+=BASEWIDTH;
//	*(u64 *)ct=lpx1;
//	ct+=BASEWIDTH;
//	*(u64 *)ct=lpx2;
//	ct+=BASEWIDTH;
//	*(u64 *)ct=lpx3;

//	mov r5, r17		//ct

	mov.q r18, @(r5,    0)
	mov.q r19, @(r5,  640)
	mov.q r20, @(r5, 1280)
	mov.q r21, @(r5, 1920)


//	calo=65536;	cahi=-1;

	mov #65535, r16
	mov #-1, r17

//	k1=(u16)(lpx0>>16);
//	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	
//	mov r18, r2
//	shlr16 r2
	shld r18, #-16, r2
	extu.w r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17

//	k1=(u16)(lpx0>>48);
//	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);

	shld.q r18, #-48, r2
//	mov r18, r2
//	shlr16 r2
//	shlr32 r2
//	extu.w r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17


//	mov r19, r2
//	extu.w r2
	extu.w r19, r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17

	shld.q r19, #-32, r2
//	mov r19, r2
//	shlr32 r2
	extu.w r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17


//	mov r20, r2
//	shlr16 r2
//	extu.w r2
	shld r20, #-16, r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17

	shld.q r20, #-48, r2
//	mov r20, r2
//	shlr16 r2
//	shlr32 r2
//	extu.w r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17


//	mov r21, r2
//	extu.w r2
	extu.w r21, r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17

	shld.q r21, #-32, r2
//	mov r21, r2
//	shlr32 r2
	extu.w r2
	cmpgt r16, r2
	cselt r16, r2, r16
	cmpgt r17, r2
	cselt r2, r17, r17


#if 0
	k1=(u16)(lpx0>>16);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx0>>48);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
#endif

#if 0
	k0=(u16)(lpx1    );	
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx1>>32);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);

	k1=(u16)(lpx2>>16);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
	k1=(u16)(lpx2>>48);
	calo=__int_min(calo, k1);		cahi=__int_max(cahi, k1);
#endif

#if 0
	k0=(u16)(lpx3    );
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
	k0=(u16)(lpx3>>32);
	calo=__int_min(calo, k0);		cahi=__int_max(cahi, k0);
#endif

#endif

//	cmin=calo;
//	cmax=cahi;
	
//	cavg=(cmin+cmax)>>1;
//	calo=(cmin+cavg)>>1;
//	cahi=(cmax+cavg)>>1;

	/*
	  R0=DLR
	  R1=DHR
	  R2=scratch
	  R3=scratch
	  R4=calo
	  R5=cahi
	  R16=cmin
	  R17=cmax
	  R18=lpx0
	  R19=lpx1
	  R20=lpx2
	  R21=lpx3
	  R22=cavg
	  R23=px2
	*/

	add r16, r17, r22
	shad r22, #-1, r22

	add r16, r22, r4
	add r17, r22, r5
	shlr1 r4
	shlr1 r5


	xor r1, r1

	/* lpx0 */
	
//	k0=(u16)(lpx0    );
//	px2=(px2<<2)|((k0>cavg)<<1);
//	px2|=(k0>cahi)|(calo>k0);

//	mov r18, r2
//	extu.w r2
	extu.w r18, r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	mov r3, r23
	addc r23, r23
//	add r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23

//	k1=(u16)(lpx0>>16);
//	px2=(px2<<2)|((k1>cavg)<<1);
//	px2|=(k1>cahi)|(calo>k1);

//	shld r23, #2, r23

//	mov r18, r2
//	shlr16 r2
//	extu.w r2
	shld r18, #-16, r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
///	or r3, r23
	addc r23, r23
//	add r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23

//	k0=(u16)(lpx0>>32);
//	px2=(px2<<2)|((k0>cavg)<<1);
//	px2|=(k0>cahi)|(calo>k0);

//	shll2 r23
//	shld r23, #2, r23

	shld.q r18, #-32, r2
//	mov r18, r2
//	shlr32 r2
	extu.w r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	or r3, r23
	addc r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23

//	k1=(u16)(lpx0>>48);
//	px2=(px2<<2)|((k1>cavg)<<1);
//	px2|=(k1>cahi)|(calo>k1);

//	shll2 r23
//	shld r23, #2, r23

	shld.q r18, #-48, r2
//	mov r18, r2
//	shlr32 r2
//	shlr16 r2
//	extu.w r2
//	shld r2, #-16, r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	or r3, r23
	addc r23, r23
//	add r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23


	/* lpx1 */

//	shld r23, #2, r23

//	mov r19, r2
//	extu.w r2
	extu.w r19, r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	or r3, r23
	addc r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23

//	shll2 r23
//	shld r23, #2, r23

//	mov r19, r2
//	shlr16 r2
	shld r19, #-16, r2
	extu.w r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	or r3, r23
	addc r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23

//	shll2 r23
//	shld r23, #2, r23

	shld.q r19, #-32, r2
//	mov r19, r2
//	shlr32 r2
	extu.w r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	or r3, r23
	addc r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23

//	shll2 r23
//	shld r23, #2, r23

	shld.q r19, #-48, r2
//	mov r19, r2
//	shlr32 r2
//	shlr16 r2
//	extu.w r2
//	shld r2, #-16, r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	or r3, r23
	addc r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23


	/* lpx2 */

//	shld r23, #2, r23

//	mov r20, r2
//	extu.w r2
	extu.w r20, r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	or r3, r23
	addc r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23

//	shll2 r23
//	shld r23, #2, r23

//	mov r20, r2
//	shlr16 r2
	shld r20, #-16, r2
	extu.w r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	or r3, r23
	addc r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23

//	shll2 r23
//	shld r23, #2, r23

	shld.q r20, #-32, r2
//	mov r20, r2
//	shlr32 r2
	extu.w r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	or r3, r23
	addc r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23

//	shll2 r23
//	shld r23, #2, r23

	shld.q r20, #-48, r2
//	mov r20, r2
//	shlr32 r2
//	shlr16 r2
//	extu.w r2
//	shld r2, #-16, r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	or r3, r23
	addc r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23


	/* lpx3 */

//	shld r23, #2, r23

//	mov r21, r2
//	extu.w r2
	extu.w r21, r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	or r3, r23
	addc r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23

//	shll2 r23
//	shld r23, #2, r23

//	mov r21, r2
//	shlr16 r2
//	extu.w r2
	shld r21, #-16, r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	or r3, r23
	addc r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23

//	shll2 r23
//	shld r23, #2, r23

	shld.q r21, #-32, r2
//	mov r21, r2
//	shlr32 r2
	extu.w r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	or r3, r23
	addc r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23

//	shll2 r23
//	shld r23, #2, r23

	shld.q r21, #-48, r2
//	mov r21, r2
//	shlr32 r2
//	shlr16 r2
//	extu.w r2
//	shld r2, #-16, r2
	cmpgt r22, r2
//	movt r3
//	shll1 r3
//	or r3, r23
	addc r23, r23
	cmpgt r5, r2
//	movt r3
//	or r3, r23
//	addc r1, r23
	addc r23, r23
	cmpgt r2, r4
//	movt r3
//	or r3, r23
	addc r1, r23



#if 0
	px2=0;

	k0=(u16)(lpx0    );				k1=(u16)(lpx0>>16);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx0>>32);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx0>>48);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);

	k0=(u16)(lpx1    );				k1=(u16)(lpx1>>16);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx1>>32);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx1>>48);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);

	k0=(u16)(lpx2    );				k1=(u16)(lpx2>>16);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx2>>32);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx2>>48);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);

	k0=(u16)(lpx3    );				k1=(u16)(lpx3>>16);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	k0=(u16)(lpx3>>32);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);
	k1=(u16)(lpx3>>48);
	px2=(px2<<2)|((k0>cavg)<<1);	px2|=(k0>cahi)|(calo>k0);
	px2=(px2<<2)|((k1>cavg)<<1);	px2|=(k1>cahi)|(calo>k1);

	px2=px2^((~(px2>>1))&0x55555555);
#endif

	mov r23, r2
	shlr1 r2
	not r2
	and #0x55555555, r2
	xor r2, r23

//	clrb=cmin>>1;
//	clra=cmax>>1;
//	dxa=0xC0000000|(clrb<<15)|clra;
//	dxb=px2;
//	*rdsta=dxa;
//	*rdstb=dxb;

	shld r16, #-1, r16
	shld r17, #-1, r17
	add r16, #0x18000, r2
	shll16 r2
	shlr1 r2
	or r17, r2
	
	mov.l r2, @r6
	mov.l r23, @r7
// }
	rts

#endif
