/*
Written by Brendan G Bohannon
This tool is placed into the public domain.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int doc, pds, amod, prx;

int symchar(int c) //AH:proxy
{
	if((c>='a') && (c<='z'))return(1);
	if((c>='A') && (c<='Z'))return(1);
	if((c>='0') && (c<='9'))return(1);
	if(c=='_')return(1);
	return(0);
}

char *id_arch()
{
#ifdef __i386__
	return("x86");
#endif
#ifdef __x86_64__
	return("x64");
#endif
#ifdef _M_IX86
	return("x86");
#endif
#ifdef _M_X64
	return("x64");
#endif
#ifdef _M_PPC
	return("ppc");
#endif
	return("unk");
}

char *id_wsuf()
{
#ifdef __i386__
	return("32");
#endif
#ifdef __x86_64__
	return("64");
#endif
#ifdef _M_IX86
	return("32");
#endif
#ifdef _M_X64
	return("64");
#endif
#ifdef _M_PPC
	return("P32");
#endif
#ifdef _M_PPC64
	return("P64");
#endif
	return("");
}

void prx_def(char *buf)
{
	char tb[1024], tbn[1024], tty[1024];
	char *s, *t, *t1, *sa;
	int ind;

	s=buf; t=tb;

#ifdef _MSC_VER
	sprintf(t, "__declspec(dllexport) "); t+=strlen(t);
#endif

	t1=tty;
	while(symchar(*s))*t1++=*s++;
	*t1++=0;
	sprintf(t, "%s", tty); t+=strlen(t);

	while(*s && (*s<=' '))*t++=*s++;

	ind=0;
	while(*s=='*') { *t++=*s++; ind++; }

	t1=tbn;
	while(*s && symchar(*s))*t1++=*s++;
	*t1++=0;

	sprintf(t, "(*_iproxy_%s)", tbn); t+=strlen(t);

	sa=s;
	while(*s)*t++=*s++;
	*t++=0;

	printf("%s;\n", tb);

	t=tb;
	sprintf(tb, "%s\n\t{ ", buf); t+=strlen(t);
	if(strcmp(tty, "void") || ind)
		{ sprintf(t, "return "); t+=strlen(t); }
	sprintf(t, "_iproxy_%s(", tbn); t+=strlen(t);

	s=sa;
	while(s && *s!=')')
	{
		while(*s && (*s!=')') && (*s!=','))s++;
		while(!symchar(*(s-1)))s--;
		while(symchar(*(s-1)))s--;
		if(!strncmp(s, "void)", 5))break;
		while(symchar(*s))*t++=*s++;
		while(*s && (*s!=')') && (*s!=','))s++;
		if(*s==',') { s++; *t++=','; *t++=' '; }
	}

	sprintf(t, "); }\n"); t+=strlen(t);
	printf("%s\n", tb);
}

int main(int argc, char *argv[])
{
	int i, j, l, isapi;
	FILE *fd;
	char *buf, *buf2, *buf3, *buf4;
	char *s, *s2, *t;
	char *cat, *api;

	buf=malloc(16384);
	buf2=malloc(16384);
	buf3=malloc(1024);
	buf4=malloc(1024);

	doc=0; pds=0; amod=0; prx=0;
	api=NULL;

	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-doc"))
			{
				doc=1;
				cat=argv[i+1];
				i++;
				continue;
			}
			if(!strcmp(argv[i], "-pds"))
			{
				pds=1;
				cat=argv[i+1];
				i++;
				continue;
			}

			if(!strcmp(argv[i], "-api"))
			{
				api=argv[i+1];
				i++;
				continue;
			}

			if(!strcmp(argv[i], "-apionly"))
			{
				api=argv[i+1]; amod=1;
				i++;
				continue;
			}

			if(!strcmp(argv[i], "-noapi"))
			{
				api=argv[i+1]; amod=2;
				i++;
				continue;
			}

			if(!strcmp(argv[i], "-proxy"))
			{
				prx=1;
				continue;
			}

			if(!strcmp(argv[i], "-idarch"))
			{
				printf("%s", id_arch());
				exit(0);
			}
			if(!strcmp(argv[i], "-idwsuf"))
			{
				printf("%s", id_wsuf());
				exit(0);
			}

			fprintf(stderr, "Unknown option '%s'\n", argv[i]);
			exit(-1);
		}

		fd=fopen(argv[i], "rt");
		if(!fd)continue;

		printf("//AHSRC:%s\n", argv[i]);
		l=0;
		while(!feof(fd))
		{
			fflush(stdout);
		
			memset(buf, 0, 256);
			fgets(buf, 255, fd);
			l++;

			s=buf;
			while(*s && (*s!='\r') && (*s!='\n'))s++;
			if(*s=='\r')*s=0;
			if(*s=='\n')*s=0;

			s=buf;
			while(*s && (*s<=' '))s++;
			if(!strncmp(s, "//AH:", 5))
			{
//				fprintf(stderr, "//AH tag %s %d\n",
//					argv[i], l);
				s+=5;
				while(*s && (*s<=' '))s++;
				if(!strcmp(s, "skip"))break;
			}

			if(pds)if(!strncmp(s, "//PDSCAT:", 9))
			{
				s+=9;
				t=buf3;
				while(*s)*t++=*s++;
				*t++=0;

				cat=strdup(buf3);
			}

			if(pds)if(!strncmp(s, "//PDS:", 6))
			{
				s+=6;
				while(*s && (*s<=' '))s++;

				t=buf3;
				while(*s && strncmp(s, "::", 2))
				{
					if((s[0]=='\\') && (s[1]<=' '))
					{
						fgets(buf, 255, fd);
						s=buf;
						while(*s && (*s<=' '))s++;
						if(!strncmp(s, "//", 2))s+=2;
						continue;
					}
					if(*s=='\r')break;
					if(*s=='\n')break;
					*t++=*s++;
				}
				*t++=0;

				if(!strncmp(s, "::", 2))s+=2;

				t=buf4;
				while(*s)
				{
					if((s[0]=='\\') && (s[1]<=' '))
					{
						fgets(buf, 255, fd);
						s=buf;
						while(*s && (*s<=' '))s++;
						if(!strncmp(s, "//", 2))s+=2;
						continue;
					}
					if(*s=='\r')break;
					if(*s=='\n')break;
					*t++=*s++;
				}
				*t++=0;

				printf(
					"/*--\n"
					"Cat %s\n"
					"Text\n"
					"\t%s\n"
					"\t%s\n"
					"--*/\n",
					cat, buf3, buf4);
			}

			if(pds)continue;

			s=buf;
			isapi=0;

			if(api)
			{
				if(!strncmp(s, api, strlen(api)))
				{
					if(amod==2)continue;

					s+=strlen(api);
					while(*s && (*s<=' '))s++;
					isapi=1;
				}else
				{
					if(amod==1)continue;
				}
			}

			if(!strncmp(s, "static", strlen("static")))
				continue;

			if(*s<=' ')continue;
			while(symchar(*s))s++;
			while(*s && (*s<=' '))s++;

			while(*s=='*')s++;
			if(!*s || (*s=='('))continue;
			while(*s && symchar(*s))s++;

			if(((*s=='=') || (*s==';')) && isapi)
			{
				//special case: API variables are exported
				t=s;
				while(*s && (*s!='/'))s++;
				if(!strncmp(s, "//AH:", 5))
				{
					s+=5;
					while(*s && (*s<=' '))s++;
					if(!strcmp(s, "ignore"))continue;
				}
				s=t;

				*s=0;
				if(!doc)printf("%s;\n", buf);

				continue;
			}

			if(*s==' ')s++;
			if(*s!='(')continue;

			t=s;
			while(*s && (*s!='/'))s++;
			if(!strncmp(s, "//AH:", 5))
			{
//				fprintf(stderr, "//AH tag %s %d\n",
//					argv[i], l);
				s+=5;
				while(*s && (*s<=' '))s++;
				if(!strcmp(s, "ignore"))continue;
			}
			s=t;

			if(*s=='(')s++;
			j=1;
			while(j)
			{
				if(!*s)
				{
					memset(buf2, 0, 256);
					fgets(buf2, 255, fd);
					l++;

					t=buf2;
					while(*t && (*t<=' '))t++;

					s2=t;
					t=s;
					while(*s2 && (*s2!='\n'))*t++=*s2++;
					*t++=' ';
					*t++=0;
				}else
				{
					if(*s=='(')j++;
					if(*s==')')j--;
					s++;
				}
			}
			t=s;
			while(*s && (*s!='/'))s++;
			if(!strncmp(s, "//AH:", 5))
			{
				fprintf(stderr, "//AH tag %s %d\n",
					argv[i], l);
				s+=5;
				while(*s && (*s<=' '))s++;
				if(!strncmp(s, "ignore", 6))continue;

				if(prx && strncmp(s, "proxy", 5))continue;
				if(!strncmp(s, "proxy", 5))isapi=2;
			}else
			{
				if(prx)continue;
			}
			s=t;

			if(*s==')')s++;
			*s=0;

			if(doc)
			{
				printf(
					"/*--\n"
					"Cat %s\n"
					"Text\n"
					"\t%s;\n"
					"\t%s:%d\n"
					"--*/\n",
					cat, buf, argv[i], l);
			}else
			{
				if(prx) { prx_def(buf); continue; }
				if(isapi==2) printf("/*AHPRX:*/ ", buf);
				printf("%s;\n", buf);
			}
		}
		fclose(fd);
	}
	return(0);
}
