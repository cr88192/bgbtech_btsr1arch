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
// gl_mesh.c: triangle model functions

#include "quakedef.h"

/*
=================================================================

ALIAS MODEL DISPLAY LIST GENERATION

=================================================================
*/

model_t		*aliasmodel;
aliashdr_t	*paliashdr;

qboolean	used[8192];

// the command list holds counts and s/t values that are valid for
// every frame
int		commands[8192];
int		numcommands;

// all frames will have their vertexes rearranged and expanded
// so they are in the order expected by the command list
int		vertexorder[8192];
int		numorder;

int		allverts, alltris;

int		stripverts[128];
int		striptris[128];
int		stripcount;

/*
================
StripLength
================
*/
int	StripLength (int starttri, int startv)
{
	int			m1, m2;
	int			j;
	mtriangle_t	*last, *check;
	int			k;

	used[starttri] = 2;

	last = &triangles[starttri];

	stripverts[0] = last->vertindex[(startv)%3];
	stripverts[1] = last->vertindex[(startv+1)%3];
	stripverts[2] = last->vertindex[(startv+2)%3];

	striptris[0] = starttri;
	stripcount = 1;

	m1 = last->vertindex[(startv+2)%3];
	m2 = last->vertindex[(startv+1)%3];

	// look for a matching triangle
nexttri:
	for (j=starttri+1, check=&triangles[starttri+1] ; j<pheader->numtris ; j++, check++)
	{
		if (check->facesfront != last->facesfront)
			continue;
		for (k=0 ; k<3 ; k++)
		{
			if (check->vertindex[k] != m1)
				continue;
			if (check->vertindex[ (k+1)%3 ] != m2)
				continue;

			// this is the next part of the fan

			// if we can't use this triangle, this tristrip is done
			if (used[j])
				goto done;

			// the new edge
			if (stripcount & 1)
				m2 = check->vertindex[ (k+2)%3 ];
			else
				m1 = check->vertindex[ (k+2)%3 ];

			stripverts[stripcount+2] = check->vertindex[ (k+2)%3 ];
			striptris[stripcount] = j;
			stripcount++;

			used[j] = 2;
			goto nexttri;
		}
	}
done:

	// clear the temp used flags
	for (j=starttri+1 ; j<pheader->numtris ; j++)
		if (used[j] == 2)
			used[j] = 0;

	return stripcount;
}

/*
===========
FanLength
===========
*/
int	FanLength (int starttri, int startv)
{
	int		m1, m2;
	int		j;
	mtriangle_t	*last, *check;
	int		k;

	used[starttri] = 2;

	last = &triangles[starttri];

	stripverts[0] = last->vertindex[(startv)%3];
	stripverts[1] = last->vertindex[(startv+1)%3];
	stripverts[2] = last->vertindex[(startv+2)%3];

	striptris[0] = starttri;
	stripcount = 1;

	m1 = last->vertindex[(startv+0)%3];
	m2 = last->vertindex[(startv+2)%3];


	// look for a matching triangle
nexttri:
	for (j=starttri+1, check=&triangles[starttri+1] ; j<pheader->numtris ; j++, check++)
	{
		if (check->facesfront != last->facesfront)
			continue;
		for (k=0 ; k<3 ; k++)
		{
			if (check->vertindex[k] != m1)
				continue;
			if (check->vertindex[ (k+1)%3 ] != m2)
				continue;

			// this is the next part of the fan

			// if we can't use this triangle, this tristrip is done
			if (used[j])
				goto done;

			// the new edge
			m2 = check->vertindex[ (k+2)%3 ];

			stripverts[stripcount+2] = m2;
			striptris[stripcount] = j;
			stripcount++;

			used[j] = 2;
			goto nexttri;
		}
	}
done:

	// clear the temp used flags
	for (j=starttri+1 ; j<pheader->numtris ; j++)
		if (used[j] == 2)
			used[j] = 0;

	return stripcount;
}


/*
================
BuildTris

Generate a list of trifans or strips
for the model, which holds for all frames
================
*/
void BuildTris (void)
{
	int		i, j, k;
	int		startv;
	mtriangle_t	*last, *check;
	int		m1, m2;
	int		striplength;
	trivertx_t	*v;
	mtriangle_t *tv;
	float	s, t;
	int		index;
	int		len, bestlen, besttype;
	int		bestverts[1024];
	int		besttris[1024];
	int		type;

	//
	// build tristrips
	//
	numorder = 0;
	numcommands = 0;
	memset (used, 0, sizeof(used));
	for (i=0 ; i<pheader->numtris ; i++)
	{
		// pick an unused triangle and start the trifan
		if (used[i])
			continue;

		bestlen = 0;
		for (type = 0 ; type < 2 ; type++)
//	type = 1;
		{
			for (startv =0 ; startv < 3 ; startv++)
			{
				if (type == 1)
					len = StripLength (i, startv);
				else
					len = FanLength (i, startv);
				if (len > bestlen)
				{
					besttype = type;
					bestlen = len;
					for (j=0 ; j<bestlen+2 ; j++)
						bestverts[j] = stripverts[j];
					for (j=0 ; j<bestlen ; j++)
						besttris[j] = striptris[j];
				}
			}
		}

		// mark the tris on the best strip as used
		for (j=0 ; j<bestlen ; j++)
			used[besttris[j]] = 1;

		if (besttype == 1)
			commands[numcommands++] = (bestlen+2);
		else
			commands[numcommands++] = -(bestlen+2);

		for (j=0 ; j<bestlen+2 ; j++)
		{
			// emit a vertex into the reorder buffer
			k = bestverts[j];
			vertexorder[numorder++] = k;

			// emit s/t coords into the commands stream
			s = stverts[k].s;
			t = stverts[k].t;
			if (!triangles[besttris[0]].facesfront && stverts[k].onseam)
				s += pheader->skinwidth / 2;	// on back side
			s = (s + 0.5) / pheader->skinwidth;
			t = (t + 0.5) / pheader->skinheight;

			*(float *)&commands[numcommands++] = s;
			*(float *)&commands[numcommands++] = t;
		}
	}

	commands[numcommands++] = 0;		// end of list marker

	Con_DPrintf ("%3i tri %3i vert %3i cmd\n", pheader->numtris, numorder, numcommands);

	allverts += numorder;
	alltris += pheader->numtris;
}


/*
================
GL_MakeAliasModelDisplayLists
================
*/
void GL_MakeAliasModelDisplayLists (model_t *m, aliashdr_t *hdr)
{
	int		i, j;
	maliasgroup_t	*paliasgroup;
	int			*cmds;
	trivertx_t	*verts;
	char	cache[MAX_QPATH], fullpath[MAX_OSPATH], *c;
	FILE	*f;
	int		len;
	byte	*data;

	aliasmodel = m;
	paliashdr = hdr;	// (aliashdr_t *)Mod_Extradata (m);

	//
	// look for a cached version
	//
	strcpy (cache, "glquake/");
	COM_StripExtension (m->name+strlen("progs/"), cache+strlen("glquake/"));
	strcat (cache, ".ms2");

	COM_FOpenFile (cache, &f);	
	if (f)
	{
		fread (&numcommands, 4, 1, f);
		fread (&numorder, 4, 1, f);
		fread (&commands, numcommands * sizeof(commands[0]), 1, f);
		fread (&vertexorder, numorder * sizeof(vertexorder[0]), 1, f);
		fclose (f);
	}
	else
	{
		//
		// build it from scratch
		//
		Con_Printf ("meshing %s...\n",m->name);

		BuildTris ();		// trifans or lists

		//
		// save out the cached version
		//
		sprintf (fullpath, "%s/%s", com_gamedir, cache);
		f = fopen (fullpath, "wb");
		if (f)
		{
			fwrite (&numcommands, 4, 1, f);
			fwrite (&numorder, 4, 1, f);
			fwrite (&commands, numcommands * sizeof(commands[0]), 1, f);
			fwrite (&vertexorder, numorder * sizeof(vertexorder[0]), 1, f);
			fclose (f);
		}
	}


	// save the data out

	paliashdr->poseverts = numorder;

	cmds = Hunk_Alloc (numcommands * 4);
	paliashdr->commands = (byte *)cmds - (byte *)paliashdr;
	memcpy (cmds, commands, numcommands * 4);

	verts = Hunk_Alloc (paliashdr->numposes * paliashdr->poseverts 
		* sizeof(trivertx_t) );
	paliashdr->posedata = (byte *)verts - (byte *)paliashdr;
	for (i=0 ; i<paliashdr->numposes ; i++)
		for (j=0 ; j<numorder ; j++)
			*verts++ = poseverts[i][vertexorder[j]];
}


extern float	shadelight, ambientlight;

byte		alias_mins[4];
byte		alias_maxs[4];

void GL_MakeAliasModelSpriteSheets (model_t *mod, aliashdr_t *phdr)
{
	static byte *sheetbuf = NULL;
	static byte *imgbuf = NULL;
	static byte *img2buf = NULL;
	char tname[256];
	byte	bsphere[4];
	float scale;
	int cellcnt, cellpix, sheetsz;
	int x0, y0, x1, y1, z0, z1, pose;
	int cr, cg, cb, ctot;
	int i, j, k;
	
	bsphere[0]=(alias_mins[0]+alias_maxs[0])/2;
	bsphere[1]=(alias_mins[1]+alias_maxs[1])/2;
	bsphere[2]=(alias_mins[2]+alias_maxs[2])/2;

	x1=alias_maxs[0]-bsphere[0];
	y1=alias_maxs[1]-bsphere[1];
	z1=alias_maxs[2]-bsphere[2];

	k=x1;
	if(y1>k)k=y1;
	if(z1>k)k=z1;
	bsphere[3]=k;

	memcpy(phdr->spr_bound, bsphere, 4);

#if 1
//	Con_Printf("Model: %s %d frames\n", mod->name, phdr->numposes);
	
	cellcnt=ceil(sqrt(phdr->numposes));
	k=32*cellcnt;
	if(k>256)k=256;
	cellpix=k/cellcnt;
	
	k=cellcnt*cellpix;
	j=2;
	while(j<k)
		j=j<<1;
	if(j>256)j=256;
	sheetsz=j;
	cellpix=j/cellcnt;
	
//	Con_Printf("Model: %s cells=%d, cellpix=%d, sheet=%d\n",
//		mod->name, cellcnt, cellpix, sheetsz);
	
	phdr->spr_cellcnt=cellcnt;
	phdr->spr_cellpix=cellpix;
	phdr->spr_sheetsz=sheetsz;
#endif

	for(i=0; i<3; i++)
	{
		sprintf (tname, "%s_sprs_%i", mod->name, i);
		k=GL_LoadTexture32(tname, sheetsz, sheetsz, NULL, 1, 1);
		phdr->spr_texnum[i]=k;
		if(k<0)
			break;
	}
	
	if(i>=3)
	{
		return;
	}

#ifdef USE_TKRA
	phdr->spr_texnum[0]=-1;
	phdr->spr_texnum[1]=-1;
	phdr->spr_texnum[2]=-1;
	return;
#endif

// #ifndef __BJX2__
#ifndef USE_TKRA

	Con_Printf("GL_MakeAliasModelSpriteSheets: "
			"%s cells=%d, cellpix=%d, sheet=%d\n",
		mod->name, cellcnt, cellpix, sheetsz);

	if(!sheetbuf)
	{
		sheetbuf=malloc(1024*1024*4);
		imgbuf=malloc(256*256*4);
		img2buf=malloc(256*256*4);
	}

//	scale=DotProduct(phdr->scale, phdr->scale);

//	qglClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	qglDepthFunc (GL_LEQUAL);
	qglDepthRange (0, 1);

	qglViewport(0, 0, 256, 256);

	qglMatrixMode(GL_PROJECTION);
	qglLoadIdentity ();
//	qglOrtho  (0, vid.width, vid.height, 0, -99999, 99999);
//	qglFrustum( -1,  1, -1, 1, 1, 1024 );

//	qglOrtho  (-bsphere[3], bsphere[3], -bsphere[3], bsphere[3], -99999, 99999);
	qglOrtho  (-bsphere[3], bsphere[3], bsphere[3], -bsphere[3], -99999, 99999);

//	qglRotatef (-90,  1, 0, 0);		// put Z going up
//	qglRotatef (90,  0, 0, 1);		// put Z going up

//		qglTranslatef (0,  256,  -128);
//	qglTranslatef (0,  0,  -128);
//	qglTranslatef (-128,  0,  -128);
//	qglTranslatef (-128,  128,  -128);
//	qglTranslatef (-bsphere[0],  bsphere[3],  -bsphere[2]);
//		qglTranslatef (0,  2,  0);
//	qglRotatef (i*90,  0, 0, 1);

//	qglMatrixMode(GL_MODELVIEW);
//	qglLoadIdentity ();

//	qglRotatef (-90,  1, 0, 0);		// put Z going up
//	qglRotatef (90,  0, 0, 1);		// put Z going up
//	qglRotatef (-r_refdef.viewangles[2],  1, 0, 0);
//	qglRotatef (-r_refdef.viewangles[0],  0, 1, 0);
//	qglRotatef (-r_refdef.viewangles[1],  0, 0, 1);
//	qglTranslatef (-r_refdef.vieworg[0],  -r_refdef.vieworg[1],  -r_refdef.vieworg[2]);

//	qglTranslatef (0,  256,  0);

//	qglDisable (GL_DEPTH_TEST);
	qglEnable (GL_DEPTH_TEST);
	qglDisable (GL_CULL_FACE);
	qglDisable (GL_BLEND);
//	qglEnable (GL_ALPHA_TEST);
	qglDisable (GL_ALPHA_TEST);

	qglEnable (GL_TEXTURE_2D);

//	qglColor4f (1,1,1,1);
//	shadelight = 0.64;

	GL_Bind(phdr->gl_texturenum[0][0]);

//	for(i=0; i<8; i++)
//	for(i=0; i<4; i++)
//	for(i=0; i<6; i++)
	for(i=0; i<3; i++)
	{
		qglClearColor(0, 1, 1, 1);
		qglClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		qglMatrixMode(GL_MODELVIEW);
		qglLoadIdentity ();

		qglEnable (GL_TEXTURE_2D);
		GL_Bind(phdr->gl_texturenum[0][0]);

//		qglTranslatef (-bsphere[0], -bsphere[1],  -bsphere[2]);

//		if(i<4)
		if(i<2)
		{
			qglRotatef (-90.0,  1.0, 0.0, 0.0);
//			qglRotatef (i*90.0,  0.0, 0.0, 1.0);
			qglRotatef (-i*90.0,  0.0, 0.0, 1.0);
//			qglRotatef (i*90,  0, 1, 0);
		}else
		{
//			qglRotatef ((i&1)?180.0:0.0,  1.0, 0.0, 0.0);
//			qglRotatef (180.0,  1.0, 0.0, 0.0);
		}

		qglTranslatef (-bsphere[0], -bsphere[1],  -bsphere[2]);

//		qglTranslatef (0, 2*bsphere[3],  0);

		for(j=0; j<cellcnt; j++)
			for(k=0; k<cellcnt; k++)
		{
			pose=j*cellcnt+k;
			if(pose>=phdr->numposes)
				continue;

			x0=k*cellpix;
			y0=j*cellpix;

//			qglViewport(0, 0, 256, 256);
			
			qglViewport(x0, y0, cellpix, cellpix);

			qglColor4f (1,1,1,1);
//			shadelight = 0.64;
			shadelight = 0.90;

			GL_DrawAliasFrame (phdr, pose);

#if 0
			qglReadPixels(0, 0, 256, 256,
				GL_RGBA, GL_UNSIGNED_BYTE,
				imgbuf);
			
			GL_ResampleTexture(imgbuf, 256, 256, img2buf, cellpix, cellpix);
			for(y1=0; y1<cellpix; y1++)
				for(x1=0; x1<cellpix; x1++)
			{
				sheetbuf[]
			}
#endif
		}
		qglFinish();

//		qglViewport(0, 0, sheetsz, sheetsz);
		qglViewport(0, 0, 256, 256);
		
		qglReadPixels(0, 0, sheetsz, sheetsz,
			GL_RGBA, GL_UNSIGNED_BYTE,
			sheetbuf);
		
		cr=0; cg=0; cb=0; ctot=0;
		for(j=0; j<(sheetsz*sheetsz); j++)
		{
			if(	(sheetbuf[j*4+0]<   8) &&
				(sheetbuf[j*4+1]>=248) &&
				(sheetbuf[j*4+2]>=248))
			{
				sheetbuf[j*4+3]=0;
			}else
			{
				cr+=sheetbuf[j*4+0];
				cg+=sheetbuf[j*4+1];
				cb+=sheetbuf[j*4+2];
				ctot++;
			}
		}
		if(ctot>0)
		{
			cr/=ctot;
			cg/=ctot;
			cb/=ctot;

			for(j=0; j<(sheetsz*sheetsz); j++)
			{
				if(sheetbuf[j*4+3]<128)
				{
					sheetbuf[j*4+0]=cr;
					sheetbuf[j*4+1]=cg;
					sheetbuf[j*4+2]=cb;
				}
			}
		}
		
		sprintf (tname, "%s_sprs_%i", mod->name, i);
		phdr->spr_texnum[i]=
			GL_LoadTexture32(tname, sheetsz, sheetsz, sheetbuf, 1, 1);
	}

	qglClearColor(1, 0, 0, 1);
#endif

}
