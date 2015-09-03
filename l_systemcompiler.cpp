#include "stable.h"
#include "l_systemcompiler.h"

using namespace std;

L_systemCompiler::L_systemCompiler()
{

}

L_systemCompiler::~L_systemCompiler()
{

}

void L_systemCompiler::LoadFromFile(const char* filename)  //load L-system rule from file
{
        std::ifstream ifs;

        ifs.open(filename, std::ifstream::in);
        //std::stringstream stringreader;

        if (ifs.bad())
        {
            std::cerr << "File Unavailable!" << std::endl;
        }

        if(!ifs.good()){
            std::cerr << "File do not exist!" << std::endl;
        }

        //parameters
        char Line[1024];
        std::string title;
        std::string content;
        string delimiter = "->";
        string ruleItem="";
        string replaceItem="";

        while (ifs.good())
        {
            ruleItem="";
            replaceItem="";
            content="";
            ifs.getline(Line, 1024);
            for (int i = 0; i < 1024; i++)
            {
                if (Line[i] == ':' || Line[i] == '#')
                    Line[i] = ' ';
            }
            std::cerr<<Line<<std::endl;
            //edbug
            std::string read(Line);
            std::cout<<read<<std::endl;
            istringstream stringreader(Line);
            if (/*stringreader.get() == '#'*/true) //syntax
            {
                /*int cur = stringreader.tellg();
                stringreader.seekg(cur - 1, stringreader.beg);*/
                //stringreader.getline(Line,1024,'#');

                stringreader >> title  >> content;
                //stringreader >> content;
                if (title == "axiom")
                {
                    axiom = content;
                }
                else if (title == "iteration")
                {
                    iterationLevel = atoi(content.c_str());
                }
                else if (title == "angle")
                {
                    theta = stof(content.c_str());
                }
                else if (title == "rule")
                {
                    rules.push_back(content);
                    ruleItem = content.substr(0, content.find(delimiter));
                    replaceItem = content.substr(content.find(delimiter)+delimiter.size(),content.size()-1);
                    ReplaceRule[ruleItem] = replaceItem;
                    replaceTarget.push_back(ruleItem);
                    replaceItems.push_back(replaceItem);
                }
            }

        }
        //delete [] Line;
        ifs.close();
}
void L_systemCompiler::replace(std::string &axiom, std::map<std::string,std::string> replaceRule,unsigned int iteration) //replace axiom using rule
{
    if(iteration==0){
        return;
    }
    std::string result="";
    //traverse rules
        unsigned int iTraversedAxiom = 0;//substring head index
        bool ismatched = false;  //match on each index
        while (iTraversedAxiom<axiom.size())
        {
            //unsigned int iTraversedlength = 0;
            ismatched = false;
            //unsigned int replaceIdx = 0;
            //match string
            for (auto &iter:replaceRule)
            {
                //replace item longer than axiom
                if (iter.first.size()>axiom.size()-iTraversedAxiom)
                {
                    continue;
                }
                else{//matching substring
                    if (axiom.substr(iTraversedAxiom, iTraversedAxiom + iter.first.size()) == iter.first) //matched
                    {
                        //axiom.replace(axiom.begin() + iTraversedAxiom, axiom.begin() + iTraversedAxiom + iter.first.size(), iter.second);
                        //iTraversedAxiom += iter.second.size();
                        result+=iter.second;
                        iTraversedAxiom++;
                        ismatched = true;
                        break;
                    }
                }
            }
            if (!ismatched) //no match started from iTraversedAxiom
            {
                result+=axiom[iTraversedAxiom];
                iTraversedAxiom++;
            }


        }
        axiom=result;
        if(iteration>0){
            replace(axiom,replaceRule,iteration-1);
        }
}

void L_systemCompiler::replace(std::string &axiom, std::vector<std::string> replacetargets,std::vector<std::string> replaceItems,unsigned int iteration){ //replace axiom using rul
    if(iteration==0){
        return;
    }
    std::string result="";
    //traverse rules
        unsigned int iTraversedAxiom = 0;//substring head index
        bool ismatched = false;  //match on each index
        while (iTraversedAxiom<axiom.size())
        {
            //unsigned int iTraversedlength = 0;
            ismatched = false;
            //unsigned int replaceIdx = 0;
            //match string
            for (unsigned int i=0;i<replacetargets.size();i++)
            {
                //replace item longer than axiom
                if (replacetargets[i].size()>axiom.size()-iTraversedAxiom)
                {
                    continue;
                }
                else{//matching substring
                    if (axiom.substr(iTraversedAxiom, replacetargets[i].size()) == replacetargets[i]) //matched
                    {
                        //axiom.replace(axiom.begin() + iTraversedAxiom, axiom.begin() + iTraversedAxiom + iter.first.size(), iter.second);
                        //iTraversedAxiom += iter.second.size();
                        result+=replaceItems[i];
                        iTraversedAxiom++;
                        ismatched = true;
                        break;
                    }
                }
            }
            if (!ismatched) //no match started from iTraversedAxiom
            {
                result+=axiom[iTraversedAxiom];
                iTraversedAxiom++;
            }


        }
        axiom=result;
        if(iteration>0){
            replace(axiom,replacetargets,replaceItems,iteration-1);
        }
}

void L_systemCompiler::interpretation(){
    int length = result.size();
        int iTraversed = 0; //index
        //Model *newModel;
        //iTraversed<length
        cur_Model->Reset();
        while (iTraversed<length)
        {
            switch (result[iTraversed])
            {
            case 'F'://forward
                //newModel = new Model(L_systemScene->LastModel());
                //newModel = (Model *)TransformationCache;
                cur_Model->Forward(F_DRAW);
                //L_systemScene->addModel(newModel);
                iTraversed++;
                break;
            case 'f': //forward no draw
                //
                cur_Model->Forward(F_NO_DRAW);
                break;
            case '|': //rotate 180
                //Model *newModel1 = new Model(L_systemScene->LastModel());
                //newModel1->Rotation(180.0, Vec3f(0.0, 0.0, 1.0), TRANS_SELF);
                cur_Model->Rotation(180.0, Vec3f(0.0, 0.0, 1.0), TRANS_PIVOT);

                //draw
                //cur_Model->VBODraw();
                cur_Model->ClassicDraw(SMOOTH,UV_RAW);
                //curTransformation = pModel;
//                L_systemScene->addModel(newModel1);
                iTraversed++;
                break;

            case '+': //rotate around U-axis of theta degree  u--up   -y -axis?
                //Model *newModel2 = new Model(L_systemScene->LastModel());
                //newModel2->Rotation(theta, Vec3f(0.0, 1.0, 0.0), TRANS_SELF);
                //curTransformation = pModel;
                cur_Model->Rotation(theta, Vec3f(0.0, 1.0, 0.0), TRANS_PIVOT);
                //cur_Model->VBODraw();
                cur_Model->ClassicDraw(SMOOTH,UV_RAW);
//                L_systemScene->addModel(newModel2);
                iTraversed++;
                break;

            case '-': //rotate around U-axis of -theta degree
                //newModel = new Model(L_systemScene->LastModel());
                //newModel->Rotation(theta, Vec3f(0.0, -1.0, 0.0), TRANS_SELF);
                //curTransformation = pModel;
                //L_systemScene->addModel(newModel);
                cur_Model->Rotation(theta, Vec3f(0.0, -1.0, 0.0), TRANS_PIVOT);
                //cur_Model->VBODraw();
                cur_Model->ClassicDraw(SMOOTH,UV_RAW);
                iTraversed++;
                break;

            case '<': //rotate around H-axis of theta degree   h-heading   z-axis?
                //newModel = new Model(L_systemScene->LastModel());
                //newModel->Rotation(theta, Vec3f(0.0, 0.0, 1.0), TRANS_SELF);
                //curTransformation = pModel;
                //L_systemScene->addModel(newModel);
                cur_Model->Rotation(theta, Vec3f(0.0, 0.0, 1.0), TRANS_PIVOT);
                //cur_Model->VBODraw();
                cur_Model->ClassicDraw(SMOOTH,UV_RAW);
                iTraversed++;
                break;

            case '>': //rotate around H-axis of -theta degree   h-heading   z-axis?
                //newModel = new Model(defualtScene->LastModel());
                //newModel->Rotation(theta, Vec3f(0.0, 0.0, -1.0), TRANS_SELF);
                //curTransformation = pModel;
                //L_systemScene->addModel(newModel);
                cur_Model->Rotation(theta, Vec3f(0.0, 0.0, -1.0), TRANS_PIVOT);
                //cur_Model->VBODraw();
                cur_Model->ClassicDraw(SMOOTH,UV_RAW);
                iTraversed++;
                break;

            case '&': //rotate around L-axis of theta degree    l-left  x-axis?
                //newModel = new Model(defualtScene->LastModel());
                //newModel->Rotation(theta, Vec3f(1.0, 0.0, 0.0), TRANS_SELF);
                //curTransformation = pModel;
                //L_systemScene->addModel(newModel);
                cur_Model->Rotation(theta, Vec3f(1.0, 0.0, 0.0), TRANS_PIVOT);
                //cur_Model->VBODraw();
                cur_Model->ClassicDraw(SMOOTH,UV_RAW);
                iTraversed++;
                break;

            case '/': //rotate around L-axis of -theta degree    l-left  x-axis?
                //newModel = new Model(defualtScene->LastModel());
                //newModel->Rotation(theta, Vec3f(-1.0, 0.0, 0.0), TRANS_SELF);
                //curTransformation = pModel;
                //L_systemScene->addModel(newModel);
                cur_Model->Rotation(theta, Vec3f(-1.0, 0.0, 0.0), TRANS_PIVOT);
                //cur_Model->VBODraw();
                cur_Model->ClassicDraw(SMOOTH,UV_RAW);
                iTraversed++;
                break;

            case '[':  //save status
                //ModelStateStack.push(defualtScene->LastModel());
                ModelStateStack.push(*cur_Model);
                iTraversed++;
                break;
            case ']':  //load status
                cur_Model->AlignTransformation(ModelStateStack.top());
                ModelStateStack.pop();
                iTraversed++;
                break;
            //
            default:
                if(result[iTraversed]>'A'&&result[iTraversed]<'z'){
                    iTraversed++;
                }
                else{
                    std::cerr << "Wrong Syntax!" << std::endl;
                    throw "Wrong Syntax!";
                }

                break;
            }
        }
}

void L_systemCompiler::CreateFromFile(const char* filename){
    LoadFromFile(filename);
    result=axiom;
    replace (result,replaceTarget,replaceItems,iterationLevel);
    //replace(result,ReplaceRule,iterationLevel);

}

void L_systemCompiler::Update(){
    result=axiom;
    replace (result,replaceTarget,replaceItems,iterationLevel);
    //replace(result,ReplaceRule,iterationLevel);
}

void L_systemCompiler::reset(){
    axiom="";
    iterationLevel=0;
    theta=0.0f;
    rules.clear();
    //alternative
    replaceTarget.clear();
    replaceItems.clear();

    ReplaceRule.clear();
    result.clear(); //result of L-system
}
