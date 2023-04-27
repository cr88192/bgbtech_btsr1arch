int GfxDrv_PrepareFramebuf();
int JX2I_GfxCon_UpdateForRegs();

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
0x00081828487E0800,	//0x34 "4"
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

#ifdef linux
byte *btesh2_gfxcon_framebuf;
int btesh2_gfxcon_fb_dirty;
int btesh2_gfxcon_fbxs;
int btesh2_gfxcon_fbys;
byte btesh2_gfxcon_swaprb=0;
#endif

u32 *jx2i_gfxcon_conbuf;
u32 *jx2i_gfxcon_lconbuf;
byte jx2i_gfxcon_dirty;
byte jx2i_gfxcon_is80col;
byte jx2i_gfxcon_is50row;
byte jx2i_gfxcon_ishalfcell;
byte jx2i_gfxcon_isqtrcell;
byte jx2i_gfxcon_is800px;
byte jx2i_gfxcon_ispow2;

byte jx2i_gfxcon_is2xcol;
byte jx2i_gfxcon_is2xrow;

byte jx2i_gfxcon_isbmap;

u32 jx2i_gfxcon_cbfrnum;
u32 jx2i_gfxcon_cblfrnum;
int jx2i_gfxcon_cbffms;

byte jx2i_gfxcon_ncx;
byte jx2i_gfxcon_ncy;

u64 jx2i_gfxcon_ctrlreg[64];
u64 jx2i_gfxcon_fontram[1024];

u64 jx2i_gfxcon_seed=0;

u32 JX2I_GfxCon_Rng32(void)
{
	u64 seed0, seed1;
	seed0=jx2i_gfxcon_seed;
	seed1=(seed0+1)*2147483647;
	jx2i_gfxcon_seed=seed1;
//	return((seed1>>32)^seed0);
//	return(seed1>>32);
	return((seed1>>32)+seed0);
}

int JX2I_GfxCon_Startup()
{
	int i;

	for(i=0; i<64; i++)
		jx2i_gfxcon_ctrlreg[i]=0;

//	btesh2_gfxcon_fbxs=320;
//	btesh2_gfxcon_fbys=200;
	btesh2_gfxcon_fbxs=640;
	btesh2_gfxcon_fbys=400;
//	btesh2_gfxcon_fbxs=800;
//	btesh2_gfxcon_fbys=600;

	jx2i_gfxcon_is80col=0;
	jx2i_gfxcon_is50row=0;
	jx2i_gfxcon_ishalfcell=0;
	jx2i_gfxcon_isbmap=0;
	jx2i_gfxcon_is800px=0;

	jx2i_gfxcon_ncx=40;
	jx2i_gfxcon_ncy=25;

	btesh2_gfxcon_fb_dirty=1;
	GfxDrv_PrepareFramebuf();

	JX2I_GfxCon_UpdateForRegs();

//	btesh2_gfxcon_fbxs=640;
//	btesh2_gfxcon_fbys=400;
	
	jx2i_gfxcon_seed=rand()*rand()*rand();
	
//	jx2i_gfxcon_conbuf=malloc(40*25*4*sizeof(u32));
//	jx2i_gfxcon_conbuf=malloc(4096*sizeof(u32));
//	jx2i_gfxcon_conbuf=malloc(8192*sizeof(u32));
//	jx2i_gfxcon_conbuf=malloc(16384*sizeof(u32));
//	jx2i_gfxcon_lconbuf=malloc(16384*sizeof(u32));
//	jx2i_gfxcon_conbuf=(u32 *)malloc(32768*sizeof(u32));
//	jx2i_gfxcon_lconbuf=(u32 *)malloc(32768*sizeof(u32));

	jx2i_gfxcon_conbuf=(u32 *)malloc(131072*sizeof(u32));
	jx2i_gfxcon_lconbuf=(u32 *)malloc(131072*sizeof(u32));
	
//	for(i=0; i<40*25*4; i++)
//	for(i=0; i<40*25*8; i++)
//	for(i=0; i<80*25*8; i++)
	for(i=0; i<80*50*8; i++)
	{
//		jx2i_gfxcon_conbuf[i]=rand()*rand()*rand();
		jx2i_gfxcon_conbuf[i]=JX2I_GfxCon_Rng32();
	}
	jx2i_gfxcon_dirty=1;
	return(0);
}

int jx2i_gfxcon_clamp255(int v)
{
	if(v<  0)	return(0);
	if(v>255)	return(255);
	return(v);
}

int JX2I_GfxCon_PutPix200(int px, int py, int clrc)
{
	int fbxs;
	
	fbxs=btesh2_gfxcon_fbxs;

	if(jx2i_gfxcon_is2xcol)
	{
		if(px&1)
			return(0);
		px>>=1;
	}
	if(jx2i_gfxcon_is2xrow)
	{
		if(py&1)
			return(0);
		py>>=1;
	}

#if 0
	if(btesh2_gfxcon_swaprb)
	{
		clrc=(clrc&0xFF00FF00U)|
			((clrc<<16)&0x00FF0000U)|
			((clrc>>16)&0x000000FFU);
	}
#endif

	if(jx2i_gfxcon_is80col)
	{
		if(jx2i_gfxcon_is50row)
		{
			((u32 *)btesh2_gfxcon_framebuf)[(py*fbxs)+px]=clrc;
		}else
		{
			((u32 *)btesh2_gfxcon_framebuf)[((py*2+0)*fbxs)+px]=clrc;
			((u32 *)btesh2_gfxcon_framebuf)[((py*2+1)*fbxs)+px]=clrc;
		}
	}else
	{
		if(jx2i_gfxcon_is50row)
		{
			((u32 *)btesh2_gfxcon_framebuf)[(py*fbxs)+(px*2+0)]=clrc;
			((u32 *)btesh2_gfxcon_framebuf)[(py*fbxs)+(px*2+1)]=clrc;
		}else
		{
			((u32 *)btesh2_gfxcon_framebuf)[((py*2+0)*fbxs)+(px*2+0)]=clrc;
			((u32 *)btesh2_gfxcon_framebuf)[((py*2+1)*fbxs)+(px*2+0)]=clrc;
			((u32 *)btesh2_gfxcon_framebuf)[((py*2+0)*fbxs)+(px*2+1)]=clrc;
			((u32 *)btesh2_gfxcon_framebuf)[((py*2+1)*fbxs)+(px*2+1)]=clrc;
		}
	}
	return(0);
}

int JX2I_GfxCon_PutPix400(int px, int py, int clrc)
{
	int fbxs;
	
	fbxs=btesh2_gfxcon_fbxs;
//	((u32 *)btesh2_gfxcon_framebuf)[(py*320)+px]=clrc;

	if(jx2i_gfxcon_is2xcol)
	{
		if(px&1)
			return(0);
		px>>=1;
	}
	if(jx2i_gfxcon_is2xrow)
	{
		if(py&1)
			return(0);
		py>>=1;
	}

#if 0
	if(btesh2_gfxcon_swaprb)
	{
		clrc=(clrc&0xFF00FF00U)|
			((clrc<<16)&0x00FF0000U)|
			((clrc>>16)&0x000000FFU);
	}
#endif
	if(jx2i_gfxcon_is50row)
	{
		if(jx2i_gfxcon_is80col)
		{
			((u32 *)btesh2_gfxcon_framebuf)[((py/2)*fbxs)+px]=clrc;
		}else
		{
			((u32 *)btesh2_gfxcon_framebuf)[((py/2)*fbxs)+(px*2+0)]=clrc;
			((u32 *)btesh2_gfxcon_framebuf)[((py/2)*fbxs)+(px*2+1)]=clrc;
		}
	}else
	{
		if(jx2i_gfxcon_is80col)
		{
			((u32 *)btesh2_gfxcon_framebuf)[(py*fbxs)+px]=clrc;
		}else
		{
			((u32 *)btesh2_gfxcon_framebuf)[(py*fbxs)+(px*2+0)]=clrc;
			((u32 *)btesh2_gfxcon_framebuf)[(py*fbxs)+(px*2+1)]=clrc;
		}
	}
	return(0);
}

u32 JX2I_GfxCon_Yvu16ToRgb24(int yuv)
{
	int cy, cu, cv;
	int cu2, cv2;
	int cr0, cg0, cb0;
	u32 clrc;

	cy=(yuv>>10)&63;
	cu=(yuv>>0)&31;
	cv=(yuv>>5)&31;
	
	cy=(cy<<2)|(cy>>4);
	cu=cu<<3;
	cv=cv<<3;

	cu2=cu-128; cv2=cv-128;
	cr0=(256*cy        +359*cv2+128)>>8;
	cg0=(256*cy- 88*cu2-183*cv2+128)>>8;
	cb0=(256*cy+454*cu2        +128)>>8;
	if((cr0|cg0|cb0)&(~255))
	{	cr0=jx2i_gfxcon_clamp255(cr0);
		cg0=jx2i_gfxcon_clamp255(cg0);
		cb0=jx2i_gfxcon_clamp255(cb0);		}
	if(btesh2_gfxcon_swaprb)
		clrc=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
	else
		clrc=0xFF000000|(cb0<<16)|(cg0<<8)|cr0;
	return(clrc);
}

u32 JX2I_GfxCon_Rgb565ToRgb24(int rgb)
{
	int cr, cg, cb;
	u32 clrc;
	
	cr=(rgb>>11)&31; cr=(cr<<3)|(cr>>2);
	cg=(rgb>> 5)&63; cg=(cg<<2)|(cg>>4);
	cb=(rgb>> 0)&31; cb=(cb<<3)|(cb>>2);

	if(btesh2_gfxcon_swaprb)
		clrc=0xFF000000|(cr<<16)|(cg<<8)|cb;
	else
		clrc=0xFF000000|(cb<<16)|(cg<<8)|cr;
	return(clrc);
}

u32 JX2I_GfxCon_Rgb555ToRgb24(int rgb)
{
	int cr, cg, cb;
	u32 clrc;
	
	cr=(rgb>>10)&31; cr=(cr<<3)|(cr>>2);
	cg=(rgb>> 5)&31; cg=(cg<<3)|(cg>>2);
	cb=(rgb>> 0)&31; cb=(cb<<3)|(cb>>2);

	if(btesh2_gfxcon_swaprb)
		clrc=0xFF000000|(cr<<16)|(cg<<8)|cb;
	else
		clrc=0xFF000000|(cb<<16)|(cg<<8)|cr;
	return(clrc);
}

int JX2I_GfxCon_UpdateCellBM(int cx, int cy)
{
	static const u32 rgbitab[16]={
		0xFF000000,	0xFF0000AA, 0xFF00AA00,	0xFF00AAAA,
		0xFFAA0000,	0xFFAA00AA, 0xFFAAAA00,	0xFFAAAAAA,
		0xFF555555,	0xFF5555FF, 0xFF55FF55,	0xFF55FFFF,
		0xFFFF5555,	0xFFFF55FF, 0xFFFFFF55,	0xFFFFFFFF};

	u32 c0, c1, c2, c3;
	u32 c4, c5, c6, c7;
	u32 p0, p1;
	u32 px, px0, px1, px2, px3;
	int ncx;
	int i, j, k;

//	ncx=40;
//	if(jx2i_gfxcon_is80col)
//		ncx=80;
	ncx=jx2i_gfxcon_ncx;

	if(jx2i_gfxcon_isqtrcell)
	{
		c0=jx2i_gfxcon_conbuf[((cy*ncx+cx)*2+0)&131071];
		c1=jx2i_gfxcon_conbuf[((cy*ncx+cx)*2+1)&131071];
		c2=JX2I_GfxCon_Rng32();
		c3=JX2I_GfxCon_Rng32();
		c4=JX2I_GfxCon_Rng32();
		c5=JX2I_GfxCon_Rng32();
		c6=JX2I_GfxCon_Rng32();
		c7=JX2I_GfxCon_Rng32();
	}else
	if(jx2i_gfxcon_ishalfcell)
	{
		c0=jx2i_gfxcon_conbuf[((cy*ncx+cx)*4+0)&131071];
		c1=jx2i_gfxcon_conbuf[((cy*ncx+cx)*4+1)&131071];
		c2=jx2i_gfxcon_conbuf[((cy*ncx+cx)*4+2)&131071];
		c3=jx2i_gfxcon_conbuf[((cy*ncx+cx)*4+3)&131071];
		c4=JX2I_GfxCon_Rng32();
		c5=JX2I_GfxCon_Rng32();
		c6=JX2I_GfxCon_Rng32();
		c7=JX2I_GfxCon_Rng32();
	}else
	{
		c0=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+0)&131071];
		c1=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+1)&131071];
		c2=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+2)&131071];
		c3=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+3)&131071];

		c4=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+4)&131071];
		c5=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+5)&131071];
		c6=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+6)&131071];
		c7=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+7)&131071];
	}

	if((jx2i_gfxcon_isbmap==0x1) || (jx2i_gfxcon_isbmap==0x9))
	{
		for(j=0; j<4; j++)
		{
			switch(j)
			{
			case 0: p0=c0; p1=c1; break;
			case 1: p0=c2; p1=c3; break;
			case 2: p0=c4; p1=c5; break;
			case 3: p0=c6; p1=c7; break;
			}
		
			if(jx2i_gfxcon_isbmap&8)
			{
//				px0=JX2I_GfxCon_Rgb565ToRgb24((p0>> 0)&0xFFFF);
//				px1=JX2I_GfxCon_Rgb565ToRgb24((p0>>16)&0xFFFF);
//				px2=JX2I_GfxCon_Rgb565ToRgb24((p1>> 0)&0xFFFF);
//				px3=JX2I_GfxCon_Rgb565ToRgb24((p1>>16)&0xFFFF);

				px0=JX2I_GfxCon_Rgb555ToRgb24((p0>> 0)&0xFFFF);
				px1=JX2I_GfxCon_Rgb555ToRgb24((p0>>16)&0xFFFF);
				px2=JX2I_GfxCon_Rgb555ToRgb24((p1>> 0)&0xFFFF);
				px3=JX2I_GfxCon_Rgb555ToRgb24((p1>>16)&0xFFFF);
			}else
			{
				px0=JX2I_GfxCon_Yvu16ToRgb24((p0>> 0)&0xFFFF);
				px1=JX2I_GfxCon_Yvu16ToRgb24((p0>>16)&0xFFFF);
				px2=JX2I_GfxCon_Yvu16ToRgb24((p1>> 0)&0xFFFF);
				px3=JX2I_GfxCon_Yvu16ToRgb24((p1>>16)&0xFFFF);
			}

			JX2I_GfxCon_PutPix200(cx*8+0, cy*8+j*2+0, px0);
			JX2I_GfxCon_PutPix200(cx*8+1, cy*8+j*2+0, px0);
			JX2I_GfxCon_PutPix200(cx*8+0, cy*8+j*2+1, px0);
			JX2I_GfxCon_PutPix200(cx*8+1, cy*8+j*2+1, px0);
			JX2I_GfxCon_PutPix200(cx*8+2, cy*8+j*2+0, px1);
			JX2I_GfxCon_PutPix200(cx*8+3, cy*8+j*2+0, px1);
			JX2I_GfxCon_PutPix200(cx*8+2, cy*8+j*2+1, px1);
			JX2I_GfxCon_PutPix200(cx*8+3, cy*8+j*2+1, px1);
			JX2I_GfxCon_PutPix200(cx*8+4, cy*8+j*2+0, px2);
			JX2I_GfxCon_PutPix200(cx*8+5, cy*8+j*2+0, px2);
			JX2I_GfxCon_PutPix200(cx*8+4, cy*8+j*2+1, px2);
			JX2I_GfxCon_PutPix200(cx*8+5, cy*8+j*2+1, px2);
			JX2I_GfxCon_PutPix200(cx*8+6, cy*8+j*2+0, px3);
			JX2I_GfxCon_PutPix200(cx*8+7, cy*8+j*2+0, px3);
			JX2I_GfxCon_PutPix200(cx*8+6, cy*8+j*2+1, px3);
			JX2I_GfxCon_PutPix200(cx*8+7, cy*8+j*2+1, px3);
		}
		return(0);
	}

	if((jx2i_gfxcon_isbmap==0x2) || (jx2i_gfxcon_isbmap==0xA))
	{
		for(j=0; j<4; j++)
		{
			switch(j)
			{
			case 0: p0=c0; p1=c1; break;
			case 1: p0=c2; p1=c3; break;
			case 2: p0=c4; p1=c5; break;
			case 3: p0=c6; p1=c7; break;
			}

			px=(p0>> 0)&0xFFFF;
			if(!btesh2_gfxcon_swaprb)
				px=(px&0xAAAA)|((px<<2)&0x4444)|((px>>2)&0x1111);
			px0=rgbitab[(px>>12)&15];	px1=rgbitab[(px>> 8)&15];
			px2=rgbitab[(px>> 4)&15];	px3=rgbitab[(px>> 0)&15];
			JX2I_GfxCon_PutPix200(cx*8+0, cy*8+j*2+0, px0);
			JX2I_GfxCon_PutPix200(cx*8+1, cy*8+j*2+0, px1);
			JX2I_GfxCon_PutPix200(cx*8+0, cy*8+j*2+1, px2);
			JX2I_GfxCon_PutPix200(cx*8+1, cy*8+j*2+1, px3);

			px=(p0>>16)&0xFFFF;
			if(!btesh2_gfxcon_swaprb)
				px=(px&0xAAAA)|((px<<2)&0x4444)|((px>>2)&0x1111);
			px0=rgbitab[(px>>12)&15];	px1=rgbitab[(px>> 8)&15];
			px2=rgbitab[(px>> 4)&15];	px3=rgbitab[(px>> 0)&15];
			JX2I_GfxCon_PutPix200(cx*8+2, cy*8+j*2+0, px0);
			JX2I_GfxCon_PutPix200(cx*8+3, cy*8+j*2+0, px1);
			JX2I_GfxCon_PutPix200(cx*8+2, cy*8+j*2+1, px2);
			JX2I_GfxCon_PutPix200(cx*8+3, cy*8+j*2+1, px3);

			px=(p1>> 0)&0xFFFF;
			if(!btesh2_gfxcon_swaprb)
				px=(px&0xAAAA)|((px<<2)&0x4444)|((px>>2)&0x1111);
			px0=rgbitab[(px>>12)&15];	px1=rgbitab[(px>> 8)&15];
			px2=rgbitab[(px>> 4)&15];	px3=rgbitab[(px>> 0)&15];
			JX2I_GfxCon_PutPix200(cx*8+4, cy*8+j*2+0, px0);
			JX2I_GfxCon_PutPix200(cx*8+5, cy*8+j*2+0, px1);
			JX2I_GfxCon_PutPix200(cx*8+4, cy*8+j*2+1, px2);
			JX2I_GfxCon_PutPix200(cx*8+5, cy*8+j*2+1, px3);

			px=(p1>>16)&0xFFFF;
			if(!btesh2_gfxcon_swaprb)
				px=(px&0xAAAA)|((px<<2)&0x4444)|((px>>2)&0x1111);
			px0=rgbitab[(px>>12)&15];	px1=rgbitab[(px>> 8)&15];
			px2=rgbitab[(px>> 4)&15];	px3=rgbitab[(px>> 0)&15];
			JX2I_GfxCon_PutPix200(cx*8+6, cy*8+j*2+0, px0);
			JX2I_GfxCon_PutPix200(cx*8+7, cy*8+j*2+0, px1);
			JX2I_GfxCon_PutPix200(cx*8+6, cy*8+j*2+1, px2);
			JX2I_GfxCon_PutPix200(cx*8+7, cy*8+j*2+1, px3);
		}
		return(0);
	}

	return(0);
}

int JX2I_GfxCon_UpdateCell(int cx, int cy)
{
	static byte pixv_c2[4]={0, 85, 170, 255};
	static byte pixv_c3[8]={0, 36, 73, 109, 146, 182, 218, 255};
	static u32 pixv6[64];
	static u32 pixv9[512];
	static u32 clrt4[4];
	static byte clrt4_y[4];
	static byte clrt4_u[4];
	static byte clrt4_v[4];
	static int pixv_init=0;

	u32 c0, c1, c2, c3;
	u32 c4, c5, c6, c7;
	int c2ya, c2yb, c2ua, c2ub, c2va, c2vb;
	int cr0, cg0, cb0, cr1, cg1, cb1;
	int cr2, cg2, cb2, cr3, cg3, cb3;
	int cy0, cu0, cv0;
	int cy1, cu1, cv1;
	int cy2, cu2, cv2;
	int cy3, cu3, cv3;
	u32 clra, clrb, clrc;
	u64 fontbits, pixbits;
	u16 pxubits, pxvbits;
	u32 px2;
	int celbits, ncx, ncy;
	int clra6, clrb6, clra9, clrb9;
	int px, py, qx, qy, by, bx;
	int i, j, k;
	
	if(!pixv_init)
	{
		pixv_init=1;
		for(i=0; i<64; i++)
		{
			clra=(255<<24)|
				(pixv_c2[(i>>4)&3]<<0)|
				(pixv_c2[(i>>2)&3]<<8)|
				(pixv_c2[(i>>0)&3]<<16);

			if(btesh2_gfxcon_swaprb)
			{
				clra=(clra&0xFF00FF00U)|
					((clra<<16)&0x00FF0000U)|
					((clra>>16)&0x000000FFU);
			}

			pixv6[i]=clra;
		}
		for(i=0; i<512; i++)
		{
			clra=(255<<24)|
				(pixv_c3[(i>>6)&7]<<0)|
				(pixv_c3[(i>>3)&7]<<8)|
				(pixv_c3[(i>>0)&7]<<16);

			if(btesh2_gfxcon_swaprb)
			{
				clra=(clra&0xFF00FF00U)|
					((clra<<16)&0x00FF0000U)|
					((clra>>16)&0x000000FFU);
			}

			pixv9[i]=clra;
		}
	}

	if(!btesh2_gfxcon_framebuf)
		return(0);
	
//	if(jx2i_gfxcon_isbmap)
	if(jx2i_gfxcon_isbmap&7)
	{
		return(JX2I_GfxCon_UpdateCellBM(cx, cy));
	}

//	ncx=40;
//	if(jx2i_gfxcon_is80col)
//		ncx=80;
	ncx=jx2i_gfxcon_ncx;

	if(jx2i_gfxcon_ishalfcell)
	{
		c0=jx2i_gfxcon_conbuf[((cy*ncx+cx)*4+0)&32767];
		c1=jx2i_gfxcon_conbuf[((cy*ncx+cx)*4+1)&32767];
		c2=jx2i_gfxcon_conbuf[((cy*ncx+cx)*4+2)&32767];
		c3=jx2i_gfxcon_conbuf[((cy*ncx+cx)*4+3)&32767];
		c4=JX2I_GfxCon_Rng32();
		c5=JX2I_GfxCon_Rng32();
		c6=JX2I_GfxCon_Rng32();
		c7=JX2I_GfxCon_Rng32();
	}else
	{
		c0=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+0)&32767];
		c1=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+1)&32767];
		c2=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+2)&32767];
		c3=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+3)&32767];

		c4=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+4)&32767];
		c5=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+5)&32767];
		c6=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+6)&32767];
		c7=jx2i_gfxcon_conbuf[((cy*ncx+cx)*8+7)&32767];
	}
	
	clra6=(c0>>16)&63;
	clrb6=(c0>>22)&63;
	clra9=(c0>>10)&511;
	clrb9=(c0>>19)&511;
	
	switch((c0>>28)&3)
	{
	case 0:
	case 1:
		clra=pixv6[clra6];
		clrb=pixv6[clrb6];
		break;
	case 2:
	case 3:
		clra=pixv9[clra9];
		clrb=pixv9[clrb9];
		break;
	}

#if 0
	if(((c0>>30)&3)==2)
	{		
		c2yb=(c0>>22)&63;
		c2ya=(c0>>16)&63;
		c2ub=(c0>>12)&15;
		c2ua=(c0>> 8)&15;
		c2vb=(c0>> 4)&15;
		c2va=(c0>> 0)&15;
		
		c2yb=(c2yb<<2)|(c2yb>>6);
		c2ya=(c2ya<<2)|(c2ya>>6);
		c2ub<<=4;		c2vb<<=4;
		c2ua<<=4;		c2va<<=4;

#if 0		
		cg0=c2yb<<2;
		cb0=cg0+(16*c2ub-128)*2;
		cr0=cg0+(16*c2vb-128)*2;

		cg1=c2ya<<2;
		cb1=cg1+(16*c2ua-128)*2;
		cr1=cg1+(16*c2va-128)*2;
		
		cr0=jx2i_gfxcon_clamp255(cr0);
		cg0=jx2i_gfxcon_clamp255(cg0);
		cb0=jx2i_gfxcon_clamp255(cb0);

		cr1=jx2i_gfxcon_clamp255(cr1);
		cg1=jx2i_gfxcon_clamp255(cg1);
		cb1=jx2i_gfxcon_clamp255(cb1);
		
		clra=0xFF000000|(cb1<<16)|(cg1<<8)|(cr1<<0);
		clrb=0xFF000000|(cb0<<16)|(cg0<<8)|(cr0<<0);
#endif

		pxubits=c1>>16;
		pxvbits=c1;
		
		if(((c0>>28)&3)==1)
		{
			pxubits=(c1>>16);
			cr0=(pxubits>>10)&31;
			cg0=(pxubits>> 5)&31;
			cb0=(pxubits>> 0)&31;
			cr0=(cr0<<3)|(cr0>>2);
			cg0=(cg0<<3)|(cg0>>2);
			cb0=(cb0<<3)|(cb0>>2);

			pxvbits=(c1    );
			cr1=(pxvbits>>10)&31;
			cg1=(pxvbits>> 5)&31;
			cb1=(pxvbits>> 0)&31;
			cr1=(cr1<<3)|(cr1>>2);
			cg1=(cg1<<3)|(cg1>>2);
			cb1=(cb1<<3)|(cb1>>2);

			if(btesh2_gfxcon_swaprb)
			{
				clrb=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
				clra=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
			}
			else
			{
				clrb=0xFF000000|(cb0<<16)|(cg0<<8)|cr0;
				clra=0xFF000000|(cb1<<16)|(cg1<<8)|cr1;
			}
		}
	}
#endif

#if 1
	if(((c0>>30)&3)==2)
	{
#if 1
		pxubits=c0>>15;
		pxvbits=c0>>0;

		cy0=(pxubits>>10)&31;
		cv0=(pxubits>> 5)&31;	
		cu0=(pxubits    )&31;

		cy1=(pxvbits>>10)&31;
		cv1=(pxvbits>> 5)&31;
		cu1=(pxvbits    )&31;

		if(jx2i_gfxcon_isbmap&8)
		{
			cr0=(cy0<<3)|(cy0>>2);
			cg0=(cv0<<3)|(cv0>>2);
			cb0=(cu0<<3)|(cu0>>2);

			cr1=(cy1<<3)|(cy1>>2);
			cg1=(cv1<<3)|(cv1>>2);
			cb1=(cu1<<3)|(cu1>>2);
			
//			cr1=127;
//			cg1=127;
//			cb1=127;
		}else
		{
			cy0=(cy0<<3)|(cy0>>2);
			cu0=(cu0<<3)|(7-(cu0>>2));
			cv0=(cv0<<3)|(7-(cv0>>2));

			cu2=cu0-128; cv2=cv0-128;
			cr0=(256*cy0        +359*cv2+128)>>8;
			cg0=(256*cy0- 88*cu2-183*cv2+128)>>8;
			cb0=(256*cy0+454*cu2        +128)>>8;


			cy1=(cy1<<3)|(cy1>>2);
			cu1=(cu1<<3)|(7-(cu1>>2));
			cv1=(cv1<<3)|(7-(cv1>>2));

			cu2=cu1-128; cv2=cv1-128;
			cr1=(256*cy1        +359*cv2+128)>>8;
			cg1=(256*cy1- 88*cu2-183*cv2+128)>>8;
			cb1=(256*cy1+454*cu2        +128)>>8;
		}

		if((cr0|cg0|cb0)&(~255))
		{
			cr0=jx2i_gfxcon_clamp255(cr0);
			cg0=jx2i_gfxcon_clamp255(cg0);
			cb0=jx2i_gfxcon_clamp255(cb0);
		}

		if((cr1|cg1|cb1)&(~255))
		{
			cr1=jx2i_gfxcon_clamp255(cr1);
			cg1=jx2i_gfxcon_clamp255(cg1);
			cb1=jx2i_gfxcon_clamp255(cb1);
		}

//		clrb=0xFF000000|(cb0<<16)|(cg0<<8)|cr0;
//		clra=0xFF000000|(cb1<<16)|(cg1<<8)|cr1;

		clrb=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
		clra=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
#endif
	}
#endif

	
	switch((c0>>7)&7)
	{
	case 0:	
	case 1:
		fontbits=btesh2_gfxcon_glyphs[c0&255];
		break;
	case 2:
		fontbits=gfxfont[0x80|(c0&127)];
		break;
	case 3:
		fontbits=gfxfont[0x00|(c0&127)];
		break;
	default:
		fontbits=btesh2_gfxcon_glyphs[c0&255];
		break;
	}
	
	fontbits=jx2i_gfxcon_fontram[c0&1023];

	celbits=c0&65535;
	
//	pixbits=(((u64)c3)<<21)|c2;
	pixbits=(((u64)c3)<<32)|c2;

	if(((c0>>30)&3)==0)
	{		
		for(py=0; py<8; py++)
			for(px=0; px<8; px++)
		{
			clrc=((fontbits>>((7-py)*8+(7-px)))&1)?clra:clrb;
//			((u32 *)btesh2_gfxcon_framebuf)[((cy*8+py)*320)+(cx*8+px)]=clrc;
//			((u32 *)btesh2_gfxcon_framebuf)[((cy*8+py)*320)+(cx*8+px)]=clrc;
			JX2I_GfxCon_PutPix200(cx*8+px, cy*8+py, clrc);
		}
	}else
		if(((c0>>30)&3)==1)
	{		
		for(py=0; py<4; py++)
			for(px=0; px<4; px++)
		{
			clrc=((celbits>>((3-py)*4+(3-px)))&1)?clra:clrb;
//			((u32 *)btesh2_gfxcon_framebuf)[
//				((cy*8+py*2+0)*320)+(cx*8+px*2+0)]=clrc;
//			((u32 *)btesh2_gfxcon_framebuf)[
//				((cy*8+py*2+0)*320)+(cx*8+px*2+1)]=clrc;
//			((u32 *)btesh2_gfxcon_framebuf)[
//				((cy*8+py*2+1)*320)+(cx*8+px*2+0)]=clrc;
//			((u32 *)btesh2_gfxcon_framebuf)[
//				((cy*8+py*2+1)*320)+(cx*8+px*2+1)]=clrc;
			JX2I_GfxCon_PutPix200(cx*8+px*2+0, cy*8+py*2+0, clrc);
			JX2I_GfxCon_PutPix200(cx*8+px*2+1, cy*8+py*2+0, clrc);
			JX2I_GfxCon_PutPix200(cx*8+px*2+0, cy*8+py*2+1, clrc);
			JX2I_GfxCon_PutPix200(cx*8+px*2+1, cy*8+py*2+1, clrc);
		}
	}else
		if(((c0>>30)&3)==2)
	{
//		if(docurblnk)
//		{
//			if(jx2i_gfxcon_curms&512)
//				pixbits|=0x000000000000FF00ULL;
//		}

		if(((c1>>30)&3)==0)
//		if(1)
		{
//			clra=0xFF7F7F7F;
//			clrb=0xFF000000;

			for(py=0; py<8; py++)
				for(px=0; px<8; px++)
			{
				clrc=((pixbits>>((7-py)*8+(7-px)))&1)?clra:clrb;
				JX2I_GfxCon_PutPix200(cx*8+px, cy*8+py, clrc);
			}
		}else
#if 0
			if(((c0>>28)&3)==0)
		{
			for(py=0; py<8; py++)
				for(px=0; px<8; px++)
			{
				k=(3-(py>>1))*4+(3-(px>>1));
				cy0=((pixbits>>((7-py)*8+(7-px)))&1)?c2ya:c2yb;
				cu0=((pxubits>>k)&1)?c2ua:c2ub;
				cv0=((pxvbits>>k)&1)?c2va:c2vb;
	//			cg0=cy0;
				cg0=cy0-((cu0+cv0-256)>>1);
				cb0=cg0+(cu0-128)*2;
				cr0=cg0+(cv0-128)*2;
				if(cr0|cg0|cb0)
				{
					cr0=jx2i_gfxcon_clamp255(cr0);
					cg0=jx2i_gfxcon_clamp255(cg0);
					cb0=jx2i_gfxcon_clamp255(cb0);
				}

				if(btesh2_gfxcon_swaprb)
					clrc=0xFF000000|(cr0<<16)|(cg0<<8)|(cb0<<0);
				else
					clrc=0xFF000000|(cb0<<16)|(cg0<<8)|(cr0<<0);

	//			clrc=((pixbits>>((7-py)*8+(7-px)))&1)?clra:clrb;
//				((u32 *)btesh2_gfxcon_framebuf)[((cy*8+py)*320)+(cx*8+px)]=clrc;
				JX2I_GfxCon_PutPix200(cx*8+px, cy*8+py, clrc);
			}
		}else
#endif
//			if(((c0>>28)&3)==2)
			if(((c1>>30)&3)==2)
		{
			for(qy=0; qy<2; qy++)
				for(qx=0; qx<2; qx++)
			{
				switch(qy*2+qx)
				{
				case 0:
					k=c3; px2=c7;
					pxubits=c3>>24;
					pxvbits=c2>>24;
					break;
				case 1:
					k=c2; px2=c6;
					pxubits=c3>>16;
					pxvbits=c2>>16;
					break;
				case 2:
					k=c1; px2=c5;
					pxubits=c3>>8;
					pxvbits=c2>>8;
					break;
				case 3:
					k=c0; px2=c4;
					pxubits=c3>>0;
					pxvbits=c2>>0;
					break;
				}


//				pxubits=(k>>14);

//				cy0=(pxubits>>11)&31;
//				cv0=(pxubits>> 5)&31;
//				cu0=(pxubits    )&31;
				
				cy0=(c1>>16)&255;
				cu0=(c1>> 8)&255;
				cv0=(c1>> 0)&255;

				cy1=(c0>>16)&255;
				cu1=(c0>> 8)&255;
				cv1=(c0>> 0)&255;

				cu3=(cu0+cu1)>>1;
				cv3=(cv0+cv1)>>1;

//				cu2=(cu0-16)<<1;	cv2=(cv0-16)<<1;
//				cu2=(cu0-128)<<1;	cv2=(cv0-128)<<1;
				cu2=(cu3-128)<<1;	cv2=(cv3-128)<<1;
				cg0=cy0-((cu2+cv2)>>1);
				cb0=cg0+cu2;	cr0=cg0+cv2;

//				pxvbits=(k<<1);
//				cy0=(pxvbits>>11)&31;
//				cv0=(pxvbits>> 5)&31;
//				cu0=(pxvbits    )&31;


//				cu2=(cu0-16)<<1;	cv2=(cv0-16)<<1;
//				cu2=(cu1-128)<<1;	cv2=(cv1-128)<<1;
				cu2=(cu3-128)<<1;	cv2=(cv3-128)<<1;
				cg1=cy1-((cu2+cv2)>>1);
				cb1=cg1+cu2;	cr1=cg1+cv2;

//				cr0=(cr0<<3)|(cr0>>2);
//				cg0=(cg0<<3)|(cg0>>2);
//				cb0=(cb0<<3)|(cb0>>2);
				if(cr0|cg0|cb0)
				{
					cr0=jx2i_gfxcon_clamp255(cr0);
					cg0=jx2i_gfxcon_clamp255(cg0);
					cb0=jx2i_gfxcon_clamp255(cb0);
				}

//				cr1=(cr1<<3)|(cr1>>2);
//				cg1=(cg1<<3)|(cg1>>2);
//				cb1=(cb1<<3)|(cb1>>2);
				if(cr1|cg1|cb1)
				{
					cr1=jx2i_gfxcon_clamp255(cr1);
					cg1=jx2i_gfxcon_clamp255(cg1);
					cb1=jx2i_gfxcon_clamp255(cb1);
				}

				cr2=(11*cr0+ 5*cr1)>>4;
				cg2=(11*cg0+ 5*cg1)>>4;
				cb2=(11*cb0+ 5*cb1)>>4;
				cr3=( 5*cr0+11*cr1)>>4;
				cg3=( 5*cg0+11*cg1)>>4;
				cb3=( 5*cb0+11*cb1)>>4;

				if(btesh2_gfxcon_swaprb)
				{
					clrt4[0]=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
					clrt4[1]=0xFF000000|(cr2<<16)|(cg2<<8)|cb2;
					clrt4[2]=0xFF000000|(cr3<<16)|(cg3<<8)|cb3;
					clrt4[3]=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
				}else
				{
					clrt4[0]=0xFF000000|(cb0<<16)|(cg0<<8)|cr0;
					clrt4[1]=0xFF000000|(cb2<<16)|(cg2<<8)|cr2;
					clrt4[2]=0xFF000000|(cb3<<16)|(cg3<<8)|cr3;
					clrt4[3]=0xFF000000|(cb1<<16)|(cg1<<8)|cr1;
				}

				clrt4_y[0]=cy0;
				clrt4_y[1]=(11*cy0+ 5*cy1)>>4;
				clrt4_y[2]=( 5*cy0+11*cy1)>>4;
				clrt4_y[3]=cy1;

				clrt4_u[0]=cu0;
				clrt4_u[1]=(11*cu0+ 5*cu1)>>4;
				clrt4_u[2]=( 5*cu0+11*cu1)>>4;
				clrt4_u[3]=cu1;

				clrt4_v[0]=cv0;
				clrt4_v[1]=(11*cv0+ 5*cv1)>>4;
				clrt4_v[2]=( 5*cv0+11*cv1)>>4;
				clrt4_v[3]=cv1;

				by=cy*8+qy*4;
				bx=cx*8+qx*4;

				for(py=0; py<4; py++)
					for(px=0; px<4; px++)
				{
					i=15-(4*py+px);
					j=(px2>>(2*i))&3;
					clrc=clrt4[j];
					cy3=clrt4_y[j];

					i=3-(2*(py/2)+(px/2));
					j=(pxubits>>(2*i))&3;
					k=(pxvbits>>(2*i))&3;
					cu3=clrt4_u[j];
					cv3=clrt4_v[k];

//					cu3=clrt4_u[3-j];
//					cv3=clrt4_v[3-k];

					cu2=(cu3-128)<<1;	cv2=(cv3-128)<<1;
					cg1=cy3-((cu2+cv2)>>1);
					cb1=cg1+cu2;	cr1=cg1+cv2;

					if(cr1|cg1|cb1)
					{
						cr1=jx2i_gfxcon_clamp255(cr1);
						cg1=jx2i_gfxcon_clamp255(cg1);
						cb1=jx2i_gfxcon_clamp255(cb1);
					}

//					clrc=0xFF000000|(cb1<<16)|(cg1<<8)|cr1;
//					((u32 *)btesh2_gfxcon_framebuf)[
//						((by+py)*320)+(bx+px)]=clrc;
					JX2I_GfxCon_PutPix200(bx+px, by+py, clrc);
				}
			}
		}else
		{
			for(py=0; py<8; py++)
				for(px=0; px<8; px++)
			{
				clrc=((pixbits>>((7-py)*8+(7-px)))&1)?clra:clrb;
//				((u32 *)btesh2_gfxcon_framebuf)[((cy*8+py)*320)+(cx*8+px)]=clrc;
				JX2I_GfxCon_PutPix200(cx*8+px, cy*8+py, clrc);
			}
		}
	}else
#if 0
		if(((c0>>30)&3)==3)
	{
		for(py=0; py<4; py++)
			for(px=0; px<4; px++)
		{
			switch(px)
			{
			case 0: k=c3; break;
			case 1: k=c2; break;
			case 2: k=c1; break;
			case 3: k=c0; break;
			}
		
			clrc=pixv6[(k>>((3-py)*6))&63];

//			clrc=((celbits>>((3-py)*4+(3-px)))&1)?clra:clrb;
//			((u32 *)btesh2_gfxcon_framebuf)[
//				((cy*8+py*2+0)*320)+(cx*8+px*2+0)]=clrc;
//			((u32 *)btesh2_gfxcon_framebuf)[
//				((cy*8+py*2+0)*320)+(cx*8+px*2+1)]=clrc;
//			((u32 *)btesh2_gfxcon_framebuf)[
//				((cy*8+py*2+1)*320)+(cx*8+px*2+0)]=clrc;
//			((u32 *)btesh2_gfxcon_framebuf)[
//				((cy*8+py*2+1)*320)+(cx*8+px*2+1)]=clrc;

			JX2I_GfxCon_PutPix200(cx*8+px*2+0, cy*8+py*2+0, clrc);
			JX2I_GfxCon_PutPix200(cx*8+px*2+1, cy*8+py*2+0, clrc);
			JX2I_GfxCon_PutPix200(cx*8+px*2+0, cy*8+py*2+1, clrc);
			JX2I_GfxCon_PutPix200(cx*8+px*2+1, cy*8+py*2+1, clrc);

		}
	}
#endif

	if((((c0>>30)&3)==3) && (((c1>>30)&3)==3))
	{
		for(qy=0; qy<2; qy++)
			for(qx=0; qx<2; qx++)
		{
			switch(qy*2+qx)
			{
//			case 0: k=c0; px2=c4; break;
//			case 1: k=c1; px2=c5; break;
//			case 2: k=c2; px2=c6; break;
//			case 3: k=c3; px2=c7; break;

			case 0: k=c3; px2=c7; break;
			case 1: k=c2; px2=c6; break;
			case 2: k=c1; px2=c5; break;
			case 3: k=c0; px2=c4; break;
			}

			if(jx2i_gfxcon_isbmap&8)
			{
				pxubits=(u16)(k>>15);
				pxvbits=(u16)(k    );

				cr0=(pxubits>>10)&31;
				cg0=(pxubits>> 5)&31;
				cb0=(pxubits    )&31;

				cr1=(pxvbits>>10)&31;
				cg1=(pxvbits>> 5)&31;
				cb1=(pxvbits    )&31;

				cr0=(cr0<<3)|(cr0>>2);
				cg0=(cg0<<3)|(cg0>>2);
				cb0=(cb0<<3)|(cb0>>2);

				cr1=(cr1<<3)|(cr1>>2);
				cg1=(cg1<<3)|(cg1>>2);
				cb1=(cb1<<3)|(cb1>>2);

				cr2=(11*cr0+ 5*cr1)>>4;
				cg2=(11*cg0+ 5*cg1)>>4;
				cb2=(11*cb0+ 5*cb1)>>4;
				cr3=( 5*cr0+11*cr1)>>4;
				cg3=( 5*cg0+11*cg1)>>4;
				cb3=( 5*cb0+11*cb1)>>4;

				if(btesh2_gfxcon_swaprb)
				{
					clrt4[0]=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
					clrt4[1]=0xFF000000|(cr2<<16)|(cg2<<8)|cb2;
					clrt4[2]=0xFF000000|(cr3<<16)|(cg3<<8)|cb3;
					clrt4[3]=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
				}else
				{
					clrt4[0]=0xFF000000|(cb0<<16)|(cg0<<8)|cr0;
					clrt4[1]=0xFF000000|(cb2<<16)|(cg2<<8)|cr2;
					clrt4[2]=0xFF000000|(cb3<<16)|(cg3<<8)|cr3;
					clrt4[3]=0xFF000000|(cb1<<16)|(cg1<<8)|cr1;
				}

				by=cy*8+qy*4;
				bx=cx*8+qx*4;

				for(py=0; py<4; py++)
					for(px=0; px<4; px++)
				{
					i=15-(4*py+px);
					j=(px2>>(2*i))&3;
					clrc=clrt4[j];
					JX2I_GfxCon_PutPix200(bx+px, by+py, clrc);
				}
			}else
			{
				pxubits=(u16)(k>>14);
				pxvbits=(u16)(k<< 1);
	//			if(pxubits>pxvbits)
				if((pxubits&0xFC00)>(pxvbits&0xFC00))
				{

					cy0=(pxubits>>11)&31;
					cv0=(pxubits>> 5)&30;	
					cu0=(pxubits    )&30;

					cy1=(pxvbits>>11)&31;
					cv1=(pxvbits>> 5)&30;
					cu1=(pxvbits    )&30;

					cy0=(cy0<<3)|(cy0>>2);
					cy1=(cy1<<3)|(cy1>>2);
	//				cu0=(cu0<<3);	cv0=(cv0<<3);
	//				cu1=(cu1<<3);	cv1=(cv1<<3);

					cu0=(cu0<<3)|(7-(cu0>>2));
					cv0=(cv0<<3)|(7-(cv0>>2));

					cu1=(cu1<<3)|(7-(cu1>>2));
					cv1=(cv1<<3)|(7-(cv1>>2));

					clrt4_y[0]=cy0;
					clrt4_y[1]=(11*cy0+ 5*cy1)>>4;
					clrt4_y[2]=( 5*cy0+11*cy1)>>4;
					clrt4_y[3]=cy1;

					by=cy*8+qy*4;
					bx=cx*8+qx*4;

					for(py=0; py<4; py++)
						for(px=0; px<4; px++)
					{
		//				i=((3-py)*4)+(3-px);
						i=15-(4*py+px);
						j=(px2>>(2*i))&3;

						cy3=clrt4_y[j];
	//					cu3=(pxubits&32)?((py&2)?cu0:cu1):((px&2)?cu0:cu1);
	//					cv3=(pxvbits&32)?((py&2)?cv0:cv1):((px&2)?cv0:cv1);

						cu3=(pxubits&32)?((py&2)?cu0:cu1):((px&2)?cu0:cu1);
						cv3=(pxvbits&32)?((py&2)?cv0:cv1):((px&2)?cv0:cv1);
						cu2=cu3-128; cv2=cv3-128;
						cr0=(256*cy3        +359*cv2+128)>>8;
						cg0=(256*cy3- 88*cu2-183*cv2+128)>>8;
						cb0=(256*cy3+454*cu2        +128)>>8;
		//			cu2=(cu0-16)<<1;	cv2=(cv0-16)<<1;
		//			cg0=cy0-((cu2+cv2)>>1);
		//			cb0=cg0+cu2;	cr0=cg0+cv2;
						if((cr0|cg0|cb0)&(~255))
						{	cr0=jx2i_gfxcon_clamp255(cr0);
							cg0=jx2i_gfxcon_clamp255(cg0);
							cb0=jx2i_gfxcon_clamp255(cb0);		}
						if(btesh2_gfxcon_swaprb)
							clrc=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
						else
							clrc=0xFF000000|(cb0<<16)|(cg0<<8)|cr0;
	//					clrc=0;

	//					((u32 *)btesh2_gfxcon_framebuf)[
	//						((by+py)*320)+(bx+px)]=clrc;
						JX2I_GfxCon_PutPix200(bx+px, by+py, clrc);
					}

				}else
				{
					cy0=(pxubits>>11)&31;
					cv0=(pxubits>> 5)&31;	
					cu0=(pxubits    )&31;
		//			cu2=(cu0-16)<<1;	cv2=(cv0-16)<<1;
		//			cg0=cy0-((cu2+cv2)>>1);
		//			cb0=cg0+cu2;	cr0=cg0+cv2;

					cy0=(cy0<<3)|(cy0>>2);
	//				cu0=(cu0<<3);	cv0=(cv0<<3);
					cu0=(cu0<<3)|(7-(cu0>>2));
					cv0=(cv0<<3)|(7-(cv0>>2));

					cu2=cu0-128; cv2=cv0-128;
					cr0=(256*cy0        +359*cv2+128)>>8;
					cg0=(256*cy0- 88*cu2-183*cv2+128)>>8;
					cb0=(256*cy0+454*cu2        +128)>>8;

					cy0=(pxvbits>>11)&31;
					cv0=(pxvbits>> 5)&31;
					cu0=(pxvbits    )&31;
		//			cu2=(cu0-16)<<1;	cv2=(cv0-16)<<1;
		//			cg1=cy0-((cu2+cv2)>>1);
		//			cb1=cg1+cu2;	cr1=cg1+cv2;

					cy0=(cy0<<3)|(cy0>>2);
	//				cu0=(cu0<<3);	cv0=(cv0<<3);
					cu0=(cu0<<3)|(7-(cu0>>2));
					cv0=(cv0<<3)|(7-(cv0>>2));

					cu2=cu0-128; cv2=cv0-128;
					cr1=(256*cy0        +359*cv2+128)>>8;
					cg1=(256*cy0- 88*cu2-183*cv2+128)>>8;
					cb1=(256*cy0+454*cu2        +128)>>8;

		//			cr0=(cr0<<3)|(cr0>>2);
		//			cg0=(cg0<<3)|(cg0>>2);
		//			cb0=(cb0<<3)|(cb0>>2);
	//				if(cr0|cg0|cb0)
					if((cr0|cg0|cb0)&(~255))
					{
						cr0=jx2i_gfxcon_clamp255(cr0);
						cg0=jx2i_gfxcon_clamp255(cg0);
						cb0=jx2i_gfxcon_clamp255(cb0);
					}

		//			cr1=(cr1<<3)|(cr1>>2);
		//			cg1=(cg1<<3)|(cg1>>2);
		//			cb1=(cb1<<3)|(cb1>>2);
	//				if(cr1|cg1|cb1)
					if((cr1|cg1|cb1)&(~255))
					{
						cr1=jx2i_gfxcon_clamp255(cr1);
						cg1=jx2i_gfxcon_clamp255(cg1);
						cb1=jx2i_gfxcon_clamp255(cb1);
					}

		#if 0
					pxubits=(k>>15);
					cr0=(pxubits>>10)&31;
					cg0=(pxubits>> 5)&31;
					cb0=(pxubits>> 0)&31;
					cr0=(cr0<<3)|(cr0>>2);
					cg0=(cg0<<3)|(cg0>>2);
					cb0=(cb0<<3)|(cb0>>2);
		//			clrb=0xFF000000|(cb0<<16)|(cg0<<8)|cr0;

					pxvbits=(k    );
					cr1=(pxvbits>>10)&31;
					cg1=(pxvbits>> 5)&31;
					cb1=(pxvbits>> 0)&31;
					cr1=(cr1<<3)|(cr1>>2);
					cg1=(cg1<<3)|(cg1>>2);
					cb1=(cb1<<3)|(cb1>>2);
		//			clra=0xFF000000|(cb1<<16)|(cg1<<8)|cr1;
		#endif

					cr2=(11*cr0+ 5*cr1)>>4;
					cg2=(11*cg0+ 5*cg1)>>4;
					cb2=(11*cb0+ 5*cb1)>>4;
					cr3=( 5*cr0+11*cr1)>>4;
					cg3=( 5*cg0+11*cg1)>>4;
					cb3=( 5*cb0+11*cb1)>>4;

					if(btesh2_gfxcon_swaprb)
					{
						clrt4[0]=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
						clrt4[1]=0xFF000000|(cr2<<16)|(cg2<<8)|cb2;
						clrt4[2]=0xFF000000|(cr3<<16)|(cg3<<8)|cb3;
						clrt4[3]=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
					}else
					{
						clrt4[0]=0xFF000000|(cb0<<16)|(cg0<<8)|cr0;
						clrt4[1]=0xFF000000|(cb2<<16)|(cg2<<8)|cr2;
						clrt4[2]=0xFF000000|(cb3<<16)|(cg3<<8)|cr3;
						clrt4[3]=0xFF000000|(cb1<<16)|(cg1<<8)|cr1;
					}

					by=cy*8+qy*4;
					bx=cx*8+qx*4;

					for(py=0; py<4; py++)
						for(px=0; px<4; px++)
					{
		//				i=((3-py)*4)+(3-px);
						i=15-(4*py+px);
						j=(px2>>(2*i))&3;
		//				clrc=(j&2)?clra:clrb;
						clrc=clrt4[j];

	//					((u32 *)btesh2_gfxcon_framebuf)[
	//						((by+py)*320)+(bx+px)]=clrc;
						JX2I_GfxCon_PutPix200(bx+px, by+py, clrc);
					}
				}
			}
		}
	}


	if((((c0>>30)&3)==3) && (((c1>>30)&3)==2))
	{
		for(qy=0; qy<4; qy++)
			for(qx=0; qx<2; qx++)
		{
			switch((qy/2)*2+qx)
			{
			case 0: k=c3; px2=c7; j=c5>>14; break;
			case 1: k=c2; px2=c7; j=c5>> 0; break;
			case 2: k=c1; px2=c6; j=c4>>14; break;
			case 3: k=c0; px2=c6; j=c4>> 0; break;
			}

//			pxubits=(u16)(k>>14);
//			pxvbits=(u16)(k<< 1);

			if(qy&1)
			{
				pxubits=(u16)(k<< 1);
				pxvbits=pxubits;
			}else
			{
				pxubits=(u16)(k>>14);
				pxvbits=pxubits;
				j>>=7;
			}

			j=(j&127);

			cy0=(pxubits>>11)&31;
			cv0=(pxubits>> 5)&31;	
			cu0=(pxubits    )&31;

			cy0=(cy0<<3)|(cy0>>2);
			cu0=(cu0<<3)|(7-(cu0>>2));
			cv0=(cv0<<3)|(7-(cv0>>2));

			cy0-=j;

			cu2=cu0-128; cv2=cv0-128;
			cr0=(256*cy0        +359*cv2+128)>>8;
			cg0=(256*cy0- 88*cu2-183*cv2+128)>>8;
			cb0=(256*cy0+454*cu2        +128)>>8;

			cy0=(pxvbits>>11)&31;
			cv0=(pxvbits>> 5)&31;
			cu0=(pxvbits    )&31;

			cy0=(cy0<<3)|(cy0>>2);
			cu0=(cu0<<3)|(7-(cu0>>2));
			cv0=(cv0<<3)|(7-(cv0>>2));

			cy0+=j;

			cu2=cu0-128; cv2=cv0-128;
			cr1=(256*cy0        +359*cv2+128)>>8;
			cg1=(256*cy0- 88*cu2-183*cv2+128)>>8;
			cb1=(256*cy0+454*cu2        +128)>>8;

			if((cr0|cg0|cb0)&(~255))
			{
				cr0=jx2i_gfxcon_clamp255(cr0);
				cg0=jx2i_gfxcon_clamp255(cg0);
				cb0=jx2i_gfxcon_clamp255(cb0);
			}

			if((cr1|cg1|cb1)&(~255))
			{
				cr1=jx2i_gfxcon_clamp255(cr1);
				cg1=jx2i_gfxcon_clamp255(cg1);
				cb1=jx2i_gfxcon_clamp255(cb1);
			}

			cr2=(11*cr0+ 5*cr1)>>4;
			cg2=(11*cg0+ 5*cg1)>>4;
			cb2=(11*cb0+ 5*cb1)>>4;
			cr3=( 5*cr0+11*cr1)>>4;
			cg3=( 5*cg0+11*cg1)>>4;
			cb3=( 5*cb0+11*cb1)>>4;

			if(btesh2_gfxcon_swaprb)
			{
				clrt4[0]=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
				clrt4[1]=0xFF000000|(cr2<<16)|(cg2<<8)|cb2;
				clrt4[2]=0xFF000000|(cr3<<16)|(cg3<<8)|cb3;
				clrt4[3]=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
			}else
			{
				clrt4[0]=0xFF000000|(cb0<<16)|(cg0<<8)|cr0;
				clrt4[1]=0xFF000000|(cb2<<16)|(cg2<<8)|cr2;
				clrt4[2]=0xFF000000|(cb3<<16)|(cg3<<8)|cr3;
				clrt4[3]=0xFF000000|(cb1<<16)|(cg1<<8)|cr1;
			}

			by=cy*8+qy*2;
			bx=cx*8+qx*4;

			for(py=0; py<2; py++)
				for(px=0; px<2; px++)
			{
				i=15-(4*((qy&1)*2+py)+(qx*2+px));
				j=(px2>>(2*i))&3;
				clrc=clrt4[j];
				JX2I_GfxCon_PutPix200(bx+px+2+0, by+py, clrc);
				JX2I_GfxCon_PutPix200(bx+px+2+1, by+py, clrc);
			}
		}
	}

	if((((c0>>30)&3)==3) && (((c1>>30)&3)==1))
	{
		for(qy=0; qy<2; qy++)
			for(qx=0; qx<2; qx++)
		{
			switch(qy*2+qx)
			{
			case 0: k=c3; px2=c7; break;
			case 1: k=c2; px2=c6; break;
			case 2: k=c1; px2=c5; break;
			case 3: k=c0; px2=c4; break;
			}

			pxubits=(u16)(k>>14);
			pxvbits=(u16)(k<< 1);

			cy0=(pxubits>>11)&31;
			cv0=(pxubits>> 5)&31;	
			cu0=(pxubits    )&31;

			cy0=(cy0<<3)|(cy0>>2);
			cu0=(cu0<<3)|(7-(cu0>>2));
			cv0=(cv0<<3)|(7-(cv0>>2));

			cu2=cu0-128; cv2=cv0-128;
			cr0=(256*cy0        +359*cv2+128)>>8;
			cg0=(256*cy0- 88*cu2-183*cv2+128)>>8;
			cb0=(256*cy0+454*cu2        +128)>>8;

			cy0=(pxvbits>>11)&31;
			cv0=(pxvbits>> 5)&31;
			cu0=(pxvbits    )&31;

			cy0=(cy0<<3)|(cy0>>2);
			cu0=(cu0<<3)|(7-(cu0>>2));
			cv0=(cv0<<3)|(7-(cv0>>2));

			cu2=cu0-128; cv2=cv0-128;
			cr1=(256*cy0        +359*cv2+128)>>8;
			cg1=(256*cy0- 88*cu2-183*cv2+128)>>8;
			cb1=(256*cy0+454*cu2        +128)>>8;

			if((cr0|cg0|cb0)&(~255))
			{
				cr0=jx2i_gfxcon_clamp255(cr0);
				cg0=jx2i_gfxcon_clamp255(cg0);
				cb0=jx2i_gfxcon_clamp255(cb0);
			}

			if((cr1|cg1|cb1)&(~255))
			{
				cr1=jx2i_gfxcon_clamp255(cr1);
				cg1=jx2i_gfxcon_clamp255(cg1);
				cb1=jx2i_gfxcon_clamp255(cb1);
			}

			if(btesh2_gfxcon_swaprb)
			{
				clrt4[0]=0xFF000000|(cr0<<16)|(cg0<<8)|cb0;
				clrt4[1]=0xFF000000|(cr1<<16)|(cg1<<8)|cb1;
			}else
			{
				clrt4[0]=0xFF000000|(cb0<<16)|(cg0<<8)|cr0;
				clrt4[1]=0xFF000000|(cb1<<16)|(cg1<<8)|cr1;
			}

			by=cy*8+qy*4;
			bx=cx*8+qx*4;

			for(py=0; py<8; py++)
				for(px=0; px<4; px++)
			{
				i=31-(4*py+px);
				j=(px2>>i)&1;
				clrc=clrt4[j];
				JX2I_GfxCon_PutPix400(bx+px, 2*by+py, clrc);
			}
		}
	}

	return(0);
}

// u64 jx2i_gfxcon_ctrlreg[64];

int JX2I_GfxCon_UpdateForRegs()
{
	int ncx, ncy;

	jx2i_gfxcon_is80col=0;
	jx2i_gfxcon_is50row=0;
	jx2i_gfxcon_ishalfcell=0;
	jx2i_gfxcon_isqtrcell=0;
	jx2i_gfxcon_is800px=0;
	jx2i_gfxcon_ispow2=0;

	jx2i_gfxcon_is2xcol=0;
	jx2i_gfxcon_is2xrow=0;

	if(jx2i_gfxcon_ctrlreg[0]&0x0001)
		jx2i_gfxcon_is80col=1;
	if(jx2i_gfxcon_ctrlreg[0]&0x0002)
		jx2i_gfxcon_ishalfcell=1;
	if(jx2i_gfxcon_ctrlreg[0]&0x0004)
		jx2i_gfxcon_is50row=1;
	if(jx2i_gfxcon_ctrlreg[0]&0x0008)
		jx2i_gfxcon_is800px=1;
	if(jx2i_gfxcon_ctrlreg[0]&0x0100)
		jx2i_gfxcon_ispow2=1;
	if(jx2i_gfxcon_ctrlreg[0]&0x0400)
		jx2i_gfxcon_isqtrcell=1;

	if(jx2i_gfxcon_ctrlreg[0]&0x040000)
		jx2i_gfxcon_is2xcol=1;
	if(jx2i_gfxcon_ctrlreg[0]&0x080000)
		jx2i_gfxcon_is2xrow=1;

	if(!jx2i_gfxcon_is800px)
	{
		ncx=40;
		ncy=25;
		if(jx2i_gfxcon_is80col)
			ncx=80;
		if(jx2i_gfxcon_is50row)
			ncy=50;
	}else
	{
		ncx=50;
		ncy=37;
		if(jx2i_gfxcon_is80col)
			ncx=100;
		if(jx2i_gfxcon_is50row)
			ncy=75;
	}
	
	if(jx2i_gfxcon_is2xcol)
		ncx*=2;
	if(jx2i_gfxcon_is2xrow)
		ncy*=2;
	
	jx2i_gfxcon_ncx=ncx;
	jx2i_gfxcon_ncy=ncy;

	if(jx2i_gfxcon_is800px)
	{
		btesh2_gfxcon_fbxs=800;
		btesh2_gfxcon_fbys=600;
	}else
	{
		btesh2_gfxcon_fbxs=640;
		btesh2_gfxcon_fbys=400;
	}

	jx2i_gfxcon_isbmap=(jx2i_gfxcon_ctrlreg[0]>>4)&15;

	return(0);
}

int JX2I_GfxCon_Update()
{
	int ncx, ncy;
	int x, y;

	if(!jx2i_gfxcon_dirty)
		return(0);

	GfxDrv_PrepareFramebuf();

	jx2i_gfxcon_dirty=0;

//	ncx=40;
//	ncy=25;
//	if(jx2i_gfxcon_is80col)
//		ncx=80;
//	if(jx2i_gfxcon_is50row)
//		ncy=50;

	ncx=jx2i_gfxcon_ncx;
	ncy=jx2i_gfxcon_ncy;

//	for(y=0; y<25; y++)
	for(y=0; y<ncy; y++)
//		for(x=0; x<40; x++)
		for(x=0; x<ncx; x++)
	{
		JX2I_GfxCon_UpdateCell(x, y);
	}

	btesh2_gfxcon_fb_dirty=4;
	return(0);
}



int BJX2_MemGfxConCb_GetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemGfxConCb_GetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

s32 BJX2_MemGfxConCb_GetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	s64 rvq;
	int ra, rv;

	if(sp)
	{
		ra=addr-sp->addr_base;
	}
	else
	{
		ra=addr;
	}
	
	rv=0;

	if(jx2i_gfxcon_conbuf)
	{
//		rv=jx2i_gfxcon_conbuf[(ra>>2)&4095];
//		rv=jx2i_gfxcon_conbuf[(ra>>2)&8191];
//		rv=jx2i_gfxcon_conbuf[(ra>>2)&16383];
		rv=jx2i_gfxcon_conbuf[(ra>>2)&32767];
	}

	return(rv);
}

s64 BJX2_MemGfxConCb_GetQWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	u32 lo, hi;

	lo=BJX2_MemGfxConCb_GetDWord(ctx, sp, addr+0);
	hi=BJX2_MemGfxConCb_GetDWord(ctx, sp, addr+4);
	return((((u64)hi)<<32)|lo);
}


int BJX2_MemGfxConCb_SetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemGfxConCb_SetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemGfxConCb_SetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val)
{
	u64 tv;
	u32 va, vb;
	int ra, ix;
	
	if(sp)
		ra=addr-sp->addr_base;
	else
		ra=addr;

	if((addr&0x1FF00)==0x1FF00)
	{
		jx2i_gfxcon_ctrlreg[(addr>>2)&63]=val;
		JX2I_GfxCon_UpdateForRegs();

		if(((addr>>2)&63)==8)
		{
			jx2i_gfxcon_cbfrnum=val;
			if(jx2i_gfxcon_cbfrnum!=jx2i_gfxcon_cblfrnum)
			{
				if(!jx2i_gfxcon_cblfrnum)
					jx2i_gfxcon_cbffms=FRGL_TimeMS();
				jx2i_gfxcon_cblfrnum=val;
//				BJX2_ThrowFaultStatus(ctx, BJX2_FLT_SCRPOKE);
			}
		}

		if(((addr>>2)&63)==0xF)
		{
			va=jx2i_gfxcon_ctrlreg[0xC];
			vb=jx2i_gfxcon_ctrlreg[0xE];
			ix=jx2i_gfxcon_ctrlreg[0xF];
			
			tv=(((u64)vb)<<32)|va;
			jx2i_gfxcon_fontram[ix&1023]=tv;
		}

		return(0);
	}

	if(jx2i_gfxcon_conbuf)
	{
		if(jx2i_gfxcon_ctrlreg[0]&65536)
		{
			ix=(ra&(~7))|((ra&3)<<1)|((ra>>2)&1);
//			jx2i_gfxcon_conbuf[ix&32767]=val;
			jx2i_gfxcon_conbuf[ix&131071]=val;
			jx2i_gfxcon_dirty=1;
		}else
		{	
	//		jx2i_gfxcon_conbuf[(ra>>2)&4095]=val;
	//		jx2i_gfxcon_conbuf[(ra>>2)&8191]=val;
	//		jx2i_gfxcon_conbuf[(ra>>2)&16383]=val;
			jx2i_gfxcon_conbuf[(ra>>2)&32767]=val;
			jx2i_gfxcon_dirty=1;
		}

#if 0
//		if(ra==(40*25*8))
		if((ra==(8100*4) && !jx2i_gfxcon_is80col) ||
			(ra==(16100*4) && jx2i_gfxcon_is80col))
		{
			jx2i_gfxcon_cbfrnum=val;
			if(jx2i_gfxcon_cbfrnum!=jx2i_gfxcon_cblfrnum)
			{
				if(!jx2i_gfxcon_cblfrnum)
					jx2i_gfxcon_cbffms=FRGL_TimeMS();
				jx2i_gfxcon_cblfrnum=val;
				BJX2_ThrowFaultStatus(ctx, BJX2_FLT_SCRPOKE);
			}
		}
#endif
	}

	return(0);
}

int BJX2_MemGfxConCb_SetQWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s64 val)
{
	BJX2_MemGfxConCb_SetDWord(ctx, sp, addr+0, val);
	BJX2_MemGfxConCb_SetDWord(ctx, sp, addr+4, val>>32);
	return(0);
}

#if 0
int BJX2_MemDefineGfxCon(BJX2_Context *ctx,
	char *name, bjx2_addr base, bjx2_addr lim)
{
	BJX2_MemSpan *sp;
	
	sp=BJX2_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->addr_sz=lim-base;
//	sp->data=malloc((lim-base)+8);
	
	sp->GetByte=BJX2_MemGfxConCb_GetByte;
	sp->GetWord=BJX2_MemGfxConCb_GetWord;
	sp->GetDWord=BJX2_MemGfxConCb_GetDWord;
	
	sp->SetByte=BJX2_MemGfxConCb_SetByte;
	sp->SetWord=BJX2_MemGfxConCb_SetWord;
	sp->SetDWord=BJX2_MemGfxConCb_SetDWord;
	
	BJX2_MemAddSpan(ctx, sp);
	return(0);
}
#endif
