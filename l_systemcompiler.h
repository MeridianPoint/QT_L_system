//L-system.h
//author Qiushuo Wang
//Read rule file, iteratively modify string and interpretate the final string
/*Rules
 * F Move forward and draw a line.               --implemented
 * f Move forward without drawing a line.        --implemented
 * + Turn left.                                  --implemented
 * − Turn right.                                 --implemented
 * ∧ Pitch up.                                  --implemented
 * & Pitch down.                                 --implemented
 * \ Roll left.                                  --implemented
 * / Roll right.                                 --implemented
 * | Turn around.
 * $ Rotate the turtle to vertical
 * [ Start a branch.
 * ] Complete a branch.
 * { Start a polygon.
 * G Move forward and draw a line. Do not record a vertex.
 * . Record a vertex in the current polygon.
 * } Complete a polygon.
 * ∼ Incorporate a predefined surface
 * ! Decrement the diameter of segments.
 *  Increment the current color index.
 * % Cut off the remainder of the branch.
 */

#ifndef L_SYSTEMCOMPILER_H
#define L_SYSTEMCOMPILER_H
#include "stable.h"

#define FILE_DIRECT "..//resources//L-systemRules//"

enum default_files {BASIC,OTHER};

class L_systemCompiler
{
protected:
    std::string axiom; //axiom base of L-system
    int iterationLevel;
    float theta;
    std::vector<std::string> rules;
    //alternative
    std::vector<std::string> replaceTarget;
    std::vector<std::string> replaceItems;

    std::map<std::string,std::string> ReplaceRule;
    std::string result; //result of L-system

    ///////////////////////////////////////////////////////
    //geometry
    Model *cur_Model;
    Scene *L_systemScene;
    std::stack<Transformation> ModelStateStack;
public:
    L_systemCompiler();
    ~L_systemCompiler();

    //functions
    virtual void reset();

    virtual void LoadFromFile(const char* filename);  //load L-system rule from file
    void replace(std::string &axiom, std::map<std::string,std::string> replaceRule,unsigned int iteration); //replace axiom using rule
    void replace(std::string &axiom, std::vector<std::string> replacetargets,std::vector<std::string> replaceItems,unsigned int iteration); //replace axiom using rule
    void interpretation();

    //Inialization
    virtual void CreateFromFile(const char* filename);
    virtual void Update();

    //set get funcitons
    void setaxiom(std::string sAxiom){
        axiom=sAxiom;
    }
    void setIterationLevel(int Iteration){
        iterationLevel=Iteration;
    }
    void setAngle(float angle){
        theta=angle;
    }

    std::string getAxiom(){
        return axiom;
    }

    int getIteration(){
        return iterationLevel;
    }

    float getAngle(){
        return theta;
    }

    QString getRules(){
        if(rules.size()==1){
            return QString::fromStdString(rules[0]);
        }
        else{
            QString outputRule;
            for(unsigned int i=0;i<rules.size();i++){
                outputRule+=QString::fromStdString(rules[i]);
                if(i!=rules.size()-1){
                    outputRule+="\n";
                }
            }
            return outputRule;
        }
    }

    QString getFinal(){
        return QString::fromStdString(result);
    }

    void linkScene(Scene *pScene){
        L_systemScene=pScene;
    }

    void PickModel(Model* pModel){
        if(L_systemScene==NULL){
            //exception_code("no scene to intialize!");
            std::cerr<<"no scene to intialize!"<<std::endl;
        }
        cur_Model=pModel;
        if(L_systemScene!=nullptr){
            L_systemScene->addModel(pModel);
        }
    }

};

#endif // L_SYSTEMCOMPILER_H
