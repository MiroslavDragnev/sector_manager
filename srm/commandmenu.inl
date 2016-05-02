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
	
	new access,callback, info[3], num;
	menu_item_getinfo(menu,item,access, info,2, _,_, callback)
	
	num = str_to_num(info) - 1;
	
	new plr = menu_indexes[id][num];
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
	else
		client_cmd(id, "%s #%d", cmd, get_user_userid(plr));
	
	menu_destroy(menu);
	set_task(0.1, "PlayerMenu", id);
	
	return PLUGIN_HANDLED;
}
		
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1033\\ f0\\ fs16 \n\\ par }
*/
