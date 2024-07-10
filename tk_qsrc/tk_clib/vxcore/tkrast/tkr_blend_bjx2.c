#ifdef __BJX2__
// #if 1

u64 TKRA_Blend_Zero_Zero(u64 sclr, u64 dclr);
u64 TKRA_Blend_Zero_One(u64 sclr, u64 dclr);

u64 TKRA_Blend_One_Zero(u64 sclr, u64 dclr);
u64 TKRA_Blend_One_One(u64 sclr, u64 dclr);

u64 TKRA_Blend_SrcAlpha_OneMinusSrcAlpha(u64 sval, u64 dval);

__asm {

.global TKRA_Blend_Zero_Zero
TKRA_Blend_Zero_Zero:
	MOV 0, R2
	RTS

.global TKRA_Blend_Zero_One
TKRA_Blend_Zero_One:
	MOV R5, R2
	RTS

.global TKRA_Blend_One_Zero
TKRA_Blend_One_Zero:
	MOV R4, R2
	RTS

.global TKRA_Blend_One_One
TKRA_Blend_One_One:
	BRA tkra_padduhw_sat

TKRA_Blend_SrcAlpha_OneMinusSrcAlpha:
	PSHUF.W		R4, 0xFF, R16
	NOT			R16, R17
	PMULU.HW	R4, R16, R18
	PMULU.HW	R5, R17, R19
	PADD.W		R18, R19, R2
	RTS
};

#endif
