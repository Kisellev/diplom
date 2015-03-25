#ifndef SETT_H
#define SETT_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QLayoutItem>
#include <QTextEdit>
#include <QTextBlock>

#include "generator.h"
#include "golay24.h"
#include "bpsk.h"
#include "noise.h"
#include "convolutional.h"
#include "myVector.h"

//class TContainer;

class Sett : public QDialog
{
    Q_OBJECT
public:
    Sett(QWidget *parent = 0, Qt::WindowFlags type = 0);
    ~Sett();
    void prepare(Noise anItem, int anIndex);
    void prepare(convolutional anItem, int aType, int anIndex);
    void prepare(Generator anItem, int anIndex);

    void prepare();

    void updateGEN(Generator anItem);
    void updateNOISE(Noise anItem);
    void updateCONV(convolutional anItem);



public slots:
    void myClose();
    void SaveGenerator();
    void SaveNoise();
    void SaveConv();
    void EmitRepeator();
signals:
    void StartRepeator(double,double,double,int);
    void SaveGeneratorValue(int);
    void SaveNoiseValue(double,double);
    void SaveConvValue(int,int,int,QStringList g2);


private:
    QGridLayout *myLayout;

};

#endif
