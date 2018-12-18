#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef unsigned char byte;
typedef unsigned int u32;
typedef unsigned long long u64;

// #define SHUF_MAXOP	6
// #define SHUF_MAXOP	8
#define SHUF_MAXOP	10

int ApplyShufW(int p0, int op)
{
	int p1;

	switch(op)
	{
	case 0:
		p1=p0;	/* NOP, 0,1,2,3 */
		break;
	case 1:		/* ShufLW, 1,0,2,3 */
		p1=((p0&0x03)<<2)|((p0&0x0C)>>2)|(p0&0xF0);
		break;

#if 1
	case 2:		/* ShufW, 1,0,3,2 */
		p1=((p0&0x03)<<2)|((p0&0x0C)>>2)|
			((p0&0x30)<<2)|((p0&0xC0)>>2);
		break;
#endif

	case 3:		/* ShufL, 2,3,0,1 */
		p1=((p0&0x0F)<<4)|((p0&0xF0)>>4);
		break;
	case 4:		/* CpyLW 0,0,2,3 */
		p1=((p0&0x03)<<2)|(p0&0x03)|(p0&0xF0);
		break;

	case 5:		/* ShufMW, 2,1,0,3 */
		p1=((p0&0x03)<<4)|((p0&0x30)>>4)|(p0&0xCC);
		break;
	case 6:		/* ShufHW, 3,1,2,0 */
		p1=((p0&0x03)<<6)|((p0&0xC0)>>6)|(p0&0x3C);
		break;

	case 7:		/* CpyMW 0,0,2,3 */
		p1=((p0&0x03)<<4)|(p0&0x03)|(p0&0xCC);
		break;
	case 8:		/* CpyHW 0,0,2,3 */
		p1=((p0&0x03)<<6)|(p0&0x03)|(p0&0x3C);
		break;

#if 0
	case 9:		/* ShufMHW 0,2,1,3 */
		p1=((p0&0x0C)<<2)|((p0&0x30)>>2)|(p0&0xC3);
		break;
	case 10:		/* ShufHHW 0,1,3,2 */
		p1=((p0&0x30)<<2)|((p0&0xC0)>>2)|(p0&0x0F);
		break;
#endif

	default:
		p1=p0;
		break;

	}
	return(p1);
}


// #define SHUF_MAXOPB	11
// #define SHUF_MAXOPB	13
#define SHUF_MAXOPB	15

int ApplyShuf8B(int p0, int op)
{
	int p1;

	switch(op)
	{
	case 0:
		p1=p0;	/* NOP, 0,1,2,3,4,5,6,7 */
		break;
	case 1:		/* ShufLB, 1,0,2,3,4,5,6,7 */
//		p1=((p0&0x000007)<<3)|((p0&0x000038)>>3)|(p0&0xFFFFC0);
		p1=((p0&000000007)<<3)|((p0&000000070)>>3)|(p0&077777700);
		break;
	case 2:		/* ShufB, 1,0,3,2,4,5,6,7 */
//		p1=	((p0&0x000007)<<3)|((p0&0x000038)>>3)|
//			((p0&0x0001C0)<<3)|((p0&0x000E00)>>3)|
//			((p0&0x007000)<<3)|((p0&0x038000)>>3)|
//			((p0&0x1C0000)<<3)|((p0&0xE00000)>>3);
//		p1=	((p0&0x000007)<<3)|((p0&0x000038)>>3)|
//			((p0&0x0001C0)<<3)|((p0&0x000E00)>>3)|
//			(p0&0xFFF000);
		p1=	((p0&000000007)<<3)|((p0&000000070)>>3)|
			((p0&000000700)<<3)|((p0&000007000)>>3)|
			 (p0&077770000);

		break;
	case 3:		/* ShufLW, 2,3,0,1,4,5,6,7 */
//		p1=((p0&0x00003F)<<6)|((p0&0x000FC0)>>6)|(p0&0xFFF000);
		p1=((p0&000000077)<<6)|((p0&00007700)>>6)|(p0&077770000);
		break;
	case 4:		/* CpyLB 0,0,2,3,4,5,6,7 */
//		p1=((p0&0x000007)<<3)|(p0&0x000007)|(p0&0xFFFFC0);
		p1=((p0&000000007)<<3)|(p0&000000007)|(p0&077777700);
		break;
	case 5:		/* ShufMB, 2,1,0,3,4,5,6,7 */
//		p1=((p0&0x000007)<<6)|((p0&0x0001C0)>>6)|(p0&0xFFFE00);
		p1=((p0&000000007)<<6)|((p0&000000700)>>6)|(p0&077777070);
		break;
	case 6:		/* ShufHB, 3,1,2,0,4,5,6,7 */
//		p1=((p0&0x000007)<<9)|((p0&0x000E00)>>9)|(p0&0xFFF1F8);
		p1=((p0&000000007)<<9)|((p0&000007000)>>9)|(p0&077770770);
		break;

	case 7:		/* ShufW, 1,0,3,2 */
//		p1=	((p0&0x00003F)<<6)|((p0&0x000FC0)>>6)|
//			((p0&0x03F000)<<6)|((p0&0xFC0000)>>6);
		p1=	((p0&000000077)<<6)|((p0&000007700)>>6)|
			((p0&000770000)<<6)|((p0&077000000)>>6);
		break;
	case 8:		/* ShufL, 2,3,0,1 */
//		p1=((p0&0x000FFF)<<12)|((p0&0xFFF000)>>12);
		p1=((p0&000007777)<<12)|((p0&077770000)>>12);
		break;
	case 9:		/* CpyLW 0,0,2,3 */
//		p1=((p0&0x00003F)<<6)|(p0&0x00003F)|(p0&0xFFF000);
		p1=((p0&000000077)<<6)|(p0&000000077)|(p0&077770000);
		break;

	case 10:	// ShufMW, 2,1,0,3
//		p1=((p0&0x00003F)<<12)|((p0&0x03F000)>>12)|(p0&0xFC0FC0);
		p1=((p0&000000077)<<12)|((p0&000770000)>>12)|(p0&077007700);
		break;
	case 11:	// ShufHW, 3,1,2,0
//		p1=((p0&0x00003F)<<18)|((p0&0xFC0000)>>18)|(p0&0x03FFC0);
		p1=((p0&000000077)<<18)|((p0&077000000)>>18)|(p0&000777700);
		break;

	case 12:		/* CpyMB 0,1,0,3,4,5,6,7 */
//		p1=((p0&0x000007)<<6)|(p0&0x000007)|(p0&0xFFFE38);
		p1=((p0&000000007)<<6)|(p0&000000007)|(p0&077777070);
		break;
	case 13:		/* CpyHB 0,1,2,0,4,5,6,7 */
//		p1=((p0&0x000007)<<9)|(p0&0x000007)|(p0&0xFFF1F8);
		p1=((p0&000000007)<<9)|(p0&000000007)|(p0&077770770);
		break;

	case 14:		/* CpyMW 0,1,0,3,4,5,6,7 */
//		p1=((p0&0x000007)<<6)|(p0&0x000007)|(p0&0xFFFE38);
		p1=((p0&0000000077)<<12)|(p0&0000000077)|(p0&077007700);
		break;
	case 15:		/* CpyHW 0,1,2,0,4,5,6,7 */
//		p1=((p0&0x000007)<<9)|(p0&0x000007)|(p0&0xFFF1F8);
//		p1=((p0&000000007)<<9)|(p0&000000007)|(p0&077770770);
		p1=((p0&0000000077)<<18)|(p0&0000000077)|(p0&000777700);
		break;

	default:
		p1=p0;
		break;

	}
	return(p1);
}

int stat_shuf[16];
int stat_len;
int stat_cnt;

int FindShuf(int tgt, int *ropa)
{
	int op1, op2, op3, op4;
	int op5, op6, op7, op8;
	int p0;
	
	for(op8=0; op8<=SHUF_MAXOP; op8++)
		for(op7=0; op7<=SHUF_MAXOP; op7++)
			for(op6=0; op6<=SHUF_MAXOP; op6++)
				for(op5=0; op5<=SHUF_MAXOP; op5++)
	{
		for(op4=0; op4<=SHUF_MAXOP; op4++)
			for(op3=0; op3<=SHUF_MAXOP; op3++)
				for(op2=0; op2<=SHUF_MAXOP; op2++)
					for(op1=0; op1<=SHUF_MAXOP; op1++)
		{
			p0=(0<<0)|(1<<2)|(2<<4)|(3<<6);
			p0=ApplyShufW(p0, op1);
			p0=ApplyShufW(p0, op2);
			p0=ApplyShufW(p0, op3);
			p0=ApplyShufW(p0, op4);

			p0=ApplyShufW(p0, op5);
			p0=ApplyShufW(p0, op6);
			p0=ApplyShufW(p0, op7);
			p0=ApplyShufW(p0, op8);
						
			if(p0==tgt)
			{
				if(op1)		stat_shuf[op1]++;
				if(op2)		stat_shuf[op2]++;
				if(op3)		stat_shuf[op3]++;
				if(op4)		stat_shuf[op4]++;
				if(op5)		stat_shuf[op5]++;
				if(op6)		stat_shuf[op6]++;
				if(op7)		stat_shuf[op7]++;
				if(op8)		stat_shuf[op8]++;

				if(op1 && !op2)		stat_len+=1;
				if(op2 && !op3)		stat_len+=2;
				if(op3 && !op4)		stat_len+=3;
				if(op4 && !op5)		stat_len+=4;
				if(op5 && !op6)		stat_len+=5;
				if(op6 && !op7)		stat_len+=6;
				if(op7 && !op8)		stat_len+=7;
				if(op8)				stat_len+=8;
				
				stat_cnt++;

				ropa[0]=op1;
				ropa[1]=op2;
				ropa[2]=op3;
				ropa[3]=op4;
				ropa[4]=op5;
				ropa[5]=op6;
				ropa[6]=op7;
				ropa[7]=op8;
				return(1);
			}
		}
	}

	return(0);
}


int FindShuf8B(int tgt, int *ropa)
{
	int op1, op2, op3, op4;
	int op5, op6, op7, op8;
	int p0;
	
	op8=0;
	op7=0;
	
//	for(op8=0; op8<=SHUF_MAXOPB; op8++)
//		for(op7=0; op7<=SHUF_MAXOPB; op7++)
			for(op6=0; op6<=SHUF_MAXOPB; op6++)
				for(op5=0; op5<=SHUF_MAXOPB; op5++)
	{
		for(op4=0; op4<=SHUF_MAXOPB; op4++)
			for(op3=0; op3<=SHUF_MAXOPB; op3++)
				for(op2=0; op2<=SHUF_MAXOPB; op2++)
					for(op1=0; op1<=SHUF_MAXOPB; op1++)
		{
			p0=(0<<0)|(1<<3)|(2<<6)|(3<<9)|(4<<12)|(5<<15)|(6<<18)|(7<<21);
			p0=ApplyShuf8B(p0, op1);
			p0=ApplyShuf8B(p0, op2);
			p0=ApplyShuf8B(p0, op3);
			p0=ApplyShuf8B(p0, op4);

			p0=ApplyShuf8B(p0, op5);
			p0=ApplyShuf8B(p0, op6);
			p0=ApplyShuf8B(p0, op7);
			p0=ApplyShuf8B(p0, op8);
			
			if(p0==tgt)
			{
				ropa[0]=op1;
				ropa[1]=op2;
				ropa[2]=op3;
				ropa[3]=op4;
				ropa[4]=op5;
				ropa[5]=op6;
				ropa[6]=op7;
				ropa[7]=op8;
				return(1);
			}
		}
	}

	return(0);
}

int tgt8b_perm[16777216];	//permutation index
u64 tgt8b_opra[16777216];	//permutation operations
byte tgt8b_opln[16777216];	//permutation index

u64 tgt8b_ovfa[16777216];	//permutation overflow.
u64 tgt8b_ovfb[16777216];	//permutation overflow.
int tgt8b_novf=1;

int GenShuf_DumpPermTab8B(char *fn)
{
	FILE *fd;
	u64 v;
	int i;

#if 0
	fd=fopen(fn, "wt");
	for(i=0; i<16777216; i++)
	{
		fprintf(fd, "%016llX\n", tgt8b_opra[i]);
	}
	fclose(fd);
#endif

#if 1
	fd=fopen(fn, "wb");
	for(i=0; i<16777216; i++)
	{
		v=tgt8b_opra[i];
		fwrite(&v, 1, 8, fd);
	}
	
	tgt8b_ovfa[0]=0xAB23CD4500000000ULL|tgt8b_novf;
	for(i=0; i<tgt8b_novf; i++)
	{
		v=tgt8b_ovfa[i];
		fwrite(&v, 1, 8, fd);
	}
	
	fclose(fd);
#endif

	return(0);
}

int Shuf8B_ShufLen(u64 opv)
{
	u64 opv1;
	int i, j, k;

	if((opv>>60)==15)
	{
		if((opv>>56)&8)
		{
			if((u32)opv)
			{
				for(i=0; i<8; i++)
				{
					if(!((opv>>(i*4))&15))
						break;
				}
				return(i+32);
			}
			
			k=(opv>>32)&0xFFFFFF;
			opv1=tgt8b_ovfa[k+1];

			for(i=0; i<16; i++)
			{
				if(!((opv1>>(i*4))&15))
					break;
			}
			return(i+16);
		}

		for(i=0; i<8; i++)
		{
			if(!((opv>>(i*4))&15))
				break;
		}
		return(i+16);
	}

	for(i=0; i<16; i++)
	{
		if(!((opv>>(i*4))&15))
			break;
	}
	return(i);
}

int CompactPermTab8B(void)
{
	u64 v;
	int i, j, k, l, n;

	n=tgt8b_novf;
	for(i=1; i<n; i++)
		{ tgt8b_ovfb[i]=tgt8b_ovfa[i]; }

	tgt8b_novf=1;
	for(i=0; i<16777216; i++)
	{
		v=tgt8b_opra[i];
		
		if(((v>>60)&15)!=15)
			continue;
		if(((v>>56)&0xFF)==0xFF)
			continue;

		j=(v>>32)&0xFFFFFF;

		if((v>>56)&8)
		{
			n=tgt8b_novf++;
			tgt8b_novf++;			
			tgt8b_ovfa[n+0]=tgt8b_ovfb[j+0];
			tgt8b_ovfa[n+1]=tgt8b_ovfb[j+1];
		}else
		{
			n=tgt8b_novf++;
			tgt8b_ovfa[n]=tgt8b_ovfb[j];
		}
		
		v=(v&0xFF000000FFFFFFFFULL)|(((u64)n)<<32);
		tgt8b_opra[i]=v;
	}
	
	return(0);
}

int LoadPermTab8B(char *fn)
{
	FILE *fd;
	u64 v;
	int i, j, k, l, n;
	
	fd=fopen(fn, "rb");
	if(!fd)
		return(-1);
	
	for(i=0; i<16777216; i++)
	{
		fread(&v, 1, 8, fd);
		
		if((v>>56)==0xFF)
		{
			tgt8b_perm[i]=-1;
			tgt8b_opra[i]=-1;
			tgt8b_opln[i]=0;
		}else
		{
			l=Shuf8B_ShufLen(v);
			tgt8b_perm[i]=i;
			tgt8b_opra[i]=v;
			tgt8b_opln[i]=l;
		}
	}

	fread(&v, 1, 8, fd);
	n=v;
	tgt8b_novf=n;
	tgt8b_ovfa[0]=v;
	
	for(i=1; i<n; i++)
	{
		fread(&v, 1, 8, fd);
		tgt8b_ovfa[i]=v;
	}
	
	fclose(fd);

	CompactPermTab8B();

	return(0);
}

int GenShufPert8B_OpRand(u64 *rhsn)
{
	u64 hsn;
	int op;
	
	hsn=*rhsn;
	hsn=hsn*1152921504606846883ULL+7;
	
	op=(hsn>>60)&15;
	while((op<1) || (op>SHUF_MAXOPB))
	{
		hsn=hsn*1152921504606846883ULL+7;
		op=(hsn>>60)&15;
	}
	*rhsn=hsn;
	return(op);
}

int GenShufPert8B_RingMulHsna(u64 *rhsn)
{
	u64 th0, th1, th2, th3;
	
	th0=(rhsn[0]*4294967291)+(rhsn[3]>>32)+3;
	th1=(rhsn[1]*4294967291)+(rhsn[0]>>32)+3;
	th2=(rhsn[2]*4294967291)+(rhsn[1]>>32)+3;
	th3=(rhsn[3]*4294967291)+(rhsn[2]>>32)+3;
	rhsn[0]=th0^th1;
	rhsn[1]=th1^th2;
	rhsn[2]=th2^th3;
	rhsn[3]=th3^th0;
	return(0);
}

int GenShufPert8B_RehashHsna(u64 *rhsn, u64 bits)
{
	GenShufPert8B_RingMulHsna(rhsn);
	rhsn[0]+=bits;
	return(0);
}

int GenShufPert8B_RandHsna(u64 *rhsn)
{
	int i;
	GenShufPert8B_RingMulHsna(rhsn);
	i=(rhsn[3]>>48)&65535;
	return(i);
}

byte gen_shuf8_cancel=0;

int GenCheckCancel()
{
	int i, j, k;

#ifdef _WIN32
	while(_kbhit())
	{
		j=_getch();

		if(j=='\x1B')
		{
			printf("Cancel Seen\n");
			gen_shuf8_cancel=1;
		}
	}
#endif	
}

#define SHUF8(a, b, c, d, e, f, g, h)				\
	(((a)    )|((b)<< 3)|((c)<< 6)|((d)<< 9)|		\
	 ((e)<<12)|((f)<<15)|((g)<<18)|((h)<<21))

int GenShufPert8B()
{
	static u64 hsna[4];
	static byte hsna_seeded=0;
	char tb[128];
	u64 opv, hsn;
	u32 opv0, opv1, opv2, opv3;
	int op1, op2, op3, op4;
	int op5, op6, op7, op8;
	int op9, op10, op11, op12;
	int op13, op14, op15, op16;
	int op17, op18, op19, op20;
	int op21, op22, op23, op24;
	int op25, op26, op27, op28;
	int op29, op30, op31, op32;

	int ln0, ln1, itn;
	int p0, snn, shtn;
	int t0, t1, t2, t3, t00;
	int i, j, k, l;
	
	snn=0;

	op12=0;		op11=0;
	op10=0;		op9=0;
	op8=0;		op7=0;

	t00=clock();

#if 1
	p0=SHUF8(1, 3, 5, 7, 0, 2, 4, 6);
	if(tgt8b_perm[p0]<0)
	{
		printf("Brute-Force, Len <= 8\n");

	//	for(op10=0; op10<=SHUF_MAXOPB; op10++)
	//		for(op9=0; op9<=SHUF_MAXOPB; op9++)
	//		for(op9=op10?1:0; op9<=SHUF_MAXOPB; op9++)
		if(1)
		{
		
	//		if(op10 && !op9)
	//			continue;

	//		if(op9 && (op9==op10))
	//			continue;
		
	//		for(op8=0; op8<=SHUF_MAXOPB; op8++)
	//			for(op7=0; op7<=SHUF_MAXOPB; op7++)
	//				for(op6=0; op6<=SHUF_MAXOPB; op6++)
	//					for(op5=0; op5<=SHUF_MAXOPB; op5++)

			t0=clock();

			for(op8=op9?1:0; op8<=SHUF_MAXOPB; op8++)
				for(op7=op8?1:0; op7<=SHUF_MAXOPB; op7++)
			{
				GenCheckCancel();
				if(gen_shuf8_cancel)
					break;

				for(op6=op7?1:0; op6<=SHUF_MAXOPB; op6++)
					for(op5=op6?1:0; op5<=SHUF_MAXOPB; op5++)
				{				
		//			if(op9 && !op8)
		//				continue;
		//			if(op8 && !op7)
		//				continue;
		//			if(op7 && !op6)
		//				continue;
		//			if(op6 && !op5)
		//				continue;

		//			if(op8==op9)	continue;
		//			if(op7==op8)	continue;
		//			if(op6==op7)	continue;
		//			if(op5==op6)	continue;

					if(op8 && (op8==op9))	continue;
					if(op7 && (op7==op8))	continue;
					if(op6 && (op6==op7))	continue;
					if(op5 && (op5==op6))	continue;
				
		//			ln0=4;
		//			if(op5)ln0=5;
		//			if(op6)ln0=6;
		//			if(op7)ln0=7;
		//			if(op8)ln0=8;
				
		//			for(op4=0; op4<=SHUF_MAXOPB; op4++)
		//				for(op3=0; op3<=SHUF_MAXOPB; op3++)
					for(op4=op5?1:0; op4<=SHUF_MAXOPB; op4++)
						for(op3=op4?1:0; op3<=SHUF_MAXOPB; op3++)
					{
		//				if(op5 && !op4)
		//					continue;
		//				if(op4 && !op3)
		//					continue;

		//				for(op2=0; op2<=SHUF_MAXOPB; op2++)
		//					for(op1=0; op1<=SHUF_MAXOPB; op1++)
						for(op2=op3?1:0; op2<=SHUF_MAXOPB; op2++)
							for(op1=op2?1:0; op1<=SHUF_MAXOPB; op1++)
						{
							p0=	(0<< 0)|(1<< 3)|(2<< 6)|(3<< 9)|
								(4<<12)|(5<<15)|(6<<18)|(7<<21);

							p0=ApplyShuf8B(p0, op1);
							p0=ApplyShuf8B(p0, op2);
							p0=ApplyShuf8B(p0, op3);
							p0=ApplyShuf8B(p0, op4);

							p0=ApplyShuf8B(p0, op5);
							p0=ApplyShuf8B(p0, op6);
							p0=ApplyShuf8B(p0, op7);
							p0=ApplyShuf8B(p0, op8);

							p0=ApplyShuf8B(p0, op9);
							p0=ApplyShuf8B(p0, op10);
							p0=ApplyShuf8B(p0, op11);
							p0=ApplyShuf8B(p0, op12);
							
							p0&=16777215;
							
							if(tgt8b_perm[p0]<0)
							{
								opv0=(op1<< 0)|(op2<< 4)|
									(op3<< 8)|(op4<<12)|
									(op5<<16)|(op6<<20)|
									(op7<<24)|(op8<<28);

								opv1=(op9<< 0)|(op10<< 4)|
									(op11<< 8)|(op12<<12);

								opv=opv0 | (((u64)opv1)<<32);

								ln0=Shuf8B_ShufLen(opv);

								tgt8b_perm[p0]=p0;
								tgt8b_opra[p0]=opv;
								tgt8b_opln[p0]=ln0;
							}
						}
					}
				}
			}

			t1=clock();
			t2=t1-t0;

			j=0;
			for(i=0; i<16777216; i++)
			{
				if(tgt8b_perm[i]>=0)
					j++;
			}
			
			k=16777216;
			printf("%d-%d: %d/%d %.4f%% dt=%d\n",
				op10, op9, j, k, (j*100.0)/k, t2);
			
	//		sprintf(tb, "swaps_tab8b_sn%d.bin", snn);
			sprintf(tb, "swaps_tab8b_sn.dat", snn);
			snn++;
			
			GenShuf_DumpPermTab8B(tb);
		
		}
	}
#endif
	
	if(!hsna_seeded)
	{
		hsna[0]=0x12345678;		hsna[1]=0x12345678;
		hsna[2]=0x12345678;		hsna[3]=0x12345678;
		for(i=0; i<16777216; i++)
			{ GenShufPert8B_RehashHsna(hsna, tgt8b_opra[i]); }
		hsna_seeded=1;
		
		printf("HSNA Reseed %016llX %016llX %016llX %016llX\n",
			hsna[0], hsna[1], hsna[2], hsna[3]);
	}

	itn = 1024;
	
//	itn = 2048;
//	itn = 6144;
	
#if 1
	hsn=0x12345678;
	for(i=0; i<16; i++)
		hsn=(hsn*65521)+7;

	shtn=0;
	
//	for(k=0; k<5; k++)
	for(k=1; k<9; k++)
	{
		op9=0;		op10=0;
		op11=0;		op12=0;

		op13=0;		op14=0;
		op15=0;		op16=0;

		GenCheckCancel();
		if(gen_shuf8_cancel)
			break;

//		printf("Probes n=%d\n", 12+k);
		printf("Probes len=%d\n", 8+k);
		ln0=8+k;
		
		t0=clock();
		
	//	for(i=0; i<(1<<30); i++)
//		for(i=0; i<1024; i++)
//		for(i=0; i<2048; i++)
		for(i=0; i<itn; i++)
		{
			GenCheckCancel();
			if(gen_shuf8_cancel)
				break;

			hsn=(hsn*65521)+GenShufPert8B_RandHsna(hsna);
			hsn=(hsn*65521)+GenShufPert8B_RandHsna(hsna);
			hsn=(hsn*65521)+GenShufPert8B_RandHsna(hsna);
			hsn=(hsn*65521)+GenShufPert8B_RandHsna(hsna);

//			hsn=hsn*65521+snn;
//			snn++;
		
//			printf("%d / 1024\r", i);
//			printf("%d / 2048\r", i);
			printf("%d / %d\r", i, itn);

			for(j=0; j<(1<<20); j++)
			{
				op1=GenShufPert8B_OpRand(&hsn);
				op2=GenShufPert8B_OpRand(&hsn);
				op3=GenShufPert8B_OpRand(&hsn);
				op4=GenShufPert8B_OpRand(&hsn);
				op5=GenShufPert8B_OpRand(&hsn);
				op6=GenShufPert8B_OpRand(&hsn);
				op7=GenShufPert8B_OpRand(&hsn);
				op8=GenShufPert8B_OpRand(&hsn);
//				op9=GenShufPert8B_OpRand(&hsn);
//				op10=GenShufPert8B_OpRand(&hsn);
//				op11=GenShufPert8B_OpRand(&hsn);
//				op12=GenShufPert8B_OpRand(&hsn);
				
				switch(k)
				{
				default:
				case 8:		op16=GenShufPert8B_OpRand(&hsn);
				case 7:		op15=GenShufPert8B_OpRand(&hsn);
				case 6:		op14=GenShufPert8B_OpRand(&hsn);
				case 5:		op13=GenShufPert8B_OpRand(&hsn);
				case 4:		op12=GenShufPert8B_OpRand(&hsn);
				case 3:		op11=GenShufPert8B_OpRand(&hsn);
				case 2:		op10=GenShufPert8B_OpRand(&hsn);
				case 1:		op9=GenShufPert8B_OpRand(&hsn);
				case 0:
					break;
				}
			
				p0=	(0<< 0)|(1<< 3)|(2<< 6)|(3<< 9)|
					(4<<12)|(5<<15)|(6<<18)|(7<<21);

				p0=ApplyShuf8B(p0, op1);
				p0=ApplyShuf8B(p0, op2);
				p0=ApplyShuf8B(p0, op3);
				p0=ApplyShuf8B(p0, op4);

				p0=ApplyShuf8B(p0, op5);
				p0=ApplyShuf8B(p0, op6);
				p0=ApplyShuf8B(p0, op7);
				p0=ApplyShuf8B(p0, op8);

				p0=ApplyShuf8B(p0, op9);
				p0=ApplyShuf8B(p0, op10);
				p0=ApplyShuf8B(p0, op11);
				p0=ApplyShuf8B(p0, op12);

				p0=ApplyShuf8B(p0, op13);
				p0=ApplyShuf8B(p0, op14);
				p0=ApplyShuf8B(p0, op15);
				p0=ApplyShuf8B(p0, op16);
				
				p0&=16777215;
				
				ln1=tgt8b_opln[p0];
				
//				if(tgt8b_perm[p0]<0)
				if((tgt8b_perm[p0]<0) || (ln0<ln1))
				{
					if(op16==15)
						continue;
					
					if(tgt8b_perm[p0]>=0)
						shtn++;

					opv0=	(op1 << 0)|(op2 << 4)|(op3 << 8)|(op4 <<12)|
							(op5 <<16)|(op6 <<20)|(op7 <<24)|(op8 <<28);
					opv1=	(op9 << 0)|(op10<< 4)|(op11<< 8)|(op12<<12)|
							(op13<<16)|(op14<<20)|(op15<<24)|(op16<<28);

					opv=opv0 | (((u64)opv1)<<32);

					tgt8b_perm[p0]=p0;
					tgt8b_opra[p0]=opv;
					tgt8b_opln[p0]=ln0;
				}
			}
		}
		printf("\n");

		t1=clock();
		t2=t1-t0;
		t3=t1-t00;

		j=0;
		for(i=0; i<16777216; i++)
		{
			if(tgt8b_perm[i]>=0)
				j++;
		}
		
		i=16777216;
		printf("%d/%d %.4f%% shtn=%d(%.2f%%) dt=%.2fs t=%.2fs\n",
			j, i, (j*100.0)/i, shtn, (shtn*100.0)/i,
			t2/((double)CLOCKS_PER_SEC),
			t3/((double)CLOCKS_PER_SEC));

		sprintf(tb, "swaps_tab8b_sn1.dat", snn);
		GenShuf_DumpPermTab8B(tb);

		if(j>=16777216)
			break;		
	}
#endif

#if 1
	for(k=0; k<9; k++)
	{
		GenCheckCancel();
		if(gen_shuf8_cancel)
			break;

		op17=0;
		op18=0;
		op19=0;
		op20=0;

		op21=0;
		op22=0;
		op23=0;
		op24=0;
	
		printf("Probes len=%d\n", 16+k);
		ln0=16+k;

		t0=clock();
		
	//	for(i=0; i<(1<<30); i++)
//		for(i=0; i<1024; i++)
//		for(i=0; i<2048; i++)
		for(i=0; i<itn; i++)
		{
			GenCheckCancel();
			if(gen_shuf8_cancel)
				break;

			hsn=(hsn*65521)+GenShufPert8B_RandHsna(hsna);
			hsn=(hsn*65521)+GenShufPert8B_RandHsna(hsna);
			hsn=(hsn*65521)+GenShufPert8B_RandHsna(hsna);
			hsn=(hsn*65521)+GenShufPert8B_RandHsna(hsna);

//			hsn=hsn*65521+snn;
//			snn++;

//			printf("%d / 1024\r", i);
//			printf("%d / 2048\r", i);
			printf("%d / %d\r", i, itn);
			for(j=0; j<(1<<20); j++)
			{
				op1=GenShufPert8B_OpRand(&hsn);
				op2=GenShufPert8B_OpRand(&hsn);
				op3=GenShufPert8B_OpRand(&hsn);
				op4=GenShufPert8B_OpRand(&hsn);
				op5=GenShufPert8B_OpRand(&hsn);
				op6=GenShufPert8B_OpRand(&hsn);
				op7=GenShufPert8B_OpRand(&hsn);
				op8=GenShufPert8B_OpRand(&hsn);
				op9=GenShufPert8B_OpRand(&hsn);
				op10=GenShufPert8B_OpRand(&hsn);
				op11=GenShufPert8B_OpRand(&hsn);
				op12=GenShufPert8B_OpRand(&hsn);
				op13=GenShufPert8B_OpRand(&hsn);
				op14=GenShufPert8B_OpRand(&hsn);
				op15=GenShufPert8B_OpRand(&hsn);
				op16=GenShufPert8B_OpRand(&hsn);
				
				switch(k)
				{
				default:
				case 8:		op24=GenShufPert8B_OpRand(&hsn);
				case 7:		op23=GenShufPert8B_OpRand(&hsn);
				case 6:		op22=GenShufPert8B_OpRand(&hsn);
				case 5:		op21=GenShufPert8B_OpRand(&hsn);
				case 4:		op20=GenShufPert8B_OpRand(&hsn);
				case 3:		op19=GenShufPert8B_OpRand(&hsn);
				case 2:		op18=GenShufPert8B_OpRand(&hsn);
				case 1:		op17=GenShufPert8B_OpRand(&hsn);
				case 0:
					break;
				}
			
				p0=	(0<< 0)|(1<< 3)|(2<< 6)|(3<< 9)|
					(4<<12)|(5<<15)|(6<<18)|(7<<21);

				p0=ApplyShuf8B(p0, op1);
				p0=ApplyShuf8B(p0, op2);
				p0=ApplyShuf8B(p0, op3);
				p0=ApplyShuf8B(p0, op4);

				p0=ApplyShuf8B(p0, op5);
				p0=ApplyShuf8B(p0, op6);
				p0=ApplyShuf8B(p0, op7);
				p0=ApplyShuf8B(p0, op8);

				p0=ApplyShuf8B(p0, op9);
				p0=ApplyShuf8B(p0, op10);
				p0=ApplyShuf8B(p0, op11);
				p0=ApplyShuf8B(p0, op12);

				p0=ApplyShuf8B(p0, op13);
				p0=ApplyShuf8B(p0, op14);
				p0=ApplyShuf8B(p0, op15);
				p0=ApplyShuf8B(p0, op16);

				p0=ApplyShuf8B(p0, op17);
				p0=ApplyShuf8B(p0, op18);
				p0=ApplyShuf8B(p0, op19);
				p0=ApplyShuf8B(p0, op20);

				if(op21)
				{
					p0=ApplyShuf8B(p0, op21);
					p0=ApplyShuf8B(p0, op22);
					p0=ApplyShuf8B(p0, op23);
					p0=ApplyShuf8B(p0, op24);
				}
				
				p0&=16777215;

				ln1=tgt8b_opln[p0];
				
//				if(tgt8b_perm[p0]<0)
				if((tgt8b_perm[p0]<0) || (ln0<ln1))
				{
					if(tgt8b_perm[p0]>=0)
						shtn++;

					opv0=	(op1 << 0)|(op2 << 4)|(op3 << 8)|(op4 <<12)|
							(op5 <<16)|(op6 <<20)|(op7 <<24)|(op8 <<28);
					opv1=	(op9 << 0)|(op10<< 4)|(op11<< 8)|(op12<<12)|
							(op13<<16)|(op14<<20)|(op15<<24)|(op16<<28);
					opv2=	(op17<< 0)|(op18<< 4)|(op19<< 8)|(op20<<12)|
							(op21<<16)|(op22<<20)|(op23<<24)|(op24<<28);

					l=tgt8b_novf++;
//					opv=opv0 | ((0xF0000000ULL+l)<<32);
					opv=opv2 | ((0xF0000000ULL+l)<<32);
//					opv=opv0 | (((u64)opv1)<<32);

					tgt8b_perm[p0]=p0;
					tgt8b_opra[p0]=opv;
					tgt8b_opln[p0]=ln0;

//					opv=opv1 | (((u64)opv2)<<32);
					opv=opv0 | (((u64)opv1)<<32);
					tgt8b_ovfa[l]=opv;
				}
			}
		}
		printf("\n");

		t1=clock();
		t2=t1-t0;
		t3=t1-t00;

		j=0;
		for(i=0; i<16777216; i++)
		{
			if(tgt8b_perm[i]>=0)
				j++;
		}
		
		i=16777216;
//		printf("%d/%d %.4f%% shtn=%d dt=%d\n", j, i, (j*100.0)/i, shtn, t2);
		printf("%d/%d %.4f%% shtn=%d(%.2f%%) dt=%.2fs t=%.2fs\n",
			j, i, (j*100.0)/i, shtn, (shtn*100.0)/i,
			t2/((double)CLOCKS_PER_SEC),
			t3/((double)CLOCKS_PER_SEC));

		sprintf(tb, "swaps_tab8b_sn2.dat", snn);
		GenShuf_DumpPermTab8B(tb);

		if(j>=16777216)
			break;		
	}
#endif

#if 0
	for(k=0; k<9; k++)
	{
		GenCheckCancel();
		if(gen_shuf8_cancel)
			break;

		op25=0;
		op26=0;
		op27=0;
		op28=0;

		op29=0;
		op30=0;
		op31=0;
		op32=0;
	
		printf("Probes len=%d\n", 24+k);
		ln0=24+k;

		t0=clock();
		
	//	for(i=0; i<(1<<30); i++)
//		for(i=0; i<1024; i++)
//		for(i=0; i<2048; i++)
		for(i=0; i<itn; i++)
		{
			GenCheckCancel();
			if(gen_shuf8_cancel)
				break;

			hsn=(hsn*65521)+GenShufPert8B_RandHsna(hsna);
			hsn=(hsn*65521)+GenShufPert8B_RandHsna(hsna);
			hsn=(hsn*65521)+GenShufPert8B_RandHsna(hsna);
			hsn=(hsn*65521)+GenShufPert8B_RandHsna(hsna);

//			hsn=hsn*65521+snn;
//			snn++;

//			printf("%d / 1024\r", i);
//			printf("%d / 2048\r", i);
			printf("%d / %d\r", i, itn);
			for(j=0; j<(1<<20); j++)
			{
				op1=GenShufPert8B_OpRand(&hsn);
				op2=GenShufPert8B_OpRand(&hsn);
				op3=GenShufPert8B_OpRand(&hsn);
				op4=GenShufPert8B_OpRand(&hsn);
				op5=GenShufPert8B_OpRand(&hsn);
				op6=GenShufPert8B_OpRand(&hsn);
				op7=GenShufPert8B_OpRand(&hsn);
				op8=GenShufPert8B_OpRand(&hsn);
				op9=GenShufPert8B_OpRand(&hsn);
				op10=GenShufPert8B_OpRand(&hsn);
				op11=GenShufPert8B_OpRand(&hsn);
				op12=GenShufPert8B_OpRand(&hsn);
				op13=GenShufPert8B_OpRand(&hsn);
				op14=GenShufPert8B_OpRand(&hsn);
				op15=GenShufPert8B_OpRand(&hsn);
				op16=GenShufPert8B_OpRand(&hsn);
				op17=GenShufPert8B_OpRand(&hsn);
				op18=GenShufPert8B_OpRand(&hsn);
				op19=GenShufPert8B_OpRand(&hsn);
				op20=GenShufPert8B_OpRand(&hsn);
				op21=GenShufPert8B_OpRand(&hsn);
				op22=GenShufPert8B_OpRand(&hsn);
				op23=GenShufPert8B_OpRand(&hsn);
				op24=GenShufPert8B_OpRand(&hsn);
				
				switch(k)
				{
				default:
				case 8:		op32=GenShufPert8B_OpRand(&hsn);
				case 7:		op31=GenShufPert8B_OpRand(&hsn);
				case 6:		op30=GenShufPert8B_OpRand(&hsn);
				case 5:		op29=GenShufPert8B_OpRand(&hsn);
				case 4:		op28=GenShufPert8B_OpRand(&hsn);
				case 3:		op27=GenShufPert8B_OpRand(&hsn);
				case 2:		op26=GenShufPert8B_OpRand(&hsn);
				case 1:		op25=GenShufPert8B_OpRand(&hsn);
				case 0:
					break;
				}
			
				p0=	(0<< 0)|(1<< 3)|(2<< 6)|(3<< 9)|
					(4<<12)|(5<<15)|(6<<18)|(7<<21);

				p0=ApplyShuf8B(p0, op1);
				p0=ApplyShuf8B(p0, op2);
				p0=ApplyShuf8B(p0, op3);
				p0=ApplyShuf8B(p0, op4);

				p0=ApplyShuf8B(p0, op5);
				p0=ApplyShuf8B(p0, op6);
				p0=ApplyShuf8B(p0, op7);
				p0=ApplyShuf8B(p0, op8);

				p0=ApplyShuf8B(p0, op9);
				p0=ApplyShuf8B(p0, op10);
				p0=ApplyShuf8B(p0, op11);
				p0=ApplyShuf8B(p0, op12);

				p0=ApplyShuf8B(p0, op13);
				p0=ApplyShuf8B(p0, op14);
				p0=ApplyShuf8B(p0, op15);
				p0=ApplyShuf8B(p0, op16);

				p0=ApplyShuf8B(p0, op17);
				p0=ApplyShuf8B(p0, op18);
				p0=ApplyShuf8B(p0, op19);
				p0=ApplyShuf8B(p0, op20);

				p0=ApplyShuf8B(p0, op21);
				p0=ApplyShuf8B(p0, op22);
				p0=ApplyShuf8B(p0, op23);
				p0=ApplyShuf8B(p0, op24);

				p0=ApplyShuf8B(p0, op25);
				p0=ApplyShuf8B(p0, op26);
				p0=ApplyShuf8B(p0, op27);
				p0=ApplyShuf8B(p0, op28);

				if(op29)
				{
					p0=ApplyShuf8B(p0, op29);
					p0=ApplyShuf8B(p0, op30);
					p0=ApplyShuf8B(p0, op31);
					p0=ApplyShuf8B(p0, op32);
				}
				
				p0&=16777215;

				ln1=tgt8b_opln[p0];
				
//				if(tgt8b_perm[p0]<0)
				if((tgt8b_perm[p0]<0) || (ln0<ln1))
				{
					if(tgt8b_perm[p0]>=0)
						shtn++;

					opv0=	(op1 << 0)|(op2 << 4)|(op3 << 8)|(op4 <<12)|
							(op5 <<16)|(op6 <<20)|(op7 <<24)|(op8 <<28);
					opv1=	(op9 << 0)|(op10<< 4)|(op11<< 8)|(op12<<12)|
							(op13<<16)|(op14<<20)|(op15<<24)|(op16<<28);
					opv2=	(op17<< 0)|(op18<< 4)|(op19<< 8)|(op20<<12)|
							(op21<<16)|(op22<<20)|(op23<<24)|(op24<<28);
					opv3=	(op25<< 0)|(op26<< 4)|(op27<< 8)|(op28<<12)|
							(op29<<16)|(op30<<20)|(op31<<24)|(op32<<28);

					l=tgt8b_novf++;
					tgt8b_novf++;
//					opv=opv0 | ((0xF0000000ULL+l)<<32);
//					opv=opv2 | ((0xF0000000ULL+l)<<32);
//					opv=opv0 | (((u64)opv1)<<32);
					opv=(0xF9000000ULL+l)<<32;

					tgt8b_perm[p0]=p0;
					tgt8b_opra[p0]=opv;
					tgt8b_opln[p0]=ln0;

//					opv=opv1 | (((u64)opv2)<<32);
					opv=opv0 | (((u64)opv1)<<32);
					tgt8b_ovfa[l+0]=opv;

					opv=opv2 | (((u64)opv3)<<32);
					tgt8b_ovfa[l+1]=opv;
				}
			}
		}
		printf("\n");

		t1=clock();
		t2=t1-t0;
		t3=t1-t00;

		j=0;
		for(i=0; i<16777216; i++)
		{
			if(tgt8b_perm[i]>=0)
				j++;
		}
		
		i=16777216;
//		printf("%d/%d %.4f%% shtn=%d dt=%d\n", j, i, (j*100.0)/i, shtn, t2);
		printf("%d/%d %.4f%% shtn=%d(%.2f%%) dt=%.2fs t=%.2fs\n",
			j, i, (j*100.0)/i, shtn, (shtn*100.0)/i,
			t2/((double)CLOCKS_PER_SEC),
			t3/((double)CLOCKS_PER_SEC));

		sprintf(tb, "swaps_tab8b_sn2.dat", snn);
		GenShuf_DumpPermTab8B(tb);

		if(j>=16777216)
			break;		
	}
#endif

	j=0;
	for(i=0; i<16777216; i++)
	{
		if(tgt8b_perm[i]>=0)
			j++;
	}
	
	k=16777216;
	printf("%d/%d %.4f%%\n", j, k, (j*100.0)/k);

	CompactPermTab8B();
	
	GenShuf_DumpPermTab8B("swaps_tab8b.dat");

	return(j>=k);
}

int main()
{
	int opa[8];
	int i, j, k;

	for(i=0; i<16777216; i++)
	{
		tgt8b_perm[i]=-1;
		tgt8b_opra[i]=-1;
	}

	for(i=0; i<256; i++)
	{
		j=FindShuf(i, opa);
		if(j<=0)
		{
			printf("%02X(%d,%d,%d,%d) Fail\n", i,
				(i>>0)&3, (i>>2)&3, (i>>4)&3, (i>>6)&3);
			continue;
		}
		
		printf("%02X(%d,%d,%d,%d)  %d %d %d %d  %d %d %d %d\n", i,
			(i>>0)&3, (i>>2)&3, (i>>4)&3, (i>>6)&3,
			opa[0], opa[1], opa[2], opa[3],
			opa[4], opa[5], opa[6], opa[7]);
	}

#if 1
	for(i=0; i<256; i++)
	{
		j=FindShuf(i, opa);
		if(j<=0)
		{
			printf("/* %02X(%d,%d,%d,%d) Fail */\n", i,
				(i>>0)&3, (i>>2)&3, (i>>4)&3, (i>>6)&3);
			break;
		}
		
		printf("{%d, %d, %d, %d, %d, %d, %d, %d}, /* %02X(%d,%d,%d,%d) */\n",
			opa[0], opa[1], opa[2], opa[3],
			opa[4], opa[5], opa[6], opa[7], i,
			(i>>0)&3, (i>>2)&3, (i>>4)&3, (i>>6)&3);
	}
#endif

#if 0
	for(i=0; i<16; i++)
	{
		j=(rand() ^ (rand()<<8))&0xFFFFFF;
		
		k=FindShuf8B(j, opa);
		if(k<=0)
		{
			printf("%06X(%d,%d,%d,%d,%d,%d,%d,%d) Fail\n", j,
				(j>> 0)&7, (j>> 3)&7, (j>> 6)&7, (j>> 9)&7,
				(j>>12)&7, (j>>15)&7, (j>>18)&7, (j>>21)&7);
			continue;
		}
		
		printf("%06X(%d,%d,%d,%d,%d,%d,%d,%d)  %d %d %d %d  %d %d %d %d\n", i,
			(j>> 0)&7, (j>> 3)&7, (j>> 6)&7, (j>> 9)&7,
			(j>>12)&7, (j>>15)&7, (j>>18)&7, (j>>21)&7,
			opa[0], opa[1], opa[2], opa[3],
			opa[4], opa[5], opa[6], opa[7]);
	}
#endif
	
	printf("Shuf Stat ");
	for(i=0; i<16; i++)
		printf(" %d", stat_shuf[i]);
	printf("\n");

	printf("Avg Len %f\n", (1.0*stat_len)/stat_cnt);
	
	LoadPermTab8B("swaps_tab8b.dat");

	while(1)
	{
		GenCheckCancel();
		if(gen_shuf8_cancel)
			break;
		i=GenShufPert8B();
		if(i!=0)
			break;
	}
}
