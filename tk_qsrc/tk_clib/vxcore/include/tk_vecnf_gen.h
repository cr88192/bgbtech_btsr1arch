typedef struct vec2f_s vec2f;
typedef struct vec3f_s vec3f;
typedef struct vec4f_s vec4f;

struct vec2f_s {
	float x;
	float y;
};

struct vec4f_s {
	float x;
	float y;
	float z;
	float w;
};

static vec2f v2fAdd(vec2f a, vec2f b)
{
	vec2f c;
	c.x=a.x+b.x;
	c.y=a.y+b.y;
}
