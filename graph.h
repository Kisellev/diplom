#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QLayoutItem>
#include <QTextEdit>
#include <QTextBlock>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextCursor>

#include "myvector.h"

class Graph : public QWidget
{
    Q_OBJECT
public:
    Graph(QWidget *parent = 0);
    QString WhatIs(int aType);
    void prepare();

public slots:
    void RedrawPoints(int proc);
    void RedrawGraph(vector<double> TypeSnr,vector<int> nbError,myVector aGenerated);
private:
    QVBoxLayout *myLayout;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QTextEdit *text;
    QPen *aPen;
    QLabel *Legend;
    QSpinBox *spin_proc;

    vector<int> Values_Golay;
    vector<int> NumberOfEachSnr_Golay;

    vector<int> Values_Conv;
    vector<int> NumberOfEachSnr_Conv;
};

#endif
