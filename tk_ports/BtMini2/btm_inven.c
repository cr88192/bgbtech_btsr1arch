/*
Copyright (C) 2022  Brendan G Bohannon

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

/*
Inventory System

(31:24): Reserved
(23:16): Item Count (0..99)
(15:12): Item Attribute
(11: 0): Item Type
 */

byte	btm_inven_open;

int			btm_texfont_tinycnt;
int			btm_texfont_inven;


int BTM_InvenOpenP()
{
	return(btm_inven_open);
}

int BTM_InvenDoInput(int dt)
{
	return(0);
}

int BTM_DrawInventoryCell(BTM_World *wrl, int cx, int cy, u32 cell)
{
	int cnt, cbcd;

	cnt=(cell>>16)&0xFF;
	cbcd=(cnt%10)+((cnt/10)<<4);

	BTM_DrawCharPrim(
		cx, cy, 32, 32, cell&0xFF,
		btm_texfont_inven, 0xFFFFFFFFU);
	if(cnt>1)
	{
		BTM_DrawCharPrim(
			cx, cy, 8, 8, cbcd,
			btm_texfont_tinycnt, 0xFF000000U);
	}
	return(0);
}

int BTM_DrawInventoryGrid(BTM_World *wrl)
{
	int x, y;
	
	for(y=0; y<5; y++)
		for(x=0; x<8; x++)
	{
		BTM_DrawCharPrim(
			-128+(x*32), -96+(y*32),
			32, 32, 0xFE,
			btm_texfont_inven, 0xFFFFFFFFU);

		BTM_DrawInventoryCell(wrl,
			-128+(x*32), -96+(y*32),
			wrl->cam_inven[y*8+x]);
	}
	return(0);
}

int BTM_DrawInventory(BTM_World *wrl)
{
	if(!btm_inven_open)
		return(0);

	BTM_DrawInventoryGrid(wrl);
	return(0);
}

int BTM_InventoryHandleKey(BTM_World *wrl, u16 key)
{
	if(BTM_ConDownP() || BTM_MenuDownP())
		return(0);
	
	if(key=='\t')
	{
		btm_inven_open=!btm_inven_open;
		return(0);
	}

	if(!btm_inven_open)
		return(0);

	return(0);	
}
