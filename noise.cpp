#include "noise.h"

Noise::Noise()
{
    this->set_rate(0.5);
    this->set_snr(7.5);
    this->calc_amp();
}
Noise::Noise(double aRate,double aSnr)
{
    this->set_rate(aRate);
    this->set_snr(aSnr);
    this->calc_amp();
}

void Noise::calc_amp()
{
    this->amp = sqrt(2.0*this->get_rate()*pow(10.0,(this->get_snr()/10.0)));
}

void Noise::set_rate(double aValue)
{
    this->rate = aValue;
    this->calc_amp();
}

void Noise::set_snr(double aValue)
{
    this->snr = aValue;
    this->calc_amp();
}

double Noise::get_rate()
{
    return this->rate;
}

double Noise::get_snr()
{
    return this->snr;
}
double Noise::get_amp()
{
    return this->amp;
}

vector<double> Noise::generate(myVector a)
{

    vector<double> aResult;

    default_random_engine generator;
    normal_distribution<double> distribution(0.0,1.0);

    double noise; // normal distr variable

    for (myVector::iterator it = a.begin(); it!=a.end(); ++it){
        noise = distribution(generator);
        aResult.push_back(*it + noise/this->get_amp());
    }
    return aResult;
}

myVector Noise::check_noise()
{
    const int nrolls=10000;  // number of experiments

    default_random_engine generator;
    normal_distribution<double> distribution(5.0,2.0);

    myVector p(10,0);

    for (int i=0; i<nrolls; ++i) {
        double number = distribution(generator);
        if ((number>=0.0)&&(number<10.0)){
            ++p.at(int(number));
        }
    }
    return p;
}

QStringList Noise::getInfo()
{
    QStringList result;
    result.append("Code rate " + QString::number(this->get_rate()));
    result.append("snr (Code/Noise)" + QString::number(this->get_snr()));
    return result;
}
