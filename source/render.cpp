#include "render.h"

Render::Render()
{
    // Load the vertex shader, create a shader program and bind it
	this->vshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin, vshader_shbin_size);
	shaderProgramInit(&this->program);
	shaderProgramSetVsh(&this->program, &this->vshader_dvlb->DVLE[0]);
	C3D_BindProgram(&this->program);

	// Get the location of the uniforms
	this->uLoc_projection   = shaderInstanceGetUniformLocation(this->program.vertexShader, "projection");
	this->uLoc_modelView    = shaderInstanceGetUniformLocation(this->program.vertexShader, "modelView");
	this->uLoc_lightVec     = shaderInstanceGetUniformLocation(this->program.vertexShader, "lightVec");
	this->uLoc_lightHalfVec = shaderInstanceGetUniformLocation(this->program.vertexShader, "lightHalfVec");
	this->uLoc_lightClr     = shaderInstanceGetUniformLocation(this->program.vertexShader, "lightClr");
	this->uLoc_material     = shaderInstanceGetUniformLocation(this->program.vertexShader, "material");

	// Configure attributes for use with the vertex shader
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v1=texcoord
	AttrInfo_AddLoader(attrInfo, 2, GPU_FLOAT, 3); // v2=normal

	// Compute the projection matrix
	Mtx_PerspTilt(&this->projection, C3D_AngleFromDegrees(80.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, false);

	// Configure the first fragment shading substage to blend the texture color with
	// the vertex color (calculated by the vertex shader using a lighting algorithm)
	// See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR, (GPU_TEVSRC)0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_ADD);
}

void Render::Draw()
{
    for (int i = 0; i < this->bufinfos.size(); i++)
	{
		C3D_BufInfo* bufinfo = std::get<0>(bufinfos[i]);
		C3D_SetBufInfo(bufinfo);
		C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, this->uLoc_projection, &this->projection);
		model.BindModelToModelMtx(i, this->uLoc_modelView);
		C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, this->uLoc_material,   &this->material);
		C3D_FVUnifSet(GPU_VERTEX_SHADER, this->uLoc_lightVec,     1.0f, 0.0f, -1.0f, 1.0f);
		//C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightHalfVec, 1.0f, 0.0f, -1.0f, 0.0f);
		C3D_FVUnifSet(GPU_VERTEX_SHADER, this->uLoc_lightClr,     1.0f, 1.0f,  0.0f, 1.0f);
		// Draw the VBO
		C3D_DrawArrays(GPU_TRIANGLES, 0, std::get<1>(bufinfos[i]));
	}
}