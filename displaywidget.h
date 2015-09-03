#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H
#include "stable.h"
#include <QObject>
#include <QWidget>
#include <QGLWidget>
#include <QDir>
#include <QOpenGLShaderProgram>

class DisplayWidget : public  QGLWidget
{
    Q_OBJECT
protected:   //variables
    //Scene defualt Scene
    Camera *cur_Camera;
//    L_systemCompiler L_system;
    Stochastic_l_systemcomplier L_system;
    Scene Main_Scene;
    Model model;
    fbx_loader ModelLoader;
    Basic_Material *material;
    Simple_material *smaterial;
    Point_Light *pointLight;
    GLuint ProgramID;
    QOpenGLShaderProgram Program1;
public:
    DisplayWidget(QWidget *parent = 0);
    ~DisplayWidget();
public slots:
    void changeAngle(QString degree);
    void UpdateL_system();
    void ChangeTemplate(QString TemplateName);
    void changeAxiom(QString SAxiom); //change axiom
    void changeIteration(int iteration); //change iteration
    void getChangedColor(QColor newcolor); //change color
signals:
    void axiomRead(QString axiom);
    void angleRead(QString angle);
    void RuleRead(QString rules);
    void iterationRead(int iterate);
    void RuleCalculated(QString FinalRule);
protected:
    void Init();
    void InitL_system();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent* e);
    ///////////////////////////////////////////////
    //l-system
    void ChangeFile(std::string fileIndicator);
    void setAxiom(std::string axiom);
    void setIteration(std::string iteration);
    void setAngle(std::string angle);
    GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
};

#endif // DISPLAYWIDGET_H
