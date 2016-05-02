public BanCheck(id)
{
	new systime = get_systime();
	
	get_user_name(id, id_name[id], LEN_NAME);
	get_user_info(id, FLEX_PARAM, id_flex[id], LEN_FLEX);
	get_user_ip(id, id_ip[id], LEN_IP, 1);
	id_network[id] = get_network(id_ip[id]);
	get_user_authid(id, id_steam[id], LEN_STEAM);
	
	if(!is_valid_flex(id_flex[id]))
		formatex(id_flex[id], LEN_FLEX, "%s", FLEX_CONST);
	
	if(SkipCheck(id))
		return PLUGIN_CONTINUE;
	
	for(new i = 0; i < MAX_ENTRIES; i++)
	{
		if(!is_record_valid(BAN, i))
			continue;
			
		if(is_network_punishment(ban_ip[i]))
		{
			if(equali(id_network[id], get_network(ban_ip[i])))
			{
				if(!has_punishment_started(BAN, i))
					ban_time[i] = -1*ban_time[i] + systime;
					
				new is_minutes, tempt = floatround(float(ban_time[i]-systime)/60/60);
					
				if(tempt < 1)
				{
					tempt = floatround(float(ban_time[i]-systime)/60);
						
					is_minutes = true;
				}
					
				if(is_punishment_permanent(BAN, i))
					formatex(info_msg[id], LEN_INFO, "Bannat si permanentno na %s ot %s.", UnixToString(ban_ts[i], SHORT_DATE), ban_admin[i]);
				else
					formatex(info_msg[id], LEN_INFO, "Bannat si na %s ot %s, ostavashto vreme: %i %s.", UnixToString(ban_ts[i], SHORT_DATE), ban_admin[i], tempt, is_minutes ? "minuti":"chas(a)");
					
				if(ban_reason[i][0])
					formatex(info_msg[id], LEN_INFO, "%s Prichina: %s.", info_msg[id], ban_reason[i]);
					
				console_print(id, info_msg[id]);
					
				server_cmd("kick #%d ^"%s^"", get_user_userid(id), info_msg[id]);
					
				break;
			}
		}		
		
		if((equali(id_name[id], ban_names[i]) && !do_skip_name(id_name[id])) && is_cmd_ban(ban_ip[i]))
		{
			if(!has_punishment_started(BAN, i))
				ban_time[i] = -1*ban_time[i] + systime;			
			
			if(is_punishment_permanent(BAN, i) || ban_time[i] > systime)	
				formatex(cmdban[id], LEN_CMDBAN, "%s", ban_flex[i]);
			
			continue;
		}
		
		if((equali(id_name[id], ban_names[i]) && !do_skip_name(id_name[id])) || equali(id_ip[id], ban_ip[i]) || (equali(id_flex[id], ban_flex[i]) && !equali(id_flex[id], FLEX_CONST)))
		{	
			if(equali(ban_flex[i], FLEX_CONST) && !equali(id_flex[id], FLEX_CONST))
				formatex(ban_flex[i], LEN_FLEX, "%s", id_flex[id]);
			
			if(equali(ban_ip[i], IP_CONST))
				formatex(ban_ip[i], LEN_IP, "%s", id_ip[id]);
			
			if(!has_punishment_started(BAN, i))
				ban_time[i] = -1*ban_time[i] + systime;
				
			new is_minutes, tempt = floatround(float(ban_time[i]-systime)/60/60);
					
			if(tempt < 1)
			{
				tempt = floatround(float(ban_time[i]-systime)/60);
						
				is_minutes = true;
			}
				
			if(is_punishment_permanent(BAN, i))
				formatex(info_msg[id], LEN_INFO, "Bannat si permanentno na %s ot %s.", UnixToString(ban_ts[i], SHORT_DATE), ban_admin[i]);
			else
				formatex(info_msg[id], LEN_INFO, "Bannat si na %s ot %s, ostavashto vreme: %i %s.", UnixToString(ban_ts[i], SHORT_DATE), ban_admin[i], tempt, is_minutes ? "minuti":"chas(a)");
					
			if(ban_reason[i][0])
				formatex(info_msg[id], LEN_INFO, "%s Prichina: %s.", info_msg[id], ban_reason[i]);
					
			console_print(id, info_msg[id]);
					
			server_cmd("kick #%d ^"%s^"", get_user_userid(id), info_msg[id]);
				
			break;
		}
	}
	
	return PLUGIN_CONTINUE;
}

public GagCheck(id)
{
	if(SkipCheck(id))
		return PLUGIN_CONTINUE;
	
	new i, systime = get_systime();
	
	for(i = 0; i < MAX_ENTRIES; i++)
	{
		if(!is_record_valid(GAG, i))
			continue;
			
		if(is_network_punishment(gag_ip[i]))
		{
			if(equali(id_network[id], get_network(gag_ip[i])))
			{
				if(!has_punishment_started(GAG, i))
					gag_time[i] = -1*gag_time[i] + systime;
					
				new is_minutes, tempt = floatround(float(gag_time[i]-systime)/60/60);
					
				if(tempt < 1)
				{
					tempt = floatround(float(gag_time[i]-systime)/60);
						
					is_minutes = true;
				}
					
				if(is_punishment_permanent(GAG, i))
					formatex(info_msg[id], LEN_INFO, "[GAG INFO]^x01 Gagnat si permanentno na %s ot %s.", UnixToString(gag_ts[i], SHORT_DATE), gag_admin[i]);
				else
					formatex(info_msg[id], LEN_INFO, "[GAG INFO]^x01 Gagnat si na %s ot %s, ostavashto vreme: %i %s.", UnixToString(gag_ts[i], SHORT_DATE), gag_admin[i], tempt, is_minutes ? "minuti":"chas(a)");
					
				if(gag_reason[i][0])
					formatex(info_msg[id], LEN_INFO, "%s Prichina: %s.", info_msg[id], gag_reason[i]);
					
				c_gagged[id] = true;
				client_cmd(id, "-voicerecord");
					
				set_task(8.0, "ShowGagMsg", id);
					
				break;
			}
		}		
		
		if((equali(id_name[id], gag_names[i]) && !do_skip_name(id_name[id])) || equali(id_ip[id], gag_ip[i]) || (equali(id_flex[id], gag_flex[i]) && !equali(id_flex[id], FLEX_CONST)))
		{	
			if(equali(gag_flex[i], FLEX_CONST) && !equali(id_flex[id], FLEX_CONST))
				formatex(gag_flex[i], LEN_FLEX, "%s", id_flex[id]);
			
			if(equali(gag_ip[i], IP_CONST))
				formatex(gag_ip[i], LEN_IP, "%s", id_ip[id]);
			
			if(!has_punishment_started(GAG, i))
				gag_time[i] = -1*gag_time[i] + systime;
		
			new is_minutes, tempt = floatround(float(gag_time[i]-systime)/60/60);
					
			if(tempt < 1)
			{
				tempt = floatround(float(gag_time[i]-systime)/60);
						
				is_minutes = true;
			}
				
			if(is_punishment_permanent(GAG, i))
				formatex(info_msg[id], LEN_INFO, "[GAG INFO]^x01 Gagnat si permanentno na %s ot %s.", UnixToString(gag_ts[i], SHORT_DATE), gag_admin[i]);
			else
				formatex(info_msg[id], LEN_INFO, "[GAG INFO]^x01 Gagnat si na %s ot %s, ostavashto vreme: %i %s.", UnixToString(gag_ts[i], SHORT_DATE), gag_admin[i], tempt, is_minutes ? "minuti":"chas(a)");
					
			if(gag_reason[i][0])
				formatex(info_msg[id], LEN_INFO, "%s Prichina: %s.", info_msg[id], gag_reason[i]);
					
			c_gagged[id] = true;
			client_cmd(id, "-voicerecord");
					
			set_task(8.0, "ShowGagMsg", id);
				
			break;
		}
	}
	
	if(!c_gagged[id])
	{
		new temp[LEN_NAME+1], sz = ArraySize(slay_gag);
		
		for(i = 0; i < sz; i++)
		{
			ArrayGetString(slay_gag, i, temp, LEN_NAME);
			
			if(equali(id_name[id], temp))
			{
				sl_gagged[id] = true;
				
				break;
			}
		}
	}
	
	return PLUGIN_CONTINUE;
}

public ShowGagMsg(id)
{
	ColorChat(id, RED, info_msg[id]);
	replace(info_msg[id], LEN_INFO, "^x01", "");
	console_print(id, info_msg[id]);
}

public SecondaryCheck(const type, const name[], const fid[], const ip[])
{
	new i, network[LEN_NETWORK+1];
	network = get_network(ip);
	
	switch(type)
	{
		case BAN:
		{
			for(i = 0; i < MAX_ENTRIES; i++)
			{
				if(!is_record_valid(BAN, i))
					continue;
					
				if(is_network_punishment(ban_ip[i]))
					if(equali(network, get_network(ban_ip[i])))
						return i;		
				
				if((equali(name, ban_names[i]) && !do_skip_name(name)) || (equali(ip, ban_ip[i]) && !equali(ip, IP_CONST)) || (equali(fid, ban_flex[i]) && !equali(fid, FLEX_CONST)))	
					return i;
			}
		}
		
		case GAG:
		{
			for(i = 0; i < MAX_ENTRIES; i++)
			{
				if(!is_record_valid(GAG, i))
					continue;
					
				if(is_network_punishment(gag_ip[i]))
					if(equali(network, get_network(gag_ip[i])))
						return i;		
				
				if((equali(name, gag_names[i]) && !do_skip_name(name)) || (equali(ip, gag_ip[i]) && !equali(ip, IP_CONST)) || (equali(fid, gag_flex[i]) && !equali(fid, FLEX_CONST)))	
					return i;
			}
		}
		
		case CMD_BAN:
		{
			for(i = 0; i < MAX_ENTRIES; i++)
			{
				if(!is_record_valid(BAN, i))
					continue;
					
				if((equali(name, ban_names[i]) && !do_skip_name(name)) && is_cmd_ban(ban_ip[i]))
					return i;
			}
		}
	}
	
	return -1;
}

public CheckPlayers(const type, const admin, const name[], const fid[], const ip[])
{
	switch(type)
	{
		case GAG:
		{
			for(new i = 1; i <= maxplayers; i++)
			{
				if((equali(id_name[i], name) || equali(id_flex[i], fid) || equali(id_ip[i], ip)) && c_gagged[i])
				{
					c_gagged[i] = false;
					
					switch(activity())
					{
						case 1:ColorChat(0, RED, "[ADMIN]^x01 ungagged %s.", id_name[i]);
						case 2:ColorChat(0, RED, "[ADMIN]^x01 %s ungagged %s.", id_name[admin], id_name[i]);
					}
				}
			}
		}
		
		case CMD_BAN:
		{
			for(new i = 1; i <= maxplayers; i++)
			{
				if(equali(id_name[i], name) && cmdban[i][0])
				{
					formatex(cmdban[i], LEN_CMDBAN, "");
				}
			}
		}
	}
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1026\\ f0\\ fs16 \n\\ par }
*/
