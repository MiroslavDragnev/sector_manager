public cmdBan(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 3))
		return PLUGIN_HANDLED
	
	if(!(id_flags[id] & ADMIN_BAN) && !get_pcvar_num(vote_ban))
	{
		console_print(id, "You have no access to that command.");
		return PLUGIN_HANDLED;
	}
	
	new arg[LEN_NAME+1], arg2[LEN_TIME+1], arg3[LEN_REASON+1];
	read_argv(1, arg, LEN_NAME);
	read_argv(2, arg2, LEN_TIME);
	read_argv(3, arg3, LEN_REASON);
	
	new player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!player || player == id)
		return PLUGIN_HANDLED;
	
	new idid = FindEmptyIndex(BAN);
	
	if(idid == -1)
	{
		console_print(id, "Dostignat e maksimalniqt broi zapisi. Za poveche info - DragonClaw.");
		
		return PLUGIN_HANDLED;
	}
	
	new tm = determine_multiplier(arg2, id, BAN, STARTED, API_OFF);
	
	if(!tm)
	{
		console_print(id, "Nevalidno chislo, vuvedeno za vreme.");
		
		return PLUGIN_HANDLED;
	}
	
	formatex(ban_names[idid], LEN_NAME, "%s", id_name[player]);
	formatex(ban_flex[idid], LEN_FLEX, "%s", id_flex[player]);
	formatex(ban_ip[idid], LEN_IP, "%s", id_ip[player]);
	ban_time[idid] = tm;
	formatex(ban_admin[idid], LEN_NAME, "%s", id_name[id]);
	formatex(ban_reason[idid], LEN_REASON, "%s", arg3);
	ban_ts[idid] = get_systime();
	
	console_print(id, "Igrachut e uspeshno bannat.");
	
	c_index[id] = idid;
	ban_details[id] = true;
	gag_details[id] = false;
	last_list[id] = false;
	BanDetails(id);
	
	#if defined LOG_ENABLED
	log_to_file(srm_log, "%s banned %s (flex: %s; ip: %s; time: %s)", id_name[id], id_name[player], id_flex[player], id_ip[player], arg2);
	#endif
	
	if(arg3[0])
		format(arg3, LEN_REASON, " Prichina: %s.", arg3);
	
	server_cmd("kick #%d ^"Beshe bannat ot ADMIN %s.%s^"", get_user_userid(player), id_name[id], arg3);
	
	switch(activity())
	{
		case 1:ColorChat(0, RED, "[ADMIN]^x01 banned %s.", id_name[player]);
		case 2:ColorChat(0, RED, "[ADMIN]^x01 %s banned %s.", id_name[id], id_name[player]);
	}
	
	return PLUGIN_HANDLED;
}

public cmdGag(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 3))
		return PLUGIN_HANDLED
	
	if(!(id_flags[id] & ADMIN_BAN) && !get_pcvar_num(vote_gag))
	{
		console_print(id, "You have no access to that command.");
		return PLUGIN_HANDLED;
	}
	
	new arg[LEN_NAME+1], arg2[LEN_TIME+1], arg3[LEN_REASON+1];
	read_argv(1, arg, LEN_NAME);
	read_argv(2, arg2, LEN_TIME);
	read_argv(3, arg3, LEN_REASON);
	
	new player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!player || player == id)
		return PLUGIN_HANDLED;
	
	new idid = FindEmptyIndex(GAG);
	
	if(idid == -1)
	{
		console_print(id, "Dostignat e maksimalniqt broi zapisi. Za poveche info - DragonClaw.");
		
		return PLUGIN_HANDLED;
	}
	
	new tm = determine_multiplier(arg2, id, GAG, STARTED, API_OFF);
	
	if(!tm)
	{
		console_print(id, "Nevalidno chislo, vuvedeno za vreme.");
		
		return PLUGIN_HANDLED;
	}
	
	formatex(gag_names[idid], LEN_NAME, "%s", id_name[player]);
	formatex(gag_flex[idid], LEN_FLEX, "%s", id_flex[player]);
	formatex(gag_ip[idid], LEN_IP, "%s", id_ip[player]);
	gag_time[idid] = tm;
	formatex(gag_admin[idid], LEN_NAME, "%s", id_name[id]);
	formatex(gag_reason[idid], LEN_REASON, "%s", arg3);
	gag_ts[idid] = get_systime();
	
	console_print(id, "Igrachut e uspeshno gagnat.");
	
	c_index[id] = idid;
	ban_details[id] = false;
	gag_details[id] = true;
	last_list[id] = false;
	GagDetails(id);
	
	#if defined LOG_ENABLED
	log_to_file(srm_log, "%s gagged %s (flex: %s; ip: %s; time: %s)", id_name[id], id_name[player], id_flex[player], id_ip[player], arg2);
	#endif
	
	c_gagged[player] = true;
	client_cmd(player, "-voicerecord");
	
	switch(activity())
	{
		case 1:ColorChat(0, RED, "[ADMIN]^x01 gagged %s.", id_name[player]);
		case 2:ColorChat(0, RED, "[ADMIN]^x01 %s gagged %s.", id_name[id], id_name[player]);
	}
	
	return PLUGIN_HANDLED;
}

public cmdCmdBan(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 4))
		return PLUGIN_HANDLED
	
	if(!(id_flags[id] & ADMIN_BAN) && !get_pcvar_num(vote_ban))
	{
		console_print(id, "You have no access to that command.");
		return PLUGIN_HANDLED;
	}
	
	new arg[LEN_NAME+1], arg2[LEN_TIME+1], arg3[LEN_CMDBAN+1];
	read_argv(1, arg, LEN_NAME);
	read_argv(2, arg3, LEN_CMDBAN);
	read_argv(3, arg2, LEN_TIME);
	
	/*new player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(player)
		formatex(arg, LEN_NAME, "%s", id_name[player]);*/
	
	new player;
	
	for(new i = 1; i <= maxplayers; i++)
	{
		if(!is_user_connected(i))
			continue;
		
		if(equali(id_name[i], arg))
		{
			player = i;
			break;
		}
	}
	
	format(arg3, LEN_CMDBAN, "%s%s%s", containi(arg3, FLAG_DIE) > -1 ? FLAG_DIE:"", containi(arg3, FLAG_GAG) > -1 ? FLAG_GAG:"", containi(arg3, FLAG_SWAP) > -1 ? FLAG_SWAP:"");
	
	if(!arg3[0])
	{
		console_print(id, "Ne sa vuvedeni validni flagove");
		
		return PLUGIN_HANDLED;
	}
	
	new idid = FindEmptyIndex(BAN);
	
	if(idid == -1)
	{
		console_print(id, "Dostignat e maksimalniqt broi zapisi. Za poveche info - DragonClaw.");
		
		return PLUGIN_HANDLED;
	}
	
	new tm = determine_multiplier(arg2, id, BAN, player == 0 ? PAUSED:STARTED, API_OFF);
	
	if(!tm)
	{
		console_print(id, "Nevalidno chislo, vuvedeno za vreme.");
		
		return PLUGIN_HANDLED;
	}
	
	formatex(ban_names[idid], LEN_NAME, "%s", arg);
	formatex(ban_flex[idid], LEN_FLEX, "%s", arg3);
	formatex(ban_ip[idid], LEN_IP, "%s", CMD_CONST);
	ban_time[idid] = tm;
	formatex(ban_admin[idid], LEN_NAME, "%s", id_name[id]);
	ban_ts[idid] = get_systime();

	console_print(id, "Igrachut e uspeshno bannat.");
	
	c_index[id] = idid;
	ban_details[id] = true;
	gag_details[id] = false;
	last_list[id] = false;
	BanDetails(id);
	
	#if defined LOG_ENABLED
	log_to_file(srm_log, "%s cmd-banned %s (flags: %s)", id_name[id], arg, arg3);
	#endif
	
	if(player)
		formatex(cmdban[player], LEN_CMDBAN, "%s", arg3);
	
	return PLUGIN_HANDLED;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1026\\ f0\\ fs16 \n\\ par }
*/
