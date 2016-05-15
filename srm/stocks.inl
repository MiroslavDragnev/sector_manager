stock activity() return get_cvar_num("amx_show_activity");

stock is_network_punishment(const ip[]) return containi(ip, "x") > -1 ? 1:0;

stock is_cmd_ban(const ip[]) return equali(ip, CMD_CONST) ? 1:0;

stock is_cmd_banned(const id, const flags[]) return containi(cmdban[id], flags) > -1 ? 1:0;

stock get_network(const ip[])
{
	new temp[16], dots, len = strlen(ip);
	
	for(new i = 0; i < len; i++)
	{
		if(ip[i] == '.') dots++;
			
		if(dots == 2) break;
			
		temp[i] = ip[i];
	}
	
	return temp;
}

stock is_record_valid(const type, const index)
{
	new systime = get_systime();
	
	switch(type)
	{
		case ADMIN:if(!admin_lvl[index]) 		return 0;
		case BAN:if(0 <= ban_time[index] <= systime) 	return 0;
		case GAG:if(0 <= gag_time[index] <= systime)	return 0;
		case LAST:if(!last_ts[index]) 			return 0;
	}
	
	return 1;
}

stock is_punishment_permanent(const type, const index)
{
	switch(type)
	{
		case BAN:return ban_time[index] == -1 ? 1:0;
		case GAG:return gag_time[index] == -1 ? 1:0;
	}
	
	return 0;
}

stock has_punishment_started(const type, const index)
{
	switch(type)
	{
		case BAN:return ban_time[index] < -1 ? 0:1;
		case GAG:return gag_time[index] < -1 ? 0:1;
	}
	
	return 0;
}

stock is_valid_ip(const ip[])
{
	new tip[LEN_IP+1], dots, i, len;
	
	copy(tip, LEN_IP, ip);
	replace(tip, LEN_IP, "x", "");
	len = strlen(tip);
	
	for(i = 0; i < len; i++)
	{
		if(tip[i] == '.')
		{
			dots++;
			tip[i] = ' ';
		}
	}
	
	if(dots != 3)
		return 0;
		
	new temp[5][5];
	
	parse(tip, temp[0], 4, temp[1], 4, temp[2], 4, temp[3], 4, temp[4], 4);
	
	for(i = 0; i < 4; i++)	
	{
		if(!is_str_num(temp[i]))
			return 0;
			
		len = str_to_num(temp[i]);
		
		if(len < 0 || len > 255)
			return 0;
	}
	
	if(temp[4][0] != 0)
		return 0;
	
	return 1;
}

stock is_valid_flex(const flex[])
{
	if(strlen(flex) != LEN_FLEX)
		return 0;
		
	return 1;
}

stock determine_multiplier(const arg[], const id, const type, const started, const api)
{
	new string[LEN_TIME+1], m, systime = get_systime(), res;
	copy(string, LEN_TIME, arg);
	new len = strlen(arg)-1;
	
	if(string[len] == 'm')
	{
		replace(string, LEN_TIME, "m", "");
		m = 60;
	}
	else if(string[len] == 'h')
	{
		replace(string, LEN_TIME, "h", "");
		m = 3600;
	}
	else if(string[len] == 'd')
	{
		replace(string, LEN_TIME, "d", "");
		m = 86400
	}
	else if(string[len] == 'w')
	{
		replace(string, LEN_TIME, "w", "");
		m = 604800
	}
	else if(string[len] == 'M')
	{
		replace(string, LEN_TIME, "M", "");
		m = 2592000;
	}
	else m = 3600;
	
	if(!is_str_num(string))
		return 0;
	
	res = m * str_to_num(string);
	
	if(res <= 0 || res > LEN_PUNISHMENT)
		res = -1;
	
	if(!api)
	{
		switch(type)
		{
			case BAN:m = get_pcvar_num(vote_ban);
			case GAG:m = get_pcvar_num(vote_gag);
		}
				
		if(!(id_flags[id] & ADMIN_BAN) && (res == -1 || float(res)/3600 > m))
			res = m*3600;
	}
	
	if(res != -1)
	{
		if(started)
			res += systime;
		else
			res *= -1;
	}
	
	return res;
}

stock fix_score_team(const id, const TEAM[])
{
	emessage_begin(MSG_BROADCAST, g_iMsgTeamInfo);
	ewrite_byte(id);
	ewrite_string(TEAM);
	emessage_end();
}

stock send_ScoreAttrib(id, flags)
{
	message_begin(MSG_ALL, g_iMsgScoreAttrib, _, 0)
	write_byte(id)
	write_byte(flags)
	message_end()
}

stock do_skip_name(const name[])
{
	for(new j = 0; j < sizeof(popular_names); j++)
		if(equali(name, popular_names[j]))
			return 1;
	
	return 0;
}

stock AdminCheck(const id, const mode)
{
	for(new i = 0; i < MAX_ADMINS; i++)
	{
		if(!is_record_valid(ADMIN, i) || !equali(id_name[id], admin_names[i]) || !equali(id_flex[id], admin_flex[i]))
			continue;
		
		if(!mode)
		{
			new flags = read_flags(access_levels[admin_lvl[i]-1]);
				
			set_user_flags(id, flags);
						
			id_flags[id] = get_user_flags(id);
		}
		
		return true;
	}
	
	return false;
}

stock FindEmptyIndex(const list)
{
	new index = -1;
	
	switch(list)
	{
		case ADMIN:
		{
			for(new i = 0; i < MAX_ADMINS; i++)
			{
				if(!is_record_valid(ADMIN, i))
				{
					index = i;
					
					break;
				}
			}
		}
		case BAN:
		{
			for(new i = 0; i < MAX_ENTRIES; i++)
			{
				if(!ban_time[i]) // we do not use is_record_valid, because we want the list to be sorted at all times
				{
					index = i;
					
					break;
				}
			}
		}
		case GAG:
		{
			for(new i = 0; i < MAX_ENTRIES; i++)
			{
				if(!gag_time[i]) // we do not use is_record_valid, because we want the list to be sorted at all times
				{
					index = i;
					
					break;
				}
			}
		}
		case LAST:
		{
			new min = last_ts[0];
			index = 0;
			
			for(new i = 1; i < MAX_LAST; i++)
			{
				if(last_ts[i] < min)
				{
					min = last_ts[i];
					index = i;
				}
			}
		}
	}
	
	return index;
}

stock LastCheck(const id, const when)
{
	new name[LEN_NAME+1], i, found = -1, systime = get_systime();
	copy(name, LEN_NAME, id_name[id]);
	
	for(i = 0; i < MAX_LAST; i++)
	{
		if(!equali(name, last_names[i]))
			continue;
		
		found = i;
		break;
	}
	
	switch(when)
	{
		case LAST_CONNECT:
		{
			if(found > -1)
				last_ts[found] = 0;
		}
		
		case LAST_DISCONNECT:
		{
			if(found == -1)
				found = FindEmptyIndex(LAST);
				
			copy(last_names[found], LEN_NAME, name);
			copy(last_flex[found], LEN_FLEX, id_flex[id]);
			copy(last_ip[found], LEN_IP, id_ip[id]);
			last_ts[found] = systime;
		}
	}
	
	if(systime >= last_sort)
		BubbleSort(LAST);
}

stock UnixToString(const timestamp, const type)
{
	new tm[64], iYear, iMonth, iDay, iHour, iMinute, iSecond;
	UnixToTime(timestamp, iYear, iMonth, iDay, iHour, iMinute, iSecond);
	
	switch(type)
	{
		case LONG:formatex(tm, charsmax(tm), "%02d.%02d.%d - %02d:%02d:%02d", iDay, iMonth, iYear, iHour, iMinute, iSecond);
		case SHORT_DATE:formatex(tm, charsmax(tm), "%02d.%02d.%d", iDay, iMonth, iYear);
		case SHORT_TIME:formatex(tm, charsmax(tm), "%02d:%02d:%02d", iHour, iMinute, iSecond);
		case LONG_NOYS:formatex(tm, charsmax(tm), "%02d.%02d - %02d:%02d", iDay, iMonth, iHour, iMinute);
	}
	
	return tm;
}

stock GetPunishmentTimeleft(timestamp)
{
	new timeleft[33], dd, hh, mm, ss;
	
	dd = timestamp / 86400;
	timestamp -= dd*86400;
	hh = timestamp / 3600;
	timestamp -= hh*3600;
	mm = timestamp / 60;
	timestamp -= mm*60;
	ss = timestamp;
			
	formatex(timeleft, charsmax(timeleft), "%i:%02d:%02d:%02d", dd, hh, mm, ss);
	
	return timeleft;
}

stock BubbleSort(const type)
{
	switch(type)
	{
		case ADMIN:
		{
			new i, j, calc, temp_names[LEN_NAME+1], temp_flex[LEN_FLEX+1], temp_lvl;
			
			for(i = 0; i < MAX_ADMINS-1; i++)
			{
				calc = MAX_ADMINS - i - 1;
				
				for(j = 0; j < calc; j++)
				{
					if(admin_lvl[j] < admin_lvl[j+1]) //sort admins in decreasing order
					{
						copy(temp_names, LEN_NAME, admin_names[j]);
						copy(temp_flex, LEN_FLEX, admin_flex[j]);
						temp_lvl = admin_lvl[j];
						
						copy(admin_names[j], LEN_NAME, admin_names[j+1]);
						copy(admin_flex[j], LEN_FLEX, admin_flex[j+1]);
						admin_lvl[j] = admin_lvl[j+1];
						
						copy(admin_names[j+1], LEN_NAME, temp_names);
						copy(admin_flex[j+1], LEN_FLEX, temp_flex);
						admin_lvl[j+1] = temp_lvl;
					}
				}
			}
		}	
		
		case BAN:
		{
			new i, j, calc, temp_names[LEN_NAME+1], temp_flex[LEN_FLEX+1], temp_ip[LEN_IP+1], temp_time, temp_admin[LEN_NAME+1], temp_reason[LEN_REASON+1], temp_ts;
			
			for(i = 0; i < MAX_ENTRIES-1; i++)
			{
				calc = MAX_ENTRIES - i - 1;
				
				for(j = 0; j < calc; j++)
				{
					if(ban_ts[j] > ban_ts[j+1]) //sort bans in increasing order
					{
						copy(temp_names, LEN_NAME, ban_names[j]);
						copy(temp_flex, LEN_FLEX, ban_flex[j]);
						copy(temp_ip, LEN_IP, ban_ip[j]);
						temp_time = ban_time[j];
						copy(temp_admin, LEN_NAME, ban_admin[j]);
						copy(temp_reason, LEN_REASON, ban_reason[j]);
						temp_ts = ban_ts[j];
						
						copy(ban_names[j], LEN_NAME, ban_names[j+1]);
						copy(ban_flex[j], LEN_FLEX, ban_flex[j+1]);
						copy(ban_ip[j], LEN_IP, ban_ip[j+1]);
						ban_time[j] = ban_time[j+1];
						copy(ban_admin[j], LEN_NAME, ban_admin[j+1]);
						copy(ban_reason[j], LEN_REASON, ban_reason[j+1]);
						ban_ts[j] = ban_ts[j+1];
						
						copy(ban_names[j+1], LEN_NAME, temp_names);
						copy(ban_flex[j+1], LEN_FLEX, temp_flex);
						copy(ban_ip[j+1], LEN_IP, temp_ip);
						ban_time[j+1] = temp_time;
						copy(ban_admin[j+1], LEN_NAME, temp_admin);
						copy(ban_reason[j+1], LEN_REASON, temp_reason);
						ban_ts[j+1] = temp_ts;
					}
				}
			}
		}
		
		case GAG:
		{
			new i, j, calc, temp_names[LEN_NAME+1], temp_flex[LEN_FLEX+1], temp_ip[LEN_IP+1], temp_time, temp_admin[LEN_NAME+1], temp_reason[LEN_REASON+1], temp_ts;
			
			for(i = 0; i < MAX_ENTRIES-1; i++)
			{
				calc = MAX_ENTRIES - i - 1;
				
				for(j = 0; j < calc; j++)
				{
					if(gag_ts[j] > gag_ts[j+1]) //sort gags in increasing order
					{
						copy(temp_names, LEN_NAME, gag_names[j]);
						copy(temp_flex, LEN_FLEX, gag_flex[j]);
						copy(temp_ip, LEN_IP, gag_ip[j]);
						temp_time = gag_time[j];
						copy(temp_admin, LEN_NAME, gag_admin[j]);
						copy(temp_reason, LEN_REASON, gag_reason[j]);
						temp_ts = gag_ts[j];
						
						copy(gag_names[j], LEN_NAME, gag_names[j+1]);
						copy(gag_flex[j], LEN_FLEX, gag_flex[j+1]);
						copy(gag_ip[j], LEN_IP, gag_ip[j+1]);
						gag_time[j] = gag_time[j+1];
						copy(gag_admin[j], LEN_NAME, gag_admin[j+1]);
						copy(gag_reason[j], LEN_REASON, gag_reason[j+1]);
						gag_ts[j] = gag_ts[j+1];
						
						copy(gag_names[j+1], LEN_NAME, temp_names);
						copy(gag_flex[j+1], LEN_FLEX, temp_flex);
						copy(gag_ip[j+1], LEN_IP, temp_ip);
						gag_time[j+1] = temp_time;
						copy(gag_admin[j+1], LEN_NAME, temp_admin);
						copy(gag_reason[j+1], LEN_REASON, temp_reason);
						gag_ts[j+1] = temp_ts;
					}
				}
			}
		}
		
		case LAST:
		{
			new i, j, calc, temp_names[LEN_NAME+1], temp_flex[LEN_FLEX+1], temp_ip[LEN_IP+1], temp_ts;
			
			for(i = 0; i < MAX_LAST-1; i++)
			{
				calc = MAX_LAST - i - 1;
				
				for(j = 0; j < calc; j++)
				{
					if(last_ts[j] > last_ts[j+1]) //sort last in increasing order
					{
						copy(temp_names, LEN_NAME, last_names[j]);
						copy(temp_flex, LEN_FLEX, last_flex[j]);
						copy(temp_ip, LEN_IP, last_ip[j]);
						temp_ts = last_ts[j];
						
						copy(last_names[j], LEN_NAME, last_names[j+1]);
						copy(last_flex[j], LEN_FLEX, last_flex[j+1]);
						copy(last_ip[j], LEN_IP, last_ip[j+1]);
						last_ts[j] = last_ts[j+1];
						
						copy(last_names[j+1], LEN_NAME, temp_names);
						copy(last_flex[j+1], LEN_FLEX, temp_flex);
						copy(last_ip[j+1], LEN_IP, temp_ip);
						last_ts[j+1] = temp_ts;
					}
				}
			}
			
			for(i = 1; i <= maxplayers; i++)
				if(last_list[i])
					client_cmd(i, "say /last");
					
			last_sort = get_systime() + 3;
		}
	}
}

stock RemoveVipGag(const id)
{
	vip_gagged[id] = false;
	
	for(new i = 1; i <= maxplayers; i++)
	{
		for(new j = 0; j < 3; j++)
		{
			if(vip_gags[i][j] == id)
			{
				vip_gags[i][j] = 0;
				
				break;
			}
		}
	}
}

stock CopyToConsole(const id, const type)
{
	switch(type)
	{
		case BAN:
		{
			new index = c_index[id];
			
			if(is_cmd_ban(ban_ip[index]))
			{
				console_print(id, "^"%s^" - -", ban_names[index]);
				console_print(id, "sr_addban ^"%s^" - - 1m", ban_names[index]);
				console_print(id, "sr_addgag ^"%s^" - - 1m", ban_names[index]);
				console_print(id, "sr_cmdban ^"%s^" %s 1m", ban_names[index], ban_flex[index]);
			}
			else
			{
				console_print(id, "^"%s^" %s %s", ban_names[index], ban_flex[index], ban_ip[index]);
				console_print(id, "sr_addban ^"%s^" %s %s 1m", ban_names[index], ban_flex[index], ban_ip[index]);
				console_print(id, "sr_addgag ^"%s^" %s %s 1m", ban_names[index], ban_flex[index], ban_ip[index]);
				console_print(id, "sr_cmdban ^"%s^" dgs 1m", ban_names[index]);
			}
		}
		
		case GAG:
		{
			new index = c_index[id];
			
			console_print(id, "^"%s^" %s %s", gag_names[index], gag_flex[index], gag_ip[index]);
			console_print(id, "sr_addban ^"%s^" %s %s 1m", gag_names[index], gag_flex[index], gag_ip[index]);
			console_print(id, "sr_addgag ^"%s^" %s %s 1m", gag_names[index], gag_flex[index], gag_ip[index]);
			console_print(id, "sr_cmdban ^"%s^" dgs 1m", gag_names[index]);
		}
		
		case LAST:
		{
			console_print(id, "^"%s^" %s %s", target_names[id], target_flex[id], target_ip[id]);
			console_print(id, "sr_addban ^"%s^" %s %s 1m", target_names[id], target_flex[id], target_ip[id]);
			console_print(id, "sr_addgag ^"%s^" %s %s 1m", target_names[id], target_flex[id], target_ip[id]);
			console_print(id, "sr_cmdban ^"%s^" dgs 1m", target_names[id]);
		}
	}
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1026\\ f0\\ fs16 \n\\ par }
*/
