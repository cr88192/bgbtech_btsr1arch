typedef struct BGBMID_PackHead_s BGBMID_PackHead;
typedef struct BGBMID_PackDirEnt_s BGBMID_PackDirEnt;
typedef struct BGBMID_PackState_s BGBMID_PackState;

struct BGBMID_PackHead_s {
char fcc[4];
byte offs[4];
byte ents[4];
};

struct BGBMID_PackDirEnt_s {
char name[56];
byte offs[4];
byte size[4];
};

struct BGBMID_PackState_s {
BGBMID_PackState *next;
char *name;

BGBMID_PackHead hdr;
BGBMID_PackDirEnt *dir;
void *fd;
int doffs, dents;
};
