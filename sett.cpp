#include "sett.h"

Sett::Sett(QWidget *parent, Qt::WindowFlags type)
    : QDialog(parent, type)
{
    this->setModal(true);
    myLayout = new QGridLayout(this);
}

Sett::~Sett()
{
    delete myLayout;
}

void Sett::myClose(){
    this->close();
}

void Sett::SaveGenerator(){
    QSpinBox *spin1 = qobject_cast<QSpinBox*>(myLayout->itemAtPosition(0,1)->widget());
    emit SaveGeneratorValue(spin1->value());
}

void Sett::SaveConv(){
    QSpinBox *spin_n2 = qobject_cast<QSpinBox*>(myLayout->itemAtPosition(0,1)->widget());
    QSpinBox *spin_m2 = qobject_cast<QSpinBox*>(myLayout->itemAtPosition(1,1)->widget());
    QSpinBox *spin_length = qobject_cast<QSpinBox*>(myLayout->itemAtPosition(2,1)->widget());
    QTextEdit *polynoms = qobject_cast<QTextEdit*>(myLayout->itemAtPosition(4,0)->widget());
    QStringList myQStringList;
    for(int loc_i = 0; loc_i < spin_n2->value(); loc_i++){
        QString check_str = polynoms->document()->findBlockByLineNumber(loc_i).text();
        myQStringList.append(check_str);
    }

    emit SaveConvValue(spin_n2->value(),spin_m2->value(),spin_length->value(),myQStringList);
}
void Sett::updateNOISE(Noise anItem){
    QDoubleSpinBox *spin1 = qobject_cast<QDoubleSpinBox*>(myLayout->itemAtPosition(0,1)->widget());
    QDoubleSpinBox *spin2 = qobject_cast<QDoubleSpinBox*>(myLayout->itemAtPosition(1,1)->widget());

    spin1->setValue(anItem.get_rate());
    spin2->setValue(anItem.get_snr());
    this->show();
}

void Sett::SaveNoise(){
    QDoubleSpinBox *spin1 = qobject_cast<QDoubleSpinBox*>(myLayout->itemAtPosition(0,1)->widget());
    QDoubleSpinBox *spin2 = qobject_cast<QDoubleSpinBox*>(myLayout->itemAtPosition(1,1)->widget());

    emit SaveNoiseValue(spin1->value(),spin2->value());
}


void Sett::EmitRepeator(){
    QDoubleSpinBox *spin1 = qobject_cast<QDoubleSpinBox*>(myLayout->itemAtPosition(0,1)->widget());
    QDoubleSpinBox *spin2 = qobject_cast<QDoubleSpinBox*>(myLayout->itemAtPosition(1,1)->widget());
    QDoubleSpinBox *spin3 = qobject_cast<QDoubleSpinBox*>(myLayout->itemAtPosition(2,1)->widget());
    QSpinBox *spin4 = qobject_cast<QSpinBox*>(myLayout->itemAtPosition(3,1)->widget());
    emit StartRepeator(spin1->value(),spin2->value(),spin3->value(),spin4->value());
}

void Sett::prepare(){
    if(!myLayout->isEmpty()){
        this->show();
        return;
    }
    this->setWindowTitle("Запустить N раз");
    QLabel *label1 = new QLabel("Начальное snr:");
    myLayout->addWidget(label1,0,0,Qt::AlignLeft);

    QDoubleSpinBox *spin1 = new QDoubleSpinBox();
    spin1->setMinimum(0.01);
    spin1->setValue(0.01);
    spin1->setSingleStep(0.1);
    myLayout->addWidget(spin1,0,1,Qt::AlignRight);
    //*********************************************
    QLabel *label2 = new QLabel("Конечное snr:");
    myLayout->addWidget(label2,1,0,Qt::AlignLeft);

    QDoubleSpinBox *spin2 = new QDoubleSpinBox();
    spin2->setMinimum(0.01);
    spin2->setMaximum(10.00);
    spin1->setValue(10.00);
    spin2->setSingleStep(0.1);
    myLayout->addWidget(spin2,1,1,Qt::AlignRight);
    //*********************************************
    QLabel *label3 = new QLabel("Шаг snr:");
    myLayout->addWidget(label3,2,0,Qt::AlignLeft);

    QDoubleSpinBox *spin3 = new QDoubleSpinBox();
    spin3->setMinimum(0.01);
    spin3->setValue(0.2);
    spin3->setSingleStep(0.1);
    myLayout->addWidget(spin3,2,1,Qt::AlignRight);
    //*********************************************
    QLabel *label4 = new QLabel("Количество прогонов N:");
    myLayout->addWidget(label4,3,0,Qt::AlignLeft);

    QSpinBox *spin4 = new QSpinBox();
    spin4->setValue(5);
    myLayout->addWidget(spin4,3,1,Qt::AlignRight);
    //*********************************************
    QPushButton *btn1 = new QPushButton("Start");
    myLayout->addWidget(btn1,4,0,Qt::AlignLeft);
    //*********************************************
    QPushButton *btn2 = new QPushButton("Cancel");
    myLayout->addWidget(btn2,4,1,Qt::AlignRight);
    connect(btn2,SIGNAL(clicked()),this,SLOT(myClose()));
    connect(btn1,SIGNAL(clicked()),this,SLOT(myClose()));
    connect(btn1,SIGNAL(clicked()),this,SLOT(EmitRepeator()));
    this->show();
}

void Sett::prepare(Noise anItem, int anIndex){
    if(!myLayout->isEmpty()){
        updateNOISE(anItem);
        return;
    }
    this->setWindowTitle("Settings of Noise generator " + QString::number(anIndex+1));
    //*********************************************
    QLabel *label1 = new QLabel("Code rate: ");
    myLayout->addWidget(label1,0,0,Qt::AlignLeft);

    QDoubleSpinBox *spin1 = new QDoubleSpinBox();
    spin1->setValue(anItem.get_rate());
    myLayout->addWidget(spin1,0,1,Qt::AlignRight);
    //*********************************************
    QLabel *label2 = new QLabel("snr (Code/Noise): ");
    myLayout->addWidget(label2,1,0,Qt::AlignLeft);

    QDoubleSpinBox *spin2 = new QDoubleSpinBox();
    spin2->setValue(anItem.get_snr());
    myLayout->addWidget(spin2,1,1,Qt::AlignRight);
    //*********************************************
    QPushButton *btn1 = new QPushButton("Save");
    myLayout->addWidget(btn1,2,0,Qt::AlignLeft);
    //*********************************************
    QPushButton *btn2 = new QPushButton("Cancel");
    myLayout->addWidget(btn2,2,1,Qt::AlignRight);

    connect(btn2,SIGNAL(clicked()),this,SLOT(myClose()));
    connect(btn1,SIGNAL(clicked()),this,SLOT(myClose()));
    connect(btn1,SIGNAL(clicked()),this,SLOT(SaveNoise()));
    this->show();
}

void Sett::updateCONV(convolutional anItem){
    QSpinBox *spin_n2 = qobject_cast<QSpinBox*>(myLayout->itemAtPosition(0,1)->widget());
    QSpinBox *spin_m2 = qobject_cast<QSpinBox*>(myLayout->itemAtPosition(1,1)->widget());
    QSpinBox *spin_length = qobject_cast<QSpinBox*>(myLayout->itemAtPosition(2,1)->widget());

    QTextEdit *polynoms = qobject_cast<QTextEdit*>(myLayout->itemAtPosition(4,0)->widget());

    spin_n2->setValue(anItem.get_n2());
    spin_m2->setValue(anItem.get_m2());
    spin_length->setValue(anItem.get_TRUNC_LENGTH());

    polynoms->clear();
    QStringList myQStringList = anItem.get_g2();
    for(int loc_i = 0; loc_i < spin_n2->value(); loc_i++){
        polynoms->append(myQStringList.at(loc_i));
    }
    this->show();
}


void Sett::prepare(convolutional anItem, int aType, int anIndex){
    if(!myLayout->isEmpty()){
        updateCONV(anItem);
        return;
    }
    switch(aType){
    case 2:
        this->setWindowTitle("Settings of Conv coder " + QString::number(anIndex+1));
        break;
    case 3:
        this->setWindowTitle("Settings of Conv decoder " + QString::number(anIndex+1));
        break;
    default:
        QMessageBox msgbox;
        msgbox.setText("Debug Error : some wrong index in Sett::prepare(convolutional anItem, int aType, int anIndex)");
        msgbox.show();
        break;
    }
    //***********************  N2  **********************************************************
    QLabel *label2 = new QLabel("Number of output bites n2: ");
    myLayout->addWidget(label2,0,0,Qt::AlignLeft);

    QSpinBox *spin2 = new QSpinBox();
    spin2->setMinimum(0);
    spin2->setMaximum(10000);
    spin2->setValue(anItem.get_n2());
    myLayout->addWidget(spin2,0,1,Qt::AlignRight);
//    //***********************   M2  *********************************************************
    QLabel *label3 = new QLabel("memory m2: ");
    myLayout->addWidget(label3,1,0,Qt::AlignLeft);

    QSpinBox *spin3 = new QSpinBox();
    spin3->setMinimum(0);
    spin3->setMaximum(10000);
    spin3->setValue(anItem.get_m2());
    myLayout->addWidget(spin3,1,1,Qt::AlignRight);
//    //************************  TRUNC_LENGTH  *********************************************************
    QLabel *label4 = new QLabel("trunc length (for decoder): ");
    myLayout->addWidget(label4,2,0,Qt::AlignLeft);

    QSpinBox *spin4 = new QSpinBox();
    spin4->setMinimum(0);
    spin4->setMaximum(10000);
    spin4->setValue(anItem.get_TRUNC_LENGTH());
    myLayout->addWidget(spin4,2,1,Qt::AlignRight);
    //*********************************************************************************

    QLabel *alist_title = new QLabel("Polynoms in octal form(number n2):");
    myLayout->addWidget(alist_title,3,0,Qt::AlignLeft);

    QTextEdit *alist = new QTextEdit();
    QStringList myQStringList = anItem.get_g2();
    for(int loc_i = 0; loc_i < myQStringList.size(); loc_i++){
        alist->append(myQStringList.at(loc_i));
    }
    myLayout->addWidget(alist,4,0,Qt::AlignLeft);

    //*********************************************************************************
    QPushButton *btn1 = new QPushButton("Save");
    myLayout->addWidget(btn1,5,0,Qt::AlignLeft);

    QPushButton *btn2 = new QPushButton("Cancel");
    myLayout->addWidget(btn2,5,1,Qt::AlignRight);

    connect(btn2,SIGNAL(clicked()),this,SLOT(myClose()));
    connect(btn1,SIGNAL(clicked()),this,SLOT(myClose()));
    connect(btn1,SIGNAL(clicked()),this,SLOT(SaveConv()));


    this->show();
}

void Sett::updateGEN(Generator anItem){
    QSpinBox *spin1 = qobject_cast<QSpinBox*>(myLayout->itemAtPosition(0,1)->widget());
    spin1->setValue(anItem.GetNbGenerated());
    this->show();
}

void Sett::prepare(Generator anItem, int anIndex){
    if(!myLayout->isEmpty()){
        updateGEN(anItem);
        return;
    }
    this->setWindowTitle("Settings of Generator " + QString::number(anIndex+1));
    QLabel *label1 = new QLabel("Number of generated items: ");
    myLayout->addWidget(label1,0,0,Qt::AlignLeft);

    QSpinBox *spin1 = new QSpinBox();
    spin1->setMinimum(0);
    spin1->setMaximum(1000);
    spin1->setValue(anItem.GetNbGenerated());
    myLayout->addWidget(spin1,0,1,Qt::AlignRight);

    QPushButton *btn1 = new QPushButton("Save");
    myLayout->addWidget(btn1,1,0,Qt::AlignLeft);

    QPushButton *btn2 = new QPushButton("Cancel");
    myLayout->addWidget(btn2,1,1,Qt::AlignRight);

    connect(btn2,SIGNAL(clicked()),this,SLOT(myClose()));
    connect(btn1,SIGNAL(clicked()),this,SLOT(myClose()));
    connect(btn1,SIGNAL(clicked()),this,SLOT(SaveGenerator()));
    this->show();
}
