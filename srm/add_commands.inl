public cmdAddBan(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 4))
		return PLUGIN_HANDLED;
	
	if(!(id_flags[id] & ADMIN_BAN) && !get_pcvar_num(vote_ban))
	{
		console_print(id, "You have no access to that command.");
		return PLUGIN_HANDLED;
	}
	
	new arg[LEN_NAME+1], arg2[LEN_FLEX+1], arg3[LEN_IP+1], arg4[LEN_TIME+1], arg5[LEN_REASON+1];
	read_argv(1, arg, LEN_NAME);
	read_argv(2, arg2, LEN_FLEX);
	read_argv(3, arg3, LEN_IP);
	read_argv(4, arg4, LEN_TIME);
	read_argv(5, arg5, LEN_REASON);
	
	for(new m = 1; m < maxplayers; m++)
	{
		if(!is_user_connected(m))
			continue;
			
		if((equali(id_name[m], arg) && !do_skip_name(arg)) || equali(id_ip[m], arg3) || (equali(id_flex[m], arg2) && !equali(id_flex[m], FLEX_CONST)))
		{
			console_cmd(id, "sr_ban #%d %s ^"%s^"", get_user_userid(m), arg4, arg5);
				
			return PLUGIN_HANDLED;
		}
	}
	
	new idid = FindEmptyIndex(BAN);
	
	if(idid == -1)
	{
		console_print(id, "Dostignat e maksimalniqt broi zapisi. Za poveche info - DragonClaw.");
		
		return PLUGIN_HANDLED;
	}
	
	new tm = determine_multiplier(arg4, id, BAN, PAUSED, API_OFF);
	
	if(!tm)
	{
		console_print(id, "Nevalidno chislo, vuvedeno za vreme.");
		
		return PLUGIN_HANDLED;
	}
	
	if(!is_valid_flex(arg2))
		formatex(arg2, LEN_FLEX, "%s", FLEX_CONST);
		
	if(!is_valid_ip(arg3))
		formatex(arg3, LEN_IP, "%s", IP_CONST);
	
	formatex(ban_names[idid], LEN_NAME, "%s", arg);
	formatex(ban_flex[idid], LEN_FLEX, "%s", arg2);
	formatex(ban_ip[idid], LEN_IP, "%s", arg3);
	ban_time[idid] = tm;
	formatex(ban_admin[idid], LEN_NAME, "%s", id_name[id]);
	formatex(ban_reason[idid], LEN_REASON, "%s", arg5);
	ban_ts[idid] = get_systime();
	
	console_print(id, "Igrachut e uspeshno bannat.");
	
	c_index[id] = idid;
	ban_details[id] = true;
	gag_details[id] = false;
	in_list[id] = 0;
	BanDetails(id);
	
	#if defined LOG_ENABLED
	log_to_file(srm_log, "%s addbanned %s (flex: %s; ip: %s; time: %s)", id_name[id], arg, arg2, arg3, arg4);
	#endif
	
	return PLUGIN_HANDLED;
}

public cmdAddGag(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 4))
		return PLUGIN_HANDLED;
	
	if(!(id_flags[id] & ADMIN_BAN) && !get_pcvar_num(vote_gag))
	{
		console_print(id, "You have no access to that command.");
		return PLUGIN_HANDLED;
	}
	
	new arg[LEN_NAME+1], arg2[LEN_FLEX+1], arg3[LEN_IP+1], arg4[LEN_TIME+1], arg5[LEN_REASON+1];
	read_argv(1, arg, LEN_NAME);
	read_argv(2, arg2, LEN_FLEX);
	read_argv(3, arg3, LEN_IP);
	read_argv(4, arg4, LEN_TIME);
	read_argv(5, arg5, LEN_REASON);
	
	for(new m = 1; m < maxplayers; m++)
	{
		if(!is_user_connected(m))
			continue;
			
		if((equali(id_name[m], arg) && !do_skip_name(arg)) || equali(id_ip[m], arg3) || (equali(id_flex[m], arg2) && !equali(id_flex[m], FLEX_CONST)))
		{
			console_cmd(id, "sr_gag #%d %s ^"%s^"", get_user_userid(m), arg4, arg5);
				
			return PLUGIN_HANDLED;
		}
	}
	
	new idid = FindEmptyIndex(GAG);
	
	if(idid == -1)
	{
		console_print(id, "Dostignat e maksimalniqt broi zapisi. Za poveche info - DragonClaw.");
		
		return PLUGIN_HANDLED;
	}
	
	new tm = determine_multiplier(arg4, id, GAG, PAUSED, API_OFF);
	
	if(!tm)
	{
		console_print(id, "Nevalidno chislo, vuvedeno za vreme.");
		
		return PLUGIN_HANDLED;
	}
	
	if(!is_valid_flex(arg2))
		formatex(arg2, LEN_FLEX, "%s", FLEX_CONST);
		
	if(!is_valid_ip(arg3))
		formatex(arg3, LEN_IP, "%s", IP_CONST);
	
	formatex(gag_names[idid], LEN_NAME, "%s", arg);
	formatex(gag_flex[idid], LEN_FLEX, "%s", arg2);
	formatex(gag_ip[idid], LEN_IP, "%s", arg3);
	gag_time[idid] = tm;
	formatex(gag_admin[idid], LEN_NAME, "%s", id_name[id]);
	formatex(gag_reason[idid], LEN_REASON, "%s", arg5);
	gag_ts[idid] = get_systime();
	
	console_print(id, "Igrachut e uspeshno gagnat.");
	
	c_index[id] = idid;
	ban_details[id] = false;
	gag_details[id] = true;
	in_list[id] = 0;
	GagDetails(id);
	
	#if defined LOG_ENABLED
	log_to_file(srm_log, "%s addgagged %s (flex: %s; ip: %s; time: %s)", id_name[id], arg, arg2, arg3, arg4);
	#endif
	
	return PLUGIN_HANDLED;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1026\\ f0\\ fs16 \n\\ par }
*/
