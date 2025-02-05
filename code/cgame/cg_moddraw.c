#include "cg_local.h"

void CG_DrawField (int x, int y, int width, int value);

///
/// CG_DrawCrosshair
/// Draws player weapon crosshair on the screen.
///
static void CG_ModDrawCrosshair(void)
{
	float		w, h;
	qhandle_t	hShader;
	float		x, y;
	int			ca;

	/// Skip if no valid crosshair object exists.
	if ( !cg_drawCrosshair.integer ) {
		return;
	}

	/// Skip when spectating.
	if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR) {
		return;
	}

	/// Draw crosshair.
	w = h = cg_crosshairSize.value;
	x = cg_crosshairX.integer;
	y = cg_crosshairY.integer;
	CG_AdjustFrom640( &x, &y, &w, &h );

	ca = cg_drawCrosshair.integer;
	if (ca < 0) {
		ca = 0;
	}

	hShader = cgs.media.mod_crosshairtex;
	trap_R_DrawStretchPic(
		x + cg.refdef.x + 0.5 * (cg.refdef.width - w), 
		y + cg.refdef.y + 0.5 * (cg.refdef.height - h), 
		w,
		h,
		0,
		0,
		1,
		1,
		hShader
	);

	trap_R_SetColor( NULL );
}

///
/// CG_ModDrawHUDPlayerInfo
/// Draws player stats portion of the HUD (health & armor)
static void CG_ModDrawHUDPlayerInfo(playerState_t *ps) {

	const int hp	= ps->stats[STAT_HEALTH];
	const int armor	= ps->stats[STAT_ARMOR];

	// Draw health points.
	if (hp < 10) {
		CG_DrawField ( 16, 16, 1, hp);
	}
	else if (hp < 100) {
		CG_DrawField ( 16, 16, 2, hp);
	}
	else {
		CG_DrawField ( 16, 16, 3, hp); 
	}

	// Draw armor points.
	if (armor < 10) {
		CG_DrawField ( 16, 50, 1, armor);
	}
	else if (armor < 100) {
		CG_DrawField ( 16, 50, 2, armor);
	}
	else {
		CG_DrawField ( 16, 50, 3, armor);
	}
}

///
/// CG_ModDrawHUDWeaponInfo
/// Draws player weapon portion of the HUD.
static void CG_ModDrawHUDWeaponInfo(centity_t *player, playerState_t *state) {

	vec3_t			origin;
	vec3_t			angles;

	// Draw player weapon.
	if (player->currentState.weapon) {
		origin[0] = 50;
		origin[1] = 0;
		origin[2] = 0;
		angles[YAW] = 45 + 120 * sin( cg.time / 1000.0 );
		CG_Draw3DModel(
			CHAR_WIDTH*0.5 + TEXT_ICON_SPACE,
			410,
			ICON_SIZE,
			ICON_SIZE,
			cg_weapons[ player->currentState.weapon ].weaponModel,
			0,
			origin,
			angles
		);
	}

	// Draw player weapon ammo.
	CG_DrawField ( 32, 410, 3, state->ammo[player->currentState.weapon]);
}

///
/// CG_ModDrawHUD
/// Draw player HUD UI to the screen.
static void CG_ModDrawHUD(stereoFrame_t stereoFrame) {

	centity_t		*cent;
	playerState_t	*ps;

	// Get client entity and state.
	cent = &cg_entities[cg.snap->ps.clientNum];
	ps = &cg.snap->ps;

	CG_ModDrawHUDPlayerInfo(ps);
	CG_ModDrawHUDWeaponInfo(cent, ps);
}

///
/// CG CG_ModDraw
/// Draws 2D elements on the client screen.
///
static void CG_ModDraw2D(stereoFrame_t stereoFrame) {
	CG_ModDrawCrosshair();
	CG_ModDrawHUD(stereoFrame);
}

///
/// CG_ModDrawActive
/// Draws client graphics to screen (2D + 3D)
///
void CG_ModDrawActive(stereoFrame_t stereoFrame) {
	if ( !cg.snap ) {
		CG_DrawInformation();
		return;
	}

	CG_TileClear();
	trap_R_RenderScene( &cg.refdef );
	CG_ModDraw2D(stereoFrame);
}
