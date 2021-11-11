#include <bgbccc.h>

char *BCCX_PrintText(char *s, char *t, int flag)
{
	int is;

	is=0;
	while(*t)
	{
		switch(*t)
		{
		case '<': strcpy(s, "&lt;"); s+=strlen(s); break;
		case '>': strcpy(s, "&gt;"); s+=strlen(s); break;
		case '&': strcpy(s, "&amp;"); s+=strlen(s); break;
		case '\'': strcpy(s, "&apos;"); s+=strlen(s); break;
		case '"': strcpy(s, "&quot;"); s+=strlen(s); break;

		case '\t': strcpy(s, "&#9;"); s+=strlen(s); break;
		case '\r': strcpy(s, "&#13;"); s+=strlen(s); break;
		case '\n': strcpy(s, "&#10;"); s+=strlen(s); break;

		case ' ':
			if(!is) { strcpy(s, "&#32;"); s+=strlen(s); }
				else *s++=' ';
			is=0;
			break;

		default:
			is=1;
			*s++=*t;
			break;
		}
		t++;
	}
	*s=0;
	return(s);
}

int BCCX_SizeNodeAttr(BCCX_Node *node, int flag)
{
	u16 *attr_n, *an;
	BCCX_AttrVal *attr_v, *av;
	char *s0;
	int sz;
	int i, j, k;
	
//	if(!node->mattr)
	if(!node->malvl)
	{
		attr_n=node->attr_n;
		attr_v=(BCCX_AttrVal *)(node->attr_v);
	}else
	{
		attr_n=NULL;
		attr_v=NULL;
//		attr_n=(u16 *)(node->attr_v[0].s);
//		attr_v=(BCCX_AttrVal *)(node->attr_v[1].s);
	}
	
	sz=0;
	for(i=0; i<node->nattr; i++)
	{
		if(attr_n)
		{
			an=attr_n+i;
			av=attr_v+i;
		}else
		{
			BCCX_FetchAttrArrIx(node, i, &an, &av);
		}
	
//		j=node->attr_n[i];
		j=*an;

		s0=BCCX_StridxToString(j);
		if(*s0=='$')
			continue;

		if((j>>12)==BCCX_IVTY_STRING)
		{
			sz+=strlen(s0)+2;
			sz+=strlen(av->s)+2;
		}
		if((j>>12)==BCCX_IVTY_INT)
		{
			sz+=strlen(s0)+2;
			sz+=16;
		}
		if((j>>12)==BCCX_IVTY_REAL)
		{
			sz+=strlen(s0)+2;
			sz+=16;
		}
	}
	return(sz);
}

char *BCCX_PrintBufNodeAttr(BCCX_Node *node, char *buf, int flag)
{
	u16 *attr_n, *an;
	BCCX_AttrVal *attr_v, *av;
	char *t, *s0;
	int i, j, k;
	
//	if(!node->mattr)
	if(!node->malvl)
	{
		attr_n=node->attr_n;
		attr_v=(BCCX_AttrVal *)(node->attr_v);
	}else
	{
		attr_n=NULL;
		attr_v=NULL;
	}
	
	t=buf;
	for(i=0; i<node->nattr; i++)
	{
		if(attr_n)
		{
			an=attr_n+i;
			av=attr_v+i;
		}else
		{
			BCCX_FetchAttrArrIx(node, i, &an, &av);
		}

		j=*an;

		s0=BCCX_StridxToString(j);
		if(*s0=='$')
			continue;

		if((j>>12)==BCCX_IVTY_STRING)
		{
			*t++=' ';
			sprintf(t, "%s=\"", s0);
			t+=strlen(t);
			t=BCCX_PrintText(t, av->s, flag);
			*t++='\"'; *t=0;
		}
		if((j>>12)==BCCX_IVTY_INT)
		{
			*t++=' ';
			sprintf(t, "%s=%lld", s0, av->i);
			t+=strlen(t);
		}
		if((j>>12)==BCCX_IVTY_REAL)
		{
			*t++=' ';
			sprintf(t, "%s=%f", s0, av->f);
			t+=strlen(t);
		}
	}
	return(t);
}

void BCCX_PrintBufSpecial(BCCX_Node *node, int ind,
	void (*pb)(void *p, char *b), void *ob, int flag)
{
	char buf[4096];
	BCCX_Node *ncur;
//	BCCX_Attr *cur;
	char *tb, *t, *s;
	int i, j, k, sz;

	tb=buf;

//	if(!strcmp(BCCX_Tag(node), "!CDATA"))
	if(!strcmp(BCCX_Tag(node), "$cdata"))
	{
		pb(ob, "<![CDATA[");
//		pb(ob, node->text);
		pb(ob, BCCX_Get(node, "$text"));
		pb(ob, "]]>");
		return;
	}

//	if(!strcmp(BCCX_Tag(node), "!BDATA"))
	if(!strcmp(BCCX_Tag(node), "$bdata"))
	{
		sz=BCCX_GetIntCst(node, &bgbcc_rcst_size, "size");
		
//		s=node->text;
		s=BCCX_GetCst(node, &bgbcc_rcst_Stext, "$text");
		t=tb;
		if((ind<0) || ((ind+16+(sz*3))<76))
		{
			t=tb;
			for(i=0; i<ind; i++)*t++=' ';
			sprintf(t, "<![BDATA["); t+=strlen(t);
			pb(ob, tb);

			for(i=0; i<sz; i++)
			{
				sprintf(tb, "%02X ", ((byte *)s)[i]);
				pb(ob, tb);
			}

			t=tb;
			sprintf(t, "]]>"); t+=strlen(t);
			if(ind>=0)*t++='\n';
			*t++=0;
			pb(ob, tb);
		}else
		{
			t=tb;
			for(i=0; i<ind; i++)*t++=' ';
			sprintf(t, "<![BDATA[\n"); t+=strlen(t);
			pb(ob, tb);

			k=(76-ind)/3;
			if(k>24)k=24;
			else if(k>16)k=16;
			else if(k>12)k=12;
			else if(k>8)k=8;
			else if(k>4)k=4;
			
			for(i=0; i<(sz/k); i++)
			{
				t=tb;
				for(j=0; j<ind; j++)*t++=' ';
				for(j=0; j<k; j++)
				{
					sprintf(t, "%02X ", *(byte *)s++);
					t+=strlen(t);
				}
				*t++='\n';
				*t++=0;
				pb(ob, tb);
			}
			if(sz%k)
			{
				t=tb;
				for(j=0; j<ind; j++)*t++=' ';
				for(i=0; i<(sz%k); i++)
				{
					sprintf(t, "%02X ", *(byte *)s++);
					t+=strlen(t);
				}
				*t++='\n';
				*t++=0;
				pb(ob, tb);
			}
			for(i=0; i<ind; i++)*t++=' ';
			sprintf(t, "]]>\n"); t+=strlen(t);
			pb(ob, tb);
		}
		return;
	}

	i=4;
	if(ind>=0)i+=ind+8;
//	if(node->ns)i+=strlen(node->ns)+1;
	i+=strlen(BCCX_Tag(node));
//	i+=strlen(node->text);
	i+=strlen(BCCX_Get(node, "$text"));

	if(i>=256)tb=malloc(8*i);


	t=tb;

	for(i=0; i<ind; i++)*t++=' ';

	*t++='<';
	sprintf(t, "%s", BCCX_Tag(node)); t+=strlen(t);

	*t++=' ';
	t=BCCX_PrintText(t, BCCX_Get(node, "$text"), flag);

//	if(!node->down)
	if(1)
	{
		*t++='>';
		if(ind>=0)*t++='\n';
		*t=0;

		pb(ob, tb);
		if(tb!=buf)free(tb);
		return;
	}

#if 0
	*t++='[';
	if(ind>=0)*t++='\n';
	*t=0;

	pb(ob, tb);

	i=ind; if(i>=0)i+=2;
	ncur=node->down;
	while(ncur)
	{
		BCCX_PrintBuf(ncur, i, pb, ob);
		ncur=ncur->next;
	}

	t=tb;
	for(i=0; i<ind; i++)*t++=' ';
	*t++=']';
	*t++='>';
	if(ind>=0)*t++='\n';
	*t=0;

	pb(ob, tb);

	if(tb!=buf)free(tb);
	return;
#endif
}

void BCCX_PrintBufNodeAttrNode(BCCX_Node *node, int ind,
	void (*pb)(void *p, char *b), void *ob, int flag)
{
	char tb[256];
	u16 *attr_n, *an;
	BCCX_Node *ncur;
	BCCX_AttrVal *attr_v, *av;
	char *t, *s0, *s1;
	int i, j, k, do_full;
	
//	if(!node->mattr)
	if(!node->malvl)
	{
		attr_n=node->attr_n;
		attr_v=(BCCX_AttrVal *)(node->attr_v);
	}else
	{
		attr_n=NULL;
		attr_v=NULL;
	}
	
//	t=buf;
	for(i=0; i<node->nattr; i++)
	{
		if(attr_n)
		{
			an=attr_n+i;
			av=attr_v+i;
		}else
		{
			BCCX_FetchAttrArrIx(node, i, &an, &av);
		}
	
		j=*an;
		if((j>>12)!=BCCX_IVTY_NODE)
			continue;

		if((j&4095)>=4088)
			continue;

		s0=BCCX_StridxToString(j);

		if(!av->p)
		{
			t=tb;
			for(k=0; k<ind; k++)*t++=' ';
			sprintf(t, "<%s/>", s0);
//			sprintf(t, "%s={}", s0);
			t+=strlen(t);
			if(ind)*t++='\n';
			*t=0;
			pb(ob, tb);
			continue;
		}

		do_full=(flag&1);

		ncur=av->p;

		s1=BCCX_Tag(ncur);
		if(*s1=='$')
			do_full=1;

		t=tb;
		for(k=0; k<ind; k++)*t++=' ';
		if(do_full)
		{
			sprintf(t, "<%s>", s0);
		}else
		{
			sprintf(t, "<%s", s0);
	//		sprintf(t, "%s={", s0);
		}
		t+=strlen(t);
		if(ind)*t++='\n';
		*t=0;
		pb(ob, tb);

		k=(ind>0)?(ind+2):0;

//		ncur=attr_v[i].p;

		BCCX_PrintBuf(ncur, k, pb, ob, flag);

//		while(ncur)
//		{
//			BCCX_PrintBuf(ncur, k, pb, ob);
//			ncur=ncur->next;
//		}

		t=tb;
		for(k=0; k<ind; k++)*t++=' ';
		if(do_full)
		{
			sprintf(t, "</%s>", s0);
		}else
		{
			*t++='/';
			*t++='>';
	//		*t++='}';
			*t=0;
		}

		t+=strlen(t);
		if(ind)*t++='\n';
		*t=0;
		pb(ob, tb);
	}
}

int BCCX_CheckNodeAttrNode(BCCX_Node *node)
{
	u16 *attr_n, *an;
	BCCX_AttrVal *attr_v, *av;
	int i, j, k;
	
//	if(!node->mattr)
	if(!node->malvl)
	{
		attr_n=node->attr_n;
		attr_v=(BCCX_AttrVal *)(node->attr_v);
	}else
	{
		attr_n=NULL;
		attr_v=NULL;
	}
	
	for(i=0; i<node->nattr; i++)
	{
		if(attr_n)
		{
			an=attr_n+i;
			av=attr_v+i;
		}else
		{
			BCCX_FetchAttrArrIx(node, i, &an, &av);
		}
	
		j=*an;

		if((j&4095)>=4088)
			continue;

		if((j>>12)==BCCX_IVTY_NODE)
			return(1);
	}
	return(0);
}

void BCCX_PrintBuf(BCCX_Node *node, int ind,
	void (*pb)(void *p, char *b), void *ob, int flag)
{
	char buf[4096];
	BCCX_Node *ncur;
	char *tb, *t, *s0;
	int i, na, ci, do_full;

	if(!node)
	{
		pb(ob, "<!--NULL-NODE-->");
		return;
	}

	if(node->itag && BCCX_Get(node, "$text"))
	{
		BCCX_PrintBufSpecial(node, ind, pb, ob, flag);
		return;
	}

	if(BCCX_Get(node, "$text"))
	{
		i=strlen(BCCX_Get(node, "$text"));
		if(ind>=0)i+=ind+8;

		tb=buf;
		if(i>256)tb=malloc(2*i);

		t=tb;
		for(i=0; i<ind; i++)*t++=' ';
//		*t++='\"';	*t++='\"';	*t++='\"';
		t=BCCX_PrintText(t, BCCX_Get(node, "$text"), flag);
//		*t++='\"';	*t++='\"';	*t++='\"';
		if(ind>=0)*t++='\n';
		*t=0;

		pb(ob, tb);

		if(tb!=buf)free(tb);
		return;
	}

	if(!node->itag)
	{
		pb(ob, "<!--BAD-NODE-->");
		return;
	}

	tb=buf;

	i=4;
	if(ind>=0)i+=ind+8;
	i+=strlen(BCCX_Tag(node));

	i+=BCCX_SizeNodeAttr(node, flag);

	if(i>=256)tb=malloc(8*i);


	t=tb;

	for(i=0; i<ind; i++)*t++=' ';

	*t++='<';
	sprintf(t, "%s", BCCX_Tag(node)); t+=strlen(t);
//	*t++='{';

	t=BCCX_PrintBufNodeAttr(node, t, flag);

	na=BCCX_GetNodeChildCount(node);

//	if(!node->down && !BCCX_CheckNodeAttrNode(node))
	if((na<=0) && !BCCX_CheckNodeAttrNode(node))
//	if(na<=0)
	{
		*t++='/'; *t++='>';
//		*t++='}';
		if(ind>=0)*t++='\n';
		*t=0;

		pb(ob, tb);
		if(tb!=buf)free(tb);
		return;
	}

	do_full=(flag&1);
	for(ci=0; ci<na; ci++)
	{
		ncur=BCCX_GetNodeIndex(node, ci);
		s0=BCCX_Tag(ncur);
		if(*s0=='$')
		{
			do_full=1;
			break;
		}
	}

	if(do_full)
	{
		*t++='>';
//		*t++=' ';
	}
	if(ind>=0)*t++='\n';
	*t=0;

	pb(ob, tb);

	i=ind;
	if(i>=0)i+=2;

	BCCX_PrintBufNodeAttrNode(node, i, pb, ob, flag);

	for(ci=0; ci<na; ci++)
	{
		ncur=BCCX_GetNodeIndex(node, ci);
		BCCX_PrintBuf(ncur, i, pb, ob, flag);
	}

	t=tb;
	for(i=0; i<ind; i++)*t++=' ';
	if(do_full)
	{
		*t++='<'; *t++='/';
		sprintf(t, "%s", BCCX_Tag(node)); t+=strlen(t);
		*t++='>';
	}else
	{
		*t++='/';
		*t++='>';
	//	*t++='}';
	}

	if(ind>=0)*t++='\n';
	*t=0;

	pb(ob, tb);

	if(tb!=buf)free(tb);
	return;
}

static void bccx_print_fd(void *p, char *b)
{
	fputs(b, (FILE *)p);
}

static void bccx_print_str(void *p, char *b)
{
	char *s, **a;

	a=(char **)p;
	s=*a;
	strcpy(s, b);
	s+=strlen(s);
	*a=s;
}

void BCCX_DumpFD(FILE *fd, BCCX_Node *node)
	{ BCCX_PrintBuf(node, -1, bccx_print_fd, fd, 1); }
void BCCX_PrintFD(FILE *fd, BCCX_Node *node)
	{ BCCX_PrintBuf(node, 0, bccx_print_fd, fd, 1); }

char *BCCX_DumpStr(char *buf, BCCX_Node *node)
{
	char *s;

	s=buf;
	BCCX_PrintBuf(node, -1, bccx_print_str, &s, 1);
	return(s);
}

char *BCCX_PrintStr(char *buf, BCCX_Node *node)
{
	char *s;

	s=buf;
	BCCX_PrintBuf(node, 0, bccx_print_str, &s, 1);
	return(s);
}

void BCCX_Print(BCCX_Node *node)
{
	BCCX_PrintFD(stdout, node);
}

