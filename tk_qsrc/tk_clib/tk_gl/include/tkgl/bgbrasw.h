#ifndef BGBRASW_H
#define BGBRASW_H

// #ifdef BGBRASW_DLL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #endif

#include <tkgl/rasw_conf.h>

#if defined(WIN64) || (_M_IX86_FP>=2)
#include <xmmintrin.h>
#include <emmintrin.h>
#define HAVE_SSE2
#endif

#include <tkgl/rasw_raobj.h>
#include <tkgl/rasw_gl.h>
#include <tkgl/rasw_barf.h>

#ifndef VECMATH_API
#define VECMATH_API
#endif

// #ifdef BGBRASW_DLL
#include <tkgl/vecmath.h>
#include <tkgl/inlines.h>

#include <tkgl/tkgl_ractx.h>
#include <tkgl/tkgl_trans.h>

// #endif

#include <tkgl/tkgl_auto.h>

#endif
