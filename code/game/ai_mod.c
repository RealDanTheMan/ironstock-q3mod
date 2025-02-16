/*****************************************************************************
 * name:		ai_mod.c
 *
 * desc:		Entry point logic for handling AI bots in Ironstock mod.
 *
 *
 *****************************************************************************/


#include "g_local.h"
#include "../botlib/botlib.h"
#include "../botlib/be_aas.h"
#include "../botlib/be_ea.h"
#include "../botlib/be_ai_char.h"
#include "../botlib/be_ai_chat.h"
#include "../botlib/be_ai_gen.h"
#include "../botlib/be_ai_goal.h"
#include "../botlib/be_ai_move.h"
#include "../botlib/be_ai_weap.h"
//
#include "ai_main.h"
#include "ai_mod.h"
#include "ai_dmq3.h"
#include "ai_chat.h"
#include "ai_cmd.h"
#include "ai_dmnet.h"
#include "ai_team.h"
//
#include "chars.h"				//characteristics
#include "inv.h"				//indexes into the inventory
#include "syn.h"				//synonyms
#include "match.h"				//string matching types and vars


///
/// AI_ModUpdateBot
/// Main update method for all Ironstock AI bots.
/// Called every frame of the game.
void AI_ModUpdateBot(bot_state_t *state, float thinktime) {
	
	int i;

	/// Initialise the bot if needed.
	if (state->setupcount > 0) {
		AI_ModInitBot(state);
	}

	/// If bot has no active ai state begin root state
	if (!state->ainode) {
		AIEnter_Idle(state, "No active state found");
	}

	/// Execute bot AI state queue.
	BotResetNodeSwitches();
	for (i = 0; i < MAX_NODESWITCHES; i++) {
		if (state->ainode(state)) break;
	}

	/// Update bot global state properties. 
	state->lastframe_health = state->inventory[INVENTORY_HEALTH];
	state->lasthitcount = state->cur_ps.persistant[PERS_HITS];
}

///
/// AI_ModInitBot
/// Sets up intial states of the given bot.
void AI_ModInitBot(bot_state_t *state) {
	char gender[144], name[144];
	char userinfo[MAX_INFO_STRING];

	state->setupcount--;
	if (state->setupcount > 0) return;
	
	/// Setup bot gener properties.
	trap_Characteristic_String(state->character, CHARACTERISTIC_GENDER, gender, sizeof(gender));
	trap_GetUserinfo(state->client, userinfo, sizeof(userinfo));
	Info_SetValueForKey(userinfo, "sex", gender);
	trap_SetUserinfo(state->client, userinfo);
	
	/// Setup bot chat gender.
	if (gender[0] == 'm') trap_BotSetChatGender(state->cs, CHAT_GENDERMALE);
	else if (gender[0] == 'f')  trap_BotSetChatGender(state->cs, CHAT_GENDERFEMALE);
	else  trap_BotSetChatGender(state->cs, CHAT_GENDERLESS);
	
	/// Setup bot chat name
	ClientName(state->client, name, sizeof(name));
	trap_BotSetChatName(state->cs, name, state->client);
		
	state->lastframe_health = state->inventory[INVENTORY_HEALTH];
	state->lasthitcount = state->cur_ps.persistant[PERS_HITS];
	state->setupcount = 0;
}

///
/// AI_ModRandomMapArea
/// Get random reachagble area on loaded map.
/// Note: Q3 area indices are 1 based.
/// Returns -1 if no random area could be resolved.
int AI_ModRandomMapArea(void) {

	int	area		= 0;
	int num			= 0;
	int numareas	= 0;
	int areas[128];

	if (!trap_AAS_Initialized()) {
		Com_Error(0, "Attempting to use AAS when not initialised!");
		return -1;
	}

	numareas = trap_AAS_NumAreas();

	/// Return -1 if there is not a single area in the current map.
	/// This indicates that either map is not loaded or its in a broken state.
	if (numareas == 0) {
		return -1;
	}

	/// If there is only one area in the map there is no need for random logic or
	/// reachability tests.
	if (numareas == 1) {
		return 1;
	}

	/// Lookup all AAS reachable areas.
	for (area = 0; area < numareas; area++) {
		if (trap_AAS_AreaReachability(area + 1)) {
			areas[num] = area + 1;
			num ++;
		}

		/// Exit early if we reached max number of area lookups.
		if (num == 128) {
			break;
		}
	}

	/// Return -1 if we havent found at least one reachable area.
	if (num == 0) {
		return -1;
	}

	/// No need for random area if there is only one.
	if (num == 1) {
		return areas[0];
	}

	return areas[rand() & (num - 1)];
}

///
/// AI_ModRandomMapPosition
/// Get random position on active map.
qboolean AI_ModRandomMapLocation(map_location_t *loc) {

	int				area;
	aas_areainfo_t	info;
	Com_Memset(loc, 0, sizeof(map_location_t));

	/// Fetch random area from current map.
	area = AI_ModRandomMapArea();
	if (area == -1) {
#ifdef DEBUG_MOD_AI
		Com_Printf(S_COLOR_YELLOW "WARNING! Mod AI: Failed to resolve a random map area\n");
#endif
		return qfalse;
	}

	/// Compute random location on the map area.
	trap_AAS_AreaInfo(area, &info);
	loc->pos[0] = info.mins[0] + (random() * (info.maxs[0] - info.mins[0]));
	loc->pos[1] = info.mins[1] + (random() * (info.maxs[1] - info.mins[1]));
	loc->pos[2] = 0.0f;
	loc->areanum = area;

	return qtrue;
}

///
/// AIEnter_Idle
/// State machine idle state.
void AIEnter_Idle(bot_state_t *state, char* msg) {
	Com_Printf("Mod AI: entering idle state -> %s\n", msg);
	state->ainode = AINode_Idle;
}

///
/// AIEnter_WalkAround
/// Generates random location on the map and move the bot towards it.
void AIEnter_WalkAround(bot_state_t *state, char* msg) {

	map_location_t	loc;
	bot_goal_t		goal;

	Com_Printf("Mod AI: entering walk around state -> %s\n", msg);
	if (AI_ModRandomMapLocation(&loc)) {

		/// Create bot goal with random walking destination.
		Com_Memset(&goal, 0, sizeof(bot_goal_t));
		VectorCopy(loc.pos, goal.origin);
		goal.areanum = loc.areanum;

		/// Set as top goal and move to AI node to execute.
		trap_BotPushGoal(state->gs, &goal);
		state->ainode = AINode_MoveToGoal;

#ifdef DEBUG_MOD_AI
		Com_Printf(
			"Mod AI: Random map pos (area: %i) -> x:%f y:%f z:%f\n",
			goal.areanum,
			goal.origin[0],
			goal.origin[1],
			goal.origin[2]
		);
#endif
	}
	else {
		AIEnter_Idle(state, "Failed to walk around, back to idle");
	}
}

///
/// AINode_Idle
/// AI state machine node for doing nothing.
int AINode_Idle(bot_state_t *state) {
	//return qtrue;
	AIEnter_WalkAround(state, "Idle requests walk around");
	return qfalse;
}

///
/// AINode_MoveToGoal
/// AI state machine node for moving a bot towards active goal.
int AINode_MoveToGoal(bot_state_t *state) {
	
	float				dist;
	bot_goal_t			goal;
	bot_moveresult_t	result;

	/// Fetch current goal and move towardsit.
	trap_BotGetTopGoal(state->gs, &goal);

	state->tfl = TFL_DEFAULT;
	BotSetupForMovement(state);
	trap_BotMoveToGoal(&result, state->ms, &goal, state->tfl);

	/// Reset the bot if for some reason we are unable to move.
	if (result.failure) {
		Com_Printf("Mod AI: Failed to move a bot!\n");
		trap_BotResetAvoidReach(state->ms);
		
		/// Reset current goal and go back to idle state.
		trap_BotPopGoal(state->gs);
		AIEnter_Idle(state, "Move to Goal failed, back to idle state");
	}

	BotAIBlocked(state, &result, qtrue);
	
	/// Handle bot reaching its destination.
	dist = Distance(state->origin, goal.origin);
	if (dist < 1) {
		trap_BotPopGoal(state->gs);
		AIEnter_Idle(state, "Goal location reached, entering idle");
	}

	return qtrue;
}
