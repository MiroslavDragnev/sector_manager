public cmdBanList(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 1))
		return PLUGIN_HANDLED
	
	new menu = menu_create("\rBanlist:^n\wIzberi ime za poveche info^n\rStranica: \w", "banlist_handler");
	
	new item[64], count_str[3], count;
	
	for(new i = MAX_ENTRIES - 1; i >= 0; i--)
	{
		if(!is_record_valid(BAN, i))
			continue;
		
		count++;
		
		if(has_punishment_started(BAN, i))
			formatex(item, charsmax(item), "\d%s: %s\w%s", UnixToString(ban_ts[i], SHORT_DATE), is_cmd_ban(ban_ip[i]) == 1?"\r(\wCMD\r)\w ":"", ban_names[i]);
		else
			formatex(item, charsmax(item), "\d%s: %s\y%s", UnixToString(ban_ts[i], SHORT_DATE), is_cmd_ban(ban_ip[i]) == 1?"\r(\wCMD\r)\w ":"", ban_names[i]);
		            
		num_to_str(count, count_str, 2);
		
		menu_additem(menu, item, count_str, 0);
		
		menu_indexes[id][count-1] = i;
	}
	
	menu_setprop(menu, MPROP_EXIT, MEXIT_ALL);
	menu_display(id, menu, id_page[id][BAN]);
	
	in_list[id] = BAN;
	
	return PLUGIN_HANDLED;
}

public banlist_handler(id,menu,item)
{
	if(item == MENU_EXIT)
	{
		in_list[id] = 0;
		menu_destroy(menu)
		return PLUGIN_HANDLED
	}
	
	new access, callback, info[3];
	menu_item_getinfo(menu, item, access, info, 2, _, _, callback);
	
	c_index[id] = menu_indexes[id][item];
	id_page[id][BAN] = item / 7;
	ban_details[id] = true;
	BanDetails(id);
	in_list[id] = 0;
	menu_destroy(menu);
	
	return PLUGIN_HANDLED
}

public BanDetails(id)
{	
	if(!ban_details[id])
		return PLUGIN_HANDLED;
	
	new title[384], timeleft[33], stemp, network[LEN_IP+1], since[64];
	
	new idid = c_index[id];
	new systime = get_systime();
	new started = has_punishment_started(BAN, idid);
	new isnetwork = is_network_punishment(ban_ip[idid]);
	new isperm = is_punishment_permanent(BAN, idid);
	new isvalidip = is_valid_ip(ban_ip[idid]);
	new iscmd = is_cmd_ban(ban_ip[idid]);
	since = UnixToString(ban_ts[idid], LONG)
	
	if(isnetwork)
		formatex(network, LEN_IP, "%s.x.x", get_network(ban_ip[idid]));
	
	if(isperm)
	{
		formatex(timeleft, charsmax(timeleft), "permanenten");
	}
	else
	{
		if(started)
		{
			stemp = ban_time[idid] - systime;
			
			if(stemp <= 0)
			{
				ban_details[id] = false;
						
				client_cmd(id, "sr_banlist");
						
				return PLUGIN_HANDLED;
			}
		}
		else
		{
			stemp = -1*ban_time[idid];
		}
		
		timeleft = GetPunishmentTimeleft(stemp);
	}
	
	if(iscmd)
	{
		if(started) formatex(title, charsmax(title), "\rName: \w%s^n\rFlags: \w%s^n\rBanned on: \w%s^n\rTime left: \w%s^n\rBanned by: \w%s", ban_names[idid], ban_flex[idid], since, timeleft, ban_admin[idid]);
		else formatex(title, charsmax(title), "\rName: \y%s^n\rFlags: \y%s^n\rBanned on: \y%s^n\rTime left: \y%s^n\rBanned by: \y%s", ban_names[idid], ban_flex[idid], since, timeleft, ban_admin[idid]);
	}
	else
	{
		if(started) formatex(title, charsmax(title), "\rName: \w%s^n\rFlexID: \w%s^n\rIP: \w%s^n\rBanned on: \w%s^n\rTime left: \w%s^n\rBanned by: \w%s^n\rReason: \w%s", ban_names[idid], ban_flex[idid], isnetwork ? network:ban_ip[idid], since, timeleft, ban_admin[idid], ban_reason[idid]);
		else formatex(title, charsmax(title), "\rName: \y%s^n\rFlexID: \y%s^n\rIP: \y%s^n\rBanned on: \y%s^n\rTime left: \y%s^n\rBanned by: \y%s^n\rReason: \y%s", ban_names[idid], ban_flex[idid], isnetwork ? network:ban_ip[idid], since, timeleft, ban_admin[idid], ban_reason[idid]);
	}
	
	new menu = menu_create(title, "bandetails_handler");
	
	if(id_flags[id] & ADMIN_BAN)
	{
		menu_additem(menu, "\wVurni se kum spisuka", "1", 0);
		menu_additem(menu, "\wZatvori menuto^n", "2", 0);
		
		menu_additem(menu, "\wPremahni bana", "3", 0);
			
		if(isnetwork) menu_additem(menu, "\wPremahni bana na mrejata^n", "4", 0);
		else isvalidip == 1 ? menu_additem(menu, "\wBanni mrejata^n", "4", 0) : menu_additem(menu, "\dBanni mrejata^n", "4", 0);
		
		menu_additem(menu, "\wKopirai v konzolata", "5", 0);
	}
	else if(equali(id_name[id], ban_admin[idid]) && get_pcvar_num(vote_ban) && id_flags[id] & ADMIN_VOTE)
	{
		menu_additem(menu, "\wVurni se kum spisuka", "1", 0);
		menu_additem(menu, "\wZatvori menuto^n", "2", 0);
		
		menu_additem(menu, "\wPremahni bana", "3", 0);
		menu_additem(menu, "\dBanni mrejata^n", "4", 0);
		
		menu_additem(menu, "\wKopirai v konzolata", "5", 0);
	}
	else
	{
		menu_additem(menu, "\wVurni se kum spisuka", "1", 0);
		menu_additem(menu, "\wZatvori menuto^n", "2", 0);
		
		menu_additem(menu, "\dPremahni bana", "3", 0);
		menu_additem(menu, "\dBanni mrejata^n", "4", 0);
		
		menu_additem(menu, "\wKopirai v konzolata", "5", 0);
	}
	
	menu_setprop(menu, MPROP_EXIT, MEXIT_NEVER);
	menu_display(id, menu, 0);

	set_task(1.0, "BanDetails", id);
	
	return PLUGIN_HANDLED;
}

public bandetails_handler( id, menu, item )
{
	new data[ 6 ], access, callback, key;
	menu_item_getinfo( menu, item, access, data, charsmax(data), _, _, callback );
	key = str_to_num( data );
	
	switch(key)
	{
		case 1:
		{
			ban_details[id] = false;
			
			console_cmd(id, "sr_banlist");
		}
		
		case 2:
		{
			ban_details[id] = false;
		}
		
		case 3:
		{
			new idid = c_index[id];
			
			if(!(id_flags[id] & ADMIN_BAN) && (!equali(id_name[id], ban_admin[idid]) || !get_pcvar_num(vote_ban)))
				return PLUGIN_HANDLED;
			
			new still_exists, iscmd = is_cmd_ban(ban_ip[idid]);
			
			ban_time[idid] = 1;
			
			if(iscmd)
			{
				still_exists = SecondaryCheck(CMD_BAN, ban_names[idid], ban_flex[idid], ban_ip[idid]);
				
				ColorChat(id, RED, "[%s v%s]^x01 Banut na %s e uspeshno premahnat.", PLUGIN, VERSION, ban_names[idid]);
				
				#if defined LOG_ENABLED
				log_to_file(srm_log, "%s unbanned %s (flex: %s; ip: %s)", id_name[id], ban_names[idid], ban_flex[idid], ban_ip[idid]);
				#endif
			}
			else
			{
				still_exists = SecondaryCheck(BAN, ban_names[idid], ban_flex[idid], ban_ip[idid]);
				
				ColorChat(id, RED, "[%s v%s]^x01 Banut na %s e uspeshno premahnat.", PLUGIN, VERSION, ban_names[idid]);
				
				#if defined LOG_ENABLED
				log_to_file(srm_log, "%s unbanned %s (flex: %s; ip: %s)", id_name[id], ban_names[idid], ban_flex[idid], ban_ip[idid]);
				#endif
			}
			
			if(still_exists > -1)
			{
				ColorChat(id, RED, "[%s v%s]^x01 Sushtestvuva i drug zapis sus suvpadashti danni.", PLUGIN, VERSION);
				c_index[id] = still_exists;
				BanDetails(id);
			}
			else
			{
				ban_details[id] = false;
				console_cmd(id, "sr_banlist");
				
				if(iscmd)
					CheckPlayers(CMD_BAN, id, ban_names[idid], ban_flex[idid], ban_ip[idid]);
			}
		}
		
		case 4:
		{	
			new idid = c_index[id];
		
			if(!(id_flags[id] & ADMIN_BAN) || !is_valid_ip(ban_ip[idid]))
				return PLUGIN_HANDLED;
		
			if(is_network_punishment(ban_ip[idid]))
			{
				replace(ban_ip[idid], LEN_IP, "x", "");
				
				ColorChat(id, RED, "[%s v%s]^x01 Mrejata e uspeshno unbannata.", PLUGIN, VERSION);
				
				#if defined LOG_ENABLED
				log_to_file(srm_log, "%s unbanned network %s", id_name[id], get_network(ban_ip[idid]));
				#endif
			}
			else
			{
				format(ban_ip[idid], LEN_IP, "%sx", ban_ip[idid]);
				
				ColorChat(id, RED, "[%s v%s]^x01 Mrejata e uspeshno bannata.", PLUGIN, VERSION);
					
				#if defined LOG_ENABLED
				log_to_file(srm_log, "%s banned network %s", id_name[id], get_network(ban_ip[idid]));
				#endif
			}
		}
		
		case 5:
		{
			CopyToConsole(id, BAN);
		}
	}
	
	menu_destroy( menu );
	
	return PLUGIN_HANDLED;
}

public cmdGagList(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 1))
		return PLUGIN_HANDLED
	
	new menu = menu_create("\rGaglist:^n\wIzberi ime za poveche info^n\rStranica: \w", "gaglist_handler");
	
	new item[64], count_str[3], count;
	
	for(new i = MAX_ENTRIES - 1; i >= 0; i--)
	{
		if(!is_record_valid(GAG, i))
			continue;
		
		count++;
		
		if(has_punishment_started(GAG, i))
			formatex(item, charsmax(item), "\d%s: \w%s", UnixToString(gag_ts[i], SHORT_DATE), gag_names[i]);
		else
			formatex(item, charsmax(item), "\d%s: \y%s", UnixToString(gag_ts[i], SHORT_DATE), gag_names[i]);
		
		num_to_str(count, count_str, 2);
		
		menu_additem(menu, item, count_str, 0);
		
		menu_indexes[id][count-1] = i;
	}
	
	menu_setprop(menu, MPROP_EXIT, MEXIT_ALL);
	menu_display(id, menu, id_page[id][GAG]);
	
	in_list[id] = GAG;
	
	return PLUGIN_HANDLED;
}

public gaglist_handler(id,menu,item)
{
	if(item == MENU_EXIT)
	{
		in_list[id] = 0;
		menu_destroy(menu)
		return PLUGIN_HANDLED
	}
	
	new access,callback, info[3];
	menu_item_getinfo(menu,item,access, info,2, _,_, callback)
	
	c_index[id] = menu_indexes[id][item];
	id_page[id][GAG] = item / 7;
	gag_details[id] = true;
	GagDetails(id);
	in_list[id] = 0;
	menu_destroy(menu);
	
	return PLUGIN_HANDLED
}

public GagDetails(id)
{	
	if(!gag_details[id])
		return PLUGIN_HANDLED;
	
	new title[384], timeleft[33], stemp, network[LEN_IP+1], since[64];
	
	new idid = c_index[id];
	new systime = get_systime();
	new started = has_punishment_started(GAG, idid);
	new isnetwork = is_network_punishment(gag_ip[idid]);
	new isperm = is_punishment_permanent(GAG, idid);
	new isvalidip = is_valid_ip(gag_ip[idid]);
	since = UnixToString(gag_ts[idid], LONG);
	
	if(isnetwork)
		formatex(network, LEN_IP, "%s.x.x", get_network(gag_ip[idid]));
	
	if(isperm)
	{
		formatex(timeleft, charsmax(timeleft), "permanenten");
	}
	else
	{
		if(started)
		{
			stemp = gag_time[idid] - systime;
			
			if(stemp <= 0)
			{
				gag_details[id] = false;
						
				client_cmd(id, "sr_gaglist");
						
				return PLUGIN_HANDLED;
			}
		}
		else
		{
			stemp = -1*gag_time[idid];
		}
		
		timeleft = GetPunishmentTimeleft(stemp);
	}

	if(started) formatex(title, charsmax(title), "\rName: \w%s^n\rFlexID: \w%s^n\rIP: \w%s^n\rGagged on: \w%s^n\rTime left: \w%s^n\rGagged by: \w%s^n\rReason: \w%s", gag_names[idid], gag_flex[idid], isnetwork ? network:gag_ip[idid], since, timeleft, gag_admin[idid], gag_reason[idid]);
	else formatex(title, charsmax(title), "\rName: \y%s^n\rFlexID: \y%s^n\rIP: \y%s^n\rGagged on: \y%s^n\rTime left: \y%s^n\rGagged by: \y%s^n\rReason: \y%s", gag_names[idid], gag_flex[idid], isnetwork ? network:gag_ip[idid], since, timeleft, gag_admin[idid], gag_reason[idid]);
	
	new menu = menu_create(title, "gagdetails_handler");
	
	if(id_flags[id] & ADMIN_BAN)
	{
		menu_additem(menu, "\wVurni se kum spisuka", "1", 0);
		menu_additem(menu, "\wZatvori menuto^n", "2", 0);
		
		menu_additem(menu, "\wPremahni gaga", "3", 0);
			
		if(isnetwork) menu_additem(menu, "\wPremahni gaga na mrejata^n", "4", 0);
		else isvalidip == 1 ? menu_additem(menu, "\wGagni mrejata^n", "4", 0) : menu_additem(menu, "\dGagni mrejata^n", "4", 0);
		
		menu_additem(menu, "\wKopirai v konzolata", "5", 0);
	}
	else if(equali(id_name[id], gag_admin[idid]) && get_pcvar_num(vote_gag) && id_flags[id] & ADMIN_VOTE)
	{
		menu_additem(menu, "\wVurni se kum spisuka", "1", 0);
		menu_additem(menu, "\wZatvori menuto^n", "2", 0);
		
		menu_additem(menu, "\wPremahni gaga", "3", 0);
		menu_additem(menu, "\dGagni mrejata^n", "4", 0);
		
		menu_additem(menu, "\wKopirai v konzolata", "5", 0);
	}
	else
	{
		menu_additem(menu, "\wVurni se kum spisuka", "1", 0);
		menu_additem(menu, "\wZatvori menuto^n", "2", 0);
		
		menu_additem(menu, "\dPremahni gaga", "3", 0);
		menu_additem(menu, "\dGagni mrejata^n", "4", 0);
		
		menu_additem(menu, "\wKopirai v konzolata", "5", 0);
	}
	
	menu_setprop(menu, MPROP_EXIT, MEXIT_NEVER);
	menu_display(id, menu, 0);

	set_task(1.0, "GagDetails", id);
	
	return PLUGIN_HANDLED;
}

public gagdetails_handler( id, menu, item )
{
	new data[ 6 ], access, callback, key;
	menu_item_getinfo( menu, item, access, data, charsmax(data), _, _, callback );
	key = str_to_num( data );
	
	switch(key)
	{
		case 1:
		{
			gag_details[id] = false;
			
			console_cmd(id, "sr_gaglist");
		}
		
		case 2:
		{
			gag_details[id] = false;
		}
		
		case 3:
		{
			new idid = c_index[id];
			
			if(!(id_flags[id] & ADMIN_BAN) && (!equali(id_name[id], gag_admin[idid]) || !get_pcvar_num(vote_gag)))
				return PLUGIN_HANDLED;
			
			gag_time[idid] = 1;
			
			new still_exists = SecondaryCheck(GAG, gag_names[idid], gag_flex[idid], gag_ip[idid]);
			
			ColorChat(id, RED, "[%s v%s]^x01 Gagut na %s e uspeshno premahnat.", PLUGIN, VERSION, gag_names[idid]);
				
			#if defined LOG_ENABLED
			log_to_file(srm_log, "%s ungagned %s (flex: %s; ip: %s)", id_name[id], gag_names[idid], gag_flex[idid], gag_ip[idid]);
			#endif
			
			if(still_exists > -1)
			{
				ColorChat(id, RED, "[%s v%s]^x01 Sushtestvuva i drug zapis sus suvpadashti danni.", PLUGIN, VERSION);
				c_index[id] = still_exists;
				GagDetails(id);
			}
			else
			{
				gag_details[id] = false;
				console_cmd(id, "sr_gaglist");
				CheckPlayers(GAG, id, gag_names[idid], gag_flex[idid], gag_ip[idid]);
			}
		}
		
		case 4:
		{		
			new idid = c_index[id];
		
			if(!(id_flags[id] & ADMIN_BAN) || !is_valid_ip(gag_ip[idid]))
				return PLUGIN_HANDLED;
		
			if(is_network_punishment(gag_ip[idid]))
			{
				replace(gag_ip[idid], LEN_IP, "x", "");
				
				ColorChat(id, RED, "[%s v%s]^x01 Mrejata e uspeshno ungagnata.", PLUGIN, VERSION);
				
				#if defined LOG_ENABLED
				log_to_file(srm_log, "%s ungagned network %s", id_name[id], get_network(gag_ip[idid]));
				#endif
			}
			else
			{
				format(gag_ip[idid], LEN_IP, "%sx", gag_ip[idid]);
				
				ColorChat(id, RED, "[%s v%s]^x01 Mrejata e uspeshno gagnata.", PLUGIN, VERSION);
					
				#if defined LOG_ENABLED
				log_to_file(srm_log, "%s gagned network %s", id_name[id], get_network(gag_ip[idid]));
				#endif
			}
		}
		
		case 5:
		{
			CopyToConsole(id, GAG);
		}
	}
	
	menu_destroy( menu );
	
	return PLUGIN_HANDLED;
}

public cmdSayLast(id, level, cid)
{
	if(!cmd_access(id, level, cid, 1))
		return PLUGIN_HANDLED;
	
	new menu = menu_create("\wSpisukut se obnovqva avtomatichno^n^n\rLast:^n\wIzberi ime za poveche opcii^n\rStranica: \w", "lastlist_handler");
	
	new item[64], count_str[3], count, key[LEN_NAME+1];
	copy(key, LEN_NAME, last_search_key[id]);
	new searched = key[0] == 0 ? false : true;
	new type = (key[0] == '"' && key[strlen(key)-1] == '"') ? 1:0;
	remove_quotes(key);
	
	switch(searched)
	{
		case false:
		{
			menu_additem(menu, "Search^n", "1", 0);
			count = 1;
		}
		default:
		{
			menu_additem(menu, "Search again", "1", 0);
			formatex(item, charsmax(item), "Back^n^nShowing results for ^"%s^"^n", key);
			menu_additem(menu, item, "2", 0);
			count = 2;
		}
	}
	
	for(new i = MAX_LAST - 1; i >= 0; i--)
	{
		if(!is_record_valid(LAST, i))
			continue;
		
		if(searched)
		{
			switch(type)
			{
				case 0:
				{
					if(containi(last_names[i], key) == -1 && containi(last_flex[i], key) == -1 && containi(last_ip[i], key) == -1)
						continue;
				}
				default:
				{
					if(!equali(last_names[i], key) && !equali(last_flex[i], key) && !equali(last_ip[i], key))
						continue;
				}
			}
		}
		
		count++;
		
		formatex(item, charsmax(item), "\d%s: \w%s", UnixToString(last_ts[i], LONG_NOYS), last_names[i]);
		
		num_to_str(count, count_str, 2);
		
		menu_additem(menu, item, count_str, 0);
		
		menu_indexes[id][searched == 1 ? count-3:count-2] = i;
	}
	
	menu_setprop(menu, MPROP_EXIT, MEXIT_ALL);
	menu_setprop(menu, MPROP_BACKNAME, "Go Back");
	menu_display(id, menu, searched == 1 ? 0:id_page[id][LAST]);
	
	in_list[id] = LAST;
	
	return PLUGIN_HANDLED;
}

public lastlist_handler(id,menu,item)
{	
	if(item == MENU_EXIT)
	{
		in_list[id] = 0;
		menu_destroy(menu);
		return PLUGIN_HANDLED;
	}
	
	new access,callback, info[3];
	menu_item_getinfo(menu,item,access, info,2, _,_, callback)
	
	new key[LEN_NAME+1];
	copy(key, LEN_NAME, last_search_key[id]);
	new searched = key[0] == 0 ? false : true;
	
	if(item == 0)
	{
		client_cmd(id, "messagemode _____SEARCH_FOR");
		last_search_key[id][0] = 0;
	}
	else if(item == 1 && searched)
	{
		last_search_key[id][0] = 0;
		client_cmd(id, "say /last");
	}
	else
	{
		new num = searched == 1 ? item - 2 : item - 1;
		new idid = menu_indexes[id][num];
		copy(target_names[id], LEN_NAME, last_names[idid]);
		copy(target_flex[id], LEN_FLEX, last_flex[idid]);
		copy(target_ip[id], LEN_IP, last_ip[idid]);
		target_ts[id] = last_ts[idid];
		id_page[id][LAST] = item / 7;
		LastDetails(id);
	}
	
	in_list[id] = 0;
	menu_destroy(menu);
	
	return PLUGIN_HANDLED;
}

public LastDetails(id)
{
	new title[384];
	
	formatex(title, charsmax(title), "\rName: \w%s^n\rFlexID: \w%s^n\rIP: \w%s^n\rLast Online: \w%s", target_names[id], target_flex[id], target_ip[id], UnixToString(target_ts[id], LONG));
	
	new menu = menu_create(title, "lastdetails_handler");
	
	menu_additem(menu, "\wVurni se kum spisuka", "1", 0);
	menu_additem(menu, "\wZatvori menuto^n", "2", 0);

	menu_additem(menu, "\wBan", "3", 0);
	menu_additem(menu, "\wGag", "4", 0);
	menu_additem(menu, "\wCMD Ban^n", "5", 0);
	menu_additem(menu, "\wKopirai v konzolata", "6", 0);
	
	menu_setprop(menu, MPROP_EXIT, MEXIT_NEVER);
	menu_display(id, menu, 0);
	
	return PLUGIN_HANDLED;
}

public lastdetails_handler( id, menu, item )
{
	new data[ 6 ], access, callback, key;
	menu_item_getinfo( menu, item, access, data, charsmax(data), _, _, callback );
	key = str_to_num( data );
	
	switch(key)
	{
		case 1:
		{
			client_cmd(id, "say /last");
		}
		
		case 2:{ }
		
		case 3:
		{
			if(id_flags[id] & ADMIN_BAN ||
			(id_flags[id] & ADMIN_VOTE && get_pcvar_num(vote_ban)))
				console_cmd(id, "sr_addban ^"%s^" %s %s 1m", target_names[id], target_flex[id], target_ip[id]);
			else LastDetails(id);
		}
		
		case 4:
		{
			if(id_flags[id] & ADMIN_BAN ||
			(id_flags[id] & ADMIN_VOTE && get_pcvar_num(vote_gag)))
				console_cmd(id, "sr_addgag ^"%s^" %s %s 1m", target_names[id], target_flex[id], target_ip[id]);
			else LastDetails(id);
		}
		
		case 5:
		{
			if(id_flags[id] & ADMIN_BAN ||
			(id_flags[id] & ADMIN_VOTE && get_pcvar_num(vote_ban)))
				console_cmd(id, "sr_cmdban ^"%s^" dgs 1m", target_names[id]);
			else LastDetails(id);
		}
		
		case 6:
		{
			CopyToConsole(id, LAST);
			LastDetails(id);
		}
	}
	
	menu_destroy( menu );
	
	return PLUGIN_HANDLED;
}

public cmdSearchInLast(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 1))
		return PLUGIN_HANDLED;
	
	read_args(last_search_key[id], LEN_NAME);
	remove_quotes(last_search_key[id]);
	
	console_cmd(id, "sr_last");
	return PLUGIN_HANDLED;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1026\\ f0\\ fs16 \n\\ par }
*/
