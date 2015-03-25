#ifndef convolutional_H
#define convolutional_H

#include "myvector.h"

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>

#include <QListWidget>

#define MAX_RANDOM LONG_MAX

class surv
{
public:
    surv(){
        this->size = 1024;
        for(int i = 0; i < this->size; i++){
            this->data.push_back(0);
            this->state.push_back(0);
        }
    }
    double metric;
    vector<int> data;
    vector<int> state;
    int size;
};

class trel
{
public:
    trel(){
        this->size = 10;
        for(int i = 0; i < this->size;i++){
            this->output.push_back(0);
        }
    }
    int init;                /* initial state */
    int data;                /* data symbol */
    int final;               /* final state */
    vector<double> output;
    int size;
};


class convolutional
{
public:
    convolutional();
    int random_data();
    myVector code(myVector a);
    myVector decode(myVector a);
    double comp_metric(double rec, double ref);

    void trellis();
    vector<myVector> get_trellis();
    QStringList getInfo();

    long get_TRUNC_LENGTH();
    int get_k2();
    int get_n2();
    int get_m2();

    void set_TRUNC_LENGTH(long aValue);
    void set_k2(int aValue);
    void set_n2(int aValue);
    void set_m2(int aValue);

    void set_g2(QStringList);
    QStringList get_g2();


private:
    long TRUNC_LENGTH;
    int k2, n2, m2;
    int NUM_STATES, OUT_SYM, NUM_TRANS;
    int g2[100][100];
    unsigned int memory2, output, state;            /* Memory and output */
    unsigned int data2;                      /* Data */
    vector<myVector> aTrellis;
};

#endif // convolutional_H
