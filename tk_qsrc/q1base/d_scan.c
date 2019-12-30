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
// d_scan.c
//
// Portable C scan-level rasterization code, all pixel depths.

#include "quakedef.h"
#include "r_local.h"
#include "d_local.h"

unsigned char	*r_turb_pbase, *r_turb_pdest;
unsigned short	*r_turb_pbase16, *r_turb_pdest16;
fixed16_t		r_turb_s, r_turb_t, r_turb_sstep, r_turb_tstep;
int				*r_turb_turb;
int				r_turb_spancount;

void D_DrawTurbulent8Span (void);


/*
=============
D_WarpScreen

// this performs a slight compression of the screen at the same time as
// the sine warp, to keep the edges from wrapping
=============
*/

void D_WarpScreen16 (void);

void D_WarpScreen (void)
{
	int		w, h;
	int		u,v;
	byte	*dest;
	int		*turb;
	int		*col;
	byte	**row;
	byte	*rowptr[MAXHEIGHT+(AMP2*2)];
	int		column[MAXWIDTH+(AMP2*2)];
	float	wratio, hratio;

	if(r_pixbytes==2)
	{
		D_WarpScreen16();
		return;
	}

	w = r_refdef.vrect.width;
	h = r_refdef.vrect.height;

	wratio = w / (float)scr_vrect.width;
	hratio = h / (float)scr_vrect.height;

	for (v=0 ; v<scr_vrect.height+AMP2*2 ; v++)
	{
		rowptr[v] = d_viewbuffer + (r_refdef.vrect.y * screenwidth) +
				 (screenwidth * (int)((float)v * hratio * h / (h + AMP2 * 2)));
	}

	for (u=0 ; u<scr_vrect.width+AMP2*2 ; u++)
	{
		column[u] = r_refdef.vrect.x +
				(int)((float)u * wratio * w / (w + AMP2 * 2));
	}

	turb = intsintable + ((int)(cl.time*SPEED)&(CYCLE-1));
	dest = vid.buffer + scr_vrect.y * vid.rowbytes + scr_vrect.x;

	for (v=0 ; v<scr_vrect.height ; v++, dest += vid.rowbytes)
	{
		col = &column[turb[v]];
		row = &rowptr[v];

		for (u=0 ; u<scr_vrect.width ; u+=4)
		{
			dest[u+0] = row[turb[u+0]][col[u+0]];
			dest[u+1] = row[turb[u+1]][col[u+1]];
			dest[u+2] = row[turb[u+2]][col[u+2]];
			dest[u+3] = row[turb[u+3]][col[u+3]];
		}
	}
}

void D_WarpScreen16 (void)
{
	int		w, h;
	int		u,v;
	u16	*dest;
	int		*turb;
	int		*col;
	u16		**row;
	u16		*rowptr[MAXHEIGHT+(AMP2*2)];
	int		column[MAXWIDTH+(AMP2*2)];
	float	wratio, hratio;

	w = r_refdef.vrect.width;
	h = r_refdef.vrect.height;

	wratio = w / (float)scr_vrect.width;
	hratio = h / (float)scr_vrect.height;

	for (v=0 ; v<scr_vrect.height+AMP2*2 ; v++)
	{
		rowptr[v] = ((u16 *)d_viewbuffer) +
			(r_refdef.vrect.y * screenwidth) +
			(screenwidth * (int)((float)v * hratio * h / (h + AMP2 * 2)));
	}

	for (u=0 ; u<scr_vrect.width+AMP2*2 ; u++)
	{
		column[u] = r_refdef.vrect.x +
				(int)((float)u * wratio * w / (w + AMP2 * 2));
	}

	turb = intsintable + ((int)(cl.time*SPEED)&(CYCLE-1));
	dest = (u16 *)vid.buffer + scr_vrect.y * (vid.rowbytes/2) + scr_vrect.x;

	for (v=0 ; v<scr_vrect.height ; v++, dest += (vid.rowbytes/2))
	{
		col = &column[turb[v]];
		row = &rowptr[v];

		for (u=0 ; u<scr_vrect.width ; u+=4)
		{
			dest[u+0] = row[turb[u+0]][col[u+0]];
			dest[u+1] = row[turb[u+1]][col[u+1]];
			dest[u+2] = row[turb[u+2]][col[u+2]];
			dest[u+3] = row[turb[u+3]][col[u+3]];
		}
	}
}


// #if	!id386
#if 1

/*
=============
D_DrawTurbulent8Span
=============
*/
void D_DrawTurbulent8Span (void)
{
	int		sturb, tturb;

	do
	{
		sturb = ((r_turb_s + r_turb_turb[(r_turb_t>>16)&(CYCLE-1)])>>16)&63;
		tturb = ((r_turb_t + r_turb_turb[(r_turb_s>>16)&(CYCLE-1)])>>16)&63;
		*r_turb_pdest++ = *(r_turb_pbase + (tturb<<6) + sturb);
		r_turb_s += r_turb_sstep;
		r_turb_t += r_turb_tstep;
	} while (--r_turb_spancount > 0);
}

#endif	// !id386

void D_DrawTurbulent16Span (void)
{
	int		sturb, tturb;

	do
	{
		sturb = ((r_turb_s + r_turb_turb[(r_turb_t>>16)&(CYCLE-1)])>>16)&63;
		tturb = ((r_turb_t + r_turb_turb[(r_turb_s>>16)&(CYCLE-1)])>>16)&63;
//		*r_turb_pdest16++ = *(r_turb_pbase16 + (tturb<<6) + sturb);
		*r_turb_pdest16++ = d_8to16table[*(r_turb_pbase + (tturb<<6) + sturb)];
		r_turb_s += r_turb_sstep;
		r_turb_t += r_turb_tstep;
	} while (--r_turb_spancount > 0);
}


/*
=============
Turbulent8
=============
*/
void Turbulent8 (espan_t *pspan)
{
	int				count;
	fixed16_t		snext, tnext;
	float			sdivz, tdivz, zi, z, du, dv, spancountminus1;
	float			sdivz16stepu, tdivz16stepu, zi16stepu;
	
	r_turb_turb = sintable + ((int)(cl.time*SPEED)&(CYCLE-1));

	r_turb_sstep = 0;	// keep compiler happy
	r_turb_tstep = 0;	// ditto

	r_turb_pbase = (unsigned char *)cacheblock;

	sdivz16stepu = d_sdivzstepu * 16;
	tdivz16stepu = d_tdivzstepu * 16;
	zi16stepu = d_zistepu * 16;

	do
	{
		r_turb_pdest = (unsigned char *)((byte *)d_viewbuffer +
				(screenwidth * pspan->v) + pspan->u);

		count = pspan->count;

	// calculate the initial s/z, t/z, 1/z, s, and t and clamp
		du = (float)pspan->u;
		dv = (float)pspan->v;

		sdivz = d_sdivzorigin + dv*d_sdivzstepv + du*d_sdivzstepu;
		tdivz = d_tdivzorigin + dv*d_tdivzstepv + du*d_tdivzstepu;
		zi = d_ziorigin + dv*d_zistepv + du*d_zistepu;
		z = (float)0x10000 / zi;	// prescale to 16.16 fixed-point

		r_turb_s = (int)(sdivz * z) + sadjust;
		if (r_turb_s > bbextents)
			r_turb_s = bbextents;
		else if (r_turb_s < 0)
			r_turb_s = 0;

		r_turb_t = (int)(tdivz * z) + tadjust;
		if (r_turb_t > bbextentt)
			r_turb_t = bbextentt;
		else if (r_turb_t < 0)
			r_turb_t = 0;

		do
		{
		// calculate s and t at the far end of the span
			if (count >= 16)
				r_turb_spancount = 16;
			else
				r_turb_spancount = count;

			count -= r_turb_spancount;

			if (count)
			{
			// calculate s/z, t/z, zi->fixed s and t at far end of span,
			// calculate s and t steps across span by shifting
				sdivz += sdivz16stepu;
				tdivz += tdivz16stepu;
				zi += zi16stepu;
				z = (float)0x10000 / zi;	// prescale to 16.16 fixed-point

				snext = (int)(sdivz * z) + sadjust;
				if (snext > bbextents)
					snext = bbextents;
				else if (snext < 16)
					snext = 16;	// prevent round-off error on <0 steps from
								//  from causing overstepping & running off the
								//  edge of the texture

				tnext = (int)(tdivz * z) + tadjust;
				if (tnext > bbextentt)
					tnext = bbextentt;
				else if (tnext < 16)
					tnext = 16;	// guard against round-off error on <0 steps

				r_turb_sstep = (snext - r_turb_s) >> 4;
				r_turb_tstep = (tnext - r_turb_t) >> 4;
			}
			else
			{
			// calculate s/z, t/z, zi->fixed s and t at last pixel in span (so
			// can't step off polygon), clamp, calculate s and t steps across
			// span by division, biasing steps low so we don't run off the
			// texture
				spancountminus1 = (float)(r_turb_spancount - 1);
				sdivz += d_sdivzstepu * spancountminus1;
				tdivz += d_tdivzstepu * spancountminus1;
				zi += d_zistepu * spancountminus1;
				z = (float)0x10000 / zi;	// prescale to 16.16 fixed-point
				snext = (int)(sdivz * z) + sadjust;
				if (snext > bbextents)
					snext = bbextents;
				else if (snext < 16)
					snext = 16;	// prevent round-off error on <0 steps from
								//  from causing overstepping & running off the
								//  edge of the texture

				tnext = (int)(tdivz * z) + tadjust;
				if (tnext > bbextentt)
					tnext = bbextentt;
				else if (tnext < 16)
					tnext = 16;	// guard against round-off error on <0 steps

				if (r_turb_spancount > 1)
				{
//					r_turb_sstep = (snext - r_turb_s) / (r_turb_spancount - 1);
//					r_turb_tstep = (tnext - r_turb_t) / (r_turb_spancount - 1);

					r_turb_sstep = D_SoftDiv((snext - r_turb_s),
						(r_turb_spancount - 1));
					r_turb_tstep = D_SoftDiv((tnext - r_turb_t),
						(r_turb_spancount - 1));
				}
			}

			r_turb_s = r_turb_s & ((CYCLE<<16)-1);
			r_turb_t = r_turb_t & ((CYCLE<<16)-1);

			D_DrawTurbulent8Span ();

			r_turb_s = snext;
			r_turb_t = tnext;

		} while (count > 0);

	} while ((pspan = pspan->pnext) != NULL);
}


/*
=============
Turbulent8
=============
*/
void Turbulent16 (espan_t *pspan)
{
	int				count;
	fixed16_t		snext, tnext;
	float			sdivz, tdivz, zi, z, du, dv, spancountminus1;
	float			sdivz16stepu, tdivz16stepu, zi16stepu;
	
	r_turb_turb = sintable + ((int)(cl.time*SPEED)&(CYCLE-1));

	r_turb_sstep = 0;	// keep compiler happy
	r_turb_tstep = 0;	// ditto

	r_turb_pbase = (unsigned char *)cacheblock;
	r_turb_pbase16 = (unsigned short *)cacheblock;

	sdivz16stepu = d_sdivzstepu * 16;
	tdivz16stepu = d_tdivzstepu * 16;
	zi16stepu = d_zistepu * 16;

	do
	{
		r_turb_pdest16 = (unsigned short *)((short *)d_viewbuffer +
				(screenwidth * pspan->v) + pspan->u);

		count = pspan->count;

	// calculate the initial s/z, t/z, 1/z, s, and t and clamp
		du = (float)pspan->u;
		dv = (float)pspan->v;

		sdivz = d_sdivzorigin + dv*d_sdivzstepv + du*d_sdivzstepu;
		tdivz = d_tdivzorigin + dv*d_tdivzstepv + du*d_tdivzstepu;
		zi = d_ziorigin + dv*d_zistepv + du*d_zistepu;
		z = (float)0x10000 / zi;	// prescale to 16.16 fixed-point

		r_turb_s = (int)(sdivz * z) + sadjust;
		if (r_turb_s > bbextents)
			r_turb_s = bbextents;
		else if (r_turb_s < 0)
			r_turb_s = 0;

		r_turb_t = (int)(tdivz * z) + tadjust;
		if (r_turb_t > bbextentt)
			r_turb_t = bbextentt;
		else if (r_turb_t < 0)
			r_turb_t = 0;

		do
		{
		// calculate s and t at the far end of the span
			if (count >= 16)
				r_turb_spancount = 16;
			else
				r_turb_spancount = count;

			count -= r_turb_spancount;

			if (count)
			{
			// calculate s/z, t/z, zi->fixed s and t at far end of span,
			// calculate s and t steps across span by shifting
				sdivz += sdivz16stepu;
				tdivz += tdivz16stepu;
				zi += zi16stepu;
				z = (float)0x10000 / zi;	// prescale to 16.16 fixed-point

				snext = (int)(sdivz * z) + sadjust;
				if (snext > bbextents)
					snext = bbextents;
				else if (snext < 16)
					snext = 16;	// prevent round-off error on <0 steps from
								//  from causing overstepping & running off the
								//  edge of the texture

				tnext = (int)(tdivz * z) + tadjust;
				if (tnext > bbextentt)
					tnext = bbextentt;
				else if (tnext < 16)
					tnext = 16;	// guard against round-off error on <0 steps

				r_turb_sstep = (snext - r_turb_s) >> 4;
				r_turb_tstep = (tnext - r_turb_t) >> 4;
			}
			else
			{
			// calculate s/z, t/z, zi->fixed s and t at last pixel in span (so
			// can't step off polygon), clamp, calculate s and t steps across
			// span by division, biasing steps low so we don't run off the
			// texture
				spancountminus1 = (float)(r_turb_spancount - 1);
				sdivz += d_sdivzstepu * spancountminus1;
				tdivz += d_tdivzstepu * spancountminus1;
				zi += d_zistepu * spancountminus1;
				z = (float)0x10000 / zi;	// prescale to 16.16 fixed-point
				snext = (int)(sdivz * z) + sadjust;
				if (snext > bbextents)
					snext = bbextents;
				else if (snext < 16)
					snext = 16;	// prevent round-off error on <0 steps from
								//  from causing overstepping & running off the
								//  edge of the texture

				tnext = (int)(tdivz * z) + tadjust;
				if (tnext > bbextentt)
					tnext = bbextentt;
				else if (tnext < 16)
					tnext = 16;	// guard against round-off error on <0 steps

				if (r_turb_spancount > 1)
				{
//					r_turb_sstep = (snext - r_turb_s) / (r_turb_spancount - 1);
//					r_turb_tstep = (tnext - r_turb_t) / (r_turb_spancount - 1);

					r_turb_sstep = D_SoftDiv((snext - r_turb_s),
						(r_turb_spancount - 1));
					r_turb_tstep = D_SoftDiv((tnext - r_turb_t),
						(r_turb_spancount - 1));
				}
			}

			r_turb_s = r_turb_s & ((CYCLE<<16)-1);
			r_turb_t = r_turb_t & ((CYCLE<<16)-1);

			D_DrawTurbulent16Span ();

			r_turb_s = snext;
			r_turb_t = tnext;

		} while (count > 0);

	} while ((pspan = pspan->pnext) != NULL);
}


// #if	!id386
#if 1

/*
=============
D_DrawSpans8
=============
*/
void D_DrawSpans8 (espan_t *pspan)
{
	int				count, spancount;
	unsigned char	*pbase, *pdest;
	fixed16_t		s, t, snext, tnext, sstep, tstep;
	float			sdivz, tdivz, zi, z, du, dv, spancountminus1;
	float			sdivz8stepu, tdivz8stepu, zi8stepu;

	sstep = 0;	// keep compiler happy
	tstep = 0;	// ditto

	pbase = (unsigned char *)cacheblock;

	sdivz8stepu = d_sdivzstepu * 8;
	tdivz8stepu = d_tdivzstepu * 8;
	zi8stepu = d_zistepu * 8;

	do
	{
		pdest = (unsigned char *)((byte *)d_viewbuffer +
				(screenwidth * pspan->v) + pspan->u);

		count = pspan->count;

	// calculate the initial s/z, t/z, 1/z, s, and t and clamp
		du = (float)pspan->u;
		dv = (float)pspan->v;

		sdivz = d_sdivzorigin + dv*d_sdivzstepv + du*d_sdivzstepu;
		tdivz = d_tdivzorigin + dv*d_tdivzstepv + du*d_tdivzstepu;
		zi = d_ziorigin + dv*d_zistepv + du*d_zistepu;
		z = (float)0x10000 / zi;	// prescale to 16.16 fixed-point

		s = (int)(sdivz * z) + sadjust;
		if (s > bbextents)
			s = bbextents;
		else if (s < 0)
			s = 0;

		t = (int)(tdivz * z) + tadjust;
		if (t > bbextentt)
			t = bbextentt;
		else if (t < 0)
			t = 0;

		do
		{
		// calculate s and t at the far end of the span
			if (count >= 8)
				spancount = 8;
			else
				spancount = count;

			count -= spancount;

			if (count)
			{
			// calculate s/z, t/z, zi->fixed s and t at far end of span,
			// calculate s and t steps across span by shifting
				sdivz += sdivz8stepu;
				tdivz += tdivz8stepu;
				zi += zi8stepu;
				z = (float)0x10000 / zi;	// prescale to 16.16 fixed-point

				snext = (int)(sdivz * z) + sadjust;
				if (snext > bbextents)
					snext = bbextents;
				else if (snext < 8)
					snext = 8;	// prevent round-off error on <0 steps from
								//  from causing overstepping & running off the
								//  edge of the texture

				tnext = (int)(tdivz * z) + tadjust;
				if (tnext > bbextentt)
					tnext = bbextentt;
				else if (tnext < 8)
					tnext = 8;	// guard against round-off error on <0 steps

				sstep = (snext - s) >> 3;
				tstep = (tnext - t) >> 3;
			}
			else
			{
			// calculate s/z, t/z, zi->fixed s and t at last pixel in span (so
			// can't step off polygon), clamp, calculate s and t steps across
			// span by division, biasing steps low so we don't run off the
			// texture
				spancountminus1 = (float)(spancount - 1);
				sdivz += d_sdivzstepu * spancountminus1;
				tdivz += d_tdivzstepu * spancountminus1;
				zi += d_zistepu * spancountminus1;
				z = (float)0x10000 / zi;	// prescale to 16.16 fixed-point
				snext = (int)(sdivz * z) + sadjust;
				if (snext > bbextents)
					snext = bbextents;
				else if (snext < 8)
					snext = 8;	// prevent round-off error on <0 steps from
								//  from causing overstepping & running off the
								//  edge of the texture

				tnext = (int)(tdivz * z) + tadjust;
				if (tnext > bbextentt)
					tnext = bbextentt;
				else if (tnext < 8)
					tnext = 8;	// guard against round-off error on <0 steps

				if (spancount > 1)
				{
//					sstep = (snext - s) / (spancount - 1);
//					tstep = (tnext - t) / (spancount - 1);

					sstep = D_SoftDiv((snext - s), (spancount - 1));
					tstep = D_SoftDiv((tnext - t), (spancount - 1));
				}
			}

			do
			{
				*pdest++ = *(pbase + (s >> 16) + (t >> 16) * cachewidth);
				s += sstep;
				t += tstep;
			} while (--spancount > 0);

			s = snext;
			t = tnext;

		} while (count > 0);

	} while ((pspan = pspan->pnext) != NULL);
}

float __fpu_fdiv_sf(float x, float y);
float __fpu_frcp_sf(float x);

/*
=============
D_DrawSpans16
=============
*/
#if 0
void D_DrawSpans16 (espan_t *pspan)
{
	int				count, spancount;
	register int		px;
//	unsigned char		*pbase;
	register unsigned short		*pbase;
	register unsigned short		*pdest;
	fixed16_t		snext, tnext;
	register fixed16_t		s, t, sstep, tstep;
	register int			t_cachewidth;
	float			sdivz, tdivz, zi, z, du, dv, spancountminus1;
	float			sdivz8stepu, tdivz8stepu, zi8stepu;

//	return;	//BGB Debug

	__hint_use_egpr();

	sstep = 0;	// keep compiler happy
	tstep = 0;	// ditto

//	pbase = (unsigned char *)cacheblock;
	pbase = (unsigned short *)cacheblock;

	sdivz8stepu = d_sdivzstepu * 8;
	tdivz8stepu = d_tdivzstepu * 8;
	zi8stepu = d_zistepu * 8;

//	z=1.0;

//	sdivz8stepu = d_sdivzstepu * 8.0f;
//	tdivz8stepu = d_tdivzstepu * 8.0f;
//	zi8stepu = d_zistepu * 8.0f;

//	__debugbreak();

//	printf("d_sdivzstepu %f %f\n", d_sdivzstepu, sdivz8stepu);
//	printf("d_tdivzstepu %f %f\n", d_tdivzstepu, tdivz8stepu);

//	__debugbreak();

	do
	{
		pdest = (unsigned short *)((short *)d_viewbuffer +
				(screenwidth * pspan->v) + pspan->u);

		count = pspan->count;

	// calculate the initial s/z, t/z, 1/z, s, and t and clamp
		du = (float)pspan->u;
		dv = (float)pspan->v;

		sdivz = d_sdivzorigin + dv*d_sdivzstepv + du*d_sdivzstepu;
		tdivz = d_tdivzorigin + dv*d_tdivzstepv + du*d_tdivzstepu;
		zi = d_ziorigin + dv*d_zistepv + du*d_zistepu;
//		z = (float)0x10000 / zi;	// prescale to 16.16 fixed-point
//		z = 65536.0 / zi;	// prescale to 16.16 fixed-point
		z = __fpu_frcp_sf(zi) * 65536.0;

//		tk_printf("zio=%f %f %f\n", d_ziorigin, d_zistepv, d_zistepu);

		s = (int)(sdivz * z) + sadjust;
		if (s > bbextents)
			s = bbextents;
		else if (s < 0)
			s = 0;

		t = (int)(tdivz * z) + tadjust;
		if (t > bbextentt)
			t = bbextentt;
		else if (t < 0)
			t = 0;

		do
		{
		// calculate s and t at the far end of the span
			if (count >= 8)
				spancount = 8;
			else
				spancount = count;

			count -= spancount;

			if (count)
			{
			// calculate s/z, t/z, zi->fixed s and t at far end of span,
			// calculate s and t steps across span by shifting
				sdivz += sdivz8stepu;
				tdivz += tdivz8stepu;
				zi += zi8stepu;
//				z = (float)0x10000 / zi;	// prescale to 16.16 fixed-point
//				z = 65536.0 / zi;	// prescale to 16.16 fixed-point
//				z = __fpu_fdiv_sf(65536.0, zi);
				z = __fpu_frcp_sf(zi) * 65536.0;

				snext = (int)(sdivz * z) + sadjust;
//				snext = (sdivz * z) + sadjust;
				if (snext > bbextents)
					snext = bbextents;
				else if (snext < 8)
					snext = 8;	// prevent round-off error on <0 steps from
								//  from causing overstepping & running off the
								//  edge of the texture

				tnext = (int)(tdivz * z) + tadjust;
				if (tnext > bbextentt)
					tnext = bbextentt;
				else if (tnext < 8)
					tnext = 8;	// guard against round-off error on <0 steps

				sstep = (snext - s) >> 3;
				tstep = (tnext - t) >> 3;
			}
			else
			{
			// calculate s/z, t/z, zi->fixed s and t at last pixel in span (so
			// can't step off polygon), clamp, calculate s and t steps across
			// span by division, biasing steps low so we don't run off the
			// texture
				spancountminus1 = (float)(spancount - 1);
				sdivz += d_sdivzstepu * spancountminus1;
				tdivz += d_tdivzstepu * spancountminus1;
				zi += d_zistepu * spancountminus1;
//				z = (float)0x10000 / zi;	// prescale to 16.16 fixed-point
//				z = 65536.0 / zi;	// prescale to 16.16 fixed-point
//				z = __fpu_fdiv_sf(65536.0, zi);
				z = __fpu_frcp_sf(zi) * 65536.0;

				snext = (int)(sdivz * z) + sadjust;
//				snext = (sdivz * z) + sadjust;
				if (snext > bbextents)
					snext = bbextents;
				else if (snext < 8)
					snext = 8;	// prevent round-off error on <0 steps from
								//  from causing overstepping & running off the
								//  edge of the texture

				tnext = (int)(tdivz * z) + tadjust;
				if (tnext > bbextentt)
					tnext = bbextentt;
				else if (tnext < 8)
					tnext = 8;	// guard against round-off error on <0 steps

				if (spancount > 1)
				{
//					sstep = (snext - s) / (spancount - 1);
//					tstep = (tnext - t) / (spancount - 1);

					sstep = D_SoftDiv((snext - s), (spancount - 1));
					tstep = D_SoftDiv((tnext - t), (spancount - 1));
				}
			}

#if 0
			if(!sstep)
			{
				tk_printf("sstep %d %d\n", sstep, tstep);
				tk_printf("s snext %d %d %d\n", s, snext, bbextents);
				tk_printf("sdivz=%f z=%f zi=%f zi8step=%f\n",
					sdivz, z, zi, zi8stepu);
				tk_printf("sdstep=%f %f sdi8st=%f\n", d_sdivzstepu,
					d_sdivzstepu * spancountminus1,
					sdivz8stepu);
			}
#endif
			
//			tk_printf("%d %d\n", sstep, tstep);
			t_cachewidth = cachewidth;

			while(spancount>=4)
			{
				px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
				s += sstep;		t += tstep;
				*pdest++ = px;

				px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
				s += sstep;		t += tstep;
				*pdest++ = px;

				px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
				s += sstep;		t += tstep;
				*pdest++ = px;

				px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
				s += sstep;		t += tstep;
				*pdest++ = px;

				spancount-=4;
			}
			if(spancount>=2)
			{
				px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
				s += sstep;		t += tstep;
				*pdest++ = px;

				px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
				s += sstep;		t += tstep;
				*pdest++ = px;

				spancount-=2;
			}
			if(spancount>0)
			{
				px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
				s += sstep;		t += tstep;
				*pdest++ = px;
			}

#if 0
			do
			{
				px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
//				px = *(pbase + (s >> 16) + (t >> 16) * (cachewidth>>1));

//				px = d_8to16table[px];
//				px = 0x7FFF;
				*pdest++ = px;
				s += sstep;
				t += tstep;
			} while (--spancount > 0);
#endif

			s = snext;
			t = tnext;

		} while (count > 0);

	} while ((pspan = pspan->pnext) != NULL);
}
#endif


#if 0

unsigned short		*d_pbase;
unsigned short		*d_pdest;
float			d_sdivz8stepu, d_tdivz8stepu, d_zi8stepu;
int				d_count;
fixed16_t		d_sstep, d_tstep;
float		d_sdivz, d_tdivz;
float		d_zi;
fixed16_t	d_s, d_t;
// float	d_du, d_dv;

//	register int		px;
//	fixed16_t		snext, tnext;
//	register fixed16_t		s, t, sstep, tstep;
//	register int			t_cachewidth;
//	float			sdivz, tdivz, zi, z, du, dv, spancountminus1;

void D_DrawSpans16_Inner ()
{
	unsigned short		*pbase;
	unsigned short		*pdest;
	float			sdivz8stepu, tdivz8stepu, zi8stepu;
	int				count;
	fixed16_t		sstep, tstep;
	float		sdivz, tdivz;
	float		zi;
	fixed16_t	s, t;

	int			spancount;
	int			px;
	fixed16_t	snext, tnext;
	int			t_cachewidth;
	float		z, spancountminus1;


	sdivz8stepu		= d_sdivz8stepu;
	tdivz8stepu		= d_tdivz8stepu;
	zi8stepu		= d_zi8stepu;

	pbase	= d_pbase;
	pdest	= d_pdest;
	count	= d_count;
	sstep	= d_sstep;
	tstep	= d_tstep;
	sdivz	= d_sdivz;
	tdivz	= d_tdivz;
	zi		= d_zi;
//	z		= d_z;
	s		= d_s;
	t		= d_t;

#if 1
	z = __fpu_frcp_sf(zi) * 65536.0;

	s = (int)(sdivz * z) + sadjust;
		if (s > bbextents)
			s = bbextents;
		else if (s < 0)
			s = 0;

		t = (int)(tdivz * z) + tadjust;
		if (t > bbextentt)
			t = bbextentt;
		else if (t < 0)
			t = 0;
#endif

	t_cachewidth = cachewidth;

	do
	{
		if (count >= 8)
			spancount = 8;
		else
			spancount = count;

		count -= spancount;

		if (count)
		{
			sdivz += sdivz8stepu;
			tdivz += tdivz8stepu;
			zi += zi8stepu;
			z = __fpu_frcp_sf(zi) * 65536.0;

			snext = (int)(sdivz * z) + sadjust;
			if (snext > bbextents)
				snext = bbextents;
			else if (snext < 8)
				snext = 8;
			tnext = (int)(tdivz * z) + tadjust;
			if (tnext > bbextentt)
				tnext = bbextentt;
			else if (tnext < 8)
				tnext = 8;
			sstep = (snext - s) >> 3;
			tstep = (tnext - t) >> 3;
		}
		else
		{
			spancountminus1 = (float)(spancount - 1);
			sdivz += d_sdivzstepu * spancountminus1;
			tdivz += d_tdivzstepu * spancountminus1;
			zi += d_zistepu * spancountminus1;
			z = __fpu_frcp_sf(zi) * 65536.0;

			snext = (int)(sdivz * z) + sadjust;
			if (snext > bbextents)
				snext = bbextents;
			else if (snext < 8)
				snext = 8;
			tnext = (int)(tdivz * z) + tadjust;
			if (tnext > bbextentt)
				tnext = bbextentt;
			else if (tnext < 8)
				tnext = 8;
			if (spancount > 1)
			{
				sstep = D_SoftDiv((snext - s), (spancount - 1));
				tstep = D_SoftDiv((tnext - t), (spancount - 1));
			}
		}
	
#if 0
		while(spancount>=4)
		{
			px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
			s += sstep;		t += tstep;
			*pdest++ = px;

			px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
			s += sstep;		t += tstep;
			*pdest++ = px;

			px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
			s += sstep;		t += tstep;
			*pdest++ = px;

			px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
			s += sstep;		t += tstep;
			*pdest++ = px;

			spancount-=4;
		}
#endif

//		if(spancount>=2)
		while(spancount>=2)
		{
			px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
			s += sstep;		t += tstep;
			*pdest++ = px;

			px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
			s += sstep;		t += tstep;
			*pdest++ = px;

			spancount-=2;
		}
		if(spancount>0)
		{
			px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
			s += sstep;		t += tstep;
			*pdest++ = px;
		}

		s = snext;
		t = tnext;

	} while (count > 0);
}

void D_DrawSpans16 (espan_t *pspan)
{
//	int				count, spancount;
//	register int		px;
//	fixed16_t		snext, tnext;
//	register fixed16_t		s, t, sstep, tstep;
//	register int			t_cachewidth;
//	float			sdivz, tdivz, zi, z, du, dv, spancountminus1;
	float z;
	float du, dv;

	__hint_use_egpr();

	d_sstep = 0;	// keep compiler happy
	d_tstep = 0;	// ditto

	d_pbase = (unsigned short *)cacheblock;

	d_sdivz8stepu = d_sdivzstepu * 8;
	d_tdivz8stepu = d_tdivzstepu * 8;
	d_zi8stepu = d_zistepu * 8;

	do
	{
		d_pdest = (unsigned short *)((short *)d_viewbuffer +
				(screenwidth * pspan->v) + pspan->u);

		d_count = pspan->count;

		du = (float)pspan->u;
		dv = (float)pspan->v;

		d_sdivz = d_sdivzorigin + dv*d_sdivzstepv + du*d_sdivzstepu;
		d_tdivz = d_tdivzorigin + dv*d_tdivzstepv + du*d_tdivzstepu;
		d_zi = d_ziorigin + dv*d_zistepv + du*d_zistepu;

#if 0
		z = __fpu_frcp_sf(d_zi) * 65536.0;

		d_s = (int)(d_sdivz * z) + sadjust;
		if (d_s > bbextents)
			d_s = bbextents;
		else if (d_s < 0)
			d_s = 0;

		d_t = (int)(d_tdivz * z) + tadjust;
		if (d_t > bbextentt)
			d_t = bbextentt;
		else if (d_t < 0)
			d_t = 0;
#endif

		D_DrawSpans16_Inner();

	} while ((pspan = pspan->pnext) != NULL);
}
#endif



#if 1
#if 0
unsigned short		*d_pbase2;
unsigned short		*d_pdest2;
int				d_spancount2;
fixed16_t		d_s2, d_t2, d_sstep2, d_tstep2;
#endif

void D_DrawSpans16_InnerPx();

#if 0
void D_DrawSpans16_InnerPx()
{
	unsigned short		*pbase;
	unsigned short		*pdest;
	int				spancount;
	fixed16_t		s, t, sstep, tstep;
	int				px, t_cachewidth;

	__hint_use_egpr();

	pbase		= d_pbase2;
	pdest		= d_pdest2;
	spancount	= d_spancount2;
	s			= d_s2;
	t			= d_t2;
	sstep		= d_sstep2;
	tstep		= d_tstep2;

	t_cachewidth = cachewidth;

//	if(spancount>=2)
	while(spancount>=2)
	{
//		px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
		px = pbase[(s >> 16) + (t >> 16) * t_cachewidth];
		s += sstep;		t += tstep;
		*pdest++ = px;

//		px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
		px = pbase[(s >> 16) + (t >> 16) * t_cachewidth];
		s += sstep;		t += tstep;
		*pdest++ = px;

		spancount-=2;
	}

	if(spancount>0)
	{
//		px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
		px = pbase[(s >> 16) + (t >> 16) * t_cachewidth];
//		s += sstep;		t += tstep;
		*pdest++ = px;
	}

	d_pdest2 = pdest;
}
#endif

#if 0
void D_DrawSpans16_InnerPx2(
	unsigned short		*pbase,
	unsigned short		*pdest,
	int				spancount,
	int 			t_cachewidth,
	fixed16_t		s,
	fixed16_t		t,
	fixed16_t		sstep,
	fixed16_t		tstep)
{
//	int				px, t_cachewidth;
	int				px;

	__hint_use_egpr();

//	if(spancount>=2)
	while(spancount>=2)
	{
//		px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
		px = pbase[(s >> 16) + (t >> 16) * t_cachewidth];
		s += sstep;		t += tstep;
		*pdest++ = px;

//		px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
		px = pbase[(s >> 16) + (t >> 16) * t_cachewidth];
		s += sstep;		t += tstep;
		*pdest++ = px;

		spancount-=2;
	}

	if(spancount>0)
	{
//		px = *(pbase + (s >> 16) + (t >> 16) * t_cachewidth);
		px = pbase[(s >> 16) + (t >> 16) * t_cachewidth];
//		s += sstep;		t += tstep;
		*pdest++ = px;
	}

//	d_pdest2 = pdest;
}
#endif

#if 0
void D_DrawSpans16 (espan_t *pspan)
{
	unsigned short		*pbase;
	unsigned short		*pdest;
	int				count, spancount;
	int		px;
	fixed16_t		snext, tnext;
	fixed16_t		s, t, sstep, tstep;
	int			t_cachewidth;
	float			sdivz, tdivz, zi, z, du, dv, spancountminus1;
	float			sdivz8stepu, tdivz8stepu, zi8stepu;

	__hint_use_egpr();

	sstep = 0;	// keep compiler happy
	tstep = 0;	// ditto

	pbase = (unsigned short *)cacheblock;

	sdivz8stepu = d_sdivzstepu * 8;
	tdivz8stepu = d_tdivzstepu * 8;
	zi8stepu = d_zistepu * 8;

	do
	{
		pdest = (unsigned short *)((short *)d_viewbuffer +
				(screenwidth * pspan->v) + pspan->u);

//		d_pbase2		= pbase;
//		d_pdest2		= pdest;

		count = pspan->count;

		du = (float)pspan->u;
		dv = (float)pspan->v;

		sdivz = d_sdivzorigin + dv*d_sdivzstepv + du*d_sdivzstepu;
		tdivz = d_tdivzorigin + dv*d_tdivzstepv + du*d_tdivzstepu;
		zi = d_ziorigin + dv*d_zistepv + du*d_zistepu;
		z = __fpu_frcp_sf(zi) * 65536.0;

		s = (int)(sdivz * z) + sadjust;
		if (s > bbextents)
			s = bbextents;
		else if (s < 0)
			s = 0;

		t = (int)(tdivz * z) + tadjust;
		if (t > bbextentt)
			t = bbextentt;
		else if (t < 0)
			t = 0;

		do
		{
			if (count >= 8)
				spancount = 8;
			else
				spancount = count;

			count -= spancount;

			if (count)
			{
				sdivz += sdivz8stepu;
				tdivz += tdivz8stepu;
				zi += zi8stepu;
				z = __fpu_frcp_sf(zi) * 65536.0;

				snext = (int)(sdivz * z) + sadjust;
				if (snext > bbextents)
					snext = bbextents;
				else if (snext < 8)
					snext = 8;

				tnext = (int)(tdivz * z) + tadjust;
				if (tnext > bbextentt)
					tnext = bbextentt;
				else if (tnext < 8)
					tnext = 8;

				sstep = (snext - s) >> 3;
				tstep = (tnext - t) >> 3;
			}
			else
			{
				spancountminus1 = (float)(spancount - 1);
				sdivz += d_sdivzstepu * spancountminus1;
				tdivz += d_tdivzstepu * spancountminus1;
				zi += d_zistepu * spancountminus1;
				z = __fpu_frcp_sf(zi) * 65536.0;

				snext = (int)(sdivz * z) + sadjust;
				if (snext > bbextents)
					snext = bbextents;
				else if (snext < 8)
					snext = 8;

				tnext = (int)(tdivz * z) + tadjust;
				if (tnext > bbextentt)
					tnext = bbextentt;
				else if (tnext < 8)
					tnext = 8;

				if (spancount > 1)
				{
					sstep = D_SoftDiv((snext - s), (spancount - 1));
					tstep = D_SoftDiv((tnext - t), (spancount - 1));
				}
			}

#if 0
//			d_pbase2		= pbase;
			d_pdest2		= pdest;
			d_spancount2	= spancount;
			d_s2			= s;
			d_t2			= t;
			d_sstep2		= sstep;
			d_tstep2		= tstep;

			D_DrawSpans16_InnerPx();
#endif

			D_DrawSpans16_InnerPx2(
				pbase,			pdest,
				spancount,		cachewidth,
				s,				t,
				sstep,			tstep);


			pdest += spancount;
			
//			pdest = d_pdest2;

			s = snext;
			t = tnext;
		} while (count > 0);

	} while ((pspan = pspan->pnext) != NULL);
}
#endif

#if 1
void D_DrawSpans16 (espan_t *pspan)
{
	unsigned short		*pbase;
	unsigned short		*pdest;
	int				count, spancount;
	int		px;
	fixed16_t		snext, tnext;
	fixed16_t		s, t, sstep, tstep;
	int			t_cachewidth;
	float			sdivz, tdivz, zi, z, du, dv, spancountminus1;
	float			sdivz8stepu, tdivz8stepu, zi8stepu;
	float			z8stepu, zstepu;

	__hint_use_egpr();

	sstep = 0;	// keep compiler happy
	tstep = 0;	// ditto

	pbase = (unsigned short *)cacheblock;

	sdivz8stepu = d_sdivzstepu * 16;
	tdivz8stepu = d_tdivzstepu * 16;
	zi8stepu = d_zistepu * 16;
	
	zstepu = __fpu_frcp_sf(d_zistepu) * 65536.0;
	z8stepu = __fpu_frcp_sf(zi8stepu) * 65536.0;

	do
	{
		pdest = (unsigned short *)((short *)d_viewbuffer +
				(screenwidth * pspan->v) + pspan->u);

		count = pspan->count;

		du = (float)pspan->u;
		dv = (float)pspan->v;

		sdivz = d_sdivzorigin + dv*d_sdivzstepv + du*d_sdivzstepu;
		tdivz = d_tdivzorigin + dv*d_tdivzstepv + du*d_tdivzstepu;
		zi = d_ziorigin + dv*d_zistepv + du*d_zistepu;
//		z = __fpu_frcp_sf(zi) * 65536.0;
		z = __fpu_fdiv_sf(65536.0, zi);

		s = (int)(sdivz * z) + sadjust;
		t = (int)(tdivz * z) + tadjust;
		s = __int_clamp(s, 0, bbextents);
		t = __int_clamp(t, 0, bbextentt);

		do
		{
			if (count >= 16)
				spancount = 16;
			else
				spancount = count;

			count -= spancount;

			if (count)
			{
				sdivz += sdivz8stepu;
				tdivz += tdivz8stepu;
				zi += zi8stepu;
//				z = __fpu_frcp_sf(zi) * 65536.0;
				z = __fpu_fdiv_sf(65536.0, zi);
//				z += z8stepu;

				snext = (int)(sdivz * z) + sadjust;
				tnext = (int)(tdivz * z) + tadjust;
				snext = __int_clamp(snext, 0, bbextents);
				tnext = __int_clamp(tnext, 0, bbextentt);

				sstep = (snext - s) >> 4;
				tstep = (tnext - t) >> 4;
			}
			else
			{
//				spancountminus1 = (float)(spancount - 1);
				spancountminus1 = (float)(spancount);
				sdivz += d_sdivzstepu * spancountminus1;
				tdivz += d_tdivzstepu * spancountminus1;
				zi += d_zistepu * spancountminus1;
//				z = __fpu_frcp_sf(zi) * 65536.0;
				z = __fpu_fdiv_sf(65536.0, zi);
//				z += zstepu * spancountminus1;

				snext = (int)(sdivz * z) + sadjust;
				tnext = (int)(tdivz * z) + tadjust;
				snext = __int_clamp(snext, 0, bbextents);
				tnext = __int_clamp(tnext, 0, bbextentt);

				if (spancount > 1)
				{
//					sstep = D_SoftDiv((snext - s), (spancount - 1));
//					tstep = D_SoftDiv((tnext - t), (spancount - 1));

					sstep = D_SoftDiv((snext - s), spancount);
					tstep = D_SoftDiv((tnext - t), spancount);
				}
			}

			D_DrawSpans16_InnerPx2(
				pbase,			pdest,
				spancount,		cachewidth,
				s,				t,
				sstep,			tstep);

			pdest += spancount;

			s = snext;
			t = tnext;
		} while (count > 0);

	} while ((pspan = pspan->pnext) != NULL);
}
#endif

#endif


#endif


// #if	!id386
#if 1

#if 0
void D_DrawZSpans_Inner (
	short *pdest, int izi,
	int izistep, int count)
{
	unsigned		ltemp;

#if 1
	while (count>=2)
	{
		ltemp = izi >> 16;
		izi += izistep;
		ltemp |= izi & 0xFFFF0000;
		izi += izistep;
		*(int *)pdest = ltemp;
		pdest += 2;
		count -= 2;
	}
#endif

	if (count & 1)
		*pdest = (short)(izi >> 16);
}
#endif

/*
=============
D_DrawZSpans
=============
*/
void D_DrawZSpans (espan_t *pspan)
{
	int				count, doublecount, izistep;
	int				izi;
	short			*pdest;
	unsigned		ltemp;
	double			zi;
	float			du, dv;

// FIXME: check for clamping/range problems
// we count on FP exceptions being turned off to avoid range problems
	izistep = (int)(d_zistepu * 0x8000 * 0x10000);

	do
	{
		pdest = d_pzbuffer + (d_zwidth * pspan->v) + pspan->u;

		count = pspan->count;

	// calculate the initial 1/z
		du = (float)pspan->u;
		dv = (float)pspan->v;

		zi = d_ziorigin + dv*d_zistepv + du*d_zistepu;
	// we count on FP exceptions being turned off to avoid range problems
		izi = (int)(zi * 0x8000 * 0x10000);

		D_DrawZSpans_Inner(pdest, izi, izistep, count);

#if 0
#if 0
		if ((nlint)pdest & 0x02)
		{
			*pdest++ = (short)(izi >> 16);
			izi += izistep;
			count--;
		}

		if ((doublecount = count >> 1) > 0)
		{
			do
			{
				ltemp = izi >> 16;
				izi += izistep;
				ltemp |= izi & 0xFFFF0000;
				izi += izistep;
				*(int *)pdest = ltemp;
				pdest += 2;
			} while (--doublecount > 0);
		}
#endif

#if 1
		while (count>=2)
		{
			ltemp = izi >> 16;
			izi += izistep;
			ltemp |= izi & 0xFFFF0000;
			izi += izistep;
			*(int *)pdest = ltemp;
			pdest += 2;
			count -= 2;
		}
#endif

		if (count & 1)
			*pdest = (short)(izi >> 16);
#endif

	} while ((pspan = pspan->pnext) != NULL);
}

#endif

