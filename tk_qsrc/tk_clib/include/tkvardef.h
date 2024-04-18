/*
 * Define BGBCC dynamic-type extension keywords as virtual C keywords.
 */

#ifndef __TKVARDEF_H__
#define __TKVARDEF_H__


#ifndef __BGBCC
#error "tkvardef.h is a BGBCC language extension."
#endif

#define	public		__public
#define	private	__private
#define	protected	__protected

#define	var			__var

#define	dynamic		__dynamic

#define	class		__class
#define	interface	__interface

#define	package		__namespace
#define	namespace	__namespace

#define	import		__using
#define	using		__using

#define	variant		__variant
#define	fixnum		__fixnum
#define	flonum		__flonum
#define	string		__string
#define	object		__object

#define	instanceof	__instanceof

#endif
