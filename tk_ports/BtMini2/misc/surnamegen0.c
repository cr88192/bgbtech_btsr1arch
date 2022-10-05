#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *sursfxtab[]={
"",
"e",
"man",
"more",
"son",
"smith",
"field",
"fan",
"ton",
"el",
"ly",
"ey",
"show",
"tree",
NULL
};

char *surpfxtab[]={
"Gold",
"Silver",
"Iron",
"Feld",

"Han",
"Evan",
"John",
"Ron",
"Bonne",

"Hawk",
"Wolf",
"Crow",
"Puma",
"Frog",

"Bus",
"Ship",
"Road",

"Rock",
"Stone",
"Roll",

"Brake",
"Gas",
"Steer",
"Turn",
"Dig",

"Sword",
"Hoe",
"Shovel",
"Wheel",
"Spear",

"Jet",
"Boat",
"Space",

"Book",
"Hand",
"Foot",

"Ghost",
"Skele",
"Grave",

"Light",
"Dark",

"Whit",
// "Wily",
"Wi",

"Old",
"Corn",
"Whisk",
"Wine",

"Oak",
"Pine",
"Spruce",
"Maple",

"Barrel",
"Casket",

"Spam",

"Dog",
"Cat",
"Mouse",

"Gray",
"White",
"Black",
"Green",
"Yellow",
NULL
};

int sursfxtabsz=(sizeof(sursfxtab)/sizeof(char *))-1;
int surpfxtabsz=(sizeof(surpfxtab)/sizeof(char *))-1;


char *firsfxtab_fem[]={
"a",
// "ah",
"anne",
"marie",
"ella",
"elia",
"lette",
"lina",
"liza",
"ica",
"ina",
"sol",
"yssa",
"issa",
"lene",
"ily",
"ilie",
// "mi",
NULL
};

char *firsfxtab_mas[]={
"o",
"oe",
"no",
"dan",
"don",
"ol",
"on",
"om",
"mo",
"ino",
"in",
"imo",
"en",
// "em",
"isel",
NULL
};

char *firpfxtab[]={
"Dan",
"Dani",
"Jan",
"Sab",
"Jon",
"Jo",
"So",
"Emi",
"Kam",
"Kami",
"Mari",
"Mali",
"Meli",
"Bri",
"Teri",
"Ari",
"Aman",
"Amani",
"Lu",
"Li",
"Ri",
"Tei",
"Cru",
"Cri",
"Col",
// "Hito",
"Am",
"Om",
"Um",
"Em",
//"Emil",
"Lion",
"Er",
"Ar",
"Al",
"Ty",
"My",
"Tyr",
"Gry",
"Gwy",
"Gru",
// "Arr",
NULL
};

int firsfxtab_fem_sz=(sizeof(firsfxtab_fem)/sizeof(char *))-1;
int firsfxtab_mas_sz=(sizeof(firsfxtab_mas)/sizeof(char *))-1;
int firpfxtabsz=(sizeof(firpfxtab)/sizeof(char *))-1;

int NameCharConsonantHard(int ch)
{
	int rt;

	switch(ch)
	{
		case 'b': rt=1;		break;
		case 'd': rt=1;		break;
		case 'f': rt=1;		break;
		case 'g': rt=1;		break;
		case 'j': rt=1;		break;
		case 'k': rt=1;		break;

		case 'p': rt=1;		break;
		case 'q': rt=1;		break;
		case 'r': rt=1;		break;

		case 's': rt=1;		break;
		case 't': rt=1;		break;

		case 'v': rt=1;		break;
		case 'x': rt=1;		break;
		case 'z': rt=1;		break;

		default: rt=0;		break;
	}
	
	return(rt);
}

int NameCheckEatSfxChar(char *pfx, char *sfx)
{
	int ch0, ch1;

	ch0=pfx[strlen(pfx)-1];
	ch1=sfx[0];

	if(ch0==ch1)
		return(1);

	if((ch0=='i') && (ch1=='y'))
		return(1);

	return(0);
}

int NameCheckReject(char *pfx, char *sfx)
{
	int ch0, ch1;

	ch0=pfx[strlen(pfx)-1];
	ch1=sfx[0];

	if((ch0=='s') || (ch0=='r') ||
		(ch0=='m') || (ch0=='n') ||
		(ch0=='l'))
	{
		if(NameCharConsonantHard(ch1))
			return(1);
	}
	
	if((ch0=='s') || (ch0=='r'))
	{
		if((ch1=='m') || (ch1=='n'))
			return(1);
		if(ch1=='l')
			return(0);
	}

	if(NameCharConsonantHard(ch0))
	{
		if(NameCharConsonantHard(ch1))
			return(1);

		if((ch1=='m') || (ch1=='n') || (ch1=='l'))
			return(1);
		return(1);
	}

	return(0);
}

char *NameGenerateLast()
{
	char tb[128];
	char *s0, *s1;
	int j, k;

//	j=rand()%surpfxtabsz;
//	k=rand()%sursfxtabsz;	
//	s0=surpfxtab[j];
//	s1=sursfxtab[k];

	while(1)
	{
		j=rand()%surpfxtabsz;
		k=rand()%sursfxtabsz;
		
		s0=surpfxtab[j];
		s1=sursfxtab[k];
		
		if(NameCheckEatSfxChar(s0, s1))
			s1++;
		
		if((strlen(s0)+strlen(s1))<6)
			continue;
			
//		if(NameCheckReject(s0, s1))
//			continue;
		
		break;
	}
	
	sprintf(tb, "%s%s", s0, s1);
	return(strdup(tb));

//	sprintf(tb, "%s%s", surpfxtab[j], sursfxtab[k]);
//	return(strdup(tb));
}

char *NameGenerateFirstFem()
{
	char tb[128];
	char *s0, *s1;
	int j, k;

	while(1)
	{
		j=rand()%firpfxtabsz;
		k=rand()%firsfxtab_fem_sz;
		
		s0=firpfxtab[j];
		s1=firsfxtab_fem[k];
		
		if(NameCheckEatSfxChar(s0, s1))
			s1++;	
			
		if(NameCheckReject(s0, s1))
			continue;
		
		break;
	}
	
	sprintf(tb, "%s%s", s0, s1);
	return(strdup(tb));
}

char *NameGenerateFirstMas()
{
	char tb[128];
	char *s0, *s1;
	int j, k;

	while(1)
	{
		j=rand()%firpfxtabsz;
		k=rand()%firsfxtab_mas_sz;
		
		s0=firpfxtab[j];
		s1=firsfxtab_mas[k];
		
		if(NameCheckEatSfxChar(s0, s1))
			s1++;	
			
		if(NameCheckReject(s0, s1))
			continue;
		
		break;
	}
	
	sprintf(tb, "%s%s", s0, s1);
	return(strdup(tb));
}

int main()
{
	char *s0, *s1;
	int i, j, k;

	for(i=0; i<100; i++)
	{
		s0=NameGenerateFirstFem();
		s1=NameGenerateLast();
		
		printf("%-12s %-12s\n", s0, s1);
	}
	printf("\n");

	for(i=0; i<100; i++)
	{
		s0=NameGenerateFirstMas();
		s1=NameGenerateLast();
		
		printf("%-12s %-12s\n", s0, s1);
	}

#if 0
	for(i=0; i<100; i++)
	{
		j=rand()%surpfxtabsz;
		k=rand()%sursfxtabsz;
		
		printf("%s%s\n", surpfxtab[j], sursfxtab[k]);
	}
	
	printf("\n");
	
	for(i=0; i<100; i++)
	{
		j=rand()%firpfxtabsz;
		k=rand()%firsfxtab_fem_sz;
		
		s0=firpfxtab[j];
		s1=firsfxtab_fem[k];
//		if(s0[strlen(s0)-1]==s1[0])
//			s1++;
		
		if(NameCheckEatSfxChar(s0, s1))
			s1++;
		
		
			
		if(NameCheckReject(s0, s1))
		{
			i--;
			continue;
		}
		
		printf("%s%s\n", s0, s1);
	}
#endif

}

