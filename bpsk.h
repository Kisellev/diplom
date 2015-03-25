#ifndef BPSK_H
#define BPSK_H

#include "myvector.h"

class bpsk
{
public:
    bpsk();
    myVector modulate(myVector a);
    myVector demodulate(vector<double> a);
};

#endif // BPSK_H
