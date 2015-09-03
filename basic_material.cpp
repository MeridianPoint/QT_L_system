#include "stable.h"
#include "basic_material.h"

Basic_Material::Basic_Material()
{

}

Basic_Material::~Basic_Material()
{

}

void Basic_Material::alignTexture(){
    //load
    std::string T1filename=(QCoreApplication::applicationDirPath()+"/resources/DiffuseMap.jpg").toStdString();
    TextureMaps[0].load(T1filename.c_str());

    std::string T2filename=(QCoreApplication::applicationDirPath()+"/resources/SpecularMap.jpg").toStdString();
    TextureMaps[1].load(T2filename.c_str());

    std::string T3filename=(QCoreApplication::applicationDirPath()+"/resources/NormalMap.jpg").toStdString();
    TextureMaps[2].load(T3filename.c_str());

    //bind
    glUseProgram(ShaderProgramID);

    GLuint texLoc;

    texLoc = glGetUniformLocation(ShaderProgramID, "DiffuseTextureSampler");
    glUniform1i(texLoc, 0);

    texLoc = glGetUniformLocation(ShaderProgramID, "SpecularTextureSampler");
    glUniform1i(texLoc, 1);

    texLoc = glGetUniformLocation(ShaderProgramID, "NormalTextureSampler");
    glUniform1i(texLoc, 2);

    glGenTextures(3, TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID[0]);
    gluBuild2DMipmaps(GL_TEXTURE_2D,TextureMaps[0].format(),TextureMaps[0].width(),TextureMaps[0].height(),TextureMaps[0].format(),GL_UNSIGNED_BYTE,TextureMaps[0].data_ptr());
    glTexImage2D(GL_TEXTURE_2D,0,3,TextureMaps[0].width(),TextureMaps[0].height(),0,TextureMaps[0].format(),GL_UNSIGNED_BYTE,TextureMaps[0].data_ptr());

    glBindTexture(GL_TEXTURE_2D, TextureID[1]);
    gluBuild2DMipmaps(GL_TEXTURE_2D,TextureMaps[1].format(),TextureMaps[1].width(),TextureMaps[1].height(),TextureMaps[1].format(),GL_UNSIGNED_BYTE,TextureMaps[1].data_ptr());
    glTexImage2D(GL_TEXTURE_2D,0,3,TextureMaps[1].width(),TextureMaps[1].height(),0,TextureMaps[1].format(),GL_UNSIGNED_BYTE,TextureMaps[1].data_ptr());

    glBindTexture(GL_TEXTURE_2D, TextureID[2]);
    gluBuild2DMipmaps(GL_TEXTURE_2D,TextureMaps[2].format(),TextureMaps[2].width(),TextureMaps[2].height(),TextureMaps[2].format(),GL_UNSIGNED_BYTE,TextureMaps[2].data_ptr());
    glTexImage2D(GL_TEXTURE_2D,0,3,TextureMaps[2].width(),TextureMaps[2].height(),0,TextureMaps[2].format(),GL_UNSIGNED_BYTE,TextureMaps[2].data_ptr());
}
void Basic_Material::BindParameters(){
    glUseProgram(ShaderProgramID);

    GLuint Specularlevel=glGetUniformLocation(Specularlevel,"specularlevel");
    glUniform1f(Specularlevel,SpecularLevel);

    GLuint ns=glGetUniformLocation(ns,"NormalStrength");
    glUniform1f(ns,normalStrength);

}
