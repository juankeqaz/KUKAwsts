#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include <QPaintEvent>
#include <QColor>
#include <QPoint>
#include <QPainter>
#include <QPen>
#include <QCheckBox>
#include <QMouseEvent>
#include <QPolygonF>
#include <QPicture>
#include <QBitmap>

class MyButton:public QWidget
{
    Q_OBJECT
public:
    explicit MyButton(int flag,QWidget *parent=0);
    ~MyButton();
signals:
    void btnclicked();
public slots:
    void setcheck(bool ischeck);
    void setpressstatus(bool ispress);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    bool isinpicture(QPointF point);


private:
    QColor normalcolor;
    QColor hovercolor;
    QColor presscolor;
    QColor paintcolor;
    QColor currentcolor;
    bool ischecked;
    bool ispressed;
    int posflag;
    QVector<QPoint> edge;
    QCheckBox *checkbox;
    QPixmap picture;
    QPainterPath path;
    QPolygon pl;

    int w;
    int h;
    int hh;

    QBitmap m_pixmap;



};

#endif // MYBUTTON_H
