#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPoint>
#include <QListWidget>
#include <QGroupBox>
#include <QPainter>
#include <QFile>
#include <QTabWidget>

#include "tcontainer.h"
#include "arrow.h"
#include "graph.h"
#include "sett.h"

//enum types{
//    BPSK_M =  0,
//    BPSK_D =  1,
//    CONV_C =  2,
//    CONV_D =  3,
//    GEN =     4,
//    GOLAY_C = 5,
//    GOLAY_D = 6,
//    NOISE =   7,
//    RES = 8
//};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setMode(int);
    int getMode();

    void tryConnect();
    void DrawArrows(QPainter *p);



protected:
    void mouseReleaseEvent(QMouseEvent *mouseEvent);
    void mouseMoveEvent(QMouseEvent *mouseEvent);
    void mousePressEvent(QMouseEvent *mouseEvent);
    void paintEvent(QPaintEvent *event);

signals:
    void Start();
    void RedrawGraph(vector<double> TypeSnr,vector<int> nbError,myVector aGenerated);
    void SaveNoiseValue(double);

public slots:
    void StartRepeater(double,double,double,int);
    void Repeater();
    void AddGraphResult(vector<double> TypeSnr,vector<int> nbError,myVector aGenerated);

    void showGraph();
    void show();
    void save();
    void getStart();

    void posChanged();

    void myClose();

    void check();

    void add_part();

    void add_bpsk_m();
    void add_bpsk_d();
    void add_conv_c();
    void add_conv_d();
    void add_gen();
    void add_golay_c();
    void add_golay_d();
    void add_noise();
    void add_res();

    void del_item(int anIndex, int aType);
    void del_arrow(int anIndex, int aType);
    void puts(QString astr);
    void eraseAll();



    
private:
    Sett *myRepeater;
    Ui::MainWindow *ui;
    bool rigth_button;

    int mode;

    Graph *myGraph;
    QList<QList <TContainer*>> parts;

    QList<TContainer*> bpsk_modulators;
    QList<TContainer*> bpsk_demodulators;
    QList<TContainer*> conv_coders;
    QList<TContainer*> conv_decoder;
    QList<TContainer*> generators;
    QList<TContainer*> golay_coders;
    QList<TContainer*> golay_decoders;
    QList<TContainer*> noises;
    QList<TContainer*> receivers;

    QList<Arrow*> arrows;

    QListWidget check_list;
    QListWidget output_list;
    QListWidget connect_list;
    QListWidget arrows_list;

    QTabWidget *logs;

    QPoint myStart;
    QPoint myEnd;
    QPoint myCurr;
    QPoint myPrev;

};

#endif // MAINWINDOW_H
