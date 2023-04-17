#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

typedef unsigned char byte;
typedef signed char sbyte;

typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;


#define bgbmid_fopen	fopen
#define bgbmid_fclose	fclose
#define bgbmid_fread	fread
#define bgbmid_fwrite	fwrite
#define bgbmid_fgetc	fgetc
#define bgbmid_fputc	fputc
#define bgbmid_fseek	fseek
#define bgbmid_ftell	ftell

void *bgbmid_malloc(int sz)
{
	void *ptr;
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

void bgbmid_free(void *ptr)
{
	free(ptr);
}
