#include "graph.h"

Graph::Graph(QWidget *parent)
    : QWidget(parent)
{
    aPen = new QPen(Qt::black);

    for(int i = 0; i < 1000; i++){
        Values_Golay.push_back(0);
        NumberOfEachSnr_Golay.push_back(0);

        Values_Conv.push_back(0);
        NumberOfEachSnr_Conv.push_back(0);
    }

    this->setWindowTitle("График");
    myLayout = new QVBoxLayout;

    scene = new QGraphicsScene();
    scene->setSceneRect(QRectF(0, 0, 1000, 500));

    view = new QGraphicsView(scene);
    myLayout->addWidget(view);

    QHBoxLayout *myLayout_2 = new QHBoxLayout();
    Legend = new QLabel();
    myLayout_2->addWidget(Legend);

    spin_proc = new QSpinBox();
    connect(spin_proc,SIGNAL(valueChanged(int)),this,SLOT(RedrawPoints(int)));
    spin_proc->setMinimum(1);
    spin_proc->setMaximum(100);
    spin_proc->setValue(100);
    spin_proc->setSuffix("%");
    myLayout_2->addWidget(spin_proc);

    myLayout->addLayout(myLayout_2);

    text = new QTextEdit();
    myLayout->addWidget(text);

    this->setLayout(myLayout);

    this->prepare();


}

void Graph::prepare(){
    for(int i = 0; i <= 10; i++){
        scene->addLine(QLineF(QPoint(100*i,scene->height()),QPoint(100*i,0)));
        scene->addLine(QLineF(QPoint(0,50*i),QPoint(scene->width(),50*i)));
    }
    Legend->setText("График отношения процента ошибок декодирования к соотношению (сигнал/шум), синие точки - код Голея, красные - Свёрточный код. Масштаб по оси Y: ");
}
void Graph::RedrawPoints(int proc){
    scene->clear();
    this->prepare();
    QLabel asd;
    for(int i = 0; i < 1000; i++){
        if(NumberOfEachSnr_Golay[i] != 0){
            scene->addRect(i,scene->height()-((double)Values_Golay[i]/(double)NumberOfEachSnr_Golay[i])*(100.0/(double)proc)*scene->height()/100.0,3.0,3.0,QPen(Qt::blue));
        }
        if(NumberOfEachSnr_Conv[i] != 0){
            scene->addRect(i,scene->height()-((double)Values_Conv[i]/(double)NumberOfEachSnr_Conv[i])*(100.0/(double)proc)*scene->height()/100.0,3.0,3.0,QPen(Qt::red));
        }
    }
}

//   (int aType, double aSnr), int nbError, int indexes, int generated
void Graph::RedrawGraph(vector<double> TypeSnrVector,vector<int> nbErrorVector,myVector aGeneratedVector){
    //**********     TEXT FORMATTING       ********************************************
    QString temp_output_string;
    temp_output_string += this->WhatIs(TypeSnrVector.at(0));
    temp_output_string += ", noise = " + QString::number(TypeSnrVector.at(1));
    temp_output_string += ", nbErrors = " + QString::number(nbErrorVector.at(0));
    temp_output_string += ", result = ";
    int curErrIndex = 1;
    for(int i = 0; i < aGeneratedVector.size(); i++){
        if(curErrIndex < nbErrorVector.size() && i == nbErrorVector.at(curErrIndex)){
            temp_output_string += "<font color=\"red\">" + QString::number(aGeneratedVector.at(i)) + "</font>";
            curErrIndex++;
        } else {
            temp_output_string += QString::number(aGeneratedVector.at(i));
        }
    }
    text->append(temp_output_string);
    //*****************************************
    //add point
    //    TypeSnrVector.at(1)   = snr
    //    nbErrorVector.at(0) = nberrors
    //    TypeSnrVector.at(0) = type(Golay = 6 / Conv = 3)
    //    Values_Conv
    //    NumberOfEachSnr_Conv
    //
    //    Values_Golay
    //    NumberOfEachSnr_Golay

    switch((int)TypeSnrVector.at(0)){
    case 3:
        Values_Conv[(int)(TypeSnrVector.at(1)*100)] += ((double)nbErrorVector.at(0)/(double)aGeneratedVector.size())*100;
        NumberOfEachSnr_Conv[(int)(TypeSnrVector.at(1)*100)] += 1;
        break;
    case 6:
        Values_Golay[(int)(TypeSnrVector.at(1)*100)] += ((double)nbErrorVector.at(0)/(double)aGeneratedVector.size())*100;
        NumberOfEachSnr_Golay[(int)(TypeSnrVector.at(1)*100)] += 1;
        break;
    default:
        break;
    }

    //*************** DRAW POINTS ***************************************
    this->RedrawPoints(spin_proc->value());
}


QString Graph::WhatIs(int aType){
    QString result;
    switch (aType)
    {
    case 0:
        result = "bpsk_modulator";
        break;
    case 1:
        result = "bpsk_demodulator";
        break;
    case 2:
        result = "conv_coder";
        break;
    case 3:
        result = "conv_decoder";
        break;
    case 4:
        result = "generator";
        break;
    case 5:
        result = "golay_coder";
        break;
    case 6:
        result = "golay_decoder";
        break;
    case 7:
        result = "noise";
        break;
    case 8:
        result = "receiver";
        break;
    default:
        break;
    }
    return result;
}
