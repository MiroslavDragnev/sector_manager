public vformat_gag(id, level, cid)
{
	if(!cmd_access(id, level, cid, 1)) return PLUGIN_HANDLED;
	
	if(!get_pcvar_num(enable_gag))
	{
		ColorChat(id, GREEN, "[V.I.P. Menu]^x01 Komandata e izkliuchena za tozi server.");
		return PLUGIN_HANDLED;
	}
	else if(is_cmd_banned(id, FLAG_GAG))
	{
		ColorChat(id, GREEN, "[V.I.P. Menu]^x01 Dostuput ti do tazi komanda e blokiran ot admin poradi zloupotreba.");
		return PLUGIN_HANDLED;
	}
	
	formatex(id_cmd[id], LEN_IP, "amx_gag");
	remove_task(id);
	VipMenu(id);
	return PLUGIN_HANDLED;
}

public vformat_die(id, level, cid)
{
	if(!cmd_access(id, level, cid, 1)) return PLUGIN_HANDLED;
	
	if(!get_pcvar_num(vip_die) && !(id_flags[id] & ADMIN_IMMUNITY)) return PLUGIN_HANDLED;
	
	if(!get_pcvar_num(enable_kill))
	{
		ColorChat(id, GREEN, "[V.I.P. Menu]^x01 Komandata e izkliuchena za tozi server.");
		return PLUGIN_HANDLED;
	}
	else if(is_cmd_banned(id, FLAG_DIE))
	{
		ColorChat(id, GREEN, "[V.I.P. Menu]^x01 Dostuput ti do tazi komanda e blokiran ot admin poradi zloupotreba.");
		return PLUGIN_HANDLED;
	}
	
	formatex(id_cmd[id], LEN_IP, "amx_die");
	remove_task(id);
	VipMenu(id);
	return PLUGIN_HANDLED;
}

public vformat_swap(id, level, cid)
{
	if(!cmd_access(id, level, cid, 1)) return PLUGIN_HANDLED;
	
	if(!get_pcvar_num(enable_swap))
	{
		ColorChat(id, GREEN, "[V.I.P. Menu]^x01 Komandata e izkliuchena za tozi server.");
		return PLUGIN_HANDLED;
	}
	else if(is_cmd_banned(id, FLAG_SWAP))
	{
		ColorChat(id, GREEN, "[V.I.P. Menu]^x01 Dostuput ti do tazi komanda e blokiran ot admin poradi zloupotreba.");
		return PLUGIN_HANDLED;
	}
	
	formatex(id_cmd[id], LEN_IP, "amx_swap");
	remove_task(id);
	VipMenu(id);
	return PLUGIN_HANDLED;
}

public format_slap(id, level, cid)
{
	if(!cmd_access(id, level, cid, 1)) return PLUGIN_HANDLED;
	formatex(id_cmd[id], LEN_IP, "sr_slap");
	PlayerMenu(id);
	return PLUGIN_HANDLED;
}

public format_slay(id, level, cid)
{
	if(!cmd_access(id, level, cid, 1)) return PLUGIN_HANDLED;
	formatex(id_cmd[id], LEN_IP, "sr_slay");
	PlayerMenu(id);
	return PLUGIN_HANDLED;
}

public format_kick(id, level, cid)
{
	if(!cmd_access(id, level, cid, 1)) return PLUGIN_HANDLED;
	formatex(id_cmd[id], LEN_IP, "sr_kick");
	PlayerMenu(id);
	return PLUGIN_HANDLED;
}

public format_quit(id, level, cid)
{
	if(!cmd_access(id, level, cid, 1)) return PLUGIN_HANDLED;
	formatex(id_cmd[id], LEN_IP, "sr_quit");
	PlayerMenu(id);
	return PLUGIN_HANDLED;
}

public format_swap(id, level, cid)
{
	if(!cmd_access(id, level, cid, 1)) return PLUGIN_HANDLED;
	formatex(id_cmd[id], LEN_IP, "sr_swap");
	PlayerMenu(id);
	return PLUGIN_HANDLED;
}

public format_ban(id, level, cid)
{
	if(!cmd_access(id, level, cid, 1)) return PLUGIN_HANDLED;
	formatex(id_cmd[id], LEN_IP, "sr_ban");
	PlayerMenu(id);
	return PLUGIN_HANDLED;
}

public format_gag(id, level, cid)
{
	if(!cmd_access(id, level, cid, 1)) return PLUGIN_HANDLED;
	formatex(id_cmd[id], LEN_IP, "sr_gag");
	PlayerMenu(id);
	return PLUGIN_HANDLED;
}

public format_cmdban(id, level, cid)
{
	if(!cmd_access(id, level, cid, 1)) return PLUGIN_HANDLED;
	formatex(id_cmd[id], LEN_IP, "sr_cmdban");
	PlayerMenu(id);
	return PLUGIN_HANDLED;
}

public VipMenu(id)
{
	new counter = 0;
	new CsTeams:team;
	new CsTeams:target_team;
	team = cs_get_user_team(id);
	new cmd = equali(id_cmd[id], "amx_die") ? 2 : equali(id_cmd[id], "amx_swap") ? 1 : 0; // messy...
	
	for(new i = 1; i <= maxplayers; i++)
	{
		// making sure the menu contains only the users, on which the commands are executable
		// for better UX
		if(!is_user_connected(i) || id_flags[i] & ADMIN_LEVEL_D)
			continue;
		
		if(cmd == 2) // die
		{
			if(cs_get_user_team(i) != team || !is_user_alive(i))
				continue;
		}
		else if(cmd == 1) // swap
		{
			if(is_user_alive(i))
				continue;
				
			target_team = cs_get_user_team(i);
			
			if((team == CS_TEAM_CT && target_team != CS_TEAM_T)
			|| (team == CS_TEAM_T && target_team != CS_TEAM_CT))
				continue;
		}
		else if(cmd == 0) // gagged
		{
			if(vip_gagged[i] || sl_gagged[i] || c_gagged[i])
				continue;
		}
		
		menu_indexes[id][counter] = i;
		counter++;
	}
	
	new secondary_string[33];
	new Float:ctime = get_gametime();
	new cannot_use = true;
	
	if(!counter)
	{
		formatex(secondary_string, charsmax(secondary_string), "\dNo players available");
	}
	else
	{
		switch(cmd)
		{
			case 0: // gag
			{
				if(HasMoreGags(id) == -1)
					formatex(secondary_string, charsmax(secondary_string), "\dYou can't gag more than \r3\d players");
			}
			case 1: // swap
			{
				if(ctime < swap_usage[id])
					formatex(secondary_string, charsmax(secondary_string), "\dUse again in \r%i\d seconds", floatround(swap_usage[id] - ctime));
				else if(is_user_alive(id))
					formatex(secondary_string, charsmax(secondary_string), "\dYou need to be \rDEAD");
			}
			case 2: // die
			{
				if(ctime < die_usage[id])
					formatex(secondary_string, charsmax(secondary_string), "\dUse again in \r%i\d seconds", floatround(die_usage[id] - ctime));
			}
		}
	}
		
	if(!secondary_string[0])
	{
		formatex(secondary_string, charsmax(secondary_string), "\wReady to use");
		cannot_use = false;
	}
	
	id_page[id][PLAYER] = counter > 0 ? clamp(id_page[id][PLAYER], 0, (counter-1)/6) : 0;
	
	new title[384], item[64], str[4], id_str[4];
	formatex(title, charsmax(title), "\rCurrent command: \w%s^n%s%s%i/%i", id_cmd[id], secondary_string, "^n\rPage: \w", id_page[id][PLAYER]+1, counter == 0 ? 1:floatround(float(counter)/6, floatround_ceil));
	formatex(str, charsmax(str), "%s", cannot_use == 1 ? "\d":"\w");
	new menu = menu_create(title, "vipmenu_handler");
	
	new startindex = id_page[id][PLAYER]*6, endindex = startindex+6, plr;
	
	//formatex(item, charsmax(item), "Exit");
	menu_additem(menu, "Exit", "menuexit", 0);
	
	formatex(item, charsmax(item), "%sBack", startindex > 0 ? "\w":"\d");
	menu_additem(menu, item, "menuback", 0);
	
	formatex(item, charsmax(item), "%sNext^n%s", endindex < counter ? "\w":"\d", counter == 0 ? "^n\r(\dwaiting for players\r)":"");
	menu_additem(menu, item, "menunext", 0);
	
	for(new i = startindex; i < endindex; i++)
	{
		if(i >= counter)
			break;
			
		plr = menu_indexes[id][i];
		formatex(item, charsmax(item), "%s%s", str, id_name[plr]);
		num_to_str(plr, id_str, 2);
		
		menu_additem(menu, item, id_str, 0);
	}
	
	set_task(1.0, "VipMenu", id);
	
	menu_setprop(menu, MPROP_EXIT, MEXIT_NEVER);
	menu_setprop(menu, MPROP_PERPAGE, 0);
	menu_display(id, menu, 0);
}

public vipmenu_handler(id,menu,item)
{
	if(item < 0)
		return PLUGIN_HANDLED;
	
	new access, callback, info[16];
	menu_item_getinfo(menu, item, access, info, charsmax(info), _, _, callback);
	
	if(equal(info, "menuexit"))
	{
		remove_task(id);
		menu_destroy(menu);
		return PLUGIN_HANDLED;
	}
	else if(equal(info, "menuback"))
	{
		id_page[id][PLAYER]--;
	}
	else if(equal(info, "menunext"))
	{
		id_page[id][PLAYER]++;
	}
	else
	{
		new plr = str_to_num(info);
		
		if(!plr)
			ColorChat(id, GREEN, "[V.I.P. Menu]^x01 Player has left.");
		else
			client_cmd(id, "%s #%d", id_cmd[id], get_user_userid(plr));
	}
	
	menu_destroy(menu);
	
	remove_task(id);
	set_task(0.1, "VipMenu", id);
	
	return PLUGIN_HANDLED;
}

public PlayerMenu(id)
{
	new players[32], num, calc;
	get_players(players, num);
	
	for(new i = 0; i < num; i++)
		menu_indexes[id][i] = players[i];
	
	new plr1, plr2;
	new CsTeams:team1, CsTeams:team2;
	
	for(new i = 0; i < num; i++)
	{
		calc = num - i - 1;
				
		for(new j = 0; j < calc; j++)
		{
			plr1 = menu_indexes[id][j];
			plr2 = menu_indexes[id][j+1];
					
			if(is_user_alive(plr1) < is_user_alive(plr2))
			{
				SwapIndexes(id, j, j+1);
			}
		}
	}
	
	for(new i = 0; i < num; i++)
	{
		calc = num - i - 1;
				
		for(new j = 0; j < calc; j++)
		{
			plr1 = menu_indexes[id][j];
			team1 = cs_get_user_team(plr1);
			plr2 = menu_indexes[id][j+1];
			team2 = cs_get_user_team(plr2);
					
			if(team1 > team2)
			{
				SwapIndexes(id, j, j+1);
			}
		}
	}
	
	new title[384], item[64], str[4], cur_player, count_str[3];
	formatex(title, charsmax(title), "\rCurrent command: \w%s%s", id_cmd[id], num > 7 ? "^n\rPage: \w":"");
	new menu = menu_create(title, "players_handler");
	new darken_plr[4], darken_dead = equali(id_cmd[id], "sr_slap") || equali(id_cmd[id], "sr_slay");
	
	for(new i = 0; i < num; i++)
	{
		cur_player = menu_indexes[id][i];
		
		switch(cs_get_user_team(cur_player))
		{
			case CS_TEAM_CT:formatex(str, charsmax(str), "CT");
			case CS_TEAM_T:formatex(str, charsmax(str), "T");
		}
		
		if(darken_dead && !is_user_alive(cur_player)) formatex(darken_plr, charsmax(darken_plr), "\d");
		else formatex(darken_plr, charsmax(darken_plr), "\w");

		formatex(item, charsmax(item), "%s%s - \r[%s%s\r]", darken_plr, id_name[cur_player], darken_plr, str);
		
		num_to_str(i+1, count_str, 2);
		menu_additem(menu, item, count_str, 0);
	}
	
	menu_setprop(menu, MPROP_EXIT, MEXIT_ALL);
	menu_display(id, menu, 0);
}

stock SwapIndexes(const index, const id1, const id2)
{
	new temp = menu_indexes[index][id2];
	menu_indexes[index][id2] = menu_indexes[index][id1];
	menu_indexes[index][id1] = temp;
}

public players_handler(id,menu,item)
{
	if(item == MENU_EXIT)
	{
		menu_destroy(menu)
		return PLUGIN_HANDLED
	}
	
	new access,callback, info[3];
	menu_item_getinfo(menu,item,access, info,2, _,_, callback)
	
	new plr = menu_indexes[id][item];
	new cmd[LEN_IP+1];
	copy(cmd, LEN_IP, id_cmd[id]);
		
	if(equali(cmd, "sr_ban"))
		client_cmd(id, "sr_ban #%d 1m", get_user_userid(plr));
	else if(equali(cmd, "sr_gag"))
		client_cmd(id, "sr_gag #%d 1m", get_user_userid(plr));
	else if(equali(cmd, "sr_slap"))
	{
		client_cmd(id, "sr_slap #%d", get_user_userid(plr));
		menu_destroy(menu);
		PlayerMenu(id);
		return PLUGIN_HANDLED;
	}
	else if(equali(cmd, "sr_cmdban"))
		client_cmd(id, "sr_cmdban ^"%s^" dgs 1m", id_name[plr]);
	else
		client_cmd(id, "%s #%d", cmd, get_user_userid(plr));
	
	menu_destroy(menu);
	set_task(0.1, "PlayerMenu", id);
	
	return PLUGIN_HANDLED;
}
		
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1033\\ f0\\ fs16 \n\\ par }
*/
