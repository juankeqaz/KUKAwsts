#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QToolButton>
#include <QAction>
#include <imagebutton.h>
#include <QLabel>
#include <QPushButton>
#include <runpage.h>
#include "ipinputbox.h"
#include "dataanysis.h"
#include "workpiece.h"
#include "scannersetting.h"
#include "detecsting.h"

namespace Ui {
class startwidget;
}

class startwidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit startwidget(QWidget *parent = nullptr);
    ~startwidget();

signals:
    void cardclicked();
public slots:
    void switchcards();
protected:
    void resizeEvent(QResizeEvent *event);
    void inittoolbtn();

private slots:
    void onpushButtonclicked();

private:
    void resizecards();
    void risecard(QString names);
    void showbynames(int index);


private:
    Ui::startwidget *ui;
    //QToolButton *tbtn1;
    imagebutton *tbtn1;
    imagebutton *tbtn2;
    imagebutton *tbtn3;
    imagebutton *tbtn4;
    imagebutton *tbtn5;

    QList<QPair<QString,QString> > btns;
    QLabel *mlbg;
    QList<imagebutton*> toolbuttonlist;
    QList<QPushButton*> buttonlist;
    Runpage *runpage;
    OccView *myview;
    IPInputbox *ipinput;
    DataAnysis *anysis;
    WorkPiece *workpiece;
    scannersetting *scanner;
    DetecSting *Detec;


};

#endif // STARTWIDGET_H
