#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include <bgbrasw.h>

#include <GL/gl.h>
#include <GL/glext.h>

int main(int argc, char *argv[])
{
	RASWGL_Context *ctx;
	bgbrasw_pixel *tbuf;
	float x, y;
	int txs, tys, tn;

	ctx=RaGlCreateContext(1440, 900, 0);
	RaGlSetContext(ctx);
	
	glClearColor(0, 255, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, 1440, 900);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

//	glOrtho(-720, 720, -450, 450, -9000, 9000);
	glFrustum(-1, 1, -1, 1, 1, 9000);
//	glTranslatef(0,0,-1000);
//	glTranslatef(0,0,100);
	glTranslatef(0,0,-1000);
	
	glEnable(GL_DEPTH_TEST);

	tbuf=BGBRASW_Img_LoadTGAPixel("fixed_16px.tga", &txs, &tys);
	glGenTextures(1, &tn);
	printf("%d %d -> Tn:%d\n", txs, tys, tn);
	glBindTexture(GL_TEXTURE_2D, tn);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, txs, tys, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, tbuf);

	glDisable(GL_TEXTURE_2D);
//	glEnable(GL_TEXTURE_2D);

	glBegin(GL_TRIANGLES);

	glTexCoord2f(0, 0);
	glColor4f(1, 0, 0, 1);
	glVertex2f(-360, -240);

	glTexCoord2f(1, 0);
	glColor4f(0, 0, 1, 1);
	glVertex2f(360, -240);

	glTexCoord2f(0.5, 1);
	glColor4f(1, 0, 1, 1);
	glVertex2f(0, 240);

	glEnd();

//	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);

	x=512; y=0;

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glColor4f(0, 1, 1, 1);
	glVertex2f(-360+x, 0+y);

	glTexCoord2f(1, 0);
	glColor4f(1, 0, 1, 1);
	glVertex2f(0+x, -240+y);

	glTexCoord2f(1, 1);
	glColor4f(1, 1, 0, 1);
	glVertex2f(360+x, 0+y);

	glTexCoord2f(0, 1);
	glColor4f(0, 0, 0, 1);
	glVertex2f(0+x, 240+y);

	glEnd();


//	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);

	x=512; y=256;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glColor4f(0, 1, 1, 0.75);
	glVertex2f(-360+x, 0+y);

	glTexCoord2f(1, 0);
	glColor4f(1, 0, 1, 0.75);
	glVertex2f(0+x, -240+y);

	glTexCoord2f(1, 1);
	glColor4f(1, 1, 0, 0.75);
	glVertex2f(360+x, 0+y);

	glTexCoord2f(0, 1);
	glColor4f(1, 1, 1, 0.75);
	glVertex2f(0+x, 240+y);

	glEnd();


//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);

	glDisable(GL_BLEND);
//	glColor4f(1, 1, 1, 1);
	glColor4f(1, 0, 1, 1);

	x=0; y=0;
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
//	glColor4f(0, 1, 1, 0.75);
	glVertex2f(-128+x, -128+y);

	glTexCoord2f(1, 0);
//	glColor4f(1, 0, 1, 0.75);
	glVertex2f(128+x, -128+y);

	glTexCoord2f(1, 1);
//	glColor4f(1, 1, 0, 0.75);
	glVertex2f(128+x, 128+y);

	glTexCoord2f(0, 1);
//	glColor4f(1, 1, 1, 0.75);
	glVertex2f(-128+x, 128+y);

	glEnd();


	BGBRASW_Img_SaveTGAPixel("bgbraswgltst0.tga",
		ctx->ractx->tgt_rgba, ctx->ractx->tgt_xs, ctx->ractx->tgt_ys);
}
