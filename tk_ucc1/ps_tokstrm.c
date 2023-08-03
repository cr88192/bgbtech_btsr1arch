/*
 * Parser token stream.
 * Pulls lines from preprocessor if needed, maintain a logical stream of tokens.
 * State only applies for a single translation unit.
 */

int TKUCC_TokStrmGetMark(TKUCC_MainContext *ctx)
{
	return(ctx->tokstrm_idx);
}

int TKUCC_TokStrmSetMark(TKUCC_MainContext *ctx, int idx)
{
	ctx->tokstrm_idx=idx;
	return(0);
}

int TKUCC_TokStrmAddTokenEnd(TKUCC_MainContext *ctx, char *tok)
{
	int strix, tkidx;
	
	strix=TKUCC_InternTokenToIndex(ctx, tok, TKUCC_ZID_TOKSTRM);
	
	tkidx=ctx->tokstrm_max++;
	if(!ctx->tokstrm_span[tkidx>>12])
	{
		ctx->tokstrm_span[tkidx>>12]=tkucc_malloc(4096*sizeof(int));
	}
	
	ctx->tokstrm_span[tkidx>>12][tkidx&4095]=strix;
	return(tkidx);
}

char *TKUCC_TokStrmGetToken(TKUCC_MainContext *ctx, int idx)
{
	TKUCC_LineBuf *line;
	char **a;
	int i;

	while(idx>=ctx->tokstrm_max)
	{
		line=TKUCC_PpGetProcessedLines(ctx);
		if(!line)
			break;
		a=TKUCC_TokSplitLine(line->text);
		for(i=0; a[i]; i++)
			TKUCC_TokStrmAddTokenEnd(ctx, a[i]);
		TKUCC_FreeLine(ctx, line);
	}

	if(idx>=ctx->tokstrm_max)
		return(NULL);
		
	i=ctx->tokstrm_span[idx>>12][idx&4095];
	return(TKUCC_InternIndexToString(ctx, i));
}

char *TKUCC_TokStrmPeekToken(TKUCC_MainContext *ctx, int relidx)
{
	int idx;
	idx=ctx->tokstrm_idx+relidx;
	return(TKUCC_TokStrmGetToken(ctx, idx));
}

void TKUCC_TokStrmStepToken(TKUCC_MainContext *ctx, int relidx)
{
	ctx->tokstrm_idx+=relidx;
}

char *TKUCC_TokStrmNextToken(TKUCC_MainContext *ctx)
{
	char *str;
	str=TKUCC_TokStrmPeekToken(ctx, 0);
	TKUCC_TokStrmStepToken(ctx, 1);
	return(str);
}
