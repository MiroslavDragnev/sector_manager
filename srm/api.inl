public srm_api_ban(const id, const time[], const plugin[], const reason[])
{
	if(!is_user_connected(id))
		return PLUGIN_CONTINUE;
	
	new idid = FindEmptyIndex(BAN);
	
	if(idid == -1)
		return PLUGIN_HANDLED;
	
	new tm = determine_multiplier(time, 0, BAN, STARTED, API_ON);
	
	if(!tm)
		return PLUGIN_HANDLED;
	
	formatex(ban_names[idid], LEN_NAME, "%s", id_name[id]);
	formatex(ban_flex[idid], LEN_FLEX, "%s", id_flex[id]);
	formatex(ban_ip[idid], LEN_IP, "%s", id_ip[id]);
	ban_time[idid] = tm;
	formatex(ban_admin[idid], LEN_NAME, "%s", plugin);
	formatex(ban_reason[idid], LEN_REASON, "%s", reason);
	ban_ts[idid] = get_systime();
	
	#if defined LOG_ENABLED
	log_to_file(srm_log, "%s banned %s (flex: %s; ip: %s; time: %s)", plugin, id_name[id], id_flex[id], id_ip[id], time);
	#endif
	
	new temp_reason[LEN_REASON+1];
	
	if(reason[0])
		formatex(temp_reason, LEN_REASON, " Prichina: %s.", reason);
	
	server_cmd("kick #%d ^"Beshe bannat ot %s.%s^"", get_user_userid(id), plugin, temp_reason);
	
	return PLUGIN_CONTINUE;
}

public srm_api_gag(const id, const time[], const plugin[], const reason[])
{
	if(!is_user_connected(id))
		return PLUGIN_CONTINUE;
	
	new idid = FindEmptyIndex(GAG);
	
	if(idid == -1)
		return PLUGIN_HANDLED;
	
	new tm = determine_multiplier(time, 0, GAG, STARTED, API_ON);
	
	if(!tm)
		return PLUGIN_HANDLED;
	
	formatex(gag_names[idid], LEN_NAME, "%s", id_name[id]);
	formatex(gag_flex[idid], LEN_FLEX, "%s", id_flex[id]);
	formatex(gag_ip[idid], LEN_IP, "%s", id_ip[id]);
	gag_time[idid] = tm;
	formatex(gag_admin[idid], LEN_NAME, "%s", plugin);
	formatex(gag_reason[idid], LEN_REASON, "%s", reason);
	gag_ts[idid] = get_systime();
	
	#if defined LOG_ENABLED
	log_to_file(srm_log, "%s gagged %s (flex: %s; ip: %s; time: %s)", plugin, id_name[id], id_flex[id], id_ip[id], time);
	#endif
	
	c_gagged[id] = true;
	client_cmd(id, "-voicerecord");
	
	return PLUGIN_CONTINUE;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1026\\ f0\\ fs16 \n\\ par }
*/
