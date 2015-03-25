#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->rigth_button = false;
    myGraph = new Graph();
    myRepeater = new Sett();
    connect(myRepeater,SIGNAL(StartRepeator(double,double,double,int)),this,SLOT(StartRepeater(double,double,double,int)));

    //    BPSK_M =  0,
    //    BPSK_D =  1,
    //    CONV_C =  2,
    //    CONV_D =  3,
    //    GEN =     4,
    //    GOLAY_C = 5,
    //    GOLAY_D = 6,
    //    NOISE =   7,

    parts.append(bpsk_modulators);
    parts.append(bpsk_demodulators);
    parts.append(conv_coders);
    parts.append(conv_decoder);
    parts.append(generators);
    parts.append(golay_coders);
    parts.append(golay_decoders);
    parts.append(noises);
    parts.append(receivers);

    myStart = QPoint(0,0);
    myEnd = QPoint(0,0);
    myCurr = QPoint(0,0);
    myPrev = QPoint(0,0);

    logs = new QTabWidget();

    QWidget *tempo1 = new QWidget();
    QVBoxLayout *tayout1 = new QVBoxLayout(tempo1);
    tayout1->addWidget(&check_list);
    logs->addTab(tempo1,"Созданные объекты");

    QWidget *tempo2 = new QWidget();
    QVBoxLayout *tayout2 = new QVBoxLayout(tempo2);
    tayout2->addWidget(&output_list);
    logs->addTab(tempo2,"Лог");

    QWidget *tempo3 = new QWidget();
    QVBoxLayout *tayout3 = new QVBoxLayout(tempo3);
    tayout3->addWidget(&arrows_list);
    logs->addTab(tempo3,"Стрелки");

    QWidget *tempo4 = new QWidget();
    QVBoxLayout *tayout4 = new QVBoxLayout(tempo4);
    tayout4->addWidget(&connect_list);
    logs->addTab(tempo4,"Соединения");

    logs->setWindowTitle("Логи");

    connect(this,SIGNAL(RedrawGraph(vector<double>,vector<int>,myVector)),myGraph,SLOT(RedrawGraph(vector<double>,vector<int>,myVector)));
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StartRepeater(double initSnr,double finalSnr,double stepSnr,int nbTimes){
    double currSnr = initSnr;
    for(int i = 0; i <= (abs(finalSnr - initSnr)/stepSnr); i++){
        for(int j = 0; j < parts[NOISE].size(); j++){
            emit SaveNoiseValue(currSnr);
        }

        for(int j = 0; j < nbTimes; j++){
            this->getStart();
        }
        if(finalSnr >= initSnr){
            currSnr += stepSnr;
        } else {
            currSnr -= stepSnr;
        }
    }
}

void MainWindow::Repeater(){
    myRepeater->prepare();
}

void MainWindow::showGraph(){
    myGraph->show();
}

void MainWindow::show(){
    QMainWindow::show();
    logs->show();
    this->repaint();
}

void MainWindow::save(){
    QFile file("log.txt");
    QTextStream stream;
    if(!file.isOpen()){
        file.open(QFile::WriteOnly);
        stream.setDevice(&file);
        for (int i = 0; i < (int) output_list.count(); i++){
            stream << output_list.takeItem(i)->text() << "\r\n";
        }
        file.close();
    }
}

void MainWindow::check(){
    this->check_list.clear();
    for (int i = 0; i < parts.size(); i++){
        this->check_list.addItem(parts[i][0]->WhatIs(i));
        this->check_list.addItem(QString::number(parts[i].size()));
    }
}

void MainWindow::puts(QString astr){
    output_list.addItem(astr);
}

void MainWindow::myClose(){
    this->logs->close();
    this->myGraph->close();
    emit close();
}

void MainWindow::getStart(){
    emit Start();
}

// nbError int
// TypeSnr double
void MainWindow::AddGraphResult(vector<double> TypeSnr,vector<int> nbError,myVector aGenerated){
    emit RedrawGraph(TypeSnr,nbError,aGenerated);
}

void MainWindow::add_part(){
    TContainer *cont = new TContainer(this,QPoint(20,20));
    connect(cont,SIGNAL(del_item(int,int)),this,SLOT(del_item(int,int)));
    connect(cont,SIGNAL(puts(QString)),this,SLOT(puts(QString)));
    connect(cont,SIGNAL(posChanged()),this,SLOT(posChanged()));
    connect(this,SIGNAL(SaveNoiseValue(double)),cont,SLOT(SaveNoiseValue(double)));

    connect(cont,SIGNAL(SendGraphResult(vector<double>,vector<int>,myVector)),this,SLOT(AddGraphResult(vector<double>,vector<int>,myVector)));
    cont->setIndex(parts[this->mode].size());
    cont->setType(this->mode);
    cont->setView();
    parts[this->mode].push_back(cont);
    if(this->mode == GEN){
        connect(this,SIGNAL(Start()),cont,SLOT(Start()));
    }
    emit check();
}


void MainWindow::add_res(){
    this->mode = RES;
    emit add_part();
}
void MainWindow::add_bpsk_m(){
    this->mode = BPSK_M;
    emit add_part();
}
void MainWindow::add_bpsk_d(){
    this->mode = BPSK_D;
    emit add_part();
}
void MainWindow::add_conv_c(){
    this->mode = CONV_C;
    emit add_part();
}
void MainWindow::add_conv_d(){
    this->mode = CONV_D;
    emit add_part();
}
void MainWindow::add_gen(){
    this->mode = GEN;
    emit add_part();
}
void MainWindow::add_golay_c(){
    this->mode = GOLAY_C;
    emit add_part();
}
void MainWindow::add_golay_d(){
    this->mode = GOLAY_D;
    emit add_part();
}
void MainWindow::add_noise(){
    this->mode = NOISE;
    emit add_part();
}
void MainWindow::del_item(int anIndex, int aType){
    emit puts("Trying to delete...");
    emit puts("Type of object is " + QString::number(aType));

    emit del_arrow(anIndex,aType);
    delete(parts[aType][anIndex]);
    parts[aType].removeAt(anIndex);
    for(int i = anIndex; i < parts[aType].size(); i++){
        parts[aType].at(i)->setIndex(i);
    }

    emit check();
}
void MainWindow::del_arrow(int anIndex, int aType){
    int aSize = this->arrows.size();
    for(int i = 0; i < aSize; i++){
        vector<int> aStart = arrows[i]->GetStart();
        vector<int> anEnd = arrows[i]->GetEnd();
        if(aStart[0] == aType && aStart[1] == anIndex){
            delete(arrows[i]);
            arrows.removeAt(i);
            aSize--;
            i--;
        }
        if(anEnd[0] == aType && anEnd[1] == anIndex){
            delete(arrows[i]);
            arrows.removeAt(i);
            aSize--;
            i--;
        }
    }
    this->repaint();
}
void MainWindow::eraseAll(){
    arrows.clear();
    for(int i = 0; i < this->arrows.size(); i++){
        arrows.removeAt(i);
    }
    for(int i = 0; i < parts.size(); i++){
        for(int j = 0; j < parts[i].size(); j++){
            TContainer *Temp = parts[i][j];
            Temp->deleteLater();
        }
        parts[i].clear();
    }
    this->repaint();
    emit check();
}
void MainWindow::setMode(int aMode){
    this->mode = aMode;
}
int MainWindow::getMode(){
    return this->mode;
}


//****************************************************************************************************

void MainWindow::mousePressEvent(QMouseEvent *mouseEvent)
{
    QMainWindow::mousePressEvent(mouseEvent);
    if (mouseEvent->button() == Qt::RightButton){
        rigth_button = true;
        this->myStart.setX(mouseEvent->x());
        this->myStart.setY(mouseEvent->y());
        emit puts("begin point" + QString::number(myStart.x()) + " " + QString::number(myStart.y()));
    } else {
        rigth_button = false;
    }
    this->repaint();
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::red);
    p.setFont(QFont("Arial",30));
    DrawArrows(&p);
}
void MainWindow::DrawArrows(QPainter *p){
    for(int i = 0; i < this->arrows.size(); i++){

        vector<int> aStart = arrows[i]->GetStart();
        vector<int> anEnd = arrows[i]->GetEnd();

        TContainer *aStartCont = parts[aStart[0]][aStart[1]];
        TContainer *anEndCont = parts[anEnd[0]][anEnd[1]];

        // центры геометрии
        int StartX = aStartCont->geometry().center().x();
        int StartY = aStartCont->geometry().center().y();

        int EndX = anEndCont->geometry().center().x();
        int EndY = anEndCont->geometry().center().y();
        //-------------------------------------------------------------------
        if(!(abs(StartX-EndX) > (anEndCont->width()/2) && abs(StartY-EndY) > (anEndCont->height()/2))){
            int local_EndX = 0;
            int local_EndY = 0;
            if(abs(StartX-EndX) > (aStartCont->width()/2 + anEndCont->width()/2)){
                int znakX = (EndX-StartX)/abs(EndX-StartX);
                p->drawPolyline(QPolygon()<<QPoint(StartX+znakX*aStartCont->width()/2,StartY)<<QPoint(EndX-znakX*anEndCont->width()/2,StartY));
                local_EndX = EndX-znakX*anEndCont->width()/2;
                local_EndY = StartY;

                QPoint Add1;
                QPoint Add2;
                Add1.setY(local_EndY-5);
                Add2.setY(local_EndY+5);
                if(StartX < EndX){
                    Add1.setX(local_EndX-15);
                    Add2.setX(local_EndX-15);
                } else {
                    Add1.setX(local_EndX+15);
                    Add2.setX(local_EndX+15);
                }
                p->drawPolyline(QPolygon()<<QPoint(local_EndX,local_EndY)<<Add1<<Add2<<QPoint(local_EndX,local_EndY));
            }

            if(abs(StartY-EndY) > (aStartCont->height()/2 + anEndCont->height()/2)){
                int znakY = (EndY-StartY)/abs(EndY-StartY);
                p->drawPolyline(QPolygon()<<QPoint(StartX,StartY+znakY*aStartCont->height()/2)<<QPoint(StartX,EndY-znakY*anEndCont->height()/2));
                local_EndX = StartX;
                local_EndY = EndY-znakY*anEndCont->height()/2;

                QPoint Add1;
                QPoint Add2;
                Add1.setX(local_EndX-5);
                Add2.setX(local_EndX+5);
                if(StartY < EndY){
                    Add1.setY(local_EndY-15);
                    Add2.setY(local_EndY-15);
                } else {
                    Add1.setY(local_EndY+15);
                    Add2.setY(local_EndY+15);
                }
                p->drawPolyline(QPolygon()<<QPoint(local_EndX,local_EndY)<<Add1<<Add2<<QPoint(local_EndX,local_EndY));
            }


        } else {

            int local_EndX = 0;
            int local_EndY = 0;
            if(abs(StartX-EndX) > (aStartCont->width()/2 + anEndCont->width()/2)){
                int znakX = (EndX-StartX)/abs(EndX-StartX);
                int znakY = (EndY-StartY)/abs(EndY-StartY);
                p->drawPolyline(QPolygon()<<QPoint(StartX,StartY+znakY*aStartCont->height()/2)<< QPoint(StartX,EndY)<<QPoint(EndX - znakX*anEndCont->width()/2 ,EndY));

                local_EndX = EndX - znakX*anEndCont->width()/2;
                local_EndY = EndY;

                QPoint Add1;
                QPoint Add2;
                Add1.setY(local_EndY-5);
                Add2.setY(local_EndY+5);
                if(StartX < EndX){
                    Add1.setX(local_EndX-15);
                    Add2.setX(local_EndX-15);
                } else {
                    Add1.setX(local_EndX+15);
                    Add2.setX(local_EndX+15);
                }
                p->drawPolyline(QPolygon()<<QPoint(local_EndX,local_EndY)<<Add1<<Add2<<QPoint(local_EndX,local_EndY));
            } else if(abs(StartY-EndY) > (aStartCont->height()/2 + anEndCont->height()/2)){
                int znakX = (EndX-StartX)/abs(EndX-StartX);
                int znakY = (EndY-StartY)/abs(EndY-StartY);
                p->drawPolyline(QPolygon()<<QPoint(StartX+znakX*aStartCont->width()/2,StartY)<< QPoint(EndX,StartY)<<QPoint(EndX,EndY - znakY*anEndCont->height()/2));

                local_EndX = EndX;
                local_EndY = EndY - znakY*anEndCont->height()/2;

                QPoint Add1;
                QPoint Add2;
                Add1.setX(local_EndX-5);
                Add2.setX(local_EndX+5);
                if(StartY < EndY){
                    Add1.setY(local_EndY-15);
                    Add2.setY(local_EndY-15);
                } else {
                    Add1.setY(local_EndY+15);
                    Add2.setY(local_EndY+15);
                }
                p->drawPolyline(QPolygon()<<QPoint(local_EndX,local_EndY)<<Add1<<Add2<<QPoint(local_EndX,local_EndY));
            }
        }
   }
}

void MainWindow::posChanged(){
    this->repaint();
}


void MainWindow::mouseMoveEvent(QMouseEvent *mouseEvent){
    QMainWindow::mouseMoveEvent(mouseEvent);
   this->repaint();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::RightButton){
        this->myEnd.setX(mouseEvent->x());
        this->myEnd.setY(mouseEvent->y());
        emit puts("end point" + QString::number(myEnd.x()) + " " + QString::number(myEnd.y()));
        tryConnect();
    }
    this->repaint();
}

void MainWindow::tryConnect(){
    emit puts(QString::number(myStart.x()) + " " + QString::number(myStart.y())+"-->" + QString::number(myEnd.x())+" "+ QString::number(myEnd.y()));

    int number_start = 0;
    int number_end = 0;

    int locType_Start = -1;
    int locIndex_Start = -1;

    int locType_End = -1;
    int locIndex_End = -1;

    for(int i = 0; i < parts.size(); i++){
        for(int j = 0; j < parts[i].size(); j++){
            TContainer *CurrPart = parts[i][j];
            if(CurrPart->geometry().contains(myStart.x(),myStart.y())){
                locType_Start = CurrPart->getType();
                locIndex_Start = CurrPart->getIndex();
                number_start++;
            }
        }
    }
    for(int i = 0; i < parts.size(); i++){
        for(int j = 0; j < parts[i].size(); j++){
            TContainer *CurrPart = parts[i][j];
            if(CurrPart->geometry().contains(myEnd.x(),myEnd.y())){
                locType_End = CurrPart->getType();
                locIndex_End = CurrPart->getIndex();
                number_end++;
            }
        }
    }



    //    *********************** ДОПИСАТЬ ВСЕ ВАРИАНТЫ ОШИБОК ***************
    if(number_start > 1){
        emit puts("Error : too many items in the begin of connection");
        return;
    }
    if(number_start == 0){
        emit puts("Error : Count of items in the begin of connection equal 0");
        return;
    }
    if(number_end > 1){
        emit puts("Error : too many items in the end of connection");
        return;
    }
    if(number_end == 0){
        emit puts("Error : Count of items in the end of connection equal 0");
        return;
    }

    if (locType_Start == -1     ||
        locIndex_Start == -1    ||
        locType_End == -1       ||
        locIndex_End == -1
    ){
        emit puts("Error : some index is equal -1!!");
        return;
    }


    if (locType_Start == locType_End    &&
        locIndex_Start == locIndex_End
    ){
        emit puts("Error : same object");
        return;
    }
    //    *********************** ДОПИСАТЬ ВСЕ ВАРИАНТЫ ОШИБОК ***************

    TContainer *SignalPart = parts[locType_Start][locIndex_Start];
    emit puts("START item - " + SignalPart->WhatIs(locType_Start)+" with index " + QString::number(locIndex_Start+1));

    TContainer *SlotPart = parts[locType_End][locIndex_End];
    emit puts("END item - " + SlotPart->WhatIs(locType_End)+" with index " + QString::number(locIndex_End+1));


    Arrow *anArrow = new Arrow();
    anArrow->SetStart(locType_Start,locIndex_Start);
    anArrow->SetEnd(locType_End,locIndex_End);
    switch (SignalPart->getType()){
    case RES:
        return;
        break;
    default:
        break;
    }

    switch (SlotPart->getType())
    {
    case BPSK_D:
        connect(SignalPart,SIGNAL(output(vector<double>)),SlotPart,SLOT(input(vector<double>)));
        arrows.push_back(anArrow);
        this->repaint();
        break;
    case GEN:
        puts("Error : could not set generator as END item");
        return;
        break;
    case RES:
        if(SignalPart->getType() == NOISE){
            connect(SignalPart,SIGNAL(send_noise(double)),SlotPart,SLOT(get_noise(double)));
            arrows.push_back(anArrow);
            this->repaint();
        }
        if(SignalPart->getType() == GEN){
            connect(SignalPart,SIGNAL(send_generated(myVector)),SlotPart,SLOT(get_generated(myVector)));
            arrows.push_back(anArrow);
            this->repaint();
        }
        if(SignalPart->getType() == GOLAY_D ||
           SignalPart->getType() == CONV_D  ){
            connect(SignalPart,SIGNAL(send_decoded(myVector,int)),SlotPart,SLOT(get_decoded(myVector,int)));
            arrows.push_back(anArrow);
            this->repaint();
        }
        break;
    default:
        connect(SignalPart,SIGNAL(output(myVector)),SlotPart,SLOT(input(myVector)));
        arrows.push_back(anArrow);
        this->repaint();
        break;
    }

    emit puts(QString::number(number_start) + " " + QString::number(number_end));
    connect_list.addItem(SignalPart->WhatIs(locType_Start) + " " +QString::number(SignalPart->getIndex() + 1) + " --> " + SlotPart->WhatIs(locType_End) + " " + QString::number(SlotPart->getIndex() + 1));
}

