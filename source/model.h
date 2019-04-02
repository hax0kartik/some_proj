#pragma once
#include <citro3d.h>
#include <vector>
#include <tuple>
#include <3ds.h>

typedef struct { float position[3]; float texcoord[2]; float normal[3]; } vertex;
class Model
{
	public:
	void Load(std::string name, std::string path);
	void BindToBufInfo(std::vector<std::tuple<C3D_BufInfo*, size_t >> &vbos);
	C3D_Mtx *GetModelViewMtx(std::string model);
	void BindModelToModelMtx(int model_no, int modelView);
	
	private:
	// name, modelView, vertices
	std::vector<std::tuple<std::string, C3D_Mtx *, std::vector<vertex>>> models;
};