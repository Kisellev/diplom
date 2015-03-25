#ifndef NOISE_H
#define NOISE_H

#include "myvector.h"
#include <random>
#include <vector>
#include <QStringList>

using namespace std;

class Noise
{
public:
    Noise();
    Noise(double aRate,double aSnr);

    myVector check_noise();

    vector<double> generate(myVector a);

    void set_rate(double);
    void set_snr(double);

    double get_rate();
    double get_snr();

    double get_amp();

    void calc_amp();

    QStringList getInfo();

private:
    double rate;
    double snr;
    double amp;
};

#endif // NOISE_H
