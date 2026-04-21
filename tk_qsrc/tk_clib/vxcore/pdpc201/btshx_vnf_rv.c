#if defined(__RISCV__) || defined(__XG3__)


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
	FSTCF	R10, R16
	FSTCF	R11, R17
	EXTU.L	R16, R16
	SHLD.Q	R17, 32, R17
	OR	R17, R16, R10
	RTS
};

__asm {
__v4f_float4:
__m128_float4:
	FSTCF	R10, R16
	FSTCF	R11, R17
	EXTU.L	R16, R16
	SHLD.Q	R17, 32, R17
	OR	R17, R16, R10

	FSTCF	R12, R16
	FSTCF	R13, R17
	EXTU.L	R16, R16
	SHLD.Q	R17, 32, R17
	OR	R17, R16, R11
	RTS
};

__asm {
__vnf_v4f_add:
__vnf_v4fa_add:
__vnf_vqf_add:
__vnf_v3f_add:
.ifarch has_simd
	MOV		X10, F10
	MOV		X11, F11
	MOV		X12, F12
	MOV		X13, F13
	PADD.S	F10, F12, F14
	PADD.S	F11, F13, F15
	MOV		F14, X10
	MOV		F15, X11
	RTS
.else
	ADD		-64, SP
	MOV.Q	X10, (SP,  0)
	MOV.Q	X11, (SP,  8)
	MOV.Q	X12, (SP, 16)
	MOV.Q	X13, (SP, 24)
	MOVD.L	(SP,  0), R42
	MOVD.L	(SP,  4), R43
	MOVD.L	(SP,  8), R44
	MOVD.L	(SP, 12), R45
	MOVD.L	(SP, 16), R46
	MOVD.L	(SP, 20), R47
	MOVD.L	(SP, 24), R48
	MOVD.L	(SP, 28), R49
	FADD.S	R42, R46, R42
	FADD.S	R43, R47, R43
	FADD.S	R44, R48, R44
	FADD.S	R45, R49, R45
	MOV.L	R42, (SP,  0)
	MOV.L	R43, (SP,  4)
	MOV.L	R44, (SP,  8)
	MOV.L	R45, (SP, 12)
	MOV.Q	(SP, 0), R10
	MOV.Q	(SP, 8), R11
	ADD		64, SP
	RTS
.endif
};

__asm {
__vnf_v4f_sub:
__vnf_v4fa_sub:
__vnf_vqf_sub:
__vnf_v3f_sub:
.ifarch has_simd
	MOV		X10, F10
	MOV		X11, F11
	MOV		X12, F12
	MOV		X13, F13
	PSUB.S	F10, F12, F14
	PSUB.S	F11, F13, F15
	MOV		F14, X10
	MOV		F15, X11
	RTS
.else
	ADD		-64, SP
	MOV.Q	X10, (SP,  0)
	MOV.Q	X11, (SP,  8)
	MOV.Q	X12, (SP, 16)
	MOV.Q	X13, (SP, 24)
	MOVD.L	(SP,  0), R42
	MOVD.L	(SP,  4), R43
	MOVD.L	(SP,  8), R44
	MOVD.L	(SP, 12), R45
	MOVD.L	(SP, 16), R46
	MOVD.L	(SP, 20), R47
	MOVD.L	(SP, 24), R48
	MOVD.L	(SP, 28), R49
	FSUB.S	R42, R46, R42
	FSUB.S	R43, R47, R43
	FSUB.S	R44, R48, R44
	FSUB.S	R45, R49, R45
	MOV.L	R42, (SP,  0)
	MOV.L	R43, (SP,  4)
	MOV.L	R44, (SP,  8)
	MOV.L	R45, (SP, 12)
	MOV.Q	(SP, 0), R10
	MOV.Q	(SP, 8), R11
	ADD		64, SP
	RTS
.endif
};

__asm {
__vnf_v4f_mul:
__vnf_v4fa_mul:
__vnf_v3f_mul:
.ifarch has_simd
	MOV		X10, F10
	MOV		X11, F11
	MOV		X12, F12
	MOV		X13, F13
	PMUL.S	F10, F12, F14
	PMUL.S	F11, F13, F15
	MOV		F14, X10
	MOV		F15, X11
	RTS
.else
	ADD		-64, SP
	MOV.Q	X10, (SP,  0)
	MOV.Q	X11, (SP,  8)
	MOV.Q	X12, (SP, 16)
	MOV.Q	X13, (SP, 24)
	MOVD.L	(SP,  0), R42
	MOVD.L	(SP,  4), R43
	MOVD.L	(SP,  8), R44
	MOVD.L	(SP, 12), R45
	MOVD.L	(SP, 16), R46
	MOVD.L	(SP, 20), R47
	MOVD.L	(SP, 24), R48
	MOVD.L	(SP, 28), R49
	FMUL.S	R42, R46, R42
	FMUL.S	R43, R47, R43
	FMUL.S	R44, R48, R44
	FMUL.S	R45, R49, R45
	MOV.L	R42, (SP,  0)
	MOV.L	R43, (SP,  4)
	MOV.L	R44, (SP,  8)
	MOV.L	R45, (SP, 12)
	MOV.Q	(SP, 0), R10
	MOV.Q	(SP, 8), R11
	ADD		64, SP
	RTS
.endif
};




__asm {
__vnf_v2f_add:
__vnf_c2f_add:
__vnf_v2fa_add:
.ifarch has_simd
	MOV		X10, F10
	MOV		X11, F11
	PADD.S	F10, F11, F14
	MOV		F14, X10
	RTS
.else
	ADD		-64, SP
	MOV.Q	X10, (SP,  0)
	MOV.Q	X11, (SP,  8)
	MOVD.L	(SP,  0), R42
	MOVD.L	(SP,  4), R43
	MOVD.L	(SP,  8), R44
	MOVD.L	(SP, 12), R45
	FADD.S	R42, R44, R42
	FADD.S	R43, R45, R43
	MOV.L	R42, (SP,  0)
	MOV.L	R43, (SP,  4)
	MOV.Q	(SP, 0), R10
	ADD		64, SP
	RTS
.endif
};

__asm {
__vnf_v2f_sub:
__vnf_c2f_sub:
__vnf_v2fa_sub:
.ifarch has_simd
	MOV		X10, F10
	MOV		X11, F11
	PSUB.S	F10, F11, F14
	MOV		F14, X10
	RTS
.else
	ADD		-64, SP
	MOV.Q	X10, (SP,  0)
	MOV.Q	X11, (SP,  8)
	MOVD.L	(SP,  0), R42
	MOVD.L	(SP,  4), R43
	MOVD.L	(SP,  8), R44
	MOVD.L	(SP, 12), R45
	FSUB.S	R42, R44, R42
	FSUB.S	R43, R45, R43
	MOV.L	R42, (SP,  0)
	MOV.L	R43, (SP,  4)
	MOV.Q	(SP, 0), R10
	ADD		64, SP
	RTS
.endif
};

__asm {
__vnf_v2f_mul:
__vnf_v2fa_mul:
.ifarch has_simd
	MOV		X10, F10
	MOV		X11, F11
	PMUL.S	F10, F11, F14
	MOV		F14, X10
	RTS
.else
	ADD		-64, SP
	MOV.Q	X10, (SP,  0)
	MOV.Q	X11, (SP,  8)
	MOVD.L	(SP,  0), R42
	MOVD.L	(SP,  4), R43
	MOVD.L	(SP,  8), R44
	MOVD.L	(SP, 12), R45
	FMUL.S	R42, R44, R42
	FMUL.S	R43, R45, R43
	MOV.L	R42, (SP,  0)
	MOV.L	R43, (SP,  4)
	MOV.Q	(SP, 0), R10
	ADD		64, SP
	RTS
.endif
};


__asm {
__vnf_v4f_dot:
__vnf_v4fa_dot:
.ifarch has_simd
	MOV		X10, F10
	MOV		X11, F11
	MOV		X12, F12
	MOV		X13, F13
	PMUL.S	F10, F12, F14
	PMUL.S	F11, F13, F15
	PADD.S	F14, F15, F16
	MOVHD	F16, F16, F17
	PADD.S	F16, F17, F16
	FLDCF	F16, F10
	MOV		F10, X10
	RTS
.else
	ADD		-64, SP
	MOV.Q	X10, (SP,  0)
	MOV.Q	X11, (SP,  8)
	MOV.Q	X12, (SP, 16)
	MOV.Q	X13, (SP, 24)
	MOVD.L	(SP,  0), R42
	MOVD.L	(SP,  4), R43
	MOVD.L	(SP,  8), R44
	MOVD.L	(SP, 12), R45
	MOVD.L	(SP, 16), R46
	MOVD.L	(SP, 20), R47
	MOVD.L	(SP, 24), R48
	MOVD.L	(SP, 28), R49
	FMUL.S	R42, R46, R42
	FMUL.S	R43, R47, R43
	FMUL.S	R44, R48, R44
	FMUL.S	R45, R49, R45

	FLDCF	R42, R42
	FLDCF	R43, R43
	FLDCF	R44, R44
	FLDCF	R45, R45
	
	FADD	R42, R43, R46
	FADD	R44, R45, R47
	FADD	R46, R47, R48
	MOV		R48, R10

	ADD		64, SP
	RTS
.endif
};

__asm {
__vnf_v3f_dot:
__vnf_v3fa_dot:
.ifarch has_simd
	MOV		X10, F10
	MOV		X11, F11
	MOV		X12, F12
	MOV		X13, F13
	PMUL.S	F10, F12, F14
	PMUL.S	F11, F13, F15
	MOV		R0, F17
	MOVLD	F17, F15, F15
	PADD.S	F14, F15, F16
	MOVHD	F16, F16, F17
	PADD.S	F16, F17, F16
	FLDCF	F16, F10
	MOV		F10, X10
	RTS
.else
	ADD		-64, SP
	MOV.Q	X10, (SP,  0)
	MOV.Q	X11, (SP,  8)
	MOV.Q	X12, (SP, 16)
	MOV.Q	X13, (SP, 24)
	MOVD.L	(SP,  0), R42
	MOVD.L	(SP,  4), R43
	MOVD.L	(SP,  8), R44
	MOVD.L	(SP, 16), R46
	MOVD.L	(SP, 20), R47
	MOVD.L	(SP, 24), R48
	FMUL.S	R42, R46, R42
	FMUL.S	R43, R47, R43
	FMUL.S	R44, R48, R44

	FLDCF	R42, R42
	FLDCF	R43, R43
	FLDCF	R44, R44
	
	FADD	R42, R43, R46
	FADD	R46, R44, R48
	MOV		R48, R10

	ADD		64, SP
	RTS
.endif
};

__asm {
__vnf_v2f_dot:
__vnf_v2fa_dot:
.ifarch has_simd
	MOV		X10, F10
	MOV		X11, F11
	PMUL.S	F10, F11, F14
	MOVHD	F14, F14, F15
	PADD.S	F14, F15, F16
	FLDCF	F16, F10
	MOV		F10, X10
	RTS
.else
	ADD		-64, SP
	MOV.Q	X10, (SP,  0)
	MOV.Q	X11, (SP,  8)
	MOVD.L	(SP,  0), R42
	MOVD.L	(SP,  4), R43
	MOVD.L	(SP,  8), R44
	MOVD.L	(SP, 12), R45
	FMUL.S	R42, R44, R42
	FMUL.S	R43, R45, R43
	FLDCF	R42, R42
	FLDCF	R43, R43
	FADD	R42, R43, R46
	MOV		R46, R10
	ADD		64, SP
	RTS
.endif
};

__asm {
__vnf_v2f_cross:
__vnf_v2fa_cross:
	ADD		-64, SP
	MOV.Q	X10, (SP,  0)
	MOV.Q	X11, (SP,  8)
	MOVD.L	(SP,  0), R42
	MOVD.L	(SP,  4), R43
	MOVD.L	(SP,  8), R44
	MOVD.L	(SP, 12), R45
	FMUL.S	R42, R45, R42
	FMUL.S	R43, R44, R43
	FLDCF	R42, R42
	FLDCF	R43, R43
	FSUB	R42, R43, R46
	MOV		R46, R10
	ADD		64, SP
	RTS
};

#endif

