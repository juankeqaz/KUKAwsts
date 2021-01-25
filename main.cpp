#include "quiwidget.h"
#include "mainwindow.h"
#include <QApplication>
#include "startwidget.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft Yahei",9));
    a.setWindowIcon(QIcon(":/main.ico"));
    //QUIWidget::setCode();
    //QUIWidget::setTranslator(":/image/qt_zh_CN.qm");
    //QUIWidget::setTranslator(":/image/widgets.qm");

    QUIWidget w;
    w.setTitle("基于库卡机械臂的无损探伤系统 V2.0 （合肥中科艾帝尔机器人有限公司)");
    w.setAlignment(Qt::AlignCenter);
    w.setSizeGripEnabled(true);
    w.setVisible(QUIWidget::BtnMenu,true);
    w.setStyle(QUIWidget::Style_Silvery);
    //startwidget startwidget;
    MainWindow startwidget;
    w.setMainWidget(&startwidget);
    w.show();
    return a.exec();
}
