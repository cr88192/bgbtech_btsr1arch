/*
Text Editor Context

Lines are stored in a text-buffer, which functions as a collection of string buffers. The lines will be stored without line endings.

If a line expands beyond its allocated space, it will be reallocated in the text buffer. Periodically, the buffer may be repacked or expanded if more space is needed.
*/

#define TKEDIT_LETYPE_UNK	0	//Unknown
#define TKEDIT_LETYPE_LF	1	//LF
#define TKEDIT_LETYPE_CRLF	2	//CR+LF
#define TKEDIT_LETYPE_CR	3	//CR

#define TKEDIT_LENC_UNK		0	//Unknown / Binary
#define TKEDIT_LENC_ASCII	1	//ASCII (Plain)
#define TKEDIT_LENC_UTF8	2	//UTF8
#define TKEDIT_LENC_ASCIICP	3	//ASCII / CP1252 / ...
#define TKEDIT_LENC_UTF16L	4	//UTF16 (LE)
#define TKEDIT_LENC_UTF16B	5	//UTF16 (BE)


typedef struct TKEdit_EditContext_s TKEdit_EditContext;
typedef unsigned short tkedit_char;
typedef unsigned short tkedit_len;
typedef unsigned char tkedit_byte;

struct TKEdit_EditContext_s {
char *filename;
int n_lines;			//number of lines in context
int m_lines;			//max lines in context
tkedit_byte letype;		//line ending type
tkedit_byte lenc;		//file encoding

int *line_pos;				//offset to start of line in text buffer
tkedit_len *line_len;		//current length of line in text buffer
tkedit_len *line_max;		//max length of line in text buffer

tkedit_char *textbuf_buf;	//buffer holding text strings
int textbuf_pos;			//current position in buffer
int textbuf_size;			//size of text buffer
};
