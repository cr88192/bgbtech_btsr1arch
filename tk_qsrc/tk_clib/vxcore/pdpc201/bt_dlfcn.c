/*
Stuff for implementing the libdl interface.
 */

void *TK_DlOpenA(const char *path, int flags);
void *TK_DlSymA(void *handle, const char *symbol, int flags);

void *dlopen(const char *path, int flags)
{
	return(TK_DlOpenA(path, flags));
}

char *dlerror(void)
{
	return(TK_DlSymA(NULL, NULL, 5));
}

void *dlsym(void *handle, const char *symbol)
{
	return(TK_DlSymA(handle, symbol, 0));
}

void dlclose(void *handle)
{
	TK_DlSymA(handle, NULL, 7);
}
