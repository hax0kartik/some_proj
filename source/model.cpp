#include "model.h"
#include "objloader.h"
void Model::Load(std::string name, std::string path)
{
	objl::Loader Loader;
	bool loadout = Loader.LoadFile(path.c_str());
	if (!loadout)
		printf("Loading failed");
	else
	{
		std::vector<vertex> vertex_list;
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
		{
			// Copy one of the loaded meshes to be our current mesh
			objl::Mesh curMesh = Loader.LoadedMeshes[i];
			printf("Vertices:\n");
			for (int j = 0; j < curMesh.Vertices.size(); j++)
			{
				vertex vert = 
				{
					{curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z },
					{curMesh.Vertices[j].TextureCoordinate.X, curMesh.Vertices[j].TextureCoordinate.Y },
					{curMesh.Vertices[j].Normal.X, curMesh.Vertices[j].Normal.Y, curMesh.Vertices[j].Normal.Z }
				};
				vertex_list.push_back(vert);
			}
			static C3D_Mtx *modelView = new C3D_Mtx;
			models.push_back(make_tuple(name, modelView, vertex_list));
			printf("Loaded:\n");
		}
	}
}

void Model::BindToBufInfo(std::vector<std::tuple<C3D_BufInfo*, size_t>> &vbos)
{
	// Create the VBO (vertex buffer object)
	for (int i = 0; i < this->models.size(); i++)
	{
		std::vector<vertex> vertices = std::get<2>(this->models[i]);
		void *vbo_data = linearAlloc(vertices.size() * sizeof(vertex));
		memcpy(vbo_data, (void *)vertices.data(), vertices.size() * sizeof(vertex));
		// Configure buffers
		C3D_BufInfo* bufInfo = C3D_GetBufInfo();
		BufInfo_Init(bufInfo);
		BufInfo_Add(bufInfo, vbo_data, sizeof(vertex), 3, 0x210);
		vbos.push_back(std::make_tuple(bufInfo, vertices.size()));
	}
}

C3D_Mtx *Model::GetModelViewMtx(std::string model)
{
	for(auto i: this->models)
	{
		auto model_name = std::get<0>(i);
		if(model_name.compare(model) == 0)
		{
			printf("Model found. Returning the modelView mtx");
			return std::get<1>(i);
		}
	}
	return nullptr;
}

void Model::BindModelToModelMtx(int model_no, int modelView)
{
	C3D_Mtx *mtx = std::get<1>(models[model_no]);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, modelView,  mtx);
}