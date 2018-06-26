#include <stddef.h>

#pragma linkage(__aopen, OS)
void *__aopen(const char *ddname, int mode, int *recfm,
              int *lrecl, int *blksize, void **asmbuf, const char *mem);
#pragma linkage(__aread, OS)
int __aread(void *handle, void *buf);
#pragma linkage(__awrite, OS)
int __awrite(void *handle, const void *buf, size_t sz);
#pragma linkage(__aclose, OS)
void __aclose(void *handle);
#pragma linkage(__getclk, OS)
unsigned int __getclk(void *buf);
#pragma linkage(__getm, OS)
void *__getm(size_t sz);
#pragma linkage(__freem, OS)
void __freem(void *ptr);

#pragma linkage(__dynal, OS)
int __dynal(size_t ddn_len, char *ddn, size_t dsn_len, char *dsn);

#pragma linkage(__idcams, OS)
int __idcams(size_t len, char *data);

#pragma linkage(__system, OS)
int __system(int req_type, 
             size_t pgm_len,
             char *pgm,
             size_t parm_len,
             char *parm);
