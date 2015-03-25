#ifndef TCONTAINER_H
#define TCONTAINER_H

#include <QWidget>
#include <QMouseEvent>
#include <QtGui>
#include <QVBoxLayout>
#include <QApplication>
#include <QLineEdit>
#include <QObject>
#include <QGroupBox>
#include <QSpinBox>
#include <QDialog>
#include <QLabel>

#include "generator.h"
#include "golay24.h"
#include "bpsk.h"
#include "noise.h"
#include "convolutional.h"
#include "myVector.h"

#include "receiver.h"

#include "sett.h"

enum modes{
    IDLE = 0,
    MOVE = 1,
};

enum types{
    BPSK_M =  0,
    BPSK_D =  1,
    CONV_C =  2,
    CONV_D =  3,
    GEN =     4,
    GOLAY_C = 5,
    GOLAY_D = 6,
    NOISE =   7,
    RES = 8,
};

class TContainer : public QWidget {
    Q_OBJECT
public:
    TContainer(QWidget *parent, QPoint p, QWidget *cWidget = 0);
    ~TContainer();

    void addChildWidget(QWidget *cWidget);

    void setIndex(int anIndex);
    int getIndex();
    QString WhatIs(int aType);
    void setType(int aType);
    int getType();

    void setView();

    void ChangeSett();

private:
    QWidget *childWidget;
    QWidget *myParent;

    Sett *settings;

    int index;
    int type;

private:
    bpsk abpsk;
    Generator aGenerator;
    golay24 aGolay;
    convolutional aConvolutional;
    Noise aNoise;
    Receiver aReceiver;

    QListWidget *myInfo;
    bool left_button;


public slots:
    void input(myVector aVector);
    void input(vector<double> aVector);
    void Start();
    void updateInfo();
    void SaveGeneratorValue(int);
    void SaveNoiseValue(double,double);
    void SaveNoiseValue(double);
    void SaveConvValue(int,int,int,QStringList);

    void get_noise(double);
    void get_generated(myVector);
    void get_decoded(myVector,int);


signals:
    void newGeometry(QRect rect);
    void del_item(int anIndex, int Type);
    void output(myVector aVector);
    void output(vector<double> aVector);
    void puts(QString);
    void posChanged();

    void SendGraphResult(vector<double>,vector<int>,myVector);

    void send_noise(double);
    void send_generated(myVector);
    void send_decoded(myVector,int);

protected:
    int mode;
    QPoint position;
    QVBoxLayout* vLayout;
    bool eventFilter( QObject *obj, QEvent *evt );
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
};

#endif // TCONTAINER_H
