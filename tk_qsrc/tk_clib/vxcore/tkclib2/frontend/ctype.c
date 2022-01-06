#include <ctype.h>

#define ctype_chkrange(ch, m, n)	(((unsigned int)((ch)-(m)))<=((n)-(m)))

int isalnum(int c)
{
	if(ctype_chkrange(c, 'A', 'Z'))
		return(1);
	if(ctype_chkrange(c, 'a', 'z'))
		return(1);
	if(ctype_chkrange(c, '0', '9'))
		return(1);
	return(0);
}

int isalpha(int c)
{
	if(ctype_chkrange(c, 'A', 'Z'))
		return(1);
	if(ctype_chkrange(c, 'a', 'z'))
		return(1);
	return(0);
}

int iscntrl(int c)
{
	if(ctype_chkrange(c, 0, 31))
		return(1);
	return(0);
}

int isdigit(int c)
{
	if(ctype_chkrange(c, '0', '9'))
		return(1);
	return(0);
}

int isgraph(int c)
{
	if(ctype_chkrange(c, 33, 126))
		return(1);
	return(0);
}

int islower(int c)
{
	if(ctype_chkrange(c, 'a', 'z'))
		return(1);
	return(0);
}

int isprint(int c)
{
	if(ctype_chkrange(c, 32, 126))
		return(1);
	return(0);
}

int ispunct(int c)
{
	if(ctype_chkrange(c, 'A', 'Z'))
		return(0);
	if(ctype_chkrange(c, 'a', 'z'))
		return(0);
	if(ctype_chkrange(c, '0', '9'))
		return(0);

	if(ctype_chkrange(c, 33, 126))
		return(1);
	return(0);
}

int isspace(int c)
{
	if(		(c==' ') || (c=='\n') ||
			(c=='\t') || (c=='\r') ||
			(c=='\f') || (c=='\v')	)
		return(1);
	return(0);
}

int isupper(int c)
{
	if(ctype_chkrange(c, 'A', 'Z'))
		return(1);
	return(0);
}

int isxdigit(int c)
{
	if(ctype_chkrange(c, '0', '9'))
		return(1);
	if(ctype_chkrange(c, 'A', 'F'))
		return(1);
	if(ctype_chkrange(c, 'a', 'f'))
		return(1);
	return(0);
}

#ifdef __BJX2__

int tolower(int c);
int toupper(int c);

__asm {
tolower:
	MOV		R4, R2
	SUB		R4, 0x41, R3
	CMPHI	25, R3
	ADD?F	32, R2
	RTS

toupper:
	MOV		R4, R2
	SUB		R4, 0x61, R3
	CMPHI	25, R3
	ADD?F	-32, R2
	RTS
};

#else

int tolower(int c)
{
	if(ctype_chkrange(c, 'A', 'Z'))
		return(c+('a'-'A'));
	return(c);
}

int toupper(int c)
{
	if(ctype_chkrange(c, 'a', 'z'))
		return(c+('A'-'a'));
	return(c);
}
#endif
