#include "cg_local.h"

static void CG_IronPistolGunshot(vec3_t start, vec3_t end) {
	trace_t tr;
	int		start_surface;
	int		end_surface;

	// Ray trace environment to see what surface types we are hitting.
	CG_Trace(&tr, start, NULL, NULL, end, 0, MASK_SHOT);
	start_surface = CG_PointContents(start, 0);
	end_surface = CG_PointContents(tr.endpos, 0);

	// Create water bullet bubbles.
	if (start_surface == end_surface) {
		if (start_surface & CONTENTS_WATER) {
			CG_BubbleTrail(start, tr.endpos, 16);
		}
	}

	if (tr.surfaceFlags & SURF_NOIMPACT) {
		return;
	}

	if (cg_entities[tr.entityNum].currentState.eType == ET_PLAYER) {
		CG_MissileHitPlayer(WP_IRON_PISTOL, tr.endpos, tr.plane.normal, tr.entityNum );
	}
	else {
		CG_MissileHitWall(WP_IRON_PISTOL, 0, tr.endpos, tr.plane.normal, IMPACTSOUND_DEFAULT);
	}
}

///
/// CG_IronPistolFire
/// Client side callback for pistol fire.
void CG_IronPistolFire(entityState_t* es) {

	const int smoke_offset = 32;
	vec3_t smoke_velocity;
	vec3_t vec;

	// Get weapon muzzle direction vector.
	// origin2 contains point at max range.
	VectorSubtract(es->origin2, es->pos.trBase, vec);
	VectorNormalize(vec);

	// Spawn smoke just ahead of the muzzle
	VectorScale(vec, smoke_offset, vec);
	VectorAdd(es->pos.trBase, vec, vec);

	VectorSet(smoke_velocity, 0, 0, 8);
	CG_SmokePuff(
		vec,
		smoke_velocity,
		16,
		1,
		1,
		1,
		0.5f,
		600,
		cg.time,
		0,
		LEF_PUFF_DONT_SCALE,
		cgs.media.smokePuffShader
	);

	CG_IronPistolGunshot(es->pos.trBase, es->origin2);
}

