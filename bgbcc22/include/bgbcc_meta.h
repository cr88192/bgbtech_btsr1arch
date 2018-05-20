typedef struct BGBDYC_MetaNode_s BGBDYC_MetaNode;
typedef struct BGBDYC_MetaLeaf_s BGBDYC_MetaLeaf;
typedef struct BGBDYC_MetaBiNode_s BGBDYC_MetaBiNode;

typedef struct BGBDYC_MetaBinary_s BGBDYC_MetaBinary;

#if 0
struct BGBDYC_MetaNode_s {
BGBDYC_MetaNode *next;
BGBDYC_MetaNode *first;
BGBDYC_MetaLeaf *leaf;
char *name;
};
#endif

#if 1
struct BGBDYC_MetaNode_s {
BGBDYC_MetaNode *next;		//next node in list
BGBDYC_MetaNode *prev;		//prev node in list
BGBDYC_MetaNode *first;		//first sub-node in list
BGBDYC_MetaNode *owner;		//owner node

BGBDYC_MetaBiNode *biroot;	//binary-root of sub-nodes

BGBDYC_MetaLeaf *leaf;		//leaf-key value
char *name;			//node name
};

struct BGBDYC_MetaBiNode_s {
BGBDYC_MetaBiNode *lnode;	//left sub-node
BGBDYC_MetaBiNode *rnode;	//right sub-node
BGBDYC_MetaNode *mid;		//node item
int depth;			//current tree depth
};
#endif

struct BGBDYC_MetaLeaf_s {
BGBDYC_MetaLeaf *next;
char *key;
char *val;
byte pf;
byte idx;
};

struct BGBDYC_MetaBinary_s {
byte *buf;

char *strs[65536];

byte *ip;
byte *ipe;
};
