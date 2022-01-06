code
proc StringToFilter 160 8
file "../g_svcmds.c"
line 79
;1:/*
;2:===========================================================================
;3:Copyright (C) 1999-2005 Id Software, Inc.
;4:
;5:This file is part of Quake III Arena source code.
;6:
;7:Quake III Arena source code is free software; you can redistribute it
;8:and/or modify it under the terms of the GNU General Public License as
;9:published by the Free Software Foundation; either version 2 of the License,
;10:or (at your option) any later version.
;11:
;12:Quake III Arena source code is distributed in the hope that it will be
;13:useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
;14:MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;15:GNU General Public License for more details.
;16:
;17:You should have received a copy of the GNU General Public License
;18:along with Foobar; if not, write to the Free Software
;19:Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
;20:===========================================================================
;21:*/
;22://
;23:
;24:// this file holds commands that can be executed by the server console, but not remote clients
;25:
;26:#include "g_local.h"
;27:
;28:
;29:/*
;30:==============================================================================
;31:
;32:PACKET FILTERING
;33: 
;34:
;35:You can add or remove addresses from the filter list with:
;36:
;37:addip <ip>
;38:removeip <ip>
;39:
;40:The ip address is specified in dot format, and you can use '*' to match any value
;41:so you can specify an entire class C network with "addip 192.246.40.*"
;42:
;43:Removeip will only remove an address specified exactly the same way.  You cannot addip a subnet, then removeip a single host.
;44:
;45:listip
;46:Prints the current list of filters.
;47:
;48:g_filterban <0 or 1>
;49:
;50:If 1 (the default), then ip addresses matching the current list will be prohibited from entering the game.  This is the default setting.
;51:
;52:If 0, then only addresses matching the list will be allowed.  This lets you easily set up a private game, or a game that only allows players from your local network.
;53:
;54:TTimo NOTE: for persistence, bans are stored in g_banIPs cvar MAX_CVAR_VALUE_STRING
;55:The size of the cvar string buffer is limiting the banning to around 20 masks
;56:this could be improved by putting some g_banIPs2 g_banIps3 etc. maybe
;57:still, you should rely on PB for banning instead
;58:
;59:==============================================================================
;60:*/
;61:
;62:typedef struct ipFilter_s
;63:{
;64:	unsigned	mask;
;65:	unsigned	compare;
;66:} ipFilter_t;
;67:
;68:#define	MAX_IPFILTERS	1024
;69:
;70:static ipFilter_t	ipFilters[MAX_IPFILTERS];
;71:static int			numIPFilters;
;72:
;73:/*
;74:=================
;75:StringToFilter
;76:=================
;77:*/
;78:static qboolean StringToFilter (char *s, ipFilter_t *f)
;79:{
line 85
;80:	char	num[128];
;81:	int		i, j;
;82:	byte	b[4];
;83:	byte	m[4];
;84:	
;85:	for (i=0 ; i<4 ; i++)
ADDRLP4 132
CNSTI4 0
ASGNI4
LABELV $53
line 86
;86:	{
line 87
;87:		b[i] = 0;
ADDRLP4 132
INDIRI4
ADDRLP4 136
ADDP4
CNSTU1 0
ASGNU1
line 88
;88:		m[i] = 0;
ADDRLP4 132
INDIRI4
ADDRLP4 140
ADDP4
CNSTU1 0
ASGNU1
line 89
;89:	}
LABELV $54
line 85
ADDRLP4 132
ADDRLP4 132
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 132
INDIRI4
CNSTI4 4
LTI4 $53
line 91
;90:	
;91:	for (i=0 ; i<4 ; i++)
ADDRLP4 132
CNSTI4 0
ASGNI4
LABELV $57
line 92
;92:	{
line 93
;93:		if (*s < '0' || *s > '9')
ADDRLP4 144
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 144
INDIRI4
CNSTI4 48
LTI4 $63
ADDRLP4 144
INDIRI4
CNSTI4 57
LEI4 $61
LABELV $63
line 94
;94:		{
line 95
;95:			if (*s == '*') // 'match any'
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 42
NEI4 $64
line 96
;96:			{
line 98
;97:				// b[i] and m[i] to 0
;98:				s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 99
;99:				if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $66
line 100
;100:					break;
ADDRGP4 $59
JUMPV
LABELV $66
line 101
;101:				s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 102
;102:				continue;
ADDRGP4 $58
JUMPV
LABELV $64
line 104
;103:			}
;104:			G_Printf( "Bad filter address: %s\n", s );
ADDRGP4 $68
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 105
;105:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $52
JUMPV
LABELV $61
line 108
;106:		}
;107:		
;108:		j = 0;
ADDRLP4 128
CNSTI4 0
ASGNI4
ADDRGP4 $70
JUMPV
LABELV $69
line 110
;109:		while (*s >= '0' && *s <= '9')
;110:		{
line 111
;111:			num[j++] = *s++;
ADDRLP4 148
ADDRLP4 128
INDIRI4
ASGNI4
ADDRLP4 156
CNSTI4 1
ASGNI4
ADDRLP4 128
ADDRLP4 148
INDIRI4
ADDRLP4 156
INDIRI4
ADDI4
ASGNI4
ADDRLP4 152
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 152
INDIRP4
ADDRLP4 156
INDIRI4
ADDP4
ASGNP4
ADDRLP4 148
INDIRI4
ADDRLP4 0
ADDP4
ADDRLP4 152
INDIRP4
INDIRI1
ASGNI1
line 112
;112:		}
LABELV $70
line 109
ADDRLP4 148
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 48
LTI4 $72
ADDRLP4 148
INDIRI4
CNSTI4 57
LEI4 $69
LABELV $72
line 113
;113:		num[j] = 0;
ADDRLP4 128
INDIRI4
ADDRLP4 0
ADDP4
CNSTI1 0
ASGNI1
line 114
;114:		b[i] = atoi(num);
ADDRLP4 0
ARGP4
ADDRLP4 152
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 132
INDIRI4
ADDRLP4 136
ADDP4
ADDRLP4 152
INDIRI4
CVIU4 4
CVUU1 4
ASGNU1
line 115
;115:		m[i] = 255;
ADDRLP4 132
INDIRI4
ADDRLP4 140
ADDP4
CNSTU1 255
ASGNU1
line 117
;116:
;117:		if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $73
line 118
;118:			break;
ADDRGP4 $59
JUMPV
LABELV $73
line 119
;119:		s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 120
;120:	}
LABELV $58
line 91
ADDRLP4 132
ADDRLP4 132
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 132
INDIRI4
CNSTI4 4
LTI4 $57
LABELV $59
line 122
;121:	
;122:	f->mask = *(unsigned *)m;
ADDRFP4 4
INDIRP4
ADDRLP4 140
INDIRU4
ASGNU4
line 123
;123:	f->compare = *(unsigned *)b;
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 136
INDIRU4
ASGNU4
line 125
;124:	
;125:	return qtrue;
CNSTI4 1
RETI4
LABELV $52
endproc StringToFilter 160 8
proc UpdateIPBans 344 12
line 134
;126:}
;127:
;128:/*
;129:=================
;130:UpdateIPBans
;131:=================
;132:*/
;133:static void UpdateIPBans (void)
;134:{
line 141
;135:	byte	b[4];
;136:	byte	m[4];
;137:	int		i,j;
;138:	char	iplist_final[MAX_CVAR_VALUE_STRING];
;139:	char	ip[64];
;140:
;141:	*iplist_final = 0;
ADDRLP4 80
CNSTI1 0
ASGNI1
line 142
;142:	for (i = 0 ; i < numIPFilters ; i++)
ADDRLP4 76
CNSTI4 0
ASGNI4
ADDRGP4 $79
JUMPV
LABELV $76
line 143
;143:	{
line 144
;144:		if (ipFilters[i].compare == 0xffffffff)
ADDRLP4 76
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 ipFilters+4
ADDP4
INDIRU4
CNSTU4 4294967295
NEU4 $80
line 145
;145:			continue;
ADDRGP4 $77
JUMPV
LABELV $80
line 147
;146:
;147:		*(unsigned *)b = ipFilters[i].compare;
ADDRLP4 72
ADDRLP4 76
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 ipFilters+4
ADDP4
INDIRU4
ASGNU4
line 148
;148:		*(unsigned *)m = ipFilters[i].mask;
ADDRLP4 68
ADDRLP4 76
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 ipFilters
ADDP4
INDIRU4
ASGNU4
line 149
;149:		*ip = 0;
ADDRLP4 4
CNSTI1 0
ASGNI1
line 150
;150:		for (j = 0 ; j < 4 ; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $84
line 151
;151:		{
line 152
;152:			if (m[j]!=255)
ADDRLP4 0
INDIRI4
ADDRLP4 68
ADDP4
INDIRU1
CVUI4 1
CNSTI4 255
EQI4 $88
line 153
;153:				Q_strcat(ip, sizeof(ip), "*");
ADDRLP4 4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $90
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
ADDRGP4 $89
JUMPV
LABELV $88
line 155
;154:			else
;155:				Q_strcat(ip, sizeof(ip), va("%i", b[j]));
ADDRGP4 $91
ARGP4
ADDRLP4 0
INDIRI4
ADDRLP4 72
ADDP4
INDIRU1
CVUI4 1
ARGI4
ADDRLP4 336
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 336
INDIRP4
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
LABELV $89
line 156
;156:			Q_strcat(ip, sizeof(ip), (j<3) ? "." : " ");
ADDRLP4 4
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 0
INDIRI4
CNSTI4 3
GEI4 $95
ADDRLP4 340
ADDRGP4 $92
ASGNP4
ADDRGP4 $96
JUMPV
LABELV $95
ADDRLP4 340
ADDRGP4 $93
ASGNP4
LABELV $96
ADDRLP4 340
INDIRP4
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 157
;157:		}		
LABELV $85
line 150
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $84
line 158
;158:		if (strlen(iplist_final)+strlen(ip) < MAX_CVAR_VALUE_STRING)
ADDRLP4 80
ARGP4
ADDRLP4 336
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 340
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 336
INDIRI4
ADDRLP4 340
INDIRI4
ADDI4
CNSTI4 256
GEI4 $97
line 159
;159:		{
line 160
;160:			Q_strcat( iplist_final, sizeof(iplist_final), ip);
ADDRLP4 80
ARGP4
CNSTI4 256
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 161
;161:		}
ADDRGP4 $98
JUMPV
LABELV $97
line 163
;162:		else
;163:		{
line 164
;164:			Com_Printf("g_banIPs overflowed at MAX_CVAR_VALUE_STRING\n");
ADDRGP4 $99
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 165
;165:			break;
ADDRGP4 $78
JUMPV
LABELV $98
line 167
;166:		}
;167:	}
LABELV $77
line 142
ADDRLP4 76
ADDRLP4 76
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $79
ADDRLP4 76
INDIRI4
ADDRGP4 numIPFilters
INDIRI4
LTI4 $76
LABELV $78
line 169
;168:
;169:	trap_Cvar_Set( "g_banIPs", iplist_final );
ADDRGP4 $100
ARGP4
ADDRLP4 80
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 170
;170:}
LABELV $75
endproc UpdateIPBans 344 12
export G_FilterPacket
proc G_FilterPacket 28 0
line 178
;171:
;172:/*
;173:=================
;174:G_FilterPacket
;175:=================
;176:*/
;177:qboolean G_FilterPacket (char *from)
;178:{
line 184
;179:	int		i;
;180:	unsigned	in;
;181:	byte m[4];
;182:	char *p;
;183:
;184:	i = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 185
;185:	p = from;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $103
JUMPV
LABELV $102
line 186
;186:	while (*p && i < 4) {
line 187
;187:		m[i] = 0;
ADDRLP4 4
INDIRI4
ADDRLP4 8
ADDP4
CNSTU1 0
ASGNU1
ADDRGP4 $106
JUMPV
LABELV $105
line 188
;188:		while (*p >= '0' && *p <= '9') {
line 189
;189:			m[i] = m[i]*10 + (*p - '0');
ADDRLP4 16
ADDRLP4 4
INDIRI4
ADDRLP4 8
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 10
ADDRLP4 16
INDIRP4
INDIRU1
CVUI4 1
MULI4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 48
SUBI4
ADDI4
CVIU4 4
CVUU1 4
ASGNU1
line 190
;190:			p++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 191
;191:		}
LABELV $106
line 188
ADDRLP4 16
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 48
LTI4 $108
ADDRLP4 16
INDIRI4
CNSTI4 57
LEI4 $105
LABELV $108
line 192
;192:		if (!*p || *p == ':')
ADDRLP4 20
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $111
ADDRLP4 20
INDIRI4
CNSTI4 58
NEI4 $109
LABELV $111
line 193
;193:			break;
ADDRGP4 $104
JUMPV
LABELV $109
line 194
;194:		i++, p++;
ADDRLP4 24
CNSTI4 1
ASGNI4
ADDRLP4 4
ADDRLP4 4
INDIRI4
ADDRLP4 24
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
ASGNP4
line 195
;195:	}
LABELV $103
line 186
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $112
ADDRLP4 4
INDIRI4
CNSTI4 4
LTI4 $102
LABELV $112
LABELV $104
line 197
;196:	
;197:	in = *(unsigned *)m;
ADDRLP4 12
ADDRLP4 8
INDIRU4
ASGNU4
line 199
;198:
;199:	for (i=0 ; i<numIPFilters ; i++)
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $116
JUMPV
LABELV $113
line 200
;200:		if ( (in & ipFilters[i].mask) == ipFilters[i].compare)
ADDRLP4 16
ADDRLP4 4
INDIRI4
CNSTI4 3
LSHI4
ASGNI4
ADDRLP4 12
INDIRU4
ADDRLP4 16
INDIRI4
ADDRGP4 ipFilters
ADDP4
INDIRU4
BANDU4
ADDRLP4 16
INDIRI4
ADDRGP4 ipFilters+4
ADDP4
INDIRU4
NEU4 $117
line 201
;201:			return g_filterBan.integer != 0;
ADDRGP4 g_filterBan+12
INDIRI4
CNSTI4 0
EQI4 $122
ADDRLP4 20
CNSTI4 1
ASGNI4
ADDRGP4 $123
JUMPV
LABELV $122
ADDRLP4 20
CNSTI4 0
ASGNI4
LABELV $123
ADDRLP4 20
INDIRI4
RETI4
ADDRGP4 $101
JUMPV
LABELV $117
LABELV $114
line 199
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $116
ADDRLP4 4
INDIRI4
ADDRGP4 numIPFilters
INDIRI4
LTI4 $113
line 203
;202:
;203:	return g_filterBan.integer == 0;
ADDRGP4 g_filterBan+12
INDIRI4
CNSTI4 0
NEI4 $126
ADDRLP4 24
CNSTI4 1
ASGNI4
ADDRGP4 $127
JUMPV
LABELV $126
ADDRLP4 24
CNSTI4 0
ASGNI4
LABELV $127
ADDRLP4 24
INDIRI4
RETI4
LABELV $101
endproc G_FilterPacket 28 0
proc AddIP 8 8
line 212
;204:}
;205:
;206:/*
;207:=================
;208:AddIP
;209:=================
;210:*/
;211:static void AddIP( char *str )
;212:{
line 215
;213:	int		i;
;214:
;215:	for (i = 0 ; i < numIPFilters ; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $132
JUMPV
LABELV $129
line 216
;216:		if (ipFilters[i].compare == 0xffffffff)
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 ipFilters+4
ADDP4
INDIRU4
CNSTU4 4294967295
NEU4 $133
line 217
;217:			break;		// free spot
ADDRGP4 $131
JUMPV
LABELV $133
LABELV $130
line 215
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $132
ADDRLP4 0
INDIRI4
ADDRGP4 numIPFilters
INDIRI4
LTI4 $129
LABELV $131
line 218
;218:	if (i == numIPFilters)
ADDRLP4 0
INDIRI4
ADDRGP4 numIPFilters
INDIRI4
NEI4 $136
line 219
;219:	{
line 220
;220:		if (numIPFilters == MAX_IPFILTERS)
ADDRGP4 numIPFilters
INDIRI4
CNSTI4 1024
NEI4 $138
line 221
;221:		{
line 222
;222:			G_Printf ("IP filter list is full\n");
ADDRGP4 $140
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 223
;223:			return;
ADDRGP4 $128
JUMPV
LABELV $138
line 225
;224:		}
;225:		numIPFilters++;
ADDRLP4 4
ADDRGP4 numIPFilters
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 226
;226:	}
LABELV $136
line 228
;227:	
;228:	if (!StringToFilter (str, &ipFilters[i]))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 ipFilters
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 StringToFilter
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $141
line 229
;229:		ipFilters[i].compare = 0xffffffffu;
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 ipFilters+4
ADDP4
CNSTU4 4294967295
ASGNU4
LABELV $141
line 231
;230:
;231:	UpdateIPBans();
ADDRGP4 UpdateIPBans
CALLV
pop
line 232
;232:}
LABELV $128
endproc AddIP 8 8
export G_ProcessIPBans
proc G_ProcessIPBans 276 12
line 240
;233:
;234:/*
;235:=================
;236:G_ProcessIPBans
;237:=================
;238:*/
;239:void G_ProcessIPBans(void) 
;240:{
line 244
;241:	char *s, *t;
;242:	char		str[MAX_CVAR_VALUE_STRING];
;243:
;244:	Q_strncpyz( str, g_banIPs.string, sizeof(str) );
ADDRLP4 8
ARGP4
ADDRGP4 g_banIPs+16
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 246
;245:
;246:	for (t = s = g_banIPs.string; *t; /* */ ) {
ADDRLP4 264
ADDRGP4 g_banIPs+16
ASGNP4
ADDRLP4 0
ADDRLP4 264
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 264
INDIRP4
ASGNP4
ADDRGP4 $149
JUMPV
LABELV $146
line 247
;247:		s = strchr(s, ' ');
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 268
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 268
INDIRP4
ASGNP4
line 248
;248:		if (!s)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $154
line 249
;249:			break;
ADDRGP4 $148
JUMPV
LABELV $153
line 251
;250:		while (*s == ' ')
;251:			*s++ = 0;
ADDRLP4 272
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 272
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 272
INDIRP4
CNSTI1 0
ASGNI1
LABELV $154
line 250
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 32
EQI4 $153
line 252
;252:		if (*t)
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $156
line 253
;253:			AddIP( t );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 AddIP
CALLV
pop
LABELV $156
line 254
;254:		t = s;
ADDRLP4 4
ADDRLP4 0
INDIRP4
ASGNP4
line 255
;255:	}
LABELV $147
line 246
LABELV $149
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $146
LABELV $148
line 256
;256:}
LABELV $144
endproc G_ProcessIPBans 276 12
export Svcmd_AddIP_f
proc Svcmd_AddIP_f 1028 12
line 265
;257:
;258:
;259:/*
;260:=================
;261:Svcmd_AddIP_f
;262:=================
;263:*/
;264:void Svcmd_AddIP_f (void)
;265:{
line 268
;266:	char		str[MAX_TOKEN_CHARS];
;267:
;268:	if ( trap_Argc() < 2 ) {
ADDRLP4 1024
ADDRGP4 trap_Argc
CALLI4
ASGNI4
ADDRLP4 1024
INDIRI4
CNSTI4 2
GEI4 $159
line 269
;269:		G_Printf("Usage:  addip <ip-mask>\n");
ADDRGP4 $161
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 270
;270:		return;
ADDRGP4 $158
JUMPV
LABELV $159
line 273
;271:	}
;272:
;273:	trap_Argv( 1, str, sizeof( str ) );
CNSTI4 1
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 275
;274:
;275:	AddIP( str );
ADDRLP4 0
ARGP4
ADDRGP4 AddIP
CALLV
pop
line 277
;276:
;277:}
LABELV $158
endproc Svcmd_AddIP_f 1028 12
export Svcmd_RemoveIP_f
proc Svcmd_RemoveIP_f 1048 12
line 285
;278:
;279:/*
;280:=================
;281:Svcmd_RemoveIP_f
;282:=================
;283:*/
;284:void Svcmd_RemoveIP_f (void)
;285:{
line 290
;286:	ipFilter_t	f;
;287:	int			i;
;288:	char		str[MAX_TOKEN_CHARS];
;289:
;290:	if ( trap_Argc() < 2 ) {
ADDRLP4 1036
ADDRGP4 trap_Argc
CALLI4
ASGNI4
ADDRLP4 1036
INDIRI4
CNSTI4 2
GEI4 $163
line 291
;291:		G_Printf("Usage:  sv removeip <ip-mask>\n");
ADDRGP4 $165
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 292
;292:		return;
ADDRGP4 $162
JUMPV
LABELV $163
line 295
;293:	}
;294:
;295:	trap_Argv( 1, str, sizeof( str ) );
CNSTI4 1
ARGI4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 297
;296:
;297:	if (!StringToFilter (str, &f))
ADDRLP4 12
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 1040
ADDRGP4 StringToFilter
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $166
line 298
;298:		return;
ADDRGP4 $162
JUMPV
LABELV $166
line 300
;299:
;300:	for (i=0 ; i<numIPFilters ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $171
JUMPV
LABELV $168
line 301
;301:		if (ipFilters[i].mask == f.mask	&&
ADDRLP4 1044
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ASGNI4
ADDRLP4 1044
INDIRI4
ADDRGP4 ipFilters
ADDP4
INDIRU4
ADDRLP4 4
INDIRU4
NEU4 $172
ADDRLP4 1044
INDIRI4
ADDRGP4 ipFilters+4
ADDP4
INDIRU4
ADDRLP4 4+4
INDIRU4
NEU4 $172
line 302
;302:			ipFilters[i].compare == f.compare) {
line 303
;303:			ipFilters[i].compare = 0xffffffffu;
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 ipFilters+4
ADDP4
CNSTU4 4294967295
ASGNU4
line 304
;304:			G_Printf ("Removed.\n");
ADDRGP4 $177
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 306
;305:
;306:			UpdateIPBans();
ADDRGP4 UpdateIPBans
CALLV
pop
line 307
;307:			return;
ADDRGP4 $162
JUMPV
LABELV $172
line 309
;308:		}
;309:	}
LABELV $169
line 300
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $171
ADDRLP4 0
INDIRI4
ADDRGP4 numIPFilters
INDIRI4
LTI4 $168
line 311
;310:
;311:	G_Printf ( "Didn't find %s.\n", str );
ADDRGP4 $178
ARGP4
ADDRLP4 12
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 312
;312:}
LABELV $162
endproc Svcmd_RemoveIP_f 1048 12
export Svcmd_EntityList_f
proc Svcmd_EntityList_f 16 8
line 319
;313:
;314:/*
;315:===================
;316:Svcmd_EntityList_f
;317:===================
;318:*/
;319:void	Svcmd_EntityList_f (void) {
line 323
;320:	int			e;
;321:	gentity_t		*check;
;322:
;323:	check = g_entities+1;
ADDRLP4 0
ADDRGP4 g_entities+808
ASGNP4
line 324
;324:	for (e = 1; e < level.num_entities ; e++, check++) {
ADDRLP4 4
CNSTI4 1
ASGNI4
ADDRGP4 $184
JUMPV
LABELV $181
line 325
;325:		if ( !check->inuse ) {
ADDRLP4 0
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
NEI4 $186
line 326
;326:			continue;
ADDRGP4 $182
JUMPV
LABELV $186
line 328
;327:		}
;328:		G_Printf("%3i:", e);
ADDRGP4 $188
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 G_Printf
CALLV
pop
line 329
;329:		switch ( check->s.eType ) {
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
LTI4 $189
ADDRLP4 8
INDIRI4
CNSTI4 11
GTI4 $189
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $217
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $217
address $192
address $194
address $196
address $198
address $200
address $202
address $204
address $206
address $208
address $210
address $212
address $214
code
LABELV $192
line 331
;330:		case ET_GENERAL:
;331:			G_Printf("ET_GENERAL          ");
ADDRGP4 $193
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 332
;332:			break;
ADDRGP4 $190
JUMPV
LABELV $194
line 334
;333:		case ET_PLAYER:
;334:			G_Printf("ET_PLAYER           ");
ADDRGP4 $195
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 335
;335:			break;
ADDRGP4 $190
JUMPV
LABELV $196
line 337
;336:		case ET_ITEM:
;337:			G_Printf("ET_ITEM             ");
ADDRGP4 $197
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 338
;338:			break;
ADDRGP4 $190
JUMPV
LABELV $198
line 340
;339:		case ET_MISSILE:
;340:			G_Printf("ET_MISSILE          ");
ADDRGP4 $199
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 341
;341:			break;
ADDRGP4 $190
JUMPV
LABELV $200
line 343
;342:		case ET_MOVER:
;343:			G_Printf("ET_MOVER            ");
ADDRGP4 $201
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 344
;344:			break;
ADDRGP4 $190
JUMPV
LABELV $202
line 346
;345:		case ET_BEAM:
;346:			G_Printf("ET_BEAM             ");
ADDRGP4 $203
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 347
;347:			break;
ADDRGP4 $190
JUMPV
LABELV $204
line 349
;348:		case ET_PORTAL:
;349:			G_Printf("ET_PORTAL           ");
ADDRGP4 $205
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 350
;350:			break;
ADDRGP4 $190
JUMPV
LABELV $206
line 352
;351:		case ET_SPEAKER:
;352:			G_Printf("ET_SPEAKER          ");
ADDRGP4 $207
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 353
;353:			break;
ADDRGP4 $190
JUMPV
LABELV $208
line 355
;354:		case ET_PUSH_TRIGGER:
;355:			G_Printf("ET_PUSH_TRIGGER     ");
ADDRGP4 $209
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 356
;356:			break;
ADDRGP4 $190
JUMPV
LABELV $210
line 358
;357:		case ET_TELEPORT_TRIGGER:
;358:			G_Printf("ET_TELEPORT_TRIGGER ");
ADDRGP4 $211
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 359
;359:			break;
ADDRGP4 $190
JUMPV
LABELV $212
line 361
;360:		case ET_INVISIBLE:
;361:			G_Printf("ET_INVISIBLE        ");
ADDRGP4 $213
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 362
;362:			break;
ADDRGP4 $190
JUMPV
LABELV $214
line 364
;363:		case ET_GRAPPLE:
;364:			G_Printf("ET_GRAPPLE          ");
ADDRGP4 $215
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 365
;365:			break;
ADDRGP4 $190
JUMPV
LABELV $189
line 367
;366:		default:
;367:			G_Printf("%3i                 ", check->s.eType);
ADDRGP4 $216
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_Printf
CALLV
pop
line 368
;368:			break;
LABELV $190
line 371
;369:		}
;370:
;371:		if ( check->classname ) {
ADDRLP4 0
INDIRP4
CNSTI4 524
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $218
line 372
;372:			G_Printf("%s", check->classname);
ADDRGP4 $220
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 524
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 373
;373:		}
LABELV $218
line 374
;374:		G_Printf("\n");
ADDRGP4 $221
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 375
;375:	}
LABELV $182
line 324
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 808
ADDP4
ASGNP4
LABELV $184
ADDRLP4 4
INDIRI4
ADDRGP4 level+12
INDIRI4
LTI4 $181
line 376
;376:}
LABELV $179
endproc Svcmd_EntityList_f 16 8
export ClientForString
proc ClientForString 24 8
line 378
;377:
;378:gclient_t	*ClientForString( const char *s ) {
line 384
;379:	gclient_t	*cl;
;380:	int			i;
;381:	int			idnum;
;382:
;383:	// numeric values are just slot numbers
;384:	if ( s[0] >= '0' && s[0] <= '9' ) {
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 48
LTI4 $223
ADDRLP4 12
INDIRI4
CNSTI4 57
GTI4 $223
line 385
;385:		idnum = atoi( s );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 16
INDIRI4
ASGNI4
line 386
;386:		if ( idnum < 0 || idnum >= level.maxclients ) {
ADDRLP4 20
ADDRLP4 8
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
LTI4 $228
ADDRLP4 20
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $225
LABELV $228
line 387
;387:			Com_Printf( "Bad client slot: %i\n", idnum );
ADDRGP4 $229
ARGP4
ADDRLP4 8
INDIRI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 388
;388:			return NULL;
CNSTP4 0
RETP4
ADDRGP4 $222
JUMPV
LABELV $225
line 391
;389:		}
;390:
;391:		cl = &level.clients[idnum];
ADDRLP4 0
CNSTI4 776
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 392
;392:		if ( cl->pers.connected == CON_DISCONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 0
NEI4 $230
line 393
;393:			G_Printf( "Client %i is not connected\n", idnum );
ADDRGP4 $232
ARGP4
ADDRLP4 8
INDIRI4
ARGI4
ADDRGP4 G_Printf
CALLV
pop
line 394
;394:			return NULL;
CNSTP4 0
RETP4
ADDRGP4 $222
JUMPV
LABELV $230
line 396
;395:		}
;396:		return cl;
ADDRLP4 0
INDIRP4
RETP4
ADDRGP4 $222
JUMPV
LABELV $223
line 400
;397:	}
;398:
;399:	// check for a name match
;400:	for ( i=0 ; i < level.maxclients ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $236
JUMPV
LABELV $233
line 401
;401:		cl = &level.clients[i];
ADDRLP4 0
CNSTI4 776
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 402
;402:		if ( cl->pers.connected == CON_DISCONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 0
NEI4 $238
line 403
;403:			continue;
ADDRGP4 $234
JUMPV
LABELV $238
line 405
;404:		}
;405:		if ( !Q_stricmp( cl->pers.netname, s ) ) {
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $240
line 406
;406:			return cl;
ADDRLP4 0
INDIRP4
RETP4
ADDRGP4 $222
JUMPV
LABELV $240
line 408
;407:		}
;408:	}
LABELV $234
line 400
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $236
ADDRLP4 4
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $233
line 410
;409:
;410:	G_Printf( "User %s is not on the server\n", s );
ADDRGP4 $242
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 412
;411:
;412:	return NULL;
CNSTP4 0
RETP4
LABELV $222
endproc ClientForString 24 8
export Svcmd_ForceTeam_f
proc Svcmd_ForceTeam_f 1032 12
line 422
;413:}
;414:
;415:/*
;416:===================
;417:Svcmd_ForceTeam_f
;418:
;419:forceteam <player> <team>
;420:===================
;421:*/
;422:void	Svcmd_ForceTeam_f( void ) {
line 427
;423:	gclient_t	*cl;
;424:	char		str[MAX_TOKEN_CHARS];
;425:
;426:	// find the player
;427:	trap_Argv( 1, str, sizeof( str ) );
CNSTI4 1
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 428
;428:	cl = ClientForString( str );
ADDRLP4 0
ARGP4
ADDRLP4 1028
ADDRGP4 ClientForString
CALLP4
ASGNP4
ADDRLP4 1024
ADDRLP4 1028
INDIRP4
ASGNP4
line 429
;429:	if ( !cl ) {
ADDRLP4 1024
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $244
line 430
;430:		return;
ADDRGP4 $243
JUMPV
LABELV $244
line 434
;431:	}
;432:
;433:	// set the team
;434:	trap_Argv( 2, str, sizeof( str ) );
CNSTI4 2
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 435
;435:	SetTeam( &g_entities[cl - level.clients], str );
CNSTI4 808
ADDRLP4 1024
INDIRP4
CVPU4 4
ADDRGP4 level
INDIRP4
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 776
DIVI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 SetTeam
CALLV
pop
line 436
;436:}
LABELV $243
endproc Svcmd_ForceTeam_f 1032 12
export ConsoleCommand
proc ConsoleCommand 1072 12
line 446
;437:
;438:char	*ConcatArgs( int start );
;439:
;440:/*
;441:=================
;442:ConsoleCommand
;443:
;444:=================
;445:*/
;446:qboolean	ConsoleCommand( void ) {
line 449
;447:	char	cmd[MAX_TOKEN_CHARS];
;448:
;449:	trap_Argv( 0, cmd, sizeof( cmd ) );
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 451
;450:
;451:	if ( Q_stricmp (cmd, "entitylist") == 0 ) {
ADDRLP4 0
ARGP4
ADDRGP4 $249
ARGP4
ADDRLP4 1024
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1024
INDIRI4
CNSTI4 0
NEI4 $247
line 452
;452:		Svcmd_EntityList_f();
ADDRGP4 Svcmd_EntityList_f
CALLV
pop
line 453
;453:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $246
JUMPV
LABELV $247
line 456
;454:	}
;455:
;456:	if ( Q_stricmp (cmd, "forceteam") == 0 ) {
ADDRLP4 0
ARGP4
ADDRGP4 $252
ARGP4
ADDRLP4 1028
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1028
INDIRI4
CNSTI4 0
NEI4 $250
line 457
;457:		Svcmd_ForceTeam_f();
ADDRGP4 Svcmd_ForceTeam_f
CALLV
pop
line 458
;458:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $246
JUMPV
LABELV $250
line 461
;459:	}
;460:
;461:	if (Q_stricmp (cmd, "game_memory") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $255
ARGP4
ADDRLP4 1032
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1032
INDIRI4
CNSTI4 0
NEI4 $253
line 462
;462:		Svcmd_GameMem_f();
ADDRGP4 Svcmd_GameMem_f
CALLV
pop
line 463
;463:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $246
JUMPV
LABELV $253
line 466
;464:	}
;465:
;466:	if (Q_stricmp (cmd, "addbot") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $258
ARGP4
ADDRLP4 1036
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1036
INDIRI4
CNSTI4 0
NEI4 $256
line 467
;467:		Svcmd_AddBot_f();
ADDRGP4 Svcmd_AddBot_f
CALLV
pop
line 468
;468:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $246
JUMPV
LABELV $256
line 471
;469:	}
;470:
;471:	if (Q_stricmp (cmd, "botlist") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $261
ARGP4
ADDRLP4 1040
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $259
line 472
;472:		Svcmd_BotList_f();
ADDRGP4 Svcmd_BotList_f
CALLV
pop
line 473
;473:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $246
JUMPV
LABELV $259
line 476
;474:	}
;475:
;476:	if (Q_stricmp (cmd, "abort_podium") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $264
ARGP4
ADDRLP4 1044
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
NEI4 $262
line 477
;477:		Svcmd_AbortPodium_f();
ADDRGP4 Svcmd_AbortPodium_f
CALLV
pop
line 478
;478:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $246
JUMPV
LABELV $262
line 481
;479:	}
;480:
;481:	if (Q_stricmp (cmd, "addip") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $267
ARGP4
ADDRLP4 1048
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
NEI4 $265
line 482
;482:		Svcmd_AddIP_f();
ADDRGP4 Svcmd_AddIP_f
CALLV
pop
line 483
;483:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $246
JUMPV
LABELV $265
line 486
;484:	}
;485:
;486:	if (Q_stricmp (cmd, "removeip") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $270
ARGP4
ADDRLP4 1052
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 0
NEI4 $268
line 487
;487:		Svcmd_RemoveIP_f();
ADDRGP4 Svcmd_RemoveIP_f
CALLV
pop
line 488
;488:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $246
JUMPV
LABELV $268
line 491
;489:	}
;490:
;491:	if (Q_stricmp (cmd, "listip") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $273
ARGP4
ADDRLP4 1056
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
NEI4 $271
line 492
;492:		trap_SendConsoleCommand( EXEC_NOW, "g_banIPs\n" );
CNSTI4 0
ARGI4
ADDRGP4 $274
ARGP4
ADDRGP4 trap_SendConsoleCommand
CALLV
pop
line 493
;493:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $246
JUMPV
LABELV $271
line 496
;494:	}
;495:
;496:	if (g_dedicated.integer) {
ADDRGP4 g_dedicated+12
INDIRI4
CNSTI4 0
EQI4 $275
line 497
;497:		if (Q_stricmp (cmd, "say") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $280
ARGP4
ADDRLP4 1060
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1060
INDIRI4
CNSTI4 0
NEI4 $278
line 498
;498:			trap_SendServerCommand( -1, va("print \"server: %s\"", ConcatArgs(1) ) );
CNSTI4 1
ARGI4
ADDRLP4 1064
ADDRGP4 ConcatArgs
CALLP4
ASGNP4
ADDRGP4 $281
ARGP4
ADDRLP4 1064
INDIRP4
ARGP4
ADDRLP4 1068
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 1068
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 499
;499:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $246
JUMPV
LABELV $278
line 502
;500:		}
;501:		// everything else will also be printed as a say command
;502:		trap_SendServerCommand( -1, va("print \"server: %s\"", ConcatArgs(0) ) );
CNSTI4 0
ARGI4
ADDRLP4 1064
ADDRGP4 ConcatArgs
CALLP4
ASGNP4
ADDRGP4 $281
ARGP4
ADDRLP4 1064
INDIRP4
ARGP4
ADDRLP4 1068
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 1068
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 503
;503:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $246
JUMPV
LABELV $275
line 506
;504:	}
;505:
;506:	return qfalse;
CNSTI4 0
RETI4
LABELV $246
endproc ConsoleCommand 1072 12
import ConcatArgs
bss
align 4
LABELV numIPFilters
skip 4
align 4
LABELV ipFilters
skip 8192
import trap_SnapVector
import trap_GeneticParentsAndChildSelection
import trap_BotResetWeaponState
import trap_BotFreeWeaponState
import trap_BotAllocWeaponState
import trap_BotLoadWeaponWeights
import trap_BotGetWeaponInfo
import trap_BotChooseBestFightWeapon
import trap_BotAddAvoidSpot
import trap_BotInitMoveState
import trap_BotFreeMoveState
import trap_BotAllocMoveState
import trap_BotPredictVisiblePosition
import trap_BotMovementViewTarget
import trap_BotReachabilityArea
import trap_BotResetLastAvoidReach
import trap_BotResetAvoidReach
import trap_BotMoveInDirection
import trap_BotMoveToGoal
import trap_BotResetMoveState
import trap_BotFreeGoalState
import trap_BotAllocGoalState
import trap_BotMutateGoalFuzzyLogic
import trap_BotSaveGoalFuzzyLogic
import trap_BotInterbreedGoalFuzzyLogic
import trap_BotFreeItemWeights
import trap_BotLoadItemWeights
import trap_BotUpdateEntityItems
import trap_BotInitLevelItems
import trap_BotSetAvoidGoalTime
import trap_BotAvoidGoalTime
import trap_BotGetLevelItemGoal
import trap_BotGetMapLocationGoal
import trap_BotGetNextCampSpotGoal
import trap_BotItemGoalInVisButNotVisible
import trap_BotTouchingGoal
import trap_BotChooseNBGItem
import trap_BotChooseLTGItem
import trap_BotGetSecondGoal
import trap_BotGetTopGoal
import trap_BotGoalName
import trap_BotDumpGoalStack
import trap_BotDumpAvoidGoals
import trap_BotEmptyGoalStack
import trap_BotPopGoal
import trap_BotPushGoal
import trap_BotResetAvoidGoals
import trap_BotRemoveFromAvoidGoals
import trap_BotResetGoalState
import trap_BotSetChatName
import trap_BotSetChatGender
import trap_BotLoadChatFile
import trap_BotReplaceSynonyms
import trap_UnifyWhiteSpaces
import trap_BotMatchVariable
import trap_BotFindMatch
import trap_StringContains
import trap_BotGetChatMessage
import trap_BotEnterChat
import trap_BotChatLength
import trap_BotReplyChat
import trap_BotNumInitialChats
import trap_BotInitialChat
import trap_BotNumConsoleMessages
import trap_BotNextConsoleMessage
import trap_BotRemoveConsoleMessage
import trap_BotQueueConsoleMessage
import trap_BotFreeChatState
import trap_BotAllocChatState
import trap_Characteristic_String
import trap_Characteristic_BInteger
import trap_Characteristic_Integer
import trap_Characteristic_BFloat
import trap_Characteristic_Float
import trap_BotFreeCharacter
import trap_BotLoadCharacter
import trap_EA_ResetInput
import trap_EA_GetInput
import trap_EA_EndRegular
import trap_EA_View
import trap_EA_Move
import trap_EA_DelayedJump
import trap_EA_Jump
import trap_EA_SelectWeapon
import trap_EA_MoveRight
import trap_EA_MoveLeft
import trap_EA_MoveBack
import trap_EA_MoveForward
import trap_EA_MoveDown
import trap_EA_MoveUp
import trap_EA_Crouch
import trap_EA_Respawn
import trap_EA_Use
import trap_EA_Attack
import trap_EA_Talk
import trap_EA_Gesture
import trap_EA_Action
import trap_EA_Command
import trap_EA_SayTeam
import trap_EA_Say
import trap_AAS_PredictClientMovement
import trap_AAS_Swimming
import trap_AAS_AlternativeRouteGoals
import trap_AAS_PredictRoute
import trap_AAS_EnableRoutingArea
import trap_AAS_AreaTravelTimeToGoalArea
import trap_AAS_AreaReachability
import trap_AAS_IntForBSPEpairKey
import trap_AAS_FloatForBSPEpairKey
import trap_AAS_VectorForBSPEpairKey
import trap_AAS_ValueForBSPEpairKey
import trap_AAS_NextBSPEntity
import trap_AAS_PointContents
import trap_AAS_TraceAreas
import trap_AAS_PointReachabilityAreaIndex
import trap_AAS_PointAreaNum
import trap_AAS_Time
import trap_AAS_PresenceTypeBoundingBox
import trap_AAS_Initialized
import trap_AAS_EntityInfo
import trap_AAS_AreaInfo
import trap_AAS_BBoxAreas
import trap_BotUserCommand
import trap_BotGetServerCommand
import trap_BotGetSnapshotEntity
import trap_BotLibTest
import trap_BotLibUpdateEntity
import trap_BotLibLoadMap
import trap_BotLibStartFrame
import trap_BotLibDefine
import trap_BotLibVarGet
import trap_BotLibVarSet
import trap_BotLibShutdown
import trap_BotLibSetup
import trap_DebugPolygonDelete
import trap_DebugPolygonCreate
import trap_GetEntityToken
import trap_GetUsercmd
import trap_BotFreeClient
import trap_BotAllocateClient
import trap_EntityContact
import trap_EntitiesInBox
import trap_UnlinkEntity
import trap_LinkEntity
import trap_AreasConnected
import trap_AdjustAreaPortalState
import trap_InPVSIgnorePortals
import trap_InPVS
import trap_PointContents
import trap_Trace
import trap_SetBrushModel
import trap_GetServerinfo
import trap_SetUserinfo
import trap_GetUserinfo
import trap_GetConfigstring
import trap_SetConfigstring
import trap_SendServerCommand
import trap_DropClient
import trap_LocateGameData
import trap_Cvar_VariableStringBuffer
import trap_Cvar_VariableValue
import trap_Cvar_VariableIntegerValue
import trap_Cvar_Set
import trap_Cvar_Update
import trap_Cvar_Register
import trap_SendConsoleCommand
import trap_FS_Seek
import trap_FS_GetFileList
import trap_FS_FCloseFile
import trap_FS_Write
import trap_FS_Read
import trap_FS_FOpenFile
import trap_Args
import trap_Argv
import trap_Argc
import trap_Milliseconds
import trap_Error
import trap_Printf
import g_proxMineTimeout
import g_singlePlayer
import g_enableBreath
import g_enableDust
import g_rankings
import pmove_msec
import pmove_fixed
import g_smoothClients
import g_blueteam
import g_redteam
import g_cubeTimeout
import g_obeliskRespawnDelay
import g_obeliskRegenAmount
import g_obeliskRegenPeriod
import g_obeliskHealth
import g_filterBan
import g_banIPs
import g_teamForceBalance
import g_teamAutoJoin
import g_allowVote
import g_blood
import g_doWarmup
import g_warmup
import g_motd
import g_synchronousClients
import g_weaponTeamRespawn
import g_weaponRespawn
import g_debugDamage
import g_debugAlloc
import g_debugMove
import g_inactivity
import g_forcerespawn
import g_quadfactor
import g_knockback
import g_speed
import g_gravity
import g_needpass
import g_password
import g_friendlyFire
import g_capturelimit
import g_timelimit
import g_fraglimit
import g_dmflags
import g_restarted
import g_maxGameClients
import g_maxclients
import g_cheats
import g_dedicated
import g_gametype
import g_entities
import level
import Pickup_Team
import CheckTeamStatus
import TeamplayInfoMessage
import Team_GetLocationMsg
import Team_GetLocation
import SelectCTFSpawnPoint
import Team_FreeEntity
import Team_ReturnFlag
import Team_InitGame
import Team_CheckHurtCarrier
import Team_FragBonuses
import Team_DroppedFlagThink
import AddTeamScore
import TeamColorString
import OtherTeamName
import TeamName
import OtherTeam
import BotTestAAS
import BotAIStartFrame
import BotAIShutdownClient
import BotAISetupClient
import BotAILoadMap
import BotAIShutdown
import BotAISetup
import BotInterbreedEndMatch
import Svcmd_BotList_f
import Svcmd_AddBot_f
import G_BotConnect
import G_RemoveQueuedBotBegin
import G_CheckBotSpawn
import G_GetBotInfoByName
import G_GetBotInfoByNumber
import G_InitBots
import Svcmd_AbortPodium_f
import SpawnModelsOnVictoryPads
import UpdateTournamentInfo
import G_WriteSessionData
import G_InitWorldSession
import G_InitSessionData
import G_ReadSessionData
import Svcmd_GameMem_f
import G_InitMemory
import G_Alloc
import CheckObeliskAttack
import Team_CheckDroppedItem
import OnSameTeam
import G_RunClient
import ClientEndFrame
import ClientThink
import ClientCommand
import ClientBegin
import ClientDisconnect
import ClientUserinfoChanged
import ClientConnect
import G_Error
import G_Printf
import SendScoreboardMessageToAllClients
import G_LogPrintf
import G_RunThink
import CheckTeamLeader
import SetLeader
import FindIntermissionPoint
import DeathmatchScoreboardMessage
import G_SetStats
import MoveClientToIntermission
import FireWeapon
import SpotWouldTelefrag
import CalculateRanks
import AddScore
import player_die
import ClientSpawn
import InitBodyQue
import InitClientResp
import InitClientPersistant
import BeginIntermission
import respawn
import CopyToBodyQue
import SelectSpawnPoint
import SetClientViewAngle
import PickTeam
import TeamLeader
import TeamCount
import Weapon_HookThink
import Weapon_HookFree
import CheckGauntletAttack
import SnapVectorTowards
import CalcMuzzlePoint
import LogAccuracyHit
import TeleportPlayer
import trigger_teleporter_touch
import Touch_DoorTrigger
import G_RunMover
import fire_grapple
import fire_bfg
import fire_rocket
import fire_grenade
import fire_plasma
import fire_blaster
import G_RunMissile
import TossClientCubes
import TossClientItems
import body_die
import G_InvulnerabilityEffect
import G_RadiusDamage
import G_Damage
import CanDamage
import BuildShaderStateConfig
import AddRemap
import G_SetOrigin
import G_AddEvent
import G_AddPredictableEvent
import vectoyaw
import vtos
import tv
import G_TouchSolids
import G_TouchTriggers
import G_EntitiesFree
import G_FreeEntity
import G_Sound
import G_TempEntity
import G_Spawn
import G_InitGentity
import G_SetMovedir
import G_UseTargets
import G_PickTarget
import G_Find
import G_KillBox
import G_TeamCommand
import G_SoundIndex
import G_ModelIndex
import SaveRegisteredItems
import RegisterItem
import ClearRegisteredItems
import Touch_Item
import Add_Ammo
import ArmorIndex
import Think_Weapon
import FinishSpawningItem
import G_SpawnItem
import SetRespawn
import LaunchItem
import Drop_Item
import PrecacheItem
import UseHoldableItem
import RespawnItem
import G_RunItem
import G_CheckTeamItems
import Cmd_FollowCycle_f
import SetTeam
import BroadcastTeamChange
import StopFollowing
import Cmd_Score_f
import G_NewString
import G_SpawnEntitiesFromString
import G_SpawnVector
import G_SpawnInt
import G_SpawnFloat
import G_SpawnString
import BG_PlayerTouchesItem
import BG_PlayerStateToEntityStateExtraPolate
import BG_PlayerStateToEntityState
import BG_TouchJumpPad
import BG_AddPredictableEventToPlayerstate
import BG_EvaluateTrajectoryDelta
import BG_EvaluateTrajectory
import BG_CanItemBeGrabbed
import BG_FindItemForHoldable
import BG_FindItemForPowerup
import BG_FindItemForWeapon
import BG_FindItem
import bg_numItems
import bg_itemlist
import Pmove
import PM_UpdateViewAngles
import Com_Printf
import Com_Error
import Info_NextPair
import Info_Validate
import Info_SetValueForKey_Big
import Info_SetValueForKey
import Info_RemoveKey_big
import Info_RemoveKey
import Info_ValueForKey
import va
import Q_CleanStr
import Q_PrintStrlen
import Q_strcat
import Q_strncpyz
import Q_strrchr
import Q_strupr
import Q_strlwr
import Q_stricmpn
import Q_strncmp
import Q_stricmp
import Q_isalpha
import Q_isupper
import Q_islower
import Q_isprint
import Com_sprintf
import Parse3DMatrix
import Parse2DMatrix
import Parse1DMatrix
import SkipRestOfLine
import SkipBracedSection
import COM_MatchToken
import COM_ParseWarning
import COM_ParseError
import COM_Compress
import COM_ParseExt
import COM_Parse
import COM_GetCurrentParseLine
import COM_BeginParseSession
import COM_DefaultExtension
import COM_StripExtension
import COM_SkipPath
import Com_Clamp
import PerpendicularVector
import AngleVectors
import MatrixMultiply
import MakeNormalVectors
import RotateAroundDirection
import RotatePointAroundVector
import ProjectPointOnPlane
import PlaneFromPoints
import AngleDelta
import AngleNormalize180
import AngleNormalize360
import AnglesSubtract
import AngleSubtract
import LerpAngle
import AngleMod
import BoxOnPlaneSide
import SetPlaneSignbits
import AxisCopy
import AxisClear
import AnglesToAxis
import vectoangles
import Q_crandom
import Q_random
import Q_rand
import Q_acos
import Q_log2
import VectorRotate
import Vector4Scale
import VectorNormalize2
import VectorNormalize
import CrossProduct
import VectorInverse
import VectorNormalizeFast
import DistanceSquared
import Distance
import VectorLengthSquared
import VectorLength
import VectorCompare
import AddPointToBounds
import ClearBounds
import RadiusFromBounds
import NormalizeColor
import ColorBytes4
import ColorBytes3
import _VectorMA
import _VectorScale
import _VectorCopy
import _VectorAdd
import _VectorSubtract
import _DotProduct
import ByteToDir
import DirToByte
import ClampShort
import ClampChar
import Q_rsqrt
import Q_fabs
import axisDefault
import vec3_origin
import g_color_table
import colorDkGrey
import colorMdGrey
import colorLtGrey
import colorWhite
import colorCyan
import colorMagenta
import colorYellow
import colorBlue
import colorGreen
import colorRed
import colorBlack
import bytedirs
import Com_Memcpy
import Com_Memset
import Hunk_Alloc
import FloatSwap
import LongSwap
import ShortSwap
import acos
import fabs
import abs
import tan
import atan2
import cos
import sin
import sqrt
import floor
import ceil
import memcpy
import memset
import memmove
import sscanf
import vsprintf
import _atoi
import atoi
import _atof
import atof
import toupper
import tolower
import strncpy
import strstr
import strchr
import strcmp
import strcpy
import strcat
import strlen
import rand
import srand
import qsort
lit
align 1
LABELV $281
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 32
byte 1 34
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 34
byte 1 0
align 1
LABELV $280
byte 1 115
byte 1 97
byte 1 121
byte 1 0
align 1
LABELV $274
byte 1 103
byte 1 95
byte 1 98
byte 1 97
byte 1 110
byte 1 73
byte 1 80
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $273
byte 1 108
byte 1 105
byte 1 115
byte 1 116
byte 1 105
byte 1 112
byte 1 0
align 1
LABELV $270
byte 1 114
byte 1 101
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 105
byte 1 112
byte 1 0
align 1
LABELV $267
byte 1 97
byte 1 100
byte 1 100
byte 1 105
byte 1 112
byte 1 0
align 1
LABELV $264
byte 1 97
byte 1 98
byte 1 111
byte 1 114
byte 1 116
byte 1 95
byte 1 112
byte 1 111
byte 1 100
byte 1 105
byte 1 117
byte 1 109
byte 1 0
align 1
LABELV $261
byte 1 98
byte 1 111
byte 1 116
byte 1 108
byte 1 105
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $258
byte 1 97
byte 1 100
byte 1 100
byte 1 98
byte 1 111
byte 1 116
byte 1 0
align 1
LABELV $255
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 95
byte 1 109
byte 1 101
byte 1 109
byte 1 111
byte 1 114
byte 1 121
byte 1 0
align 1
LABELV $252
byte 1 102
byte 1 111
byte 1 114
byte 1 99
byte 1 101
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $249
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 108
byte 1 105
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $242
byte 1 85
byte 1 115
byte 1 101
byte 1 114
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 105
byte 1 115
byte 1 32
byte 1 110
byte 1 111
byte 1 116
byte 1 32
byte 1 111
byte 1 110
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 10
byte 1 0
align 1
LABELV $232
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 105
byte 1 115
byte 1 32
byte 1 110
byte 1 111
byte 1 116
byte 1 32
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 101
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $229
byte 1 66
byte 1 97
byte 1 100
byte 1 32
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 32
byte 1 115
byte 1 108
byte 1 111
byte 1 116
byte 1 58
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $221
byte 1 10
byte 1 0
align 1
LABELV $220
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $216
byte 1 37
byte 1 51
byte 1 105
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $215
byte 1 69
byte 1 84
byte 1 95
byte 1 71
byte 1 82
byte 1 65
byte 1 80
byte 1 80
byte 1 76
byte 1 69
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $213
byte 1 69
byte 1 84
byte 1 95
byte 1 73
byte 1 78
byte 1 86
byte 1 73
byte 1 83
byte 1 73
byte 1 66
byte 1 76
byte 1 69
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $211
byte 1 69
byte 1 84
byte 1 95
byte 1 84
byte 1 69
byte 1 76
byte 1 69
byte 1 80
byte 1 79
byte 1 82
byte 1 84
byte 1 95
byte 1 84
byte 1 82
byte 1 73
byte 1 71
byte 1 71
byte 1 69
byte 1 82
byte 1 32
byte 1 0
align 1
LABELV $209
byte 1 69
byte 1 84
byte 1 95
byte 1 80
byte 1 85
byte 1 83
byte 1 72
byte 1 95
byte 1 84
byte 1 82
byte 1 73
byte 1 71
byte 1 71
byte 1 69
byte 1 82
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $207
byte 1 69
byte 1 84
byte 1 95
byte 1 83
byte 1 80
byte 1 69
byte 1 65
byte 1 75
byte 1 69
byte 1 82
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $205
byte 1 69
byte 1 84
byte 1 95
byte 1 80
byte 1 79
byte 1 82
byte 1 84
byte 1 65
byte 1 76
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $203
byte 1 69
byte 1 84
byte 1 95
byte 1 66
byte 1 69
byte 1 65
byte 1 77
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $201
byte 1 69
byte 1 84
byte 1 95
byte 1 77
byte 1 79
byte 1 86
byte 1 69
byte 1 82
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $199
byte 1 69
byte 1 84
byte 1 95
byte 1 77
byte 1 73
byte 1 83
byte 1 83
byte 1 73
byte 1 76
byte 1 69
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $197
byte 1 69
byte 1 84
byte 1 95
byte 1 73
byte 1 84
byte 1 69
byte 1 77
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $195
byte 1 69
byte 1 84
byte 1 95
byte 1 80
byte 1 76
byte 1 65
byte 1 89
byte 1 69
byte 1 82
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $193
byte 1 69
byte 1 84
byte 1 95
byte 1 71
byte 1 69
byte 1 78
byte 1 69
byte 1 82
byte 1 65
byte 1 76
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $188
byte 1 37
byte 1 51
byte 1 105
byte 1 58
byte 1 0
align 1
LABELV $178
byte 1 68
byte 1 105
byte 1 100
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 102
byte 1 105
byte 1 110
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $177
byte 1 82
byte 1 101
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 100
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $165
byte 1 85
byte 1 115
byte 1 97
byte 1 103
byte 1 101
byte 1 58
byte 1 32
byte 1 32
byte 1 115
byte 1 118
byte 1 32
byte 1 114
byte 1 101
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 105
byte 1 112
byte 1 32
byte 1 60
byte 1 105
byte 1 112
byte 1 45
byte 1 109
byte 1 97
byte 1 115
byte 1 107
byte 1 62
byte 1 10
byte 1 0
align 1
LABELV $161
byte 1 85
byte 1 115
byte 1 97
byte 1 103
byte 1 101
byte 1 58
byte 1 32
byte 1 32
byte 1 97
byte 1 100
byte 1 100
byte 1 105
byte 1 112
byte 1 32
byte 1 60
byte 1 105
byte 1 112
byte 1 45
byte 1 109
byte 1 97
byte 1 115
byte 1 107
byte 1 62
byte 1 10
byte 1 0
align 1
LABELV $140
byte 1 73
byte 1 80
byte 1 32
byte 1 102
byte 1 105
byte 1 108
byte 1 116
byte 1 101
byte 1 114
byte 1 32
byte 1 108
byte 1 105
byte 1 115
byte 1 116
byte 1 32
byte 1 105
byte 1 115
byte 1 32
byte 1 102
byte 1 117
byte 1 108
byte 1 108
byte 1 10
byte 1 0
align 1
LABELV $100
byte 1 103
byte 1 95
byte 1 98
byte 1 97
byte 1 110
byte 1 73
byte 1 80
byte 1 115
byte 1 0
align 1
LABELV $99
byte 1 103
byte 1 95
byte 1 98
byte 1 97
byte 1 110
byte 1 73
byte 1 80
byte 1 115
byte 1 32
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 102
byte 1 108
byte 1 111
byte 1 119
byte 1 101
byte 1 100
byte 1 32
byte 1 97
byte 1 116
byte 1 32
byte 1 77
byte 1 65
byte 1 88
byte 1 95
byte 1 67
byte 1 86
byte 1 65
byte 1 82
byte 1 95
byte 1 86
byte 1 65
byte 1 76
byte 1 85
byte 1 69
byte 1 95
byte 1 83
byte 1 84
byte 1 82
byte 1 73
byte 1 78
byte 1 71
byte 1 10
byte 1 0
align 1
LABELV $93
byte 1 32
byte 1 0
align 1
LABELV $92
byte 1 46
byte 1 0
align 1
LABELV $91
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $90
byte 1 42
byte 1 0
align 1
LABELV $68
byte 1 66
byte 1 97
byte 1 100
byte 1 32
byte 1 102
byte 1 105
byte 1 108
byte 1 116
byte 1 101
byte 1 114
byte 1 32
byte 1 97
byte 1 100
byte 1 100
byte 1 114
byte 1 101
byte 1 115
byte 1 115
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
