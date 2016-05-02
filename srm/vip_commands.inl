public cmdVipSwap(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED;
	
	if(id_flags[id] & ADMIN_SLAY)
	{
		console_print(id, "Tova e V.I.P. komanda. Izpolzwai adminskiq ekvivalent.");
		return PLUGIN_HANDLED;
	}
	
	if(!get_pcvar_num(enable_swap))
	{
		console_print(id, "Komandata e izkliuchena za tozi server.");
		return PLUGIN_HANDLED;
	}
	
	if(is_cmd_banned(id, FLAG_SWAP))
	{
		console_print(id, "Dostuput ti do tazi komanda e blokiran ot admin poradi zloupotreba.");
			
		return PLUGIN_HANDLED;
	}
	
	new Float:ctime = get_gametime();
	
	if(ctime < swap_usage[id])
	{
		console_print(id, "Trqbwa da izchakash oshte %i sekundi.", floatround(swap_usage[id]-ctime, floatround_round));
		
		return PLUGIN_HANDLED;
	}
	
	if(is_user_alive(id))
	{
		console_print(id, "Trqbva da si DEAD.");
		
		return PLUGIN_HANDLED;
	}
	
	new arg[LEN_NAME+1];
	read_argv(1, arg, LEN_NAME);
	
	new player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!player || player == id)
		return PLUGIN_HANDLED;
	
	if(is_user_alive(player))
	{
		console_print(id, "Izbraniqt igrach trqbva da e DEAD.");
		
		return PLUGIN_HANDLED;
	}
	
	if(get_user_flags(player) & ADMIN_LEVEL_D)
	{
		console_print(id, "Izbraniqt igrach trqbva da e neregistriran.");
		
		return PLUGIN_HANDLED;
	}
	
	new CsTeams:idTeam = cs_get_user_team(id);
	new CsTeams:plTeam = cs_get_user_team(player);
	
	if(idTeam == CS_TEAM_SPECTATOR || idTeam == CS_TEAM_UNASSIGNED)
	{
		console_print(id, "Trqbva da vlezesh v igra, za da izpolzvash tazi komanda.");
		
		return PLUGIN_HANDLED;
	}
	
	if(plTeam == CS_TEAM_SPECTATOR || plTeam == CS_TEAM_UNASSIGNED)
	{
		console_print(id, "Izbraniqt igrach ne trqbva da bude spektator.");
		
		return PLUGIN_HANDLED;
	}
	
	if(idTeam == plTeam)
	{
		console_print(id, "Izbraniqt igrach trqbva da e ot protivnikoviq otbor.");
		
		return PLUGIN_HANDLED;
	}
	
	swap_usage[id] = get_gametime() + get_pcvar_float(cmd_wait);
	
	switch(idTeam)
	{
		case CS_TEAM_CT:
		{
			cs_set_user_team(id, CS_TEAM_T);
			fix_score_team(id, "TERRORIST");
			
			cs_set_user_team(player, CS_TEAM_CT);
			fix_score_team(player, "CT");
		}
		case CS_TEAM_T:
		{
			cs_set_user_team(id, CS_TEAM_CT);
			fix_score_team(id, "CT");
			
			cs_set_user_team(player, CS_TEAM_T);
			fix_score_team(player, "TERRORIST");
		}
	}
	
	ColorChat(0, GREEN, "[V.I.P.]^x01 %s swapped his team for %s's.", id_name[id], id_name[player]);
	
	return PLUGIN_HANDLED;
}

public cmdVipDie(id, level, cid)
{	
	if(!(id_flags[id] & ADMIN_IMMUNITY) && (!(get_pcvar_num(vip_die) && id_flags[id] & ADMIN_RESERVATION)))
	{
		console_print(id, "You have no access to that command.");
		return PLUGIN_HANDLED;
	}
	
	if(id_flags[id] & ADMIN_SLAY)
	{
		console_print(id, "Tova e V.I.P. komanda. Izpolzwai adminskiq ekvivalent.");
		return PLUGIN_HANDLED;
	}
	
	if(!get_pcvar_num(enable_kill))
	{
		console_print(id, "Komandata e izkliuchena za tozi server.");
		return PLUGIN_HANDLED;
	}
	
	if(is_cmd_banned(id, FLAG_DIE))
	{
		console_print(id, "Dostuput ti do tazi komanda e blokiran ot admin poradi zloupotreba.");
		return PLUGIN_HANDLED;
	}
	
	new arg[LEN_NAME+1];
	read_argv(1, arg, LEN_NAME);
	
	new Float:ctime = get_gametime();
	
	new player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!player || !is_user_alive(player))
		return PLUGIN_HANDLED;
		
	if(ctime < die_usage[id])
	{
		console_print(id, "Trqbwa da izchakash oshte %i sekundi.", floatround(die_usage[id]-ctime, floatround_round));
				
		return PLUGIN_HANDLED;
	}
		
	if(get_user_flags(player) & ADMIN_LEVEL_D)
	{
		console_print(id, "Ne mojesh da ubivash Member, V.I.P. ili admini.");
			
		return PLUGIN_HANDLED;
	}
		
	if(cs_get_user_team(player) != cs_get_user_team(id))
	{
		console_print(id, "Igrachut trqbva da e ot tvoq otbor.");
			
		return PLUGIN_HANDLED;
	}
	
	die_usage[id] = get_gametime() + get_pcvar_float(cmd_wait);	
	
	user_silentkill(player);
	
	ColorChat(0, GREEN, "[V.I.P.]^x01 %s slayed %s.", id_name[id], id_name[player]);
	
	return PLUGIN_HANDLED;
}

public cmdVipGag(id, level, cid)
{	
	if(!cmd_access(id, level, cid, 2))
		return PLUGIN_HANDLED;
	
	new arg[LEN_NAME+1];
	read_argv(1, arg, LEN_NAME);
	
	new player = cmd_target(id, arg, CMDTARGET_NO_BOTS);
	
	if(!player || player == id)
		return PLUGIN_HANDLED;
	
	new isadmin = id_flags[id] & ADMIN_SLAY;
	
	if(c_gagged[player] || sl_gagged[player])
	{
		console_print(id, "Igrachut veche e bil gagnat.");
		
		return PLUGIN_HANDLED;
	}

	if(isadmin)
	{
		if(vip_gagged[player])
			RemoveVipGag(player);
		
		sl_gagged[player] = true;
		client_cmd(player, "-voicerecord");
		ArrayPushString(slay_gag, id_name[player]);
		
		switch(activity())
		{
			case 1:ColorChat(0, RED, "[ADMIN]^x01 gagged %s.", id_name[player]);
			case 2:ColorChat(0, RED, "[ADMIN]^x01 %s gagged %s.", id_name[id], id_name[player]);
		}
		
		return PLUGIN_HANDLED;
	}
	
	if(!get_pcvar_num(enable_gag))
	{
		console_print(id, "Komandata e izkliuchena za tozi server.");
		return PLUGIN_HANDLED;
	}
	
	if(is_cmd_banned(id, FLAG_GAG))
	{
		console_print(id, "Dostuput ti do tazi komanda e blokiran ot admin poradi zloupotreba.");
		return PLUGIN_HANDLED;
	}
	
	if(id_flags[player] & ADMIN_LEVEL_D)
	{
		console_print(id, "Ne mojesh da gagvash Member, V.I.P. ili admini.");
		
		return PLUGIN_HANDLED;
	}
	
	if(vip_gagged[player])
	{
		console_print(id, "Igrachut veche e bil gagnat.");
		
		return PLUGIN_HANDLED;
	}
	
	new empty = -1;
		
	for(new i = 0; i < 3; i++)
	{
		if(vip_gags[id][i] == 0)
		{
			empty = i;
				
			break;
		}
	}
	
	if(empty == -1)
	{
		console_print(id, "Veche si gagnal 3-ma dushi i nqmash pravo na poveche gagove.");
			
		return PLUGIN_HANDLED;
	}
	
	vip_gags[id][empty] = player;	
	vip_gagged[player] = true;
	client_cmd(player, "-voicerecord");
	
	ColorChat(0, GREEN, "[V.I.P.]^x01 %s gagged %s.", id_name[id], id_name[player]);
	
	return PLUGIN_HANDLED;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1026\\ f0\\ fs16 \n\\ par }
*/
