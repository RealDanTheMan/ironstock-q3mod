/*****************************************************************************
 * name:		ai_mod.h
 *
 * desc:		Entry point logic for handling AI bots in Ironstock mod.
 *
 *
 *****************************************************************************/

#ifndef AI_MOD_H_
#define AI_MOD_H_

//#define DEBUG
//#define DEBUG_AI_MOVE

typedef struct map_location_s {
	int		areanum;
	vec3_t	pos;
} map_location_t;

void AI_ModUpdateBot(bot_state_t *state, float thinktime);
void AI_ModInitBot(bot_state_t *state);
int AI_ModRandomMapArea(void);
qboolean AI_ModRandomMapLocation(map_location_t *loc);

// State machine handlers
void AIEnter_Idle(bot_state_t *state, char* msg);
void AIEnter_WalkAround(bot_state_t *state, char* msg);

// State machine nodes
int AINode_Idle(bot_state_t *state);
int AINode_MoveToGoal(bot_state_t *state);

#endif
