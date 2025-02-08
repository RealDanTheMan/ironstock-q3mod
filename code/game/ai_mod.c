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
/// AIEnter_Idle
/// State machine idle state.
void AIEnter_Idle(bot_state_t *state, char* msg) {
	Com_Printf("Mode AI: entering idle state -> %s\n", msg);
	state->ainode = AINode_Idle;
}

///
/// AINode_Idle
/// AI Statmachine node for doing nothing.
int AINode_Idle(bot_state_t *state) {
	return qtrue;
}
