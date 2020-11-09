/*

Direct interpreter for FR3A.

 */

int FR3A_RunLoop(FR3A_Context *ctx, int maxlim)
{
	u64 *sp;
	u32 *pc;
	s64 va, vb, vc;
	u64 uva, uvb, uvc;
	s32 iva, ivb, ivc;
	u32 opc;
	int lim;
	int op, dst, srca, srcb;
	
	pc=ctx->pc;
	sp=ctx->sp;
	lim=maxlim;
	while(lim--)
	{
		opc=*pc++;
		op=(byte)opc;
		dst=(byte)(opc>>8);
		srca=(byte)(opc>>16);
		srcb=(byte)(opc>>24);
		
		if(op<0xE0)
		{
			switch(op)
			{
			case 0x00:	sp[dst]=((sbyte *)(sp[srca]))[sp[srcb]];	break;
			case 0x01:	sp[dst]=((s16 *)(sp[srca]))[sp[srcb]];	break;
			case 0x02:	sp[dst]=((s32 *)(sp[srca]))[sp[srcb]];	break;
			case 0x03:	sp[dst]=((u64 *)(sp[srca]))[sp[srcb]];	break;

			case 0x04:	sp[dst]=((byte *)(sp[srca]))[sp[srcb]];	break;
			case 0x05:	sp[dst]=((u16 *)(sp[srca]))[sp[srcb]];	break;
			case 0x06:	sp[dst]=((u32 *)(sp[srca]))[sp[srcb]];	break;

			case 0x07:
				sp[dst+0]=((u64 *)(sp[srca]))[sp[srcb]+0];
				sp[dst+1]=((u64 *)(sp[srca]))[sp[srcb]+1];
				break;

			case 0x08:	((sbyte *)(sp[srca]))[sp[srcb]]=sp[dst];	break;
			case 0x09:	((u16 *)(sp[srca]))[sp[srcb]]=sp[dst];	break;
			case 0x0A:	((u32 *)(sp[srca]))[sp[srcb]]=sp[dst];	break;
			case 0x0B:	((u64 *)(sp[srca]))[sp[srcb]]=sp[dst];	break;
	
			case 0x0C:	sp[dst]=(u64)(((s16 *)(sp[srca]))+sp[srcb]);	break;
			case 0x0D:	sp[dst]=(u64)(((s32 *)(sp[srca]))+sp[srcb]);	break;
			case 0x0E:	sp[dst]=(u64)(((u64 *)(sp[srca]))+sp[srcb]);	break;
			case 0x0F:
				((u64 *)(sp[srca]))[sp[srcb]+0]=sp[dst+0];
				((u64 *)(sp[srca]))[sp[srcb]+1]=sp[dst+1];
				break;

			case 0x10:	sp[dst]=sp[srca]+sp[srcb];		break;
			case 0x11:	sp[dst]=sp[srca]-sp[srcb];		break;
			case 0x12:	sp[dst]=sp[srca]*sp[srcb];		break;
			case 0x13:	sp[dst]=sp[srca]/sp[srcb];		break;
			case 0x14:	sp[dst]=sp[srca]%sp[srcb];		break;
			case 0x15:	sp[dst]=sp[srca]&sp[srcb];		break;
			case 0x16:	sp[dst]=sp[srca]|sp[srcb];		break;
			case 0x17:	sp[dst]=sp[srca]^sp[srcb];		break;

			case 0x18:
				sp[dst]=(s32)(sp[srca]+sp[srcb]);
				break;
			case 0x19:
				sp[dst]=(s32)(sp[srca]-sp[srcb]);
				break;
			case 0x1A:
				sp[dst]=(s32)(((s32)(sp[srca]))*((s32)(sp[srcb])));
				break;
			case 0x1B:
				sp[dst]=(s32)(((s32)(sp[srca]))/((s32)(sp[srcb])));
				break;
			case 0x1C:
				sp[dst]=(u32)(sp[srca]+sp[srcb]);
				break;
			case 0x1D:
				sp[dst]=(u32)(sp[srca]-sp[srcb]);
				break;
			case 0x1E:
				sp[dst]=(u32)(((u32)(sp[srca]))*((u32)(sp[srcb])));
				break;
			case 0x1F:
				sp[dst]=(u32)(((u32)(sp[srca]))/((u32)(sp[srcb])));
				break;

			case 0x20:
				((double *)sp)[dst]=
					((double *)sp)[srca] +
					((double *)sp)[srcb];
				break;
			case 0x21:
				((double *)sp)[dst]=
					((double *)sp)[srca] -
					((double *)sp)[srcb];
				break;
			case 0x22:
				((double *)sp)[dst]=
					((double *)sp)[srca] *
					((double *)sp)[srcb];
				break;
			case 0x23:
				((double *)sp)[dst]=
					((double *)sp)[srca] /
					((double *)sp)[srcb];
				break;

			case 0x24:
#ifdef __BJX2__
				sp[dst]=(((s64 *)sp)[srca])<<((int)(sp[srcb]));
#else
				va=(((s64 *)sp)[srca]);
				ivb=((int)(sp[srcb]));
				if(ivb>=0)
					vc=va<<ivb;
				else
					vc=va>>(-ivb);
				sp[dst]=vc;
#endif
				break;

			case 0x25:
#ifdef __BJX2__
				sp[dst]=(((u64 *)sp)[srca])<<((int)(sp[srcb]));
#else
				uva=(((s64 *)sp)[srca]);
				ivb=((int)(sp[srcb]));
				if(ivb>=0)
					uvc=uva<<ivb;
				else
					uvc=uva>>(-ivb);
				sp[dst]=uvc;
#endif
				break;


			case 0x26:
#ifdef __BJX2__
				sp[dst]=(((s32 *)sp)[srca])<<((int)(sp[srcb]));
#else
				iva=(((s32 *)sp)[srca]);
				ivb=((int)(sp[srcb]));
				if(ivb>=0)
					ivc=iva<<ivb;
				else
					ivc=iva>>(-ivb);
				sp[dst]=ivc;
#endif
				break;

			case 0x27:
#ifdef __BJX2__
				sp[dst]=(((u32 *)sp)[srca])<<((int)(sp[srcb]));
#else
				uiva=(((s64 *)sp)[srca]);
				ivb=((int)(sp[srcb]));
				if(ivb>=0)
					uivc=uiva<<ivb;
				else
					uivc=uiva>>(-ivb);
				sp[dst]=uivc;
#endif
				break;


			default:
				break;
			}
		}
	}
}
