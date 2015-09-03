/*
*class for loading fbx file 
*/
#ifndef FBX_LOADER_H_
#define FBX_LOADER_H_
#include "stable.h"
#include <fbxsdk.h>
#include <cassert>


#define MODEL_DIREC "..//resources//"



class fbx_loader
{
public:
	//
	std::vector<Vec3f> m_vertex_list;
	std::vector<Vec3f> m_vertex;
	std::vector<std::vector<int>> m_polygon;
	std::vector<Vec3f> m_vertex_normal;
	std::vector<Vec3f> m_poly_vertex_normal;
	//compute normal
	std::vector<Vec3f> computed_face_norm;
	std::vector<Vec3f> computed_Vert_norm;
	//uv coordinate
	std::vector<std::vector<Vec2f>> m_uv_coord;
	//additional
	std::vector<std::vector<Vec3f>> m_tangent;
	std::vector<std::vector<Vec3f>> m_bitangent;
public:
	fbx_loader();
	fbx_loader(std::string file_name);
	//
	int LoadModel(std::string file_name);
	void getVertexNormal(FbxMesh *pMesh);
	//
	void ComputeNormal();
	void draw();
	void easydraw();

	void drawNormal();

	//convert

	//
	~fbx_loader();
};



#endif
