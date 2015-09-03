//Model.h
// author Qiushuo Wang
//Base class for geometry
#ifndef MODEL_H_
#define MODEL_H_



#include "stable.h"


//parameters
enum ConvertType{MODEL_BASIC,MODEL_FACE_NORMAL,MODEL_BOTH_NORMAL, MODEL_UV,MODEL_UV_POS,MODEL_UV_BOTH_NORMAL,MODEL_UV_NORMAL_BT};

enum Draw_Mode{SMOOTH,FLAT};

enum UV_method{UV_LACK,UV_VERTEX,UV_INDEXED,UV_RAW};

enum Trans_Space {TRANS_WORLD,TRANS_SELF,TRANS_PARENT,TRANS_SCREEN,TRANS_PIVOT};   //transformation method

enum ForwardType {F_DRAW, F_NO_DRAW};

const float PI=3.1415926f;

class Model :public Transformation
{
protected:
	std::string Name;

	
	////////basic rendering parameters//////////////////////////////////////////////////////////////////
	std::vector<Vec3f> m_verts;    //vertex
	std::vector<std::vector<int>> m_polygons;  //polygon index (vertex normal)
	std::vector<Vec3f> m_face_normal;   //face normal
	std::vector<Vec3f> m_vert_normal;   //vertex normal
	//VBO
	std::vector<float> m_vbo_verts_raw;  //unindexed verts
	//////////////////////////////////////////////////////////////////////////

	///////geometry parameters///////////////////////////////////////////////////////////////////
	Vec3f m_center;  //center
	Vec3f m_min_bounding; //for bounding box
	Vec3f m_max_bounding; //for bounding box

	///pivot
    //Vec3f pivot_pos;
    //Vec3f pivot_x_direc;
    //Vec3f pivot_y_direc;
    //Vec3f pivot_z_direc;


	//////////////////////////////////////////////////////////////////////////

	//UV coordinate
	bool haveUV=false;
	std::vector<Vec2f> m_uv_coord;
	std::vector<std::vector<int>> m_uv_index;
	std::vector<std::vector<Vec2f>> m_uv_raw;  //un-indexed uv coordinate

	//normal mapping and advance 
    std::vector<std::vector<Vec3f>> m_tangent;
    std::vector<std::vector<Vec3f>> m_bitengent;

    //VBOs
    GLfloat *vbo_vertex;
    GLfloat *vbo_normal;
    GLfloat *vbo_uv;
    GLfloat *vbo_tangent;
    GLfloat *vbo_binormal;
    unsigned int vbo_size;
    Mat4f ProjectionViewMatrix;
    Mat3f ViewnMat;

	//material
    GLuint materialID;
	Material *pMaterial;
private:
	void GenerateTengentSpace();

public:
	//construction function
	Model();
	Model(const Model &origin){ *this = origin; }
    //copy constructor
    //Model(const Model &rhs){
        //m_verts=rhs.m_verts;
        //m_polygons=rhs.polygons;
        //m_normals=rhs.m_normals;
    //}

    //Model* operator=(const Model &origin){return *this=origin;}

	Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon) :m_verts(verts), m_polygons(polygon){   //vertex and index only
		Initate();
		getGeometryParameters();
	}
	///with normal
	Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon, std::vector<Vec3f> &Normals) :m_verts(verts), m_polygons(polygon), m_vert_normal(Normals){ //vertex, vertex index, vertex normal
		Initate();
		getGeometryParameters();
	}
	Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon, std::vector<Vec3f> &Normals, Vec3f center) :m_verts(verts), m_polygons(polygon), m_vert_normal(Normals),m_center(center){  //vertex, vertex index, vertex normal with geometry center
		Initate();
		getGeometryParameters();
	}
	// with texture coordinates
	Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon, std::vector<Vec3f> &Normals, std::vector<Vec2f> &UV_coord) :m_verts(verts), m_polygons(polygon), m_vert_normal(Normals), m_uv_coord(UV_coord){   //with texture coordinate --single uv pre-pixel
		Initate();
        haveUV=true;
		getGeometryParameters();
	}

	Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon, std::vector<Vec3f> &Normals, std::vector<std::vector<Vec2f>> &Raw_UV_coord) :m_verts(verts), m_polygons(polygon), m_vert_normal(Normals), m_uv_raw(Raw_UV_coord){   //with texture coordinate --raw
		Initate();
        haveUV=true;
		getGeometryParameters();
	}

	Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon, std::vector<Vec3f> &Normals, std::vector<Vec3f> &face_Normals, std::vector<std::vector<Vec2f>> &Raw_UV_coord) :m_verts(verts), m_polygons(polygon), m_vert_normal(Normals),m_face_normal(face_Normals), m_uv_raw(Raw_UV_coord){   //with texture coordinate --raw
		Initate();
        haveUV=true;
		getGeometryParameters();
	}

	Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon, std::vector<Vec3f> &Normals, std::vector<Vec2f> &UV_coord, Vec3f &center) :m_verts(verts), m_polygons(polygon), m_vert_normal(Normals), m_uv_coord(UV_coord), m_center(center){  //with texture coordinate and center
		Initate();
        haveUV=true;
		getGeometryParameters();
	}

    Model(std::vector<Vec3f> &verts, std::vector<std::vector<int>> &polygon, std::vector<Vec3f> &Normals, std::vector<std::vector<Vec2f>> &Raw_UV_coord, std::vector<std::vector<Vec3f>> &tangents,std::vector<std::vector<Vec3f>> &bitangents) :m_verts(verts), m_polygons(polygon), m_vert_normal(Normals),m_uv_raw(Raw_UV_coord), m_tangent(tangents),m_bitengent(bitangents){  //with texture coordinate and center
        Initate();
        haveUV=true;
        getGeometryParameters();
    }
	~Model();

    void AlignTransformation(Transformation &Trans){
        //Trans.getTransParameters(Parent_Matrix,Translate_Matrix,Rotation_Matrix,Scale_Matrix,Transformation_Matrix,pivot,pivotSpace);
        Translate_Matrix=Trans.getTranslateMatrix();
        Rotation_Matrix=Trans.getRotationMatrix();
        Scale_Matrix=Trans.getScaleMatrix();
        Transformation_Matrix=Trans.getTransformationMatrix();
        pivot=Trans.getPivot();
    }
	//building functions
	void getGeometryParameters();

	void CreateVertexVBO();
	void CreateNormalVBO();
	void CreateTextureVBO();
    void CreateTangentSpaceVBO();
    void CreateVBO(){
        vbo_size=m_polygons.size()*3;
        //GenerateTengentSpace();
        CreateVertexVBO();
        CreateNormalVBO();
        CreateTextureVBO();
        CreateTangentSpaceVBO();
    }

	//transformations
	void setParent(Mat4f P_Matrix){
		Parent_Matrix = P_Matrix;
	}
    void Translate(Vec3f &transVector, Trans_Space space);
    void Rotation(float degree, Vec3f &axis, Trans_Space space);
    void RelativeRotation(float degree, Vec3f &axis, Trans_Space space);
	void Scale(float percentage);
	Vec3f HomoGenTransformation(Mat4f TransMat, Vec3f Vec);

    void Forward(ForwardType flag);
	//pivot transformation

	//draw
    void AlignMaterial(Material *material){
        pMaterial = material;
	}
	void setmaterialID(GLuint ProgramID){
		materialID = ProgramID;
	}

    void setMatrial(Material *pmaterial){
        pMaterial=pmaterial;
    }

	void ClassicDraw(int drawType, int uv_type);

	void VBODraw();

	int getMaterialID(){
		return materialID;
	}

	//debug mode
	void drawPivot(){}
	void drawBoundingBox(){}

    void setPovitToBottom();

    void setProjectionViewMatrix(Mat4f ProMat){
        ProjectionViewMatrix=ProMat;
    }

    void setViewMatrix3x3(Mat3f viewMat){
        ViewnMat=viewMat;
    }
};

//////////////////////////////////////////////////
//class converters

//convert fbx loaded file to Model
inline Model convert2Model(fbx_loader FBX_Model, int CRT_FLAG){
	Model newModel;
	if (CRT_FLAG == MODEL_BASIC){
		newModel = Model(FBX_Model.m_vertex_list, FBX_Model.m_polygon, FBX_Model.m_vertex_normal);
	}
	else if (CRT_FLAG==MODEL_UV)
	{
		newModel = Model(FBX_Model.m_vertex_list, FBX_Model.m_polygon, FBX_Model.m_vertex_normal,FBX_Model.computed_face_norm, FBX_Model.m_uv_coord);
	}
    else if (CRT_FLAG==MODEL_UV_NORMAL_BT){
        newModel =Model(FBX_Model.m_vertex_list, FBX_Model.m_polygon, FBX_Model.m_vertex_normal, FBX_Model.m_uv_coord, FBX_Model.m_tangent,FBX_Model.m_bitangent);
    }
	
	return newModel;
}



#endif // MODEL_H_
