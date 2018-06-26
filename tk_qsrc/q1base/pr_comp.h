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

// this file is shared by quake and qcc

//typedef nlint	func_t;
//typedef nlint	string_t;
typedef int	func_t;
typedef int	string_t;

typedef enum {ev_void, ev_string, ev_float, ev_vector, ev_entity, ev_field, ev_function, ev_pointer} etype_t;


#define	OFS_NULL		0
#define	OFS_RETURN		1
#define	OFS_PARM0		4		// leave 3 ofs for each parm to hold vectors
#define	OFS_PARM1		7
#define	OFS_PARM2		10
#define	OFS_PARM3		13
#define	OFS_PARM4		16
#define	OFS_PARM5		19
#define	OFS_PARM6		22
#define	OFS_PARM7		25
#define	RESERVED_OFS	28


enum {
	OP_DONE,		//0
	OP_MUL_F,		//1
	OP_MUL_V,		//2
	OP_MUL_FV,		//3
	OP_MUL_VF,		//4
	OP_DIV_F,		//5
	OP_ADD_F,		//6
	OP_ADD_V,		//7
	OP_SUB_F,		//8
	OP_SUB_V,		//9
	
	OP_EQ_F,		//10
	OP_EQ_V,		//11
	OP_EQ_S,		//12
	OP_EQ_E,		//13
	OP_EQ_FNC,		//14
	
	OP_NE_F,		//15
	OP_NE_V,		//16
	OP_NE_S,		//17
	OP_NE_E,		//18
	OP_NE_FNC,		//19
	
	OP_LE,			//20
	OP_GE,			//21
	OP_LT,			//22
	OP_GT,			//23

	OP_LOAD_F,		//24
	OP_LOAD_V,		//25
	OP_LOAD_S,		//26
	OP_LOAD_ENT,	//27
	OP_LOAD_FLD,	//28
	OP_LOAD_FNC,	//29

	OP_ADDRESS,		//30

	OP_STORE_F,		//31
	OP_STORE_V,		//32
	OP_STORE_S,		//33
	OP_STORE_ENT,	//34
	OP_STORE_FLD,	//35
	OP_STORE_FNC,	//36

	OP_STOREP_F,	//37
	OP_STOREP_V,	//38
	OP_STOREP_S,	//39
	OP_STOREP_ENT,	//40
	OP_STOREP_FLD,	//41
	OP_STOREP_FNC,	//42

	OP_RETURN,		//43
	OP_NOT_F,		//44
	OP_NOT_V,		//45
	OP_NOT_S,		//46
	OP_NOT_ENT,		//37
	OP_NOT_FNC,		//48
	OP_IF,			//49
	OP_IFNOT,		//50
	OP_CALL0,
	OP_CALL1,
	OP_CALL2,
	OP_CALL3,
	OP_CALL4,
	OP_CALL5,
	OP_CALL6,
	OP_CALL7,
	OP_CALL8,
	OP_STATE,
	OP_GOTO,
	OP_AND,
	OP_OR,
	
	OP_BITAND,
	OP_BITOR
};


typedef struct statement_s
{
	unsigned short	op;
	short	a,b,c;
} dstatement_t;

typedef struct
{
	unsigned short	type;		// if DEF_SAVEGLOBGAL bit is set
								// the variable needs to be saved in savegames
	unsigned short	ofs;
	int			s_name;
} ddef_t;
#define	DEF_SAVEGLOBAL	(1<<15)

#define	MAX_PARMS	8

typedef struct
{
	int		first_statement;	// negative numbers are builtins
	int		parm_start;
	int		locals;				// total ints of parms + locals
	
	int		profile;		// runtime
	
	int		s_name;
	int		s_file;			// source file defined in
	
	int		numparms;
	byte	parm_size[MAX_PARMS];
} dfunction_t;


#define	PROG_VERSION	6
typedef struct
{
	int		version;
	int		crc;			// check of header file
	
	int		ofs_statements;
	int		numstatements;	// statement 0 is an error

	int		ofs_globaldefs;
	int		numglobaldefs;
	
	int		ofs_fielddefs;
	int		numfielddefs;
	
	int		ofs_functions;
	int		numfunctions;	// function 0 is an empty
	
	int		ofs_strings;
	int		numstrings;		// first string is a null string

	int		ofs_globals;
	int		numglobals;
	
	int		entityfields;
} dprograms_t;

