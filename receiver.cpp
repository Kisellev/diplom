#include "receiver.h"

Receiver::Receiver()
{
    this->aNoiseSnr = -1.;
    this->myType = -1;
}

vector<int> Receiver::compare(){
    int nb = 0;
    vector<int> indexVector;
    for (int i = 0; i < generated.size(); i++){
        if(generated.at(i) != decoded.at(i)){
            indexVector.push_back(i);
            nb++;
        }
    }
    vector<int> aResult;
    aResult.push_back(nb);
    foreach(int aValue,indexVector){
        aResult.push_back(aValue);
    }
    return aResult;
}

bool Receiver::isReady(){
    if(aNoiseSnr >= 0 &&
       this->generated.size() != 0 &&
       this->decoded.size() != 0 ) {
        return true;
    } else {
        return false;
    }
}

void Receiver::SetResult(int aType, double aSnr, vector<int> nbError){
    myResultTypeSnr.push_back(aType);
    myResultTypeSnr.push_back(aSnr);
    foreach(int aValue,nbError){
        myResultnbError.push_back(aValue);
    }
    this->eraseAll();
}

myVector Receiver::GetGenerated(){
    myVector temp = this->generated;
    this->generated.clear();
    return temp;
}

vector<double> Receiver::GetResultTypeSnr(){
    vector<double> temp = myResultTypeSnr;
    myResultTypeSnr.clear();
    return temp;
}
vector<int> Receiver::GetResultnbError(){
    vector<int> temp = myResultnbError;
    myResultnbError.clear();
    return temp;
}

bool Receiver::set_noise(double aSnr){
    aNoiseSnr = aSnr;
    return this->check();
}

bool Receiver::AddGenerated(myVector aVector){
    this->generated = aVector;
    return this->check();
}

bool Receiver::AddDecoded(myVector aVector,int aType){
    this->decoded = aVector;
    this->myType = aType;
    return this->check();
}

void Receiver::eraseAll(){
    this->decoded.clear();
    this->aNoiseSnr = -1.0;
    this->myType = -1;
}

bool Receiver::check(){
    if(this->isReady()){
        this->SetResult(this->myType,this->aNoiseSnr,compare());
        return true;
    }
    return false;
}
