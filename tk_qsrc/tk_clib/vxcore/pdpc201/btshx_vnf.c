__fcomplex	__vnf_c2f_add(__fcomplex a, __fcomplex b);
__fcomplex	__vnf_c2f_sub(__fcomplex a, __fcomplex b);
__fcomplex	__vnf_c2f_mul(__fcomplex a, __fcomplex b);
__fcomplex	__vnf_c2f_div(__fcomplex a, __fcomplex b);

__dcomplex	__vnf_c2d_add(__dcomplex a, __dcomplex b);
__dcomplex	__vnf_c2d_sub(__dcomplex a, __dcomplex b);
__dcomplex	__vnf_c2d_mul(__dcomplex a, __dcomplex b);
__dcomplex	__vnf_c2d_div(__dcomplex a, __dcomplex b);

__vec2f		__vnf_v2f_add(__vec2f a, __vec2f b);
__vec2f		__vnf_v2f_sub(__vec2f a, __vec2f b);
__vec2f		__vnf_v2f_mul(__vec2f a, __vec2f b);
__vec2f		__vnf_v2f_div(__vec2f a, __vec2f b);
float		__vnf_v2f_dot(__vec2f a, __vec2f b);
float		__vnf_v2f_cross(__vec2f a, __vec2f b);

__vec2d		__vnf_v2d_add(__vec2d a, __vec2d b);
__vec2d		__vnf_v2d_sub(__vec2d a, __vec2d b);
__vec2d		__vnf_v2d_mul(__vec2d a, __vec2d b);
__vec2d		__vnf_v2d_div(__vec2d a, __vec2d b);
double		__vnf_v2d_dot(__vec2d a, __vec2d b);
double		__vnf_v2d_cross(__vec2d a, __vec2d b);

__quatf		__vnf_vqf_add(__quatf a, __quatf b);
__quatf		__vnf_vqf_sub(__quatf a, __quatf b);
__quatf		__vnf_vqf_mul(__quatf a, __quatf b);
__quatf		__vnf_vqf_div(__quatf a, __quatf b);

__vec3f		__vnf_v3f_add(__vec3f a, __vec3f b);
__vec3f		__vnf_v3f_sub(__vec3f a, __vec3f b);
__vec3f		__vnf_v3f_mul(__vec3f a, __vec3f b);
__vec3f		__vnf_v3f_div(__vec3f a, __vec3f b);
__vec3f		__vnf_v3f_cross(__vec3f a, __vec3f b);
float		__vnf_v3f_dot(__vec3f a, __vec3f b);

__vec4f		__vnf_v4f_add(__vec4f a, __vec4f b);
__vec4f		__vnf_v4f_sub(__vec4f a, __vec4f b);
__vec4f		__vnf_v4f_mul(__vec4f a, __vec4f b);
__vec4f		__vnf_v4f_div(__vec4f a, __vec4f b);
__vec4f		__vnf_v4f_cross(__vec4f a, __vec4f b);
float		__vnf_v4f_dot(__vec4f a, __vec4f b);

__vec3fx	__vnf_v3fx_add(__vec3fx a, __vec3fx b);
__vec3fx	__vnf_v3fx_sub(__vec3fx a, __vec3fx b);
__vec3fx	__vnf_v3fx_mul(__vec3fx a, __vec3fx b);
__vec3fx	__vnf_v3fx_div(__vec3fx a, __vec3fx b);
__vec3fx	__vnf_v3fx_cross(__vec3fx a, __vec3fx b);
double		__vnf_v3fx_dot(__vec3fx a, __vec3fx b);

__vec3fq	__vnf_v3fq_add(__vec3fq a, __vec3fq b);
__vec3fq	__vnf_v3fq_sub(__vec3fq a, __vec3fq b);
__vec3fq	__vnf_v3fq_mul(__vec3fq a, __vec3fq b);
__vec3fq	__vnf_v3fq_div(__vec3fq a, __vec3fq b);
__vec3fq	__vnf_v3fq_cross(__vec3fq a, __vec3fq b);
float		__vnf_v3fq_dot(__vec3fq a, __vec3fq b);


__m64		__m64_float2(float x, float y);
__m128		__m128_float3(float x, float y, float z);
__m128		__m128_float4(float x, float y, float z, float w);

__vec2f		__v2f_float2(float x, float y);
__vec3f		__v3f_float3(float x, float y, float z);
__vec4f		__v4f_float4(float x, float y, float z, float w);
__quatf		__vqf_float4(float x, float y, float z, float w);

__vec3fq	__v3fq_float3(float x, float y, float z);
__vec3fx	__v3fx_float3(double x, double y, double z);

__vec4h		__v4h_float4(float x, float y, float z, float w);

__fcomplex	__c2f_float2(float x, float y);

__m128		__m128_double2(double x, double y);
__vec2d		__v2d_double2(double x, double y);
__dcomplex	__c2d_double2(double x, double y);


__vec3fq	__vnf_v3fq_from_v3f(__vec3f a);
__vec3f		__vnf_v3f_from_v3fq(__vec3fq a);
__vec4h		__vnf_v4h_from_v4f(__vec4f a);
__vec4f		__vnf_v4f_from_v4h(__vec4h a);

__vec3fx	__vnf_v3fx_from_v3f(__vec3f a);
__vec3f		__vnf_v3f_from_v3fx(__vec3fx a);

__asm {

__v2f_float2:
__c2f_float2:
__m64_float2:
//	FSTCF	R4, R2
//	FSTCFH	R5, R2
	FSTCF	R4, R16
	FSTCF	R5, R17
	MOVLD	R17, R16, R2
	RTS

__v3f_float3:
__m128_float3:
//	FSTCF	R4, R2
//	FSTCFH	R5, R2
	FSTCF	R4, R16
	FSTCF	R5, R17
	MOVLD	R17, R16, R2
	FSTCF	R6, R3
	RTS

__v4f_float4:
__vqf_float4:
__m128_float4:
//	FSTCF	R4, R2
//	FSTCFH	R5, R2
//	FSTCF	R6, R3
//	FSTCFH	R7, R3
//	RTSU

	FSTCF	R4, R16
	FSTCF	R5, R17
	MOVLD	R17, R16, R2
	FSTCF	R6, R18
	FSTCF	R7, R19
	MOVLD	R19, R18, R3
	RTS

__v2d_double2:
__c2d_double2:
__m128_double2:
	MOV		R4, R2
	MOV		R5, R3
	RTS

__vnf_v2f_add:
__vnf_c2f_add:
	PADD.F	R4, R5, R2
	RTS
__vnf_v2f_sub:
__vnf_c2f_sub:
	PSUB.F	R4, R5, R2
	RTS
__vnf_v2f_mul:
	PMUL.F	R4, R5, R2
	RTS

__vnf_v2f_dot:
	PMUL.F	R4, R5, R6
	FLDCF	R6, R7
	FLDCFH	R6, R3
	FADD	R3, R7, R2
	RTS

__vnf_v2f_cross:
	MOVLHD	R5, R5, R7
	PMUL.F	R4, R7, R6
	FLDCF	R6, R7
	FLDCFH	R6, R3
	FSUB	R7, R3, R2
	RTS

__vnf_c2f_mul:
	MOVLHD	R5, R5, R6		//{    Bi,     Br}
	PMUL.F	R4, R5, R16		//{ Ar*Br,  Ai*Bi}
	PMUL.F	R4, R6, R17		//{ Ar*Bi,  Ai*Br}
	FNEG	R16				//{ Ar*Br, -Ai*Bi}
	MOVLD	R17, R16, R18	//{ Ar*Br,  Ar*Bi}
	MOVHD	R17, R16, R19	//{-Ai*Bi,  Ai*Br}
	PADD.F	R18, R19, R2	//{Ar*Br-Ai*Bi, Ar*Bi+Ai*Br}
	RTSU

__vnf_v2d_add:
__vnf_c2d_add:
	FADD	R4, R6, R2
	FADD	R5, R7, R3
	RTS
__vnf_v2d_sub:
__vnf_c2d_sub:
	FADD	R4, R6, R2
	FADD	R5, R7, R3
	RTS
__vnf_v2d_mul:
	FMUL	R4, R6, R2
	FMUL	R5, R7, R3
	RTS

__vnf_v2d_dot:
	FMUL	R4, R6, R18
	FMUL	R5, R7, R19
	FADD	R18, R19, R2
	RTS
__vnf_v2d_cross:
	FMUL	R4, R7, R18
	FMUL	R5, R6, R19
	FSUB	R18, R19, R2
	RTS

__vnf_v3f_add:
__vnf_v4f_add:
__vnf_vqf_add:
	PADDX.F	R4, R6, R2
	RTS
__vnf_v3f_sub:
__vnf_v4f_sub:
__vnf_vqf_sub:
	PSUBX.F	R4, R6, R2
	RTS
__vnf_v3f_mul:
__vnf_v4f_mul:
	PMULX.F	R4, R6, R2
	RTS

#if 0
__vnf_v3f_div:
__vnf_v4f_div:
	MOV			0x7F0000007F000000, R2
	MOV			0x7F0000007F000000, R3
	PSUBX.F		R2, R6, R16
	PMUL.F		R4, R16, R2
	RTSU
#endif

__vnf_vqf_mul:
	PSHUFX.L	R4, 0xFF, R16
	PMULX.F		R6, R16, R22

	PSHUFX.L	R4, 0x92, R16
	PSHUFX.L	R6, 0x89, R18
	PMULX.F		R16, R18, R20
	PSUBX.F		R22, R20, R2

	PSHUFX.L	R4, 0x24, R16
	PSHUFX.L	R6, 0x3F, R18
	PMULX.F		R16, R18, R20
	FNEG		R21
	PADDX.F		R2, R20, R2

	PSHUFX.L	R4, 0x49, R16
	PSHUFX.L	R6, 0x52, R18
	PMULX.F		R16, R18, R20
	FNEG		R21
	PADDX.F		R2, R20, R2
	
	RTSU

#if 0
__vnf_vqf_div:
	MOV			0x8000000080000000, R2
	MOV			0x0000000080000000, R3
	PMULX.F		R6, R6, R16
	XOR			R6, R2, R18
	XOR			R7, R3, R19
	MOV			0x7F0000007F000000, R2
	MOV			0x7F0000007F000000, R3
	PSUBX.F		R2, R16, R20
	PMUL.F		R18, R20, R6
	BRA			__vnf_vqf_mul
//	RTSU
#endif

__vnf_v3f_cross:
__vnf_v4f_cross:
	PSHUFX.L	R4, 0x49, R16
	PSHUFX.L	R6, 0x52, R18
	PMULX.F		R16, R18, R20
	PSHUFX.L	R4, 0x92, R16
	PSHUFX.L	R6, 0x89, R18
	PMULX.F		R16, R18, R22
//	PADDX.F		R22, R20, R2
	PSUBX.F		R20, R22, R2
	EXTU.L		R3
	RTSU

__vnf_v3f_dot:
	PMULX.F		R4, R6, R16
	EXTU.L		R17
	PADD.F		R16, R17, R3
	FLDCF		R3, R18
	FLDCFH		R3, R19
	FADD		R18, R19, R2
	RTSU

__vnf_v4f_dot:
	PMULX.F		R4, R6, R16
	PADD.F		R16, R17, R3
	FLDCF		R3, R18
	FLDCFH		R3, R19
	FADD		R18, R19, R2
	RTSU


__v3fq_float3:
	FSTCF	R4, R16
	FSTCF	R5, R17
	FSTCF	R6, R19
	MOVLLD	R17, R16, R18
	PSTCH	R18, R2
	PSTCH	R19, R3

	SHLD	R4, -8, R20
	SHLD	R5, -3, R21
	SHLD	R6,  2, R22
	AND		0x001F, R20
	AND		0x03E0, R21
	AND		0x7C00, R22
	OR		R20, R21, R23
	OR		R23, R22, R23
	SHLD.Q	R23, 48, R23
	OR		R23, R3
	RTSU

//	SHLD	R16, -11, R16
//	SHLD	R17, -11, R17
//	SHLD	R18, -10, R18
//	SHLD.Q	R17, 21, R17
//	SHLD.Q	R18, 42, R18
//	OR		R16, R17, R19
//	OR		R18, R19, R2
//	RTSU

__v3fq_unpackx2:
	PLDCEHL	R4, R20
	PLDCEHH	R4, R21
	PLDCEHL	R5, R22
	PLDCEHH	R5, R23
	MOVX	R20, R4
	MOVX	R22, R6
	RTS

#if 0
	SHLD.Q	R4, 11, R16
	SHLD.Q	R4, -10, R17
	SHLD.Q	R4, -32, R18

	SHLD.Q	R5, 11, R20
	SHLD.Q	R5, -10, R21
	SHLD.Q	R5, -32, R22
	MOVLD	R17, R16, R4
	MOV		R18, R5
	MOVLD	R21, R20, R6
	MOV		R22, R7
	RTSU
#endif

__v3fq_packx:
	PSTCH	R2, R18
	PSTCH	R3, R19

	SHLD.Q	R2,  -8, R20
	SHLD.Q	R2, -35, R21
	SHLD.Q	R3,   2, R22
	AND		0x001F, R20
	AND		0x03E0, R21
	AND		0x7C00, R22
	OR		R20, R21, R23
	OR		R23, R22, R23

	SHLD.Q	R19, 32, R19
	OR		R19, R18

	SHLD.Q	R23, 48, R23
	OR		R23, R18

	MOV		R18, R2

	RTS

__vnf_v3fq_add:
	MOV			LR, R1
	BSR			__v3fq_unpackx2
//	PADDX.F		R4, R6, R2
//	FLDCF		R2, R4
//	FLDCFH		R2, R5
//	FLDCF		R3, R6
//	BSR			__v3fq_float3

	PADDX.F		R4, R6, R2
	BSR			__v3fq_packx
	JMP			R1

__vnf_v3fq_sub:
	MOV			LR, R1
	BSR			__v3fq_unpackx2
//	PSUBX.F		R4, R6, R2
//	FLDCF		R2, R4
//	FLDCFH		R2, R5
//	FLDCF		R3, R6
//	BSR			__v3fq_float3
//	JMP			R1
	PSUBX.F		R4, R6, R2
	BSR			__v3fq_packx
	JMP			R1

__vnf_v3fq_mul:
	MOV			LR, R1
	BSR			__v3fq_unpackx2
	PMULX.F		R4, R6, R2
	BSR			__v3fq_packx
	JMP			R1

//	PMULX.F		R4, R6, R2
//	FLDCF		R2, R4
//	FLDCFH		R2, R5
//	FLDCF		R3, R6
//	BSR			__v3fq_float3
//	JMP			R23

__vnf_v3fq_dot:
	MOV			LR, R1
	BSR			__v3fq_unpackx2
	MOV			R1, LR
	BRA			__vnf_v3f_dot
__vnf_v3fq_cross:
	MOV			LR, R1
	BSR			__v3fq_unpackx2
	BSR			__vnf_v3f_cross
	BSR			__v3fq_packx
//	FLDCF		R2, R4
//	FLDCFH		R2, R5
//	FLDCF		R3, R6
//	BSR			__v3fq_float3
	JMP			R1


__vnf_v3fq_from_v3f:
	PSTCH	R4, R18
	PSTCH	R5, R19

	SHLD.Q	R4,  -8, R20
	SHLD.Q	R4, -35, R21
	SHLD.Q	R5,   2, R22
	AND		0x001F, R20
	AND		0x03E0, R21
	AND		0x7C00, R22
	OR		R20, R21, R23
	OR		R23, R22, R23

	SHLD.Q	R19, 32, R19
	OR		R19, R18

	SHLD.Q	R23, 48, R23
	OR		R23, R18

	MOV		R18, R2

	RTS

__vnf_v3f_from_v3fq:
	PLDCEHL	R4, R2
	PLDCEHH	R4, R3
	RTS

__vnf_v4h_from_v4f:
	PSTCH	R4, R2
	PSTCH	R5, R3
	SHLD.Q	R3, 32, R3
	OR		R3, R2
	RTS

__vnf_v4f_from_v4h:
	PLDCHL	R4, R2
	PLDCHH	R4, R3
	RTS

__v4h_float4:
	FSTCH	R4, R20
	FSTCH	R5, R21
	FSTCH	R6, R22
	FSTCH	R7, R23
	SHLD.Q	R21, 16, R21
	SHLD.Q	R22, 32, R22
	SHLD.Q	R23, 48, R23
	OR		R20, R21, R18
	OR		R22, R23, R19
	OR		R18, R19, R2
	RTS

__v3fx_float3:
	SHLD	R4, -21, R16
	SHLD	R5, -21, R17
	SHLD	R6, -22, R18
	SHLD.Q	R18, 22, R18
	SHLD.Q	R17,  43, R20
	SHLD.Q	R17, -21, R21
	OR		R16, R20, R2
	OR		R18, R21, R3
	RTSU

__v3fx_unpackx2:
	SHLD.Q	R4, 21, R16
	MOV	R5, R18
	SHLD.Q	R6, 21, R20
	MOV	R7, R22
	SHLD.Q	R4, -22, R19
	SHLD.Q	R5, 22, R17
	OR		R19, R17
	SHLD.Q	R6, -22, R19
	SHLD.Q	R7, 22, R21
	OR		R19, R21
	RTSU

__vnf_v3fx_add:
	MOV			LR, R23
	BSR			__v3fx_unpackx2
	FADD		R16, R20, R4
	FADD		R17, R21, R5
	FADD		R18, R22, R6
	MOV			R23, LR
	BRA			__v3fx_float3
__vnf_v3fx_sub:
	MOV			LR, R23
	BSR			__v3fx_unpackx2
	FSUB		R16, R20, R4
	FSUB		R17, R21, R5
	FSUB		R18, R22, R6
	MOV			R23, LR
	BRA			__v3fx_float3
__vnf_v3fx_mul:
	MOV			LR, R23
	BSR			__v3fx_unpackx2
	FMUL		R16, R20, R4
	FMUL		R17, R21, R5
	FMUL		R18, R22, R6
	MOV			R23, LR
	BRA			__v3fx_float3

__vnf_v3fx_dot:
	MOV			LR, R23
	BSR			__v3fx_unpackx2
	FMUL		R16, R20, R4
	FMUL		R17, R21, R5
	FMUL		R18, R22, R6
	FADD		R4, R5, R3
	FADD		R3, R6, R2
	JMP			R23


};

__vec2f __vnf_v2f_div(__vec2f A, __vec2f B)
{
	__vec2f vc;
	vc=(__vec2f){ A.x/B.x, A.y/B.y };
	return(vc);
}

__vec3f __vnf_v3f_div(__vec3f A, __vec3f B)
{
	__vec3f vc;
	vc=(__vec3f){ A.x/B.x, A.y/B.y, A.z/B.z };
	return(vc);
}

__vec4f __vnf_v4f_div(__vec4f A, __vec4f B)
{
	__vec4f vc;
	vc=(__vec4f){ A.x/B.x, A.y/B.y, A.z/B.z, A.w/B.w };
	return(vc);
}

_Complex float __vnf_c2f_rcp(_Complex float A)
{
	__vec2f va, vb, vc, vd;
	float fa;
	va=(__vec2f)A;
//	fa=va^va;
	fa=__vnf_v2f_dot(va, va);
	vb=(__vec2f) { fa, fa };
	vc=(__vec2f) { va.x, -va.y };
	vd=vc/vb;
	return(vd);
}

_Complex float __vnf_c2f_div(_Complex float A, _Complex float B)
	{ return(A*__vnf_c2f_rcp(B)); }


__quatf __vnf_vqf_rcp(__quatf A)
{
	__vec4f va, vb, vc, vd;
	float fa;
	va=(__vec4f)A;
//	fa=va^va;
	fa=__vnf_v4f_dot(va, va);
	vb=(__vec4f) { fa, fa, fa, fa };
	vc=(__vec4f) { -va.x, -va.y, -va.z, va.w };
	vd=vc/vb;
	return((__quatf)vd);
}

__quatf		__vnf_vqf_div(__quatf A, __quatf B)
	{ return(A*__vnf_vqf_rcp(B)); }
