#pragma once
#include <vector>
#include <tuple>
#include <3ds.h>
#include <citro3d.h>
#include "vshader_shbin.h"
#include "model.h"
class Render
{
    public:
    Render();
    std::vector<std::tuple<C3D_BufInfo*, size_t>>& GetVBOVector(){ return bufinfos; };
    void Draw();
    Model model;

    private:
    DVLB_s* vshader_dvlb;
    shaderProgram_s program;
    int uLoc_projection, uLoc_modelView;
    int uLoc_lightVec, uLoc_lightHalfVec, uLoc_lightClr, uLoc_material;
    C3D_Mtx projection;
    C3D_Mtx material =
    {
	    {
	    { { 0.0f, 0.2f, 0.2f, 0.2f } }, // Ambient
	    { { 0.0f, 0.4f, 0.4f, 0.4f } }, // Diffuse
	    { { 0.0f, 0.8f, 0.8f, 0.8f } }, // Specular
	    { { 1.0f, 0.0f, 0.0f, 0.0f } }, // Emission
	    }
    };
    std::vector<std::tuple<C3D_BufInfo*, size_t>> bufinfos;
};