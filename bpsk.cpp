#include "bpsk.h"

bpsk::bpsk()
{
}

myVector bpsk::modulate(myVector a){
    for (myVector::iterator it = a.begin(); it!=a.end(); ++it){
        if(*it == 0){
            *it = 1;
        } else {
            *it = -1;
        }
    }
    return a;
}

myVector bpsk::demodulate(vector<double> a)
{
    myVector aResult;
    for (vector<double>::iterator it = a.begin(); it!=a.end(); ++it){
        if(*it >= 0){
            aResult.push_back(0);
        } else {
            aResult.push_back(1);
        }
    }
    return aResult;
}
