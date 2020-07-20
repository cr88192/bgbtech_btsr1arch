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


__m64		__m64_float2(float x, float y);
__m128		__m128_float3(float x, float y, float z);
__m128		__m128_float4(float x, float y, float z, float w);

__vec2f		__v2f_float2(float x, float y);
__vec3f		__v3f_float3(float x, float y, float z);
__vec4f		__v4f_float4(float x, float y, float z, float w);
__quatf		__vqf_float4(float x, float y, float z, float w);

__fcomplex	__c2f_float2(float x, float y);

__m128		__m128_double2(double x, double y);
__vec2d		__v2d_double2(double x, double y);
__dcomplex	__c2d_double2(double x, double y);

__asm {

__v2f_float2:
__c2f_float2:
__m64_float2:
	FSTCF	R4, R2
	FSTCFH	R5, R2
	RTSU
__v3f_float3:
__m128_float3:
	FSTCF	R4, R2
	FSTCFH	R5, R2
	FSTCF	R6, R3
	RTSU
__v4f_float4:
__vqf_float4:
__m128_float4:
	FSTCF	R4, R2
	FSTCFH	R5, R2
	FSTCF	R6, R3
	FSTCFH	R7, R3
	RTSU

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
	RTSU

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
	PADDX.F		R22, R20, R2
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
