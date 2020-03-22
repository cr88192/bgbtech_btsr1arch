
tkgl_vec4f TKGL_ProjectVertex(tkgl_vec4f vec, tkgl_mat4 mat)
{
	return(tkgl_v4fadd(
		tkgl_v4fadd(
			tkgl_v4fmul(vec, mat.row0),
			tkgl_v4fmul(vec, mat.row1)),
		tkgl_v4fadd(
			tkgl_v4fmul(vec, mat.row2),
			tkgl_v4fmul(vec, mat.row3))));
}
