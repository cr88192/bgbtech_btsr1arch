static int rottwl_staticremap[128]={
stat_gibs1,					//  0 puddle          spr1v
stat_bonusbarrel,			//  1 Green Barrel    "
stat_emptytable,			//  2 Table/chairs    "
stat_lamp,					//  3 Floor lamp      "
stat_chandelier,			//  4 Chandelier      "
stat_garb2,					//  5 Hanged man      "
stat_monkmeal,				//  6 Bad food        "
stat_rlight,				//  7 Red pillar      "
stat_tree,					//  8 Tree            spr2v
stat_garb1,					//  9 Skeleton flat   "
stat_emptybasin,			// 10 Sink            " (SOD:gibs)
stat_plant,					// 11 Potted plant    "
stat_urn,					// 12 Urn             "
stat_emptytable,			// 13 Bare table      "
stat_blight,				// 14 Ceiling light   "
stat_stool,					// 15 Gibs!
stat_emptystatue,			// 16 suit of armor   spr3v
stat_garb1,					// 17 Hanging cage    "
stat_garb2,					// 18 SkeletoninCage  "
stat_garb3,					// 19  Skeleton relax  "
stat_pedgoldkey,			// 20 Key 1           "
stat_pedsilverkey,			// 21 Key 2           "
stat_garb2,					// 22 stuff				(SOD:gibs)
stat_garb3,					// 23 stuff
stat_priestporridge,		// 24 Good food       spr4v
stat_healingbasin,			// 25 First aid       "
stat_mp40,					// 26 Clip            "
stat_bazooka,				// 27 Machine gun     "
stat_firewall,				// 28 Gatling gun     "
stat_lifeitem1,				// 29 Cross           "
stat_lifeitem2,				// 30 Chalice         "
stat_lifeitem3,				// 31 Bible           "
stat_lifeitem4,				// 32 crown           spr5v
stat_oneup,					// 33 one up          "
stat_shit,					// 34 gibs            "
stat_ironbarrel,			// 35 barrel          "
stat_pit,					// 36 well            "
stat_pit,					// 37 Empty well      "
stat_garb3,					// 38 Gibs 2          "
stat_torch,					// 39 flag			   "
stat_torch,					// 40 Red light
stat_garb1,					// 41 junk            "
stat_garb2,					// 42 junk 		   "
stat_garb3,					// 43 junk 		   "
stat_stool,					// 44 Gibs!
stat_stool,					// 45 stove           " (SOD:gibs)
stat_stool,					// 46 spears          " (SOD:gibs)
stat_stool,					// 47 vines		   "
stat_standardpole,			// 48 marble pillar
stat_random,				// 49 bonus 25 clip
stat_tomlarva,				// 50 truck
stat_kes,					// 51 SPEAR OF DESTINY!

stat_kes,					// SPEAR OF DESTINY!
stat_kes,					// SPEAR OF DESTINY!
stat_kes,					// SPEAR OF DESTINY!
stat_kes,					// SPEAR OF DESTINY!
0
};

int RemapObjTile_Wolf(int x, int y, int tile)
{
	int rmtile, orgtile;
	
	orgtile = tile;
	rmtile = 0;

	if((y==0) && (x<5))
		return(0);

	switch(tile)
	{
		case 19:
		case 20:
		case 21:
		case 22:
			rmtile=tile;
			break;

//
// guard
//
			case 180:
			case 181:
			case 182:
			case 183:
				if( gamestate.difficulty < gd_hard )
					{ rmtile=-1; break; }
				tile -= 36;
			case 144:
			case 145:
			case 146:
			case 147:
				if( gamestate.difficulty < gd_medium )
					{ rmtile=-1; break; }
				tile -= 36;
			case 108:
			case 109:
			case 110:
			case 111:
//				SpawnStand( en_guard, x, y, tile - 108, lvl );
				rmtile = tile;
				break;

			case 184:
			case 185:
			case 186:
			case 187:
				if( gamestate.difficulty < gd_hard )
					{ rmtile=-1; break; }
				tile -= 36;
			case 148:
			case 149:
			case 150:
			case 151:
				if( gamestate.difficulty < gd_medium )
					{ rmtile=-1; break; }
				tile -= 36;
			case 112:
			case 113:
			case 114:
			case 115:
//				SpawnPatrol( en_guard, x, y,tile - 112 );
				rmtile = tile;
				break;

			case 124:
//				SpawnDeadGuard( en_guard, x, y );
				break;
//
// officer
//
			case 188:
			case 189:
			case 190:
			case 191:
				if( gamestate.difficulty < gd_hard )
					{ rmtile=-1; break; }
				tile -= 36;
			case 152:
			case 153:
			case 154:
			case 155:
				if( gamestate.difficulty < gd_medium )
					{ rmtile=-1; break; }
				tile -= 36;
			case 116:
			case 117:
			case 118:
			case 119:
//				SpawnStand( en_officer, x, y, tile - 116, lvl );
				rmtile = tile;
				break;


			case 192:
			case 193:
			case 194:
			case 195:
				if( gamestate.difficulty < gd_hard )
					{ rmtile=-1; break; }
				tile -= 36;
			case 156:
			case 157:
			case 158:
			case 159:
				if( gamestate.difficulty < gd_medium )
					{ rmtile=-1; break; }
				tile -= 36;
			case 120:
			case 121:
			case 122:
			case 123:
//				SpawnPatrol( en_officer, x, y, tile - 120 );
//				rmtile = tile-4;
				rmtile = tile;
				break;
//
// SS
//
			case 198:
			case 199:
			case 200:
			case 201:
				if( gamestate.difficulty < gd_hard )
					{ rmtile=-1; break; }
				tile -= 36;
			case 162:
			case 163:
			case 164:
			case 165:
				if( gamestate.difficulty < gd_medium )
					{ rmtile=-1; break; }
				tile -= 36;
			case 126:
			case 127:
			case 128:
			case 129:
//				SpawnStand( en_ss, x, y, tile - 126, lvl );
				rmtile = tile+(144-126);
//				rmtile = tile+(116-126);
				break;

			case 202:
			case 203:
			case 204:
			case 205:
				if( gamestate.difficulty < gd_hard )
					{ rmtile=-1; break; }
				tile -= 36;
			case 166:
			case 167:
			case 168:
			case 169:
				if( gamestate.difficulty < gd_medium )
					{ rmtile=-1; break; }
				tile -= 36;
			case 130:
			case 131:
			case 132:
			case 133:
//				SpawnPatrol( en_ss, x, y, tile - 130 );
//				rmtile = tile+(144-130);
				rmtile = tile+(148-130);
//				rmtile = tile+(120-130);
				break;
//
// dogs
//
			case 206:
			case 207:
			case 208:
			case 209:
				if( gamestate.difficulty < gd_hard )
					{ rmtile=-1; break; }
				tile -= 36;
			case 170:
			case 171:
			case 172:
			case 173:
				if( gamestate.difficulty < gd_medium )
					{ rmtile=-1; break; }
				tile -= 36;
			case 134:
			case 135:
			case 136:
			case 137:
//				SpawnStand( en_dog, x, y, tile - 134, lvl );
				break;

			case 210:
			case 211:
			case 212:
			case 213:
				if( gamestate.difficulty < gd_hard )
					{ rmtile=-1; break; }
				tile -= 36;
			case 174:
			case 175:
			case 176:
			case 177:
				if( gamestate.difficulty < gd_medium )
					{ rmtile=-1; break; }
				tile -= 36;
			case 138:
			case 139:
			case 140:
			case 141:
//				SpawnPatrol( en_dog, x, y, tile - 138 );
//				rmtile = tile+(158-138);
//				rmtile = tile+(400-138);
				rmtile = tile+(364-138);
				break;

// bosses
			case 214:
//				SpawnBoss( en_boss, x, y );
				break;

			case 197:
//				SpawnBoss( en_gretel, x, y );
				break;

			case 215:
//				SpawnBoss( en_gift, x, y );
				break;

			case 179:
//				SpawnBoss( en_fat, x, y );
				break;

			case 196:
//				SpawnBoss( en_schabbs, x, y );
				break;

			case 160:
//				SpawnBoss( en_fake, x, y );
				break;

			case 178:
//				SpawnBoss( en_mecha, x, y );
				break;

//
// Spear
//
			case 106:
//				SpawnBoss( en_spectre, x, y );
				break;

			case 107:
//				SpawnBoss( en_angel, x, y );
				break;

			case 125:
//				SpawnBoss( en_trans, x, y );
				break;

			case 142:
//				SpawnBoss( en_uber, x, y );
				break;

			case 143:
//				SpawnBoss( en_will, x, y );
				break;

			case 161:
//				SpawnBoss( en_death, x, y );
				break;
//
// mutants
//
			case 252:
			case 253:
			case 254:
			case 255:
				if( gamestate.difficulty < gd_hard )
					{ rmtile=-1; break; }
				tile -= 18;
			case 234:
			case 235:
			case 236:
			case 237:
				if( gamestate.difficulty < gd_medium )
					{ rmtile=-1; break; }
				tile -= 18;
			case 216:
			case 217:
			case 218:
			case 219:
//				SpawnStand( en_mutant, x, y, tile - 216, lvl );
//				rmtile = tile;
				rmtile = tile+(188-216);
				break;

			case 256:
			case 257:
			case 258:
			case 259:
				if (gamestate.difficulty<gd_hard)
					{ rmtile=-1; break; }
				tile -= 18;
			case 238:
			case 239:
			case 240:
			case 241:
				if( gamestate.difficulty < gd_medium )
					{ rmtile=-1; break; }
				tile -= 18;
			case 220:
			case 221:
			case 222:
			case 223:
//				SpawnPatrol( en_mutant, x, y, tile - 220 );
				rmtile = tile+(184-220);
				break;

//
// ghosts
//
			case 224:
//				SpawnGhosts( en_blinky, x, y );
				rmtile = tile+(360-224);
				break;

			case 225:
//				SpawnGhosts( en_clyde, x, y );
				rmtile = tile+(396-225);
				break;

			case 226:
//				SpawnGhosts( en_pinky, x, y );
				rmtile = tile+(364-226);
				break;

			case 227:
//				SpawnGhosts( en_inky, x, y );
				rmtile = tile+(404-227);
				break;

			case 23:	case 24:
			case 25:	case 26:
			case 27:	case 28:
			case 29:	case 30:
			case 31:	case 32:
			case 33:	case 34:
			case 35:	case 36:
			case 37:	case 38:
			case 39:	case 40:
			case 41:	case 42:
			case 43:	case 44:
			case 45:	case 46:
			case 47:	case 48:
#if 1
			case 49:	case 50:
			case 51:	case 52:
			case 53:	case 54:
			case 55:	case 56:
#endif

#if 1
			case 57:	case 58:
			case 59:	case 60:
			case 61:	case 62:
			case 63:	case 64:
#endif

#if 1
			case 65:	case 66:
			case 67:	case 68:
			case 69:	case 70:
			case 71:	case 72:
			case 73:	case 74:
			case 75:	case 76:
#endif
				rmtile=rottwl_staticremap[tile-23]+23;
				break;

			case 80:
			case 96:
				rmtile=80;
				break;
			
			case 98:
				rmtile=rottwl_staticremap[49]+23;
				break;
	}
	
	if((rmtile>0) && (rmtile!=tile))
	{
//		printf("Remap Obj %d -> %d\n", tile, rmtile);
	}
	else if(tile && (rmtile<=0))
	{
		if(!rmtile)
		{
			printf("Remap Obj, Missed %d,%d\n", orgtile, tile);
		}
	}

	return(rmtile);
}
