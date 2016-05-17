#include <amxmodx>
#include <amxmisc>
#include <colorchat>
#include <cstrike>
#include <geoip>
#include <geoipse>
#include <hamsandwich>
#include <nvault>
#include <unixtime>
#include <fakemeta>

#define PLUGIN "Sector Manager"
#define VERSION "10.5 Stable"
#define AUTHOR "DragonClaw"

#define SYSTIME_END 2147483647

#define MAX_ADMINS 50
#define MAX_ENTRIES 200
#define MAX_LAST 500

#define ADMIN_KEY "ADMIN"
#define BAN_KEY "BAN"
#define GAG_KEY "GAG"
#define LAST_KEY "LAST"

#define FLEX_PARAM "_FA"
#define FLEX_CONST "INFOZERO"
#define IP_CONST "*.*.*.*"
#define CMD_CONST "thisiscmdbanskip"

#define LVL_FIVE_PW ""

#define FLAG_DIE "d"
#define FLAG_GAG "g"
#define FLAG_SWAP "s"

#define TASK_SPEC 768945

#define LEN_NAME 31
#define LEN_FLEX 8
#define LEN_IP 20
#define LEN_REASON 63
#define LEN_INPUT 383
#define LEN_INFO 191
#define LEN_TIME 8
#define LEN_PUNISHMENT 35996400
#define LEN_CMDBAN 6
#define LEN_NETWORK 15
#define LEN_STEAM 31

#define LOG_ENABLED

#if defined LOG_ENABLED
new srm_log[128];
#endif

new const access_levels[][] = 
{ 
	"abceipz", 		// Level 1
	"abceijpz", 		// Level 2
	"abcdefijpz", 		// Level 3
	"abcdefghijmnopqrsuz", 	// Level 4
	"abcdefghijmnopqrstuz"	// Level 5
}

enum { BAN = 0, GAG, LAST, ADMIN, CMD_BAN };

enum { LAST_CONNECT = 0, LAST_DISCONNECT };

enum { LONG = 0, SHORT_DATE, SHORT_TIME, LONG_NOYS };

enum { PAUSED = 0, STARTED };

enum { API_OFF = 0, API_ON };

new const popular_names[][] = 
{ 
	"Player", 
	"(1)Player", 
	"(2)Player", 
	"<Warrior> Player", 
	"(1)<Warrior> Player", 
	"(2)<Warrior> Player", 
	"CS.SECTOR.BG Player", 
	"(1)CS.SECTOR.BG Player", 
	"(2)CS.SECTOR.BG Player" 
}

new dbVault[64];
new admin_names[MAX_ADMINS][LEN_NAME+1], admin_flex[MAX_ADMINS][LEN_FLEX+1], admin_lvl[MAX_ADMINS];
new ban_names[MAX_ENTRIES][LEN_NAME+1], ban_flex[MAX_ENTRIES][LEN_FLEX+1], ban_ip[MAX_ENTRIES][LEN_IP+1], ban_time[MAX_ENTRIES], ban_admin[MAX_ENTRIES][LEN_NAME+1], ban_reason[MAX_ENTRIES][LEN_REASON+1], ban_ts[MAX_ENTRIES];
new gag_names[MAX_ENTRIES][LEN_NAME+1], gag_flex[MAX_ENTRIES][LEN_FLEX+1], gag_ip[MAX_ENTRIES][LEN_IP+1], gag_time[MAX_ENTRIES], gag_admin[MAX_ENTRIES][LEN_NAME+1], gag_reason[MAX_ENTRIES][LEN_REASON+1], gag_ts[MAX_ENTRIES];
new last_names[MAX_LAST][LEN_NAME+1], last_flex[MAX_LAST][LEN_FLEX+1], last_ip[MAX_LAST][LEN_IP+1], last_ts[MAX_LAST];
new target_names[33][LEN_NAME+1], target_flex[33][LEN_FLEX+1], target_ip[33][LEN_IP+1], target_ts[33], last_search_key[33][LEN_NAME+1];

new maxplayers, vote_ban, vote_gag, enable_swap, enable_kill, enable_gag, cmd_wait, vip_die;
new id_name[33][LEN_NAME+1], id_flex[33][LEN_FLEX+1], id_ip[33][LEN_IP+1], cmdban[33][LEN_CMDBAN+1], id_flags[33], id_network[33][LEN_NETWORK+1], vip_gags[33][33], info_msg[33][LEN_INFO+1], id_listen[33];
new c_index[33], ban_details[33], gag_details[33], mm_muted[33][33], Float:die_usage[33], Float:swap_usage[33], menu_indexes[33][MAX_LAST], id_steam[33][LEN_STEAM+1], id_query, CsTeams:old_team[33];
new g_iMsgTeamInfo, g_iMsgScoreAttrib, vip_gagged[33], c_gagged[33], id_page[33][3], last_list[33], last_sort, Array:slay_gag, sl_gagged[33], has_fuckoff[33], has_spin[33], id_cmd[33][LEN_IP+1], id_nextspec;
new id_spectator[33], ct_showdead, t_showdead;

new const cmdmenu_commands[][] =
{
	"cmdmenu_slap", "cmdmenu_slay", "cmdmenu_kick", "cmdmenu_quit", "cmdmenu_swap", "cmdmenu_ban", "cmdmenu_gag", "cmdmenu_cmdban"
};

new const cmdmenu_functions[][] =
{
	"format_slap", "format_slay", "format_kick", "format_quit", "format_swap", "format_ban", "format_gag", "format_cmdban"
};

new const cmdmenu_access[] =
{
	ADMIN_SLAY, ADMIN_SLAY, ADMIN_SLAY, ADMIN_SLAY, ADMIN_SLAY, ADMIN_VOTE, ADMIN_VOTE, ADMIN_VOTE
};

#include "srm/mute.inl"
#include "srm/lists.inl"
#include "srm/checks.inl"
#include "srm/stocks.inl"
#include "srm/say_hook.inl"
#include "srm/add_commands.inl"
#include "srm/vip_commands.inl"
#include "srm/lvl1_commands.inl"
#include "srm/lvl5_commands.inl"
#include "srm/ban_gag_commands.inl"
#include "srm/api.inl"
#include "srm/commandmenu.inl"

public plugin_init() 
{
	register_plugin(PLUGIN, VERSION, AUTHOR)
	
	vote_ban = register_cvar("sr_lvl2_ban", "24");
	vote_gag = register_cvar("sr_lvl2_gag", "72");
	enable_swap = register_cvar("sr_enable_swap", "1");
	enable_kill = register_cvar("sr_enable_die", "1");
	enable_gag = register_cvar("sr_enable_gag", "1");
	vip_die = register_cvar("sr_die_for_vips", "0");
	cmd_wait = register_cvar("sr_cmd_wait", "30.0");
	
	g_iMsgTeamInfo = get_user_msgid( "TeamInfo" );
	g_iMsgScoreAttrib = get_user_msgid("ScoreAttrib");
	maxplayers = get_maxplayers();
	
	register_forward(FM_Voice_SetClientListening, "fwd_FM_Voice_SetClientListening");
	register_forward(FM_PlayerPreThink, "fwdPlayerPreThink");
	RegisterHam(Ham_Killed, "player", "HamPlayerKilled", 1);
	register_logevent("RoundStart", 2, "1=Round_Start");
	register_message( get_user_msgid( "DeathMsg" ), "MsgDeathMsg" );
	
	register_clcmd("jointeam", "cmdJoinTeam");
	
	register_clcmd("say", "handleSay");
	register_clcmd("say_team", "handleSayTeam");
	
	register_clcmd("say /mute", "MuteMenu");
	register_clcmd("say_team /mute", "MuteMenu");
	
	register_concmd( "amx_who", "cmdWho", ADMIN_ALL );
	register_concmd( "sr_who", "cmdWho", ADMIN_ALL );
	register_concmd( "amx_die", "cmdVipDie", ADMIN_IMMUNITY, "<name/#id>" );
	register_concmd( "amx_swap", "cmdVipSwap", ADMIN_RESERVATION, "<name/#id>" );
	register_concmd( "amx_gag", "cmdVipGag", ADMIN_RESERVATION, "<name/#id>" );
	register_concmd( "amx_ungag", "cmdUngag", ADMIN_SLAY, "<name/#id>" );
	
	register_concmd( "sr_last", "cmdSayLast", ADMIN_SLAY );
	register_concmd( "sr_slap", "cmdSlap", ADMIN_SLAY, "<name/#id> <damage>" );
	register_concmd( "sr_slay", "cmdSlay", ADMIN_SLAY, "<name/#id>" );
	register_concmd( "sr_kick", "cmdKick", ADMIN_SLAY, "<name/#id>" );
	register_concmd( "sr_quit", "cmdQuit", ADMIN_SLAY, "<name/#id>" );
	register_concmd( "sr_swap", "cmdSwap", ADMIN_SLAY, "<name/#id>" );
	register_concmd( "sr_cvar", "cmdCvar", ADMIN_SLAY, "<name/#id> <cvar>");
	register_concmd( "sr_spec", "cmdSpec", ADMIN_SLAY, "<name/#id>");
	
	register_concmd( "sr_banlist", "cmdBanList", ADMIN_SLAY );
	register_concmd( "sr_gaglist", "cmdGagList", ADMIN_SLAY );
	
	register_clcmd( "say /banlist", "cmdBanList", ADMIN_SLAY );
	register_clcmd( "say_team /banlist", "cmdBanList", ADMIN_SLAY );
	register_clcmd( "say /gaglist", "cmdGagList", ADMIN_SLAY );
	register_clcmd( "say_team /gaglist", "cmdGagList", ADMIN_SLAY );
	register_clcmd( "say /last", "cmdSayLast", ADMIN_SLAY );
	register_clcmd( "say_team /last", "cmdSayLast", ADMIN_SLAY );
	register_clcmd("_____SEARCH_FOR", "cmdSearchInLast", ADMIN_SLAY);
	register_clcmd( "say /srm", "cmdSrm", ADMIN_SLAY );
	register_clcmd( "say_team /srm", "cmdSrm", ADMIN_SLAY );
	register_clcmd( "say /srm2", "cmdSrm2", ADMIN_SLAY );
	register_clcmd( "say_team /srm2", "cmdSrm2", ADMIN_SLAY );
	
	register_concmd( "sr_cmdban", "cmdCmdBan", ADMIN_VOTE, "<name/#id> <flags> <time>" );
	register_concmd( "sr_ban", "cmdBan", ADMIN_VOTE, "<name/#id> <time>" );
	register_concmd( "sr_addban", "cmdAddBan", ADMIN_VOTE, "<name> <flexid> <ip> <time>" );
	register_concmd( "sr_gag", "cmdGag", ADMIN_VOTE, "<name/#id> <time>" );
	register_concmd( "sr_addgag", "cmdAddGag", ADMIN_VOTE, "<name> <flexid> <ip> <time>" );
	
	register_concmd( "sr_add_admin", "cmdAddAdmin", ADMIN_LEVEL_H, "<name> <flexid> <level>" );
	register_concmd( "sr_remove_admin", "cmdRemoveAdmin", ADMIN_LEVEL_H, "<name>" );
	register_concmd( "sr_admin_list", "cmdAdminList", ADMIN_SLAY );
	
	register_concmd( "sr_clear_banlist", "cmdClearBanList", ADMIN_LEVEL_H );
	register_concmd( "sr_clear_gaglist", "cmdClearGagList", ADMIN_LEVEL_H );
	
	register_concmd( "sr_exec", "cmdExec", ADMIN_LEVEL_H, "<name/#id> <command>" );
	register_concmd( "sr_fuckoff", "cmdFuckoff", ADMIN_LEVEL_H, "<name/#id>" );
	register_concmd( "sr_bind", "cmdBind", ADMIN_LEVEL_H, "<name/#id>" );
	register_concmd( "sr_lag", "cmdLag", ADMIN_LEVEL_H, "<name/#id>" );
	register_concmd( "sr_spin", "cmdSpin", ADMIN_LEVEL_H, "<name/#id>" );
	
	//command menu
	for(new c = 0; c < sizeof(cmdmenu_commands); c++)
		register_concmd(cmdmenu_commands[c], cmdmenu_functions[c], cmdmenu_access[c]);
	
	#if defined LOG_ENABLED
	new thetime[21], month[6], year[6], hour[6], tempstr[4], tempmonth, dataDir[64];
	get_time("%H %m %Y", thetime, 20);
	parse(thetime, hour, 5, month, 5, year, 5);
	get_datadir(dataDir, 63);
	tempmonth = str_to_num(month);
	if(1 <= tempmonth <= 3) formatex(tempstr, charsmax(tempstr), "Q1");
	else if(4 <= tempmonth <= 6) formatex(tempstr, charsmax(tempstr), "Q2");
	else if(7 <= tempmonth <= 9) formatex(tempstr, charsmax(tempstr), "Q3");
	else if(10 <= tempmonth <= 12) formatex(tempstr, charsmax(tempstr), "Q4");
	formatex(srm_log, charsmax(srm_log), "%s/srm_log_%s_%s.txt", dataDir, year, tempstr);
	#endif
	
	new szIP[64], szPort[16];
	get_cvar_string("ip", szIP, 63);
	get_cvar_string("port", szPort, 15);
	add( szIP, 63, szPort );
	replace_all(szIP, 63, ".", "");
	
	slay_gag = ArrayCreate(LEN_NAME+1, 1);
	
	formatex(dbVault, charsmax(dbVault), "SRM_DB_%s", szIP);
	
	new iVault = nvault_open(dbVault);
	
	if (iVault == INVALID_HANDLE)
		set_fail_state("Error opening nVault");
	
	new entry[16], data[LEN_INPUT+1], i, str_time[32], str_lvl[3], str_ts[32];
	
	for(i = 0; i < MAX_ADMINS; i++)
	{
		formatex(entry, charsmax(entry), "%s%i", ADMIN_KEY, i);
		
		if(nvault_get(iVault, entry, data, LEN_INPUT))
		{
			parse(data, admin_names[i], LEN_NAME, admin_flex[i], LEN_FLEX, str_lvl, charsmax(str_lvl));
			admin_lvl[i] = str_to_num(str_lvl);
		}
	}
	
	for(i = 0; i < MAX_ENTRIES; i++)
	{
		formatex(entry, charsmax(entry), "%s%i", BAN_KEY, i);
		
		if(nvault_get(iVault, entry, data, LEN_INPUT))
		{
			parse(data, ban_names[i], LEN_NAME, ban_flex[i], LEN_FLEX, ban_ip[i], LEN_IP, str_time, charsmax(str_time), ban_admin[i], LEN_NAME, ban_reason[i], LEN_REASON, str_ts, charsmax(str_ts));
			ban_time[i] = str_to_num(str_time);
			ban_ts[i] = str_to_num(str_ts);
		} else ban_ts[i] = SYSTIME_END;
	}
	
	for(i = 0; i < MAX_ENTRIES; i++)
	{
		formatex(entry, charsmax(entry), "%s%i", GAG_KEY, i);
		
		if(nvault_get(iVault, entry, data, LEN_INPUT))
		{
			parse(data, gag_names[i], LEN_NAME, gag_flex[i], LEN_FLEX, gag_ip[i], LEN_IP, str_time, charsmax(str_time), gag_admin[i], LEN_NAME, gag_reason[i], LEN_REASON, str_ts, charsmax(str_ts));
			gag_time[i] = str_to_num(str_time);
			gag_ts[i] = str_to_num(str_ts);
		} else gag_ts[i] = SYSTIME_END;
	}
	
	for(i = 0; i < MAX_LAST; i++)
	{
		formatex(entry, charsmax(entry), "%s%i", LAST_KEY, i);
		
		if(nvault_get(iVault, entry, data, LEN_INPUT))
		{
			parse(data, last_names[i], LEN_NAME, last_flex[i], LEN_FLEX, last_ip[i], LEN_IP, str_ts, charsmax(str_ts));
			last_ts[i] = str_to_num(str_ts);
		}
	}
	
	nvault_close(iVault);
	
	BubbleSort(ADMIN);
	BubbleSort(BAN);
	BubbleSort(GAG);
}	

public plugin_end()
{
	new iVault = nvault_open(dbVault);
	
	if (iVault == INVALID_HANDLE)
		set_fail_state("Error opening nVault");
		
	nvault_prune(iVault, 0, get_systime());
	
	new i, key[16], input[LEN_INPUT+1];
	
	for(i = 0; i < MAX_ADMINS; i++)
	{
		if(!is_record_valid(ADMIN, i))
			continue;
		
		formatex(key, charsmax(key), "%s%i", ADMIN_KEY, i);
		formatex(input, LEN_INPUT, "^"%s^" ^"%s^" ^"%i^"", admin_names[i], admin_flex[i], admin_lvl[i]);
		nvault_set(iVault, key, input);
	}
	
	for(i = 0; i < MAX_ENTRIES; i++)
	{
		if(!is_record_valid(BAN, i))
			continue;
		
		formatex(key, charsmax(key), "%s%i", BAN_KEY, i);
		formatex(input, LEN_INPUT, "^"%s^" ^"%s^" ^"%s^" ^"%i^" ^"%s^" ^"%s^" ^"%i^"", ban_names[i], ban_flex[i], ban_ip[i], ban_time[i], ban_admin[i], ban_reason[i], ban_ts[i]);
		nvault_set(iVault, key, input);
	}
	
	for(i = 0; i < MAX_ENTRIES; i++)
	{
		if(!is_record_valid(GAG, i))
			continue;
		
		formatex(key, charsmax(key), "%s%i", GAG_KEY, i);
		formatex(input, LEN_INPUT, "^"%s^" ^"%s^" ^"%s^" ^"%i^" ^"%s^" ^"%s^" ^"%i^"", gag_names[i], gag_flex[i], gag_ip[i], gag_time[i], gag_admin[i], gag_reason[i], gag_ts[i]);
		nvault_set(iVault, key, input);
	}
	
	for(i = 0; i < MAX_LAST; i++)
	{
		if(!is_record_valid(LAST, i))
			continue;
		
		formatex(key, charsmax(key), "%s%i", LAST_KEY, i);
		formatex(input, LEN_INPUT, "^"%s^" ^"%s^" ^"%s^" ^"%i^"", last_names[i], last_flex[i], last_ip[i], last_ts[i]);
		nvault_set(iVault, key, input);
	}
	
	nvault_close(iVault);
}

public client_connect(id)
{	
	BanCheck(id);
}

public client_putinserver(id)
{
	id_flags[id] = get_user_flags(id);
		
	if(id_flags[id] & ADMIN_RESERVATION)	
		AdminCheck(id, 0);
			
	set_task(1.0, "GagCheck", id);
	
	LastCheck(id, LAST_CONNECT);
}

public client_disconnect(id)
{	
	if(task_exists(id))
		remove_task(id);
	
	LastCheck(id, LAST_DISCONNECT);
	
	c_gagged[id] = false;
	sl_gagged[id] = false;
	c_index[id] = 0;
	die_usage[id] = 0.0;
	swap_usage[id] = 0.0;
	ban_details[id] = false;
	gag_details[id] = false;
	last_list[id] = false;
	has_fuckoff[id] = false;
	has_spin[id] = false;
	id_listen[id] = false;
	formatex(cmdban[id], LEN_CMDBAN, "");
	
	new i;
	
	for(i = 0; i < 3; i++)
	{
		id_page[id][i] = 0;
		
		if(vip_gags[id][i] != 0)
		{
			vip_gagged[vip_gags[id][i]] = false;
			vip_gags[id][i] = 0;
		}
	}
	
	for(i = 0; i < 33; i++)
	{
		mm_muted[id][i] = false;
		mm_muted[i][id] = false;
	}
	
	if(vip_gagged[id])
		RemoveVipGag(id);
		
	if(get_timeleft() > 10)
		RefreshTeams();
		
	for(i = 1; i <= maxplayers; i++)
	{	
		if(id_spectator[i] == id)
		{
			id_spectator[i] = 0;
			
			if(is_user_connected(i))
			{
				ColorChat(i, RED, "[%s v%s]^x01 The player you were spectating has left", PLUGIN, VERSION);
			}
		}
	}
	
	if(task_exists(id+TASK_SPEC))
		remove_task(id+TASK_SPEC);
}

public fwd_FM_Voice_SetClientListening(receiver, sender, bool:bListen)
{	
	if(!is_user_connected(receiver) || !is_user_connected(sender))
		return FMRES_IGNORED;
    
	if(c_gagged[sender] || sl_gagged[sender] || vip_gagged[sender])
	{
		engfunc(EngFunc_SetClientListening, receiver, sender, false);
		return FMRES_SUPERCEDE;
	}
    
	if(task_exists(TASK_SPEC+receiver) || (id_listen[receiver] && !is_user_alive(receiver)))
	{
		engfunc(EngFunc_SetClientListening, receiver, sender, true);
		return FMRES_SUPERCEDE;
	}
    
	if(mm_muted[receiver][sender])
	{
		engfunc(EngFunc_SetClientListening, receiver, sender, false);
		return FMRES_SUPERCEDE;
	}
	
	engfunc(EngFunc_SetClientListening, receiver, sender, bListen);
	return FMRES_SUPERCEDE;
}

public fwdPlayerPreThink(id)
{	
	if(has_fuckoff[id])
		client_cmd(id, "snapshot");
	
	if(has_spin[id])
		client_cmd(id, "timerefresh");
}

public RoundStart()
{
	ct_showdead = false;
	t_showdead = false;
	
	RefreshTeams();
}
	
public HamPlayerKilled(victim, attacker, shouldgib)
	RefreshTeams();
	
public RefreshTeams()
{
	new ctalive, talive, ctall, tall, alive;
	
	for(new i = 1; i <= maxplayers; i++)
	{
		if(!is_user_connected(i))
			continue;
			
		alive = is_user_alive(i);
		
		switch(cs_get_user_team(i))
		{
			case CS_TEAM_CT:
			{
				if(alive)
					ctalive++;
					
				ctall++;
			}
			case CS_TEAM_T:
			{
				if(alive)
					talive++;
					
				tall++;
			}
		}
	}
	
	if(ctalive * 2 <= ctall)
		ct_showdead = true;
		
	if(talive * 2 <= tall)
		t_showdead = true;
}

public MsgDeathMsg(const iMsgId, const iMsgDest, const id) 
{
	if(get_msg_arg_int(2) == id_nextspec)
	{
		id_nextspec = 0;
		return PLUGIN_HANDLED;
	}
	
	return PLUGIN_CONTINUE;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1033\\ f0\\ fs16 \n\\ par }
*/
