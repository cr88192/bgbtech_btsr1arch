export G_SpawnString
code
proc G_SpawnString 8 8
file "../g_spawn.c"
line 26
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
;24:#include "g_local.h"
;25:
;26:qboolean	G_SpawnString( const char *key, const char *defaultString, char **out ) {
line 29
;27:	int		i;
;28:
;29:	if ( !level.spawning ) {
ADDRGP4 level+4504
INDIRI4
CNSTI4 0
NEI4 $53
line 30
;30:		*out = (char *)defaultString;
ADDRFP4 8
INDIRP4
ADDRFP4 4
INDIRP4
ASGNP4
line 32
;31://		G_Error( "G_SpawnString() called while not spawning" );
;32:	}
LABELV $53
line 34
;33:
;34:	for ( i = 0 ; i < level.numSpawnVars ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $59
JUMPV
LABELV $56
line 35
;35:		if ( !Q_stricmp( key, level.spawnVars[i][0] ) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 level+4512
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $61
line 36
;36:			*out = level.spawnVars[i][1];
ADDRFP4 8
INDIRP4
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 level+4512+4
ADDP4
INDIRP4
ASGNP4
line 37
;37:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $52
JUMPV
LABELV $61
line 39
;38:		}
;39:	}
LABELV $57
line 34
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $59
ADDRLP4 0
INDIRI4
ADDRGP4 level+4508
INDIRI4
LTI4 $56
line 41
;40:
;41:	*out = (char *)defaultString;
ADDRFP4 8
INDIRP4
ADDRFP4 4
INDIRP4
ASGNP4
line 42
;42:	return qfalse;
CNSTI4 0
RETI4
LABELV $52
endproc G_SpawnString 8 8
export G_SpawnFloat
proc G_SpawnFloat 16 12
line 45
;43:}
;44:
;45:qboolean	G_SpawnFloat( const char *key, const char *defaultString, float *out ) {
line 49
;46:	char		*s;
;47:	qboolean	present;
;48:
;49:	present = G_SpawnString( key, defaultString, &s );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 8
ADDRGP4 G_SpawnString
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 8
INDIRI4
ASGNI4
line 50
;50:	*out = atof( s );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 atof
CALLF4
ASGNF4
ADDRFP4 8
INDIRP4
ADDRLP4 12
INDIRF4
ASGNF4
line 51
;51:	return present;
ADDRLP4 4
INDIRI4
RETI4
LABELV $66
endproc G_SpawnFloat 16 12
export G_SpawnInt
proc G_SpawnInt 16 12
line 54
;52:}
;53:
;54:qboolean	G_SpawnInt( const char *key, const char *defaultString, int *out ) {
line 58
;55:	char		*s;
;56:	qboolean	present;
;57:
;58:	present = G_SpawnString( key, defaultString, &s );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 8
ADDRGP4 G_SpawnString
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 8
INDIRI4
ASGNI4
line 59
;59:	*out = atoi( s );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRFP4 8
INDIRP4
ADDRLP4 12
INDIRI4
ASGNI4
line 60
;60:	return present;
ADDRLP4 4
INDIRI4
RETI4
LABELV $67
endproc G_SpawnInt 16 12
export G_SpawnVector
proc G_SpawnVector 16 20
line 63
;61:}
;62:
;63:qboolean	G_SpawnVector( const char *key, const char *defaultString, float *out ) {
line 67
;64:	char		*s;
;65:	qboolean	present;
;66:
;67:	present = G_SpawnString( key, defaultString, &s );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 8
ADDRGP4 G_SpawnString
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 8
INDIRI4
ASGNI4
line 68
;68:	sscanf( s, "%f %f %f", &out[0], &out[1], &out[2] );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $69
ARGP4
ADDRLP4 12
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRLP4 12
INDIRP4
CNSTI4 8
ADDP4
ARGP4
ADDRGP4 sscanf
CALLI4
pop
line 69
;69:	return present;
ADDRLP4 4
INDIRI4
RETI4
LABELV $68
endproc G_SpawnVector 16 20
data
export fields
align 4
LABELV fields
address $72
byte 4 524
byte 4 2
skip 4
address $73
byte 4 92
byte 4 4
skip 4
address $74
byte 4 540
byte 4 2
skip 4
address $75
byte 4 544
byte 4 2
skip 4
address $76
byte 4 528
byte 4 0
skip 4
address $77
byte 4 672
byte 4 1
skip 4
address $78
byte 4 648
byte 4 2
skip 4
address $79
byte 4 652
byte 4 2
skip 4
address $80
byte 4 636
byte 4 2
skip 4
address $81
byte 4 656
byte 4 2
skip 4
address $82
byte 4 796
byte 4 1
skip 4
address $83
byte 4 800
byte 4 1
skip 4
address $84
byte 4 760
byte 4 0
skip 4
address $85
byte 4 732
byte 4 0
skip 4
address $86
byte 4 0
byte 4 9
skip 4
address $87
byte 4 740
byte 4 0
skip 4
address $88
byte 4 116
byte 4 4
skip 4
address $89
byte 4 116
byte 4 5
skip 4
address $90
byte 4 660
byte 4 2
skip 4
address $91
byte 4 664
byte 4 2
skip 4
byte 4 0
skip 12
export SP_item_botroam
code
proc SP_item_botroam 0 0
line 194
;70:}
;71:
;72:
;73:
;74://
;75:// fields are needed for spawning from the entity string
;76://
;77:typedef enum {
;78:	F_INT, 
;79:	F_FLOAT,
;80:	F_LSTRING,			// string on disk, pointer in memory, TAG_LEVEL
;81:	F_GSTRING,			// string on disk, pointer in memory, TAG_GAME
;82:	F_VECTOR,
;83:	F_ANGLEHACK,
;84:	F_ENTITY,			// index on disk, pointer in memory
;85:	F_ITEM,				// index on disk, pointer in memory
;86:	F_CLIENT,			// index on disk, pointer in memory
;87:	F_IGNORE
;88:} fieldtype_t;
;89:
;90:typedef struct
;91:{
;92:	char	*name;
;93:	int		ofs;
;94:	fieldtype_t	type;
;95:	int		flags;
;96:} field_t;
;97:
;98:field_t fields[] = {
;99:	{"classname", FOFS(classname), F_LSTRING},
;100:	{"origin", FOFS(s.origin), F_VECTOR},
;101:	{"model", FOFS(model), F_LSTRING},
;102:	{"model2", FOFS(model2), F_LSTRING},
;103:	{"spawnflags", FOFS(spawnflags), F_INT},
;104:	{"speed", FOFS(speed), F_FLOAT},
;105:	{"target", FOFS(target), F_LSTRING},
;106:	{"targetname", FOFS(targetname), F_LSTRING},
;107:	{"message", FOFS(message), F_LSTRING},
;108:	{"team", FOFS(team), F_LSTRING},
;109:	{"wait", FOFS(wait), F_FLOAT},
;110:	{"random", FOFS(random), F_FLOAT},
;111:	{"count", FOFS(count), F_INT},
;112:	{"health", FOFS(health), F_INT},
;113:	{"light", 0, F_IGNORE},
;114:	{"dmg", FOFS(damage), F_INT},
;115:	{"angles", FOFS(s.angles), F_VECTOR},
;116:	{"angle", FOFS(s.angles), F_ANGLEHACK},
;117:	{"targetShaderName", FOFS(targetShaderName), F_LSTRING},
;118:	{"targetShaderNewName", FOFS(targetShaderNewName), F_LSTRING},
;119:
;120:	{NULL}
;121:};
;122:
;123:
;124:typedef struct {
;125:	char	*name;
;126:	void	(*spawn)(gentity_t *ent);
;127:} spawn_t;
;128:
;129:void SP_info_player_start (gentity_t *ent);
;130:void SP_info_player_deathmatch (gentity_t *ent);
;131:void SP_info_player_intermission (gentity_t *ent);
;132:void SP_info_firstplace(gentity_t *ent);
;133:void SP_info_secondplace(gentity_t *ent);
;134:void SP_info_thirdplace(gentity_t *ent);
;135:void SP_info_podium(gentity_t *ent);
;136:
;137:void SP_func_plat (gentity_t *ent);
;138:void SP_func_static (gentity_t *ent);
;139:void SP_func_rotating (gentity_t *ent);
;140:void SP_func_bobbing (gentity_t *ent);
;141:void SP_func_pendulum( gentity_t *ent );
;142:void SP_func_button (gentity_t *ent);
;143:void SP_func_door (gentity_t *ent);
;144:void SP_func_train (gentity_t *ent);
;145:void SP_func_timer (gentity_t *self);
;146:
;147:void SP_trigger_always (gentity_t *ent);
;148:void SP_trigger_multiple (gentity_t *ent);
;149:void SP_trigger_push (gentity_t *ent);
;150:void SP_trigger_teleport (gentity_t *ent);
;151:void SP_trigger_hurt (gentity_t *ent);
;152:
;153:void SP_target_remove_powerups( gentity_t *ent );
;154:void SP_target_give (gentity_t *ent);
;155:void SP_target_delay (gentity_t *ent);
;156:void SP_target_speaker (gentity_t *ent);
;157:void SP_target_print (gentity_t *ent);
;158:void SP_target_laser (gentity_t *self);
;159:void SP_target_character (gentity_t *ent);
;160:void SP_target_score( gentity_t *ent );
;161:void SP_target_teleporter( gentity_t *ent );
;162:void SP_target_relay (gentity_t *ent);
;163:void SP_target_kill (gentity_t *ent);
;164:void SP_target_position (gentity_t *ent);
;165:void SP_target_location (gentity_t *ent);
;166:void SP_target_push (gentity_t *ent);
;167:
;168:void SP_light (gentity_t *self);
;169:void SP_info_null (gentity_t *self);
;170:void SP_info_notnull (gentity_t *self);
;171:void SP_info_camp (gentity_t *self);
;172:void SP_path_corner (gentity_t *self);
;173:
;174:void SP_misc_teleporter_dest (gentity_t *self);
;175:void SP_misc_model(gentity_t *ent);
;176:void SP_misc_portal_camera(gentity_t *ent);
;177:void SP_misc_portal_surface(gentity_t *ent);
;178:
;179:void SP_shooter_rocket( gentity_t *ent );
;180:void SP_shooter_plasma( gentity_t *ent );
;181:void SP_shooter_grenade( gentity_t *ent );
;182:
;183:void SP_team_CTF_redplayer( gentity_t *ent );
;184:void SP_team_CTF_blueplayer( gentity_t *ent );
;185:
;186:void SP_team_CTF_redspawn( gentity_t *ent );
;187:void SP_team_CTF_bluespawn( gentity_t *ent );
;188:
;189:#ifdef MISSIONPACK
;190:void SP_team_blueobelisk( gentity_t *ent );
;191:void SP_team_redobelisk( gentity_t *ent );
;192:void SP_team_neutralobelisk( gentity_t *ent );
;193:#endif
;194:void SP_item_botroam( gentity_t *ent ) {};
LABELV $93
endproc SP_item_botroam 0 0
data
export spawns
align 4
LABELV spawns
address $94
address SP_info_player_start
address $95
address SP_info_player_deathmatch
address $96
address SP_info_player_intermission
address $97
address SP_info_null
address $98
address SP_info_notnull
address $99
address SP_info_camp
address $100
address SP_func_plat
address $101
address SP_func_button
address $102
address SP_func_door
address $103
address SP_func_static
address $104
address SP_func_rotating
address $105
address SP_func_bobbing
address $106
address SP_func_pendulum
address $107
address SP_func_train
address $108
address SP_info_null
address $109
address SP_func_timer
address $110
address SP_trigger_always
address $111
address SP_trigger_multiple
address $112
address SP_trigger_push
address $113
address SP_trigger_teleport
address $114
address SP_trigger_hurt
address $115
address SP_target_give
address $116
address SP_target_remove_powerups
address $117
address SP_target_delay
address $118
address SP_target_speaker
address $119
address SP_target_print
address $120
address SP_target_laser
address $121
address SP_target_score
address $122
address SP_target_teleporter
address $123
address SP_target_relay
address $124
address SP_target_kill
address $125
address SP_target_position
address $126
address SP_target_location
address $127
address SP_target_push
address $86
address SP_light
address $128
address SP_path_corner
address $129
address SP_misc_teleporter_dest
address $130
address SP_misc_model
address $131
address SP_misc_portal_surface
address $132
address SP_misc_portal_camera
address $133
address SP_shooter_rocket
address $134
address SP_shooter_grenade
address $135
address SP_shooter_plasma
address $136
address SP_team_CTF_redplayer
address $137
address SP_team_CTF_blueplayer
address $138
address SP_team_CTF_redspawn
address $139
address SP_team_CTF_bluespawn
address $140
address SP_item_botroam
byte 4 0
byte 4 0
export G_CallSpawn
code
proc G_CallSpawn 12 8
line 280
;195:
;196:spawn_t	spawns[] = {
;197:	// info entities don't do anything at all, but provide positional
;198:	// information for things controlled by other processes
;199:	{"info_player_start", SP_info_player_start},
;200:	{"info_player_deathmatch", SP_info_player_deathmatch},
;201:	{"info_player_intermission", SP_info_player_intermission},
;202:	{"info_null", SP_info_null},
;203:	{"info_notnull", SP_info_notnull},		// use target_position instead
;204:	{"info_camp", SP_info_camp},
;205:
;206:	{"func_plat", SP_func_plat},
;207:	{"func_button", SP_func_button},
;208:	{"func_door", SP_func_door},
;209:	{"func_static", SP_func_static},
;210:	{"func_rotating", SP_func_rotating},
;211:	{"func_bobbing", SP_func_bobbing},
;212:	{"func_pendulum", SP_func_pendulum},
;213:	{"func_train", SP_func_train},
;214:	{"func_group", SP_info_null},
;215:	{"func_timer", SP_func_timer},			// rename trigger_timer?
;216:
;217:	// Triggers are brush objects that cause an effect when contacted
;218:	// by a living player, usually involving firing targets.
;219:	// While almost everything could be done with
;220:	// a single trigger class and different targets, triggered effects
;221:	// could not be client side predicted (push and teleport).
;222:	{"trigger_always", SP_trigger_always},
;223:	{"trigger_multiple", SP_trigger_multiple},
;224:	{"trigger_push", SP_trigger_push},
;225:	{"trigger_teleport", SP_trigger_teleport},
;226:	{"trigger_hurt", SP_trigger_hurt},
;227:
;228:	// targets perform no action by themselves, but must be triggered
;229:	// by another entity
;230:	{"target_give", SP_target_give},
;231:	{"target_remove_powerups", SP_target_remove_powerups},
;232:	{"target_delay", SP_target_delay},
;233:	{"target_speaker", SP_target_speaker},
;234:	{"target_print", SP_target_print},
;235:	{"target_laser", SP_target_laser},
;236:	{"target_score", SP_target_score},
;237:	{"target_teleporter", SP_target_teleporter},
;238:	{"target_relay", SP_target_relay},
;239:	{"target_kill", SP_target_kill},
;240:	{"target_position", SP_target_position},
;241:	{"target_location", SP_target_location},
;242:	{"target_push", SP_target_push},
;243:
;244:	{"light", SP_light},
;245:	{"path_corner", SP_path_corner},
;246:
;247:	{"misc_teleporter_dest", SP_misc_teleporter_dest},
;248:	{"misc_model", SP_misc_model},
;249:	{"misc_portal_surface", SP_misc_portal_surface},
;250:	{"misc_portal_camera", SP_misc_portal_camera},
;251:
;252:	{"shooter_rocket", SP_shooter_rocket},
;253:	{"shooter_grenade", SP_shooter_grenade},
;254:	{"shooter_plasma", SP_shooter_plasma},
;255:
;256:	{"team_CTF_redplayer", SP_team_CTF_redplayer},
;257:	{"team_CTF_blueplayer", SP_team_CTF_blueplayer},
;258:
;259:	{"team_CTF_redspawn", SP_team_CTF_redspawn},
;260:	{"team_CTF_bluespawn", SP_team_CTF_bluespawn},
;261:
;262:#ifdef MISSIONPACK
;263:	{"team_redobelisk", SP_team_redobelisk},
;264:	{"team_blueobelisk", SP_team_blueobelisk},
;265:	{"team_neutralobelisk", SP_team_neutralobelisk},
;266:#endif
;267:	{"item_botroam", SP_item_botroam},
;268:
;269:	{0, 0}
;270:};
;271:
;272:/*
;273:===============
;274:G_CallSpawn
;275:
;276:Finds the spawn function for the entity and calls it,
;277:returning qfalse if not found
;278:===============
;279:*/
;280:qboolean G_CallSpawn( gentity_t *ent ) {
line 284
;281:	spawn_t	*s;
;282:	gitem_t	*item;
;283:
;284:	if ( !ent->classname ) {
ADDRFP4 0
INDIRP4
CNSTI4 524
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $142
line 285
;285:		G_Printf ("G_CallSpawn: NULL classname\n");
ADDRGP4 $144
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 286
;286:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $141
JUMPV
LABELV $142
line 290
;287:	}
;288:
;289:	// check item spawn functions
;290:	for ( item=bg_itemlist+1 ; item->classname ; item++ ) {
ADDRLP4 4
ADDRGP4 bg_itemlist+52
ASGNP4
ADDRGP4 $148
JUMPV
LABELV $145
line 291
;291:		if ( !strcmp(item->classname, ent->classname) ) {
ADDRLP4 4
INDIRP4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 524
ADDP4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $150
line 292
;292:			G_SpawnItem( ent, item );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 G_SpawnItem
CALLV
pop
line 293
;293:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $141
JUMPV
LABELV $150
line 295
;294:		}
;295:	}
LABELV $146
line 290
ADDRLP4 4
ADDRLP4 4
INDIRP4
CNSTI4 52
ADDP4
ASGNP4
LABELV $148
ADDRLP4 4
INDIRP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $145
line 298
;296:
;297:	// check normal spawn functions
;298:	for ( s=spawns ; s->name ; s++ ) {
ADDRLP4 0
ADDRGP4 spawns
ASGNP4
ADDRGP4 $155
JUMPV
LABELV $152
line 299
;299:		if ( !strcmp(s->name, ent->classname) ) {
ADDRLP4 0
INDIRP4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 524
ADDP4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $156
line 301
;300:			// found it
;301:			s->spawn(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CALLV
pop
line 302
;302:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $141
JUMPV
LABELV $156
line 304
;303:		}
;304:	}
LABELV $153
line 298
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
LABELV $155
ADDRLP4 0
INDIRP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $152
line 305
;305:	G_Printf ("%s doesn't have a spawn function\n", ent->classname);
ADDRGP4 $158
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 524
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 306
;306:	return qfalse;
CNSTI4 0
RETI4
LABELV $141
endproc G_CallSpawn 12 8
export G_NewString
proc G_NewString 32 4
line 317
;307:}
;308:
;309:/*
;310:=============
;311:G_NewString
;312:
;313:Builds a copy of the string, translating \n to real linefeeds
;314:so message texts can be multi-line
;315:=============
;316:*/
;317:char *G_NewString( const char *string ) {
line 321
;318:	char	*newb, *new_p;
;319:	int		i,l;
;320:	
;321:	l = strlen(string) + 1;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 323
;322:
;323:	newb = G_Alloc( l );
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 G_Alloc
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 20
INDIRP4
ASGNP4
line 325
;324:
;325:	new_p = newb;
ADDRLP4 8
ADDRLP4 12
INDIRP4
ASGNP4
line 328
;326:
;327:	// turn \n into a real linefeed
;328:	for ( i=0 ; i< l ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $163
JUMPV
LABELV $160
line 329
;329:		if (string[i] == '\\' && i < l-1) {
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $164
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
GEI4 $164
line 330
;330:			i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 331
;331:			if (string[i] == 'n') {
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 110
NEI4 $166
line 332
;332:				*new_p++ = '\n';
ADDRLP4 28
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 28
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI1 10
ASGNI1
line 333
;333:			} else {
ADDRGP4 $165
JUMPV
LABELV $166
line 334
;334:				*new_p++ = '\\';
ADDRLP4 28
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 28
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI1 92
ASGNI1
line 335
;335:			}
line 336
;336:		} else {
ADDRGP4 $165
JUMPV
LABELV $164
line 337
;337:			*new_p++ = string[i];
ADDRLP4 28
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 28
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
ASGNI1
line 338
;338:		}
LABELV $165
line 339
;339:	}
LABELV $161
line 328
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $163
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $160
line 341
;340:	
;341:	return newb;
ADDRLP4 12
INDIRP4
RETP4
LABELV $159
endproc G_NewString 32 4
export G_ParseField
proc G_ParseField 60 20
line 355
;342:}
;343:
;344:
;345:
;346:
;347:/*
;348:===============
;349:G_ParseField
;350:
;351:Takes a key/value pair and sets the binary values
;352:in a gentity
;353:===============
;354:*/
;355:void G_ParseField( const char *key, const char *value, gentity_t *ent ) {
line 361
;356:	field_t	*f;
;357:	byte	*b;
;358:	float	v;
;359:	vec3_t	vec;
;360:
;361:	for ( f=fields ; f->name ; f++ ) {
ADDRLP4 0
ADDRGP4 fields
ASGNP4
ADDRGP4 $172
JUMPV
LABELV $169
line 362
;362:		if ( !Q_stricmp(f->name, key) ) {
ADDRLP4 0
INDIRP4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $173
line 364
;363:			// found it
;364:			b = (byte *)ent;
ADDRLP4 4
ADDRFP4 8
INDIRP4
ASGNP4
line 366
;365:
;366:			switch( f->type ) {
ADDRLP4 28
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
LTI4 $168
ADDRLP4 28
INDIRI4
CNSTI4 9
GTI4 $168
ADDRLP4 28
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $188
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $188
address $184
address $185
address $178
address $168
address $179
address $186
address $168
address $168
address $168
address $168
code
LABELV $178
line 368
;367:			case F_LSTRING:
;368:				*(char **)(b+f->ofs) = G_NewString (value);
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 G_NewString
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
ADDP4
ADDRLP4 36
INDIRP4
ASGNP4
line 369
;369:				break;
ADDRGP4 $168
JUMPV
LABELV $179
line 371
;370:			case F_VECTOR:
;371:				sscanf (value, "%f %f %f", &vec[0], &vec[1], &vec[2]);
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $69
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 8+4
ARGP4
ADDRLP4 8+8
ARGP4
ADDRGP4 sscanf
CALLI4
pop
line 372
;372:				((float *)(b+f->ofs))[0] = vec[0];
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
ADDP4
ADDRLP4 8
INDIRF4
ASGNF4
line 373
;373:				((float *)(b+f->ofs))[1] = vec[1];
ADDRLP4 40
CNSTI4 4
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
ADDP4
ADDRLP4 40
INDIRI4
ADDP4
ADDRLP4 8+4
INDIRF4
ASGNF4
line 374
;374:				((float *)(b+f->ofs))[2] = vec[2];
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
ADDP4
CNSTI4 8
ADDP4
ADDRLP4 8+8
INDIRF4
ASGNF4
line 375
;375:				break;
ADDRGP4 $168
JUMPV
LABELV $184
line 377
;376:			case F_INT:
;377:				*(int *)(b+f->ofs) = atoi(value);
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 44
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
ADDP4
ADDRLP4 44
INDIRI4
ASGNI4
line 378
;378:				break;
ADDRGP4 $168
JUMPV
LABELV $185
line 380
;379:			case F_FLOAT:
;380:				*(float *)(b+f->ofs) = atof(value);
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 atof
CALLF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
ADDP4
ADDRLP4 48
INDIRF4
ASGNF4
line 381
;381:				break;
ADDRGP4 $168
JUMPV
LABELV $186
line 383
;382:			case F_ANGLEHACK:
;383:				v = atof(value);
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 atof
CALLF4
ASGNF4
ADDRLP4 20
ADDRLP4 52
INDIRF4
ASGNF4
line 384
;384:				((float *)(b+f->ofs))[0] = 0;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
ADDP4
CNSTF4 0
ASGNF4
line 385
;385:				((float *)(b+f->ofs))[1] = v;
ADDRLP4 56
CNSTI4 4
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
ADDP4
ADDRLP4 56
INDIRI4
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
line 386
;386:				((float *)(b+f->ofs))[2] = 0;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
ADDP4
CNSTI4 8
ADDP4
CNSTF4 0
ASGNF4
line 387
;387:				break;
line 390
;388:			default:
;389:			case F_IGNORE:
;390:				break;
line 392
;391:			}
;392:			return;
ADDRGP4 $168
JUMPV
LABELV $173
line 394
;393:		}
;394:	}
LABELV $170
line 361
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
LABELV $172
ADDRLP4 0
INDIRP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $169
line 395
;395:}
LABELV $168
endproc G_ParseField 60 20
data
align 4
LABELV $190
address $191
address $192
address $193
address $81
address $194
address $195
address $196
address $197
address $198
export G_SpawnGEntityFromSpawnVars
code
proc G_SpawnGEntityFromSpawnVars 40 12
line 408
;396:
;397:
;398:
;399:
;400:/*
;401:===================
;402:G_SpawnGEntityFromSpawnVars
;403:
;404:Spawn an entity and fill in all of the level fields from
;405:level.spawnVars[], then call the class specfic spawn function
;406:===================
;407:*/
;408:void G_SpawnGEntityFromSpawnVars( void ) {
line 415
;409:	int			i;
;410:	gentity_t	*ent;
;411:	char		*s, *value, *gametypeName;
;412:	static char *gametypeNames[] = {"ffa", "tournament", "single", "team", "ctf", "oneflag", "obelisk", "harvester", "teamtournament"};
;413:
;414:	// get the next free entity
;415:	ent = G_Spawn();
ADDRLP4 20
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 20
INDIRP4
ASGNP4
line 417
;416:
;417:	for ( i = 0 ; i < level.numSpawnVars ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $202
JUMPV
LABELV $199
line 418
;418:		G_ParseField( level.spawnVars[i][0], level.spawnVars[i][1], ent );
ADDRLP4 24
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ASGNI4
ADDRLP4 24
INDIRI4
ADDRGP4 level+4512
ADDP4
INDIRP4
ARGP4
ADDRLP4 24
INDIRI4
ADDRGP4 level+4512+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 G_ParseField
CALLV
pop
line 419
;419:	}
LABELV $200
line 417
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $202
ADDRLP4 0
INDIRI4
ADDRGP4 level+4508
INDIRI4
LTI4 $199
line 422
;420:
;421:	// check for "notsingle" flag
;422:	if ( g_gametype.integer == GT_SINGLE_PLAYER ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
NEI4 $207
line 423
;423:		G_SpawnInt( "notsingle", "0", &i );
ADDRGP4 $210
ARGP4
ADDRGP4 $211
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 424
;424:		if ( i ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $212
line 425
;425:			G_FreeEntity( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 426
;426:			return;
ADDRGP4 $189
JUMPV
LABELV $212
line 428
;427:		}
;428:	}
LABELV $207
line 430
;429:	// check for "notteam" flag (GT_FFA, GT_TOURNAMENT, GT_SINGLE_PLAYER)
;430:	if ( g_gametype.integer >= GT_TEAM ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
LTI4 $214
line 431
;431:		G_SpawnInt( "notteam", "0", &i );
ADDRGP4 $217
ARGP4
ADDRGP4 $211
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 432
;432:		if ( i ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $215
line 433
;433:			G_FreeEntity( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 434
;434:			return;
ADDRGP4 $189
JUMPV
line 436
;435:		}
;436:	} else {
LABELV $214
line 437
;437:		G_SpawnInt( "notfree", "0", &i );
ADDRGP4 $220
ARGP4
ADDRGP4 $211
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 438
;438:		if ( i ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $221
line 439
;439:			G_FreeEntity( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 440
;440:			return;
ADDRGP4 $189
JUMPV
LABELV $221
line 442
;441:		}
;442:	}
LABELV $215
line 451
;443:
;444:#ifdef MISSIONPACK
;445:	G_SpawnInt( "notta", "0", &i );
;446:	if ( i ) {
;447:		G_FreeEntity( ent );
;448:		return;
;449:	}
;450:#else
;451:	G_SpawnInt( "notq3a", "0", &i );
ADDRGP4 $223
ARGP4
ADDRGP4 $211
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 452
;452:	if ( i ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $224
line 453
;453:		G_FreeEntity( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 454
;454:		return;
ADDRGP4 $189
JUMPV
LABELV $224
line 458
;455:	}
;456:#endif
;457:
;458:	if( G_SpawnString( "gametype", NULL, &value ) ) {
ADDRGP4 $228
ARGP4
CNSTP4 0
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 24
ADDRGP4 G_SpawnString
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $226
line 459
;459:		if( g_gametype.integer >= GT_FFA && g_gametype.integer < GT_MAX_GAME_TYPE ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 0
LTI4 $229
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 8
GEI4 $229
line 460
;460:			gametypeName = gametypeNames[g_gametype.integer];
ADDRLP4 16
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $190
ADDP4
INDIRP4
ASGNP4
line 462
;461:
;462:			s = strstr( value, gametypeName );
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 28
INDIRP4
ASGNP4
line 463
;463:			if( !s ) {
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $234
line 464
;464:				G_FreeEntity( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 465
;465:				return;
ADDRGP4 $189
JUMPV
LABELV $234
line 467
;466:			}
;467:		}
LABELV $229
line 468
;468:	}
LABELV $226
line 471
;469:
;470:	// move editor origin to pos
;471:	VectorCopy( ent->s.origin, ent->s.pos.trBase );
ADDRLP4 4
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 472
;472:	VectorCopy( ent->s.origin, ent->r.currentOrigin );
ADDRLP4 4
INDIRP4
CNSTI4 488
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 475
;473:
;474:	// if we didn't get a classname, don't bother spawning anything
;475:	if ( !G_CallSpawn( ent ) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 G_CallSpawn
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
NEI4 $236
line 476
;476:		G_FreeEntity( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 477
;477:	}
LABELV $236
line 478
;478:}
LABELV $189
endproc G_SpawnGEntityFromSpawnVars 40 12
export G_AddSpawnVarToken
proc G_AddSpawnVarToken 16 12
line 487
;479:
;480:
;481:
;482:/*
;483:====================
;484:G_AddSpawnVarToken
;485:====================
;486:*/
;487:char *G_AddSpawnVarToken( const char *string ) {
line 491
;488:	int		l;
;489:	char	*dest;
;490:
;491:	l = strlen( string );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 492
;492:	if ( level.numSpawnVarChars + l + 1 > MAX_SPAWN_VARS_CHARS ) {
ADDRGP4 level+5024
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
CNSTI4 1
ADDI4
CNSTI4 4096
LEI4 $239
line 493
;493:		G_Error( "G_AddSpawnVarToken: MAX_SPAWN_CHARS" );
ADDRGP4 $242
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 494
;494:	}
LABELV $239
line 496
;495:
;496:	dest = level.spawnVarChars + level.numSpawnVarChars;
ADDRLP4 4
ADDRGP4 level+5024
INDIRI4
ADDRGP4 level+5028
ADDP4
ASGNP4
line 497
;497:	memcpy( dest, string, l+1 );
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 499
;498:
;499:	level.numSpawnVarChars += l + 1;
ADDRLP4 12
ADDRGP4 level+5024
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ADDI4
ASGNI4
line 501
;500:
;501:	return dest;
ADDRLP4 4
INDIRP4
RETP4
LABELV $238
endproc G_AddSpawnVarToken 16 12
export G_ParseSpawnVars
proc G_ParseSpawnVars 2072 8
line 514
;502:}
;503:
;504:/*
;505:====================
;506:G_ParseSpawnVars
;507:
;508:Parses a brace bounded set of key / value pairs out of the
;509:level's entity strings into level.spawnVars[]
;510:
;511:This does not actually spawn an entity.
;512:====================
;513:*/
;514:qboolean G_ParseSpawnVars( void ) {
line 518
;515:	char		keyname[MAX_TOKEN_CHARS];
;516:	char		com_token[MAX_TOKEN_CHARS];
;517:
;518:	level.numSpawnVars = 0;
ADDRGP4 level+4508
CNSTI4 0
ASGNI4
line 519
;519:	level.numSpawnVarChars = 0;
ADDRGP4 level+5024
CNSTI4 0
ASGNI4
line 522
;520:
;521:	// parse the opening brace
;522:	if ( !trap_GetEntityToken( com_token, sizeof( com_token ) ) ) {
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 2048
ADDRGP4 trap_GetEntityToken
CALLI4
ASGNI4
ADDRLP4 2048
INDIRI4
CNSTI4 0
NEI4 $249
line 524
;523:		// end of spawn string
;524:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $246
JUMPV
LABELV $249
line 526
;525:	}
;526:	if ( com_token[0] != '{' ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 123
EQI4 $255
line 527
;527:		G_Error( "G_ParseSpawnVars: found %s when expecting {",com_token );
ADDRGP4 $253
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 528
;528:	}
ADDRGP4 $255
JUMPV
LABELV $254
line 531
;529:
;530:	// go through all the key / value pairs
;531:	while ( 1 ) {	
line 533
;532:		// parse key
;533:		if ( !trap_GetEntityToken( keyname, sizeof( keyname ) ) ) {
ADDRLP4 1024
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 2052
ADDRGP4 trap_GetEntityToken
CALLI4
ASGNI4
ADDRLP4 2052
INDIRI4
CNSTI4 0
NEI4 $257
line 534
;534:			G_Error( "G_ParseSpawnVars: EOF without closing brace" );
ADDRGP4 $259
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 535
;535:		}
LABELV $257
line 537
;536:
;537:		if ( keyname[0] == '}' ) {
ADDRLP4 1024
INDIRI1
CVII4 1
CNSTI4 125
NEI4 $260
line 538
;538:			break;
ADDRGP4 $256
JUMPV
LABELV $260
line 542
;539:		}
;540:		
;541:		// parse value	
;542:		if ( !trap_GetEntityToken( com_token, sizeof( com_token ) ) ) {
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 2056
ADDRGP4 trap_GetEntityToken
CALLI4
ASGNI4
ADDRLP4 2056
INDIRI4
CNSTI4 0
NEI4 $262
line 543
;543:			G_Error( "G_ParseSpawnVars: EOF without closing brace" );
ADDRGP4 $259
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 544
;544:		}
LABELV $262
line 546
;545:
;546:		if ( com_token[0] == '}' ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 125
NEI4 $264
line 547
;547:			G_Error( "G_ParseSpawnVars: closing brace without data" );
ADDRGP4 $266
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 548
;548:		}
LABELV $264
line 549
;549:		if ( level.numSpawnVars == MAX_SPAWN_VARS ) {
ADDRGP4 level+4508
INDIRI4
CNSTI4 64
NEI4 $267
line 550
;550:			G_Error( "G_ParseSpawnVars: MAX_SPAWN_VARS" );
ADDRGP4 $270
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 551
;551:		}
LABELV $267
line 552
;552:		level.spawnVars[ level.numSpawnVars ][0] = G_AddSpawnVarToken( keyname );
ADDRLP4 1024
ARGP4
ADDRLP4 2060
ADDRGP4 G_AddSpawnVarToken
CALLP4
ASGNP4
ADDRGP4 level+4508
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 level+4512
ADDP4
ADDRLP4 2060
INDIRP4
ASGNP4
line 553
;553:		level.spawnVars[ level.numSpawnVars ][1] = G_AddSpawnVarToken( com_token );
ADDRLP4 0
ARGP4
ADDRLP4 2064
ADDRGP4 G_AddSpawnVarToken
CALLP4
ASGNP4
ADDRGP4 level+4508
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 level+4512+4
ADDP4
ADDRLP4 2064
INDIRP4
ASGNP4
line 554
;554:		level.numSpawnVars++;
ADDRLP4 2068
ADDRGP4 level+4508
ASGNP4
ADDRLP4 2068
INDIRP4
ADDRLP4 2068
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 555
;555:	}
LABELV $255
line 531
ADDRGP4 $254
JUMPV
LABELV $256
line 557
;556:
;557:	return qtrue;
CNSTI4 1
RETI4
LABELV $246
endproc G_ParseSpawnVars 2072 8
export SP_worldspawn
proc SP_worldspawn 16 12
line 569
;558:}
;559:
;560:
;561:
;562:/*QUAKED worldspawn (0 0 0) ?
;563:
;564:Every map should have exactly one worldspawn.
;565:"music"		music wav file
;566:"gravity"	800 is default gravity
;567:"message"	Text to print during connection process
;568:*/
;569:void SP_worldspawn( void ) {
line 572
;570:	char	*s;
;571:
;572:	G_SpawnString( "classname", "", &s );
ADDRGP4 $72
ARGP4
ADDRGP4 $278
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnString
CALLI4
pop
line 573
;573:	if ( Q_stricmp( s, "worldspawn" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $281
ARGP4
ADDRLP4 4
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $279
line 574
;574:		G_Error( "SP_worldspawn: The first entity isn't 'worldspawn'" );
ADDRGP4 $282
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 575
;575:	}
LABELV $279
line 578
;576:
;577:	// make some data visible to connecting client
;578:	trap_SetConfigstring( CS_GAME_VERSION, GAME_VERSION );
CNSTI4 20
ARGI4
ADDRGP4 $283
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 580
;579:
;580:	trap_SetConfigstring( CS_LEVEL_START_TIME, va("%i", level.startTime ) );
ADDRGP4 $284
ARGP4
ADDRGP4 level+40
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 21
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 582
;581:
;582:	G_SpawnString( "music", "", &s );
ADDRGP4 $286
ARGP4
ADDRGP4 $278
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnString
CALLI4
pop
line 583
;583:	trap_SetConfigstring( CS_MUSIC, s );
CNSTI4 2
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 585
;584:
;585:	G_SpawnString( "message", "", &s );
ADDRGP4 $80
ARGP4
ADDRGP4 $278
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnString
CALLI4
pop
line 586
;586:	trap_SetConfigstring( CS_MESSAGE, s );				// map specific message
CNSTI4 3
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 588
;587:
;588:	trap_SetConfigstring( CS_MOTD, g_motd.string );		// message of the day
CNSTI4 4
ARGI4
ADDRGP4 g_motd+16
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 590
;589:
;590:	G_SpawnString( "gravity", "800", &s );
ADDRGP4 $288
ARGP4
ADDRGP4 $289
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnString
CALLI4
pop
line 591
;591:	trap_Cvar_Set( "g_gravity", s );
ADDRGP4 $290
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 593
;592:
;593:	G_SpawnString( "enableDust", "0", &s );
ADDRGP4 $291
ARGP4
ADDRGP4 $211
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnString
CALLI4
pop
line 594
;594:	trap_Cvar_Set( "g_enableDust", s );
ADDRGP4 $292
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 596
;595:
;596:	G_SpawnString( "enableBreath", "0", &s );
ADDRGP4 $293
ARGP4
ADDRGP4 $211
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnString
CALLI4
pop
line 597
;597:	trap_Cvar_Set( "g_enableBreath", s );
ADDRGP4 $294
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 599
;598:
;599:	g_entities[ENTITYNUM_WORLD].s.number = ENTITYNUM_WORLD;
ADDRGP4 g_entities+825776
CNSTI4 1022
ASGNI4
line 600
;600:	g_entities[ENTITYNUM_WORLD].classname = "worldspawn";
ADDRGP4 g_entities+825776+524
ADDRGP4 $281
ASGNP4
line 603
;601:
;602:	// see if we want a warmup time
;603:	trap_SetConfigstring( CS_WARMUP, "" );
CNSTI4 5
ARGI4
ADDRGP4 $278
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 604
;604:	if ( g_restarted.integer ) {
ADDRGP4 g_restarted+12
INDIRI4
CNSTI4 0
EQI4 $298
line 605
;605:		trap_Cvar_Set( "g_restarted", "0" );
ADDRGP4 $301
ARGP4
ADDRGP4 $211
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 606
;606:		level.warmupTime = 0;
ADDRGP4 level+16
CNSTI4 0
ASGNI4
line 607
;607:	} else if ( g_doWarmup.integer ) { // Turn it on
ADDRGP4 $299
JUMPV
LABELV $298
ADDRGP4 g_doWarmup+12
INDIRI4
CNSTI4 0
EQI4 $303
line 608
;608:		level.warmupTime = -1;
ADDRGP4 level+16
CNSTI4 -1
ASGNI4
line 609
;609:		trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
ADDRGP4 $284
ARGP4
ADDRGP4 level+16
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 5
ARGI4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 610
;610:		G_LogPrintf( "Warmup:\n" );
ADDRGP4 $308
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 611
;611:	}
LABELV $303
LABELV $299
line 613
;612:
;613:}
LABELV $277
endproc SP_worldspawn 16 12
export G_SpawnEntitiesFromString
proc G_SpawnEntitiesFromString 8 4
line 623
;614:
;615:
;616:/*
;617:==============
;618:G_SpawnEntitiesFromString
;619:
;620:Parses textual entity definitions out of an entstring and spawns gentities.
;621:==============
;622:*/
;623:void G_SpawnEntitiesFromString( void ) {
line 625
;624:	// allow calls to G_Spawn*()
;625:	level.spawning = qtrue;
ADDRGP4 level+4504
CNSTI4 1
ASGNI4
line 626
;626:	level.numSpawnVars = 0;
ADDRGP4 level+4508
CNSTI4 0
ASGNI4
line 631
;627:
;628:	// the worldspawn is not an actual entity, but it still
;629:	// has a "spawn" function to perform any global setup
;630:	// needed by a level (setting configstrings or cvars, etc)
;631:	if ( !G_ParseSpawnVars() ) {
ADDRLP4 0
ADDRGP4 G_ParseSpawnVars
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $312
line 632
;632:		G_Error( "SpawnEntities: no entities" );
ADDRGP4 $314
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 633
;633:	}
LABELV $312
line 634
;634:	SP_worldspawn();
ADDRGP4 SP_worldspawn
CALLV
pop
ADDRGP4 $316
JUMPV
LABELV $315
line 637
;635:
;636:	// parse ents
;637:	while( G_ParseSpawnVars() ) {
line 638
;638:		G_SpawnGEntityFromSpawnVars();
ADDRGP4 G_SpawnGEntityFromSpawnVars
CALLV
pop
line 639
;639:	}	
LABELV $316
line 637
ADDRLP4 4
ADDRGP4 G_ParseSpawnVars
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $315
line 641
;640:
;641:	level.spawning = qfalse;			// any future calls to G_Spawn*() will be errors
ADDRGP4 level+4504
CNSTI4 0
ASGNI4
line 642
;642:}
LABELV $309
endproc G_SpawnEntitiesFromString 8 4
import SP_team_CTF_bluespawn
import SP_team_CTF_redspawn
import SP_team_CTF_blueplayer
import SP_team_CTF_redplayer
import SP_shooter_grenade
import SP_shooter_plasma
import SP_shooter_rocket
import SP_misc_portal_surface
import SP_misc_portal_camera
import SP_misc_model
import SP_misc_teleporter_dest
import SP_path_corner
import SP_info_camp
import SP_info_notnull
import SP_info_null
import SP_light
import SP_target_push
import SP_target_location
import SP_target_position
import SP_target_kill
import SP_target_relay
import SP_target_teleporter
import SP_target_score
import SP_target_character
import SP_target_laser
import SP_target_print
import SP_target_speaker
import SP_target_delay
import SP_target_give
import SP_target_remove_powerups
import SP_trigger_hurt
import SP_trigger_teleport
import SP_trigger_push
import SP_trigger_multiple
import SP_trigger_always
import SP_func_timer
import SP_func_train
import SP_func_door
import SP_func_button
import SP_func_pendulum
import SP_func_bobbing
import SP_func_rotating
import SP_func_static
import SP_func_plat
import SP_info_podium
import SP_info_thirdplace
import SP_info_secondplace
import SP_info_firstplace
import SP_info_player_intermission
import SP_info_player_deathmatch
import SP_info_player_start
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
import G_FilterPacket
import G_ProcessIPBans
import ConsoleCommand
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
LABELV $314
byte 1 83
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 69
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 105
byte 1 101
byte 1 115
byte 1 58
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 105
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $308
byte 1 87
byte 1 97
byte 1 114
byte 1 109
byte 1 117
byte 1 112
byte 1 58
byte 1 10
byte 1 0
align 1
LABELV $301
byte 1 103
byte 1 95
byte 1 114
byte 1 101
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $294
byte 1 103
byte 1 95
byte 1 101
byte 1 110
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 66
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 0
align 1
LABELV $293
byte 1 101
byte 1 110
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 66
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 0
align 1
LABELV $292
byte 1 103
byte 1 95
byte 1 101
byte 1 110
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 68
byte 1 117
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $291
byte 1 101
byte 1 110
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 68
byte 1 117
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $290
byte 1 103
byte 1 95
byte 1 103
byte 1 114
byte 1 97
byte 1 118
byte 1 105
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $289
byte 1 56
byte 1 48
byte 1 48
byte 1 0
align 1
LABELV $288
byte 1 103
byte 1 114
byte 1 97
byte 1 118
byte 1 105
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $286
byte 1 109
byte 1 117
byte 1 115
byte 1 105
byte 1 99
byte 1 0
align 1
LABELV $284
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $283
byte 1 98
byte 1 97
byte 1 115
byte 1 101
byte 1 113
byte 1 51
byte 1 45
byte 1 49
byte 1 0
align 1
LABELV $282
byte 1 83
byte 1 80
byte 1 95
byte 1 119
byte 1 111
byte 1 114
byte 1 108
byte 1 100
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 58
byte 1 32
byte 1 84
byte 1 104
byte 1 101
byte 1 32
byte 1 102
byte 1 105
byte 1 114
byte 1 115
byte 1 116
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 32
byte 1 105
byte 1 115
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 39
byte 1 119
byte 1 111
byte 1 114
byte 1 108
byte 1 100
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 39
byte 1 0
align 1
LABELV $281
byte 1 119
byte 1 111
byte 1 114
byte 1 108
byte 1 100
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 0
align 1
LABELV $278
byte 1 0
align 1
LABELV $270
byte 1 71
byte 1 95
byte 1 80
byte 1 97
byte 1 114
byte 1 115
byte 1 101
byte 1 83
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 86
byte 1 97
byte 1 114
byte 1 115
byte 1 58
byte 1 32
byte 1 77
byte 1 65
byte 1 88
byte 1 95
byte 1 83
byte 1 80
byte 1 65
byte 1 87
byte 1 78
byte 1 95
byte 1 86
byte 1 65
byte 1 82
byte 1 83
byte 1 0
align 1
LABELV $266
byte 1 71
byte 1 95
byte 1 80
byte 1 97
byte 1 114
byte 1 115
byte 1 101
byte 1 83
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 86
byte 1 97
byte 1 114
byte 1 115
byte 1 58
byte 1 32
byte 1 99
byte 1 108
byte 1 111
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 98
byte 1 114
byte 1 97
byte 1 99
byte 1 101
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 100
byte 1 97
byte 1 116
byte 1 97
byte 1 0
align 1
LABELV $259
byte 1 71
byte 1 95
byte 1 80
byte 1 97
byte 1 114
byte 1 115
byte 1 101
byte 1 83
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 86
byte 1 97
byte 1 114
byte 1 115
byte 1 58
byte 1 32
byte 1 69
byte 1 79
byte 1 70
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 99
byte 1 108
byte 1 111
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 98
byte 1 114
byte 1 97
byte 1 99
byte 1 101
byte 1 0
align 1
LABELV $253
byte 1 71
byte 1 95
byte 1 80
byte 1 97
byte 1 114
byte 1 115
byte 1 101
byte 1 83
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 86
byte 1 97
byte 1 114
byte 1 115
byte 1 58
byte 1 32
byte 1 102
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 119
byte 1 104
byte 1 101
byte 1 110
byte 1 32
byte 1 101
byte 1 120
byte 1 112
byte 1 101
byte 1 99
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 123
byte 1 0
align 1
LABELV $242
byte 1 71
byte 1 95
byte 1 65
byte 1 100
byte 1 100
byte 1 83
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 86
byte 1 97
byte 1 114
byte 1 84
byte 1 111
byte 1 107
byte 1 101
byte 1 110
byte 1 58
byte 1 32
byte 1 77
byte 1 65
byte 1 88
byte 1 95
byte 1 83
byte 1 80
byte 1 65
byte 1 87
byte 1 78
byte 1 95
byte 1 67
byte 1 72
byte 1 65
byte 1 82
byte 1 83
byte 1 0
align 1
LABELV $228
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $223
byte 1 110
byte 1 111
byte 1 116
byte 1 113
byte 1 51
byte 1 97
byte 1 0
align 1
LABELV $220
byte 1 110
byte 1 111
byte 1 116
byte 1 102
byte 1 114
byte 1 101
byte 1 101
byte 1 0
align 1
LABELV $217
byte 1 110
byte 1 111
byte 1 116
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $211
byte 1 48
byte 1 0
align 1
LABELV $210
byte 1 110
byte 1 111
byte 1 116
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $198
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 116
byte 1 111
byte 1 117
byte 1 114
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $197
byte 1 104
byte 1 97
byte 1 114
byte 1 118
byte 1 101
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $196
byte 1 111
byte 1 98
byte 1 101
byte 1 108
byte 1 105
byte 1 115
byte 1 107
byte 1 0
align 1
LABELV $195
byte 1 111
byte 1 110
byte 1 101
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $194
byte 1 99
byte 1 116
byte 1 102
byte 1 0
align 1
LABELV $193
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $192
byte 1 116
byte 1 111
byte 1 117
byte 1 114
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $191
byte 1 102
byte 1 102
byte 1 97
byte 1 0
align 1
LABELV $158
byte 1 37
byte 1 115
byte 1 32
byte 1 100
byte 1 111
byte 1 101
byte 1 115
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 104
byte 1 97
byte 1 118
byte 1 101
byte 1 32
byte 1 97
byte 1 32
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 32
byte 1 102
byte 1 117
byte 1 110
byte 1 99
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 10
byte 1 0
align 1
LABELV $144
byte 1 71
byte 1 95
byte 1 67
byte 1 97
byte 1 108
byte 1 108
byte 1 83
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 58
byte 1 32
byte 1 78
byte 1 85
byte 1 76
byte 1 76
byte 1 32
byte 1 99
byte 1 108
byte 1 97
byte 1 115
byte 1 115
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $140
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 95
byte 1 98
byte 1 111
byte 1 116
byte 1 114
byte 1 111
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $139
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 67
byte 1 84
byte 1 70
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 0
align 1
LABELV $138
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 67
byte 1 84
byte 1 70
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 0
align 1
LABELV $137
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 67
byte 1 84
byte 1 70
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $136
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 67
byte 1 84
byte 1 70
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $135
byte 1 115
byte 1 104
byte 1 111
byte 1 111
byte 1 116
byte 1 101
byte 1 114
byte 1 95
byte 1 112
byte 1 108
byte 1 97
byte 1 115
byte 1 109
byte 1 97
byte 1 0
align 1
LABELV $134
byte 1 115
byte 1 104
byte 1 111
byte 1 111
byte 1 116
byte 1 101
byte 1 114
byte 1 95
byte 1 103
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 100
byte 1 101
byte 1 0
align 1
LABELV $133
byte 1 115
byte 1 104
byte 1 111
byte 1 111
byte 1 116
byte 1 101
byte 1 114
byte 1 95
byte 1 114
byte 1 111
byte 1 99
byte 1 107
byte 1 101
byte 1 116
byte 1 0
align 1
LABELV $132
byte 1 109
byte 1 105
byte 1 115
byte 1 99
byte 1 95
byte 1 112
byte 1 111
byte 1 114
byte 1 116
byte 1 97
byte 1 108
byte 1 95
byte 1 99
byte 1 97
byte 1 109
byte 1 101
byte 1 114
byte 1 97
byte 1 0
align 1
LABELV $131
byte 1 109
byte 1 105
byte 1 115
byte 1 99
byte 1 95
byte 1 112
byte 1 111
byte 1 114
byte 1 116
byte 1 97
byte 1 108
byte 1 95
byte 1 115
byte 1 117
byte 1 114
byte 1 102
byte 1 97
byte 1 99
byte 1 101
byte 1 0
align 1
LABELV $130
byte 1 109
byte 1 105
byte 1 115
byte 1 99
byte 1 95
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $129
byte 1 109
byte 1 105
byte 1 115
byte 1 99
byte 1 95
byte 1 116
byte 1 101
byte 1 108
byte 1 101
byte 1 112
byte 1 111
byte 1 114
byte 1 116
byte 1 101
byte 1 114
byte 1 95
byte 1 100
byte 1 101
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $128
byte 1 112
byte 1 97
byte 1 116
byte 1 104
byte 1 95
byte 1 99
byte 1 111
byte 1 114
byte 1 110
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $127
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 112
byte 1 117
byte 1 115
byte 1 104
byte 1 0
align 1
LABELV $126
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 108
byte 1 111
byte 1 99
byte 1 97
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $125
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 112
byte 1 111
byte 1 115
byte 1 105
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $124
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $123
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 114
byte 1 101
byte 1 108
byte 1 97
byte 1 121
byte 1 0
align 1
LABELV $122
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 116
byte 1 101
byte 1 108
byte 1 101
byte 1 112
byte 1 111
byte 1 114
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $121
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $120
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 108
byte 1 97
byte 1 115
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $119
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $118
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 115
byte 1 112
byte 1 101
byte 1 97
byte 1 107
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $117
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 100
byte 1 101
byte 1 108
byte 1 97
byte 1 121
byte 1 0
align 1
LABELV $116
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 114
byte 1 101
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 95
byte 1 112
byte 1 111
byte 1 119
byte 1 101
byte 1 114
byte 1 117
byte 1 112
byte 1 115
byte 1 0
align 1
LABELV $115
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 103
byte 1 105
byte 1 118
byte 1 101
byte 1 0
align 1
LABELV $114
byte 1 116
byte 1 114
byte 1 105
byte 1 103
byte 1 103
byte 1 101
byte 1 114
byte 1 95
byte 1 104
byte 1 117
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $113
byte 1 116
byte 1 114
byte 1 105
byte 1 103
byte 1 103
byte 1 101
byte 1 114
byte 1 95
byte 1 116
byte 1 101
byte 1 108
byte 1 101
byte 1 112
byte 1 111
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $112
byte 1 116
byte 1 114
byte 1 105
byte 1 103
byte 1 103
byte 1 101
byte 1 114
byte 1 95
byte 1 112
byte 1 117
byte 1 115
byte 1 104
byte 1 0
align 1
LABELV $111
byte 1 116
byte 1 114
byte 1 105
byte 1 103
byte 1 103
byte 1 101
byte 1 114
byte 1 95
byte 1 109
byte 1 117
byte 1 108
byte 1 116
byte 1 105
byte 1 112
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $110
byte 1 116
byte 1 114
byte 1 105
byte 1 103
byte 1 103
byte 1 101
byte 1 114
byte 1 95
byte 1 97
byte 1 108
byte 1 119
byte 1 97
byte 1 121
byte 1 115
byte 1 0
align 1
LABELV $109
byte 1 102
byte 1 117
byte 1 110
byte 1 99
byte 1 95
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $108
byte 1 102
byte 1 117
byte 1 110
byte 1 99
byte 1 95
byte 1 103
byte 1 114
byte 1 111
byte 1 117
byte 1 112
byte 1 0
align 1
LABELV $107
byte 1 102
byte 1 117
byte 1 110
byte 1 99
byte 1 95
byte 1 116
byte 1 114
byte 1 97
byte 1 105
byte 1 110
byte 1 0
align 1
LABELV $106
byte 1 102
byte 1 117
byte 1 110
byte 1 99
byte 1 95
byte 1 112
byte 1 101
byte 1 110
byte 1 100
byte 1 117
byte 1 108
byte 1 117
byte 1 109
byte 1 0
align 1
LABELV $105
byte 1 102
byte 1 117
byte 1 110
byte 1 99
byte 1 95
byte 1 98
byte 1 111
byte 1 98
byte 1 98
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $104
byte 1 102
byte 1 117
byte 1 110
byte 1 99
byte 1 95
byte 1 114
byte 1 111
byte 1 116
byte 1 97
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $103
byte 1 102
byte 1 117
byte 1 110
byte 1 99
byte 1 95
byte 1 115
byte 1 116
byte 1 97
byte 1 116
byte 1 105
byte 1 99
byte 1 0
align 1
LABELV $102
byte 1 102
byte 1 117
byte 1 110
byte 1 99
byte 1 95
byte 1 100
byte 1 111
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $101
byte 1 102
byte 1 117
byte 1 110
byte 1 99
byte 1 95
byte 1 98
byte 1 117
byte 1 116
byte 1 116
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $100
byte 1 102
byte 1 117
byte 1 110
byte 1 99
byte 1 95
byte 1 112
byte 1 108
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $99
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 95
byte 1 99
byte 1 97
byte 1 109
byte 1 112
byte 1 0
align 1
LABELV $98
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 95
byte 1 110
byte 1 111
byte 1 116
byte 1 110
byte 1 117
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $97
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 95
byte 1 110
byte 1 117
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $96
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 95
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 95
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $95
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 95
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 95
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 109
byte 1 97
byte 1 116
byte 1 99
byte 1 104
byte 1 0
align 1
LABELV $94
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 95
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 95
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $91
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 83
byte 1 104
byte 1 97
byte 1 100
byte 1 101
byte 1 114
byte 1 78
byte 1 101
byte 1 119
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $90
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 83
byte 1 104
byte 1 97
byte 1 100
byte 1 101
byte 1 114
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $89
byte 1 97
byte 1 110
byte 1 103
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $88
byte 1 97
byte 1 110
byte 1 103
byte 1 108
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $87
byte 1 100
byte 1 109
byte 1 103
byte 1 0
align 1
LABELV $86
byte 1 108
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 0
align 1
LABELV $85
byte 1 104
byte 1 101
byte 1 97
byte 1 108
byte 1 116
byte 1 104
byte 1 0
align 1
LABELV $84
byte 1 99
byte 1 111
byte 1 117
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $83
byte 1 114
byte 1 97
byte 1 110
byte 1 100
byte 1 111
byte 1 109
byte 1 0
align 1
LABELV $82
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $81
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $80
byte 1 109
byte 1 101
byte 1 115
byte 1 115
byte 1 97
byte 1 103
byte 1 101
byte 1 0
align 1
LABELV $79
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $78
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 0
align 1
LABELV $77
byte 1 115
byte 1 112
byte 1 101
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $76
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 115
byte 1 0
align 1
LABELV $75
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 50
byte 1 0
align 1
LABELV $74
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $73
byte 1 111
byte 1 114
byte 1 105
byte 1 103
byte 1 105
byte 1 110
byte 1 0
align 1
LABELV $72
byte 1 99
byte 1 108
byte 1 97
byte 1 115
byte 1 115
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $69
byte 1 37
byte 1 102
byte 1 32
byte 1 37
byte 1 102
byte 1 32
byte 1 37
byte 1 102
byte 1 0
