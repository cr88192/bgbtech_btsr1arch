tkucc_valtype TKUCC_TypeFromSig(TKUCC_MainContext *ctx, char *sig)
{
	tkucc_valtype ty;
	int asz[8];
	char *s;
	int ind, bty, an;
	int i, j, k;
	
	s=sig; ind=0;
	bty=-1; an=0;
	
	if((*s=='A') && (s[1]>='0') && (s[1]<='9'))
	{
		k=0; s++;
		while(*s)
		{
			if((*s>='0') && (*s<='9'))
			{
				k=(k*10)+((*s++)-'0');
				continue;
			}
			if(*s==',')
			{
				asz[an++]=k;
				k=0;
				s++;
				continue;
			}
			break;
		}
		if(*s==';')s++;
		asz[an++]=k;
	}

	while(*s=='P')
		{ ind++; s++; }

	if((*s>='a') && (*s<='z'))
	{
		switch(*s)
		{
			case 'a': bty=TKUCC_VALTYPE_TY_SB;		break;
			case 'b': bty=TKUCC_VALTYPE_TY_UB;		break;
			case 'c': bty=TKUCC_VALTYPE_TY_SB;		break;
			case 'd': bty=TKUCC_VALTYPE_TY_D;		break;
			case 'e': bty=TKUCC_VALTYPE_TY_F128;	break;
			case 'f': bty=TKUCC_VALTYPE_TY_F;		break;
			case 'g': bty=TKUCC_VALTYPE_TY_F128;	break;
			case 'h': bty=TKUCC_VALTYPE_TY_UB;		break;
			case 'i': bty=TKUCC_VALTYPE_TY_SI;		break;
			case 'j': bty=TKUCC_VALTYPE_TY_UI;		break;
			case 'k': bty=TKUCC_VALTYPE_TY_F16;		break;
			case 'l': bty=TKUCC_VALTYPE_TY_SNL;		break;
			case 'm': bty=TKUCC_VALTYPE_TY_UNL;		break;
			case 'n': bty=TKUCC_VALTYPE_TY_SI128;	break;
			case 'o': bty=TKUCC_VALTYPE_TY_UI128;	break;
			case 'p': bty=TKUCC_VALTYPE_TY_SNL;		break;
			case 'q': bty=TKUCC_VALTYPE_TY_UNL;		break;
			case 'r': bty=TKUCC_VALTYPE_TY_VAR;		break;
			case 's': bty=TKUCC_VALTYPE_TY_SS;		break;
			case 't': bty=TKUCC_VALTYPE_TY_US;		break;
			case 'u': bty=TKUCC_VALTYPE_TY_BF16;	break;
			case 'v': bty=TKUCC_VALTYPE_TY_VOID;	break;
			case 'w': bty=TKUCC_VALTYPE_TY_US;		break;
			case 'x': bty=TKUCC_VALTYPE_TY_SL;		break;
			case 'y': bty=TKUCC_VALTYPE_TY_UL;		break;
			case 'z': bty=TKUCC_VALTYPE_TY_VARARG;	break;
		}
		
		if(an==0)
		{
			ty.val=bty|(ind<<12);
			return(ty);
		}

		if((an==1) && (asz[0]<4095))
		{
			ty.val=bty|(ind<<12)|(asz[0]<<16);
			return(ty);
		}
	}

	ty.val=0;
	return(ty);
}

char *TKUCC_SigFromType(TKUCC_MainContext *ctx, tkucc_valtype ty)
{
	char tb[256];
	char *t;
	int asz, ind, bty;
	int i;
	
	if(((ty.val>>28)&15)==0)
	{
		asz=(ty.val>>16)&4095;
		ind=(ty.val>>12)&   7;
		bty=(ty.val>> 0)&4095;
		
		t=tb;
		if(asz)
		{
			sprintf(t, "A%u", asz);
			t+=strlen(t);
		}
		
		i=ind;
		while(i)
		{
			*t++='P';
		}
		
		if(bty<0x100)
		{
			switch(bty)
			{
			case TKUCC_VALTYPE_TY_SB:		*t++='a'; break;
			case TKUCC_VALTYPE_TY_UB:		*t++='h'; break;
			case TKUCC_VALTYPE_TY_SS:		*t++='s'; break;
			case TKUCC_VALTYPE_TY_US:		*t++='t'; break;
			case TKUCC_VALTYPE_TY_SI:		*t++='i'; break;
			case TKUCC_VALTYPE_TY_UI:		*t++='j'; break;
			case TKUCC_VALTYPE_TY_SL:		*t++='x'; break;
			case TKUCC_VALTYPE_TY_UL:		*t++='y'; break;
			case TKUCC_VALTYPE_TY_SNL:		*t++='l'; break;
			case TKUCC_VALTYPE_TY_UNL:		*t++='m'; break;
			case TKUCC_VALTYPE_TY_F:		*t++='f'; break;
			case TKUCC_VALTYPE_TY_D:		*t++='d'; break;
			case TKUCC_VALTYPE_TY_F16:		*t++='k'; break;
			case TKUCC_VALTYPE_TY_F128:		*t++='g'; break;
			case TKUCC_VALTYPE_TY_SI128:	*t++='n'; break;
			case TKUCC_VALTYPE_TY_UI128:	*t++='o'; break;
			case TKUCC_VALTYPE_TY_VAR:		*t++='r'; break;
			case TKUCC_VALTYPE_TY_VOID:		*t++='v'; break;
			case TKUCC_VALTYPE_TY_BF16:		*t++='u'; break;
			case TKUCC_VALTYPE_TY_VARARG:	*t++='z'; break;
			}

			*t=0;
			return(tkucc_rstrdup(tb));
		}
		
	}
	
	return(NULL);
}

int TKUCC_TypeUnsignedP(TKUCC_MainContext *ctx, tkucc_valtype ty)
{
	if(ty.val==TKUCC_VALTYPE_TY_UB)
		return(1);
	if(ty.val==TKUCC_VALTYPE_TY_US)
		return(1);
	if(ty.val==TKUCC_VALTYPE_TY_UI)
		return(1);
	if(ty.val==TKUCC_VALTYPE_TY_UNL)
		return(1);
	if(ty.val==TKUCC_VALTYPE_TY_UL)
		return(1);
	if(ty.val==TKUCC_VALTYPE_TY_UI128)
		return(1);

	return(0);
}

int TKUCC_TypeSmallSIntP(TKUCC_MainContext *ctx, tkucc_valtype ty)
{
	return(
		(ty.val>=TKUCC_VALTYPE_TY_SB) &&
		(ty.val<=TKUCC_VALTYPE_TY_SI));
}

int TKUCC_TypeSmallUIntP(TKUCC_MainContext *ctx, tkucc_valtype ty)
{
	return(
		(ty.val>=TKUCC_VALTYPE_TY_SB) &&
		(ty.val<=TKUCC_VALTYPE_TY_UI) &&
		TKUCC_TypeUnsignedP(ctx, ty));
}

int TKUCC_TypeSmallIntP(TKUCC_MainContext *ctx, tkucc_valtype ty)
{
	return(
		(ty.val>=TKUCC_VALTYPE_TY_SB) &&
		(ty.val<=TKUCC_VALTYPE_TY_UI));
}

int TKUCC_TypeSmallLongP(TKUCC_MainContext *ctx, tkucc_valtype ty)
{
	return(
		(ty.val>=TKUCC_VALTYPE_TY_SB) &&
		(ty.val<=TKUCC_VALTYPE_TY_UL));
}

int TKUCC_TypeSmallInt128P(TKUCC_MainContext *ctx, tkucc_valtype ty)
{
	return(
		(ty.val>=TKUCC_VALTYPE_TY_SB) &&
		(ty.val<=TKUCC_VALTYPE_TY_UI128));
}

int TKUCC_TypeSmallRealP(TKUCC_MainContext *ctx, tkucc_valtype ty)
{
	return(
		(ty.val>=TKUCC_VALTYPE_TY_BF16) &&
		(ty.val<=TKUCC_VALTYPE_TY_D));
}

int TKUCC_TypeSmallDoubleP(TKUCC_MainContext *ctx, tkucc_valtype ty)
{
	if(TKUCC_TypeSmallRealP(ctx, ty))
		return(1);
	if(TKUCC_TypeSmallLongP(ctx, ty))
		return(1);
	return(0);
}

int TKUCC_TypePointerP(TKUCC_MainContext *ctx, tkucc_valtype ty)
{
	if(ty.val&TKUCC_VALTYPE_PTR_MSK)
		return(1);
	return(0);
}


tkucc_valtype TKUCC_TypeFromShort24Type(TKUCC_MainContext *ctx, int sty)
{
	tkucc_valtype ty;
	if((sty&0xF00000)==0x000000)
	{
		ty.val=sty&0x0FFFFF;
		return(ty);
	}
	
	ty.val=0;
	return(ty);
}

int TKUCC_TypeToShort24Type(TKUCC_MainContext *ctx, tkucc_valtype ty)
{
	if((ty.val&0x0FFFFF)==ty.val)
		return(ty.val&0x0FFFFF);
	return(0);
}

tkucc_valtype TKUCC_TypeAutoPromoteBinary(TKUCC_MainContext *ctx,
	tkucc_valtype lty, tkucc_valtype rty)
{
	tkucc_valtype dty;

	if(TKUCC_TypeSmallIntP(ctx, lty) && TKUCC_TypeSmallIntP(ctx, rty))
	{
		dty.val=TKUCC_VALTYPE_TY_SI;
		if(lty.val==TKUCC_VALTYPE_TY_UI)
			dty.val=TKUCC_VALTYPE_TY_UI;
		if(rty.val==TKUCC_VALTYPE_TY_UI)
			dty.val=TKUCC_VALTYPE_TY_UI;
		return(dty);
	}

	if(TKUCC_TypeSmallLongP(ctx, lty) && TKUCC_TypeSmallLongP(ctx, rty))
	{
		dty.val=TKUCC_VALTYPE_TY_SL;
		if(lty.val==TKUCC_VALTYPE_TY_UL)
			dty.val=TKUCC_VALTYPE_TY_UL;
		if(rty.val==TKUCC_VALTYPE_TY_UL)
			dty.val=TKUCC_VALTYPE_TY_UL;
		return(dty);
	}

	if(TKUCC_TypeSmallInt128P(ctx, lty) && TKUCC_TypeSmallInt128P(ctx, rty))
	{
		dty.val=TKUCC_VALTYPE_TY_SI128;
		if(lty.val==TKUCC_VALTYPE_TY_UI128)
			dty.val=TKUCC_VALTYPE_TY_UI128;
		if(rty.val==TKUCC_VALTYPE_TY_UI128)
			dty.val=TKUCC_VALTYPE_TY_UI128;
		return(dty);
	}

	if(TKUCC_TypeSmallDoubleP(ctx, lty) && TKUCC_TypeSmallDoubleP(ctx, rty))
	{
		dty.val=TKUCC_VALTYPE_TY_D;
		return(dty);
	}

	dty.val=0;
	return(dty);
}
