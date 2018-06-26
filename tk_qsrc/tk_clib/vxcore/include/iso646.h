/*
	<iso646.h> -- macros for use in place of certain tokens that are
		not expressible in the invariant subset of ISO 646:1991

	This source code has been placed into the PUBLIC DOMAIN by its author.

	last edit:	1999/08/20	gwyn@arl.mil

	Implements subclause 7.9 of ISO/IEC 9899:1999 (E).

	Usage:	??=include <iso646.h>

	Notes:	The C standard has always required that the following
		source trigraphs be supported in translation phase 1:
			??= mapped to #
			??( mapped to [
			??/ mapped to \
			??) mapped to ]
			??' mapped to ^
			??< mapped to {
			??! mapped to |
			??> mapped to }
			??- mapped to ~

		ISO/IEC 9899:1994 (ISO/IEC 9899:1990/Amendment 1)
		and subsequently ISO/IEC 9899:1999, which require
		conforming implementations to provide <iso646.h>,
		also require support in translation phases 3..4
		for the following preprocessing tokens:
			%: alternate spelling for #
			%:%: alternate spelling for ##
		They also require support in translation phases 3..7
		for the following preprocessing tokens:
			<: alternate spelling for [
			:> alternate spelling for ]
			<% alternate spelling for {
			%> alternate spelling for }

		The C standard also requires that the "difficult"
		characters be somehow provided in both the basic source
		and execution character sets, regardless of the
		provision of trigraph, digraph, and macro alternatives
		for these characters.  (Thus, the alternate spellings
		are intended to help programmers, not C implementors.)

		The 1999 revision of the C standard adds support for
		international characters in identifiers via Universal
		Character Names, but UCN equivalents are not allowed
		for the aforementioned "difficult" characters.

		It is suggested that *all* C programmers avoid use of
		the following identifiers in their programs, in case
		somebody later needs to use <iso646.h> in maintaining
		the programs.
 */

/* This header doesn't need an idempotency lock; there are no typedefs here. */

/* This header doesn't need C++ extern "C"; there are no declarations here. */

#define and	&&
#define and_eq	&=
#define bitand	&
#define bitor	|
#define compl	~
#define not	!
#define not_eq	!=
#define or	||
#define or_eq	|=
#define xor	^
#define xor_eq	^=
