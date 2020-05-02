void tkcl_softdiv_u(uint32_t dd, uint32_t dv,
	uint32_t *rq, uint32_t *rr)
{
	uint64_t f;
//	uint32_t f;
	uint32_t q, r;
//	uint64_t q, r;
	int i;
	
	f=((uint64_t)dv)<<32;

//	puts("A/B ");
//	print_hex(dd);
//	puts(" ");
//	print_hex(dv);
//	puts("\n");

#if 0
	f=dv;
	for(i=0; i<32; i++)
	{
		if(f&0x80000000U)
			break;
		f<<=1;
	}
#endif

//	puts("/F ");
//	print_hex(f);
//	puts("\n");

//	__debugbreak();
	q=0;
	r=dd;
//	for(i=0; i<32; i++)
	for(i=0; i<=32; i++)
//	for(; i>0; i--)
//	for(; i>=0; i--)
	{
//		f>>=1;
		q<<=1;
		if(r>=f)
		{
			r-=f;
			q++;
		}
		f>>=1;
	}

//	__debugbreak();
//	puts("/ ");
//	print_hex(q);
//	puts(" ");
//	print_hex(r);
//	puts("\n");
	
	*rq=q;
	*rr=r;
}

void tkcl_softdiv_ull(uint64_t dd, uint64_t dv,
	uint64_t *rq, uint64_t *rr)
{
	uint64_t f;
	uint64_t q, r;
	int i;
	
	f=dv;
	for(i=0; i<64; i++)
	{
		if(f&0x8000000000000000ULL)
			break;
		f<<=1;
	}

	q=0;
	r=dd;
	for(; i>=0; i--)
	{
		q<<=1;
		if(r>=f)
		{
			r-=f;
			q++;
		}
		f>>=1;
	}
	
	*rq=q;
	*rr=r;
}

void tkcl_softdiv_s(int32_t dd, int32_t dv,
	int32_t *rq, int32_t *rr)
{
	uint32_t udd, udv;
	uint32_t uq, ur;
	int sg;
	
	if(dd<0)
		{ udd=-dd; sg=1; }
	else
		{ udd=dd; sg=0; }
	if(dv<0)
		{ udv=-dv; sg=!sg; }
	else
		{ udv=dv; }
	tkcl_softdiv_u(udd, udv, &uq, &ur);
	
	if(sg)
		{ *rq=-((int32_t)uq); }
	else
		{ *rq=((int32_t)uq); }
	*rr=ur;
}

uint32_t tkcl_div_ui(uint32_t dd, uint32_t dv)
{
	int32_t dq, dr;
	tkcl_softdiv_u(dd, dv, &dq, &dr);
	return(dq);
}

int32_t tkcl_div_si(int32_t dd, int32_t dv)
{
	int32_t dq, dr;
	tkcl_softdiv_s(dd, dv, &dq, &dr);
	return(dq);
}

int32_t tkcl_mod_si(int32_t dd, int32_t dv)
{
	int32_t dq, dr;
	tkcl_softdiv_s(dd, dv, &dq, &dr);
	return(dr);
}
