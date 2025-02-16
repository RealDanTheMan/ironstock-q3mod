// ENVIRONMENT.SHADER
// 
// This file contains shaders that are used to draw map environment objects
//


environment/shader_floor_tile_01
{
	qer_editorimage		textures/env/floor_tile_detail_01.tga
	q3map_surfacelight	1

	{
		map			textures/env/floor_tile_base_01.tga
		blendFunc	GL_ONE GL_ONE
		rgbGen		identity
	}
	{
		map			textures/env/floor_tile_detail_01.tga
		blendFunc	GL_ONE GL_ONE
		rgbGen wave sin 0.75 0.5 0.5 0.5
	}
	{
		map			textures/env/floor_tile_overlay_01.tga
		blendFunc	GL_ONE GL_ONE
		rgbGen wave sin 0.9 0.5 0 0.5
	}
}

environment/test
{
	nopicmip
	nomipmaps
	{
		map textures/test/testgrid_yellow.tga
		rgbGen identity
	}
}
