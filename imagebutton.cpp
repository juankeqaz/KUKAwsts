#include "imagebutton.h"
#include "ui_imagebutton.h"


imagebutton::imagebutton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imagebutton)
{
    ui->setupUi(this);
    opacityeffect=new QGraphicsOpacityEffect;
    this->setGraphicsEffect(opacityeffect);
    opacityeffect->setOpacity(opbase);


}

imagebutton::~imagebutton()
{
    delete ui;
}

void imagebutton::seticon(QString url)
{
    QPixmap *map = new QPixmap(url);
    map->scaled(ui->label_icon->width(),ui->label_icon->height(),Qt::KeepAspectRatio);//

    ui->label_icon->setPixmap(*map);
    ui->label_icon->setAlignment(Qt::AlignCenter);
}

void imagebutton::settext(QString text)
{

    ui->label_text->setText(text);
    ui->label_text->setWordWrap(true);
    ui->label_text->setAlignment(Qt::AlignCenter);
}

QString imagebutton::gettext()
{
    return  ui->label_text->text();
}

void imagebutton::setopacity(float op)
{
    if(op>1)
        opacityeffect->setOpacity(1.0);
    else if(op<0)
        opacityeffect->setOpacity(0.0);
    else
        opacityeffect->setOpacity(op);
}

void imagebutton::setopbase(qreal base)
{
    opbase=base;
}

void imagebutton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);

    setopacity(opbase+opoffset);
}

void imagebutton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    setopacity(opbase);
}

void imagebutton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    setopacity(opacityeffect->opacity()+opoffset);
}

void imagebutton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    setopacity(opbase+opoffset);
    emit clicked();
}
