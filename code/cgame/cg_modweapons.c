#include "cg_local.h"

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

	/// TODO: Handle player hit effects.
	/// TODO: Handle wall hit effects.
	/// TODO: Handle bullet trail.
}

