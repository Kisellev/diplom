#ifndef ARROW_H
#define ARROW_H

#include "tcontainer.h"

class Arrow
{
public:
    Arrow();

    void SetStart(int aType,int anIndex);
    void SetEnd(int aType,int anIndex);
    vector<int> GetEnd();
    vector<int> GetStart();
private:
    int StartType;
    int StartIndex;
    int EndType;
    int EndIndex;
};

#endif // ARROW_H
