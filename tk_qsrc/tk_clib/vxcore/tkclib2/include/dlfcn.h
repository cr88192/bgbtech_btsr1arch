#ifndef STD_DLFCN_H
#define STD_DLFCN_H

#define RTLD_LAZY		0x0001
#define RTLD_NOW		0x0002
#define RTLD_NOLOAD		0x0004
#define RTLD_GLOBAL		0x0100
#define RTLD_LOCAL		0x0000

#define RTLD_NEXT		((void *)(-1))
#define RTLD_DEFAULT	((void *)(-2))
#define RTLD_SELF		((void *)(-3))

int    dlclose(void *hdl);
char  *dlerror(void);
void  *dlopen(const char *name, int flag);
void  *dlsym(void *hdl, const char *name);

#endif
