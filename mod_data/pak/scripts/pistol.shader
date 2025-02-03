// PISTOL.SHADER
// 
// This file contains shaders that are used to draw various pistol weapon effects.
//


pistol/smoke
{
	nopicmip
	nomipmaps
	{
		map weapons/pistol/pistol_smoke.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

pistol/bullethole
{
	nopicmip
	nomipmaps
	{
		map weapons/pistol/pistol_bullethole.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
