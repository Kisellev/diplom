#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <vector>
#include <QString>

using namespace std;

class myVector : public vector<int>
{
public:
    explicit myVector();
    myVector(myVector a,int begin,int end);
    myVector(int aSize,int aValue);
    void Reverse();
    void Dec2Bin(int aNumber);
    int Bin2Dec();
    QString toString();
    int weight();
};

#endif // MYVECTOR_H
