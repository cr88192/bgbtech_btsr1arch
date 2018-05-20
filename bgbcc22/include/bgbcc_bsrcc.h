/*
Daisy Chain off of SHCC backend.
 */
 
 
#define BGBCC_BSR_GenLabelTemp(ctx)		\
	BGBCC_BSR_GenLabelTempLLn(ctx, __FILE__, __LINE__)
#define BGBCC_BSR_GenLabel(ctx)		\
	BGBCC_BSR_GenLabelLLn(ctx, __FILE__, __LINE__)

#define BGBCC_BSR_GETLREG_LO(reg)		(BGBCC_SH_REG_R0+((reg)&31))
#define BGBCC_BSR_GETLREG_HI(reg)		(BGBCC_SH_REG_R0+(((reg)&31)+1))


typedef struct BGBCC_SHX_Context_s BGBCC_BSR_Context;
typedef struct BGBCC_SHX_OpcodeArg_s BGBCC_BSR_OpcodeArg;
typedef struct BGBCC_SHX_VarSpan_s BGBCC_BSR_VarSpan;
typedef struct BGBCC_SHX_VarSpan2_s BGBCC_BSR_VarSpan2;

typedef struct BGBCC_SHX_EmitQueueOp_s BGBCC_BSR_EmitQueueOp;
