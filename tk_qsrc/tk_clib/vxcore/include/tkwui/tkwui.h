#ifndef __TKWUI_TKWUI_H
#define __TKWUI_TKWUI_H

#include <tkgdi/tkgdi.h>

typedef struct TKWUI_ProgramContext_s TKWUI_ProgramContext;
typedef struct TKWUI_WindowContext_s TKWUI_WindowContext;
typedef struct TKWUI_FormLayout_s TKWUI_FormLayout;
typedef struct TKWUI_FormState_s TKWUI_FormState;
typedef struct TKWUI_WidgetVt_s TKWUI_WidgetVt;
typedef struct TKWUI_WidgetInfo_s TKWUI_WidgetInfo;
typedef struct TKWUI_WidgetState_s TKWUI_WidgetState;

struct TKWUI_ProgramContext_s {
TKWUI_WindowContext *firstwindow;
TKWUI_FormLayout *firstlayout;
TKWUI_FormState *formstate;				//form state (global)
};

/* Represents a visible window.
 * Stores information about the bitmap and state of the UI forms.
 */
struct TKWUI_WindowContext_s {
TKWUI_ProgramContext *pgm;				//program context
TKWUI_WindowContext *next;				//next window (if multiple)
TKGHDC hdc;								//handle for TKGDI window
char *name;								//symbolic name for window
TKGDI_BITMAPINFOHEADER *bmpinfo;		//bitmap info for window
byte *bmpdata;							//backing buffer for window
TKWUI_FormLayout *activeform;			//active UI form
TKWUI_FormState *formstate;				//active form state
};

/* The form will describe the layout of the widgets in the window.
 * The form will not contain the actual working state of the widgets.
 */
struct TKWUI_FormLayout_s {
TKWUI_FormLayout *next;			//next layout
TKWUI_WidgetInfo *widget;		//widget definitions
char *name;						//form name
};

struct TKWUI_FormState_s {
TKWUI_WidgetState *state;	//widget shared states
};

struct TKWUI_WidgetVt_s {
void *resv0;
void *resv1;
void *resv2;
void *resv3;
void (*Draw)(TKWUI_WidgetInfo *self, TKWUI_WindowContext *win);
void (*Click)(TKWUI_WidgetInfo *self, TKWUI_WindowContext *win,
	int relx, int rely, int flags);
void (*KeyInput)(TKWUI_WidgetInfo *self, TKWUI_WindowContext *win, int key);
};

struct TKWUI_WidgetInfo_s {
TKWUI_WidgetVt *vt;
TKWUI_WidgetInfo *next;		//next widget in linked list
TKWUI_WidgetInfo *first;	//first child widget
int org_x, org_y;			//origin in pixels
int size_x, size_y;			//bounding size in pixels
int data_id;				//ID number for widget state data
char *name;					//widget symbolic name
char *title;				//widget title
};

struct TKWUI_WidgetState_s {
TKWUI_WidgetState *next;	//next widget in linked list
int data_id;				//ID number for widget state data
u64 wflags;					//widget specific flags
void *wdata;				//widget specific data
u64 uflags;					//application specific flags
void *udata;				//application specific data
};

#endif
