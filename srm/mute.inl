public MuteMenu(id)
{	
	new item[33], number[3], count = 3, CsTeams:iTeam = cs_get_user_team(id), gName[33];
	
	new menu = menu_create("\rMute Menu", "mmenu_handler");
	
	menu_additem(menu, "Mute all", "1", 0);
	menu_additem(menu, "Unmute all^n", "2", 0);
	
	for(new i = 1; i <= maxplayers; i++)
	{
		if(i == id || !is_user_connected(i) || cs_get_user_team(i) != iTeam)
			continue;
			
		formatex(number, charsmax(number), "%i", count);
		
		get_user_name(i, gName, 32);
		
		if(mm_muted[id][i])
		{
			formatex(item, charsmax(item), "\d%s", gName);
				
			menu_additem(menu, item, number, 0);
		}
		else
		{
			menu_additem(menu, gName, number, 0);
		}
			
		count ++;
	}
	
	if(count == 3)
	{
		ColorChat(id, RED, "[Mute Menu]^x01 There are no players to mute.");
		
		return PLUGIN_HANDLED;
	}
	
	menu_setprop(menu, MPROP_EXIT, MEXIT_ALL);
	menu_display(id, menu, 0);
	
	return PLUGIN_HANDLED;
}

public mmenu_handler( id, menu, item )
{
	if ( item == MENU_EXIT ) 
	{
		menu_destroy( menu );
		return PLUGIN_HANDLED;
	}
	
	new data[ 6 ], iName[33], access, callback; 
	
	menu_item_getinfo( menu, item, access, data,6, iName, 32, callback );
	
	new key = str_to_num( data );
	
	if(key == 1)
	{
		ColorChat(id, RED, "[Mute Menu]^x01 All players were muted.");
		
		for(new i = 0; i < 33; i++)
			mm_muted[id][i] = true;
	}			
	else if(key == 2)
	{
		ColorChat(id, RED, "[Mute Menu]^x01 All players were unmuted.");
		
		for(new i = 0; i < 33; i++)
			mm_muted[id][i] = false;
	}
	else
	{
		if(contain(iName, "\d") == 0)
			replace(iName, 63, "\d", "");
		
		new gName[33];
		
		for(new i = 1; i <= maxplayers; i++)
		{
			if(i == id || !is_user_connected(i))
				continue;
			
			get_user_name(i, gName, 32);
			
			if(equali(gName, iName))
			{
				if(mm_muted[id][i])
				{
					mm_muted[id][i] = false;
					ColorChat(id, RED, "[Mute Menu]^x01 %s was unmuted.", iName);
				}
				else
				{
					mm_muted[id][i] = true;
					ColorChat(id, RED, "[Mute Menu]^x01 %s was muted.", iName);
				}
				
				break;
			}
		}
	}

	menu_destroy( menu );
	
	set_task(0.01, "MuteMenu", id);
	
	return PLUGIN_HANDLED;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1026\\ f0\\ fs16 \n\\ par }
*/
