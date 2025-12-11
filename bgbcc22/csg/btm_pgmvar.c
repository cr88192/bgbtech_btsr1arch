u32 btm_var_hi32[1024];
short btm_var_n_hi32;

u64 BTM_VarAllocArray(int sz);


u64 BTM_VarWrapPointer(void *ptr, int tag)
{
	u64 v0, v1;
	u32 h;
	int i, n;
	
	v0=(u64)ptr;
	if((v0>>48)==0)
	{
		v0|=((tag&4095ULL)<<48);
		return(v0);
	}
	
	h=v0>>32;
	for(i=0; i<btm_var_n_hi32; i++)
		if(btm_var_hi32[i]==h)
			break;
	if(i>=btm_var_n_hi32)
	{
		i=btm_var_n_hi32++;
		btm_var_hi32[i]=h;
	}

	v1=((tag&4095ULL)<<48)|(0xFULL<<60)|
		((i&0xFFFFULL)<<32)|((u32)v0);
	return(v1);
}

void *BTM_VarUnwrapPointer(u64 val)
{
	u64 v0;
	int i;

	if((val>>60)==0)
	{
		v0=val&0x0000FFFFFFFFFFFFULL;
		return((void *)v0);
	}

	if((val>>60)==15)
	{
		i=(val>>32)&0xFFFF;
		v0=((u32)val)|(((u64)btm_var_hi32[i])<<32);
		return((void *)v0);
	}
	
	return(NULL);
}

int BTM_VarIsPointerTag(u64 val, int tag)
{
	if(((val>>60)==0) || ((val>>60)==15))
	{
		if(((val>>48)&4095)==tag)
			return(1);
		return(0);
	}
	return(0);
}

u64 BTM_VarWrapInt(s64 val)
{
	u64 val1;
	val1=BRM_PGMVAR_FIXNUM+(val&((1ULL<<62)-1));
	return(val1);
}

u64 BTM_VarWrapFloat(double val)
{
	u64 val0;
	u64 val1;
	memcpy(&val0, &val, 8);
	val1=BRM_PGMVAR_FLONUM+(val0>>2);
	return(val1);
}

s64 BTM_VarUnwrapInt(u64 val)
{
	if((val>>62)==1)
	{
		return(((s64)(val<<2))>>2);
	}
	
	return(0);
}

double BTM_VarUnwrapFloat(u64 val)
{
	u64 v0;
	double fv;

	if((val>>62)==1)
	{
		return(BTM_VarUnwrapInt(val));
	}

	if((val>>62)==2)
	{
		v0=val<<2;
		memcpy(&fv, &v0, 8);
		return(fv);
	}

	return(0);
}

int BTM_VarIsInt(u64 val)
{
	if((val>>62)==1)
		return(1);
	return(0);
}

int BTM_VarIsFloat(u64 val)
{
	if((val>>62)==2)
		return(1);
	return(0);
}

int BTM_VarIsString(u64 val)
{
	if((val>>48)==BRM_VART16_SMALLSTRA)
		return(1);
	if((val>>48)==BRM_VART16_SMALLSTRB)
		return(1);
	return(0);
}

char *BTM_VarUnwrapString(u64 val)
{
	char *s0;

	if((val>>48)==BRM_VART16_SMALLSTRA)
	{
		s0=bccx_stridx_i((int)val);
		return(s0+1);
	}
	if((val>>48)==BRM_VART16_SMALLSTRB)
	{
		s0=bccx_stridx_i((int)val);
		return(s0+0);
	}
	
	return(NULL);
}

u64 BTM_VarWrapString(char *str)
{
	u64 val;
	int ix;
	ix=bccx_strdup_i(str);
	val=BRM_PGMVAR_SMALLSTRB+ix;
	return(val);
}

u64 BTM_VarBinaryOp(u64 vala, u64 valb, int opr)
{
	u64 *ptra, *ptrb, *ptrc;
	u64 valc;
	s64 lx, ly, lz;
	double fx, fy, fz;
	int sza, szb, sz;
	int i, j, k;

	if((vala>>62)==(valb>>62))
	{
		if((vala>>62)==1)
		{
			lx=BTM_VarUnwrapInt(vala);
			ly=BTM_VarUnwrapInt(valb);
			
			if(opr==BRM_PGMOPR_ADD)		lz=lx+ly;
			if(opr==BRM_PGMOPR_SUB)		lz=lx-ly;
			if(opr==BRM_PGMOPR_MUL)		lz=lx*ly;
			if(opr==BRM_PGMOPR_AND)		lz=lx&ly;
			if(opr==BRM_PGMOPR_OR)		lz=lx|ly;
			if(opr==BRM_PGMOPR_XOR)		lz=lx^ly;
			if(opr==BRM_PGMOPR_SHL)		lz=lx<<((int)ly);
			if(opr==BRM_PGMOPR_SHR)		lz=lx>>((int)ly);

			if(opr==BRM_PGMOPR_DIV)
			{
				lz=0;
				if(ly!=0)
					lz=lx/ly;
			}
			if(opr==BRM_PGMOPR_MOD)
			{
				lz=0;
				if(ly!=0)
					lz=lx%ly;
			}

			if(opr==BRM_PGMOPR_EQ)		lz=lx==ly;
			if(opr==BRM_PGMOPR_NE)		lz=lx!=ly;
			if(opr==BRM_PGMOPR_GT)		lz=lx> ly;
			if(opr==BRM_PGMOPR_LE)		lz=lx<=ly;
			if(opr==BRM_PGMOPR_LT)		lz=lx< ly;
			if(opr==BRM_PGMOPR_GE)		lz=lx>=ly;
			
			return(BTM_VarWrapInt(lz));
		}
	}

	if(		(((vala>>62)==1) || ((vala>>62)==2)) ||
			(((valb>>62)==1) || ((valb>>62)==2)) )
	{
		fx=BTM_VarUnwrapFloat(vala);
		fy=BTM_VarUnwrapFloat(valb);
		lx=fx;
		ly=fy;

		if((opr>=BRM_PGMOPR_EQ) && (opr<=BRM_PGMOPR_GE))
		{
			if(opr==BRM_PGMOPR_EQ)		lz=fx==fy;
			if(opr==BRM_PGMOPR_NE)		lz=fx!=fy;
			if(opr==BRM_PGMOPR_GT)		lz=fx> fy;
			if(opr==BRM_PGMOPR_LE)		lz=fx<=fy;
			if(opr==BRM_PGMOPR_LT)		lz=fx< fy;
			if(opr==BRM_PGMOPR_GE)		lz=fx>=fy;
			return(BTM_VarWrapInt(lz));
		}

		if((opr>=BRM_PGMOPR_AND) && (opr<=BRM_PGMOPR_SHR))
		{
			if(opr==BRM_PGMOPR_AND)		lz=lx&ly;
			if(opr==BRM_PGMOPR_OR)		lz=lx|ly;
			if(opr==BRM_PGMOPR_XOR)		lz=lx^ly;
			if(opr==BRM_PGMOPR_SHL)		lz=lx<<((int)ly);
			if(opr==BRM_PGMOPR_SHR)		lz=lx>>((int)ly);
			return(BTM_VarWrapInt(lz));
		}
			
		if(opr==BRM_PGMOPR_ADD)		fz=fx+fy;
		if(opr==BRM_PGMOPR_SUB)		fz=fx-fy;
		if(opr==BRM_PGMOPR_MUL)		fz=fx*fy;
//		if(opr==BRM_PGMOPR_AND)		fz=lx&ly;
//		if(opr==BRM_PGMOPR_OR)		fz=lx|ly;
//		if(opr==BRM_PGMOPR_XOR)		fz=lx^ly;
//		if(opr==BRM_PGMOPR_SHL)		fz=lx<<((int)ly);
//		if(opr==BRM_PGMOPR_SHR)		fz=lx>>((int)ly);

		if(opr==BRM_PGMOPR_DIV)
		{
			fz=0;
			if(fy!=0)
				fz=fx/fy;
		}
		if(opr==BRM_PGMOPR_MOD)
		{
			fz=0;
			if(fy!=0)
				fz=fmod(fx, fy);
		}
		
		return(BTM_VarWrapFloat(fz));
	}
	
	if(BTM_VarIsArray(vala) && BTM_VarIsArray(valb))
	{
		ptra=BTM_VarUnwrapPointer(vala);
		ptrb=BTM_VarUnwrapPointer(valb);
		sza=ptra[0];	szb=ptrb[0];
		if(sza==szb)
		{
			if(	(opr>=BRM_PGMOPR_ADD) &&
				(opr<=BRM_PGMOPR_DIV) )
			{
				valc=BTM_VarAllocArray(sza);
				ptrc=BTM_VarUnwrapPointer(valc);
				
				for(i=0; i<sza; i++)
				{
					ptrc[2+i]=BTM_VarBinaryOp(ptra[2+i], ptrb[2+i], opr);
				}
				
				return(valc);
			}
			
			if(opr==BRM_PGMOPR_XOR)
			{
				valc=BTM_VarWrapFloat(0);
				for(i=0; i<sza; i++)
				{
					valc=BTM_VarBinaryOp(
						BTM_VarBinaryOp(ptra[2+i], ptrb[2+i],
							BRM_PGMOPR_MUL),
						valc, BRM_PGMOPR_ADD);
				}
				return(valc);
			}

			if(opr==BRM_PGMOPR_MOD)
			{
				if(sza==2)
				{
					valc=BTM_VarBinaryOp(
						BTM_VarBinaryOp(ptra[2+0], ptrb[2+1],
							BRM_PGMOPR_MUL),
						BTM_VarBinaryOp(ptra[2+1], ptrb[2+0],
							BRM_PGMOPR_MUL),
						BRM_PGMOPR_SUB);
					return(valc);
				}

				if((sza==3) || (sza==4))
				{
					valc=BTM_VarAllocArray(sza);
					ptrc=BTM_VarUnwrapPointer(valc);

					ptrc[2+0]=BTM_VarBinaryOp(
						BTM_VarBinaryOp(ptra[2+1], ptrb[2+2],
							BRM_PGMOPR_MUL),
						BTM_VarBinaryOp(ptra[2+2], ptrb[2+1],
							BRM_PGMOPR_MUL),
						BRM_PGMOPR_SUB);
					ptrc[2+1]=BTM_VarBinaryOp(
						BTM_VarBinaryOp(ptra[2+2], ptrb[2+0],
							BRM_PGMOPR_MUL),
						BTM_VarBinaryOp(ptra[2+0], ptrb[2+2],
							BRM_PGMOPR_MUL),
						BRM_PGMOPR_SUB);
					ptrc[2+2]=BTM_VarBinaryOp(
						BTM_VarBinaryOp(ptra[2+0], ptrb[2+1],
							BRM_PGMOPR_MUL),
						BTM_VarBinaryOp(ptra[2+1], ptrb[2+0],
							BRM_PGMOPR_MUL),
						BRM_PGMOPR_SUB);
					if(sza==4)
						ptrc[2+3]=BTM_VarWrapFloat(0);
				
					return(valc);
				}
			}
		}
	}
	
	return(BRM_PGMVAR_UNDEFINED);
}

s64 BTM_VarBinaryOpAsInt(u64 vala, u64 valb, int opr)
{
	u64 valc;
	valc=BTM_VarBinaryOp(vala, valb, opr);
	return(BTM_VarUnwrapInt(valc));
}

u64 BTM_VarUnaryOp(u64 vala, int opr)
{
	s64 lx, ly, lz;
	double fx, fy, fz;

	if((vala>>62)==1)
	{
		lx=BTM_VarUnwrapInt(vala);
		if(opr==BRM_PGMOPR_ADD)		lz=lx;
		if(opr==BRM_PGMOPR_SUB)		lz=-lx;		
		if(opr==BRM_PGMOPR_NOT)		lz=(lx==0);		
		if(opr==BRM_PGMOPR_BITNOT)	lz=~lx;		
		return(BTM_VarWrapInt(lz));
	}

	if((vala>>62)==2)
	{
		fx=BTM_VarUnwrapFloat(vala);
		if(opr==BRM_PGMOPR_ADD)		fz=fx;
		if(opr==BRM_PGMOPR_SUB)		fz=-fx;		
		return(BTM_VarWrapFloat(fz));
	}
	
	return(BRM_PGMVAR_UNDEFINED);
}

int btm_vartag_array;

int BTM_VarInit()
{
	if(btm_vartag_array>0)
		return(0);
	btm_vartag_array=BCCX_StringToStridx("array");
	return(1);
}

u64 BTM_VarAllocArray(int sz)
{
	u64 val;
	void *ptr;

	ptr=btm_malloc(16+sz*sizeof(u64));
	*(int *)ptr=sz;
	
	val=BTM_VarWrapPointer(ptr, btm_vartag_array);
	return(val);
}

int BTM_VarIsArray(u64 val)
{
	return(BTM_VarIsPointerTag(val, btm_vartag_array));
}

int BTM_VarGetArraySize(u64 val)
{
	u64 *ptra;
	ptra=BTM_VarUnwrapPointer(val);
	return((int)ptra[0]);
}

u64 BTM_VarGetIndexArgs(u64 vala, u64 *args, int n)
{
	u64 valb, valc;
	u64 *aptr;
	int ix;

	if(BTM_VarIsArray(vala))
	{
		if(n==1)
		{
			aptr=BTM_VarUnwrapPointer(vala);
			ix=BTM_VarUnwrapInt(args[0]);
			return(aptr[ix+2]);
		}

		if(n>1)
		{
			aptr=BTM_VarUnwrapPointer(vala);
			ix=BTM_VarUnwrapInt(args[0]);
			valb=aptr[ix+2];
			valc=BTM_VarGetIndexArgs(valb, args+1, n-1);
			return(valc);
		}
		return(BRM_PGMVAR_UNDEFINED);
	}

	return(BRM_PGMVAR_UNDEFINED);
}

u64 BTM_VarSetIndexArgs(u64 vala, u64 *args, int n, u64 val2)
{
	u64 valb, valc;
	u64 *aptr;
	int ix;

	if(BTM_VarIsArray(vala))
	{
		if(n==1)
		{
			aptr=BTM_VarUnwrapPointer(vala);
			ix=BTM_VarUnwrapInt(args[0]);
			aptr[ix+2]=val2;
			return(val2);
		}

		if(n>1)
		{
			aptr=BTM_VarUnwrapPointer(vala);
			ix=BTM_VarUnwrapInt(args[0]);
			valb=aptr[ix+2];
			valc=BTM_VarSetIndexArgs(valb, args+1, n-1, val2);
			return(valc);
		}
		return(BRM_PGMVAR_UNDEFINED);
	}

	return(BRM_PGMVAR_UNDEFINED);
}

u64 BTM_VarMakeVectorArgs(u64 *args, int sz)
{
	u64 val;
	u64 *a;
	void *ptr;
	int i;

	ptr=btm_malloc(16+sz*sizeof(u64));
	*(int *)ptr=sz;
	
	a=((u64 *)ptr)+2;
	for(i=0; i<sz; i++)
		a[i]=args[i];
	
	val=BTM_VarWrapPointer(ptr, btm_vartag_array);
	return(val);
}

char *BTM_VarGetAsPrintString(u64 val)
{
	char tb[256];
	char *ct, *s0;
	u64 valb, valc;
	u64 *aptr;
	int i, ix, n;
	s64 li;
	double f;

	if(val==BRM_PGMVAR_UNDEFINED)
		return(NULL);

	if(BTM_VarIsString(val))
	{
		return(BTM_VarUnwrapString(val));
	}

	if(BTM_VarIsInt(val))
	{
		li=BTM_VarUnwrapInt(val);
		sprintf(tb, "%lld", li);
		return(bccx_strdup(tb));
	}

	if(BTM_VarIsFloat(val))
	{
		f=BTM_VarUnwrapFloat(val);
		sprintf(tb, "%f", f);
		return(bccx_strdup(tb));
	}

	if(BTM_VarIsArray(val))
	{
		aptr=BTM_VarUnwrapPointer(val);
		n=(int)(aptr[0]);

		ct=tb;
		*ct++='[';
		for(i=0; i<n; i++)
		{
			s0=BTM_VarGetAsPrintString(aptr[2+i]);
			if(!s0)
				s0="ERR";
			strcpy(ct, s0);
			ct+=strlen(ct);
			if((i+1)<n)
				*ct++=',';
		}
		*ct++=']';
		*ct=0;
		return(bccx_strdup(tb));
	}


	return(NULL);
}

int BTM_VarUnwrapVectorAsVf(u64 val, float *args, int sz)
{
	u64 *aptr;
	int i;

	aptr=BTM_VarUnwrapPointer(val);

	if(!aptr)
	{
		for(i=0; i<sz; i++)
			args[i]=0;
		return(sz);
	}
	
	for(i=0; i<sz; i++)
	{
		args[i]=BTM_VarUnwrapFloat(aptr[i+2]);
	}
	return(sz);
}
