// ENVIRONMENT.SHADER
// 
// This file contains shaders that are used to draw map environment objects
// NOTE: shader entries have to begin with textures/ or game editor won't pick them up.
//


textures/env/shader_floor_tile_01
{
	qer_editorimage		textures/env/floor_tile_detail_01.tga

	surfaceparm nolightmap
	{
		map			textures/env/floor_tile_base_01.tga
		blendFunc	GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen		identity
	}

	surfaceparm nolightmap
	cull disable
	{
		map			textures/env/floor_tile_detail_01.tga
		blendFunc	GL_ONE GL_ONE
		alphaFunc	GE128
		rgbGen		wave triangle 0.9 1.25 0.5 0.5
	}

	surfaceparm nolightmap
	cull disable
	{
		map			textures/env/floor_tile_overlay_01.tga
		blendFunc	GL_ONE GL_ONE
		alphaFunc	GE128
		rgbGen		wave triangle 0.9 1.25 0.0 0.5
	}
}

textures/env/shader_wall_tile_01
{
	qer_editorimage		textures/env/wall_tile_detail_01.tga

	surfaceparm nolightmap
	{
		map			textures/env/wall_tile_base_01.tga
		blendFunc	GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen		identity
	}

	surfaceparm nolightmap
	cull disable
	{
		map			textures/env/wall_tile_detail_01.tga
		blendFunc	GL_ONE GL_ONE
		alphaFunc	GE128
		rgbGen		wave sin 0.9 1.25 0.5 0.5
	}

	surfaceparm nolightmap
	cull disable
	{
		map			textures/env/wall_tile_overlay_01.tga
		blendFunc	GL_ONE GL_ONE
		alphaFunc	GE128
		rgbGen		wave sin 0.9 1.25 0 0.5
	}
}
