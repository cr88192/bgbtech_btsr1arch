uint64_t __lli_shr(uint64_t v, int sh)
{
	uint32_t hi, lo, t;
	uint64_t v;
	
	if(sh>=32)
	{
		lo=v>>32;
		lo=lo>>(sh-32);
		return(lo);
	}
	
	hi=v>>32;
	lo=v;
	
	t=hi<<(32-sh);
	hi>>=sh;
	lo>>=sh;
	lo|=t;
	
	v=(((uint64_t)hi)<<32)|lo;
	return(v);
}
