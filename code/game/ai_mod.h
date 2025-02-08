/*****************************************************************************
 * name:		ai_mod.h
 *
 * desc:		Entry point logic for handling AI bots in Ironstock mod.
 *
 *
 *****************************************************************************/

#ifndef AI_MOD_H_
#define AI_MOD_H_

void AI_ModUpdateBot(bot_state_t *state, float thinktime);
void AI_ModInitBot(bot_state_t *state);

// State machine handlers
void AIEnter_Idle(bot_state_t *state, char* msg);

// State machine nodes
int AINode_Idle(bot_state_t *state);

#endif
