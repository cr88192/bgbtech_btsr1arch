#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bgbccc.h>


BGBCC_Object *BGBCC_NewContext()
{
	BGBCC_Object *tmp;

	tmp=(BGBCC_Object *)malloc(sizeof(BGBCC_Object));
	memset(tmp, 0, sizeof(BGBCC_Object));

	tmp->label_name=(char **)malloc(256*sizeof(char *));
	tmp->label_pos=(int *)malloc(256*sizeof(int));

	tmp->goto_name=(char **)malloc(256*sizeof(char *));
	tmp->goto_pos=(int *)malloc(256*sizeof(int));
	tmp->goto_type=(byte *)malloc(256*sizeof(byte));

//	tmp->const_name=(char **)malloc(256*sizeof(char *));
//	tmp->const_value=(long long *)malloc(256*sizeof(long long));

	tmp->proxy_name=(char **)malloc(256*sizeof(char *));

	tmp->m_labels=256;
	tmp->m_gotos=256;
//	tmp->m_const=256;
	tmp->m_proxy=256;

//	tmp->llbl_name=(char **)malloc(256*sizeof(char *));
//	tmp->llbl_pos=(int *)malloc(256*sizeof(int));

	return(tmp);
}

int BGBCC_DestroyContext(BGBCC_Object *ctx)
{
	free(ctx->label_name);
	free(ctx->label_pos);

	free(ctx->goto_name);
	free(ctx->goto_pos);
	free(ctx->goto_type);

//	free(ctx->const_name);
//	free(ctx->const_value);

	free(ctx->proxy_name);

	free(ctx);

	return(0);
}

void BGBCC_EmitLabelPos(BGBCC_Object *ctx, char *name, int pos)
{
	int i, j;

//	printf("Emit Label %s @ %p\n", name, ctx->ip);

	i=ctx->n_labels++;
	if(i>=ctx->m_labels)
	{
		j=ctx->m_labels+(ctx->m_labels>>1);
		ctx->label_name=(char **)
			realloc(ctx->label_name, j*sizeof(char *));
		ctx->label_pos=(int *)realloc(ctx->label_pos, j*sizeof(int));

//		ctx->llbl_name=(char **)
//			realloc(ctx->llbl_name, j*sizeof(char *));
//		ctx->llbl_pos=(int *)realloc(ctx->llbl_pos, j*sizeof(int));

		ctx->m_labels=j;
	}

	ctx->label_name[i]=bgbcc_strdup(name);
	ctx->label_pos[i]=pos;
}

void BGBCC_EmitGotoPos(BGBCC_Object *ctx, char *name, int ty, int pos)
{
	int i, j;

//	printf("Emit Goto %s\n", name);

	i=ctx->n_gotos++;
	if(i>=ctx->m_gotos)
	{
		j=ctx->m_gotos+(ctx->m_gotos>>1);
		ctx->goto_name=(char **)realloc(ctx->goto_name, j*sizeof(char *));
		ctx->goto_pos=(int *)realloc(ctx->goto_pos, j*sizeof(int));
		ctx->goto_type=(byte *)realloc(ctx->goto_type, j);
		ctx->m_gotos=j;
	}

	ctx->goto_name[i]=name?bgbcc_strdup(name):NULL;
	ctx->goto_pos[i]=pos;
	ctx->goto_type[i]=ty;
}

#if 0
// void BGBCC_EmitLabel(BGBCC_Object *ctx, char *name)
{
	int i;

	if(!name)return;

	if(ctx->fl&4)i=(2<<24)+(ctx->dp-ctx->data);
		else i=(1<<24)+(ctx->ip-ctx->text);
	BGBCC_EmitLabelPos(ctx, name, i);
}

// void BGBCC_EmitGoto(BGBCC_Object *ctx, char *name, int ty)
{
	int i;

	if(!name)return;

	if(ctx->fl&4)i=(2<<24)+(ctx->dp-ctx->data);
		else i=(1<<24)+(ctx->ip-ctx->text);
	BGBCC_EmitGotoPos(ctx, name, ty, i);
}
#endif

#if 0
// void BGBCC_EmitConst(BGBCC_Object *ctx, char *name, long long val)
{
	int i, j;

	if(!name)return;

	i=ctx->n_const++;
	if(i>=ctx->m_const)
	{
		j=ctx->m_const+(ctx->m_const>>1);
		ctx->const_name=
			(char **)realloc(ctx->const_name, j*sizeof(char *));
		ctx->const_value=
			(long long *)realloc(ctx->const_value,
				j*sizeof(long long));
		ctx->m_const=j;
	}

	ctx->const_name[i]=bgbcc_strdup(name);
	ctx->const_value[i]=val;
}
#endif
