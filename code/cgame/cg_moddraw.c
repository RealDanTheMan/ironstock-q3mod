#include "cg_local.h"

///
/// CG_DrawCrosshair
/// Draws player weapon crosshair on the screen.
///
static void CG_ModDrawCrosshair(void)
{
	float		w, h;
	qhandle_t	hShader;
	float		f;
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
/// CG CG_ModDraw
/// Draws 2D elements on the client screen.
///
static void CG_ModDraw2D(stereoFrame_t stereoFrame) {
	CG_ModDrawCrosshair();
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
