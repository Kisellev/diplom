#include "generator.h"

Generator::Generator()
{
    this->SetNbGenerated(100);
}
#include <QLabel>;
myVector Generator::generate()
{
    normal_distribution<double> distribution(0.0,100.0);
    int addition_value = (int) distribution(generator);

    srand(time(NULL) + addition_value);
    myVector aResult;
    for(int i = 0; i < this->GetNbGenerated(); i++){
        aResult.push_back((rand())%2);
    }
    return aResult;
}

void Generator::SetNbGenerated(int aValue)
{

    NbGenetated = aValue;
}

int Generator::GetNbGenerated()
{
    return this->NbGenetated;
}

QStringList Generator::getInfo()
{
    QStringList result;
    result.append("Generation array of " + QString::number(this->GetNbGenerated()) + " values");
    return result;
}
