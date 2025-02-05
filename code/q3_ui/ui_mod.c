#include "ui_mod.h"

///
/// modmainmenu_t
/// Represents mod custom main menu object.
typedef struct {
	menuframework_s menu;

	menutext_s		startgame;
	menutext_s		exitgame;
} modmainmenu_t;

static modmainmenu_t s_modmain;

void UI_ModMenuCache(void) {
	uis.charset					= trap_R_RegisterShaderNoMip("text/default_text");
	uis.charsetProp				= trap_R_RegisterShaderNoMip("gfx/2d/fonts/menu_font1.tga");
	uis.charsetPropGlow			= trap_R_RegisterShaderNoMip("gfx/2d/fonts/menu_font1_glow.tga");
	uis.charsetPropB			= trap_R_RegisterShaderNoMip("gfx/2d/fonts/menu_font2.tga");
	uis.cursor					= trap_R_RegisterShaderNoMip("mod_menu/cursor_a.tga");
	uis.menuBackShader			= trap_R_RegisterShaderNoMip("ui/menu_bg");
	uis.menuBackNoLogoShader	= trap_R_RegisterShaderNoMip("ui/menu_bg");
}

///
/// UI_ModLoadMap
/// Loads default mod test map.
void UI_ModLoadMap(const char* map_name) {
	Com_Printf( "Loading standalone mod map\n" );

	trap_Cvar_SetValue("sv_pure", 0);
	trap_Cvar_SetValue("bot_enable", 0);
	trap_Cvar_SetValue("g_gametype", 0);

	trap_Cmd_ExecuteText( EXEC_APPEND, va( "wait ; wait ; map %s; wait; \n", map_name));
}

///
/// UI_ModExitMenu
/// Exits main menu.
static void UI_ModExitMenu(qboolean result) {

	if( !result ) {
		return;
	}
	UI_PopMenu();
	UI_CreditMenu();
}

///
/// Mod_Main_MenuEvent
/// Handle custom mod main menu events.
void Mod_Main_MenuEvent (void* ptr, int event) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_MOD_BEGIN:
		UI_ModLoadMap("helloquake");
		break;

	case ID_MOD_EXIT:
		UI_ConfirmMenu( "EXIT GAME?", 0, UI_ModExitMenu);
		break;
	}
}

/// 
/// UI_ModMainMenuDraw
/// Draws custom mod main menu.
static void UI_ModMainMenuDraw( void ) {
	vec4_t color = {0.5, 0, 0, 1};
	Menu_Draw( &s_modmain.menu );

	UI_DrawString(
		320,
		450,
		"Ironstock(c) 2025, Mihai & Dan, Inc.  All Rights Reserved",
		UI_CENTER|UI_SMALLFONT,
		color
	);
}

///
/// UI_ModMainMenu
/// Constructs custom mod main menu.
void UI_ModMainMenu(void) {

	int y = 134;
	int	style = UI_CENTER | UI_DROPSHADOW;

	trap_Cvar_Set( "sv_killserver", "1" );
	memset(&s_modmain, 0, sizeof(modmainmenu_t));

	s_modmain.menu.draw = UI_ModMainMenuDraw;
	s_modmain.menu.fullscreen = qtrue;
	s_modmain.menu.wrapAround = qtrue;
	s_modmain.menu.showlogo = qfalse;

	s_modmain.startgame.generic.type		= MTYPE_PTEXT;
	s_modmain.startgame.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_modmain.startgame.generic.x			= 320;
	s_modmain.startgame.generic.y			= y;
	s_modmain.startgame.generic.id			= ID_MOD_BEGIN;
	s_modmain.startgame.generic.callback	= Mod_Main_MenuEvent; 
	s_modmain.startgame.string				= "Begin Game";
	s_modmain.startgame.color				= color_white;
	s_modmain.startgame.style				= style;

	y += MOD_MAIN_MENU_SPACING;
	s_modmain.exitgame.generic.type			= MTYPE_PTEXT;
	s_modmain.exitgame.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_modmain.exitgame.generic.x			= 320;
	s_modmain.exitgame.generic.y			= y;
	s_modmain.exitgame.generic.id			= ID_MOD_EXIT;
	s_modmain.exitgame.generic.callback		= Mod_Main_MenuEvent; 
	s_modmain.exitgame.string				= "Exit";
	s_modmain.exitgame.color				= color_red;
	s_modmain.exitgame.style				= style;

	Menu_AddItem(&s_modmain.menu, &s_modmain.startgame);
	Menu_AddItem(&s_modmain.menu, &s_modmain.exitgame);

	trap_Key_SetCatcher(KEYCATCH_UI);
	uis.menusp = 0;
	UI_PushMenu(&s_modmain.menu);
}
