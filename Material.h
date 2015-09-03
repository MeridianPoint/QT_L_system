#ifndef Material_h__
#define Material_h__
#include "stable.h"

const unsigned int textureNumber=3;
class Material
{
protected:
    GLuint TextureID[textureNumber];
    GLuint ShaderProgramID;
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
    //texture map
	//std::list<Texture_Map> MapList;
public:
	Material();
	Material(std::string vertexshader,std::string fragmentshader);
	virtual ~Material();
    virtual void alignTexture()=0;
    virtual void BindParameters()=0;
    void LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

    //get set
    GLuint getProgramID(){
        return ShaderProgramID;
    }

    void SetUpMaterial(){
        glUseProgram(ShaderProgramID);
        BindParameters();
        alignTexture();
    }

    GLuint getTextureID(unsigned int index){
        return TextureID[index];
    }
};



#endif // Material_h__


