FILE *c2be_fopen(_stdio_context_t *ctx,
	const char *filename, const char *mode);
int c2be_remove(_stdio_context_t *ctx,
	const char *filename);
int c2be_rename(_stdio_context_t *ctx,
	const char *oldname, const char *newname);
FILE **c2be_get_stream_fd(_stdio_context_t *ctx, int fd);

void *c2be_malloc_cat(_stdio_context_t *ctx, size_t size, int cat);
void *c2be_realloc(_stdio_context_t *ctx, void *ptr, size_t size);
void c2be_free(_stdio_context_t *ctx, void *ptr);
void c2be_abort(_stdio_context_t *ctx);
void c2be_exit(_stdio_context_t *ctx, int status);

signal_t c2be_signal(_stdio_context_t *ctx, int sig, signal_t func);
int c2be_raise(_stdio_context_t *ctx, int sig);
clock_t c2be_clock(_stdio_context_t *ctx);


struct _stdio_ctxvtable_s _stdio_ctxvtable_vt = {
c2be_fopen,
c2be_remove,
c2be_rename,
c2be_get_stream_fd,
c2be_malloc_cat,
c2be_realloc,
c2be_free,
c2be_abort,
c2be_exit,
c2be_signal,
c2be_raise,
c2be_clock
};



int c2be_fclose(FILE *stream);
int c2be_fflush(FILE *stream);
int c2be_feof(FILE *stream);
long c2be_ftell(FILE *stream);
int c2be_fseek(FILE *stream, long offset, int whence);

size_t c2be_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t c2be_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int c2be_fgetc(FILE *stream);
int c2be_fputc(int c, FILE *stream);
char *c2be_fgets(char *s, int n, FILE *stream);
int c2be_fputs(const char *s, FILE *stream);

int c2be_ungetc(int c, FILE *stream);

int c2be_ferror((FILE *stream);
void c2be_clearerr(FILE *stream);

int c2be_setvbuf(FILE *stream, char *buf, int mode, size_t size);
FILE *c2be_freopen(const char *filename, const char *mode, FILE *stream);

struct _stdio_vtable_s _stdio_vtable_vt = {
c2be_fclose,
c2be_fflush,
c2be_feof,
c2be_ftell,
c2be_fseek,
c2be_fread,
c2be_fwrite,
c2be_fgetc,
c2be_fputc,
c2be_fgets,
c2be_fputs,
c2be_ungetc,
c2be_ferror,
c2be_clearerr,
c2be_setvbuf,
c2be_freopen
};
