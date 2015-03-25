#ifndef RECEIVER_H
#define RECEIVER_H

#include "myvector.h"
#include <QString>
#include <QLabel>


class Receiver
{

public:
    Receiver();
    vector<int> compare();
    bool isReady();
    vector<double> GetResultTypeSnr();
    vector<int> GetResultnbError();
    myVector GetGenerated();

    bool set_noise(double aSnr);
    bool AddGenerated(myVector aVector);
    bool AddDecoded(myVector aVector,int myType);
    bool check();
    void eraseAll();
    void SetResult(int aType, double aSnr, vector<int> nbError);

private:
    myVector decoded;
    myVector generated;
    vector<double> myResultTypeSnr;
    vector<int> myResultnbError;
    int myType;
    double aNoiseSnr;
};

#endif // RECEIVER_H
