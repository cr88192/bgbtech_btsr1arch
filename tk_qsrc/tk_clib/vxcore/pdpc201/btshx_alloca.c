void **TK_GetAllocaMark();
extern LVA_VTableBasic *tkmm_lva_clsvt;
void TKMM_LVA_ArrayInit(void);

void *__alloca_begin()
{
	void *p;
	p=*(TK_GetAllocaMark());
	return(p);
}

void __alloca_end(void *mark)
{
	void **rpm;
	void *p, *p1;

	rpm=TK_GetAllocaMark();
	p=*rpm;
	while(p && (p!=mark))
	{
		p1=*(void **)p;
		tk_free(p);
		p=p1;
	}
	*rpm=p;
}

void *__alloca(int size)
{
	void **rpm;
	void *ptr, *ptr1;

	rpm=TK_GetAllocaMark();
	
	ptr=tk_malloc(size+sizeof(void *));
	ptr1=(void *)(((void **)ptr)+1);
	
	*(void **)ptr=*rpm;
	*rpm=ptr;
	return(ptr1);
}

void *__alloca_noframe(int size)
{
	void **rpm;
	void *ptr, *ptr1;

	rpm=TK_GetAllocaMark();
	
	ptr=tk_malloc(size+sizeof(void *));
	if(!ptr)
	{
		tk_printf("alloca failed alloc %d\n", size);
		return(NULL);
	}
	
	ptr1=(void *)(((void **)ptr)+1);
	
	*(void **)ptr=*rpm;
	*rpm=ptr;
	return(ptr1);
}

void *__alloca_initvla1(char *sig, int n1)
{
	LVA_TagArray *arr;
	int sz0, sz, tty;
	
	sz0=TKMM_LVA_TryGetSigSize(sig);
	tty=TKMM_LVA_TryGetSigTty(sig);
	sz=sz0*n1;

	TKMM_LVA_ArrayInit();

//	arr=TKMM_LVA_TyMalloc(tkmm_lvatyi_tagarray, sizeof(LVA_TagArray)+sz);
	arr=__alloca_noframe(sizeof(LVA_TagArray)+sz);
	if(!arr)
	{
		tk_printf("alloca_initvla1: fail n=%d sz0=%d sig=%s\n", n1, sz0, sig);
		return(NULL);
	}
	arr->p.vt=tkmm_lva_clsvt;
	arr->p.dptr=arr->p.data;
	arr->p.size=n1;
	arr->p.tty=tty;
	arr->p.pad=0;
	return(arr);
}

void *__alloca_initvla2(char *sig, int n1, int n2)
{
	LVA_TagArray *arr;
	byte *buf;
	byte **a0;
	int sz0, sz, tty;
	int i, j, k, pos, stp;
	
	sz0=TKMM_LVA_TryGetSigSize(sig);
	tty=TKMM_LVA_TryGetSigTty(sig);
	sz=(sz0*n1*n2)+(n1*sizeof(void *));

	TKMM_LVA_ArrayInit();

//	arr=TKMM_LVA_TyMalloc(tkmm_lvatyi_tagarray, sizeof(LVA_TagArray)+sz);
	arr=__alloca_noframe(sizeof(LVA_TagArray)+sz);
	if(!arr)
		return(NULL);
	arr->p.vt=tkmm_lva_clsvt;
	buf=arr->p.data;
	a0=buf+(sz0*n1*n2);
	arr->p.dptr=a0;
	arr->p.size=n1*n2;
	arr->p.tty=tty;
	arr->p.pad=0;
	
	pos=0; stp=n2*sz0;
	for(i=0; i<n1; i++)
	{
		a0[i]=buf+pos;
		pos+=stp;
	}
	
	return(arr);
}
