#ifndef STOCHASTIC_L_SYSTEMCOMPLIER_H
#define STOCHASTIC_L_SYSTEMCOMPLIER_H
#include "stable.h"
#include <QMultiMap>
#include <QMap>
#include <QList>
#include <QVector>
#include "l_systemcompiler.h"


class Stochastic_l_systemcomplier : public L_systemCompiler
{
protected:

    std::map<std::string,std::vector<std::pair<float,std::string>>> stlStochasticMap;
public:
    Stochastic_l_systemcomplier();

    ~Stochastic_l_systemcomplier();

    void LoadFromFile(const char* filename);
    void replace(std::string &axiom,std::map<std::string,std::vector<std::pair<float,std::string>>> stlStochasticMap);
    void CreateFromFile(const char* filename){
        LoadFromFile(filename);
        result=axiom;
        for(unsigned int i=0;i<iterationLevel;i++){
            replace(result,stlStochasticMap);
        }
    }
    void Update(){
        result=axiom;
        for(unsigned int i=0;i<iterationLevel;i++){
            replace(result,stlStochasticMap);
        }
    }
};

#endif // STOCHASTIC_L_SYSTEMCOMPLIER_H
