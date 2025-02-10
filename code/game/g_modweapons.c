#include "g_local.h"

///
/// Iron pistol weapon default properties
#define DEF_IRON_PISTOL_DAMAGE	10
#define DEF_IRON_PISTOL_RANGE	10000

///
/// IronPistolGunshot
/// Game server side logic for actually shooting the pistol.
/// Triggers pistol gunshot and damaging victims or environment.
qboolean IronPistolGunshot(vec3_t start, vec3_t end, gentity_t* owner, vec3_t forward) {

	trace_t		trace;
	vec3_t		tr_start;
	vec3_t		tr_end;
	gentity_t   *victim;

	VectorCopy(start, tr_start);
	VectorCopy(end, tr_end);

	/// Perform ray cast to see what gunshot bullet intersects with.
	trap_Trace(&trace, tr_start, NULL, NULL, tr_end, owner->s.number, MASK_SHOT);
	victim = &g_entities[trace.entityNum];

	/// Exit early if we havent hit anything particular.
	if(trace.surfaceFlags & SURF_NOIMPACT) {
		return qfalse;
	}

	/// Apply damage to victim.
	if (victim->takedamage) {
		G_Damage(
			victim,
			owner,
			owner,
			forward,
			trace.endpos,
			DEF_IRON_PISTOL_DAMAGE,
			0,
			MOD_IRON_PISTOL
		);

		return qtrue;
	}

	return qfalse;
}

///
/// IronPistolWeaponFire
/// Game server side logic for pistol fire.
void IronPistolWeaponFire(gentity_t *owner, vec3_t forward, vec3_t muzzle) {
	
	/// Create temporary event entity to notify client.
	gentity_t*	event; 
	event = G_TempEntity(muzzle, EV_IRONPISTOL);

	/// Populate event info.
	VectorScale(forward, DEF_IRON_PISTOL_RANGE, event->s.origin2);
	SnapVector(event->s.origin2);
	event->s.eventParm = rand() & 255;
	event->s.otherEntityNum = owner->s.number;

	/// Perform pistol gunshot.
	IronPistolGunshot(event->s.pos.trBase, event->s.origin2, owner, forward);

	/// TODO: Create blast entity.
}
