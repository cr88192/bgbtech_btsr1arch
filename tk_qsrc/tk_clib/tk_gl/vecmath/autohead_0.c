#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int doc, pds;

int symchar(int c)
{
	if((c>='a') && (c<='z'))return(1);
	if((c>='A') && (c<='Z'))return(1);
	if((c>='0') && (c<='9'))return(1);
	if(c=='_')return(1);
	return(0);
}

int main(int argc, char *argv[])
{
	int i, j, l;
	FILE *fd;
	char *buf, *buf2, *buf3, *buf4;
	char *s, *s2, *t;
	char *cat, *api;

	buf=malloc(16384);
	buf2=malloc(16384);
	buf3=malloc(256);
	buf4=malloc(256);

	doc=0;
	pds=0;
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

			fprintf(stderr, "Unknown option '%s'\n", argv[i]);
			exit(-1);
		}

		fd=fopen(argv[i], "rt");
		printf("//%s\n", argv[i]);
		l=0;
		while(!feof(fd))
		{
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

			if(api)
			{
				if(!strncmp(s, api, strlen(api)))
				{
					s+=strlen(api);
					while(*s && (*s<=' '))s++;
				}
			}

			if(*s<=' ')continue;
			while(symchar(*s))s++;
			while(*s && (*s<=' '))s++;


			while(*s=='*')s++;
			if(!*s || (*s=='('))continue;
			while(*s && symchar(*s))s++;
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
//				if(!strncmp(buf, "BSEXP", 5))
//					{ buf[2]='A'; buf[3]='P'; buf[4]='I'; }
				
				printf("%s;\n", buf);
			}
		}
		fclose(fd);
	}
	return(0);
}
