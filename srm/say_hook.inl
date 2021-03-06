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
	
	if(equali(SzMessages, "/listen", 7) && id_flags[id] & ADMIN_BAN)
	{
		switch(id_listen[id])
		{
			case true:
			{
				id_listen[id] = false;
				ColorChat(id, RED, "[%s v%s]^x01 Listening has been disabled.", PLUGIN, VERSION);
			}
			case false:
			{
				id_listen[id] = true;
				ColorChat(id, RED, "[%s v%s]^x01 Listening has been enabled.", PLUGIN, VERSION);
			}
		}
		
		return PLUGIN_HANDLED;
	}
	
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
		else if(equali(SzMessages, "flags:", 4) && is_cmd_ban(ban_ip[idid]))
		{
			replace(SzMessages, charsmax(SzMessages), "flags:", "");
			new str_flags[LEN_CMDBAN+1];
			format(str_flags, LEN_CMDBAN, "%s%s%s", containi(SzMessages, FLAG_DIE) > -1 ? FLAG_DIE:"", 
								containi(SzMessages, FLAG_GAG) > -1 ? FLAG_GAG:"", 
								containi(SzMessages, FLAG_SWAP) > -1 ? FLAG_SWAP:"");
			
			if(!str_flags[0])
			{
				ColorChat(id, RED, "[%s v%s]^x01 Nevalidni flagove.", PLUGIN, VERSION);
			}
			else
			{
				copy(ban_flex[idid], LEN_FLEX, str_flags);
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
