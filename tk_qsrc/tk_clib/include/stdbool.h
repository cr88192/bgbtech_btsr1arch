/*
	stdbool.h -- Boolean type and values

	This source code has been placed into the PUBLIC DOMAIN by its author.

	last edit:	1999/11/05	gwyn@arl.mil

	Implements subclause 7.16 of ISO/IEC 9899:1999 (E).
*/

#if	!defined(_STDBOOL_H) && !defined(_INC_STDBOOL)	/* usual lock names */
#define	_STDBOOL_H			/* idempotency lock (section 7.1.2) */
#define	_INC_STDBOOL

/* This header doesn't need C++ extern "C"; there are no declarations here. */

/* program is allowed to contain its own definitions, so ... */
#undef	bool
#undef	true
#undef	false
#undef	__bool_true_false_are_defined

#define	bool				_Bool
#define	true				1
#define	false				0
#define	__bool_true_false_are_defined	1

// #if	__STDC_VERSION__ < 199901
// typedef int	_Bool;			/* not built into pre-C99 compilers */
// #endif

#endif	/* !defined(_STDBOOL_H) && !defined(_INC_STDBOOL) */
