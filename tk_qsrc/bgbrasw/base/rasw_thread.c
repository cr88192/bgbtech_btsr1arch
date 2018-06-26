#include <bgbrasw.h>

#include <windows.h>

DWORD WINAPI bgbrasw_ThreadProc(LPVOID lpParameter)
{
	BGBRASW_Context *ctx, *pctx;
	BGBRASW_Primitive *prim;
	int sleeptick;

	sleeptick=1024;
	ctx=lpParameter;
	pctx=ctx->pctx;
	while(1)
	{
		BGBRASW_LockContext(ctx);
//		if(ctx->pridraw)
		if(ctx->pridraw)
		{
			if((ctx->pridraw->seq>=0) &&
				(ctx->pridraw->seq>(pctx->lpriseq+1)))
			{
				BGBRASW_UnlockContext(ctx);
				continue;
			}
			
			prim=ctx->pridraw;
			ctx->pridraw=prim->next;
			ctx->status|=1;		//busy
		}else
		{
			prim=NULL;
		}
		BGBRASW_UnlockContext(ctx);
		
		if(prim)
		{
			ctx->tgt_rgba=ctx->pctx->tgt_rgba;
			ctx->tgt_zbuf=ctx->pctx->tgt_zbuf;
			ctx->tgt_xs=ctx->pctx->tgt_xs;
			ctx->tgt_ys=ctx->pctx->tgt_ys;
			ctx->tgt_lxs=ctx->pctx->tgt_lxs;
			ctx->tgt_lys=ctx->pctx->tgt_lys;

#ifdef BGBRASW_SS_YUVA
			ctx->tgt_ssyuva=ctx->pctx->tgt_ssyuva;
			ctx->tgt_sszbuf=ctx->pctx->tgt_sszbuf;
#endif
		
			sleeptick=1024;
			BGBRASW_DrawPrimitive(ctx, prim);
			if(prim->seq>pctx->lpriseq)
				pctx->lpriseq=prim->seq;

			BGBRASW_LockContext(ctx);
			prim->tabs->refcount--;
			if(!prim->tabs->refcount)
			{
				BGBRASW_FreeTestBlendData(ctx, prim->tabs);
				prim->tabs=NULL;
			}
			BGBRASW_FreePrimitive(ctx, prim);

			ctx->status&=~1;	//done working
			BGBRASW_UnlockContext(ctx);

//			BGBRASW_DrawPrimitiveList(ctx, pridraw);
//			BGBRASW_FreePrimitiveList(ctx, pridraw);
			continue;
		}

		if(sleeptick>0)
		{
			sleeptick--;
			Sleep(0);
		}
		Sleep(1);
	}
}


BGBRASW_API BGBRASW_Context *BGBRASW_AllocChildContext(
	BGBRASW_Context *parent)
{
	BGBRASW_Context *tmp;
	
	tmp=bgbrasw_malloc(sizeof(BGBRASW_Context));
	tmp->pctx=parent;
	tmp->pnext=parent->child;
	parent->child=tmp;
	
	return(tmp);
}

BGBRASW_API BGBRASW_Context *BGBRASW_CreateChildThreadContext(
	BGBRASW_Context *parent)
{
	CRITICAL_SECTION *lock;
	BGBRASW_Context *tmp;
	
	tmp=BGBRASW_AllocChildContext(parent);

	lock=bgbrasw_malloc(sizeof(CRITICAL_SECTION));
	InitializeCriticalSectionAndSpinCount(lock, 0x80000400);

	tmp->th_lock=lock;
	tmp->thread=CreateThread(
		NULL, 1<<20,
		bgbrasw_ThreadProc,
		tmp,
		0, NULL);

	return(tmp);
}

BGBRASW_API void BGBRASW_LockContext(BGBRASW_Context *ctx)
{
	EnterCriticalSection(ctx->th_lock);
}

BGBRASW_API void BGBRASW_UnlockContext(BGBRASW_Context *ctx)
{
	LeaveCriticalSection(ctx->th_lock);
}


BGBRASW_API void BGBRASW_Finish(BGBRASW_Context *ctx)
{
	BGBRASW_Context *cur;
	int ws;
	
	while(1)
	{
		cur=ctx->child; ws=0;
		while(cur)
		{
			BGBRASW_LockContext(cur);
			if(cur->pridraw || (cur->status&1))ws=1;
			BGBRASW_UnlockContext(cur);
			cur=cur->pnext;
		}
		
		if(!ws)break;
		Sleep(0);
	}
	
	ctx->priseq=0;
	ctx->lpriseq=0;
}

BGBRASW_API void BGBRASW_Flush(BGBRASW_Context *ctx)
{
	BGBRASW_Context *cur;
	int ws;

	BGBRASW_Finish(ctx);

#if 0
	cur=ctx->child;
	while(cur)
	{
		BGBRASW_LockContext(cur);
		if(cur->pridraw)
		{
			BGBRASW_FreePrimitiveList(cur, cur->pridraw);
			cur->pridraw=NULL;
		}
		BGBRASW_UnlockContext(cur);
		cur=cur->pnext;
	}
#endif
}
