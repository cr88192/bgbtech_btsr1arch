/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "quakedef.h"


/*

*/

typedef struct
{
	int				s;
	dfunction_t		*f;
} prstack_t;

#define	MAX_STACK_DEPTH		32
prstack_t	pr_stack[MAX_STACK_DEPTH];
int			pr_depth;

#define	LOCALSTACK_SIZE		2048
int			localstack[LOCALSTACK_SIZE];
int			localstack_used;


qboolean	pr_trace;
dfunction_t	*pr_xfunction;
int			pr_xstatement;


int		pr_argc;

char *pr_opnames[] =
{
"DONE",

"MUL_F",
"MUL_V", 
"MUL_FV",
"MUL_VF",
 
"DIV",

"ADD_F",
"ADD_V", 
  
"SUB_F",
"SUB_V",

"EQ_F",
"EQ_V",
"EQ_S", 
"EQ_E",
"EQ_FNC",
 
"NE_F",
"NE_V", 
"NE_S",
"NE_E", 
"NE_FNC",
 
"LE",
"GE",
"LT",
"GT", 

#if 0
"INDIRECT",
"INDIRECT",
"INDIRECT", 
"INDIRECT", 
"INDIRECT",
"INDIRECT", 
#endif

#if 1
"LOAD_F",
"LOAD_V",
"LOAD_S", 
"LOAD_ENT", 
"LOAD_FLD",
"LOAD_FNC", 
#endif

"ADDRESS", 

"STORE_F",
"STORE_V",
"STORE_S",
"STORE_ENT",
"STORE_FLD",
"STORE_FNC",

"STOREP_F",
"STOREP_V",
"STOREP_S",
"STOREP_ENT",
"STOREP_FLD",
"STOREP_FNC",

"RETURN",
  
"NOT_F",
"NOT_V",
"NOT_S", 
"NOT_ENT", 
"NOT_FNC", 
  
"IF",
"IFNOT",
  
"CALL0",
"CALL1",
"CALL2",
"CALL3",
"CALL4",
"CALL5",
"CALL6",
"CALL7",
"CALL8",
  
"STATE",
  
"GOTO", 
  
"AND",
"OR", 

"BITAND",
"BITOR"
};

char *PR_GlobalString (int ofs);
char *PR_GlobalStringNoContents (int ofs);


//=============================================================================

/*
=================
PR_PrintStatement
=================
*/
void PR_PrintStatement (dstatement_t *s)
{
	int		i;
	
	if ( (unsigned)s->op < sizeof(pr_opnames)/sizeof(pr_opnames[0]))
	{
		Con_Printf ("%s ",  pr_opnames[s->op]);
		i = strlen(pr_opnames[s->op]);
		for ( ; i<10 ; i++)
			Con_Printf (" ");
	}
		
	if (s->op == OP_IF || s->op == OP_IFNOT)
		Con_Printf ("%sbranch %i",PR_GlobalString(s->a),s->b);
	else if (s->op == OP_GOTO)
	{
		Con_Printf ("branch %i",s->a);
	}
	else if ( (unsigned)(s->op - OP_STORE_F) < 6)
	{
		Con_Printf ("%s",PR_GlobalString(s->a));
		Con_Printf ("%s", PR_GlobalStringNoContents(s->b));
	}
	else
	{
		if (s->a)
			Con_Printf ("%s",PR_GlobalString(s->a));
		if (s->b)
			Con_Printf ("%s",PR_GlobalString(s->b));
		if (s->c)
			Con_Printf ("%s", PR_GlobalStringNoContents(s->c));
	}
	Con_Printf ("\n");
}

/*
============
PR_StackTrace
============
*/
void PR_StackTrace (void)
{
	dfunction_t	*f;
	int			i;
	
	if (pr_depth == 0)
	{
		Con_Printf ("<NO STACK>\n");
		return;
	}
	
	pr_stack[pr_depth].f = pr_xfunction;
	for (i=pr_depth ; i>=0 ; i--)
	{
		f = pr_stack[i].f;
		
		if (!f)
		{
			Con_Printf ("<NO FUNCTION>\n");
		}
		else
		{
			Con_Printf ("%12s : %s\n", pr_strings + f->s_file,
				pr_strings + f->s_name);
		}
	}
}


/*
============
PR_Profile_f

============
*/
void PR_Profile_f (void)
{
	dfunction_t	*f, *best;
	int			max;
	int			num;
	int			i;
	
	num = 0;	
	do
	{
		max = 0;
		best = NULL;
		for (i=0 ; i<progs->numfunctions ; i++)
		{
			f = &pr_functions[i];
			if (f->profile > max)
			{
				max = f->profile;
				best = f;
			}
		}
		if (best)
		{
			if (num < 10)
				Con_Printf ("%7i %s\n", best->profile, pr_strings+best->s_name);
			num++;
			best->profile = 0;
		}
	} while (best);
}


/*
============
PR_RunError

Aborts the currently executing function
============
*/
void PR_RunError (char *error, ...)
{
	va_list		argptr;
	char		string[1024];

	va_start (argptr,error);
	vsprintf (string,error,argptr);
	va_end (argptr);

	PR_PrintStatement (pr_statements + pr_xstatement);
	PR_StackTrace ();
	Con_Printf ("%s\n", string);
	
	pr_depth = 0;		// dump the stack so host_error can shutdown functions

	Host_Error ("Program error");
}

/*
============================================================================
PR_ExecuteProgram

The interpretation main loop
============================================================================
*/

/*
====================
PR_EnterFunction

Returns the new program statement counter
====================
*/
int PR_EnterFunction (dfunction_t *f)
{
	int		i, j, c, o;

	if(!strcmp(f->s_name, "T_MissileTouch"))
		pr_trace=true;

	pr_stack[pr_depth].s = pr_xstatement;
	pr_stack[pr_depth].f = pr_xfunction;	
	pr_depth++;
	if (pr_depth >= MAX_STACK_DEPTH)
		PR_RunError ("stack overflow");

// save off any locals that the new function steps on
	c = f->locals;
	if (localstack_used + c > LOCALSTACK_SIZE)
		PR_RunError ("PR_ExecuteProgram: locals stack overflow\n");

	for (i=0 ; i < c ; i++)
		localstack[localstack_used+i] = ((int *)pr_globals)[f->parm_start + i];
	localstack_used += c;

// copy parameters
	o = f->parm_start;
	for (i=0 ; i<f->numparms ; i++)
	{
		for (j=0 ; j<f->parm_size[i] ; j++)
		{
			((int *)pr_globals)[o] = ((int *)pr_globals)[OFS_PARM0+i*3+j];
			o++;
		}
	}

	pr_xfunction = f;
	return f->first_statement - 1;	// offset the s++
}

/*
====================
PR_LeaveFunction
====================
*/
int PR_LeaveFunction (void)
{
	int		i, c;

	if (pr_depth <= 0)
		Sys_Error ("prog stack underflow");

// restore locals from the stack
	c = pr_xfunction->locals;
	localstack_used -= c;
	if (localstack_used < 0)
		PR_RunError ("PR_ExecuteProgram: locals stack underflow\n");

	for (i=0 ; i < c ; i++)
		((int *)pr_globals)[pr_xfunction->parm_start + i] =
			localstack[localstack_used+i];

// up stack
	pr_depth--;
	pr_xfunction = pr_stack[pr_depth].f;
	return pr_stack[pr_depth].s;
}


/*
====================
PR_ExecuteProgram
====================
*/
void PR_ExecuteProgram (func_t fnum)
{
	eval_t	*a, *b, *c, *a2;
	int			s;
	dstatement_t	*st;
	dfunction_t	*f, *newf;
	ddef_t	*key;
	int		runaway;
	int		i;
	edict_t	*ed;
	int		exitdepth;
	eval_t	*ptr;

//	tk_puts("PR_ExecuteProgram: A0\n");

	if (!fnum || fnum >= progs->numfunctions)
	{
		if (pr_global_struct->self)
			ED_Print (PROG_TO_EDICT(pr_global_struct->self));
		Host_Error ("PR_ExecuteProgram: NULL function");
	}

#if 0
	key = ED_FindGlobal ("intermission_running");
	if(key)
	{
//		key->_float=0;
		ED_ParseEpair ((void *)pr_globals, key, "0");
	}
#endif

//	tk_puts("PR_ExecuteProgram: A1\n");
	
	f = &pr_functions[fnum];

	runaway = 100000;
	pr_trace = false;
//	pr_trace = true;

// make a stack frame
	exitdepth = pr_depth;

//	tk_puts("PR_ExecuteProgram: A2\n");

	s = PR_EnterFunction (f);

//	tk_puts("PR_ExecuteProgram: A3\n");

while (1)
{
//	tk_puts("PR_ExecuteProgram: B0\n");

	s++;	// next statement

	if(s >= progs->numstatements)
		__debugbreak();

	st = &pr_statements[s];
//	a = (eval_t *)&pr_globals[st->a];
//	b = (eval_t *)&pr_globals[st->b];
//	c = (eval_t *)&pr_globals[st->c];
	a = (eval_t *)(pr_globals+st->a);
	b = (eval_t *)(pr_globals+st->b);
	c = (eval_t *)(pr_globals+st->c);

	if(st->a >= progs->numglobals)
		__debugbreak();
	if(st->b >= progs->numglobals)
		__debugbreak();
	if(st->c >= progs->numglobals)
		__debugbreak();

//	tk_printf("PR_ExecuteProgram: stmt=%d op=%d(%s)"
//		" a=%d b=%d c=%d\n", s, st->op, pr_opnames[st->op],
//		st->a, st->b, st->c);

//	tk_printf("PR_ExecuteProgram: a=%d b=%d c=%d\n", st->a, st->b, st->c);
//	tk_printf("PR_ExecuteProgram: gbls=%p a=%p b=%p c=%p\n",
//		pr_globals, a, b, c);
//	tk_printf("PR_ExecuteProgram: a.i=%08X b.i=%08X c.i=%08X\n",
//		a->_int, b->_int, c->_int);

//	tk_puts("PR_ExecuteProgram: B1\n");

	if (!--runaway)
		PR_RunError ("runaway loop error");
		
	pr_xfunction->profile++;
	pr_xstatement = s;

//	tk_puts("PR_ExecuteProgram: B2\n");

	if (pr_trace)
		PR_PrintStatement (st);

//	tk_puts("PR_ExecuteProgram: B3\n");
//	tk_printf("PR_ExecuteProgram: B3 stmt=%d op=%d %s\n", s, st->op,
//		pr_opnames[st->op]);

	switch (st->op)
	{
	case OP_ADD_F:
//		tk_puts("PR_ExecuteProgram: OP_ADD_F\n");
		c->_float = a->_float + b->_float;
		break;
	case OP_ADD_V:
//		tk_puts("PR_ExecuteProgram: OP_ADD_V\n");
		c->vector[0] = a->vector[0] + b->vector[0];
		c->vector[1] = a->vector[1] + b->vector[1];
		c->vector[2] = a->vector[2] + b->vector[2];
		break;
		
	case OP_SUB_F:
//		tk_puts("PR_ExecuteProgram: OP_SUB_F\n");
		c->_float = a->_float - b->_float;
		break;
	case OP_SUB_V:
//		tk_puts("PR_ExecuteProgram: OP_SUB_V\n");
		c->vector[0] = a->vector[0] - b->vector[0];
		c->vector[1] = a->vector[1] - b->vector[1];
		c->vector[2] = a->vector[2] - b->vector[2];
		break;

	case OP_MUL_F:
//		tk_puts("PR_ExecuteProgram: OP_MUL_F\n");
		c->_float = a->_float * b->_float;
		break;
	case OP_MUL_V:
//		tk_puts("PR_ExecuteProgram: OP_MUL_V\n");
		c->_float = a->vector[0]*b->vector[0]
				+ a->vector[1]*b->vector[1]
				+ a->vector[2]*b->vector[2];
		break;
	case OP_MUL_FV:
//		tk_puts("PR_ExecuteProgram: OP_MUL_FV\n");
		c->vector[0] = a->_float * b->vector[0];
		c->vector[1] = a->_float * b->vector[1];
		c->vector[2] = a->_float * b->vector[2];
		break;
	case OP_MUL_VF:
//		tk_puts("PR_ExecuteProgram: OP_MUL_VF\n");
		c->vector[0] = b->_float * a->vector[0];
		c->vector[1] = b->_float * a->vector[1];
		c->vector[2] = b->_float * a->vector[2];
		break;

	case OP_DIV_F:
//		tk_puts("PR_ExecuteProgram: OP_DIV_F\n");
		c->_float = a->_float / b->_float;
		break;
	
	case OP_BITAND:
//		tk_puts("PR_ExecuteProgram: OP_BITAND_F\n");
		c->_float = (int)(a->_float) & (int)(b->_float);
		break;
	
	case OP_BITOR:
//		tk_puts("PR_ExecuteProgram: OP_BITOR_F\n");
		c->_float = (int)(a->_float) | (int)(b->_float);
		break;
	
		
	case OP_GE:
//		tk_puts("PR_ExecuteProgram: OP_GE\n");
		c->_float = a->_float >= b->_float;
		break;
	case OP_LE:
//		tk_puts("PR_ExecuteProgram: OP_LE\n");
		c->_float = a->_float <= b->_float;
		break;
	case OP_GT:
//		tk_puts("PR_ExecuteProgram: OP_GT\n");
		c->_float = a->_float > b->_float;
		break;
	case OP_LT:
//		tk_puts("PR_ExecuteProgram: OP_LT\n");
		c->_float = a->_float < b->_float;
		break;
	case OP_AND:
//		tk_puts("PR_ExecuteProgram: OP_AND\n");
//		c->_float = a->_float && b->_float;
		i = a->_float && b->_float;
		c->_float = i;
		break;
	case OP_OR:
//		tk_puts("PR_ExecuteProgram: OP_OR\n");
		i = a->_float || b->_float;
		c->_float = i;
//		c->_float = a->_float || b->_float;
		break;
		
	case OP_NOT_F:
//		tk_puts("PR_ExecuteProgram: OP_NOT_F\n");
		c->_float = !a->_float;
		break;
	case OP_NOT_V:
//		tk_puts("PR_ExecuteProgram: OP_NOT_V\n");
//		c->_float = !a->vector[0] && !a->vector[1] && !a->vector[2];
		i = !a->vector[0] && !a->vector[1] && !a->vector[2];
		c->_float = i;
		break;
	case OP_NOT_S:
//		tk_puts("PR_ExecuteProgram: OP_NOT_S\n");
		c->_float = !a->string || !pr_strings[a->string];
		break;
	case OP_NOT_FNC:
//		tk_puts("PR_ExecuteProgram: OP_FNC\n");
		c->_float = !a->function;
		break;
	case OP_NOT_ENT:
//		tk_puts("PR_ExecuteProgram: OP_ENT\n");
		c->_float = (PROG_TO_EDICT(a->edict) == sv.edicts);
		break;

	case OP_EQ_F:
//		tk_puts("PR_ExecuteProgram: OP_EQ_F\n");
		c->_float = a->_float == b->_float;
		break;
	case OP_EQ_V:
//		tk_puts("PR_ExecuteProgram: OP_EQ_V\n");
//		c->_float = (a->vector[0] == b->vector[0]) &&
//					(a->vector[1] == b->vector[1]) &&
//					(a->vector[2] == b->vector[2]);
		i = (a->vector[0] == b->vector[0]) &&
			(a->vector[1] == b->vector[1]) &&
			(a->vector[2] == b->vector[2]);
		c->_float = i;
		break;
	case OP_EQ_S:
//		tk_puts("PR_ExecuteProgram: OP_EQ_S\n");
//		if((a->string>>16) || (b->string>>16))
//			{ c->_float=-1; break; }
		c->_float = !strcmp(pr_strings+a->string,pr_strings+b->string);
		break;
	case OP_EQ_E:
//		tk_puts("PR_ExecuteProgram: OP_EQ_E\n");
		c->_float = a->_int == b->_int;
//		__debugbreak();
		break;
	case OP_EQ_FNC:
//		tk_puts("PR_ExecuteProgram: OP_EQ_FNC\n");
		c->_float = a->function == b->function;
		break;


	case OP_NE_F:
//		tk_puts("PR_ExecuteProgram: OP_NE_F\n");
		c->_float = a->_float != b->_float;
		break;
	case OP_NE_V:
//		tk_puts("PR_ExecuteProgram: OP_NE_V\n");
//		c->_float = (a->vector[0] != b->vector[0]) ||
//					(a->vector[1] != b->vector[1]) ||
//					(a->vector[2] != b->vector[2]);
		i =	(a->vector[0] != b->vector[0]) ||
			(a->vector[1] != b->vector[1]) ||
			(a->vector[2] != b->vector[2]);
		c->_float = i;
		break;
	case OP_NE_S:
//		tk_puts("PR_ExecuteProgram: OP_NE_S\n");
//		c->_float = strcmp(pr_strings+a->string,pr_strings+b->string);
		i = strcmp(pr_strings+a->string,pr_strings+b->string);
		c->_float = i;
		break;
	case OP_NE_E:
//		tk_puts("PR_ExecuteProgram: OP_NE_E\n");
		c->_float = a->_int != b->_int;
		break;
	case OP_NE_FNC:
//		tk_puts("PR_ExecuteProgram: OP_NE_FNC\n");
		c->_float = a->function != b->function;
		break;

//==================
	case OP_STORE_F:
	case OP_STORE_ENT:
	case OP_STORE_FLD:		// integers
	case OP_STORE_S:
	case OP_STORE_FNC:		// pointers
//		tk_puts("PR_ExecuteProgram: OP_STORE_F\n");
		b->_int = a->_int;
		break;
	case OP_STORE_V:
//		tk_puts("PR_ExecuteProgram: OP_STORE_V\n");
//		b->vector[0] = a->vector[0];
//		b->vector[1] = a->vector[1];
//		b->vector[2] = a->vector[2];
		memcpy(b->vector, a->vector, 3*sizeof(float));
		break;
		
	case OP_STOREP_F:
	case OP_STOREP_ENT:
	case OP_STOREP_FLD:		// integers
	case OP_STOREP_S:
	case OP_STOREP_FNC:		// pointers
//		tk_puts("PR_ExecuteProgram: OP_STOREP_F\n");
//		ptr = (eval_t *)((byte *)sv.edicts + b->_int);
		ptr = (eval_t *)(((byte *)sv.edicts) + b->_int);
		ptr->_int = a->_int;
		break;
	case OP_STOREP_V:
//		tk_puts("PR_ExecuteProgram: OP_STOREP_V\n");
//		ptr = (eval_t *)((byte *)sv.edicts + b->_int);
		ptr = (eval_t *)(((byte *)sv.edicts) + b->_int);
//		ptr->vector[0] = a->vector[0];
//		ptr->vector[1] = a->vector[1];
//		ptr->vector[2] = a->vector[2];
		memcpy(ptr->vector, a->vector, 3*sizeof(float));
		break;
		
	case OP_ADDRESS:
//		tk_puts("PR_ExecuteProgram: OP_ADDRESS\n");
		ed = PROG_TO_EDICT(a->edict);
#ifdef PARANOID
		NUM_FOR_EDICT(ed);		// make sure it's in range
#endif
//		if (ed == (edict_t *)sv.edicts && sv.state == ss_active)
		if ((ed == (edict_t *)sv.edicts) && (sv.state == ss_active))
			PR_RunError ("assignment to world entity");
//		c->_int = (byte *)((int *)&ed->v + b->_int) - (byte *)sv.edicts;
		c->_int = (byte *)(((int *)(&ed->v)) + b->_int) - (byte *)sv.edicts;
		break;
		
	case OP_LOAD_F:
	case OP_LOAD_FLD:
	case OP_LOAD_ENT:
	case OP_LOAD_S:
	case OP_LOAD_FNC:
//		tk_puts("PR_ExecuteProgram: OP_LOAD_F\n");
		ed = PROG_TO_EDICT(a->edict);
#ifdef PARANOID
		NUM_FOR_EDICT(ed);		// make sure it's in range
#endif

//		a = (eval_t *)((int *)&ed->v + b->_int);
		a2 = (eval_t *)(((int *)(&ed->v)) + b->_int);

//		printf("ed=%d key=%d val=%d\n", a->edict, b->_int, a2->_int);

//		c->_int = a->_int;
		c->_int = a2->_int;
		break;

	case OP_LOAD_V:
//		tk_puts("PR_ExecuteProgram: OP_LOAD_V\n");
		ed = PROG_TO_EDICT(a->edict);
#ifdef PARANOID
		NUM_FOR_EDICT(ed);		// make sure it's in range
#endif
//		a = (eval_t *)((int *)&ed->v + b->_int);
		a = (eval_t *)(((int *)(&ed->v)) + b->_int);
//		c->vector[0] = a->vector[0];
//		c->vector[1] = a->vector[1];
//		c->vector[2] = a->vector[2];
		memcpy(c->vector, a->vector, 3*sizeof(float));
		break;
		
//==================

	case OP_IFNOT:
//		tk_puts("PR_ExecuteProgram: OP_IFNOT\n");
		if (!a->_int)
			s += st->b - 1;	// offset the s++
		break;
		
	case OP_IF:
//		tk_puts("PR_ExecuteProgram: OP_IF\n");
		if (a->_int)
			s += st->b - 1;	// offset the s++
		break;
		
	case OP_GOTO:
//		tk_puts("PR_ExecuteProgram: OP_GOTO\n");
		s += st->a - 1;	// offset the s++
		break;
		
	case OP_CALL0:
	case OP_CALL1:
	case OP_CALL2:
	case OP_CALL3:
	case OP_CALL4:
	case OP_CALL5:
	case OP_CALL6:
	case OP_CALL7:
	case OP_CALL8:
//		tk_puts("PR_ExecuteProgram: OP_CALLn\n");

		pr_argc = st->op - OP_CALL0;
		if (!a->function)
			PR_RunError ("NULL function");

//		tk_printf("PR_ExecuteProgram: OP_CALLn %d\n", pr_argc);

#if 0
		tk_printf("PR_ExecuteProgram: OP_CALLn %d (%X %X %X) (%X %X %X)\n",
			pr_argc,
			((u32 *)pr_globals)[OFS_PARM0+0],
			((u32 *)pr_globals)[OFS_PARM0+1],
			((u32 *)pr_globals)[OFS_PARM0+2],
			((u32 *)pr_globals)[OFS_PARM1+0],
			((u32 *)pr_globals)[OFS_PARM1+1],
			((u32 *)pr_globals)[OFS_PARM1+2]);
#endif

		if((a->function<0) || (a->function>=progs->numfunctions))
		{
			tk_printf("Bad Function %d\n", a->function);
			PR_RunError ("Bad Function");
			break;
		}

		newf = &pr_functions[a->function];
//		newf = pr_functions + a->function;

//		if((a->function<0) || (a->function!=((short)a->function)))
//			__debugbreak();

//		tk_puts("PR_ExecuteProgram: OP_CALLn: A1\n");

//		tk_printf("PR_ExecuteProgram: %d %p %p\n",
//			a->function, pr_functions, newf);

		if (newf->first_statement < 0)
		{
//			tk_puts("PR_ExecuteProgram: OP_CALLn: B1\n");
			// negative statements are built in functions
			i = -newf->first_statement;
			if (i >= pr_numbuiltins)
				PR_RunError ("Bad builtin call number");
//			tk_puts("PR_ExecuteProgram: OP_CALLn: B2\n");
			pr_builtins[i] ();
//			tk_puts("PR_ExecuteProgram: OP_CALLn: B3\n");
			break;
		}

//		tk_puts("PR_ExecuteProgram: OP_CALLn: A2\n");
		s = PR_EnterFunction (newf);
		break;

	case OP_DONE:
	case OP_RETURN:
//		tk_puts("PR_ExecuteProgram: OP_RETURN\n");
		pr_globals[OFS_RETURN] = pr_globals[st->a];
		pr_globals[OFS_RETURN+1] = pr_globals[st->a+1];
		pr_globals[OFS_RETURN+2] = pr_globals[st->a+2];
	
		s = PR_LeaveFunction ();
		if (pr_depth == exitdepth)
			return;		// all done
		break;
		
	case OP_STATE:
//		tk_puts("PR_ExecuteProgram: OP_STATE\n");

		ed = PROG_TO_EDICT(pr_global_struct->self);
#ifdef FPS_20
		ed->v.nextthink = pr_global_struct->time + 0.05;
#else
		ed->v.nextthink = pr_global_struct->time + 0.1;
#endif
		if (a->_float != ed->v.frame)
		{
			ed->v.frame = a->_float;
		}
		ed->v.think = b->function;
		break;
		
	default:
//		tk_puts("PR_ExecuteProgram: OP: default\n");
		PR_RunError ("Bad opcode %i", st->op);
	}
}

}
