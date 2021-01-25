#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <QWidget>
#include "QGraphicsEffect"

namespace Ui {
class imagebutton;
}

class imagebutton : public QWidget
{
    Q_OBJECT

public:
    explicit imagebutton(QWidget *parent = nullptr);
    ~imagebutton();
signals:
    void clicked();
public slots:
    void seticon(QString url);
    void settext(QString text);
    QString gettext();
    void setopacity(float op);
    void setopbase(qreal base);
protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event) ;


private:
    Ui::imagebutton *ui;
    QGraphicsOpacityEffect *opacityeffect;
    qreal opoffset=0.2;
    qreal opbase=0.5;
};

#endif // IMAGEBUTTON_H
