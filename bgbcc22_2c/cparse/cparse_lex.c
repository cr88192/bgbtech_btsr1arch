#include <bgbccc.h>

char **BGBCP_LexBuffer(char *str)
{
	char b[256], b2[256];
	char **tkarr, **tkc, **tke;
	char *s, *s1, *s2;
	int i, j, ty, ty2, fl, cty;

	s=*str;
	tkarr=malloc(16*sizeof(char *));
	tkc=tkarr; tke=tkarr+16;

	while(*s)
	{
		if((tkc+4)>=tke)
		{
			i=tkc-tkarr; j=tke-tkarr; j=j+(j>>1);
			tkarr=realloc(tkarr, j*sizeof(char *));
			tkc=tkarr+i; tke=tkarr+j;
		}

		s=BGBCP_EatWhiteOnly2(s);
		if(*s=='\n')
		{
			b2[0]=1;
			b2[1]=1;
			b2[2]=1;
			b2[3]=BTK_BREAK;

			strcpy(b2+4, "\n");
			s1=bgbcc_strdup(b2);
			*tkc++=s1+4;
			
			s++;
			continue;
		}
		
		s=BGBCP_Token(s, b, &ty);
		b2[0]=ty;
		strcpy(b2+1, b);
		s1=bgbcc_strdup(b2);
		*tkc++=s1+1;
	}

	*tkc++=NULL;
	
	return(tkarr);
}

