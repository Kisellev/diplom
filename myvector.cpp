#include "myvector.h"
//**************************************************************************************
myVector::myVector()
{
}
//**************************************************************************************
myVector::myVector(myVector a,int begin,int end)
{
    for(int i = begin; i <= end; i++){
        this->push_back(a.at(i));
    }
}
//**************************************************************************************
myVector::myVector(int aSize,int aValue)
{
    for(int i = 0; i < aSize; i++){
        this->push_back(aValue);
    }
}
//**************************************************************************************
QString myVector::toString()
{
    QString aResult ="";
    for(int i = 0; i < this->size(); i++){
        aResult.append(QString::number(this->at(i)));
        //aResult.append(" ");
    }
    return aResult;
}

//**************************************************************************************
void myVector::Reverse(){
    myVector temp;
    temp.swap(*this);
    for(int i = 0; i <= temp.size() - 1; i++){
        this->push_back(temp.at(temp.size() - 1 - i));
    }
}
//**************************************************************************************
void myVector::Dec2Bin(int aNumber){
    int aSize = (int) this->size();
    this->clear();
    for(int i = aSize - 1; i >= 0; --i){
        this->push_back((aNumber>>i) & 1);
    }
}
//**************************************************************************************
int myVector::Bin2Dec()
{
    int result = 0;
    this->Reverse();
    for(int i = (int) this->size() - 1; i >=0; i--){
        result += this->at(i)<<i;
    }
    this->Reverse();
    return result;
}
//**************************************************************************************
int myVector::weight(){
    int aux = 0;
    for (int i=0; i<this->size(); i++){
        if ( (this->Bin2Dec()>>i)&1 ){
            aux++;
        }
    }
    return(aux);
}
