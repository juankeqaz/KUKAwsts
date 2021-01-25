#include "mybutton.h"
#include <QDebug>

MyButton::MyButton(int flag, QWidget *parent):QWidget(parent)
{
    posflag=flag;
    paintcolor=QColor("#d0d0d0");
    normalcolor=QColor("#d9d9d9");
    presscolor=QColor("#eeeeee");
    hovercolor=QColor("#c0c0c0");
    ispressed=false;
}

MyButton::~MyButton()
{

}

void MyButton::setcheck(bool ischeck)
{
//    if(ischecked==ischeck) return;
//    ischecked=ischeck;
    if(ischeck==ispressed)
        return;
    else
    {
        ispressed=ischeck;
        currentcolor=normalcolor;
        paintcolor=normalcolor;
        repaint();
    }


}

void MyButton::setpressstatus(bool ispress)
{
    if(ispressed==ispress) return;
    ispressed=ispress;
    repaint();
}

void MyButton::paintEvent(QPaintEvent *)
{
    edge.clear();
    w=this->width();
    h=this->height();
    hh=h/2;
    //qDebug()<<w<<h<<hh;
    if(posflag==1)
    {
        edge.push_back(QPoint(0,0));
        edge.push_back(QPoint(0,h));
        edge.push_back(QPoint(w,h));
        edge.push_back(QPoint(w+hh,hh));
        edge.push_back(QPoint(w,0));
        edge.push_back(QPoint(0,0));
    }else if(posflag==2)
    {
        edge.push_back(QPoint(0,0));
        edge.push_back(QPoint(hh,hh));
        edge.push_back(QPoint(0,h));
        edge.push_back(QPoint(w,h));
        edge.push_back(QPoint(w+hh,hh));
        edge.push_back(QPoint(w,0));
        edge.push_back(QPoint(0,0));
    }else if(posflag==3)
    {
        edge.push_back(QPoint(0,0));
        edge.push_back(QPoint(hh,hh));
        edge.push_back(QPoint(0,h));
        edge.push_back(QPoint(w,h));
        edge.push_back(QPoint(w,0));
        edge.push_back(QPoint(0,0));
    }
    for(int i=0;i<edge.size();++i)
    {
        path.lineTo(edge.at(i));
    }
    pl=QPolygon(edge);
    path.addPolygon(pl);
    QPainter *painter=new QPainter(this);

    QPen *pen=new QPen;
    pen->setWidth(2);
    pen->setColor(QColor("#0f0f0f"));
    painter->setPen(*pen);
    painter->setBrush(paintcolor);
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->drawConvexPolygon(pl.data(),edge.size());
    painter->end();
}

void MyButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        paintcolor=presscolor;

        repaint();
        event->accept();
    }else {
        return;
    }

}

void MyButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        ispressed=true;
        currentcolor=paintcolor;
        emit btnclicked();
    }

}

void MyButton::mouseMoveEvent(QMouseEvent *event)
{
    if(event->button()&Qt::LeftButton)
    {

        event->accept();
    }
}

void MyButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    currentcolor=paintcolor;
    paintcolor=hovercolor;
    repaint();
}

void MyButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);

    paintcolor=currentcolor;
    repaint();

}

void MyButton::resizeEvent(QResizeEvent *event)
{
    repaint();
    QRegion maskedRegion(pl);
    setMask(maskedRegion);
    event->accept();

}

bool MyButton::isinpicture(QPointF point)
{
    return  path.contains(point);
}
