#ifndef BASIC_MATERIAL_H
#define BASIC_MATERIAL_H
#include "stable.h"
#include <QImage>




class Basic_Material : public Material
{
protected:
    //QGLShader materialSahder;

    float SpecularLevel;
    float normalStrength;
    QImage TextureMaps[3];
    //textures
public:
    Basic_Material();
    ~Basic_Material();

    void setParameters(float Specular,float normalLevel){
        SpecularLevel=Specular;
        normalStrength=normalLevel;
    }
    void alignTexture();
    void BindParameters();
    void InitShader(){
        //loadshader
        std::string shader1=(QCoreApplication::applicationDirPath()+"/barkbasic.vert").toStdString();
        std::string shader2=(QCoreApplication::applicationDirPath()+"/barkbasic.frag").toStdString();
        LoadShaders(shader1.c_str(),shader2.c_str());
        //glUseProgram(ShaderProgramID);
        alignTexture();
        BindParameters();
    }
};

#endif // BASIC_MATERIAL_H
