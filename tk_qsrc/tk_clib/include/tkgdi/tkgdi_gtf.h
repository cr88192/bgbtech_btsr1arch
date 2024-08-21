#define BTMGL_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define BGTF_MAGIC	BTMGL_FOURCC('B', 'G', 'T', 'F')

typedef struct BGTF_FontImage_s BGTF_FontImage;
typedef struct BGTF_FontGlyph_s BGTF_FontGlyph;
typedef struct BGTF_FontVert_s BGTF_FontVert;

struct BGTF_FontImage_s {
u32	*img_data;
int sz_image;
u32 *pagedir;
int celsz_x;
int celsz_y;
int min_x, min_y;
int max_x, max_y;
byte dircompact;
BGTF_FontGlyph *cache;
};

struct BGTF_FontVert_s {
short x;
short y;
byte tag;	//tag bits
};

struct BGTF_FontGlyph_s {
BGTF_FontGlyph *next;
BGTF_FontImage *image;
BGTF_FontVert *verts;
BGTF_FontVert *spans;
int nvert;
int nspan;
int utf;
int adv_x;
int csz_x, csz_y;
int org_x, org_y;
int min_x, min_y;
int max_x, max_y;
};


BGTF_FontImage *BGTF_LoadFontImage(char *name);
BGTF_FontGlyph *BGTF_ImageGetGlyph(BGTF_FontImage *img, int utf);

int BTGF_GlyphCheckPoint(BGTF_FontGlyph *glif, int x, int y, int psz);
int BTGF_GlyphDrawIndex8(BGTF_FontGlyph *glif,
	byte *oimg, int oxs, int oys, int oxo, int oyo,
	int cxs, int cys, int clr0, int clr1);
