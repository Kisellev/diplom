#include "tcontainer.h"

TContainer::TContainer(QWidget *parent, QPoint p, QWidget *cWidget) : QWidget(parent) {
    myParent = parent;

    left_button = false;
    mode = IDLE;
    childWidget = cWidget;
    setAttribute(Qt::WA_DeleteOnClose);
    this->setVisible(true);
    this->setAutoFillBackground(false);
    this->setMouseTracking(true);
    this->setFocusPolicy(Qt::ClickFocus);
    this->setFocus();
    this->move(p);
    vLayout = new QVBoxLayout(this);
    if (cWidget != 0) {
        cWidget->setParent(this);
        cWidget->releaseMouse();
        cWidget->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        vLayout->addWidget(cWidget);
        vLayout->setContentsMargins(0,0,0,0);
    }
    this->setLayout(vLayout);
    this->installEventFilter(parent);

}

void TContainer::SaveConvValue(int loc_n2, int loc_m2, int loc_lenght,QStringList new_g2){
    aConvolutional.set_n2(loc_n2);
    aConvolutional.set_m2(loc_m2);
    aConvolutional.set_TRUNC_LENGTH(loc_lenght);
    aConvolutional.set_g2(new_g2);
    this->updateInfo();
}
void TContainer::SaveGeneratorValue(int aValue){
    aGenerator.SetNbGenerated(aValue);
    this->updateInfo();
}
void TContainer::SaveNoiseValue(double aSnr){
    aNoise.set_snr(aSnr);
    this->updateInfo();
}

void TContainer::SaveNoiseValue(double aRate, double aSnr){
    aNoise.set_rate(aRate);
    aNoise.set_snr(aSnr);
    this->updateInfo();
}

void TContainer::setView(){
    settings = new Sett();
    connect(settings,SIGNAL(SaveGeneratorValue(int)),this,SLOT(SaveGeneratorValue(int)));
    connect(settings,SIGNAL(SaveNoiseValue(double,double)),this,SLOT(SaveNoiseValue(double,double)));
    connect(settings,SIGNAL(SaveConvValue(int,int,int,QStringList)),this,SLOT(SaveConvValue(int,int,int,QStringList)));

    myInfo = new QListWidget();

    if(this->getType() != BPSK_M &&
       this->getType() != BPSK_D &&
       this->getType() != GOLAY_C &&
       this->getType() != GOLAY_D &&
       this->getType() != RES      ){
        QLabel *alabel = new QLabel(this->WhatIs(this->getType())+" " + QString::number(this->getIndex()+1));
        this->addChildWidget(alabel);
        this->addChildWidget(myInfo);
    } else {
        QGroupBox *abox = new QGroupBox(this->WhatIs(this->getType())+" " + QString::number(this->getIndex()+1));
        this->addChildWidget(abox);
    }

    this->updateInfo();
}

//---------------------------------------------------
void TContainer::get_noise(double aSnr){
    if(aReceiver.set_noise(aSnr)){
        vector<int> nbError = aReceiver.GetResultnbError();
        vector<double> TypeSnr = aReceiver.GetResultTypeSnr();
        myVector aGenerated = aReceiver.GetGenerated();
        emit SendGraphResult(TypeSnr,nbError,aGenerated);
    }
}

void TContainer::get_generated(myVector aGen){
    if(aReceiver.AddGenerated(aGen)){
        vector<int> nbError = aReceiver.GetResultnbError();
        vector<double> TypeSnr = aReceiver.GetResultTypeSnr();
        myVector aGenerated = aReceiver.GetGenerated();
        emit SendGraphResult(TypeSnr,nbError,aGenerated);
    }
}

void TContainer::get_decoded(myVector aDec, int aType){
    if(aReceiver.AddDecoded(aDec,aType)){
        vector<int> nbError = aReceiver.GetResultnbError();
        vector<double> TypeSnr = aReceiver.GetResultTypeSnr();
        myVector aGenerated = aReceiver.GetGenerated();
        emit SendGraphResult(TypeSnr,nbError,aGenerated);
    }
}
//---------------------------------------------------
void TContainer::updateInfo()
{
    myInfo->clear();
    QStringList temp;
    switch (this->getType())
    {
    case CONV_C:
        temp = aConvolutional.getInfo();
        break;
    case CONV_D:
        temp = aConvolutional.getInfo();;
        break;
    case GEN:
        temp = aGenerator.getInfo();
        break;
    case NOISE:
        temp = aNoise.getInfo();
        break;
    default:
        this->resize(170,40);
        return;
        break;
    }
    myInfo->addItems(temp);
    this->resize(200,temp.size()*15+30);
}
// такая же функция есть в graph.cpp
QString TContainer::WhatIs(int aType){
    QString result;
    switch (aType)
    {
    case BPSK_M:
        result = "bpsk_modulator";
        break;
    case BPSK_D:
        result = "bpsk_demodulator";
        break;
    case CONV_C:
        result = "conv_coder";
        break;
    case CONV_D:
        result = "conv_decoder";
        break;
    case GEN:
        result = "generator";
        break;
    case GOLAY_C:
        result = "golay_coder";
        break;
    case GOLAY_D:
        result = "golay_decoder";
        break;
    case NOISE:
        result = "noise";
        break;
    case RES:
        result = "receiver";
        break;
    default:
        break;
    }
    return result;
}

void TContainer::Start(){
    myVector result = aGenerator.generate();
    emit puts("Generator " + QString::number(this->getIndex()+1) + ":");
    emit puts(result.toString());
    emit send_generated(result);
    emit output(result);
}

void TContainer::setType(int aType){
    this->type = aType;
}

int TContainer::getType(){
    return this->type;
}

void TContainer::setIndex(int anIndex){
    this->index = anIndex;
}

int TContainer::getIndex(){
    return this->index;
}

TContainer::~TContainer() {
    delete vLayout;
}

void TContainer::addChildWidget(QWidget *cWidget) {
    if (cWidget != 0) {
        childWidget = cWidget;
        childWidget->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        childWidget->setParent(this);
        vLayout->addWidget(cWidget);
        vLayout->setContentsMargins(0,0,0,0);
    }
}

bool TContainer::eventFilter( QObject *obj, QEvent *evt ) {
    return QWidget::eventFilter(obj, evt);
}

void TContainer::mousePressEvent(QMouseEvent *e) {
    QWidget::mousePressEvent(e);
    if(e->button() != Qt::LeftButton){
        this->left_button = false;
    } else {
        this->left_button = true;
    }
    position = QPoint(e->globalX()-geometry().x(), e->globalY()-geometry().y());

    if(e->button() == Qt::MiddleButton){
        this->ChangeSett();
    }
}


void TContainer::ChangeSett() {
    switch (this->getType())
    {
    case CONV_C:
        settings->prepare(aConvolutional,this->getType(),this->getIndex());
        break;
    case CONV_D:
        settings->prepare(aConvolutional,this->getType(),this->getIndex());
        break;
    case GEN:
        settings->prepare(aGenerator,this->getIndex());
        break;
    case NOISE:
        settings->prepare(aNoise,this->getIndex());
        break;
    default:
        QMessageBox *msgbox = new QMessageBox();
        msgbox->setWindowTitle("Warning");
        msgbox->setText("Warning : there are no settings for selected item");
        msgbox->show();
        break;
    }
    this->updateInfo();
}

void TContainer::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Delete) {
        this->deleteLater();
        emit del_item(this->getIndex(),this->getType());
        settings->deleteLater();
    }
}

void TContainer::mouseReleaseEvent(QMouseEvent *e) {
    QWidget::mouseReleaseEvent(e);
}

void TContainer::mouseMoveEvent(QMouseEvent *e) {
    QWidget::mouseMoveEvent(e);
    if ((mode == MOVE || mode == IDLE) && e->buttons() && Qt::LeftButton && this->left_button) {
        QPoint toMove = e->globalPos() - position;
        if (toMove.x() < 0) return;
        if (toMove.y() < 15) return;
        if (toMove.x() > this->parentWidget()->width()-this->width()) return;
        if (toMove.y() > this->parentWidget()->height()-this->height()) return;
        move(toMove);
        emit newGeometry(this->geometry());
        this->parentWidget()->repaint();
        emit posChanged();
        return;
    }
    emit newGeometry(this->geometry());
}

void TContainer::input(vector<double> aVector){
    myVector res;
    switch (this->type)
    {
    case BPSK_D:
        res = abpsk.demodulate(aVector);
        emit puts("Result of bpsk demodulation:");
        emit puts(res.toString());
        emit output(res);
        break;
    default:
        break;
    }
}

void TContainer::input(myVector aVector){
    myVector res;
    vector<double> dres;
    QString dres_str;
    switch (this->type)
    {
    case BPSK_M:
        res = abpsk.modulate(aVector);
        emit puts("Result of bpsk modulation:");
        emit puts(res.toString());
        emit output(res);
        break;
    case BPSK_D:
        break;
    case CONV_C:
        res = aConvolutional.code(aVector);
        emit puts("Result of convolutional code:");
        emit puts(res.toString());
        emit output(res);
        break;
    case CONV_D:
        res = aConvolutional.decode(aVector);
        emit puts("Result of convolutional decode:");
        emit puts(res.toString());
        emit output(res);
        emit send_decoded(res,this->type);
        break;
    case GEN:
        break;
    case GOLAY_C:
        res = aGolay.code(aVector);
        emit puts("Result of golay code:");
        emit puts(res.toString());
        emit output(res);
        break;
    case GOLAY_D:
        res = aGolay.decode(aVector);
        emit puts("Result of golay decode:");
        emit puts(res.toString());
        emit output(res);
        emit send_decoded(res,this->type);
        break;
    case NOISE:
        dres = aNoise.generate(aVector);
        for(int i = 0; i < dres.size(); i++){
            dres_str += QString::number(dres.at(i));
        }
        emit puts("Result of noise generation:");
        emit puts(dres_str);
        emit output(dres);
        emit send_noise(aNoise.get_snr());
        break;
    case RES:
        if(aReceiver.isReady()){
            QLabel *asd = new QLabel("ready!");
            asd->show();
        }
        break;
    default:
        break;
    }
}
