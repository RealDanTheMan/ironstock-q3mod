#include "cg_local.h"

#define MOD_HUD_ICON_SIZE	10
#define MOD_HUD_TEXT_SIZE	SMALLCHAR_WIDTH * 1.25

///
/// CG_ModDrawNum
/// Modified version of CG_DrawField which allows custom character size.
static void CG_ModDrawNum (int x, int y, int count, int value, float width, float height) {
	char	num[16], *ptr;
	int		l;
	int		frame;

	if (count < 1) {
		return;
	}

	// draw number string
	if (count > 5) {
		count = 5;
	}

	switch (count) {
	case 1:
		value = value > 9 ? 9 : value;
		value = value < 0 ? 0 : value;
		break;
	case 2:
		value = value > 99 ? 99 : value;
		value = value < -9 ? -9 : value;
		break;
	case 3:
		value = value > 999 ? 999 : value;
		value = value < -99 ? -99 : value;
		break;
	case 4:
		value = value > 9999 ? 9999 : value;
		value = value < -999 ? -999 : value;
		break;
	}

	Com_sprintf (num, sizeof(num), "%i", value);
	l = strlen(num);
	if (l > count)
		l = count;
	x += 2 + width*(count - l);

	ptr = num;
	while (*ptr && l)
	{
		if (*ptr == '-')
			frame = STAT_MINUS;
		else
			frame = *ptr -'0';

		CG_DrawPic( x,y, width, height, cgs.media.numberShaders[frame] );
		x += width;
		ptr++;
		l--;
	}
}

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

	// Draw health icon and points.
	CG_DrawPic(6, 10, MOD_HUD_ICON_SIZE, MOD_HUD_ICON_SIZE, cgs.media.mod_hudHpIcon);
	if (hp < 10) {
		CG_ModDrawNum(16, MOD_HUD_TEXT_SIZE, 1, hp, MOD_HUD_TEXT_SIZE, MOD_HUD_TEXT_SIZE);
	}
	else if (hp < 100) {
		CG_ModDrawNum(16, MOD_HUD_TEXT_SIZE, 2, hp, MOD_HUD_TEXT_SIZE, MOD_HUD_TEXT_SIZE);
	}
	else {
		CG_ModDrawNum(16,MOD_HUD_TEXT_SIZE, 3, hp, MOD_HUD_TEXT_SIZE, MOD_HUD_TEXT_SIZE); 
	}

	// Draw armor icon and points.
	CG_DrawPic(
		6,
		MOD_HUD_TEXT_SIZE*2,
		MOD_HUD_ICON_SIZE,
		MOD_HUD_ICON_SIZE,
		cgs.media.mod_hudArmorIcon
	);

	if (armor < 10) {
		CG_ModDrawNum(16, MOD_HUD_TEXT_SIZE*2, 1, armor, MOD_HUD_TEXT_SIZE, MOD_HUD_TEXT_SIZE);
	}
	else if (armor < 100) {
		CG_ModDrawNum(16, MOD_HUD_TEXT_SIZE*2, 2, armor, MOD_HUD_TEXT_SIZE, MOD_HUD_TEXT_SIZE);
	}
	else {
		CG_ModDrawNum(16, MOD_HUD_TEXT_SIZE*2, 3, armor, MOD_HUD_TEXT_SIZE, MOD_HUD_TEXT_SIZE);
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
	CG_ModDrawNum(
		55,
		425,
		3,
		state->ammo[player->currentState.weapon],
		MOD_HUD_TEXT_SIZE,
		MOD_HUD_TEXT_SIZE
	);
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
