/*
Daisy Chain off of SHCC backend.
 */
 
 
#define BGBCC_BSR_GenLabelTemp(ctx)		\
	BGBCC_BSR_GenLabelTempLLn(ctx, __FILE__, __LINE__)
#define BGBCC_BSR_GenLabel(ctx)		\
	BGBCC_BSR_GenLabelLLn(ctx, __FILE__, __LINE__)

#define BGBCC_BSR_GETLREG_LO(reg)		(BGBCC_SH_REG_R0+((reg)&31))
#define BGBCC_BSR_GETLREG_HI(reg)		(BGBCC_SH_REG_R0+(((reg)&31)+1))


typedef struct BGBCC_CMG_Context_s BGBCC_BSR_Context;
typedef struct BGBCC_CMG_OpcodeArg_s BGBCC_BSR_OpcodeArg;
typedef struct BGBCC_CMG_VarSpan_s BGBCC_BSR_VarSpan;
typedef struct BGBCC_CMG_VarSpan2_s BGBCC_BSR_VarSpan2;

typedef struct BGBCC_CMG_EmitQueueOp_s BGBCC_BSR_EmitQueueOp;



 
#define BGBCC_JX2_GenLabelTemp(ctx)		\
	BGBCC_JX2_GenLabelTempLLn(ctx, __FILE__, __LINE__)
#define BGBCC_JX2_GenLabel(ctx)		\
	BGBCC_JX2_GenLabelLLn(ctx, __FILE__, __LINE__)

#define BGBCC_JX2_GETLREG_LO(reg)		(BGBCC_SH_REG_R0+((reg)&31))
#define BGBCC_JX2_GETLREG_HI(reg)		(BGBCC_SH_REG_R0+(((reg)&31)+1))


typedef struct BGBCC_CMG_Context_s BGBCC_JX2_Context;
typedef struct BGBCC_CMG_OpcodeArg_s BGBCC_JX2_OpcodeArg;
typedef struct BGBCC_CMG_VarSpan_s BGBCC_JX2_VarSpan;
typedef struct BGBCC_CMG_VarSpan2_s BGBCC_JX2_VarSpan2;

typedef struct BGBCC_CMG_EmitQueueOp_s BGBCC_JX2_EmitQueueOp;
