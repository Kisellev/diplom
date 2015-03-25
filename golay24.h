#ifndef GOLAY24_H
#define GOLAY24_H

#include "myvector.h"

class golay24
{
public:
    golay24();
    myVector code(myVector a);
    myVector decode(myVector a);
    int weight(int intVector);
};

#endif // GOLAY24_H
