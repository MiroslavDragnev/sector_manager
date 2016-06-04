public RefreshTeams()
{
	new systime = get_systime();
	
	if(systime >= last_teamcheck)
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
			ct_showdead = 1;
			
		if(talive * 2 <= tall)
			t_showdead = 1;
			
		last_teamcheck = systime + 1;
	}
}

public TaskSpec(id)
{
	id -= TASK_SPEC;
	
	new id_iuser2 = id_spectator[id];
	new CsTeams:team = id_iuser2 == 0 ? old_team[id] : cs_get_user_team(id_iuser2);
	
	if((id_iuser2 && team == CS_TEAM_CT) || (!id_iuser2 && team == CS_TEAM_T))
	{
		fix_score_team(id, "TERRORIST");
		send_ScoreAttrib(id, t_showdead);
	}
	else if((id_iuser2 && team == CS_TEAM_T) || (!id_iuser2 && team == CS_TEAM_CT))
	{
		fix_score_team(id, "CT");
		send_ScoreAttrib(id, ct_showdead);
	}
	else
	{
		fix_score_team(id, "TERRORIST");
		send_ScoreAttrib(id, t_showdead);
	}
}

public AdminHint()
{	
	for(new j = 1; j <= maxplayers; j++)
	{
		if(!is_user_connected(j) || is_user_alive(j))
			continue;
			
		if(task_exists(TASK_SPEC+j) && id_hint[j])
		{
			if(pev(j, pev_iuser1) != 4)
				continue;
			
			new spec_id = pev(j, pev_iuser2);
			
			if(!spec_id || !is_user_alive(spec_id))
				continue;
				
			new Float:my_origin[3];
			entity_get_vector(spec_id, EV_VEC_origin, my_origin);
			new CsTeams:sTeam = cs_get_user_team(spec_id);
	
			for(new i = 1; i <= maxplayers; i++)
			{
				if(!is_user_connected(i) || !is_user_alive(i) || i == spec_id || cs_get_user_team(i) == sTeam)
					continue;
	
				new Float:target_origin[3];
				entity_get_vector(i,EV_VEC_origin,target_origin);
				
				new Float:distance = vector_distance(my_origin,target_origin);
				
				new width = distance < 2040.0 ? (255-floatround(distance/8.0))/3 : 1;
				
				ent_beam(j, target_origin, width, sTeam == CS_TEAM_CT ? 0:1);
				
				new trace = 0, Float:fraction;
				engfunc(EngFunc_TraceLine, my_origin, target_origin, IGNORE_MONSTERS, 0, trace);
				get_tr2(trace, TR_flFraction, fraction);
				    
				if(fraction != 1.0) // if isn't in line of sight, create green box entity
				{
					new Float:v_middle[3];
					subVec(target_origin,my_origin,v_middle);
											
					new Float:v_hitpoint[3];
					trace_line (-1,my_origin,target_origin,v_hitpoint);
											
					new Float:distance_to_hitpoint=vector_distance(my_origin,v_hitpoint);
											
					new Float:scaled_bone_len;
					
					if (ducking[spec_id]) scaled_bone_len=distance_to_hitpoint/distance*(50.0-18.0);
					else scaled_bone_len=distance_to_hitpoint/distance*50.0;
					
					scaled_bone_len=distance_to_hitpoint/distance*50.0;
											
					new Float:scaled_bone_width=distance_to_hitpoint/distance*150.0;
											
					new Float:v_bone_start[3],Float:v_bone_end[3];
					new Float:offset_vector[3];
			
					normalize(v_middle,offset_vector,distance_to_hitpoint-10.0);
											
					new Float:eye_level[3];
					copyVec(my_origin,eye_level);
											
					if (ducking[spec_id]) eye_level[2]+=12.3;
					else eye_level[2]+=17.5;
											
					addVec(offset_vector,eye_level);
											
					copyVec(offset_vector,v_bone_start);
					copyVec(offset_vector,v_bone_end);
					v_bone_end[2]-=scaled_bone_len;
											
					new Float:distance_target_hitpoint=distance-distance_to_hitpoint;
											
					new actual_bright=255;
											
					if (distance_target_hitpoint<2040.0) actual_bright=(255-floatround(distance_target_hitpoint/12.0));
					else actual_bright=85;
					
					ent_box(j, v_bone_start, v_bone_end, floatround(scaled_bone_width), actual_bright);
				}
			}
		}
	}
}

public Float:getVecLen(Float:Vec[3])
{
	new Float:VecNull[3]={0.0,0.0,0.0}
	new Float:len=vector_distance(Vec,VecNull)
	return len
}

public copyVec(Float:Vec[3],Float:Ret[3])
{
	Ret[0]=Vec[0]
	Ret[1]=Vec[1]
	Ret[2]=Vec[2]
}

public addVec(Float:Vec1[3],Float:Vec2[3])
{
	Vec1[0]+=Vec2[0]
	Vec1[1]+=Vec2[1]
	Vec1[2]+=Vec2[2]
}

public normalize(Float:Vec[3],Float:Ret[3],Float:multiplier)
{
	new Float:len=getVecLen(Vec)
	copyVec(Vec,Ret)
	Ret[0]/=len
	Ret[1]/=len
	Ret[2]/=len
	Ret[0]*=multiplier
	Ret[1]*=multiplier
	Ret[2]*=multiplier
}

public subVec(Float:Vec1[3],Float:Vec2[3],Float:Ret[3])
{
	Ret[0]=Vec1[0]-Vec2[0]
	Ret[1]=Vec1[1]-Vec2[1]
	Ret[2]=Vec1[2]-Vec2[2]
}

public ent_box(id, Float:Vec1[3], Float:Vec2[3], width, brightness)
{
	message_begin(MSG_ONE_UNRELIABLE ,SVC_TEMPENTITY,{0,0,0},id) //message begin
	write_byte(0)
	write_coord(floatround(Vec1[0])) // start position
	write_coord(floatround(Vec1[1]))
	write_coord(floatround(Vec1[2]))
	write_coord(floatround(Vec2[0])) // end position
	write_coord(floatround(Vec2[1]))
	write_coord(floatround(Vec2[2]))
	write_short(laser) // sprite index
	write_byte(3) // starting frame
	write_byte(0) // frame rate in 0.1's
	write_byte(1) // life in 0.1's
	write_byte(width) // line width in 0.1's
	write_byte(0) // noise amplitude in 0.01's
	write_byte(box_color[0]) //r
	write_byte(box_color[1]) //g
	write_byte(box_color[2]) //b
	write_byte(brightness) // brightness)
	write_byte(0) // scroll speed in 0.1's
	message_end()
}

public ent_beam(id, Float:target_origin[3], width, color)
{
	message_begin(MSG_ONE_UNRELIABLE,SVC_TEMPENTITY,{0,0,0},id)
	write_byte(1)
	write_short(id)
	write_coord(floatround(target_origin[0]))
	write_coord(floatround(target_origin[1]))
	write_coord(floatround(target_origin[2]))
	write_short(laser)
	write_byte(1)		
	write_byte(1)
	write_byte(1) //ms?
	write_byte(width) //width
	write_byte(0)
	write_byte(beam_color[color][0]) //r
	write_byte(beam_color[color][1]) //g
	write_byte(beam_color[color][2]) //b
	write_byte(255)
	write_byte(0)
	message_end()
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1026\\ f0\\ fs16 \n\\ par }
*/
