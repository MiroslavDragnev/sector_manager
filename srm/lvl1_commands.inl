public cmdSrm(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 1))
		return PLUGIN_HANDLED;
	
	new buffer[2048], len = formatex(buffer, 2047, "<body bgcolor=#D9D9D9><table width=100%% cellpadding=2 cellspacing=0 border=0>");
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#000000><th width=50%%> <font color=FFFFFF>Command</font> <th width=50%% align=left> <font color=FFFFFF>Usage</font>");
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#ECECEC><th width=50%%> amx_gag, amx_ungag <th width=50%% align=left> |Name/#ID|");
	len += formatex(buffer[len], 2047-len, "<tr align=left><th width=50%%> sr_who, sr_last <th width=50%% align=left> -");
	//len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#ECECEC><th width=50%%> sr_slap <th width=50%% align=left> |Name/#ID| /Damage/");
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#ECECEC><th width=50%%> sr_slap, sr_slay, sr_kick, sr_quit, sr_swap, sr_spec <th width=50%% align=left> |Name/#ID|");
	len += formatex(buffer[len], 2047-len, "<tr align=left><th width=50%%> sr_banlist, sr_gaglist, /banlist, /gaglist, /last <th width=50%% align=left> -");
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#ECECEC><th width=50%%> sr_cmdban <th width=50%% align=left> |Full Name (if offline)/Name/#ID| |Flags| |Time|");
	len += formatex(buffer[len], 2047-len, "<tr align=left><th width=50%%> sr_ban, sr_gag <th width=50%% align=left> |Name/#ID| |Time| /Reason/");
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#ECECEC><th width=50%%> sr_addban, sr_addgag <th width=50%% align=left> |Full Name| |FlexID/-| |IP/-| |Time| /Reason/");
	len += formatex(buffer[len], 2047-len, "<tr align=left><th width=50%%> sr_add_admin <th width=50%% align=left> |Full Name| |FlexID| |Level|");
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#ECECEC><th width=50%%> sr_remove_admin <th width=50%% align=left> |Full Name|");
	//len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#ECECEC><th width=50%%> sr_admin_list <th width=50%% align=left> -");
	len += formatex(buffer[len], 2047-len, "<tr align=left><th width=50%%> sr_admin_list, sr_clear_banlist, sr_clear_gaglist <th width=50%% align=left> -");
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#ECECEC><th width=50%%> sr_exec, sr_cvar <th width=50%% align=left> |Name/#ID| |Command|");
	
	len += formatex(buffer[len], 2047-len, "</table></body>")
		
	show_motd(id, buffer, "SRM Commands")

	return PLUGIN_HANDLED
}

public cmdSrm2(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 1))
		return PLUGIN_HANDLED;
	
	new i, count[4], buffer[2048];
	
	for(i = 0; i < MAX_ADMINS; i++)
	{
		if(is_record_valid(ADMIN, i))
			count[ADMIN] ++;
	}
	
	for(i = 0; i < MAX_ENTRIES; i++)
	{
		if(is_record_valid(BAN, i))
			count[BAN] ++;
			
		if(is_record_valid(GAG, i))
			count[GAG] ++;
	}
	
	for(i = 0; i < MAX_LAST; i++)
	{
		if(is_record_valid(LAST, i))
			count[LAST] ++;
	}
	
	new len = formatex(buffer, 2047, "<body bgcolor=#D9D9D9><table width=100%% cellpadding=2 cellspacing=0 border=0>");
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#000000><th width=20%%> <font color=FFFFFF>Fullness statistics</font> <th width=80%% align=left>");
	len += formatex(buffer[len], 2047-len, "<tr align=left><th width=20%%> Admin list <th width=80%% align=left> %i/%i", count[ADMIN], MAX_ADMINS);
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#ECECEC><th width=20%%> Ban list <th width=80%% align=left> %i/%i", count[BAN], MAX_ENTRIES);
	len += formatex(buffer[len], 2047-len, "<tr align=left><th width=20%%> Gag list <th width=80%% align=left> %i/%i", count[GAG], MAX_ENTRIES);
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#ECECEC><th width=20%%> Last users list <th width=80%% align=left> %i/%i", count[LAST], MAX_LAST);
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#000000><th width=20%%> <font color=FFFFFF>CVAR Settings</font> <th width=80%% align=left>");
	len += formatex(buffer[len], 2047-len, "<tr align=left><th width=20%%> sr_lvl2_ban <th width=80%% align=left> %i", get_pcvar_num(vote_ban));
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#ECECEC><th width=20%%> sr_lvl2_gag <th width=80%% align=left> %i", get_pcvar_num(vote_gag));
	len += formatex(buffer[len], 2047-len, "<tr align=left><th width=20%%> sr_enable_die <th width=80%% align=left> %i", get_pcvar_num(enable_kill));
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#ECECEC><th width=20%%> sr_enable_gag <th width=80%% align=left> %i", get_pcvar_num(enable_gag));
	len += formatex(buffer[len], 2047-len, "<tr align=left><th width=20%%> sr_enable_swap <th width=80%% align=left> %i", get_pcvar_num(enable_swap));
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#ECECEC><th width=20%%> sr_die_for_vips <th width=80%% align=left> %i", get_pcvar_num(vip_die));
	len += formatex(buffer[len], 2047-len, "<tr align=left><th width=20%%> sr_cmd_wait <th width=80%% align=left> %i", get_pcvar_num(cmd_wait));
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#000000><th width=20%%> <font color=FFFFFF>About</font> <th width=80%% align=left>");
	len += formatex(buffer[len], 2047-len, "<tr align=left><th width=20%%> %s <th width=80%% align=left> by %s", PLUGIN, AUTHOR);
	len += formatex(buffer[len], 2047-len, "<tr align=left bgcolor=#ECECEC><th width=20%%> Version <th width=80%% align=left> %s", VERSION);
	
	len += formatex(buffer[len], 2047-len, "</table></body>")
		
	show_motd(id, buffer, "SRM Info")

	return PLUGIN_HANDLED
}
	
public cmdWho(id)
{	
	new count = 1, i, sflags[33], country[33], city[33];
	
	if(id_flags[id] & ADMIN_SLAY)
	{
		console_print(id, "# Name     IP     Flex ID     Steam ID     Flags     Country/City");
		
		for(i = 1; i <= maxplayers; i++)
		{
			if(!is_user_connected(i))
				continue;
			
			get_flags(id_flags[i], sflags, charsmax(sflags));
			geoip_country(id_ip[i], country, 32);
			geoip_city(id_ip[i], city, 32);
			
			if(equali(country, "error"))
				formatex(country, charsmax(country), "Unknown");
					
			if(equali(city, "error"))
				formatex(city, charsmax(city), "Unknown");
				
			console_print(id, "%i %s     %s     %s     %s     %s     %s/%s", count, id_name[i], id_ip[i], id_flex[i], id_steam[i], sflags, country, city);

			count ++;
		}
	}
	else
	{
		console_print(id, "# Name     Flags");
	
		for(i = 1; i <= maxplayers; i++)
		{
			if(!is_user_connected(i))
				continue;
			
			get_flags(id_flags[i], sflags, charsmax(sflags));
			
			console_print(id, "%i %s     %s", count, id_name[i], sflags);
			
			count ++;
		}
	}
	
	return PLUGIN_HANDLED;
}
 
/*public cmdLast(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 1))
		return PLUGIN_HANDLED;
	
	for(new i = 0; i < MAX_LAST; i++)
	{
		if(!is_record_valid(LAST, i))
			continue;
			
		console_print(id, "^"%s^" %s %s", last_names[i], last_flex[i], last_ip[i]);
	}
	
	return PLUGIN_HANDLED;
}*/

public cmdSlap(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED;
	
	new arg[LEN_NAME+1], arg2[5], dmg;
	read_argv(1, arg, LEN_NAME);
	read_argv(2, arg2, charsmax(arg2));
	
	new player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!player || !is_user_alive(player))
		return PLUGIN_HANDLED;	
	
	dmg = str_to_num(arg2);
	
	user_slap(player, dmg);
	
	switch(activity())
	{
		case 1:ColorChat(0, RED, "[ADMIN]^x01 slapped %s with %i damage.", id_name[player], dmg);
		case 2:ColorChat(0, RED, "[ADMIN]^x01 %s slapped %s with %i damage.", id_name[id], id_name[player], dmg);
	}
	
	return PLUGIN_HANDLED;
}

public cmdSlay(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED;
	
	new arg[LEN_NAME+1];
	read_argv(1, arg, LEN_NAME);
	
	new player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!player || !is_user_alive(player))
		return PLUGIN_HANDLED;	
	
	user_silentkill(player);
	
	switch(activity())
	{
		case 1:ColorChat(0, RED, "[ADMIN]^x01 slayed %s.", id_name[player]);
		case 2:ColorChat(0, RED, "[ADMIN]^x01 %s slayed %s.", id_name[id], id_name[player]);
	}
	
	return PLUGIN_HANDLED;
}

public cmdKick(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED;
	
	new arg[LEN_NAME+1];
	read_argv(1, arg, LEN_NAME);
	
	new player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!player)
		return PLUGIN_HANDLED;	
	
	server_cmd("kick #%d", get_user_userid(player));
	
	switch(activity())
	{
		case 1:ColorChat(0, RED, "[ADMIN]^x01 kicked %s.", id_name[player]);
		case 2:ColorChat(0, RED, "[ADMIN]^x01 %s kicked %s.", id_name[id], id_name[player]);
	}
	
	return PLUGIN_HANDLED;
}

public cmdQuit(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED
	
	new arg[LEN_NAME+1];
	read_argv(1, arg, LEN_NAME);
	
	new player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!player)
		return PLUGIN_HANDLED;
	
	new authid[32];
	get_user_authid(player, authid, charsmax(authid));
	
	if(equali(authid, "STEAM", 5))
	{
		console_cmd(id, "sr_kick #%d", get_user_userid(player));
		
		console_print(id, "Igrachut beshe kicknat; sr_quit ne raboti vurhu Steam.");
		
		return PLUGIN_HANDLED;
	}
	else client_cmd(player, "quit");
	
	switch(activity())
	{
		case 1:ColorChat(0, RED, "[ADMIN]^x01 made %s quit the game.", id_name[player]);
		case 2:ColorChat(0, RED, "[ADMIN]^x01 %s made %s quit the game.", id_name[id], id_name[player]);
	}
	
	return PLUGIN_HANDLED;
}

public cmdSwap(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 1))
		return PLUGIN_HANDLED
	
	new arg[LEN_NAME+1];
	read_argv(1, arg, LEN_NAME);
	
	new player;
	
	if(arg[0] == 0)
		player = id;
	else
	{
		player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
		
		if(!player)
			return PLUGIN_HANDLED;
	}
	
	new str[21];
	
	switch(cs_get_user_team(player))
	{
		case CS_TEAM_CT:
		{
			cs_set_user_team(player, CS_TEAM_T);
			fix_score_team(player, "TERRORIST");
		}
		case CS_TEAM_T:
		{
			cs_set_user_team(player, CS_TEAM_CT);
			fix_score_team(player, "CT");
			
			formatex(str, charsmax(str), "Counter-");
		}
		case CS_TEAM_SPECTATOR:
		{
			console_print(id, "Ne mojesh da izpolzvash tazi komanda vurhu spektatori.");
			
			return PLUGIN_HANDLED;
		}
		case CS_TEAM_UNASSIGNED:
		{
			console_print(id, "Ne mojesh da izpolzvash tazi komanda vurhu spektatori.");
			
			return PLUGIN_HANDLED;
		}
	}
	
	if(is_user_alive(player))
		ExecuteHamB(Ham_CS_RoundRespawn, player);
	
	if(player == id)
		ColorChat(player, RED, "[ADMIN]^x01 You were transferred to the %sTerrorists' force.", str);
	else
	{
		switch(activity())
		{
			case 1:ColorChat(player, RED, "[ADMIN]^x01 You were transferred to the %sTerrorists' force.", str);
			case 2:ColorChat(player, RED, "[ADMIN]^x01 %s transferred you to the %sTerrorists' force.", id_name[id], str);
		}
		
		ColorChat(id, RED, "[ADMIN]^x01 %s was transferred to the %sTerrorists' force.", id_name[player], str);
	}
	
	return PLUGIN_HANDLED;
}

public cmdUngag(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED;
	
	new arg[LEN_NAME+1];
	read_argv(1, arg, LEN_NAME);
	
	new player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!player)
		return PLUGIN_HANDLED;
	
	if(c_gagged[player])
	{
		console_print(id, "Igrachut se namira v gaglistata, premahni gaga ot tam.");
		
		return PLUGIN_HANDLED;
	}
	
	if(!vip_gagged[player] && !sl_gagged[player])
	{
		console_print(id, "Igrachut ne e gagnat.");
		
		return PLUGIN_HANDLED;
	}
	
	if(sl_gagged[player])
	{
		new temp[LEN_NAME+1], sz = ArraySize(slay_gag);
		
		for(new i = 0; i < sz; i++)
		{
			ArrayGetString(slay_gag, i, temp, LEN_NAME);

			if(equali(id_name[player], temp))
			{
				ArrayDeleteItem(slay_gag, i);
				sl_gagged[player] = false;
				
				break;
			}
		}
	}
	
	if(vip_gagged[player])
		RemoveVipGag(player);
	
	switch(activity())
	{
		case 1:ColorChat(0, RED, "[ADMIN]^x01 ungagged %s.", id_name[player]);
		case 2:ColorChat(0, RED, "[ADMIN]^x01 %s ungagged %s.", id_name[id], id_name[player]);
	}
	
	return PLUGIN_HANDLED;
}

public cmdCvar(id, level, cid)
{
	if(!cmd_access(id, level, cid, 3))
		return PLUGIN_HANDLED;
		
	new arg[LEN_NAME+1], arg2[33];
	read_argv(1, arg, LEN_NAME);
	read_argv(2, arg2, charsmax(arg2));
	
	new player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!player)
		return PLUGIN_HANDLED;
	
	id_query = id;
	
	query_client_cvar(player, arg2, "cvar_result_func");
	
	return PLUGIN_HANDLED;
}

public cvar_result_func(id, const cvar[], const result[])
{
	console_print(id_query, "Name: %s | Cvar: %s | Value: %s", id_name[id], cvar, result);
}

public cmdSpec(id, level, cid)
{
	if(!cmd_access(id, level, cid, 1))
		return PLUGIN_HANDLED;
	
	new arg[LEN_NAME+1];
	read_argv(1, arg, LEN_NAME);
	
	if(!arg[0] && task_exists(id+TASK_SPEC))
	{
		remove_task(id+TASK_SPEC);
		console_print(id, "You are no longer a spectator.");
		cs_set_user_team(id, old_team[id]);
		return PLUGIN_HANDLED;
	}
	
	new player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(player == id)
		player = 0;
	
	switch(player)
	{
		case 0:console_print(id, "You are now a spectator (no target).");
		default:console_print(id, "You are now spectating %s", id_name[player]);
	}
	
	old_team[id] = cs_get_user_team(id);
	
	id_nextspec = id;
	id_spectator[id] = player;
	
	set_pev(id, pev_origin, Float:{9999.0, 9999.0, 9999.0});
	user_silentkill(id);
	cs_set_user_team(id, CS_TEAM_SPECTATOR);
	
	set_task(1.0, "TaskSpec", TASK_SPEC+id, _, _, "b");
	
	return PLUGIN_HANDLED;
}

public TaskSpec(id)
{
	id -= TASK_SPEC;
	
	new show_as = 1;
	
	if(id_spectator[id] != 0)
	{
		new CsTeams:team;
		team = cs_get_user_team(id_spectator[id]);
		
		switch(team)
		{
			case CS_TEAM_CT:
			{
				fix_score_team(id, "TERRORIST");
				show_as = 2;
			}
			case CS_TEAM_T:fix_score_team(id, "CT");
			default:fix_score_team(id, "TERRORIST");
		}
	} else fix_score_team(id, "TERRORIST");
	
	switch(show_as)
	{
		case 2:
		{
			if(t_showdead)
				send_ScoreAttrib(id, 1);
			else
				send_ScoreAttrib(id, 0);
		}
		default:
		{
			if(ct_showdead)
				send_ScoreAttrib(id, 1);
			else
				send_ScoreAttrib(id, 0);
		}
	}
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1026\\ f0\\ fs16 \n\\ par }
*/
