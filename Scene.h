
#ifndef Scene_h__
#define Scene_h__
#include "stable.h"

class Scene
{
private:
	std::vector<Model*> Models;
	std::vector<Material*> Materials;
    std::vector<Camera*> Cameras;
    unsigned int cur_CameraIdx;
public:
	Scene();
	~Scene();
	//functions

    void addModel(Model *pModel){  //add Model
		Models.push_back(pModel);
	}

	Model* LastModel(){
		return Models[Models.size() - 1];
	}

	void addMaterial(Material* pMaterial){
		Materials.push_back(pMaterial);
	}

	void linkMaterial(int model_index,int material_index){
		if (model_index<Models.size()&&material_index<Materials.size())
		{
            Models[model_index]->AlignMaterial(Materials[material_index]);
		}
		else{
            //throw exception("vector out of range");
		}
	}
	void draw(){
        //for each (Model* pModel in Models)
        //{
            //glUseProgram(pModel->getMaterialID());
            //pModel->VBODraw();
        //}

	}
};
#endif // Scene_h__

