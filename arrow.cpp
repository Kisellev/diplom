#include "arrow.h"

Arrow::Arrow()
{

}

void Arrow::SetStart(int aType,int anIndex){
    StartType = aType;
    StartIndex = anIndex;
}

void Arrow::SetEnd(int aType,int anIndex){
    EndType = aType;
    EndIndex = anIndex;
}

vector<int> Arrow::GetEnd(){
    vector<int> res;
    res.push_back(EndType);
    res.push_back(EndIndex);
    return res;
}

vector<int> Arrow::GetStart(){
    vector<int> res;
    res.push_back(StartType);
    res.push_back(StartIndex);
    return res;
}
