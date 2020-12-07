#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

#include "bt1h_targa.c"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

u64 gfxfont[256]={
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
// 0x0000000000000000ULL,		//0C, Dummy
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
// 0x0000000000000000ULL,		//1C, Dummy
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
0x0F0F0F0F0F0F0F0FULL,		//80
0x0303030303030303ULL,		//81
0x3F3F3F3F3F3F3F3FULL,		//82
0x03030F0F0F0F3F3FULL,		//83
0x0000030303030F0FULL,		//84
0x0F0F3F3F3F3FFFFFULL,		//85
0x03030F0F3F3FFFFFULL,		//86
0x000003030F0F3F3FULL,		//87
0x0000000003030F0FULL,		//88
0x0F0F3F3FFFFFFFFFULL,		//89
0x000003033F3FFFFFULL,		//8A
0x0000000003033F3FULL,		//8B
0x03033F3FFFFFFFFFULL,		//8C
0x00000000FFFFFFFFULL,		//8D
0x0000FFFFFFFFFFFFULL,		//8E
0x000000000000FFFFULL,		//8F
0x0000C0C0FCFCFFFFULL,		//90
0x3F3F030300000000ULL,		//91
0x00000000C0C0FCFCULL,		//92
0x3F3F0F0F03030000ULL,		//93
0x0F0F030300000000ULL,		//94
0x0000C0C0F0F0FCFCULL,		//95
0x00000000C0C0F0F0ULL,		//96
0x3F3F0F0F0F0F0303ULL,		//97
0x0F0F030303030000ULL,		//98
0x0000C0C0C0C0F0F0ULL,		//99
0x3C3C3C3C3C3C3C3CULL,		//9A
0x0F0F3C3C3C3CF0F0ULL,		//9B
0x03033F3FFCFCC0C0ULL,		//9C
0x0000FFFFFFFF0000ULL,		//9D
0x3F3F0303C0C0FCFCULL,		//9E
0x0F0FC3C3C3C3F0F0ULL,		//9F
0x3333333333333333ULL,		//A0
0x0000FFFF0000FFFFULL,		//A1
0x3333CCCC3333CCCCULL,		//A2
0x0F0F0F0FF0F0F0F0ULL,		//A3
0x0F0FF0F00F0FF0F0ULL,		//A4
0x33333333CCCCCCCCULL,		//A5
0x3C3CC3C33C3CC3C3ULL,		//A6
0x3333CCCCCCCC3333ULL,		//A7
0x3F3F0F0FF0F0FCFCULL,		//A8
0x03030F0FF0F0C0C0ULL,		//A9
0x0F0F0C0C3030F0F0ULL,		//AA
0x0F0FCFCFF3F3F0F0ULL,		//AB
0x3C3CC3C3C3C33C3CULL,		//AC
0x0F0FF0F0F0F00F0FULL,		//AD
0x3C3C3C3CC3C3C3C3ULL,		//AE
0x00003C3C3C3C0000ULL,		//AF
0x3030FCFC30300000ULL,		//B0
0x0C0C3F3F0C0C0000ULL,		//B1
0x00000C0C3F3F0C0CULL,		//B2
0x00003030FCFC3030ULL,		//B3
0x3C3CF0F0C3C30F0FULL,		//B4
0x0F0F3C3CF0F0C3C3ULL,		//B5
0x3C3C0F0FC3C3F0F0ULL,		//B6
0x0F0FC3C3F0F03C3CULL,		//B7
0x3C3CF0F0F0F0C0C0ULL,		//B8
0x3C3C0F0F0F0F0303ULL,		//B9
0x3F3FFCFCC0C00303ULL,		//BA
0x0303C0C0FCFC3F3FULL,		//BB
0x0000FFFF0F0F0F0FULL,		//BC
0x0F0F0F0FFFFF0000ULL,		//BD
0x0C0C0C0CFCFCFCFCULL,		//BE
0x303030303F3F3F3FULL		//BF
};

u64 btesh2_gfxcon_glyphs[256]=
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
0x0008183C487E0800,	//0x34 "4"
0x007E407C04443C00,	//0x35 "5"
0x003C407C44443800,	//0x36 "6"
0x007E040810202000,	//0x37 "7"
0x003C443C44443800,	//0x38 "8"
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


int hexci(char c)
{
	if((c>='0') && (c<='9'))
		return(c-'0');
	if((c>='A') && (c<='F'))
		return(10+(c-'A'));
	if((c>='a') && (c<='f'))
		return(10+(c-'a'));
	return(-1);
}

int hex2i(char *s0)
{
	return(hexci(s0[0])*16+hexci(s0[1]));
}

int hex4i(char *s0)
{
	return(hex2i(s0+0)*256+hex2i(s0+2));
}

int clamp255(int clr)
{
	if(clr<0)return(0);
	if(clr>255)return(255);
	return(clr);
}

u32 vqdec_fromyuv(int cy, int cu, int cv)
{
	int cr, cg, cb;
	int cc;

//	cg=cy;
//	cg=(4*cy-(cu+cv-256))/2;
	cg=cy-(cu+cv-256)/2;
	cr=cg+(cv-128)*2;
	cb=cg+(cu-128)*2;
	cr=clamp255(cr);
	cg=clamp255(cg);
	cb=clamp255(cb);
	cc=0xFF000000|(cb<<16)|(cg<<8)|cr;
	return(cc);
}

u32 vqdec_clr6(int clr, int ctsp)
{
	int mod4[4]={0, 85, 170, 255};
	int mod4uv[4]={85, 128, 170, 255};
	int mod7u[8]={255, 170, 85, 128,  128, 170,  85, 128};
	int mod7v[8]={255, 128, 85,  85,  170, 170, 128, 128};
	int cr, cg, cb;
	int cc, cy, cu, cv;

	switch(ctsp)
	{
	case 0:
		cr=mod4[(clr>>4)&3];
		cg=mod4[(clr>>2)&3];
		cb=mod4[(clr>>0)&3];
		cc=0xFF000000|(cb<<16)|(cg<<8)|cr;
		break;
	case 1:
		cy=(clr&63)>>3;
		cy=(cy<<5)|(cy<<2)|(cy>>2);
		cu=mod7u[clr&7];
		cv=mod7v[clr&7];

//		cy=mod4[(clr>>4)&3];
//		cu=mod4[(clr>>2)&3];
//		cv=mod4[(clr>>0)&3];
	
		cc=vqdec_fromyuv(cy, cu, cv);
		break;
	default:
		cc=0;
		break;
	}
	return(cc);
}

u32 vqdec_clr9(int clr, int ctsp)
{
	int mod8[8]={0, 36, 73, 109, 145, 182, 219, 255};
	int cr, cg, cb;
	int cc, cy, cu, cv;

	switch(ctsp)
	{
	case 0:
	case 2:
		cr=mod8[(clr>>6)&7];
		cg=mod8[(clr>>3)&7];
		cb=mod8[(clr>>0)&7];
		cc=0xFF000000|(cb<<16)|(cg<<8)|cr;
		break;
	}
	return(cc);
}

int vqenc_rgb2p512(int cr, int cg, int cb, int ctsp)
{
	u32 p0;
	int pr, pg, pb;
	int dr, dg, db, d, bd, bi;
	int i, j, k;

	bd=999999999; bi=0;
	for(i=0; i<512; i++)
	{
		p0=vqdec_clr9(i, ctsp);
		pr=(p0    )&255;
		pg=(p0>> 8)&255;
		pb=(p0>>16)&255;
		
		dr=cr-pr;
		dg=cg-pg;
		db=cb-pb;
//		d=dr*dr+dg*dg+db*db;
		d=dr*dr+(2*dg*dg)+(db*db/2);
		
		if(d<bd)
			{ bi=i; bd=d; }
	}
	return(bi);
}

int vqenc_rgb2p64(int cr, int cg, int cb, int ctsp)
{
	u32 p0;
	int pr, pg, pb;
	int dr, dg, db, d, bd, bi;
	int i, j, k;

	bd=999999999; bi=0;
	for(i=0; i<64; i++)
	{
		p0=vqdec_clr6(i, ctsp);
		pr=(p0    )&255;
		pg=(p0>> 8)&255;
		pb=(p0>>16)&255;
		
		dr=cr-pr;
		dg=cg-pg;
		db=cb-pb;
//		d=dr*dr+dg*dg+db*db;
		d=dr*dr+(2*dg*dg)+(db*db/2);
		
		if(d<bd)
			{ bi=i; bd=d; }
	}
	return(bi);
}


int vqdec(byte *iblk, int ystr, u32 blk)
{
	u32 *ct0, *ct1, *ct2, *ct3;
	u32 clra, clrb;
//	u64 sta, stb;
	int dcy, ddy, dcu, dcv;		//deltas
	int ecy, edy, ecu, ecv;		//exps
	int pcy, pdy, pcu, pcv;		//preds
	int mcy, ncy;
	int cta, ctb, ctsp;
//	int cy, cu, cv;
//	int mcy, ncy, acy, dcy, cu, cv;
	int i, j, k;

//	if(rst)
//		sta=*rst;
//	else
//		sta=0;
//	stb=sta;

	switch((blk>>30)&3)
	{
	case 1:
		ctsp=(blk>>28)&3;
		cta=(blk>>22)&63;
		ctb=(blk>>16)&63;
		clra=vqdec_clr6(cta, ctsp);
		clrb=vqdec_clr6(ctb, ctsp);
		break;

	case 3:
//		pcy=(sta    )&255;	pdy=(sta>> 8)&255;
//		pcu=(sta>>16)&255;	pcv=(sta>>24)&255;
//		ecy=(sta>>32)&15;	edy=(sta>>36)&15;
//		ecu=(sta>>40)&15;	ecv=(sta>>44)&15;

		dcy=(blk>>26)&15;	ddy=(blk>>22)&15;
		dcu=(blk>>19)&7;	dcv=(blk>>16)&7;
		
		pcy=(dcy<<4)|dcy;
		pdy=(ddy<<4)|ddy;
		pcu=dcu<<5;
		pcv=dcv<<5;

		k=5;
		ecy=k;	edy=k;	ecu=k;	ecv=k;

		mcy=pcy-(pdy>>1);
		ncy=mcy+pdy;
		clra=vqdec_fromyuv(mcy, pcu, pcv);
		clrb=vqdec_fromyuv(ncy, pcu, pcv);

		i=pcy|(pdy<<8)|(pcu<<16)|(pcv<<24);
		j=(ecy<<0)|(edy<<4)|(ecu<<8)|(ecv<<12);
//		stb=((u32)i)|(((u64)j)<<32);

		break;

	}

//	if(rst)
//		*rst=stb;

//	ct0=(u32 *)(iblk+0*ystr);
//	ct1=(u32 *)(iblk+1*ystr);
//	ct2=(u32 *)(iblk+2*ystr);
//	ct3=(u32 *)(iblk+3*ystr);

	ct3=(u32 *)(iblk+0*ystr);
	ct2=(u32 *)(iblk+1*ystr);
	ct1=(u32 *)(iblk+2*ystr);
	ct0=(u32 *)(iblk+3*ystr);

	ct0[0]=(blk&0x0008)?clrb:clra;
	ct0[1]=(blk&0x0004)?clrb:clra;
	ct0[2]=(blk&0x0002)?clrb:clra;
	ct0[3]=(blk&0x0001)?clrb:clra;
	ct1[0]=(blk&0x0080)?clrb:clra;
	ct1[1]=(blk&0x0040)?clrb:clra;
	ct1[2]=(blk&0x0020)?clrb:clra;
	ct1[3]=(blk&0x0010)?clrb:clra;
	ct2[0]=(blk&0x0800)?clrb:clra;
	ct2[1]=(blk&0x0400)?clrb:clra;
	ct2[2]=(blk&0x0200)?clrb:clra;
	ct2[3]=(blk&0x0100)?clrb:clra;
	ct3[0]=(blk&0x8000)?clrb:clra;
	ct3[1]=(blk&0x4000)?clrb:clra;
	ct3[2]=(blk&0x2000)?clrb:clra;
	ct3[3]=(blk&0x1000)?clrb:clra;
}

int vqenc(byte *iblk, int ystr, u32 *oblk)
{
	byte pxa[16*4];
	byte pxb[16*4];
	byte pxc[16*4];
	short pxy[16];
	
	u64 sta, stb, stc, std;
	int min, max;
	u32 blk, blka, blkb, blkc;

	int acr, acg, acb, acy;

	int cr, cg, cb, cy;
	int mr, mg, mb, my, mdy;
	int nr, ng, nb, ny, ndy;
	int cu, cv;

	int mr2, mg2, mb2, my2;
	int nr2, ng2, nb2, ny2;

	int cy_c, cy_m, cy_y;
	int my_c, my_m, my_y;
	int ny_c, ny_m, ny_y;
	int ax, dy;
	int e0, e1;

	int cya, cyb, ctsp;
	int acu, acv, ady;
	int ia, ib, ix, px;
	int ixa, ixb, ixc;
	int i, j, k, l;
	
	my=256; ny=-1;
	my_c=256; ny_c=-1;
	my_m=256; ny_m=-1;
	my_y=256; ny_y=-1;
	acr=0; acg=0; acb=0; acy=0;
	acu=0; acv=0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];
		cy=(cr+2*cg+cb)/4;

		k=i*4+j;
//		k=(3-i)*4+j;
		pxc[k*4+0]=cr;
		pxc[k*4+1]=cg;
		pxc[k*4+2]=cb;
		pxc[k*4+3]=255;

		cy_c=(1*cr+4*cg+3*cb)/8;
		cy_m=(4*cr+1*cg+3*cb)/8;
		cy_y=(3*cr+4*cg+1*cb)/8;

		cu=((cb-cg)/2)+128;
		cv=((cr-cg)/2)+128;

		if(cy<my) { my=cy; }
		if(cy>ny) { ny=cy; }
		if(cy_c<my_c) { my_c=cy_c; }
		if(cy_c>ny_c) { ny_c=cy_c; }
		if(cy_m<my_m) { my_m=cy_m; }
		if(cy_m>ny_m) { ny_m=cy_m; }
		if(cy_y<my_y) { my_y=cy_y; }
		if(cy_y>ny_y) { ny_y=cy_y; }
		
		acr+=cr;	acg+=cg;
		acb+=cb;	acy+=cy;
		acu+=cu;	acv+=cv;
	}
	
	acr=acr/16;	acg=acg/16;
	acb=acb/16;	acy=acy/16;
	acu=acu/16;	acv=acv/16;

	acy=(2*acy+my+ny)/4;

	ax=0; dy=ny-my;

#if 1
	dy=2*(ny-acy);
	
//	i=ny_c-my_c; if(i>dy) { ax=1; dy=i; }
//	i=ny_m-my_m; if(i>dy) { ax=2; dy=i; }
//	i=ny_y-my_y; if(i>dy) { ax=3; dy=i; }

	mr=0; mg=0; mb=0; my=0; cya=0;
	nr=0; ng=0; nb=0; ny=0; cyb=0;
	mdy=0; ndy=0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];
//		cy=(cr+2*cg+cb)/4;

		switch(ax)
		{
		case 0: cy=(cr+2*cg+cb)/4; break;
		case 1: cy=(1*cr+4*cg+3*cb)/8; break;
		case 2: cy=(4*cr+1*cg+3*cb)/8; break;
		case 3: cy=(3*cr+4*cg+1*cb)/8; break;
		}

		pxy[i*4+j]=cy;
		
		if(cy<acy)
			{ mr+=cr; mg+=cg; mb+=cb; my+=cy; cya++; mdy+=acy-cy; }
		else
			{ nr+=cr; ng+=cg; nb+=cb; ny+=cy; cyb++; ndy+=cy-acy; }
	}
	
	if(!cya)cya++;
	if(!cyb)cyb++;

	mr=mr/cya; mg=mg/cya;
	mb=mb/cya; my=my/cya;

	nr=nr/cyb; ng=ng/cyb;
	nb=nb/cyb; ny=ny/cyb;

	mdy=mdy/cya;
	ndy=ndy/cya;
	ady=(mdy+ndy)/2;
#endif

#if 1
	my2=256; ny2=-1;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];

		switch(ax)
		{
		case 0: cy=(cr+2*cg+cb)/4; break;
		case 1: cy=(1*cr+4*cg+3*cb)/8; break;
		case 2: cy=(4*cr+1*cg+3*cb)/8; break;
		case 3: cy=(3*cr+4*cg+1*cb)/8; break;
		}
		
		pxy[i*4+j]=cy;
		if(cy<my2)
			{ my2=cy; mr2=cr; mg2=cg; mb2=cb; }
		if(cy>ny2)
			{ ny2=cy; nr2=cr; ng2=cg; nb2=cb; }
	}

#if 0	
	mr=(mr+mr2)/2;
	mg=(mg+mg2)/2;
	mb=(mb+mb2)/2;
	my=(my+my2)/2;

	nr=(nr+nr2)/2;
	ng=(ng+ng2)/2;
	nb=(nb+nb2)/2;
	ny=(ny+ny2)/2;
#endif

#if 1
	mr=(3*mr+mr2)/4;
	mg=(3*mg+mg2)/4;
	mb=(3*mb+mb2)/4;
	my=(3*my+my2)/4;

	nr=(3*nr+nr2)/4;
	ng=(3*ng+ng2)/4;
	nb=(3*nb+nb2)/4;
	ny=(3*ny+ny2)/4;
#endif

#endif
	
	ctsp=0;
	ia=vqenc_rgb2p64(mr, mg, mb, ctsp);
	ib=vqenc_rgb2p64(nr, ng, nb, ctsp);
	ixa=0x4000|(ctsp<<12)|(ia<<6)|ib;
//	ixa=0x4000|(ctsp<<12)|(ib<<6)|ia;

//	ctsp=1;
//	ia=vqenc_rgb2p64(mr, mg, mb, ctsp);
//	ib=vqenc_rgb2p64(nr, ng, nb, ctsp);
//	ixc=0x4000|(ctsp<<12)|(ia<<6)|ib;

	cya=(5*my+3*ny)/8;
	cyb=(3*my+5*ny)/8;

	px=0;

#if 0
	if(pxy[ 0]>cya)px|=0x0008;
	if(pxy[ 1]>cyb)px|=0x0004;
	if(pxy[ 2]>cya)px|=0x0002;
	if(pxy[ 3]>cyb)px|=0x0001;

	if(pxy[ 4]>cyb)px|=0x0080;
	if(pxy[ 5]>cya)px|=0x0040;
	if(pxy[ 6]>cyb)px|=0x0020;
	if(pxy[ 7]>cya)px|=0x0010;

	if(pxy[ 8]>cya)px|=0x0800;
	if(pxy[ 9]>cyb)px|=0x0400;
	if(pxy[10]>cya)px|=0x0200;
	if(pxy[11]>cyb)px|=0x0100;

	if(pxy[12]>cyb)px|=0x8000;
	if(pxy[13]>cya)px|=0x4000;
	if(pxy[14]>cyb)px|=0x2000;
	if(pxy[15]>cya)px|=0x1000;
#endif

#if 1
	if(pxy[ 0]>cya)px|=0x8000;
	if(pxy[ 1]>cyb)px|=0x4000;
	if(pxy[ 2]>cya)px|=0x2000;
	if(pxy[ 3]>cyb)px|=0x1000;

	if(pxy[ 4]>cyb)px|=0x0800;
	if(pxy[ 5]>cya)px|=0x0400;
	if(pxy[ 6]>cyb)px|=0x0200;
	if(pxy[ 7]>cya)px|=0x0100;

	if(pxy[ 8]>cya)px|=0x0080;
	if(pxy[ 9]>cyb)px|=0x0040;
	if(pxy[10]>cya)px|=0x0020;
	if(pxy[11]>cyb)px|=0x0010;

	if(pxy[12]>cyb)px|=0x0008;
	if(pxy[13]>cya)px|=0x0004;
	if(pxy[14]>cyb)px|=0x0002;
	if(pxy[15]>cya)px|=0x0001;
#endif

	blka=px|(ixa<<16);
//	blkb=px|(ixb<<16);
//	blkc=px|(ixc<<16);

	blk=blka;

	*oblk=blk;
//	*rsta=stb;
	return(0);
}



int vqwenc(byte *iblk, int ystr,
	u32 *oblka, u32 *oblkb, u32 *oblkc, u32 *oblkd)
{
	byte pxa[64*4];
	byte pxb[64*4];
	byte pxc[64*4];
	short pxy[64];
	short pxu[64];
	short pxv[64];
	
	u64 sta, stb, stc, std;
	int min, max;
	u32 blk, blka, blkb, blkc;
	u64 pxw;

	int acr, acg, acb, acy;

	int cr, cg, cb;
	int mr, mg, mb, my, mdy, mu, mv;
	int nr, ng, nb, ny, ndy, nu, nv;
	int cy, cu, cv;

	int mr2, mg2, mb2, my2;
	int nr2, ng2, nb2, ny2;

	int cy_c, cy_m, cy_y;
	int my_c, my_m, my_y;
	int ny_c, ny_m, ny_y;
	int ax, dy;
	int e0, e1;

	int cya, cyb, ctsp;
	int cua, cub, cva, cvb;
	int acu, acv, ady;
	int ia, ib, ix, px;
	int ixa, ixb, ixc;
	int tpxu, tpxv;
	int i, j, k, l;
	
	my=256; ny=-1;
	mu=256; nu=-1;
	mv=256; nv=-1;
	my_c=256; ny_c=-1;
	my_m=256; ny_m=-1;
	my_y=256; ny_y=-1;
	acr=0; acg=0; acb=0; acy=0;
	acu=0; acv=0;
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];
		cy=(cr+2*cg+cb)/4;

		k=i*8+j;
//		k=(3-i)*4+j;
		pxc[k*4+0]=cr;
		pxc[k*4+1]=cg;
		pxc[k*4+2]=cb;
		pxc[k*4+3]=255;

		cy_c=(1*cr+4*cg+3*cb)/8;
		cy_m=(4*cr+1*cg+3*cb)/8;
		cy_y=(3*cr+4*cg+1*cb)/8;

		cu=((cb-cg)/2)+128;
		cv=((cr-cg)/2)+128;

		if(cy<my) { my=cy; }
		if(cy>ny) { ny=cy; }
		if(cu<mu) { mu=cu; }
		if(cu>nu) { nu=cu; }
		if(cv<mv) { mv=cv; }
		if(cv>nv) { nv=cv; }

		if(cy_c<my_c) { my_c=cy_c; }
		if(cy_c>ny_c) { ny_c=cy_c; }
		if(cy_m<my_m) { my_m=cy_m; }
		if(cy_m>ny_m) { ny_m=cy_m; }
		if(cy_y<my_y) { my_y=cy_y; }
		if(cy_y>ny_y) { ny_y=cy_y; }
		
		acr+=cr;	acg+=cg;
		acb+=cb;	acy+=cy;
		acu+=cu;	acv+=cv;
	}
	
	acr=acr/64;	acg=acg/64;
	acb=acb/64;	acy=acy/64;
	acu=acu/64;	acv=acv/64;

	acy=(2*acy+my+ny)/4;

	ax=0; dy=ny-my;

#if 1
	dy=2*(ny-acy);
	
//	i=ny_c-my_c; if(i>dy) { ax=1; dy=i; }
//	i=ny_m-my_m; if(i>dy) { ax=2; dy=i; }
//	i=ny_y-my_y; if(i>dy) { ax=3; dy=i; }

	mr=0; mg=0; mb=0; my=0; cya=0;
	nr=0; ng=0; nb=0; ny=0; cyb=0;
	mu=0; mv=0; cua=0; cva=0;
	nu=0; nv=0; cub=0; cvb=0;
	mdy=0; ndy=0;
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];
//		cy=(cr+2*cg+cb)/4;

		cu=(cb-cg)/2+128;
		cv=(cr-cg)/2+128;

		switch(ax)
		{
		case 0: cy=(cr+2*cg+cb)/4; break;
		case 1: cy=(1*cr+4*cg+3*cb)/8; break;
		case 2: cy=(4*cr+1*cg+3*cb)/8; break;
		case 3: cy=(3*cr+4*cg+1*cb)/8; break;
		}

		pxy[i*8+j]=cy;
		pxu[i*8+j]=cu;
		pxv[i*8+j]=cv;
		
		if(cu<acu)		{ mu+=cu; cua++; }
		else			{ nu+=cu; cub++; }

		if(cv<acv)		{ mv+=cv; cva++; }
		else			{ nv+=cv; cvb++; }
		
		if(cy<acy)
			{ mr+=cr; mg+=cg; mb+=cb; my+=cy; cya++; mdy+=acy-cy; }
		else
			{ nr+=cr; ng+=cg; nb+=cb; ny+=cy; cyb++; ndy+=cy-acy; }
	}
	
	if(!cya)
	{
		mr+=acr;
		mg+=acg;
		mb+=acb;
		cya++;
	}
	if(!cyb)
	{
		nr+=acr;
		ng+=acg;
		nb+=acb;
		cyb++;
	}

	mr=mr/cya; mg=mg/cya;
	mb=mb/cya; my=my/cya;

	nr=nr/cyb; ng=ng/cyb;
	nb=nb/cyb; ny=ny/cyb;

	mdy=mdy/cya;
	ndy=ndy/cya;
	ady=(mdy+ndy)/2;
	
	if(cua)mu=mu/cua;
		else mu=acu;
	if(cva)mv=mv/cva;
		else mv=acv;
	if(cub)nu=nu/cub;
		else nu=acu;
	if(cvb)nv=nv/cvb;
		else nv=acv;
#endif

#if 1
	my2=256; ny2=-1;
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];

		switch(ax)
		{
		case 0: cy=(cr+2*cg+cb)/4; break;
		case 1: cy=(1*cr+4*cg+3*cb)/8; break;
		case 2: cy=(4*cr+1*cg+3*cb)/8; break;
		case 3: cy=(3*cr+4*cg+1*cb)/8; break;
		}
		
		pxy[i*8+j]=cy;
		if(cy<my2)
			{ my2=cy; mr2=cr; mg2=cg; mb2=cb; }
		if(cy>ny2)
			{ ny2=cy; nr2=cr; ng2=cg; nb2=cb; }
	}

#if 0	
	mr=(mr+mr2)/2;
	mg=(mg+mg2)/2;
	mb=(mb+mb2)/2;
	my=(my+my2)/2;

	nr=(nr+nr2)/2;
	ng=(ng+ng2)/2;
	nb=(nb+nb2)/2;
	ny=(ny+ny2)/2;
#endif

#if 1
	mr=(3*mr+mr2)/4;
	mg=(3*mg+mg2)/4;
	mb=(3*mb+mb2)/4;
	my=(3*my+my2)/4;

	nr=(3*nr+nr2)/4;
	ng=(3*ng+ng2)/4;
	nb=(3*nb+nb2)/4;
	ny=(3*ny+ny2)/4;
#endif

#endif
	
	ctsp=0;
//	ia=vqenc_rgb2p64(mr, mg, mb, ctsp);
//	ib=vqenc_rgb2p64(nr, ng, nb, ctsp);
//	ixa=0x4000|(ctsp<<12)|(ia<<6)|ib;
//	ixa=0x4000|(ctsp<<12)|(ib<<6)|ia;

//	my=(mr+2*mg+mb)/4;
//	ny=(nr+2*ng+nb)/4;

//	mu=(mb-mg)/2+128;	mv=(mr-mg)/2+128;
//	nu=(nb-ng)/2+128;	nv=(nr-ng)/2+128;

	my=clamp255(my);	ny=clamp255(ny);
	mu=clamp255(mu);	nu=clamp255(nu);
	mv=clamp255(mv);	nv=clamp255(nv);

//	my=255;		ny=255;
//	mu=128;		nu=128;
//	mv=128;		nv=128;

	ixa=0x80000000|
		((my>>2)<<22)|
		((ny>>2)<<16)|
		((mu>>4)<<12)|
		((nu>>4)<< 8)|
		((mv>>4)<< 4)|
		((nv>>4)<< 0);

	cya=(5*my+3*ny)/8;
	cyb=(3*my+5*ny)/8;

	px=0;

#if 1
	pxw=0;
	for(i=0; i<8; i++)
	{
		if(pxy[i*8+0]>cya)pxw|=0x80ULL<<((7-i)*8);
		if(pxy[i*8+1]>cyb)pxw|=0x40ULL<<((7-i)*8);
		if(pxy[i*8+2]>cya)pxw|=0x20ULL<<((7-i)*8);
		if(pxy[i*8+3]>cyb)pxw|=0x10ULL<<((7-i)*8);
		if(pxy[i*8+4]>cyb)pxw|=0x08ULL<<((7-i)*8);
		if(pxy[i*8+5]>cya)pxw|=0x04ULL<<((7-i)*8);
		if(pxy[i*8+6]>cyb)pxw|=0x02ULL<<((7-i)*8);
		if(pxy[i*8+7]>cya)pxw|=0x01ULL<<((7-i)*8);
	}
#endif

#if 1
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		pxu[(i*2+0)*8+(j*2+0)] =
		(	pxu[(i*2+0)*8+(j*2+0)]+
			pxu[(i*2+0)*8+(j*2+1)]+
			pxu[(i*2+1)*8+(j*2+0)]+
			pxu[(i*2+1)*8+(j*2+1)] )/4;
		pxv[(i*2+0)*8+(j*2+0)] =
		(	pxv[(i*2+0)*8+(j*2+0)]+
			pxv[(i*2+0)*8+(j*2+1)]+
			pxv[(i*2+1)*8+(j*2+0)]+
			pxv[(i*2+1)*8+(j*2+1)] )/4;
	}

	tpxu=0; tpxv=0;
	for(i=0; i<4; i++)
	{
		if(nu>=mu)
		{
			if(pxu[(i*2)*8+0]>acu)tpxu|=0x8ULL<<((3-i)*4);
			if(pxu[(i*2)*8+2]>acu)tpxu|=0x4ULL<<((3-i)*4);
			if(pxu[(i*2)*8+4]>acu)tpxu|=0x2ULL<<((3-i)*4);
			if(pxu[(i*2)*8+6]>acu)tpxu|=0x1ULL<<((3-i)*4);
		}else
		{
			if(pxu[(i*2)*8+0]<acu)tpxu|=0x8ULL<<((3-i)*4);
			if(pxu[(i*2)*8+2]<acu)tpxu|=0x4ULL<<((3-i)*4);
			if(pxu[(i*2)*8+4]<acu)tpxu|=0x2ULL<<((3-i)*4);
			if(pxu[(i*2)*8+6]<acu)tpxu|=0x1ULL<<((3-i)*4);
		}

		if(nv>=mv)
		{
			if(pxv[(i*2)*8+0]>acv)tpxv|=0x8ULL<<((3-i)*4);
			if(pxv[(i*2)*8+2]>acv)tpxv|=0x4ULL<<((3-i)*4);
			if(pxv[(i*2)*8+4]>acv)tpxv|=0x2ULL<<((3-i)*4);
			if(pxv[(i*2)*8+6]>acv)tpxv|=0x1ULL<<((3-i)*4);
		}else
		{
			if(pxv[(i*2)*8+0]<acv)tpxv|=0x8ULL<<((3-i)*4);
			if(pxv[(i*2)*8+2]<acv)tpxv|=0x4ULL<<((3-i)*4);
			if(pxv[(i*2)*8+4]<acv)tpxv|=0x2ULL<<((3-i)*4);
			if(pxv[(i*2)*8+6]<acv)tpxv|=0x1ULL<<((3-i)*4);
		}
	}
	
	blkb=(tpxu<<16)|tpxv;
#endif

//	blka=px|(ixa<<16);
//	blkb=px|(ixb<<16);
//	blkc=px|(ixc<<16);

	blka=ixa;

	*oblka=blka;
	*oblkb=blkb;
	*oblkc=pxw;
	*oblkd=pxw>>32;

//	*rsta=stb;
	return(0);
}



u64 vqtenc_getcellbits(int glyph)
{
	u64 cell;

	if(glyph&256)
	{
		if(glyph&128)
			cell=gfxfont[glyph&127];
		else
			cell=btesh2_gfxcon_glyphs[glyph&127];
	}else
	{
		cell=btesh2_gfxcon_glyphs[glyph&127];
	}
}

double vqtenc_chkcell_err(short *pxy, int cya, int cyb, u64 cell)
{
	double e;
	int y0, y1, y2;
	int i, j, k;

	e=0;
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
	{
		k=(cell>>((7-i)*8+(7-j)))&1;
	
		y0=pxy[i*8+j];
		y1=k?cyb:cya;
		y2=y0-y1;
		e+=y2*y2;
	}
//	return(sqrt(e/64.0));
	return(e);
}

int vqtenc_bestcell(short *pxy, int cya, int cyb)
{
	u64 cell;
	double e, be;
	int bi;
	int i, j, k;

	be=999999999.0; bi=0;
//	for(i=0; i<192; i++)
//	for(i=0; i<128; i++)
	for(i=0; i<256; i++)
//	for(i=128; i<192; i++)
	{
		j=i;
//		j=i^0x80;

#if 1
		cell=vqtenc_getcellbits(0x100|j);
//		cell=gfxfont[j];
		e=vqtenc_chkcell_err(pxy, cya, cyb, cell);
		if(e<be)	{ bi=0x0100|j; be=e; }
		e=vqtenc_chkcell_err(pxy, cyb, cya, cell);
		if(e<be)	{ bi=0x1100|j; be=e; }
#endif

#if 1
		cell=vqtenc_getcellbits(0x000|j);
//		cell=btesh2_gfxcon_glyphs[j];
		e=vqtenc_chkcell_err(pxy, cya, cyb, cell);
		if(e<be)	{ bi=0x0000|j; be=e; }
		e=vqtenc_chkcell_err(pxy, cyb, cya, cell);
		if(e<be)	{ bi=0x1000|j; be=e; }
#endif
	}
	return(bi);
}

int vqtenc(byte *iblk, int ystr, u32 *oblk)
{
	byte pxa[64*4];
	byte pxb[64*4];
	byte pxc[64*4];
	short pxy[64];
	
	u64 sta, stb, stc, std;
	int min, max;
	u32 blk, blka, blkb, blkc;

	int acr, acg, acb, acy;

	int cr, cg, cb, cy;
	int mr, mg, mb, my, mdy;
	int nr, ng, nb, ny, ndy;
	int cu, cv;

	int mr2, mg2, mb2, my2;
	int nr2, ng2, nb2, ny2;

	int cy_c, cy_m, cy_y;
	int my_c, my_m, my_y;
	int ny_c, ny_m, ny_y;
	int ax, dy;
	int e0, e1;

	int cya, cyb, ctsp;
	int acu, acv, ady;
	int ia, ib, ix, px;
	int ixa, ixb, ixc;
	int i, j, k, l;
	
	my=256; ny=-1;
	my_c=256; ny_c=-1;
	my_m=256; ny_m=-1;
	my_y=256; ny_y=-1;
	acr=0; acg=0; acb=0; acy=0;
	acu=0; acv=0;
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];
		cy=(cr+2*cg+cb)/4;

		k=i*8+j;
		pxc[k*4+0]=cr;
		pxc[k*4+1]=cg;
		pxc[k*4+2]=cb;
		pxc[k*4+3]=255;

		pxy[i*8+j]=cy;

		cy_c=(1*cr+4*cg+3*cb)/8;
		cy_m=(4*cr+1*cg+3*cb)/8;
		cy_y=(3*cr+4*cg+1*cb)/8;

		cu=((cb-cg)/2)+128;
		cv=((cr-cg)/2)+128;

		if(cy<my) { my=cy; mr=cr; mg=cg; mb=cb; }
		if(cy>ny) { ny=cy; nr=cr; ng=cg; nb=cb; }
		if(cy_c<my_c) { my_c=cy_c; }
		if(cy_c>ny_c) { ny_c=cy_c; }
		if(cy_m<my_m) { my_m=cy_m; }
		if(cy_m>ny_m) { ny_m=cy_m; }
		if(cy_y<my_y) { my_y=cy_y; }
		if(cy_y>ny_y) { ny_y=cy_y; }
		
		acr+=cr;	acg+=cg;
		acb+=cb;	acy+=cy;
		acu+=cu;	acv+=cv;
	}
	
	acr=acr/64;	acg=acg/64;
	acb=acb/64;	acy=acy/64;
	acu=acu/64;	acv=acv/64;

	acy=(2*acy+my+ny)/4;

	ax=0; dy=ny-my;

#if 1
	dy=2*(ny-acy);

	cya=(5*my+3*ny)/8;
	cyb=(3*my+5*ny)/8;

	mr=0; mg=0; mb=0; my=0; cya=0;
	nr=0; ng=0; nb=0; ny=0; cyb=0;
	mdy=0; ndy=0;
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];

		switch(ax)
		{
		case 0: cy=(cr+2*cg+cb)/4; break;
		case 1: cy=(1*cr+4*cg+3*cb)/8; break;
		case 2: cy=(4*cr+1*cg+3*cb)/8; break;
		case 3: cy=(3*cr+4*cg+1*cb)/8; break;
		}

		pxy[i*8+j]=cy;
		
		if(cy<acy)
//		if(cy<cya)
			{ mr+=cr; mg+=cg; mb+=cb; my+=cy; cya++; mdy+=acy-cy; }
		else
//		if(cy>cyb)
			{ nr+=cr; ng+=cg; nb+=cb; ny+=cy; cyb++; ndy+=cy-acy; }
	}
	
	if(!cya)
	{
		mr+=acr; mg+=acg; mb+=acb; my+=acy;
		cya++;
	}
	if(!cyb)
	{
		nr+=acr; ng+=acg; nb+=acb; ny+=acy;
		cyb++;
	}

	mr=mr/cya; mg=mg/cya;
	mb=mb/cya; my=my/cya;

	nr=nr/cyb; ng=ng/cyb;
	nb=nb/cyb; ny=ny/cyb;

	mdy=mdy/cya;
	ndy=ndy/cya;
	ady=(mdy+ndy)/2;
#endif
	
//	mr=my; mg=my; mb=my;
//	nr=ny; ng=ny; nb=ny;
	
#if 0
	ctsp=0;
	ia=vqenc_rgb2p64(mr, mg, mb, ctsp);
	ib=vqenc_rgb2p64(nr, ng, nb, ctsp);
	ixa=0x0000|(ctsp<<12)|(ia<<6)|ib;
	ixb=0x0000|(ctsp<<12)|(ib<<6)|ia;
#endif

#if 1
	ctsp=2;
	ia=vqenc_rgb2p512(mr, mg, mb, ctsp);
	ib=vqenc_rgb2p512(nr, ng, nb, ctsp);
	ixa=0x0000|(ctsp<<18)|(ia<<9)|ib;
	ixb=0x0000|(ctsp<<18)|(ib<<9)|ia;
#endif

//	ctsp=1;
//	ia=vqenc_rgb2p64(mr, mg, mb, ctsp);
//	ib=vqenc_rgb2p64(nr, ng, nb, ctsp);
//	ixc=0x4000|(ctsp<<12)|(ia<<6)|ib;
//	ixa=0x0000|(ctsp<<12)|(ia<<6)|ib;
//	ixb=0x0000|(ctsp<<12)|(ib<<6)|ia;

	cya=(5*my+3*ny)/8;
	cyb=(3*my+5*ny)/8;

	px=0;
	
	px=vqtenc_bestcell(pxy, my, ny);
	if(px&0x1000)
		{ ixa=ixb; }

//	px=px&4095;
//	blka=px|(ixa<<16);

	px=px&1023;
	blka=px|(ixa<<10);

	blk=blka;

	*oblk=blk;
//	*rsta=stb;
	return(0);
}


int vqenc2x(byte *iblk, int ystr, u32 *oblk)
{
	byte px[4*4*4];
	int cr, cg, cb;
	int i1, j1, k1;
	int i, j, k;

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=(	iblk[(i*2+0)*ystr+(j*2+0)*4+0] +
				iblk[(i*2+0)*ystr+(j*2+1)*4+0] +
				iblk[(i*2+1)*ystr+(j*2+0)*4+0] +
				iblk[(i*2+1)*ystr+(j*2+1)*4+0] )/4;
		cg=(	iblk[(i*2+0)*ystr+(j*2+0)*4+1] +
				iblk[(i*2+0)*ystr+(j*2+1)*4+1] +
				iblk[(i*2+1)*ystr+(j*2+0)*4+1] +
				iblk[(i*2+1)*ystr+(j*2+1)*4+1] )/4;
		cb=(	iblk[(i*2+0)*ystr+(j*2+0)*4+2] +
				iblk[(i*2+0)*ystr+(j*2+1)*4+2] +
				iblk[(i*2+1)*ystr+(j*2+0)*4+2] +
				iblk[(i*2+1)*ystr+(j*2+1)*4+2] )/4;
		px[(i*4+j)*4+0]=cr;
		px[(i*4+j)*4+1]=cg;
		px[(i*4+j)*4+2]=cb;
		px[(i*4+j)*4+3]=255;
	}
	
	vqenc(px, 16, oblk);
	return(0);
}

double vqencb_err(byte *iblk, int ystr, byte *pxt)
{
	double e;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int i, j, k;

	e=0;
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
	{
		cr0=iblk[i*ystr+j*4+0];
		cg0=iblk[i*ystr+j*4+1];
		cb0=iblk[i*ystr+j*4+2];
		
		cr1=pxt[(i*8+j)*4+0];
		cg1=pxt[(i*8+j)*4+1];
		cb1=pxt[(i*8+j)*4+2];
		
		cr2=cr0-cr1;
		cg2=cg0-cg1;
		cb2=cb0-cb1;
		
		e+=cr2*cr2;
		e+=2*cg2*cg2;
		e+=cb2*cb2;
	}
	
	return(e);
}

int vqtdec(byte *iblk, int ystr, u32 blk);

int vqencb(byte *iblk, int ystr, u32 *oblk)
{
	byte pxt[8*8*4];
	double e0, e1, be;
	u32 blk0, blk1;
	u32 blk;

	vqtenc(iblk, ystr, &blk0);
	vqenc2x(iblk, ystr, &blk1);
	
	vqtdec(pxt, 8*4, blk0);
	e0=vqencb_err(iblk, ystr, pxt);

	vqtdec(pxt, 8*4, blk1);
	e1=vqencb_err(iblk, ystr, pxt);
	
	blk=blk0; be=e0;
	if(e1<be) { blk=blk1; be=e1; }
	
	*oblk=blk;
//	return(blk);
}

int vqtdec(byte *iblk, int ystr, u32 blk)
{
	byte pxt[4*4*4];
	u32 clra, clrb;
	u64 pxa;
	int cr, cg, cb;
	int cpa, cpt, cpb, csp;
	int i, j, k;
	
	cpt=(blk>>(16+14))&3;
	
	if(cpt==1)
//	if(0)
	{
		vqdec(pxt, 16, blk);
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			cr=pxt[(i*4+j)*4+0];
			cg=pxt[(i*4+j)*4+1];
			cb=pxt[(i*4+j)*4+2];
			iblk[(i*2+0)*ystr+(j*2+0)*4+0]=cr;
			iblk[(i*2+0)*ystr+(j*2+0)*4+1]=cg;
			iblk[(i*2+0)*ystr+(j*2+0)*4+2]=cb;
			iblk[(i*2+0)*ystr+(j*2+1)*4+0]=cr;
			iblk[(i*2+0)*ystr+(j*2+1)*4+1]=cg;
			iblk[(i*2+0)*ystr+(j*2+1)*4+2]=cb;
			iblk[(i*2+1)*ystr+(j*2+0)*4+0]=cr;
			iblk[(i*2+1)*ystr+(j*2+0)*4+1]=cg;
			iblk[(i*2+1)*ystr+(j*2+0)*4+2]=cb;
			iblk[(i*2+1)*ystr+(j*2+1)*4+0]=cr;
			iblk[(i*2+1)*ystr+(j*2+1)*4+1]=cg;
			iblk[(i*2+1)*ystr+(j*2+1)*4+2]=cb;
		}
		return(1);
	}

	csp=(blk>>(16+12))&3;
	cpa=(blk>>(16+6))&63;
	cpb=(blk>>(16+0))&63;
	clra=vqdec_clr6(cpa, csp);
	clrb=vqdec_clr6(cpb, csp);
	
	if(csp==2)
	{
		cpa=(blk>>19)&511;
		cpb=(blk>>10)&511;
		clra=vqdec_clr9(cpa, csp);
		clrb=vqdec_clr9(cpb, csp);

		pxa=vqtenc_getcellbits(blk&1023);

//		if((blk&0x0300)==0x100)
//			pxa=gfxfont[blk&255];
//		else
//			pxa=btesh2_gfxcon_glyphs[blk&255];
	}else
	{
		pxa=vqtenc_getcellbits(blk&1023);

//		if((blk&0x0F00)==0x100)
//			pxa=gfxfont[blk&255];
//		else
//			pxa=btesh2_gfxcon_glyphs[blk&255];
	}

	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
	{
		k=(pxa>>((7-i)*8+(7-j)))&1;
		if(k)
		{	cb=(clrb>>16)&255;
			cg=(clrb>> 8)&255;
			cr=(clrb    )&255;	}
		else
		{	cb=(clra>>16)&255;
			cg=(clra>> 8)&255;
			cr=(clra    )&255;	}
		
		iblk[i*ystr+j*4+0]=cr;
		iblk[i*ystr+j*4+1]=cg;
		iblk[i*ystr+j*4+2]=cb;
	}
}

int vqwdec(byte *iblk, int ystr,
	u32 blka, u32 blkb, u32 blkc, u32 blkd)
{
	u32 clr, clra, clrb;
	u64 pxa;
	int pxu, pxv;
	int my, mu, mv;
	int ny, nu, nv;
	int cy, cu, cv;
	int cr, cg, cb;
	int i, j, k;

	my=(blka>>22)&63;	ny=(blka>>16)&63;
	mu=(blka>>12)&15;	nu=(blka>> 8)&15;
	mv=(blka>> 4)&15;	nv=(blka>> 0)&15;
	
	my=(my<<2)|(my>>4);	ny=(ny<<2)|(ny>>4);
	mu=mu<<4;		nu=nu<<4;
	mv=mv<<4;		nv=nv<<4;
	
//	mu=128;		nu=128;
//	mv=128;		nv=128;
	
	clra=vqdec_fromyuv(my, mu, mv);
	clrb=vqdec_fromyuv(ny, nu, nv);

	pxa=blkc | (((u64)blkd)<<32);
	
	pxu=(u16)(blkb>>16);	pxv=(u16)(blkb    );
//	pxv=(u16)(blkb>>16);	pxu=(u16)(blkb    );

#if 1
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
	{
		k=((7-i)*8+(7-j));
		cy=((pxa>>k)&1)?ny:my;

		k=((3-(i/2))*4+(3-(j/2)));
		cu=((pxu>>k)&1)?nu:mu;
		cv=((pxv>>k)&1)?nv:mv;

		clr=vqdec_fromyuv(cy, cu, cv);

		cb=(clr>>16)&255;
		cg=(clr>> 8)&255;
		cr=(clr    )&255;
		
		iblk[i*ystr+j*4+0]=cr;
		iblk[i*ystr+j*4+1]=cg;
		iblk[i*ystr+j*4+2]=cb;
	}
#endif

#if 0
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
	{
		k=(pxa>>((7-i)*8+(7-j)))&1;
		if(k)
		{	cb=(clrb>>16)&255;
			cg=(clrb>> 8)&255;
			cr=(clrb    )&255;	}
		else
		{	cb=(clra>>16)&255;
			cg=(clra>> 8)&255;
			cr=(clra    )&255;	}
		
		iblk[i*ystr+j*4+0]=cr;
		iblk[i*ystr+j*4+1]=cg;
		iblk[i*ystr+j*4+2]=cb;
	}
#endif
}

int main()
{
	int celstat[512];
	int celidx[512];
	u32 *blkbuf;
	byte *ibuf, *dbuf;
	byte *obuf;
	char *ifn, *ofn;
	int isz, xs, ys, sz;
	int oxs, oys;
	u32 blk, blka, blkb, blkc, blkd;
	u64 pxa;
	char *s;
	FILE *fda;
	FILE *fdb;
	FILE *fdc;
	FILE *fdd;
	int ch, clr;
	int i, j, k;

//	ifn="../DE3D0_160x100.tga";
	ifn="../DE3D0.tga";
//	ifn="../4.2.03_320.tga";
//	ifn="../Lenna_320.tga";
	ibuf=BTIC1H_Img_LoadTGA(ifn, &xs, &ys);

	blkbuf=malloc(40*25*4*sizeof(u32));
	obuf=malloc((42*8)*(28*8)*4);

	fda=fopen("scrmem_1a.txt", "wt");
	fdb=fopen("scrmem_1b.txt", "wt");
	fdc=fopen("scrmem_1c.txt", "wt");
	fdd=fopen("scrmem_1d.txt", "wt");
	
	for(i=0; i<25; i++)
	{
		for(j=0; j<40; j++)
		{
//			vqenc(ibuf+((ys-i*4-1)*xs+j*4)*4, -xs*4, &blk);
//			vqenc2x(ibuf+((ys-i*8-1)*xs+j*8)*4, -xs*4, &blk);
//			vqtenc(ibuf+((ys-i*8-1)*xs+j*8)*4, -xs*4, &blk);
//			vqencb(ibuf+((ys-i*8-1)*xs+j*8)*4, -xs*4, &blka);
			vqwenc(ibuf+((ys-i*8-1)*xs+j*8)*4, -xs*4,
				&blka, &blkb, &blkc, &blkd);
			blkbuf[(i*40+j)*4+0]=blka;
			blkbuf[(i*40+j)*4+1]=blkb;
			blkbuf[(i*40+j)*4+2]=blkc;
			blkbuf[(i*40+j)*4+3]=blkd;
			fprintf(fda, "%08X\n", blka);
			fprintf(fdb, "%08X\n", blkb);
			fprintf(fdc, "%08X\n", blkc);
			fprintf(fdd, "%08X\n", blkd);
		}
	}

	fclose(fda);
	fclose(fdb);
	fclose(fdc);
	fclose(fdd);

	for(i=0; i<512; i++)
	{
		celstat[i]=0;
		celidx[i]=i;
	}

	oxs=40*8;
	oys=25*8;
	for(i=0; i<25; i++)
	{
		for(j=0; j<40; j++)
		{
			blka=blkbuf[(i*40+j)*4+0];
			blkb=blkbuf[(i*40+j)*4+1];
			blkc=blkbuf[(i*40+j)*4+2];
			blkd=blkbuf[(i*40+j)*4+3];
			
			blk=blka;
			
			if(blk>>30)
				celstat[512-(blk>>30)]++;
			else
				celstat[blk&511]++;
			
//			vqtdec(obuf+((oys-i*8-1)*oxs+j*8)*4, -oxs*4, blk);
			vqwdec(obuf+((oys-i*8-1)*oxs+j*8)*4, -oxs*4,
				blka, blkb, blkc, blkd);
		}
	}

	
//	fprintf(fd, "%04X_%04X\n", clr, ' ');
	
	BTIC1H_Img_SaveTGA("txtmem1.tga", obuf, oxs, oys);
	
#if 0
	for(i=0; i<512; i++)
		for(j=i+1; j<512; j++)
	{
		if(celstat[j]>celstat[i])
		{
			k=celstat[i]; celstat[i]=celstat[j]; celstat[j]=k;
			k=celidx[i]; celidx[i]=celidx[j]; celidx[j]=k;
		}
	}

	for(i=0; i<64; i++)
	{
		j=celidx[i];
//		if(j>=0x100)
//			pxa=gfxfont[j&255];
//		else
//			pxa=btesh2_gfxcon_glyphs[j&255];
		pxa=vqtenc_getcellbits(j);

		s="?  ";
		if((j>=0x180) && (j<=0x1FF))s=".  ";
//		if((j>=0x100) && (j<=0x17F))s=".  ";
//		if((j>=0x180) && (j<=0x1BF))s="BC7";
		if((j>=0x000) && (j<=0x07F))s="ASC";
		if((j>=0x080) && (j<=0x0FF))s="437";
		if(j==0x1FF)s="PIX";
		printf("%03X %3d %s %016llX\n", celidx[i], celstat[i], s, pxa);
	}
#endif
}
