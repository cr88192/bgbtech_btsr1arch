#ifndef BSDE_MATH_DOT_H
#define BSDE_MATH_DOT_H

#define V3F_NDOT(a, n)		((a)[0]*(n)[0]+(a)[1]*(n)[1]+ \
				(a)[2]*(n)[2]-(n)[3])
#define V3F_NDOT(a, n)		((a)[0]*(n)[0]+(a)[1]*(n)[1]+ \
				(a)[2]*(n)[2]-(n)[3])

#define V3D_NDOT(a, n)		((a)[0]*(n)[0]+(a)[1]*(n)[1]+ \
				(a)[2]*(n)[2]-(n)[3])
#define V3D_NDOT(a, n)		((a)[0]*(n)[0]+(a)[1]*(n)[1]+ \
				(a)[2]*(n)[2]-(n)[3])

#define V2D_DOTJK(a, b, j, k)	((a)[0*(j)]*(b)[0*(k)] + \
				 (a)[1*(j)]*(b)[1*(k)])
#define V3D_DOTJK(a, b, j, k)	((a)[0*(j)]*(b)[0*(k)] + \
				 (a)[1*(j)]*(b)[1*(k)] + \
				 (a)[2*(j)]*(b)[2*(k)])
#define V4D_DOTJK(a, b, j, k)	((a)[0*(j)]*(b)[0*(k)] + \
				 (a)[1*(j)]*(b)[1*(k)] + \
				 (a)[2*(j)]*(b)[2*(k)] + \
				 (a)[3*(j)]*(b)[3*(k)])

#define V2D_DOT11(a, b)		V2D_DOTJK(a, b, 1, 1)
#define V2D_DOT12(a, b)		V2D_DOTJK(a, b, 1, 2)
#define V2D_DOT13(a, b)		V2D_DOTJK(a, b, 1, 3)
#define V2D_DOT14(a, b)		V2D_DOTJK(a, b, 1, 4)
#define V2D_DOT21(a, b)		V2D_DOTJK(a, b, 2, 1)
#define V2D_DOT22(a, b)		V2D_DOTJK(a, b, 2, 2)
#define V2D_DOT23(a, b)		V2D_DOTJK(a, b, 2, 3)
#define V2D_DOT24(a, b)		V2D_DOTJK(a, b, 2, 4)
#define V2D_DOT31(a, b)		V2D_DOTJK(a, b, 3, 1)
#define V2D_DOT32(a, b)		V2D_DOTJK(a, b, 3, 2)
#define V2D_DOT33(a, b)		V2D_DOTJK(a, b, 3, 3)
#define V2D_DOT34(a, b)		V2D_DOTJK(a, b, 3, 4)
#define V2D_DOT41(a, b)		V2D_DOTJK(a, b, 4, 1)
#define V2D_DOT42(a, b)		V2D_DOTJK(a, b, 4, 2)
#define V2D_DOT43(a, b)		V2D_DOTJK(a, b, 4, 3)
#define V2D_DOT44(a, b)		V2D_DOTJK(a, b, 4, 4)

#define V3D_DOT11(a, b)		V3D_DOTJK(a, b, 1, 1)
#define V3D_DOT12(a, b)		V3D_DOTJK(a, b, 1, 2)
#define V3D_DOT13(a, b)		V3D_DOTJK(a, b, 1, 3)
#define V3D_DOT14(a, b)		V3D_DOTJK(a, b, 1, 4)
#define V3D_DOT21(a, b)		V3D_DOTJK(a, b, 2, 1)
#define V3D_DOT22(a, b)		V3D_DOTJK(a, b, 2, 2)
#define V3D_DOT23(a, b)		V3D_DOTJK(a, b, 2, 3)
#define V3D_DOT24(a, b)		V3D_DOTJK(a, b, 2, 4)
#define V3D_DOT31(a, b)		V3D_DOTJK(a, b, 3, 1)
#define V3D_DOT32(a, b)		V3D_DOTJK(a, b, 3, 2)
#define V3D_DOT33(a, b)		V3D_DOTJK(a, b, 3, 3)
#define V3D_DOT34(a, b)		V3D_DOTJK(a, b, 3, 4)
#define V3D_DOT41(a, b)		V3D_DOTJK(a, b, 4, 1)
#define V3D_DOT42(a, b)		V3D_DOTJK(a, b, 4, 2)
#define V3D_DOT43(a, b)		V3D_DOTJK(a, b, 4, 3)
#define V3D_DOT44(a, b)		V3D_DOTJK(a, b, 4, 4)

#define V4D_DOT11(a, b)		V4D_DOTJK(a, b, 1, 1)
#define V4D_DOT12(a, b)		V4D_DOTJK(a, b, 1, 2)
#define V4D_DOT13(a, b)		V4D_DOTJK(a, b, 1, 3)
#define V4D_DOT14(a, b)		V4D_DOTJK(a, b, 1, 4)
#define V4D_DOT21(a, b)		V4D_DOTJK(a, b, 2, 1)
#define V4D_DOT22(a, b)		V4D_DOTJK(a, b, 2, 2)
#define V4D_DOT23(a, b)		V4D_DOTJK(a, b, 2, 3)
#define V4D_DOT24(a, b)		V4D_DOTJK(a, b, 2, 4)
#define V4D_DOT31(a, b)		V4D_DOTJK(a, b, 3, 1)
#define V4D_DOT32(a, b)		V4D_DOTJK(a, b, 3, 2)
#define V4D_DOT33(a, b)		V4D_DOTJK(a, b, 3, 3)
#define V4D_DOT34(a, b)		V4D_DOTJK(a, b, 3, 4)
#define V4D_DOT41(a, b)		V4D_DOTJK(a, b, 4, 1)
#define V4D_DOT42(a, b)		V4D_DOTJK(a, b, 4, 2)
#define V4D_DOT43(a, b)		V4D_DOTJK(a, b, 4, 3)
#define V4D_DOT44(a, b)		V4D_DOTJK(a, b, 4, 4)

#endif

