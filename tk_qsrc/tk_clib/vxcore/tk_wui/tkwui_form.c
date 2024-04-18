TKWUI_ProgramContext *tkwui_globalctx;

TKWUI_ProgramContext *TKWUI_GetCurrentProgramContext()
{
	if(tkwui_globalctx)
		return(tkwui_globalctx);
	tkwui_globalctx=tk_malloc(sizeof(TKWUI_ProgramContext));
	memset(tkwui_globalctx, 0, sizeof(TKWUI_ProgramContext));
	return(tkwui_globalctx);
}

TKWUI_FormLayout *TKWUI_ProgramLookupLayout(
	TKWUI_ProgramContext *ctx,
	char *name)
{
	TKWUI_FormLayout *cur;
	
	cur=ctx->firstlayout;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

TKWUI_FormLayout *TKWUI_ProgramGetLayout(
	TKWUI_ProgramContext *ctx,
	char *name)
{
	TKWUI_FormLayout *cur;
	
	cur=TKWUI_ProgramLookupLayout(ctx, name);
	if(cur)
		return(cur);

	cur=tk_malloc(sizeof(TKWUI_FormLayout));
	memset(cur, 0, sizeof(TKWUI_FormLayout));
	
	cur->name=tk_strdupi(name);
	cur->next=ctx->firstlayout;
	ctx->firstlayout=cur;

	return(cur);
}

TKWUI_WindowContext *TKWUI_ProgramLookupWindow(
	TKWUI_ProgramContext *ctx,
	char *name)
{
	TKWUI_WindowContext *cur;
	
	cur=ctx->firstwindow;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

TKWUI_WindowContext *TKWUI_ProgramGetWindow(
	TKWUI_ProgramContext *ctx,
	char *name)
{
	TKWUI_WindowContext *cur;

	cur=TKWUI_ProgramLookupWindow(ctx, name);
	if(cur)
		return(cur);

	cur=tk_malloc(sizeof(TKWUI_WindowContext));
	memset(cur, 0, sizeof(TKWUI_WindowContext));
	
	cur->name=tk_strdupi(name);
	cur->next=ctx->firstwindow;
	ctx->firstwindow=cur;

	return(cur);
}

int TKWUI_WindowSetLayout(
	TKWUI_WindowContext *ctx,
	char *name)
{
	TKWUI_FormLayout *lo;
	
	lo=TKWUI_ProgramLookupLayout(ctx->pgm, name);
	if(!lo)
		return(-1);
	
	ctx->activeform=lo;
	return(0);
}

TKWUI_WidgetState *TKWUI_FormStateLookupStateId(
	TKWUI_FormState *fsctx,
	int data_id)
{
	TKWUI_WidgetState *wscur;
	wscur=fsctx->state;
	while(wscur)
	{
		if(wscur->data_id==data_id)
			return(wscur);
		wscur=wscur->next;
	}
	return(NULL);
}

TKWUI_WidgetState *TKWUI_FormStateGetStateId(
	TKWUI_FormState *fsctx,
	int data_id)
{
	TKWUI_WidgetState *wscur;
	
	wscur=TKWUI_FormStateLookupStateId(fsctx, data_id);
	if(wscur)
		return(wscur);

	wscur=tk_malloc(sizeof(TKWUI_WidgetState));
	memset(wscur, 0, sizeof(TKWUI_WidgetState));
	
	wscur->data_id=data_id;
	wscur->next=fsctx->state;
	fsctx->state=wscur;
	return(wscur);
}

TKWUI_WidgetState *TKWUI_WindowLookupStateId(
	TKWUI_WindowContext *wctx,
	int data_id)
{
	TKWUI_WidgetState *wscur;

	if(wctx->formstate)
	{
		wscur=TKWUI_FormStateLookupStateId(wctx->formstate, data_id);
		if(wscur)
			return(wscur);
	}

	if(wctx->pgm->formstate)
	{
		wscur=TKWUI_FormStateLookupStateId(wctx->pgm->formstate, data_id);
		if(wscur)
			return(wscur);
	}

	return(NULL);
}

int TKWUI_WindowRedraw(
	TKWUI_WindowContext *wctx)
{
	TKWUI_WidgetInfo *wcur;
	
	wcur=wctx->activeform->widget;
	while(wcur)
	{
		if(	wcur->vt &&
			wcur->vt->Draw)
		{
			wcur->vt->Draw(wcur, wctx);
		}
		wcur=wcur->next;
	}
	return(0);
}

int TKWUI_WindowClickEvent(
	TKWUI_WindowContext *wctx, int relx, int rely, int flags)
{
	TKWUI_WidgetInfo *wcur;
	
	wcur=wctx->activeform->widget;
	while(wcur)
	{
		if(	wcur->vt &&
			wcur->vt->Click &&
			(relx>=wcur->org_x) &&
			(relx>=wcur->org_y) &&
			(relx<(wcur->org_x+wcur->size_x)) &&
			(rely<(wcur->org_y+wcur->size_y)))
		{
			wcur->vt->Click(wcur, wctx, relx, rely, flags);
		}
		wcur=wcur->next;
	}
	return(0);
}
