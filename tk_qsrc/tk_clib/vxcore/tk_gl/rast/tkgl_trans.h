typedef __vec4f	tkgl_vec4f;

#define tkgl_v4fmul(a, b)	((a)*(b))
#define tkgl_v4fadd(a, b)	((a)+(b))

typedef struct {
	tkgl_vec4f	row0;
	tkgl_vec4f	row1;
	tkgl_vec4f	row2;
	tkgl_vec4f	row3;
}tkgl_mat4;