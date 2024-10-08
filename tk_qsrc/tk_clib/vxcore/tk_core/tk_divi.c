#if 0
static int _fcn_clz32(u32 v)
{
	const u32 m1=0x80000000U;
	const u32 m8=0xFF000000U;
	const u32 m16=0xFFFF0000U;
	u32 c;
	int n;
	c=v; n=0;
	if(!(c&m16))
		{ n+=16; c<<=16; }
	while(!(c&m8))
		{ n+=8; c<<=8; }
	while(!(c&m1))
		{ n++; c+=c; }
	return(n);
}
#endif

#if 0
u32 __udivsi3(u32 n, u32 d)
{
	u32 q, r;
	int s, c;
//	int sr;
	byte sr;

	if(!d || !n)
		return(0);
//	sr=_fcn_clz(d)-_fcn_clz(n);
	sr=(byte)(_fcn_clz32(d)-_fcn_clz32(n));

	if(sr>=31)
	{
		if(sr==31)
			return(n);
		return(0);
	}

	sr++;
	q=n<<(32-sr); r=n>>sr; c=0;
	while(sr--)
	{
		r=(r<<1)|(q>>31);
		q=(q<<1)|c;
		s=((int)(d-r-1))>>31;
		c=s&1;
		r-=d&s;
	}
	q=(q<<1)|c;
	q=(u32)q;
	return(q);
}
#endif

#if 0
s32 __sdivsi3(s32 a, s32 b)
{
	s32 sga, sgb;
	sga=a>>31;		sgb=b>>31;
	a=(a^sga)-sga;	b=(b^sgb)-sgb;
	sga^=sgb;
	return((__udivsi3(a, b)^sga)-sga);
}
#endif

#ifndef __BJX2__
static int _fcn_clz64(u64 v)
{
	const u64 m1=0x8000000000000000ULL;
	const u64 m8=0xFF00000000000000ULL;
	const u64 m16=0xFFFF000000000000ULL;
	const u64 m32=0xFFFFFFFF00000000ULL;
	u64 c;
	int n;
	c=v; n=0;
	if(!(c&m32))
		{ n+=32; c<<=32; }
	if(!(c&m16))
		{ n+=16; c<<=16; }
	while(!(c&m8))
		{ n+=8; c<<=8; }
	while(!(c&m1))
		{ n++; c+=c; }
	return(n);
}
#endif

#ifndef __BJX2__
u64 __udivdi3(u64 n, u64 d)
{
	u64 q, r;
	int s, c;
//	int sr;
	byte sr;

	if(!d || !n)
		return(0);
//	sr=_fcn_clz(d)-_fcn_clz(n);
	sr=(byte)(_fcn_clz64(d)-_fcn_clz64(n));

	if(sr>=63)
	{
		if(sr==63)
			return(n);
		return(0);
	}

	sr++;
	q=n<<(64-sr); r=n>>sr; c=0;
	while(sr--)
	{
		r=(r<<1)|(q>>63);
		q=(q<<1)|c;
		s=((s64)(d-r-1))>>63;
		c=s&1;
		r-=d&s;
	}
	q=(q<<1)|c;
	return(q);
}
#endif

#ifndef __BJX2__
s64 __sdivdi3(s64 a, s64 b)
{
	s64 sga, sgb;
	sga=a>>63;		sgb=b>>63;
	a=(a^sga)-sga;	b=(b^sgb)-sgb;
	sga^=sgb;
	return((__udivdi3(a, b)^sga)-sga);
}

u64 __udivti3(u64 a, u64 b)
{
	return(__udivdi3(a, b));
}
#endif


#ifdef __BJX2__

__asm {

.ifnarch has_qmul

#if 0
_tk_divtab:
.int 0x00000000, 0x00000000, 0x80000000, 0x55555556
.int 0x40000000, 0x33333334, 0x2AAAAAAB, 0x24924925
.int 0x20000000, 0x1C71C71D, 0x1999999A, 0x1745D175
.int 0x15555556, 0x13B13B14, 0x12492493, 0x11111112
.int 0x10000000, 0x0F0F0F10, 0x0E38E38F, 0x0D79435F
.int 0x0CCCCCCD, 0x0C30C30D, 0x0BA2E8BB, 0x0B21642D
.int 0x0AAAAAAB, 0x0A3D70A4, 0x09D89D8A, 0x097B425F
.int 0x0924924A, 0x08D3DCB1, 0x08888889, 0x08421085
.int 0x08000000, 0x07C1F07D, 0x07878788, 0x07507508
.int 0x071C71C8, 0x06EB3E46, 0x06BCA1B0, 0x06906907
.int 0x06666667, 0x063E7064, 0x06186187, 0x05F417D1
.int 0x05D1745E, 0x05B05B06, 0x0590B217, 0x0572620B
.int 0x05555556, 0x0539782A, 0x051EB852, 0x05050506
.int 0x04EC4EC5, 0x04D4873F, 0x04BDA130, 0x04A7904B
.int 0x04924925, 0x047DC120, 0x0469EE59, 0x0456C798
.int 0x04444445, 0x04325C54, 0x04210843, 0x04104105
.int 0x04000000, 0x03F03F04, 0x03E0F83F, 0x03D22636
.int 0x03C3C3C4, 0x03B5CC0F, 0x03A83A84, 0x039B0AD2
.int 0x038E38E4, 0x0381C0E1, 0x03759F23, 0x0369D037
.int 0x035E50D8, 0x03531DED, 0x03483484, 0x033D91D3
.int 0x03333334, 0x03291620, 0x031F3832, 0x03159722
.int 0x030C30C4, 0x03030304, 0x02FA0BE9, 0x02F14991
.int 0x02E8BA2F, 0x02E05C0C, 0x02D82D83, 0x02D02D03
.int 0x02C8590C, 0x02C0B02D, 0x02B93106, 0x02B1DA47
.int 0x02AAAAAB, 0x02A3A0FE, 0x029CBC15, 0x0295FAD5
.int 0x028F5C29, 0x0288DF0D, 0x02828283, 0x027C4598
.int 0x02762763, 0x02702703, 0x026A43A0, 0x02647C6A
.int 0x025ED098, 0x02593F6A, 0x0253C826, 0x024E6A18
.int 0x02492493, 0x0243F6F1, 0x023EE090, 0x0239E0D6
.int 0x0234F72D, 0x02302303, 0x022B63CC, 0x0226B903
.int 0x02222223, 0x021D9EAE, 0x02192E2A, 0x0214D022
.int 0x02108422, 0x020C49BB, 0x02082083, 0x02040811
.int 0x02000000, 0x01FC07F1, 0x01F81F82, 0x01F4465A
.int 0x01F07C20, 0x01ECC07C, 0x01E9131B, 0x01E573AD
.int 0x01E1E1E2, 0x01DE5D6F, 0x01DAE608, 0x01D77B66
.int 0x01D41D42, 0x01D0CB59, 0x01CD8569, 0x01CA4B31
.int 0x01C71C72, 0x01C3F8F1, 0x01C0E071, 0x01BDD2B9
.int 0x01BACF92, 0x01B7D6C4, 0x01B4E81C, 0x01B20365
.int 0x01AF286C, 0x01AC5702, 0x01A98EF7, 0x01A6D01B
.int 0x01A41A42, 0x01A16D40, 0x019EC8EA, 0x019C2D15
.int 0x0199999A, 0x01970E50, 0x01948B10, 0x01920FB5
.int 0x018F9C19, 0x018D3019, 0x018ACB91, 0x01886E60
.int 0x01861862, 0x0183C978, 0x01818182, 0x017F4060
.int 0x017D05F5, 0x017AD221, 0x0178A4C9, 0x01767DCF
.int 0x01745D18, 0x01724288, 0x01702E06, 0x016E1F77
.int 0x016C16C2, 0x016A13CE, 0x01681682, 0x01661EC7
.int 0x01642C86, 0x01623FA8, 0x01605817, 0x015E75BC
.int 0x015C9883, 0x015AC057, 0x0158ED24, 0x01571ED4
.int 0x01555556, 0x01539095, 0x0151D07F, 0x01501502
.int 0x014E5E0B, 0x014CAB89, 0x014AFD6B, 0x0149539F
.int 0x0147AE15, 0x01460CBD, 0x01446F87, 0x0142D663
.int 0x01414142, 0x013FB014, 0x013E22CC, 0x013C995B
.int 0x013B13B2, 0x013991C3, 0x01381382, 0x013698E0
.int 0x013521D0, 0x0133AE46, 0x01323E35, 0x0130D191
.int 0x012F684C, 0x012E025D, 0x012C9FB5, 0x012B404B
.int 0x0129E413, 0x01288B02, 0x0127350C, 0x0125E228
.int 0x0124924A, 0x01234568, 0x0121FB79, 0x0120B471
.int 0x011F7048, 0x011E2EF4, 0x011CF06B, 0x011BB4A5
.int 0x011A7B97, 0x01194539, 0x01181182, 0x0116E069
.int 0x0115B1E6, 0x011485F1, 0x01135C82, 0x0112358F
.int 0x01111112, 0x010FEF02, 0x010ECF57, 0x010DB20B
.int 0x010C9715, 0x010B7E6F, 0x010A6811, 0x010953F4
.int 0x01084211, 0x01073261, 0x010624DE, 0x01051980
.int 0x01041042, 0x0103091C, 0x01020409, 0x01010102
#endif

#if 1
_tk_divtab:
.int 0x00000000, 0x80000000, 0x80000000, 0xAAAAAAAB
.int 0x80000000, 0xCCCCCCCD, 0xAAAAAAAB, 0x92492493
.int 0x80000000, 0xE38E38E4, 0xCCCCCCCD, 0xBA2E8BA3
.int 0xAAAAAAAB, 0x9D89D89E, 0x92492493, 0x88888889
.int 0x80000000, 0xF0F0F0F1, 0xE38E38E4, 0xD79435E6
.int 0xCCCCCCCD, 0xC30C30C4, 0xBA2E8BA3, 0xB21642C9
.int 0xAAAAAAAB, 0xA3D70A3E, 0x9D89D89E, 0x97B425EE
.int 0x92492493, 0x8D3DCB09, 0x88888889, 0x84210843
.int 0x80000000, 0xF83E0F84, 0xF0F0F0F1, 0xEA0EA0EB
.int 0xE38E38E4, 0xDD67C8A7, 0xD79435E6, 0xD20D20D3
.int 0xCCCCCCCD, 0xC7CE0C7D, 0xC30C30C4, 0xBE82FA0C
.int 0xBA2E8BA3, 0xB60B60B7, 0xB21642C9, 0xAE4C415D
.int 0xAAAAAAAB, 0xA72F053A, 0xA3D70A3E, 0xA0A0A0A1
.int 0x9D89D89E, 0x9A90E7DA, 0x97B425EE, 0x94F20950
.int 0x92492493, 0x8FB823EF, 0x8D3DCB09, 0x8AD8F2FC
.int 0x88888889, 0x864B8A7E, 0x84210843, 0x82082083
.int 0x80000000, 0xFC0FC0FD, 0xF83E0F84, 0xF4898D60
.int 0xF0F0F0F1, 0xED7303B6, 0xEA0EA0EB, 0xE6C2B449
.int 0xE38E38E4, 0xE070381D, 0xDD67C8A7, 0xDA740DA8
.int 0xD79435E6, 0xD4C77B04, 0xD20D20D3, 0xCF6474A9
.int 0xCCCCCCCD, 0xCA4587E7, 0xC7CE0C7D, 0xC565C87C
.int 0xC30C30C4, 0xC0C0C0C1, 0xBE82FA0C, 0xBC52640C
.int 0xBA2E8BA3, 0xB81702E1, 0xB60B60B7, 0xB40B40B5
.int 0xB21642C9, 0xB02C0B03, 0xAE4C415D, 0xAC769185
.int 0xAAAAAAAB, 0xA8E83F58, 0xA72F053A, 0xA57EB503
.int 0xA3D70A3E, 0xA237C32C, 0xA0A0A0A1, 0x9F1165E8
.int 0x9D89D89E, 0x9C09C09D, 0x9A90E7DA, 0x991F1A52
.int 0x97B425EE, 0x964FDA6D, 0x94F20950, 0x939A85C5
.int 0x92492493, 0x90FDBC0A, 0x8FB823EF, 0x8E78356E
.int 0x8D3DCB09, 0x8C08C08D, 0x8AD8F2FC, 0x89AE408A
.int 0x88888889, 0x8767AB60, 0x864B8A7E, 0x85340854
.int 0x84210843, 0x83126E98, 0x82082083, 0x81020409
.int 0x80000000, 0xFE03F810, 0xFC0FC0FD, 0xFA232CF3
.int 0xF83E0F84, 0xF6603D99, 0xF4898D60, 0xF2B9D649
.int 0xF0F0F0F1, 0xEF2EB720, 0xED7303B6, 0xEBBDB2A6
.int 0xEA0EA0EB, 0xE865AC7C, 0xE6C2B449, 0xE525982B
.int 0xE38E38E4, 0xE1FC780F, 0xE070381D, 0xDEE95C4D
.int 0xDD67C8A7, 0xDBEB61EF, 0xDA740DA8, 0xD901B204
.int 0xD79435E6, 0xD62B80D7, 0xD4C77B04, 0xD3680D37
.int 0xD20D20D3, 0xD0B69FCC, 0xCF6474A9, 0xCE168A78
.int 0xCCCCCCCD, 0xCB8727C1, 0xCA4587E7, 0xC907DA4F
.int 0xC7CE0C7D, 0xC6980C6A, 0xC565C87C, 0xC4372F86
.int 0xC30C30C4, 0xC1E4BBD6, 0xC0C0C0C1, 0xBFA02FE9
.int 0xBE82FA0C, 0xBD691048, 0xBC52640C, 0xBB3EE722
.int 0xBA2E8BA3, 0xB92143FB, 0xB81702E1, 0xB70FBB5B
.int 0xB60B60B7, 0xB509E68B, 0xB40B40B5, 0xB30F6353
.int 0xB21642C9, 0xB11FD3B9, 0xB02C0B03, 0xAF3ADDC7
.int 0xAE4C415D, 0xAD602B59, 0xAC769185, 0xAB8F69E3
.int 0xAAAAAAAB, 0xA9C84A48, 0xA8E83F58, 0xA80A80A9
.int 0xA72F053A, 0xA655C43A, 0xA57EB503, 0xA4A9CF1E
.int 0xA3D70A3E, 0xA3065E40, 0xA237C32C, 0xA16B312F
.int 0xA0A0A0A1, 0x9FD809FE, 0x9F1165E8, 0x9E4CAD24
.int 0x9D89D89E, 0x9CC8E161, 0x9C09C09D, 0x9B4C6F9F
.int 0x9A90E7DA, 0x99D722DB, 0x991F1A52, 0x9868C80A
.int 0x97B425EE, 0x97012E03, 0x964FDA6D, 0x95A02569
.int 0x94F20950, 0x94458095, 0x939A85C5, 0x92F11385
.int 0x92492493, 0x91A2B3C5, 0x90FDBC0A, 0x905A3864
.int 0x8FB823EF, 0x8F1779DA, 0x8E78356E, 0x8DDA5203
.int 0x8D3DCB09, 0x8CA29C05, 0x8C08C08D, 0x8B70344B
.int 0x8AD8F2FC, 0x8A42F871, 0x89AE408A, 0x891AC73B
.int 0x88888889, 0x87F78088, 0x8767AB60, 0x86D90545
.int 0x864B8A7E, 0x85BF3762, 0x85340854, 0x84A9F9C9
.int 0x84210843, 0x83993053, 0x83126E98, 0x828CBFBF
.int 0x82082083, 0x81848DA9, 0x81020409, 0x80808081
_tk_divtab_sh:
.byte 0x00, 0xE1, 0xE0, 0xDF, 0xDF, 0xDE, 0xDE, 0xDE
.byte 0xDE, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD
.byte 0xDD, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC
.byte 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC
.byte 0xDC, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB
.byte 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB
.byte 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB
.byte 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB
.byte 0xDB, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA
.byte 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA
.byte 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA
.byte 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA
.byte 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA
.byte 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA
.byte 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA
.byte 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA
.byte 0xDA, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
.byte 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9
#endif

__udivsi3_tab:
//	CMPHS		2, R5
//	BF			.L1

//	BRA			__udivsi3_i

	CMPHI		255, R5
	BF			.L0

#if 0
	LEA.B		_tk_divtab, R3
	CLZ			R5, R1
	SUB			24, R1
	SHLD		R5, R1, R2

	MOVU.L		(R3, R2), R16
	SHLD		R16, R1, R17
	DMULU.L		R4, R17, R18
	SHLD.Q		R18, -32, R2

	/* Check that modulo is in range. */
	MOV			3, R3
	.L1A:
	DMULU.L		R2, R5, R6
	SUB			R4, R6, R7
	CMPQHI		R7, R5
//	BT			.L2

	BF			.L1B
	RTS

//	BREAK

#if 1
	.L1B:

//	BREAK

	/* Adjust up or down */
	CMPQGT		0, R7
	ADD?T		 1, R2
	ADD?F		-1, R2
	ADD			-1, R3
	CMPGT		0, R3
	BT			.L1A

//	BRA			.L1A
#endif

#if 1
	/* Fall back to 'binary long division' divider. */
	BRA			__udivsi3_i

	.L2:
	RTS
#endif

#endif

	.L3:
	BRA			__udivsi3_i
	
	.L0:
#if 0
	MOV			_tk_divtab, R3
	CMPGT		1, R5
	MOV			R4, R2	|
	MOVU.L?T	(R3, R5), R6
	DMULU.L?T	R4, R6, R7
	SHLD.Q?T	R7, -32, R2
#endif

#if 1
	CMPGE		0, R4
	BF			.L3

	LEA.B		_tk_divtab, R18
	LEA.B		_tk_divtab_sh, R19
	MOVU.L		(R18, R5), R6
	MOV.B		(R19, R5), R7
	DMULU.L		R4, R6, R3
	SHLD.Q		R3, R7, R2
#endif

//	SHLD.Q		R7, -32, R2
//	CMPGT		1, R5
//	MOV?F		R4, R2

	RTS
	
//	.L1:
//	MOV		R4, R2
//	RTS

.endif

};

#endif
