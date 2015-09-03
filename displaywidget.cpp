#include "stable.h"
#include "displaywidget.h"
#include <QGLWidget>

DisplayWidget::DisplayWidget(QWidget *parent): QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

}

DisplayWidget::~DisplayWidget()
{

}

void DisplayWidget::changeAngle(QString degree){
    L_system.setAngle(degree.toFloat());
}

void DisplayWidget::Init(){
    //load model
    //QDir model_dir("/resources/");
    QString qs="/resources/cylinder.FBX";
    std::string ModelName=qs.toUtf8();
    ModelLoader.LoadModel(ModelName);
    model=convert2Model(ModelLoader,MODEL_UV_NORMAL_BT);
    model.CreateVBO();
    smaterial=new Simple_material(0.3,0.8,0.2,Vec3f(1.0,1.0,0.0));

    //set up camera

    //set up scene
    cur_Camera=new Camera();
    cur_Camera->setPosition(Vec3f(258.0,258.0,75.0));
//    cur_Camera->setPosition(Vec3f(15.0,15.0,13.0));
    cur_Camera->setTarget(Vec3f(-120.0,-120.0,230.0));
    cur_Camera->setUpVector(Vec3f(0.0,0.0,1.0));
    cur_Camera->SetFrustum(0.2f,250.0f,0.4f,1.0f);
    cur_Camera->setAspect(1.0);
    cur_Camera->CalculateObjSpace();

    model.setProjectionViewMatrix(cur_Camera->getProjectionViewMatrix());
    model.setViewMatrix3x3(cur_Camera->getViewMatrix3x3());
    //smaterial->InitShader();
    std::string shader1=(QCoreApplication::applicationDirPath()+"/basicmaterial.vert").toStdString();
    std::string shader2=(QCoreApplication::applicationDirPath()+"/basicmaterial.frag").toStdString();
    ProgramID=LoadShaders(shader1.c_str(),shader2.c_str());


    glUseProgram(ProgramID);

    GLfloat color[] = { 1.0f, 1.0f, 0.0f,1.0f};
        GLuint ColorID = glGetUniformLocation(ProgramID, "color");
        glUniform3fv(ColorID, 1, color);

        float ka = 0.7f;
        GLuint AmbientStrength = glGetUniformLocation(ProgramID, "Ka");
        glUniform1f(AmbientStrength, ka);

        float kd = 0.8f;
        GLuint DiffuseStrength = glGetUniformLocation(ProgramID, "Kd");
        glUniform1f(DiffuseStrength, kd);

        float ks = 0.2f;
        GLuint SpecularStrength = glGetUniformLocation(ProgramID, "Ks");
        glUniform1f(SpecularStrength, ks);


    L_system.linkScene(&Main_Scene);

    glUseProgram(ProgramID);

    glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        GLfloat lightpos[] = { 90.0, 90.0, 200.0, 0.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
        GLfloat white[] = { 0.9f, 0.9f, 0.9f, 1.0f };
        GLfloat black[]={0.0,0.0,0.0,1.0};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
            glLightfv(GL_LIGHT0, GL_SPECULAR, black);
            glLightfv(GL_LIGHT0, GL_AMBIENT, white);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
                glMaterialfv(GL_FRONT, GL_SPECULAR, white);
                glMaterialfv(GL_FRONT, GL_AMBIENT, white);
}

void DisplayWidget::InitL_system(){
//    std::string filename=(QCoreApplication::applicationDirPath()+"/resources/Standard.txt").toStdString();
    std::string filename=(QCoreApplication::applicationDirPath()+"/resources/StochasticsStandard3.txt").toStdString();
    L_system.CreateFromFile(filename.c_str());
    //emit signals
    QString QSAxiom=QString::fromStdString(L_system.getAxiom());
    emit axiomRead(QSAxiom);
    QString QSAngle=QString::number(L_system.getAngle());
    emit angleRead(QSAngle);

    emit RuleRead(L_system.getRules());

    //emit iteration
    //QString QSIteration=QString::number(L_system.getIteration());
    emit iterationRead(L_system.getIteration());

    emit RuleCalculated(L_system.getFinal());
}

void DisplayWidget::UpdateL_system(){
    //update L_system
    L_system.Update();
    //emit signals
    QString QSAxiom=QString::fromStdString(L_system.getAxiom());
    emit axiomRead(QSAxiom);
    QString QSAngle=QString::number(L_system.getAngle());
    emit angleRead(QSAngle);

    emit RuleRead(L_system.getRules());

    //emit iteration
    //QString QSIteration=QString::number(L_system.getIteration());
    emit iterationRead(L_system.getIteration());

    emit RuleCalculated(L_system.getFinal());
    //update draw
    update();
}

void DisplayWidget::initializeGL(){

//    InitL_system();


    GLenum err = glewInit();
        if(err != GLEW_OK)
        {
            printf("%s",glewGetErrorString(err));
        }
    Init();

/*    material=new Basic_Material();
    material->setParameters(0.2f,0.7f);
    material->InitShader()*/;

    model.setmaterialID(ProgramID);
    //model.AlignMaterial(smaterial);
    model.setPovitToBottom();
//    glUseProgram(material->getProgramID());
//    glUseProgram(smaterial->getProgramID());

//    pointLight=new Point_Light(Vec3f(50.0,50.0,50.0),Vec3f(1.0,1.0,1.0),1.0);
//    GLuint lightpos =glGetUniformLocation(material->getProgramID(),"LightPosition_worldspace");
//    glUniform3fv(lightpos,1,pointLight->getPosition().getPtr());

//    Mat4f View_Mat=cur_Camera->getViewMatrix();
//    GLuint ViewMatrixID=glGetUniformLocation(material->getProgramID(),"V");
//    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, View_Mat.ToArray());

    //material->alignTexture();
    //material->BindParameters();

    //Init L-system
    //L_system.CreateFromFile("/resources/L-systemRules/Standard.rule");

    //set up shader
}
void DisplayWidget::paintGL(){  //drawing function

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0.01f,0.0f,0.0,1.0);

    //model.VBODraw();
    //glMatrixMode(GL_PROJECTION_MATRIX);
    //glLoadIdentity();
    //glLoadMatrixf(cur_Camera->getProjectionMatrix().ToArray());


    //glMatrixMode(GL_MODELVIEW_MATRIX);
    //glLoadIdentity();


    //use Program
    //glUseProgram(material->getProgramID());



    //debugge
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    int fov = 2 * atan(cur_Camera->h()*cur_Camera->Aspect() /cur_Camera->n()) / PI * 180;
        //gluPerspective(fov, cur_Camera->Aspect(), cur_Camera->n(), cur_Camera->f());



    glLoadMatrixf(cur_Camera->getProjectionMatrix().ToArray());

    gluLookAt(cur_Camera->getPosx(), cur_Camera->getPosy(), cur_Camera->getPosz(),
        cur_Camera->getPosx() + cur_Camera->getDircetx(), cur_Camera->getPosy() + cur_Camera->getDircety(), cur_Camera->getPosz() + cur_Camera->getDircetz(),
        cur_Camera->getUpVectorx(), cur_Camera->getUpVectory(), cur_Camera->getUpVectorz());
    //glOrtho(-40.0,140.-30,0.0,40.0,-40.0,40.0);
    //glBegin(GL_TRIANGLES);
    //glColor3f(0.2,0.7,0.9);
    //glVertex3f(10.0,9.0,6.0);
    //glVertex3f(10.0,-3.0,5.0);
    //glVertex3f(20.0,3.0,4.0);
    //glEnd();

    Mat4f MV = cur_Camera->getViewMatrix();

    //glLoadMatrixf(MV.ToArray());

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    //gluLookAt(cur_Camera->getPosx(), cur_Camera->getPosy(), cur_Camera->getPosz(),
            //cur_Camera->getPosx() + cur_Camera->getDircetx(), cur_Camera->getPosy() + cur_Camera->getDircety(), cur_Camera->getPosz() + cur_Camera->getDircetz(),
            //cur_Camera->getUpVectorx(), cur_Camera->getUpVectory(), cur_Camera->getUpVectorz());

//    glUseProgram(0);
    //glBegin(GL_TRIANGLES);
    //glColor3f(0.2,0.7,0.9);
    //glVertex3f(10.0,6.0,3.0);
    //glVertex3f(10.0,7.0,3.0);
    //glVertex3f(20.0,15.0,3.0);
    //glEnd();
    glUseProgram(ProgramID);
//    std::cout<<"Here!"<<std::endl;

    glBegin(GL_QUADS);
    glColor3f(1.0,0.1,0.1);
    glVertex3f(-13.0,-13.0,0.0);

    glColor3f(0.2,0.2,0.9);
    glVertex3f(13.0,-13.0,0.0);

    glColor3f(0.2,0.7,0.1);
    glVertex3f(13.0,13.0,0.0);

    glColor3f(0.1,0.7,0.9);
    glVertex3f(-13.0,13.0,0.0);
    glEnd();
    model.Reset();
    model.setPovitToBottom();
    //model.Translate(Vec3f(3.0,1.0,1.0),TRANS_SELF);
    //model.Forward(F_NO_DRAW);
    //model.ClassicDraw(SMOOTH,UV_RAW);
     L_system.PickModel(&model);  //pick model
     //glUseProgram(0);
     glUseProgram(ProgramID);
    //model.Forward(F_DRAW);
    //model.Forward(F_DRAW);
    //model.VBODraw();
    //model.Forward(F_DRAW);
    model.ClassicDraw(SMOOTH,UV_RAW);
    //model.Rotation(20.0, Vec3f(0.0, 0.0, 1.0), TRANS_PIVOT);
    //model.ClassicDraw(SMOOTH,UV_RAW);
    //model.Rotation(60.0, Vec3f(0.0, 0.0, 1.0), TRANS_WORLD);
    //model.ClassicDraw(SMOOTH,UV_RAW);
    glColor3f(1.0,0.0,0.0);
    //model.Forward(F_DRAW);
    model.Rotation(60.0, Vec3f(0.0, 0.0, 1.0), TRANS_PIVOT);
    model.ClassicDraw(SMOOTH,UV_RAW);
    //model.ClassicDraw(SMOOTH,UV_RAW);

    //l-system
     L_system.interpretation();
    //
   //swapBuffers();
}
void DisplayWidget::resizeGL(int width, int height){
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,1.0);
}

void DisplayWidget::changeAxiom(QString SAxiom) //change axiom
{
    L_system.setaxiom(SAxiom.toStdString());
}
void DisplayWidget::changeIteration(int iteration) //change iteration
{
    L_system.setIterationLevel(iteration);
}

void DisplayWidget::ChangeTemplate(QString TemplateName){
    if(TemplateName=="Example1"){
        L_system.reset();
        L_system.CreateFromFile((QCoreApplication::applicationDirPath()+"/resources/LsystemRules/StochasticsStandard1.txt").toStdString().c_str());
        std::cerr<<"changed!";
    }
    else if (TemplateName=="Example2") {
        L_system.reset();
        L_system.CreateFromFile((QCoreApplication::applicationDirPath()+"/resources/LsystemRules/StochasticsStandard2.txt").toStdString().c_str());
        std::cerr<<"changed!";
    }else if (TemplateName=="Example3") {
        L_system.reset();
        L_system.CreateFromFile((QCoreApplication::applicationDirPath()+"/resources/LsystemRules/StochasticsStandard3.txt").toStdString().c_str());
        std::cerr<<"changed!";
    }
    else if(TemplateName=="(none)"){
        L_system.reset();
    }
//    else if (TemplateName=="Example4") {
//        L_system.reset();
//        L_system.CreateFromFile((QCoreApplication::applicationDirPath()+"/resources/LsystemRules/Standard3.txt").toStdString().c_str());
//        std::cerr<<"changed!";
//    }

    //emit signals
    QString QSAxiom=QString::fromStdString(L_system.getAxiom());
    emit axiomRead(QSAxiom);
    QString QSAngle=QString::number(L_system.getAngle());
    emit angleRead(QSAngle);

    emit RuleRead(L_system.getRules());

    //emit iteration
    //QString QSIteration=QString::number(L_system.getIteration());
    emit iterationRead(L_system.getIteration());

    emit RuleCalculated(L_system.getFinal());

    update();
}

void DisplayWidget::getChangedColor(QColor newcolor){
    int r,g,b;
    newcolor.getRgb(&r,&g,&b);
    Vec3f color=Vec3f((float)r/255,(float)g/255,(float)b/255);
    smaterial->setColor(color);
    GLuint ColorID = glGetUniformLocation(ProgramID, "color");
    glUniform3fv(ColorID, 1, color.getPtr());

}

void DisplayWidget::keyPressEvent(QKeyEvent* e){
//    switch(e->key){
//    case Qt::Key_W:cur_Camera->Translate(Vec3f(0.0,0.0,0.3));
//        break;
//    case Qt::Key_S:cur_Camera->Translate(Vec3f(0.0,0.0,-0.3));
//    }
    if(e->key()==Qt::Key_W){
        cur_Camera->Translate(Vec3f(0.0,0.0,10));
//        std::cerr<<"Move!"<<std::endl;
        update();
    }
    else if(e->key()==Qt::Key_S){
        cur_Camera->Translate(Vec3f(0.0,0.0,-10));
        update();
    }
    else if(e->key()==Qt::Key_Right){
        cur_Camera->Translate(Vec3f(10.0,0.0,0.0));
        update();
    }
    else if(e->key()==Qt::Key_Left){
        cur_Camera->Translate(Vec3f(-10.0,0.0,0.0));
        update();
    }
    else if(e->key()==Qt::Key_Up){
        cur_Camera->Translate(Vec3f(0.0,10.0,0.0));
        update();
    }
    else if(e->key()==Qt::Key_Down){
        cur_Camera->Translate(Vec3f(0.0,-10.0,0.0));
        update();
    }
}

GLuint DisplayWidget::LoadShaders(const char * vertex_file_path, const char * fragment_file_path){
    //create shader
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open())
    {
        std::string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open()){
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

    // Link the program
    fprintf(stdout, "Linking program\n");
    std::cout<<"Linking program\n";
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage(std::max(InfoLogLength, int(1)));
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
//    GLint success = 0;
//    /*glGetShaderiv*/(ProgramID, GL_COMPILE_STATUS, &success);

    return ProgramID;
}
