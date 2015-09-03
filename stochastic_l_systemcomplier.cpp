#include "stochastic_l_systemcomplier.h"

using namespace std;

Stochastic_l_systemcomplier::Stochastic_l_systemcomplier()
{

}

Stochastic_l_systemcomplier::~Stochastic_l_systemcomplier()
{

}

void Stochastic_l_systemcomplier::LoadFromFile(const char* filename){
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
    std::string delimiter = "->";
    std::string ruleItem="";
    std::string replaceItem="";
    float chances=0.0f;

    while (ifs.good())
    {
        ruleItem="";
        replaceItem="";
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
        std::istringstream stringreader(Line);
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
            else if (title == "rule")  //get rule
            {
                rules.push_back(content);
                 ruleItem = content.substr(0, content.find(delimiter));
                 replaceItem = content.substr(content.find(delimiter) + delimiter.size(), content.size() - 1);
                 //ReplaceRule[ruleItem] = replaceItem;
                 replaceTarget.push_back(ruleItem);
                 replaceItems.push_back(replaceItem);

                 //split rule
                 string schance = ruleItem.substr(ruleItem.find('(') + 1, ruleItem.find(')') - 2);
                 chances = stof(schance);
                 string newRule = ruleItem.substr(0, ruleItem.find('('));

                 if (stlStochasticMap.count(newRule) != 0){ //do exist
                    auto iter = stlStochasticMap.find(newRule);
                    chances += iter->second[iter->second.size() - 1].first;
                  //chances += iter.second[ruleItem][stlStochasticMap[ruleItem].size() - 1].first;
                 }

                 //add into multimap
                 std::pair<float, std::string> replace_pair;
                 replace_pair = std::make_pair(chances, replaceItem);
                 stlStochasticMap[newRule].push_back(replace_pair);
            }


        }
      }
    //delete [] Line;
    ifs.close();
}

void Stochastic_l_systemcomplier::replace(std::string &axiom,std::map<std::string,std::vector<std::pair<float,std::string>>> stlStochasticMap){
    std::string result="";
    float randomNum=0.0f;
    float random=1.2f;
    //traverse rules
        unsigned int iTraversedAxiom = 0;//substring head index
        bool ismatched = false;  //match on each index

        //random number parameters
        std::default_random_engine generator;
        std::uniform_real_distribution<float> dis(0, 1);
        time_t timer;

        while (iTraversedAxiom<axiom.size())
        {
            //unsigned int iTraversedlength = 0;
            ismatched = false;
            //unsigned int replaceIdx = 0;
            //match string
            for (auto &iter:stlStochasticMap)
            {
                //replace item longer than axiom
                if (iter.first.size()>axiom.size()-iTraversedAxiom)
                {
                    continue;
                }
                else{//matching substring
                    if (axiom.substr(iTraversedAxiom, iter.first.size()) == iter.first) //matched
                    {
                        //axiom.replace(axiom.begin() + iTraversedAxiom, axiom.begin() + iTraversedAxiom + iter.first.size(), iter.second);
                        //iTraversedAxiom += iter.second.size();
                        //stochastic
                        if(iter.second.size()==1){
                            result+=iter.second[0].second;
                        }
                        else{
                            time(&timer);
                            int divide=random*100;
                            generator.seed(timer%divide);
                            randomNum =dis(generator);
                            random+=randomNum/10;
                            if(randomNum>=iter.second[iter.second.size()-1].first){
                                result+=iter.second[iter.second.size()-1].second;
                            }
                            std::vector<std::pair<float,std::string>>::iterator it=std::find_if(iter.second.begin(),iter.second.end(),[randomNum](const std::pair<float,std::string> item){return item.first>=randomNum;});
                            if(it!=iter.second.end()){
                                if((result.size()+it->second.size())<=result.max_size()){
                                    result+=it->second;
                                }
                            }
                            else{
                                result+=iter.second[iter.second.size()-1].second;
                            }

                        }
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

}
//void Stochastic_l_systemcomplier::CreateFromFile(const char* filename){

//}

