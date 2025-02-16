// ENVIRONMENT.SHADER
// 
// This file contains shaders that are used to draw map environment objects
// NOTE: shader entries have to begin with textures/ or game editor won't pick them up.
//


textures/env/shader_floor_tile_01
{
	qer_editorimage		textures/env/floor_tile_detail_01.tga

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
