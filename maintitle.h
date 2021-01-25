#ifndef MAINTITLE_H
#define MAINTITLE_H
#include <QWidget>
#include <QPainter>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPaintEvent>
#include <QColor>
#include <QBrush>
#include <QPoint>
#include <QPainter>
#include <mybutton.h>
#include <QPaintEvent>
#include <QLabel>
#include <QTextCodec>
#include "quiwidget.h"


class MainTitle:public QWidget
{
    Q_OBJECT
public:
    MainTitle(QList<QString> list,QWidget *parent=0);
    ~MainTitle();


signals:
    void clicked(int flag);
private:
    void initforms();
public slots:
    void btisclicked();
    void checkstatus(int flag);
    void uncheckstatus(int flag);
protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent*);
private:
    QList<QString> titletexts;
    QHBoxLayout *layout;
    QToolButton *button;
    MyButton *buttonhome;
    MyButton *buttonconnect;
    MyButton *buttonwpice;
    MyButton *buttoncsset;
    MyButton *buttontest;
    MyButton *buttonrun;
    MyButton *buttonanysis;
    QVector<MyButton*> bt;
    QVector<QLabel*> bt_check;
    QVector<QLabel*> bt_text;

    int w,h,hh;
    QLabel *homeicon;



};

#endif // MAINTITLE_H
