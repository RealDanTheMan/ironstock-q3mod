// PISTOL.SHADER
// 
// This file contains shaders that are used to draw various pistol weapon effects.
//


pistol/smoke
{
	cull none
	entityMergable
	{
		map weapons/pistol/pistol_smoke.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen		vertex
		alphaGen	vertex
	}
}

pistol/bullethole
{
	polygonOffset
	{
		map weapons/pistol/pistol_bullethole.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen exactVertex
	}
}

pistol/pistol
{
	cull disable
	{
		map textures/test/testgrid_blue.tga
	}
}
