typedef struct TKPE_ImageInfo_s	TKPE_ImageInfo;
typedef struct TKPE_TaskInfo_s		TKPE_TaskInfo;

struct TKPE_ImageInfo_s {
int pboix;
byte *imgbase;
void *imgname;
byte *bootptr;		//entry point for image
byte *bootgbr;		//pointer to start of data section
byte *bootgbre;		//pointer to end of data section
};

struct TKPE_TaskInfo_s {
int pid;
void *baseptr;		//base pointer for main binary
void *bootptr;		//entry point for main binary
void *basegbr;		//GBR for main binary, set on program startup.
void *boottbr;		//main process TBR (threads), self-pointer for main thread.
char tlsdat[2048];	//data for TLS

void *span_ptr[256];
int span_sz[256];
int n_span;
};
