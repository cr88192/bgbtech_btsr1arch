#include "tkucc.h"

char *TKUCC_AllocLongLine(TKUCC_MainContext *ctx)
{
	char *ll;
	
	ll=ctx->longline;
	if(ll)
	{
		ctx->longline=*(char **)ll;
		return(ll);
	}
	
	ll=tkucc_malloc(256);
	return(ll);
}

void TKUCC_FreeLongLine(TKUCC_MainContext *ctx, char *text)
{
	*(char **)text=ctx->longline;
	ctx->longline=text;
}

TKUCC_LineBuf *TKUCC_AllocLine(TKUCC_MainContext *ctx)
{
	TKUCC_LineBuf *tmp;
	
	tmp=ctx->free_line;
	if(tmp)
	{
		ctx->free_line=tmp->next;
		tmp->next=NULL;
		return(tmp);
	}
	
	tmp=tkucc_malloc(sizeof(TKUCC_LineBuf));
	tmp->next=NULL;
	return(tmp);
}

void TKUCC_FreeLine(TKUCC_MainContext *ctx, TKUCC_LineBuf *line)
{
	if(line->text!=line->t_buf)
	{
		TKUCC_FreeLongLine(ctx, line->text);
		line->text=NULL;
	}

	line->next=ctx->free_line;
	ctx->free_line=line;
}

TKUCC_LineBuf *TKUCC_AllocLineForString(TKUCC_MainContext *ctx,
	char *str)
{
	TKUCC_LineBuf *tmp;
	
	tmp=TKUCC_AllocLine(ctx);
	if(strlen(str)<TKUCC_LINEBUF_FIXEDLIM)
	{
		tmp->text=tmp->t_buf;
		strcpy(tmp->text, str);
		return(tmp);
	}
	
	tmp->text=TKUCC_AllocLongLine(ctx);
	strcpy(tmp->text, str);
	return(tmp);
}
