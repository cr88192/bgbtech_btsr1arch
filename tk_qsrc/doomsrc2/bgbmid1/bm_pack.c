#include "bgbmid.h"

BGBMID_PackState *bgbmid_pack_root=NULL;

int bgbmid_strcmp(const char *stra, const char *strb)
{
	const char *sa, *sb;
	
	sa=stra; sb=strb;
	while(*sa && *sb)
	{
		if(*sa!=*sb)break;
		sa++; sb++;
	}
	if(!(*sa) && !(*sb))
		return(0);
	return(((*sa)>(*sb))?(-1):1);
}

int bgbmid_stricmp(const char *stra, const char *strb)
{
	const char *sa, *sb;
	int a, b;
	
	sa=stra; sb=strb;
	while(*sa && *sb)
	{
		a=*sa; b=*sb;
		if((a>='A') && (a<='Z'))a=(a-'A')+'a';
		if((b>='A') && (b<='Z'))b=(b-'A')+'a';
		if(a!=b)break;
		sa++; sb++;
	}
	if(!(*sa) && !(*sb))
		return(0);
	return(((*sa)>(*sb))?(-1):1);
}

s32 bgbmid_get_le32(byte *b)
{
	return(b[0]|(b[1]<<8)|(b[2]<<16)|(b[3]<<24));
}

int bgbmid_get_parsehex(char *str)
{
	char *s;
	int i, j, k;
	s=str; k=0;

	while(*s)
	{
		j=*s++;
		if((j>='0') && (j<='9'))
			{ k=k*16+(j-'0'); continue; }
		if((j>='A') && (j<='F'))
			{ k=k*16+((j-'A')+10); continue; }
		if((j>='a') && (j<='f'))
			{ k=k*16+((j-'a')+10); continue; }
		break;
	}
	return(k);
}

BGBMID_PackState *BGBMID_LookupPack(char *name)
{
	BGBMID_PackState *cur;

	cur=bgbmid_pack_root;
	while(cur)
	{
		if(!bgbmid_stricmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BGBMID_PackState *BGBMID_OpenPack(char *name)
{
	BGBMID_PackState *tmp;

	tmp=BGBMID_LookupPack(name);
	if(tmp)return(tmp);

	tmp=bgbmid_malloc(sizeof(BGBMID_PackState));
	memset(tmp, 0, sizeof(BGBMID_PackState));

	tmp->name=bgbmid_strdup(name);

	tmp->fd=bgbmid_fopen(name, "rb");
	if(!tmp->fd)
	{
		bgbmid_free(tmp);
		return(NULL);
	}

	bgbmid_fread(&tmp->hdr, 1, 12, tmp->fd);

//	tmp->doffs=*(int *)(tmp->hdr.offs);
//	tmp->dents=*(int *)(tmp->hdr.ents);
	tmp->doffs=bgbmid_get_le32(tmp->hdr.offs);
	tmp->dents=bgbmid_get_le32(tmp->hdr.ents);

	tmp->dir=bgbmid_malloc(tmp->dents*sizeof(BGBMID_PackDirEnt));

	bgbmid_fseek(tmp->fd, tmp->doffs, 0);
	bgbmid_fread(tmp->dir, 64, tmp->dents, tmp->fd);

	tmp->next=bgbmid_pack_root;
	bgbmid_pack_root=tmp;

	return(tmp);
}

int BGBMID_LookupPackFile(BGBMID_PackState *ctx, char *name,
	int *roffs, int *rsize)
{
	char tb[256];
	char *s;
	int i, j, k;

//	printf("Check Pack %s for %s\n", ctx->name, name);

	for(i=0; i<ctx->dents; i++)
	{
		s=ctx->dir[i].name;

		if((s[0]=='@') && (s[1]=='@') && (s[2]=='0') && (s[3]=='x'))
		{
			k=bgbmid_get_parsehex(s+4);
			bgbmid_fseek(ctx->fd, k, 0);
			bgbmid_fread(tb, 1, 256, ctx->fd);
			s=tb;
		}

		if(bgbmid_stricmp(s, name))
			continue;

//		if(bgbmid_stricmp(ctx->dir[i].name, name))
//			continue;

//		j=*(int *)(ctx->dir[i].offs);
//		k=*(int *)(ctx->dir[i].size);
		j=bgbmid_get_le32(ctx->dir[i].offs);
		k=bgbmid_get_le32(ctx->dir[i].size);

		*roffs=j; *rsize=k;
		return(1);
	}

//	printf("Not Found %s for %s\n", ctx->name, name);

	return(0);
}

void *BGBMID_PackLookupFile(char *name, int *roffs, int *rsize)
{
	BGBMID_PackState *cur;
	int i;

	cur=bgbmid_pack_root;
	while(cur)
	{
		i=BGBMID_LookupPackFile(cur, name, roffs, rsize);
		if(i>0)return(cur->fd);

		cur=cur->next;
	}
	return(NULL);
}

byte *BGBMID_PackReadFile(char *name, int *rsize)
{
	void *fd;
	byte *buf;
	int offs, size;

	fd=BGBMID_PackLookupFile(name, &offs, &size);
	if(!fd)return(NULL);

//	printf("read %s %10d %d\n", name, offs, size);

	buf=bgbmid_malloc(size+1);
	buf[size]=0;

	bgbmid_fseek(fd, offs, 0);
	bgbmid_fread(buf, 1, size, fd);

//	printf("%c%c%c%c\n", buf[0], buf[1], buf[2], buf[3]);

	if(rsize)*rsize=size;
	return(buf);
}

