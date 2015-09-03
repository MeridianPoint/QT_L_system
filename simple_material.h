#ifndef SIMPLE_MATERIAL_H
#define SIMPLE_MATERIAL_H
#include "stable.h"
#include "Material.h"

class Simple_material : public Material
{
private:
    float Ka;
    float Kd;
    float Ks;
    Vec3f Allcolor;
public:
    Simple_material();
    Simple_material(float ka,float kd,float ks,Vec3f icolor):Ka(ka),Kd(kd),Ks(ks),Allcolor(icolor){}
    ~Simple_material();


    inline void InitShader(){
        //loadshader
        std::string shader1=(QCoreApplication::applicationDirPath()+"/basicmaterial.vert").toStdString();
        std::string shader2=(QCoreApplication::applicationDirPath()+"/basicmaterial.frag").toStdString();
        LoadShaders(shader1.c_str(),shader2.c_str());
        glUseProgram(ShaderProgramID);
        BindParameters();
    }

    inline void BindParameters(){
            GLuint ColorID = glGetUniformLocation(ShaderProgramID, "color");
            glUniform3fv(ColorID, 3 * sizeof(float), Allcolor.getPtr());

            GLuint AmbientStrength = glGetUniformLocation(ShaderProgramID, "Ka");
            glUniform1f(AmbientStrength, Ka);

            GLuint DiffuseStrength = glGetUniformLocation(ShaderProgramID, "Kd");
            glUniform1f(DiffuseStrength, Kd);

            GLuint SpecularStrength = glGetUniformLocation(ShaderProgramID, "Ks");
            glUniform1f(SpecularStrength, Ks);
    }

    void setColor(Vec3f Color){
        Allcolor=Color;
    }
    void alignTexture();
};

#endif // SIMPLE_MATERIAL_H
