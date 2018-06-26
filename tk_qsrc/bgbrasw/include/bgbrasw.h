#ifndef BGBRASW_H
#define BGBRASW_H

#ifdef BGBRASW_DLL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include <bgbrasw_conf.h>
#include <bgbrasw_raobj.h>
#include <bgbrasw_gl.h>
#include <bgbrasw/barf.h>

#ifndef VECMATH_API
#define VECMATH_API
#endif

#ifdef BGBRASW_DLL
#include <bgbrasw/vecmath.h>
#include <bgbrasw/inlines.h>
#endif

#include <bgbrasw_auto.h>

#endif
