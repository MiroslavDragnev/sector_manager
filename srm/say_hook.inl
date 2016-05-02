public handleSay(id)
{
	if(c_gagged[id] || sl_gagged[id] || vip_gagged[id])
	{
		client_print(id, print_chat, "Gagnat si. Ne mojesh da pishesh i govorish.");
		
		return PLUGIN_HANDLED;
	}
	
	new SzMessages[192];
	read_args(SzMessages, charsmax(SzMessages));
	remove_quotes(SzMessages);
	
	if(!strlen(SzMessages))
		return PLUGIN_HANDLED;
		
	if(SzMessages[0] == '@')
		return PLUGIN_CONTINUE;
	
	if(ban_details[id])
	{
		new idid = c_index[id];
		
		if(equali(SzMessages, "reason:", 7))
		{
			if(!is_cmd_ban(ban_ip[idid]))
			{
				replace(SzMessages, charsmax(SzMessages), "reason:", "");
				copy(ban_reason[idid], LEN_REASON, SzMessages);
			}
			
			return PLUGIN_HANDLED;
		}
		
		new tm;
		
		switch(has_punishment_started(BAN, idid))
		{
			case true:tm = determine_multiplier(SzMessages, id, BAN, STARTED, API_OFF);
			default:tm = determine_multiplier(SzMessages, id, BAN, PAUSED, API_OFF);
		}
		
		if(!tm)
		{
			ColorChat(id, RED, "[%s v%s]^x01 Nevalidno chislo, vuvedeno za vreme.", PLUGIN, VERSION);
		
			return PLUGIN_HANDLED;
		}
		
		ban_time[idid] = tm;
		copy(ban_admin[idid], LEN_NAME, id_name[id]);
		ban_ts[idid] = get_systime();

		return PLUGIN_HANDLED;
	}
	else if(gag_details[id])
	{
		new idid = c_index[id];
		
		if(equali(SzMessages, "reason:", 7))
		{
			replace(SzMessages, charsmax(SzMessages), "reason:", "");
			copy(gag_reason[idid], LEN_REASON, SzMessages);
			
			return PLUGIN_HANDLED;
		}
		
		new tm;
		
		switch(has_punishment_started(GAG, idid))
		{
			case true:tm = determine_multiplier(SzMessages, id, GAG, STARTED, API_OFF);
			default:tm = determine_multiplier(SzMessages, id, GAG, PAUSED, API_OFF);
		}
		
		if(!tm)
		{
			ColorChat(id, RED, "[%s v%s]^x01 Nevalidno chislo, vuvedeno za vreme.", PLUGIN, VERSION);
		
			return PLUGIN_HANDLED;
		}
		
		gag_time[idid] = tm;
		copy(gag_admin[idid], LEN_NAME, id_name[id]);
		gag_ts[idid] = get_systime();

		return PLUGIN_HANDLED;
	}
	
	return PLUGIN_CONTINUE;
}

public handleSayTeam(id)
{
	if(c_gagged[id] || sl_gagged[id] || vip_gagged[id])
	{
		client_print(id, print_chat, "Gagnat si. Ne mojesh da pishesh i govorish.");
		
		return PLUGIN_HANDLED;
	}
	
	return PLUGIN_CONTINUE;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1026\\ f0\\ fs16 \n\\ par }
*/
