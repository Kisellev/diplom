#ifndef GENERATOR_H
#define GENERATOR_H

#include "myvector.h"
#include <time.h>
#include <QStringList>
#include <random>


class Generator
{

public:
    Generator();
    myVector generate();
    int GetNbGenerated();
    QStringList getInfo();
    void SetNbGenerated(int aValue);

private:
    int NbGenetated; 
    default_random_engine generator;
};

#endif // GENERATOR_H
