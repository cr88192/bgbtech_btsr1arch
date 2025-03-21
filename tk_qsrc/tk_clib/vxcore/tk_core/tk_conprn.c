// byte tk_con_x;
// byte tk_con_y;
// byte tk_con_ena;
// volatile u32 *tk_con->buf;

struct conparm_s {
byte x;
byte y;
byte ena;
byte resv_1;
//byte attr;
//byte fgclr;
//byte bgclr;
//byte resv_2;
//byte resv_3;
// volatile u32 *buf;
u64 buf_addr;
u32 text_attr;
u32 text_attr_dfl;
u16 fgclr_555;
u16 bgclr_555;
};

struct conparm_s tk_con_bss;

struct conparm_s *tk_con=NULL;


void TK_Con_SetColorFg(int x);
void TK_Con_SetColorBg(int x);
void tk_con_clear();


u64 tk_gfxcon_glyphs[128]=
{
0x0000000000000000,	//0x00
0x7c82aa82aaba827c,	//0x01
0x007effdbffdbc37e,	//0x02
0x00367f7f7f3e1c08,	//0x03
0x00081c3e3e3e1c08,	//0x04
0x1c1c087f6b08087f,	//0x05
0x081c3e7f7f7f087f,	//0x06
0x00001c3e3e1c0000,	//0x07
0x3f3f332121333f3f,	//0x08
0x000e1111110e0000,	//0x09
0x3f3f332d2d333f3f,	//0x0A
0x000f030539484830,	//0x0B
0x1c2222221c083e08,	//0x0C
0x00040605040c1c18,	//0x0D
0x0c0e0b0909393307,	//0x0E
0x08492a1c7f1c2a49,	//0x0F
0x0000303c3e3c3000,	//0x10
0x00000c3c7c3c0c00,	//0x11
0x183c7e18187e3c18,	//0x12
0x0000121212001200,	//0x13
0x00071d3d1d050505,	//0x14
0x0007080e0907010e,	//0x15
0x0000000f0f000000,	//0x16
0x081c08081c08003e,	//0x17
0x0000081c2a080808,	//0x18
0x00000808082a1c08,	//0x19
0x000004023f020400,	//0x1A
0x000008103f100800,	//0x1B
0x0000000808080f00,	//0x1C
0x00002442ff422400,	//0x1D
0x0000081c1c3e3e00,	//0x1E
0x00003e3e1c1c0800,	//0x1F

#if 0
0x0000000000000000,	//0x20 " "
0x0010101010001000,	//0x21 "!"
0x0044444400000000,	//0x22 "\""
0x0044FF4444FF4400,	//0x23 "#"
0x103C403C047C1000,	//0x24 "$"
0x0044481020444400,	//0x25 "%"
0x1C241C384F443A00,	//0x26 "&"
0x0010101000000000,	//0x27 "'"
0x0008101010100800,	//0x28 "("
0x0020101010102000,	//0x29 ")"
0x00443CFF3C440000,	//0x2A "*"
0x0010107E10100000,	//0x2B "+"
0x0000000000101020,	//0x2C ","
0x0000007E00000000,	//0x2D "-"
0x0000000000101000,	//0x2E "."
0x0002040810204000,	//0x2F "/"
0x0038444C74443800,	//0x20 "0"
0x0010301010107E00,	//0x31 "1"
0x003C440810207E00,	//0x32 "2"
0x007C081008443C00,	//0x33 "3"
0x00081828487C0800,	//0x34 "4"
0x007E407C04443C00,	//0x35 "5"
0x003C407C44443800,	//0x36 "6"
0x007E040810202000,	//0x37 "7"
0x0038443844443800,	//0x38 "8"
0x003C443E04083000,	//0x39 "9"
0x0000101000101000,	//0x3A ":"
0x0000101000101020,	//0x3B ";"
0x0408102010080400,	//0x3C "<"
0x00007E00007E0000,	//0x3D "="
0x4020100810204000,	//0x3E ">"
0x003C440810001000,	//0x3F "?"
0x003C446E6E403E00,	//0x40 "@"
0x001038447C444400,	//0x41 "A"
0x0078447844447800,	//0x42 "B"
0x0038444040443800,	//0x43 "C"
0x0070484444487000,	//0x44 "D"
0x007E407C40407E00,	//0x45 "E"
0x007E407C40404000,	//0x46 "F"
0x003E40404E443C00,	//0x47 "G"
0x0044447C44444400,	//0x48 "H"
0x007E101010107E00,	//0x49 "I"
0x0004040404443800,	//0x4A "J"
0x0044487870484400,	//0x4B "K"
0x0040404040407E00,	//0x4C "L"
0x0042667E5A424200,	//0x4D "M"
0x00446454544C4400,	//0x4E "N"
0x0038444444443800,	//0x4F "O"
0x0078444478404000,	//0x50 "P"
0x003C444444483400,	//0x51 "Q"
0x0078444478484400,	//0x52 "R"
0x003C403C04047800,	//0x53 "S"
0x007E101010101000,	//0x54 "T"
0x0044444444443800,	//0x55 "U"
0x0044444444381000,	//0x56 "V"
0x0042425A5A664200,	//0x57 "W"
0x0044443838444400,	//0x58 "X"
0x0044443810101000,	//0x59 "Y"
0x007C081020407C00,	//0x5A "Z"
0x001E101010101E00,	//0x5B "["
0x0040201008040200,	//0x5C "\\"
0x0078080808087800,	//0x5D "]"
0x0008142200000000,	//0x5E "^"
0x000000000000FF00,	//0x5F "_"
0x0020201000000000,	//0x40 "`"
0x000038043C443E00,	//0x61 "a"
0x0040407844447800,	//0x62 "b"
0x0000384440443800,	//0x42 "c"
0x0004043C44443C00,	//0x64 "d"
0x000038447E403C00,	//0x65 "e"
0x000E103E10101000,	//0x44 "f"
0x00003844443C0478,	//0x67 "g"
0x0040407844444400,	//0x68 "h"
0x0010003010103800,	//0x69 "i"
0x0004000404040438,	//0x6A "j"
0x0040444870484400,	//0x6B "k"
0x0030101010103800,	//0x48 "l"
0x00007C5252524200,	//0x6D "m"
0x0000784444444400,	//0x6E "n"
0x0000384444443800,	//0x6F "o"
0x0000784444784040,	//0x70 "p"
0x00003C44443C0404,	//0x71 "q"
0x00406E7040404000,	//0x72 "r"
0x00003C4038047800,	//0x73 "s"
0x00107E1010100E00,	//0x74 "t"
0x0000444444443800,	//0x75 "u"
0x0000444444381000,	//0x76 "v"
0x00004252523C2400,	//0x77 "w"
0x0000442810284400,	//0x78 "x"
0x0000444444380870,	//0x79 "y"
0x00007C0810207C00,	//0x7A "z"
0x000E107010100E00,	//0x7B "{"
0x0010100010101000,	//0x7C "|"
0x0070100E10107000,	//0x7D "}"
0x0077DC0000000000,	//0x7E "~"
0x000000081C36427F,	//0x7F "\x7F"
#endif

#if 1
0x0000000000000000, /* 20 ' ' */
0x0010101010001000, /* 21 '!' */
0x0044444400000000, /* 22 '"' */
0x0044FE4444FE4400, /* 23 '#' */
0x103C403804781000, /* 24 '$' */
0x0044481024440000, /* 25 '%' */
0x18241C384F443A00, /* 26 '&' */
0x0010101000000000, /* 27 ''' */
0x0008101010100800, /* 28 '(' */
0x0020101010102000, /* 29 ')' */
0x004438FE38440000, /* 2A '*' */
0x0010107C10100000, /* 2B '+' */
0x0000000000101020, /* 2C ',' */
0x0000007E00000000, /* 2D '-' */
0x0000000000101000, /* 2E '.' */
0x0002040810204000, /* 2F '/' */
0x0038444C74443800, /* 30 '0' */
0x0010301010107C00, /* 31 '1' */
0x0038440810207C00, /* 32 '2' */
0x007C081008443800, /* 33 '3' */
0x00081828487C0800, /* 34 '4' */
0x007C407804443800, /* 35 '5' */
0x003C407844443800, /* 36 '6' */
0x007E040810202000, /* 37 '7' */
0x0038443844443800, /* 38 '8' */
0x0038443C04083000, /* 39 '9' */
0x0000101000101000, /* 3A ':' */
0x0000101000101020, /* 3B ';' */
0x0408102010080400, /* 3C '<' */
0x00007E00007E0000, /* 3D '=' */
0x4020100810204000, /* 3E '>' */
0x0038440810001000, /* 3F '?' */
0x00384C545C403C00, /* 40 '@' */
0x001028447C444400, /* 41 'A' */
0x0078447844447800, /* 42 'B' */
0x0038444040443800, /* 43 'C' */
0x0070484444487000, /* 44 'D' */
0x007C407840407C00, /* 45 'E' */
0x007C407840404000, /* 46 'F' */
0x003844404E443C00, /* 47 'G' */
0x0044447C44444400, /* 48 'H' */
0x0038101010103800, /* 49 'I' */
0x0004040404443800, /* 4A 'J' */
0x0044487048444400, /* 4B 'K' */
0x0040404040407C00, /* 4C 'L' */
0x0042665A5A424200, /* 4D 'M' */
0x00446454544C4400, /* 4E 'N' */
0x0038444444443800, /* 4F 'O' */
0x0078444478404000, /* 50 'P' */
0x0038444444483400, /* 51 'Q' */
0x0078444478484400, /* 52 'R' */
0x003C403804047800, /* 53 'S' */
0x007C101010101000, /* 54 'T' */
0x0044444444443800, /* 55 'U' */
0x0044444444281000, /* 56 'V' */
0x0042425A5A664200, /* 57 'W' */
0x0044281010284400, /* 58 'X' */
0x0044442810101000, /* 59 'Y' */
0x007C081020407C00, /* 5A 'Z' */
0x001E101010101E00, /* 5B '[' */
0x0040201008040200, /* 5C '\' */
0x0078080808087800, /* 5D ']' */
0x0008142200000000, /* 5E '^' */
0x000000000000FF00, /* 5F '_' */
0x0020201000000000, /* 60 '`' */
0x000038043C443E00, /* 61 'a' */
0x0040407844447800, /* 62 'b' */
0x0000384440443800, /* 63 'c' */
0x0004043C44443C00, /* 64 'd' */
0x000038447C403C00, /* 65 'e' */
0x000E103E10101000, /* 66 'f' */
0x00003844443C0478, /* 67 'g' */
0x0040407844444400, /* 68 'h' */
0x0010003010103800, /* 69 'i' */
0x0004000404040438, /* 6A 'j' */
0x0040444870484400, /* 6B 'k' */
0x0030101010103800, /* 6C 'l' */
0x00007C5252524200, /* 6D 'm' */
0x0000784444444400, /* 6E 'n' */
0x0000384444443800, /* 6F 'o' */
0x0000784444784040, /* 70 'p' */
0x00003C44443C0404, /* 71 'q' */
0x00406E7040404000, /* 72 'r' */
0x00003C4038047800, /* 73 's' */
0x00107E1010100E00, /* 74 't' */
0x0000444444443800, /* 75 'u' */
0x0000444444281000, /* 76 'v' */
0x00004252523C2400, /* 77 'w' */
0x0000442810284400, /* 78 'x' */
0x0000444444380870, /* 79 'y' */
0x00007C0810207C00, /* 7A 'z' */
0x000C106010100C00, /* 7B '{' */
0x0010100010101000, /* 7C '|' */
0x0060100C10106000, /* 7D '}' */
0x0076DC0000000000, /* 7E '~' */
0x000000001824427E  /* 7F ' ' */
#endif

};

u64 tk_gfxcon_glyphs_lat1ext[128]=
{
#if 1
0x003844F040443800, /* 80 ' ' */
0x0000000000000000, /* 81 ' ' */
0x0000000000080800, /* 82 ' ' */
0x000C103810106000, /* 83 ' ' */
0x00000000000A0A00, /* 84 ' ' */
0x0000000000005400, /* 85 ' ' */
0x0010381010101000, /* 86 ' ' */
0x00081C081C080800, /* 87 ' ' */
0x0010284400000000, /* 88 ' ' */
0x0000501020405400, /* 89 ' ' */
0x28103C4038047800, /* 8A ' ' */
0x0000000008100800, /* 8B ' ' */
0x003E505C50503E00, /* 8C ' ' */
0x0000000000000000, /* 8D ' ' */
0x28107C0810207C00, /* 8E ' ' */
0x0000000000000000, /* 8F ' ' */
0x0038444040443800, /* 90 ' ' */
0x0008101800000000, /* 91 ' ' */
0x0018081000000000, /* 92 ' ' */
0x0024486C00000000, /* 93 ' ' */
0x0036122400000000, /* 94 ' ' */
0x0000103810000000, /* 95 ' ' */
0x0000007E00000000, /* 96 ' ' */
0x0000007E00000000, /* 97 ' ' */
0x2850000000000000, /* 98 ' ' */
0x00F15B5551000000, /* 99 ' ' */
0x28103C4038047800, /* 9A ' ' */
0x0000000010081000, /* 9B ' ' */
0x00003C525E503E00, /* 9C ' ' */
0x0000000000000000, /* 9D ' ' */
0x28107C0810207C00, /* 9E ' ' */
0x1400221408080800, /* 9F ' ' */
0x0000000000000000, /* A0 ' ' */
0x0010001010101010, /* A1 ' ' */
0x00081C2A282A1C08, /* A2 ' ' */
0x0008141038103800, /* A3 ' ' */
0x0084784848788400, /* A4 ' ' */
0x0044281038103810, /* A5 ' ' */
0x0000101000101000, /* A6 ' ' */
0x3C24302C340C243C, /* A7 ' ' */
0x2800000000000000, /* A8 ' ' */
0x38449AA2A29A4438, /* A9 ' ' */
0x18041C2418003C00, /* AA ' ' */
0x0028285050282800, /* AB ' ' */
0x000000003C040400, /* AC ' ' */
0x0000000078000000, /* AD ' ' */
0x7C82B2AAB2AA827C, /* AE ' ' */
0x003E000000000000, /* AF ' ' */
0x0010281000000000, /* B0 ' ' */
0x0010381000380000, /* B1 ' ' */
0x00180408101C0000, /* B2 ' ' */
0x0018041C04180000, /* B3 ' ' */
0x0008100000000000, /* B4 ' ' */
0x0000242438202000, /* B5 ' ' */
0x003C747434141400, /* B6 ' ' */
0x0000000010000000, /* B7 ' ' */
0x0000000020102000, /* B8 ' ' */
0x0008180808000000, /* B9 ' ' */
0x0018242418003C00, /* BA ' ' */
0x0050502828505000, /* BB ' ' */
0x4448481517212100, /* BC ' ' */
0x4448481612242600, /* BD ' ' */
0xC448C855D7212100, /* BE ' ' */
0x0008000810221C00, /* BF ' ' */
0x10080018243C2400, /* C0 ' ' */
0x08100018243C2400, /* C1 ' ' */
0x18240018243C2400, /* C2 ' ' */
0x14280018243C2400, /* C3 ' ' */
0x00280018243C2400, /* C4 ' ' */
0x00080018243C2400, /* C5 ' ' */
0x00001C283C282C00, /* C6 ' ' */
0x0018242020241830, /* C7 ' ' */
0x2010003820382038, /* C8 ' ' */
0x0810003820382038, /* C9 ' ' */
0x1028003820382038, /* CA ' ' */
0x0028003820382038, /* CB ' ' */
0x1008003810101038, /* CC ' ' */
0x0810003810101038, /* CD ' ' */
0x1028003810101038, /* CE ' ' */
0x0028003810101038, /* CF ' ' */
0x704844E444487000, /* D0 ' ' */
0x14280024342C2400, /* D1 ' ' */
0x1008182424241800, /* D2 ' ' */
0x0810182424241800, /* D3 ' ' */
0x1824182424241800, /* D4 ' ' */
0x1428182424241800, /* D5 ' ' */
0x2800182424241800, /* D6 ' ' */
0x0000000028102800, /* D7 ' ' */
0x041C242C34243820, /* D8 ' ' */
0x1008002424241800, /* D9 ' ' */
0x0810002424241800, /* DA ' ' */
0x1824002424241800, /* DB ' ' */
0x0028002424241800, /* DC ' ' */
0x0810242418080800, /* DD ' ' */
0x0020382424382000, /* DE ' ' */
0x0038242824282000, /* DF ' ' */
0x100818041C241C00, /* E0 ' ' */
0x081018041C241C00, /* E1 ' ' */
0x182418041C241C00, /* E2 ' ' */
0x142818041C241C00, /* E3 ' ' */
0x280018041C241C00, /* E4 ' ' */
0x080018041C241C00, /* E5 ' ' */
0x00006C127E906E00, /* E6 ' ' */
0x0000182420241830, /* E7 ' ' */
0x10080018243C201C, /* E8 ' ' */
0x08100018243C201C, /* E9 ' ' */
0x18240018243C201C, /* EA ' ' */
0x00280018243C201C, /* EB ' ' */
0x2010003010103800, /* EC ' ' */
0x1020003010103800, /* ED ' ' */
0x1028003010103800, /* EE ' ' */
0x0028003010103800, /* EF ' ' */
0x140814041C241800, /* F0 ' ' */
0x1428001824242400, /* F1 ' ' */
0x1008001824241800, /* F2 ' ' */
0x0810001824241800, /* F3 ' ' */
0x1824001824241800, /* F4 ' ' */
0x1428001824241800, /* F5 ' ' */
0x0028001824241800, /* F6 ' ' */
0x0000100038001000, /* F7 ' ' */
0x0000041C2C343820, /* F8 ' ' */
0x0010080024241800, /* F9 ' ' */
0x0008100024241800, /* FA ' ' */
0x0018240024241800, /* FB ' ' */
0x0000280024241800, /* FC ' ' */
0x0810002424180830, /* FD ' ' */
0x0000181414181000, /* FE ' ' */
0x0050004848301060  /* FF ' ' */
#endif
};

u64 tk_gfxcon_glyphs_437ext[128]=
{
0x000f0908090f020e,	//0x80 "?"
0x000a001212120c00,	//0x81 "?"
0x0618000e111f100f,	//0x82 "?"
0x0609000e010f110f,	//0x83 "?"
0x000a000e010f110f,	//0x84 "?"
0x0c03000e010f110f,	//0x85 "?"
0x0606000e010f110f,	//0x86 "?"
0x0000070808070206,	//0x87 "?"
0x040a000e111f100f,	//0x88 "?"
0x000a000e111f100f,	//0x89 "?"
0x0c03000e111f100f,	//0x8A "?"
0x000a000602020207,	//0x8B "?"
0x0205000602020207,	//0x8C "?"
0x0c03000602020207,	//0x8D "?"
0x09000e1b111f1111,	//0x8E "?"
0x0e000e1b111f1111,	//0x8F "?"
0x00000f080e080f00,	//0x90 "?"
0x00003c0a3e50522c,	//0x91 "?"
0x00003e487e484e00,	//0x92 "?"
0x040a000609090600,	//0x93 "?"
0x000a000609090600,	//0x94 "?"
0x0c03000609090600,	//0x95 "?"
0x040a000909090600,	//0x96 "?"
0x0c03000909090600,	//0x97 "?"
0x000a000909070106,	//0x98 "?"
0x0a00060909090906,	//0x99 "?"
0x0500090909090600,	//0x9A "?"
0x00040e1514150e04,	//0x9B "?"
0x000007040e040f00,	//0x9C "?"
0x110a041f041f0404,	//0x9D "?"
0x0000302b342a2906,	//0x9E "?"
0x0002040e04040418,	//0x9F "?"
0x030c000e010f110f,	//0xA0 "?"
0x0618000c04040e00,	//0xA1 "?"
0x030c000609090600,	//0xA2 "?"
0x030c000909090600,	//0xA3 "?"
0x050a000609090900,	//0xA4 "?"
0x050a00090d0f0b09,	//0xA5 "?"
0x18041c2418003e00,	//0xA6 "?"
0x0018242418003c00,	//0xA7 "?"
0x0008000810221c00,	//0xA8 "?"
0x000000000f080808,	//0xA9 "?"
0x00000000f8080808,	//0xAA "?"
0x4244485721470407,	//0xAB "?"
0x4244485525470101,	//0xAC "?"
0x0000020002020200,	//0xAD "?"
0x0014142828141400,	//0xAE "?"
0x0014140a0a141400,	//0xAF "?"
0xaa00aa00aa00aa00,	//0xB0 "?"
0xaa55aa55aa55aa55,	//0xB1 "?"
0xaaffaaffaaffaaff,	//0xB2 "?"
0x0808080808080808,	//0xB3 "?"
0x080808f808080808,	//0xB4 "?"
0x0808f808f8080808,	//0xB5 "?"
0x14141414f4141414,	//0xB6 "?"
0x00000000fc141414,	//0xB7 "?"
0x000000f808f80808,	//0xB8 "?"
0x141414f404f41414,	//0xB9 "?"
0x1414141414141414,	//0xBA "?"
0x000000fc04f41414,	//0xBB "?"
0x141414f404fc0000,	//0xBC "?"
0x14141414fc000000,	//0xBD "?"
0x0808083808380000,	//0xBE "?"
0x0000001c04040404,	//0xBF "?"
0x080808080f000000,	//0xC0 "?"
0x10101010ff000000,	//0xC1 "?"
0x00000000ff080808,	//0xC2 "?"
0x080808080f080808,	//0xC3 "?"
0x00000000ff000000,	//0xC4 "?"
0x08080808ff080808,	//0xC5 "?"
0x0808080f080f0808,	//0xC6 "?"
0x1414141417141414,	//0xC7 "?"
0x0808080f080f0000,	//0xC8 "?"
0x0000001f10171414,	//0xC9 "?"
0x141414f404fc0000,	//0xCA "?"
0x000000ff00f71414,	//0xCB "?"
0x1414141710171414,	//0xCC "?"
0x000000ff00ff0000,	//0xCD "?"
0x141414f700f71414,	//0xCE "?"
0x040404ff00ff0000,	//0xCF "?"
0x141414143e000000,	//0xD0 "?"
0x0000003e003e0808,	//0xD1 "?"
0x000000001f141414,	//0xD2 "?"
0x141414141f000000,	//0xD3 "?"
0x0808080f080f0000,	//0xD4 "?"
0x0000000f080f0808,	//0xD5 "?"
0x000000001f141414,	//0xD6 "?"
0x14141414f7141414,	//0xD7 "?"
0x080808ff08ff0808,	//0xD8 "?"
0x08080808ff000000,	//0xD9 "?"
0x000000000f080808,	//0xDA "?"
0xffffffffffffffff,	//0xDB "?"
0x00000000ffffffff,	//0xDC "?"
0xf0f0f0f0f0f0f0f0,	//0xDD "?"
0x0f0f0f0f0f0f0f0f,	//0xDE "?"
0xffffffff00000000,	//0xDF "?"
0x00023a4444443a02,	//0xE0 "?"
0x00382c243e22362c,	//0xE1 "?"
0x003c202020202000,	//0xE2 "?"
0x00003e1414141400,	//0xE3 "?"
0x003e10080408103e,	//0xE4 "?"
0x00001f2222221c00,	//0xE5 "?"
0x00000012121e1010,	//0xE6 "?"
0x0000003e08080806,	//0xE7 "?"
0x7f083e49493e087f,	//0xE8 "?"
0x001c22223e22221c,	//0xE9 "?"
0x001c222222221422,	//0xEA "?"
0x000704040e11110e,	//0xEB "?"
0x0000003649360000,	//0xEC "?"
0x0026494949493e08,	//0xED "?"
0x000609101e100906,	//0xEE "?"
0x00001c2222222222,	//0xEF "?"
0x00003e003e003e00,	//0xF0 "?"
0x0008083e0808003e,	//0xF1 "?"
0x0804020408001f00,	//0xF2 "?"
0x0204080402001f00,	//0xF3 "?"
0x0000030202020202,	//0xF4 "?"
0x0202020202060000,	//0xF5 "?"
0x000008003e000800,	//0xF6 "?"
0x0000324c00324c00,	//0xF7 "?"
0x0008141408000000,	//0xF8 "?"
0x000000000c0c0000,	//0xF9 "?"
0x000000000c000000,	//0xFA "?"
0x0007040424140c04,	//0xFB "?"
0x00000e0909090000,	//0xFC "?"
0x0007010704070000,	//0xFD "?"
0x0000001e1e1e1e00,	//0xFE "?"
};

u64 tk_gfxcon_glyphs_cyril0[128]=
{
0x081C000808080600, /* 00 ' ' */
0x001C221824241800, /* 01 ' ' */
0x0000000000000000, /* 02 ' ' */
0x003E202020202020, /* 03 ' ' */
0x0008141422223E00, /* 04 ' ' */
0x0000000000000000, /* 05 ' ' */
0x0000000000000000, /* 06 ' ' */
0x0000000000000000, /* 07 ' ' */
0x003E41415D41413E, /* 08 ' ' */
0x0000000000000000, /* 09 ' ' */
0x0000000000000000, /* 0A ' ' */
0x0008141422224141, /* 0B ' ' */
0x0000000000000000, /* 0C ' ' */
0x0000000000000000, /* 0D ' ' */
0x003E22001C00223E, /* 0E ' ' */
0x0000000000000000, /* 0F ' ' */
0x0000000000000000, /* 10 ' ' */
0x0000003E08080600, /* 11 ' ' */
0x0000000000000000, /* 12 ' ' */
0x003E20180418203E, /* 13 ' ' */
0x0000000000000000, /* 14 ' ' */
0x0000000000000000, /* 15 ' ' */
0x00083E4949493E08, /* 16 ' ' */
0x0000000000000000, /* 17 ' ' */
0x004949493E080808, /* 18 ' ' */
0x001C224141221477, /* 19 ' ' */
0x0000000000000000, /* 1A ' ' */
0x0014002214080808, /* 1B ' ' */
0x0808001A24241A00, /* 1C ' ' */
0x0000000000000000, /* 1D ' ' */
0x0808001C22222202, /* 1E ' ' */
0x0000000000000000, /* 1F ' ' */
0x081C002222221C00, /* 20 ' ' */
0x00001A2424241A00, /* 21 '!' */
0x00001C223C223C20, /* 22 '"' */
0x14001C223E201E00, /* 23 '#' */
0x00001C2218221C00, /* 24 '$' */
0x00000C1212120202, /* 25 '%' */
0x001E040810100C02, /* 26 '&' */
0x0014000808080800, /* 27 ''' */
0x001C22223E22221C, /* 28 '(' */
0x0000000808080800, /* 29 ')' */
0x0000121418141200, /* 2A '*' */
0x0030080C14222200, /* 2B '+' */
0x000012121E101000, /* 2C ',' */
0x0000043820202000, /* 2D '-' */
0x001C201C201C020C, /* 2E '.' */
0x0808000808080800, /* 2F '/' */
0x00001F0A0A0A1200, /* 30 '0' */
0x00001E2422221C00, /* 31 '1' */
0x001C2220100C021C, /* 32 '2' */
0x14003E203C203E00, /* 33 '3' */
0x001C22201C20221C, /* 34 '4' */
0x0000002222221C00, /* 35 '5' */
0x00002629291E0808, /* 36 '6' */
0x14001C0808081C00, /* 37 '7' */
0x00002A2A1C080800, /* 38 '8' */
0x000014222A2A1400, /* 39 '9' */
0x0000000000000000, /* 3A ':' */
0x0014001212120C00, /* 3B ';' */
0x0808001C22221C00, /* 3C '<' */
0x00023C2020202000, /* 3D '=' */
0x08080014222A1400, /* 3E '>' */
0x0404001212120C00, /* 3F '?' */
0x0000762939297600, /* 40 '@' */
0x000038043C443E00, /* 41 'A' */
0x001C203C22223C00, /* 42 'B' */
0x0000242424243E02, /* 43 'C' */
0x00000C14143E2200, /* 44 'D' */
0x00001C223E201E00, /* 45 'E' */
0x0008083E49493E08, /* 46 'F' */
0x0000003820202000, /* 47 'G' */
0x0000221408142200, /* 48 'H' */
0x000022262A322200, /* 49 'I' */
0x000822262A322200, /* 4A 'J' */
0x0000222438242200, /* 4B 'K' */
0x00003F1212122200, /* 4C 'L' */
0x000022362A222200, /* 4D 'M' */
0x000022223E222200, /* 4E 'N' */
0x00001C2222221C00, /* 4F 'O' */
0x00007E2424242400, /* 50 'P' */
0x00001E221E122200, /* 51 'Q' */
0x00003C22223C2020, /* 52 'R' */
0x00001C2220221C00, /* 53 'S' */
0x00003E0808080800, /* 54 'T' */
0x00002222221C0438, /* 55 'U' */
0x0000492A1C2A4900, /* 56 'V' */
0x0000382438243800, /* 57 'W' */
0x000020203C223C00, /* 58 'X' */
0x0000212139253900, /* 59 'Y' */
0x00001C220C221C00, /* 5A 'Z' */
0x00002A2A2A2A3E00, /* 5B '[' */
0x00001E210F211E00, /* 5C '\' */
0x00002A2A2A2A3F01, /* 5D ']' */
0x000024243C040400, /* 5E '^' */
0x000060203C223C00, /* 5F '_' */
0x0076292939297600, /* 60 '`' */
0x000814223E222200, /* 61 'a' */
0x003C203C22223C00, /* 62 'b' */
0x0024242424243E02, /* 63 'c' */
0x000C1424243E2200, /* 64 'd' */
0x003E203C20203E00, /* 65 'e' */
0x083E4949493E0800, /* 66 'f' */
0x003C202020202000, /* 67 'g' */
0x0022140808142200, /* 68 'h' */
0x0022262A2A322200, /* 69 'i' */
0x140822262A2A3222, /* 6A 'j' */
0x0022243824222200, /* 6B 'k' */
0x003F121212122200, /* 6C 'l' */
0x0021332D2D212100, /* 6D 'm' */
0x0022223E22222200, /* 6E 'n' */
0x001C222222221C00, /* 6F 'o' */
0x007E242424242400, /* 70 'p' */
0x001E22221E122200, /* 71 'q' */
0x003C22223C202000, /* 72 'r' */
0x001C222020221C00, /* 73 's' */
0x003E080808080800, /* 74 't' */
0x002222221C043800, /* 75 'u' */
0x00492A1C1C2A4900, /* 76 'v' */
0x003C223C22223C00, /* 77 'w' */
0x0020203C22223C00, /* 78 'x' */
0x0021213925253900, /* 79 'y' */
0x001C220C02221C00, /* 7A 'z' */
0x002A2A2A2A2A3E00, /* 7B '{' */
0x001E210F01211E00, /* 7C '|' */
0x002A2A2A2A2A3F01, /* 7D '}' */
0x000024243C040400, /* 7E '~' */
0x0060203C22223C00, /* 7F ' ' */
};

u64 tk_gfxcon_glyphs_gfx0[128]=
{
0x0000000000000000ULL,		//00
0x00005028D4FAF4EAULL,		//01
0x0000000000000303ULL,		//02
0x000000000000C0C0ULL,		//03
0x0707070707070707ULL,		//04
0xE0E0E0E0E0E0E0E0ULL,		//05
0x0101010101010101ULL,		//06
0x8080808080808080ULL,		//07
0x000101030307070FULL,		//08
0x008080C0C0E0E0F0ULL,		//09
0x0000000001071F7FULL,		//0A
0x01071F7FFFFFFFFFULL,		//0B
0xFFFEFCF8F0E0C080ULL,		//0C
0x0000001F1F181818ULL,		//0D
0x000000FFFF181818ULL,		//0E
0x000000F8F8181818ULL,		//0F
0xDAF4B2E8D4E8B2E8ULL,		//10
0x00228855AA77DDFFULL,		//11
0x0303000000000000ULL,		//12
0xC0C0000000000000ULL,		//13
0x0000000000FFFFFFULL,		//14
0xFFFFFF0000000000ULL,		//15
0x00000000000000FFULL,		//16
0xFF00000000000000ULL,		//17
0x0F1F1F3F3F7F7FFFULL,		//18
0xF0F8F8FCFCFEFEFFULL,		//19
0xFF3F0F0300000000ULL,		//1A
0xFFFFFFFF7F1F0701ULL,		//1B
0xFF7F3F1F0F070301ULL,		//1C
0x1818181F1F181818ULL,		//1D
0x181818FFFF181818ULL,		//1E
0x181818F8F8181818ULL,		//1F
0x000000070F1C1818ULL,		//20
0x000000E0F0381818ULL,		//21
0x000000000F0F0F0FULL,		//22
0x00000000F0F0F0F0ULL,		//23
0xCCCC3333CCCC3333ULL,		//24
0x00000000C0C03030ULL,		//25
0xF0F0F0F00F0F0F0FULL,		//26
0xAA55AA55AA55AA55ULL,		//27
0x02050A152B572F5FULL,		//28
0x40A050A8D4EAF5FAULL,		//29
0x0103070F1F3F7FFFULL,		//2A
0x80C0E0F0F8FCFEFFULL,		//2B
0x0000000000000000ULL,		//2C, Dummy
0x1818181F1F000000ULL,		//2D
0x181818FFFF000000ULL,		//2E
0x181818F8F8000000ULL,		//2F
0x18181C0F07000000ULL,		//30
0x181838F0E0000000ULL,		//31
0x0F0F0F0F00000000ULL,		//32
0xF0F0F0F000000000ULL,		//33
0x00000000CCCC3333ULL,		//34
0xC0C03030C0C03030ULL,		//35
0x00000000FFFFFFFFULL,		//36
0xF0F0F0F0F0F0F0F0ULL,		//37
0x00008855AA77FFFFULL,		//38
0xD4E8F0E8D4E8F0E8ULL,		//39
0xC0E070381C0E0703ULL,		//3A
0x03070E1C3870E0C0ULL,		//3B
0x0000183C3C180000ULL,		//3C
0xFFFFC0C0C0C0C0C0ULL,		//3D
0xFFFF000000000000ULL,		//3E
0xFFFF030303030303ULL,		//3F
0x000000070F1F1F1FULL,		//40
0x000000E0F0F8F8F8ULL,		//41
0x0000000000030707ULL,		//42
0x0000000000C0E0E0ULL,		//43
0x0000000000FFFFFFULL,		//44
0x000000FFFFFFFFFFULL,		//45
0x0000FFFFFFFFFFFFULL,		//46
0x00FFFFFFFFFFFFFFULL,		//47
0xFFFFFF7F3F1F0F07ULL,		//48
0xFFFFFFFEFCF8F0E0ULL,		//49
0xFFFFFFFFFF7F3F1FULL,		//4A
0xFFFFFFFFFFFEFCF8ULL,		//4B
0x003C7E66667E3C00ULL,		//4C
0xC0C0C0C0C0C0C0C0ULL,		//4D
0x0000000000000000ULL,		//4E
0x0303030303030303ULL,		//4F
0x1F1F1F0F07000000ULL,		//50
0xF8F8F8F0E0000000ULL,		//51
0x0707030000000000ULL,		//52
0xE0E0C00000000000ULL,		//53
0xE0E0E0E0E0E0E0E0ULL,		//54
0xF8F8F8F8F8F8F8F8ULL,		//55
0xFCFCFCFCFCFCFCFCULL,		//56
0xFEFEFEFEFEFEFEFEULL,		//57
0x3F1F0F0703010000ULL,		//58
0xFCF8F0E0C0800000ULL,		//59
0x0F07030100000000ULL,		//5A
0xF0E0C08000000000ULL,		//5B
0x003C7E7E7E7E3C00ULL,		//5C
0xC0C0C0C0C0C0FFFFULL,		//5D
0x000000000000FFFFULL,		//5E
0x030303030303FFFFULL,		//5F
0x00071F3F3F7F7F7FULL,		//60
0x00E0F8FCFCFEFEFEULL,		//61
0x0000000C1E1E0C00ULL,		//62
0x0000000378780300ULL,		//63
0x3C3C3C3C3C3C3C3CULL,		//64
0x0000FFFFFFFF0000ULL,		//65
0x0F1F3F7FFEFCF8F0ULL,		//66
0xF0F8FCFE7F3F1F0FULL,		//67
0x0000000000FFFF00ULL,		//68
0x00000000FFFF0000ULL,		//69
0x000000FFFF000000ULL,		//6A
0x0000FFFF00000000ULL,		//6B
0x00FFFF0000000000ULL,		//6C
0x367F7F7F3E1C8000ULL,		//6D
0x1818666618183C00ULL,		//6E
0xC3E77E3C3C7EE7C3ULL,		//6F
0x7F7F7F3F3F1F0700ULL,		//70
0xFEFEFEFCFCF8E000ULL,		//71
0x000C1E1E0C000000ULL,		//72
0x0003787803000000ULL,		//73
0x6666666666666666ULL,		//74
0x00FFFF0000FFFF00ULL,		//75
0x1C3871E3C78E1C38ULL,		//76
0x381C8EC7E371381CULL,		//77
0x6060606060606060ULL,		//78
0x3030303030303030ULL,		//79
0x1818181818181818ULL,		//7A
0x0C0C0C0C0C0C0C0CULL,		//7B
0x0606060606060606ULL,		//7C
0x081C3E7F7F1C3E00ULL,		//7D
0x081C3E7F3E1C0800ULL,		//7E
0x0000000000000000ULL,		//7F, Dummy
};

byte tk_con_clr16to64[16]={
	0x00, 0x20, 0x08, 0x28, 0x02, 0x22, 0x0A, 0x2A,
	0x00, 0x30, 0x0C, 0x3C, 0x03, 0x33, 0x0F, 0x3F };

u16 tk_con_clr64to555[64];

u64 tk_con_hexdig[16] = {
	0x0000020505050200ULL, /* 0 */
	0x0000060202020700ULL, /* 1 */
	0x0000060107040700ULL, /* 2 */
	0x0000060106010600ULL, /* 3 */
	0x0000050507010100ULL, /* 4 */
	0x0000070406010600ULL, /* 5 */
	0x0000060407050200ULL, /* 6 */
	0x0000070101010100ULL, /* 7 */
	0x0000020502050200ULL, /* 8 */
	0x0000020503010100ULL, /* 9 */
	0x0000020507050500ULL, /* A */
	0x0000060506050600ULL, /* B */
	0x0000030404040300ULL, /* C */
	0x0000060505050600ULL, /* D */
	0x0000070407040700ULL, /* E */
	0x0000070407040400ULL  /* F */
};

u64 tk_gfxcon_hexblock[256];


// #define TK_CONWIDTH		40
#define TK_CONWIDTH		80
#define TK_CONHEIGHT	25
#define TK_CONHEIGHTN1	24
#define TK_CONHEIGHTN2	23

u32 *TKGDI_BlitUpdate_GetConbuf();

u32 *tk_con_conbuf2;

extern int tkgdi_vid_scrmode;

void *TK_ConGetCtxV(void)
{
	TK_SysArg ar[4];
	void *p;
	
//	ar[0].i=sz;
	p=NULL;
	tk_syscall(NULL, TK_UMSG_GETCONPARAM, &p, ar);
	return(p);
}

void *tk_con_getctx(void)
{
	return(tk_con);
}

void tk_con_uploadglyph(int idx, u64 tv)
{
	u64 ta, tb;
	
	ta=(u32)(tv>> 0);
	tb=(u32)(tv>>32);
	ta|=((u64)idx)<<32;
	tb|=((u64)idx)<<32;
	*((u64 *)0xFFFFF00BFF30ULL)=ta;
	*((u64 *)0xFFFFF00BFF38ULL)=tb;
}

void tk_con_init()
{
	u64 tv;
	int cr, cg, cb;
	int i, j, k;

	if(tk_iskernel())
	{
//		tk_con=&tk_con_bss;
		tk_con=NULL;
	}else
	{
		tk_con=TK_ConGetCtxV();
//		if(!tk_con)
//			tk_con=&tk_con_bss;
	}

	if(!tk_con)
	{
		tk_con=&tk_con_bss;
//		tk_con->buf=(u32 *)0xF00A0000UL;
//		tk_con->buf=(u32 *)(MMIO_BASE+0x000A0000ULL);
		tk_con->buf_addr=(MMIO_BASE+0x000A0000ULL);
		tk_con->x=0;
		tk_con->y=0;
		tk_con->ena=1;
//		tk_con->attr=0;
//		tk_con->fgclr=0x3F;
//		tk_con->bgclr=0x00;
//		tk_con->text_attr=0x003F0000;
//		tk_con->text_attr_dfl=0x003F0000;
		tk_con->text_attr_dfl=0x002A0000;
		tk_con->text_attr=tk_con->text_attr_dfl;

		TK_Con_SetColorBg(0);
		TK_Con_SetColorFg(tk_con_clr16to64[7]);
	}

	if(tk_iskernel())
	{
//		((u32 *)0xF00BFF00UL)[0]=0x0015;		//320x200x16bpp
//		((u32 *)0xF00BFF00UL)[0]=0x0005;		//
//		((u32 *)0xF00BFF00UL)[0]=0x0001;		//
//		((u32 *)(MMIO_BASE+0x000BFF00UL))[0]=0x0001;		//
		((u32 *)(MMIO_BASE+0x000BFF00UL))[0]=0x0081;		//

		((u32 *)(MMIO_BASE+0x000BFF00UL))[2]=0x20A00000;		//

		tk_con_conbuf2=TKGDI_BlitUpdate_GetConbuf();
		tk_con_conbuf2=TKGDI_BlitUpdate_GetConbuf();
		tk_con_conbuf2=TKGDI_BlitUpdate_GetConbuf();
		tk_con_conbuf2=TKGDI_BlitUpdate_GetConbuf();

		tk_con_clear();
#if 1
		for(i=0; i<256; i++)
		{
			tv=tk_con_hexdig[i&15] | (tk_con_hexdig[(i>>4)&15]<<4);
			tk_gfxcon_hexblock[i]=tv;
		}
#endif

		for(i=0; i<512; i++)
		{
			if((i>=0x0000) && (i<0x0080))
			{
				tv=tk_gfxcon_glyphs[i-0x0000];
			}else if((i>=0x0080) && (i<0x0100))
			{
				tv=tk_gfxcon_glyphs_lat1ext[i-0x0080];
			}else if((i>=0x0100) && (i<0x0180))
			{
				tv=tk_gfxcon_glyphs_gfx0[i-0x0100];
			}else if((i>=0x0180) && (i<0x0200))
			{
				tv=tk_gfxcon_glyphs_437ext[i-0x0180];
			}else
			{
				tv=0;
			}
			
			tk_con_uploadglyph(i, tv);
		}
		
		for(i=0; i<64; i++)
		{
			cr=i&0x30; cg=i&0x0C; cb=i&0x03;
			k=(cr<<9)|(cr<<7)|(cg<<6)|(cg<<4)|(cb<<3)|(cb<<1);
			tk_con_clr64to555[i]=k;
		}
	}
}

void tk_con_disable()
{
	tk_con->ena=0;
}

void tk_con_enable()
{
	tk_con->ena=1;
}

int tk_con_isdisabled()
{
	if(!tk_con)
		return(0);
	if(!tk_iskernel())
		return(0);

	return(tk_con->ena==0);
}

void tk_con_bufferpokeall()
{
	volatile u32 *buf;
	u64 q0, q1, tv;
	int i0, i1;
	int i, j, k;

	if(!tk_con_conbuf2)
		return;
	
	tv=(u64)tk_con_conbuf2;
	if(((tv>>44)&15)==15)
		return;

	buf=(volatile u32 *)tk_con_conbuf2;

	for(i=0; i<25; i++)
	{
		i0=(i*TK_CONWIDTH)*8;
		i0+=16384;
		for(j=0; j<TK_CONWIDTH; j++)
		{
			q0=*(u64 *)(buf+i0+0);
			q1=*(u64 *)(buf+i0+2);
			i0+=8;
		}
	}
}

void tk_con_clear()
{
	volatile u32 *buf;
	u64 q0, q1, tv;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

	i1=tk_con->text_attr;

	buf=(volatile u32 *)(tk_con->buf_addr);

	if(tk_con_conbuf2)
		buf=(volatile u32 *)tk_con_conbuf2;

	q1=0;
	q0=(2ULL<<30)|(tk_con->bgclr_555<<15)|tk_con->fgclr_555;

	for(i=0; i<25; i++)
	{
		i0=(i*TK_CONWIDTH)*8;
		for(j=0; j<TK_CONWIDTH; j++)
		{
	//		i0=((i+0)*TK_CONWIDTH+j)*8;
	//		*(u64 *)(tk_con->buf+i0)=0;
	//		*(u64 *)(tk_con->buf+i0)=i1;
			*(u64 *)(buf+i0+0)=q0;
			*(u64 *)(buf+i0+2)=q1;
			i0+=8;
		}
	}

#if 0
	for(j=0; j<TK_CONWIDTH; j++)
	{
//		tk_con->buf[((24*TK_CONWIDTH)+j)*8]=0;
		buf[((24*TK_CONWIDTH)+j)*8]=i1;
	}
#endif

#if 0
	for(i=0; i<256; i++)
	{
		tv=tk_con_hexdig[i&15] | (tk_con_hexdig[(i>>4)&15]<<4);
		tv=0x55AA55AA55AA55AAULL;
		tk_gfxcon_hexblock[i]=tv;
	}
#endif

	tk_con_bufferpokeall();
}

void TK_Con_UpdateHwCursor()
{
	u32 py;
	py=((tk_con->y)<<8)|(tk_con->x);
//	((u32 *)0xF00BFF00UL)[1]=py;		//move hardware cursor
	((u32 *)(MMIO_BASE+0x000BFF00UL))[1]=py;		//move hardware cursor
}

void TK_Con_SetCursorPos(int x, int y)
{
	tk_con->x=x;
	tk_con->y=y;
	TK_Con_UpdateHwCursor();
}

void TK_Con_SetColorFg(int x)
{
	int cr, cg, cb, cc;
	tk_con->text_attr=(tk_con->text_attr&0xFFC0FFFFU)|((x&0x3F)<<16);
	
	cr=x&0x30; cg=x&0x0C; cb=x&0x03;
	cc=(cr<<9)|(cr<<7)|(cg<<6)|(cg<<4)|(cb<<3)|(cb<<1);
	tk_con->fgclr_555=cc;
}

void TK_Con_SetColorBg(int x)
{
	int cr, cg, cb, cc;
	tk_con->text_attr=(tk_con->text_attr&0xF03FFFFFU)|((x&0x3F)<<22);
	cr=x&0x30; cg=x&0x0C; cb=x&0x03;
	cc=(cr<<9)|(cr<<7)|(cg<<6)|(cg<<4)|(cb<<3)|(cb<<1);
	tk_con->bgclr_555=cc;
}

void tk_con_reset()
{
	tk_con_init();
	tk_con_clear();
	TK_Con_SetCursorPos(0, 0);
	tk_con->text_attr=tk_con->text_attr_dfl;
	TK_Con_SetColorBg(0);
	TK_Con_SetColorFg(tk_con_clr16to64[7]);
	tkgdi_vid_scrmode=0;
}

void tk_con_chkreset()
{
	if(!tk_con || !tk_con->ena)
	{
		tk_con_init();
		tk_con_clear();
//		((u32 *)0xF009F000)[0]=0x0029;
//		((u32 *)0xF009F000)[0]=0x0000;
//		((u32 *)(MMIO_BASE+0x000BFF00UL))[0]=0x0001;		//
		((u32 *)(MMIO_BASE+0x000BFF00UL))[0]=0x0081;		//
		TK_Con_SetCursorPos(0, 0);

		tkgdi_vid_scrmode=0;

		tk_con->text_attr=tk_con->text_attr_dfl;
		TK_Con_SetColorBg(0);
		TK_Con_SetColorFg(tk_con_clr16to64[7]);
	}
}

void tk_con_scroll_up()
{
	volatile u32 *buf;
	volatile u64 *qb0, *qb1;
	u64 q0, q1, q2, q3;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

//	buf=tk_con->buf;
	buf=(volatile u32 *)(tk_con->buf_addr);

	if(tk_con_conbuf2)
		buf=(volatile u32 *)tk_con_conbuf2;

#if 1
	for(i=0; i<TK_CONHEIGHTN1; i++)
	{
		i0=((i+0)*TK_CONWIDTH)*8;
		i1=((i+1)*TK_CONWIDTH)*8;

#if 1
		qb0=(u64 *)(buf+i0);
		qb1=(u64 *)(buf+i1);
		for(j=0; j<TK_CONWIDTH; j+=4)
		{
			q0=qb1[0];		q1=qb1[1];
			q2=qb1[4];		q3=qb1[5];
			qb0[0]=q0;		qb0[1]=q1;
			qb0[4]=q2;		qb0[5]=q3;

			q0=qb1[8];		q1=qb1[ 9];
			q2=qb1[12];		q3=qb1[13];
			qb0[ 8]=q0;		qb0[ 9]=q1;
			qb0[12]=q2;		qb0[13]=q3;

			qb0+=16;		qb1+=16;
		}
#endif

#if 0
		qb0=(u64 *)(buf+i0);
		qb1=(u64 *)(buf+i1);
		for(j=0; j<TK_CONWIDTH; j+=8)
		{
			q0=qb1[0];		q1=qb1[4];
			q2=qb1[8];		q3=qb1[12];
			qb0[0]=q0;		qb0[4]=q1;
			qb0[8]=q2;		qb0[12]=q3;

			q0=qb1[16];		q1=qb1[20];
			q2=qb1[24];		q3=qb1[28];
			qb0[16]=q0;		qb0[20]=q1;
			qb0[24]=q2;		qb0[28]=q3;

			qb0+=32;		qb1+=32;
		}
#endif

#if 0
		for(j=0; j<TK_CONWIDTH; j++)
		{
			q0=*(u64 *)(buf+i1);
			*(u64 *)(buf+i0)=q0;
			i0+=8;
			i1+=8;
		}
#endif
	}

	q1=0;
	q0=(2ULL<<30)|(tk_con->bgclr_555<<15)|tk_con->fgclr_555;

	i0=(TK_CONHEIGHTN1*TK_CONWIDTH)*8;
	i1=tk_con->text_attr;
	for(j=0; j<TK_CONWIDTH; j++)
//		{ *(u64 *)(buf+i0)=0; i0+=8; }
//		{ *(u64 *)(buf+i0)=i1; i0+=8; }
	{
		((u64 *)(buf+i0))[0]=q0;
		((u64 *)(buf+i0))[1]=q1;
		i0+=8;
	}

#endif

#if 0
	for(i=0; i<24; i++)
		for(j=0; j<TK_CONWIDTH; j++)
	{
		i0=((i+0)*TK_CONWIDTH+j)*8;		i1=((i+1)*TK_CONWIDTH+j)*8;

		q0=*(u64 *)(tk_con->buf+i1);
		*(u64 *)(tk_con->buf+i0)=q0;

//		p0=tk_con->buf[i1+0];
//		p1=tk_con->buf[i1+1];
//		p2=tk_con->buf[i1+2];	p3=tk_con->buf[i1+3];
//		tk_con->buf[i0+0]=p0;
//		tk_con->buf[i0+1]=p1;
//		tk_con->buf[i0+2]=p2;	tk_con->buf[i0+3]=p3;
//		p0=tk_con->buf[i1+4];
//		p1=tk_con->buf[i1+5];
//		p2=tk_con->buf[i1+6];	p3=tk_con->buf[i1+7];
//		tk_con->buf[i0+4]=p0;
//		tk_con->buf[i0+5]=p1;
//		tk_con->buf[i0+6]=p2;	tk_con->buf[i0+7]=p3;
	}

	for(j=0; j<TK_CONWIDTH; j++)
	{
		tk_con->buf[((24*TK_CONWIDTH)+j)*8]=0;
	}
#endif

	tk_con_bufferpokeall();
}

void tk_con_scroll_down()
{
	volatile u32 *buf;
	u64 q0, q1;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

//	buf=tk_con->buf;
	buf=(volatile u32 *)(tk_con->buf_addr);

	if(tk_con_conbuf2)
		buf=(volatile u32 *)tk_con_conbuf2;

#if 1
	for(i=TK_CONHEIGHTN1; i>0; i--)
	{
		i0=((i+0)*TK_CONWIDTH)*8;
		i1=((i-1)*TK_CONWIDTH)*8;
		for(j=0; j<TK_CONWIDTH; j++)
		{
			q0=((u64 *)(buf+i1))[0];
			q1=((u64 *)(buf+i1))[1];
			((u64 *)(buf+i0))[0]=q0;
			((u64 *)(buf+i0))[1]=q1;
			i0+=8;
			i1+=8;
		}
	}

	q1=0;
	q0=(2ULL<<30)|(tk_con->bgclr_555<<15)|tk_con->fgclr_555;

	i0=0;
	i1=tk_con->text_attr;
	for(j=0; j<TK_CONWIDTH; j++)
//		{ *(u64 *)(buf+i0)=0; i0+=8; }
	{
//		*(u64 *)(buf+i0)=i1; i0+=8;
		*(u64 *)(buf+i0)=i1;
//		((u64 *)(buf+i0))[0]=q0;
//		((u64 *)(buf+i0))[1]=q1;
		i0+=8;
	}
#endif

	tk_con_bufferpokeall();
}

void tk_con_newline()
{
//	if(!tk_con)
//		tk_con_init();
	
	if(!tk_con)
		return;
	
//	tk_con->x=0;
	tk_con->y++;
	if(tk_con->y>=25)
	{
		tk_con_scroll_up();
		tk_con->y--;
	}
}

static int tk_con_isesc=0;
static int tk_con_escval0;
static int tk_con_escval1;
static int tk_con_escval2;
static int tk_con_escval3;

void TK_Con_PutcEscapeSgr(int mode)
{
	switch(mode)
	{
	case 0:
		tk_con->text_attr=tk_con->text_attr_dfl;
		break;
	case 1:
		tk_con->text_attr|=0x003F0000;
		break;
	case 2:
		break;
	case 3:
	case 7:
		tk_con->text_attr|=0x00002000;
		break;
	case 4:
		tk_con->text_attr|=0x00000800;
		break;
	case 5:
		tk_con->text_attr&=~0x0000C000;
		tk_con->text_attr|= 0x00008000;
		break;
	case 6:
		tk_con->text_attr&=~0x0000C000;
		tk_con->text_attr|= 0x00004000;
		break;
	case 9:
		tk_con->text_attr|=0x00000400;
		break;
	case 10:
		tk_con->text_attr&=~0x00000300;
		break;
	
	case 22:
		tk_con->text_attr=
			(tk_con->text_attr&(~0x003F0000))|
			(tk_con->text_attr_dfl&0x003F0000);
		break;
	case 23:
		tk_con->text_attr&=~0x00002000;
		break;
	case 24:
		tk_con->text_attr&=~0x00000800;
		break;
	case 25:
		tk_con->text_attr&=~0x0000C000;
		break;
	case 29:
		tk_con->text_attr&=~0x00000400;
		break;
	
	case 30:	case 31:
	case 32:	case 33:
	case 34:	case 35:
	case 36:	case 37:
		TK_Con_SetColorFg(tk_con_clr16to64[mode-30]);
		break;
	case 39:
		TK_Con_SetColorFg(0x2A);
		break;		
	case 40:	case 41:
	case 42:	case 43:
	case 44:	case 45:
	case 46:	case 47:
		TK_Con_SetColorBg(tk_con_clr16to64[mode-40]);
		break;
	case 49:
		TK_Con_SetColorBg(0);
		break;		

	case 90:	case 91:
	case 92:	case 93:
	case 94:	case 95:
	case 96:	case 97:
		TK_Con_SetColorFg(tk_con_clr16to64[8+(mode-90)]);
		break;
	case 100:	case 101:
	case 102:	case 103:
	case 104:	case 105:
	case 106:	case 107:
		TK_Con_SetColorBg(tk_con_clr16to64[8+(mode-100)]);
		break;
	}
}

void TK_Con_PutcEscape(int ch)
{
	int i, j, k;

	if(tk_con_isesc==1)
	{
		if(ch=='[')
		{
			tk_con_escval3=0;
			tk_con_escval2=0;
			tk_con_escval1=0;
			tk_con_escval0=0;
			tk_con_isesc=2;
			return;
		}
		
		tk_con_isesc=0;
		return;
	}

	if(tk_con_isesc==2)
	{
		switch(ch)
		{
		case '0':	case '1':
		case '2':	case '3':
		case '4':	case '5':
		case '6':	case '7':
		case '8':	case '9':
			tk_con_escval0=(tk_con_escval0*10)+(ch-'0');
			break;
		case ';':
			tk_con_escval3=tk_con_escval2;
			tk_con_escval2=tk_con_escval1;
			tk_con_escval1=tk_con_escval0;
			tk_con_escval0=0;
			break;
		case 'A':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(tk_con->x, tk_con->y-i);
			tk_con_isesc=0;
			break;
		case 'B':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(tk_con->x, tk_con->y+i);
			tk_con_isesc=0;
			break;
		case 'C':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(tk_con->x+i, tk_con->y);
			tk_con_isesc=0;
			break;
		case 'D':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(tk_con->x-i, tk_con->y);
			tk_con_isesc=0;
			break;

		case 'E':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(0, tk_con->y+i);
			tk_con_isesc=0;
			break;
		case 'F':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(0, tk_con->y-i);
			tk_con_isesc=0;
			break;
		case 'G':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(i-1, tk_con->y);
			tk_con_isesc=0;
			break;
		case 'H':
		case 'f':
			i=tk_con_escval0;
			j=tk_con_escval1;
			if(!i)i=1;
//			if(!j)j=1;
			if(!j) { j=i; i=1; }
			TK_Con_SetCursorPos(i-1, j-1);
			tk_con_isesc=0;
			break;

		case 'J':
			switch(tk_con_escval0)
			{
			case 2:
			case 3:
				tk_con_clear();
				break;
			}
			tk_con_isesc=0;
			break;

		case 'S':
			i=tk_con_escval0;
			if(!i)i=1;
			while(i--)
				{ tk_con_scroll_up(); }
			tk_con_isesc=0;
			break;
		case 'T':
			i=tk_con_escval0;
			if(!i)i=1;
			while(i--)
				{ tk_con_scroll_down(); }
			tk_con_isesc=0;
			break;

		case 'm':
			TK_Con_PutcEscapeSgr(tk_con_escval0);
			tk_con_isesc=0;
			break;

		default:
			tk_con_isesc=0;
			break;
		}
		return;
	}

	tk_con_isesc=0;
}

u16 tk_con_chpremap_key[256];
u16 tk_con_chpremap_val[256];

u16 tk_con_remaptab[]={
0x20AC, 0x0080,		0x201A, 0x0082,
0x0192, 0x0083,		0x201E, 0x0084,
0x2026, 0x0085,		0x2020, 0x0086,
0x2021, 0x0087,		0x02C6, 0x0088,
0x2030, 0x0089,		0x0160, 0x008A,
0x2039, 0x008B,		0x0152, 0x008C,
0x017D, 0x008E,

0x2018, 0x0091,		0x2019, 0x0092,
0x201C, 0x0093,		0x201D, 0x0094,
0x2022, 0x0095,		0x2013, 0x0096,
0x2014, 0x0097,		0x02DC, 0x0098,
0x2122, 0x0099,		0x0161, 0x009A,
0x203A, 0x009B,		0x0153, 0x009C,
0x017E, 0x009E,		0x0178, 0x009F,

0x20A7, 0x019E,		0x2310, 0x01A9,

0x2591, 0x01B0,		0x2592, 0x01B1,		0x2593, 0x01B2,		0x2502, 0x01B3,
0x2524, 0x01B4,		0x2561, 0x01B5,		0x2562, 0x01B6,		0x2556, 0x01B7,
0x2555, 0x01B8,		0x2563, 0x01B9,		0x2551, 0x01BA,		0x2557, 0x01BB,
0x255D, 0x01BC,		0x255C, 0x01BD,		0x255B, 0x01BE,		0x2510, 0x01BF,
0x2514, 0x01C0,		0x2534, 0x01C1,		0x252C, 0x01C2,		0x251C, 0x01C3,
0x2500, 0x01C4,		0x253C, 0x01C5,		0x255E, 0x01C6,		0x255F, 0x01C7,
0x255A, 0x01C8,		0x2554, 0x01C9,		0x2569, 0x01CA,		0x2566, 0x01CB,
0x2560, 0x01CC,		0x2550, 0x01CD,		0x256C, 0x01CE,		0x2567, 0x01CF,
0x2568, 0x01D0,		0x2564, 0x01D1,		0x2565, 0x01D2,		0x2559, 0x01D3,
0x2558, 0x01D4,		0x2552, 0x01D5,		0x2553, 0x01D6,		0x256B, 0x01D7,
0x256A, 0x01D8,		0x2518, 0x01D9,		0x25DC, 0x01DA,		0x2588, 0x01DB,
0x2585, 0x01DC,		0x258C, 0x01DD,		0x259D, 0x01DE,		0x2580, 0x01DF,
0x03B1, 0x01E0,		0x00DF, 0x01E1,		0x0393, 0x01E2,		0x03C0, 0x01E3,
0x03A3, 0x01E4,		0x03C3, 0x01E5,		0x00B5, 0x01E6,		0x03C4, 0x01E7,
0x03A6, 0x01E8,		0x0398, 0x01E9,		0x03A9, 0x01EA,		0x03B4, 0x01EB,
0x221E, 0x01EC,		0x03C6, 0x01ED,		0x03B5, 0x01EE,		0x2229, 0x01EF,
0x2261, 0x01F0,		0x00B1, 0x01F1,		0x2265, 0x01F2,		0x2264, 0x01F3,
0x2320, 0x01F4,		0x2321, 0x01F5,		0x00F7, 0x01F6,		0x2248, 0x01F7,
0x00B0, 0x01F8,		0x2219, 0x01F9,		0x00B7, 0x01FA,		0x221A, 0x01FB,
0x207F, 0x01FC,		0x00B2, 0x01FD,		0x25A0, 0x01FE,		0x00A0, 0x01FF,

0x236A, 0x0001,
0x236B, 0x0002,		0x2665, 0x0003,
0x2666, 0x0004,		0x2663, 0x0005,		0x2660, 0x0006,		0x2022, 0x0007,
0x25D8, 0x0008,		0x25CB, 0x0009,		0x2509, 0x000A,		0x2642, 0x000B,
0x2640, 0x000C,		0x266A, 0x000D,		0x266B, 0x000E,		0x263C, 0x000F,

0x25BA, 0x0010,		0x25C4, 0x0011,		0x2195, 0x0012,		0x203C, 0x0013,
0x00B6, 0x0014,		0x00A7, 0x0015,		0x25AC, 0x0016,		0x21A8, 0x0017,
0x2191, 0x0018,		0x2193, 0x0019,		0x2192, 0x001A,		0x2190, 0x001B,
0x221F, 0x001C,		0x2194, 0x001D,		0x25B2, 0x001E,		0x25BC, 0x001F,

0x044E, 0x0140,		0x0430, 0x0141,		0x0431, 0x0142,		0x0446, 0x0143,
0x0434, 0x0144,		0x0435, 0x0145,		0x0444, 0x0146,		0x0433, 0x0147,
0x0445, 0x0148,		0x0438, 0x0149,		0x0439, 0x014A,		0x043A, 0x014B,
0x043B, 0x014C,		0x043C, 0x014D,		0x043D, 0x014E,		0x043E, 0x014F,

0x043F, 0x0150,		0x044F, 0x0151,		0x0440, 0x0152,		0x0441, 0x0153,
0x0442, 0x0154,		0x0443, 0x0155,		0x0436, 0x0156,		0x0432, 0x0157,
0x044C, 0x0158,		0x044B, 0x0159,		0x0437, 0x015A,		0x0448, 0x015B,
0x044D, 0x015C,		0x0449, 0x015D,		0x0447, 0x015E,		0x044A, 0x015F,

0x042E, 0x0160,		0x0410, 0x0161,		0x0411, 0x0162,		0x0426, 0x0163,
0x0414, 0x0164,		0x0415, 0x0165,		0x0424, 0x0166,		0x0413, 0x0167,
0x0425, 0x0168,		0x0418, 0x0169,		0x0419, 0x016A,		0x041A, 0x016B,
0x041B, 0x016C,		0x041C, 0x016D,		0x041D, 0x016E,		0x041E, 0x016F,

0x041F, 0x0170,		0x042F, 0x0171,		0x0420, 0x0172,		0x0421, 0x0173,
0x0422, 0x0174,		0x0423, 0x0175,		0x0416, 0x0176,		0x0412, 0x0177,
0x042C, 0x0178,		0x042B, 0x0179,		0x0417, 0x017A,		0x0428, 0x017B,
0x042D, 0x017C,		0x0429, 0x017D,		0x0427, 0x017E,		0x042A, 0x017F,

0x0451, 0x0123,		0x0401, 0x0133,
0x0454, 0x0124,		0x0404, 0x0134,
0x0456, 0x0069,		0x0406, 0x0049,
0x0457, 0x0127,		0x0407, 0x0137,
0x0491, 0x012D,		0x0490, 0x013D,

0x0390, 0x0127,		0x0391, 0x0041,		0x0392, 0x0042,		0x0393, 0x0167,
0x0394, 0x0104,		0x0395, 0x0045,		0x0396, 0x005A,		0x0397, 0x0048,
0x0398, 0x0108,		0x0399, 0x0049,		0x039A, 0x004A,		0x039B, 0x010B,
0x039C, 0x016C,		0x039D, 0x004E,		0x039E, 0x010F,		0x039F, 0x016F,
0x03A0, 0x0110,		0x03A1, 0x0172,		0x03A2, 0x0040,		0x03A3, 0x0113,
0x03A4, 0x0174,		0x03A5, 0x0059,		0x03A6, 0x0116,		0x03A7, 0x0168,
0x03A8, 0x0118,		0x03A9, 0x0119,		0x03AA, 0x011A,		0x03AB, 0x011B,
0x03AC, 0x011C,		0x03AD, 0x011D,		0x03AE, 0x011E,		0x03AF, 0x0069,
0x03B0, 0x0120,		0x03B1, 0x0121,		0x03B2, 0x0122,		0x03B3, 0x0112,
0x03B4, 0x0102,		0x03B5, 0x0124,		0x03B6, 0x0126,		0x03B7, 0x0125,
0x03B8, 0x0128,		0x03B9, 0x0129,		0x03BA, 0x014B,		0x03BB, 0x012B,
0x03BC, 0x012C,		0x03BD, 0x0076,		0x03BE, 0x012E,		0x03BF, 0x014F,
0x03C0, 0x0150,		0x03C1, 0x0152,		0x03C2, 0x0132,		0x03C3, 0x0131,
0x03C4, 0x0154,		0x03C5, 0x0135,		0x03C6, 0x0136,		0x03C7, 0x0148,
0x03C8, 0x0138,		0x03C9, 0x0139,		0x03CA, 0x0127,		0x03CB, 0x013B,
0x03CC, 0x013C,		0x03CD, 0x013F,		0x03CE, 0x013E,		0x03CF, 0x0139,

0, 0
};

int tk_con_doremap(int cp)
{
	int ix, cp1;
	int i;
	
	ix=((cp*251)>>8)&0xFF;
	if(tk_con_chpremap_key[ix]==cp)
		return(tk_con_chpremap_val[ix]);
	
	cp1=cp;
	
	for(i=0; tk_con_remaptab[i]; i+=2)
	{
		if(tk_con_remaptab[i]==cp)
		{
			cp1=tk_con_remaptab[i+1];
			break;
		}
	}
	
	tk_con_chpremap_key[ix]=cp;
	tk_con_chpremap_val[ix]=cp1;
	return(cp1);
}

void tk_con_putc(int ch)
{
	volatile u32 *buf;
	int tx, ty, tz;
	u64 q0, q1;
	u32 px, py;
	
	if(!tk_con)
		tk_con_init();
	
	if(!tk_con->ena)
		return;

	buf=(volatile u32 *)(tk_con->buf_addr);

	if(tk_con_isesc)
	{
		TK_Con_PutcEscape(ch);
		return;
	}

	if(ch<' ')
	{
		if(ch=='\b')
		{
			if(tk_con->x>0)tk_con->x--;

#if 0
			px=0x003F8000|'_';
			py=0x003F0000|' ';
			
			ty=tk_con->y;
			tx=tk_con->x;
			tz=(ty*TK_CONWIDTH+tx)*8;
			tk_con->buf[tz+0]=px;
			tk_con->buf[tz+8]=py;
#endif

//			py=((tk_con->y)<<8)|(tk_con->x);
//			((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
			TK_Con_UpdateHwCursor();

			return;
		}

		if(ch=='\r')
		{
			tk_con->x=0;
//			py=((tk_con->y)<<8)|(tk_con->x);
//			((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
			TK_Con_UpdateHwCursor();
			return;
		}
		if(ch=='\n')
		{
			tk_con_newline();

//			py=((tk_con->y)<<8)|(tk_con->x);
//			((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
			TK_Con_UpdateHwCursor();
			return;
		}
		if(ch=='\t')
		{
			tk_con->x=(tk_con->x+8)&(~7);
			if(tk_con->x>=TK_CONWIDTH)
				{ tk_con_newline(); tk_con->x=0; }

//			py=((tk_con->y)<<8)|(tk_con->x);
//			((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
			TK_Con_UpdateHwCursor();
			return;
		}

		if(ch=='\x1B')
		{
			tk_con_isesc=1;
			return;
		}

		if(ch=='\x1f')
		{
#if 1
			px=0x003F8000|'_';
			ty=tk_con->y;
			tx=tk_con->x;
			tz=(ty*TK_CONWIDTH+tx)*8;
			buf[tz+0]=px;
			tx++;
			tk_con->x=tx;
#endif
			return;
		}

		return;
	}
	
//	q1=0x55AA55AAULL;

//	px=0x0FC00000|ch;
//	px=0x003F0000|ch;
	px=(tk_con->text_attr)|ch;
//	px=ch|((tk_con->fgclr)<<16)|((tk_con->bgclr)<<22);
//	py=0x003F8000|'_';
	
	ty=tk_con->y;
	tx=tk_con->x;
//	tz=(ty*TK_CONWIDTH+tx)*8;
//	tk_con->buf[tz]=px;

	if(ch<0x100)
	{
//		tz=(ty*TK_CONWIDTH+tx)*8;
//		tk_con->buf[tz]=px;

		tz=(ty*TK_CONWIDTH+tx)*4;
		((u64 *)buf)[tz]=px;
	}else
	{
		if(ch>=0x100)
		{
			ch=tk_con_doremap(ch);
		}

		if((ch>=0x00) && (ch<=0x7F))
		{
			q1=tk_gfxcon_glyphs[ch];
		}else
			if((ch>=0x80) && (ch<=0xFF))
		{
			q1=tk_gfxcon_glyphs_lat1ext[ch&0x7F];
		}else
			if((ch>=0x100) && (ch<=0x17F))
		{
			q1=tk_gfxcon_glyphs_cyril0[ch&0x7F];
		}else
			if((ch>=0x180) && (ch<=0x1FF))
		{
			q1=tk_gfxcon_glyphs_437ext[ch&0x7F];
		}else
	//		if((ch>=0x1F00) && (ch<=0x1FFF))
			if((ch>=0x0600) && (ch<=0x06FF))
		{
			q1=tk_gfxcon_hexblock[ch&0xFF];
	//		q1=tk_gfxcon_glyphs[0xBF];
		}else
		{
			q1=tk_gfxcon_glyphs[0xBF];
		}

		q0=(2ULL<<30)|(tk_con->bgclr_555<<15)|tk_con->fgclr_555;

		tz=(ty*TK_CONWIDTH+tx)*4;
		((u64 *)buf)[tz+0]=q0;
		((u64 *)buf)[tz+1]=q1;
	}

//	tk_con->buf[tz+8]=py;

//	__debugbreak();
	
	tx++;
	tk_con->x=tx;
//	if(tk_con->x>=TK_CONWIDTH)
	if(tx>=TK_CONWIDTH)
	{
		tk_con_newline();
		tk_con->x=0;
	}

//	py=((tk_con->y)<<8)|(tk_con->x);
//	((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
	TK_Con_UpdateHwCursor();
}

u64 TK_Con_GlyphSwapByte(byte bits)
{
	static byte swaptab[16]={
		0x0, 0x8, 0x4, 0xC,
		0x2, 0xA, 0x6, 0xE,
		0x1, 0x9, 0x5, 0xD,
		0x3, 0xB, 0x7, 0xF};
	int v;
	v=(swaptab[bits&15]<<4)|swaptab[bits>>4];
	return(v);
}

u64 TK_Con_GlyphFromCelBits(byte *celbits)
{
	u64 v;
	v=	(TK_Con_GlyphSwapByte(celbits[7])<<56) |
		(TK_Con_GlyphSwapByte(celbits[6])<<48) |
		(TK_Con_GlyphSwapByte(celbits[5])<<40) |
		(TK_Con_GlyphSwapByte(celbits[4])<<32) |
		(TK_Con_GlyphSwapByte(celbits[3])<<24) |
		(TK_Con_GlyphSwapByte(celbits[2])<<16) |
		(TK_Con_GlyphSwapByte(celbits[1])<< 8) |
		(TK_Con_GlyphSwapByte(celbits[0])<< 0) ;
	return(v);
}

void TKGDI_FetchUnifontSdfBits(
	byte *dstbits, int dxs, int dys,
	int fontid, int codepoint);

u64 TK_Con_GlyphForCodepoint(int ch)
{
	static u64 pxab[64];
	static u16 pxac[64];
	byte celbits[64];
	int ch0, h;
	u64 q1;

	ch0=ch;
	if(ch>=0x100)
	{
		ch=tk_con_doremap(ch);
	}

#if 1
	if((ch>=0x00) && (ch<=0x7F))
	{
		q1=tk_gfxcon_glyphs[ch];
	}else
		if((ch>=0x80) && (ch<=0xFF))
	{
		q1=tk_gfxcon_glyphs_lat1ext[ch&0x7F];
	}else
		if((ch>=0x100) && (ch<=0x17F))
	{
		q1=tk_gfxcon_glyphs_cyril0[ch&0x7F];
	}else
		if((ch>=0x180) && (ch<=0x1FF))
	{
		q1=tk_gfxcon_glyphs_437ext[ch&0x7F];
	}else
//		if((ch>=0x1F00) && (ch<=0x1FFF))
		if((ch>=0x0600) && (ch<=0x06FF))
	{
		q1=tk_gfxcon_hexblock[ch&0xFF];
//		q1=tk_gfxcon_glyphs[0xBF];
	}else
#else
	if(1)
#endif
	{
//		q1=tk_gfxcon_glyphs[0xBF];

		h=(((ch0^0x0055)*251)>>8)&63;
		if(pxac[h]==ch0)
			{ return(pxab[h]); }

		TKGDI_FetchUnifontSdfBits(celbits, 8, 8, 1, ch0);
		q1=TK_Con_GlyphFromCelBits(celbits);

//		tk_dbg_printf("TK_Con_GlyphForCodepoint: ch=%04X pxb=%X\n", ch0, q1);

		pxab[h]=q1;
		pxac[h]=ch0;
	}
	
	return(q1);
}
